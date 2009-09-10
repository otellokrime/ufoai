/**
 * @file g_ai.c
 * @brief Artificial Intelligence.
 */

/*
Copyright (C) 2002-2009 UFO: Alien Invasion.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "g_local.h"
#include "g_ai.h"
#include "lua/lauxlib.h"

#define POS3_METATABLE	"pos3" /**< Pos3 Lua Metatable name. */
#define ACTOR_METATABLE	"actor" /**< Actor Lua Metable name. */

/**
 * Provides an api like luaL_dostring for buffers.
 */
#define luaL_dobuffer(L, b, n, s) \
	(luaL_loadbuffer(L, b, n, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define AIL_invalidparameter(n)	\
	gi.dprintf("AIL: Invalid parameter #%d in '%s'.\n", n, __func__)

/**
 * @brief Wrapper around edict.
 */
typedef struct aiActor_s {
	edict_t *ent; /**< Actual actor. */
} aiActor_t;


/*
 * Current AI Actor.
 */
static edict_t *AIL_ent; /**< Actor currently running the Lua AI. */
static player_t *AIL_player; /**< Player currently running the Lua AI. */


/*
 * Actor metatable.
 */
/* Internal functions. */
static int actorL_register(lua_State *L);
static int lua_isactor(lua_State *L, int index);
static aiActor_t* lua_toactor(lua_State *L, int index);
static aiActor_t* lua_pushactor(lua_State *L, aiActor_t *actor);
/* Metatable functions. */
static int actorL_tostring(lua_State *L);
static int actorL_pos(lua_State *L);
static int actorL_shoot(lua_State *L);
static int actorL_face(lua_State *L);
static int actorL_team(lua_State *L);
/** Lua Actor metatable methods. */
static const luaL_reg actorL_methods[] = {
	{"__tostring", actorL_tostring},
	{"pos", actorL_pos},
	{"shoot", actorL_shoot},
	{"face", actorL_face},
	{"team", actorL_team},
	{NULL, NULL}
};


/**
 * pos3 metatable.
 */
/* Internal functions. */
static int pos3L_register(lua_State *L);
static int lua_ispos3(lua_State *L, int index);
static pos3_t* lua_topos3(lua_State *L, int index);
static pos3_t* lua_pushpos3(lua_State *L, pos3_t *pos);
/* Metatable functions. */
static int pos3L_tostring(lua_State *L);
static int pos3L_goto(lua_State *L);
static int pos3L_face(lua_State *L);
/** Lua Pos3 metatable methods. */
static const luaL_reg pos3L_methods[] = {
	{"__tostring", pos3L_tostring},
	{"goto", pos3L_goto},
	{"face", pos3L_face},
	{NULL, NULL}
};


/**
 * General AI bindings.
 */
static int AIL_print(lua_State *L);
static int AIL_see(lua_State *L);
static int AIL_crouch(lua_State *L);
static int AIL_TU(lua_State *L);
static int AIL_reactionfire(lua_State *L);
static int AIL_roundsleft(lua_State *L);
static int AIL_canreload(lua_State *L);
static int AIL_reload(lua_State *L);
static int AIL_positionshoot(lua_State *L);
static int AIL_positionhide(lua_State *L);
/** Lua AI module methods. */
static const luaL_reg AIL_methods[] = {
	{"print", AIL_print},
	{"see", AIL_see},
	{"crouch", AIL_crouch},
	{"TU", AIL_TU},
	{"reactionfire", AIL_reactionfire},
	{"roundsleft", AIL_roundsleft},
	{"canreload", AIL_canreload},
	{"reload", AIL_reload},
	{"positionshoot", AIL_positionshoot},
	{"positionhide", AIL_positionhide},
	{NULL, NULL}
};


/**
 *    A C T O R L
 */

/**
 * @brief Registers the actor metatable in the lua_State.
 * @param[in,out] L State to register the metatable in.
 * @return 0 on success.
 */
static int actorL_register (lua_State *L)
{
	/* Create the metatable */
	luaL_newmetatable(L, ACTOR_METATABLE);

	/* Create the access table */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	/* Register the values */
	luaL_register(L, NULL, actorL_methods);

	/* Clean up stack. */
	lua_pop(L, 1);

	return 0; /* No error */
}

/**
 * @brief Checks to see if there is a actor metatable at index in the lua_State.
 * @param[in,out] L Lua state to check.
 * @param[in] index Index to check for a actor metatable.
 * @return 1 if index has a actor metatable otherwise returns 0.
 */
static int lua_isactor (lua_State *L, int index)
{
	int ret;

	if (lua_getmetatable(L, index) == 0)
		return 0;
	lua_getfield(L, LUA_REGISTRYINDEX, ACTOR_METATABLE);

	ret = 0;
	if (lua_rawequal(L, -1, -2))  /* does it have the correct metatable? */
		ret = 1;

	lua_pop(L, 2);  /* remove both metatables */
	return ret;
}

/**
 * @brief Returns the actor from the metatable at index.
 */
static aiActor_t* lua_toactor (lua_State *L, int index)
{
	if (lua_isactor(L,index)) {
		return (aiActor_t*) lua_touserdata(L, index);
	}
	luaL_typerror(L, index, ACTOR_METATABLE);
	return NULL;
}

/**
 * @brief Pushes a actor as a metatable at the top of the stack.
 */
static aiActor_t* lua_pushactor (lua_State *L, aiActor_t *actor)
{
	aiActor_t *a;
	a = (aiActor_t*) lua_newuserdata(L, sizeof(*a));
	*a = *actor;
	luaL_getmetatable(L, ACTOR_METATABLE);
	lua_setmetatable(L, -2);
	return a;
}

/**
 * @brief Pushes the actor as a string.
 */
static int actorL_tostring (lua_State *L)
{
	aiActor_t *target;
	char buf[MAX_VAR];

	assert(lua_isactor(L, 1));

	target = lua_toactor(L, 1);
	Com_sprintf(buf, sizeof(buf), "Actor( %s )", target->ent->chr.name);

	lua_pushstring(L, buf);
	return 1;
}

/**
 * @brief Gets the actors position.
 */
static int actorL_pos (lua_State *L)
{
	aiActor_t *target;

	assert(lua_isactor(L, 1));

	target = lua_toactor(L, 1);
	lua_pushpos3(L, &target->ent->pos);
	return 1;
}

/**
 * @brief Shoots the actor.
 */
static int actorL_shoot (lua_State *L)
{
	int fm, tu, shots;
	aiActor_t *target;
	const item_t *item;
	const fireDef_t *fdArray;
	objDef_t *od;

	assert(lua_isactor(L, 1));

	/* Target */
	target = lua_toactor(L, 1);

	/* Number of TU to spend shooting, fire mode will adjust to that. */
	if (lua_gettop(L) > 1) {
		assert(lua_isnumber(L, 2)); /* Must be a number. */

		tu = (int) lua_tonumber(L, 2);
	} else {
		tu = AIL_ent->TU;
	}

	/** @todo figure out this shot mode stuff out. */
	fm = 0;

	/* Figure out weapon to use. */
	if (IS_SHOT_RIGHT(fm) && RIGHT(AIL_ent)
			&& RIGHT(AIL_ent)->item.m
			&& RIGHT(AIL_ent)->item.t->weapon
			&& (!RIGHT(AIL_ent)->item.t->reload
				|| RIGHT(AIL_ent)->item.a > 0)) {
		item = &RIGHT(AIL_ent)->item;
	} else if (IS_SHOT_LEFT(fm) && LEFT(AIL_ent)
			&& LEFT(AIL_ent)->item.m
			&& LEFT(AIL_ent)->item.t->weapon
			&& (!LEFT(AIL_ent)->item.t->reload
				|| LEFT(AIL_ent)->item.a > 0)) {
		item = &LEFT(AIL_ent)->item;
	} else {
		/* Failure - no weapon. */
		lua_pushboolean(L, 0);
		return 1;
	}

	/** @todo Choose fire mode based on TU available - currently the first one is used. */
	fdArray = FIRESH_FiredefForWeapon(item);
	if (fdArray == NULL) {
		/* Failure - no weapon. */
		lua_pushboolean(L, 0);
		return 1;
	}

	if (!item->m)
		od = item->t;
	else
		od = item->m;

	shots = tu / fdArray->time;

	while (shots > 0) {
		shots--;
		/** @todo actually handle fire modes */
		G_ClientShoot(AIL_player, AIL_ent->number, target->ent->pos,
				0, 0, NULL, qtrue, 0);
	}

	/* Success. */
	lua_pushboolean(L, 1);
	return 1;
}

/**
 * @brief Makes the actor face the position.
 */
static int actorL_face (lua_State *L)
{
	aiActor_t *target;

	assert(lua_isactor(L, 1));

	/* Target */
	target = lua_toactor(L, 1);

	AI_TurnIntoDirection(AIL_ent, target->ent->pos);

	/* Success. */
	lua_pushboolean(L, 1);
	return 1;
}

/**
 * @brief Gets the actor's team.
 */
static int actorL_team (lua_State *L)
{
	const aiActor_t *target;

	assert(lua_isactor(L, 1));

	target = lua_toactor(L, 1);
	switch (target->ent->team) {
	case TEAM_PHALANX:
		lua_pushstring(L, "phalanx");
		break;
	case TEAM_CIVILIAN:
		lua_pushstring(L, "civilian");
		break;
	case TEAM_ALIEN:
		lua_pushstring(L, "alien");
		break;
	default:
		lua_pushstring(L, "unknown");
		break;
	}
	return 1;
}


/**
 *   P O S 3 L
 */

/**
 * @brief Registers the pos3 metatable in the lua_State.
 * @param[in] L State to register the metatable in.
 * @return 0 on success.
 */
static int pos3L_register (lua_State *L)
{
	/* Create the metatable */
	luaL_newmetatable(L, POS3_METATABLE);

	/* Create the access table */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	/* Register the values */
	luaL_register(L, NULL, pos3L_methods);

	/* Clean up the stack. */
	lua_pop(L, 1);

	return 0; /* No error */
}

/**
 * @brief Checks to see if there is a pos3 metatable at index in the lua_State.
 * @param[in] L Lua state to check.
 * @param[in] index Index to check for a pos3 metatable.
 * @return 1 if index has a pos3 metatable otherwise returns 0.
 */
static int lua_ispos3 (lua_State *L, int index)
{
	int ret;

	if (lua_getmetatable(L, index) == 0)
		return 0;
	lua_getfield(L, LUA_REGISTRYINDEX, POS3_METATABLE);

	ret = 0;
	if (lua_rawequal(L, -1, -2))  /* does it have the correct metatable? */
		ret = 1;

	lua_pop(L, 2);  /* remove both metatables */
	return ret;
}

/**
 * @brief Returns the pos3 from the metatable at index.
 */
static pos3_t* lua_topos3 (lua_State *L, int index)
{
	if (lua_ispos3(L, index)) {
		return (pos3_t*) lua_touserdata(L, index);
	}
	luaL_typerror(L, index, POS3_METATABLE);
	return NULL;
}

/**
 * @brief Pushes a pos3 as a metatable at the top of the stack.
 */
static pos3_t* lua_pushpos3 (lua_State *L, pos3_t *pos)
{
	pos3_t *p;
	p = (pos3_t*) lua_newuserdata(L, sizeof(*p));
	memcpy(p, pos, sizeof(*p));
	luaL_getmetatable(L, POS3_METATABLE);
	lua_setmetatable(L, -2);
	return p;
}

/**
 * @brief Puts the pos3 information in a string.
 */
static int pos3L_tostring (lua_State *L)
{
	pos3_t *p;
	char buf[MAX_VAR];

	assert(lua_ispos3(L, 1));

	p = lua_topos3(L, 1);
	Com_sprintf(buf, sizeof(buf), "Pos3( x=%d, y=%d, z=%d )", (*p)[0], (*p)[1], (*p)[2]);

	lua_pushstring(L, buf);
	return 1;
}

/**
 * @brief Makes the actor head to the position.
 */
static int pos3L_goto (lua_State *L)
{
	pos3_t *pos;
	const byte crouchingState = AIL_ent->state & STATE_CROUCHED ? 1 : 0;

	assert(lua_ispos3(L, 1));

	/* Calculate move table. */
	G_MoveCalc(0, AIL_ent->pos, AIL_ent->fieldSize, crouchingState, AIL_ent->TU);
	gi.MoveStore(gi.pathingMap);

	/* Move. */
	pos = lua_topos3(L, 1);
	G_ClientMove(AIL_player, 0, AIL_ent->number, *pos, qfalse, QUIET);

	lua_pushboolean(L, 1);
	return 1;
}

/**
 * @brief Makes the actor face the position.
 */
static int pos3L_face (lua_State *L)
{
	pos3_t *pos;

	assert(lua_ispos3(L, 1));

	pos = lua_topos3(L, 1);
	AI_TurnIntoDirection(AIL_ent, *pos);

	lua_pushboolean(L, 1);
	return 1;
}

/**
 *    A I L
 */
/**
 * @brief Works more or less like Lua's builtin print.
 */
static int AIL_print (lua_State *L)
{
	int i;
	const int n = lua_gettop(L);  /* number of arguments */

	for (i = 1; i <= n; i++) {
		const char *s;
		qboolean meta = qfalse;

		lua_pushvalue(L, i);   /* value to print */
		if (luaL_callmeta(L, 1, "__tostring")) {
			s = lua_tostring(L, -1);
			meta = qtrue;
		} else {
			switch (lua_type(L, -1)) {
			case LUA_TNUMBER:
			case LUA_TSTRING:
				s = lua_tostring(L, -1);
				break;
			case LUA_TBOOLEAN:
				s = lua_toboolean(L, -1) ? "true" : "false";
				break;
			case LUA_TNIL:
				s = "nil";
				break;

			default:
				s = "unknown lua type";
				break;
			}
		}
		gi.dprintf("%s%s", (i > 1) ? "\t" : "", s);
		lua_pop(L, 1); /* Pop the value */
		if (meta) /* Meta creates an additional string. */
			lua_pop(L, 1);
	}

	gi.dprintf("\n");
	return 0;
}

/**
 * @brief Returns what the actor can see.
 */
static int AIL_see (lua_State *L)
{
	int vision, team;
	int i, j, k, n, cur;
	edict_t *check;
	aiActor_t target;
	edict_t *sorted[MAX_EDICTS], *unsorted[MAX_EDICTS];
	float distLookup[MAX_EDICTS];

	/* Defaults. */
	team = TEAM_NO_ACTIVE;
	vision = 0;

	/* Handle parameters. */
	if ((lua_gettop(L) > 0)) {
		/* Get what to "see" with. */
		if (lua_isstring(L, 1)) {
			const char *s = lua_tostring(L, 1);
			/** @todo Properly implement at edict level, get rid of magic numbers.
			 * These are only "placeholders". */
			if (strcmp(s, "all") == 0)
				vision = 0;
			else if (strcmp(s, "sight") == 0)
				vision = 1;
			else if (strcmp(s, "psionic") == 0)
				vision = 2;
			else if (strcmp(s, "infrared") == 0)
				vision = 3;
			else
				AIL_invalidparameter(1);
		} else
			AIL_invalidparameter(1);

		/* We now check for different teams. */
		if ((lua_gettop(L) > 1)) {
			if (lua_isstring(L, 2)) {
				const char *s = lua_tostring(L, 2);
				if (strcmp(s, "all") == 0)
					team = TEAM_NO_ACTIVE;
				else if (strcmp(s, "alien") == 0)
					team = TEAM_ALIEN;
				else if (strcmp(s, "civilian") == 0)
					team = TEAM_CIVILIAN;
				else if (strcmp(s, "phalanx") == 0)
					team = TEAM_PHALANX;
				else
					AIL_invalidparameter(2);
			} else
				AIL_invalidparameter(2);
		}
	}

	n = 0;
	/* Get visible things. */
	/** @todo check for what they can see instead of seeing all. */
	for (i = 0, check = g_edicts; i < globals.num_edicts; i++, check++)
		if (check->inuse && G_IsLivingActor(check) && AIL_ent != check
		 && vision == 0 /* Vision checks. */
		 && (team == TEAM_NO_ACTIVE || check->team == team)) {/* Check for team match if needed. */
			distLookup[n] = VectorDistSqr(AIL_ent->pos, check->pos);
			unsorted[n++] = check;
		}

	/* Sort by distance - nearest first. */
	for (i = 0; i < n; i++) { /* Until we fill sorted */
		cur = -1;
		for (j = 0; j < n; j++) { /* Check for closest */
			/* Is shorter then current minimum? */
			if (cur < 0 || distLookup[j] < distLookup[cur]) {
				/* Check if not already in sorted. */
				for (k = 0; k < i; k++)
					if (sorted[k] == unsorted[j])
						break;

				/* Not already sorted and is new minimum. */
				if (k == i)
					cur = j;
			}
		}

		sorted[i] = unsorted[cur];
	}

	/* Now save it in a Lua table. */
	lua_newtable(L);
	for (i = 0; i < n; i++) {
		lua_pushnumber(L, i + 1); /* index, starts with 1 */
		target.ent = sorted[i];
		lua_pushactor(L, &target); /* value */
		lua_rawset(L, -3); /* store the value in the table */
	}
	return 1; /* Returns the table of actors. */
}

/**
 * @brief Toggles crouch state with true/false and returns current crouch state.
 */
static int AIL_crouch (lua_State *L)
{
	if (lua_gettop(L) > 0) {
		if (lua_isboolean(L, 1)) {
			const int state = lua_toboolean(L, 1);
			G_ClientStateChange(AIL_player, AIL_ent->number, STATE_CROUCHED,
				(state) ? qtrue : qfalse);
		} else
			AIL_invalidparameter(1);
	}

	lua_pushboolean(L, AIL_ent->state & STATE_CROUCHED);
	return 1;
}

/**
 * @brief Gets the number of TU the actor has left.
 */
static int AIL_TU (lua_State *L)
{
	lua_pushnumber(L, AIL_ent->TU);
	return 1;
}

/**
 * @brief Sets the actor's reaction fire.
 */
static int AIL_reactionfire (lua_State *L)
{
	if (lua_gettop(L) > 0) {
		if (lua_isboolean(L, 1)) {
			const int state = lua_toboolean(L, 1);
			G_ClientStateChange(AIL_player, AIL_ent->number, STATE_REACTION_ONCE,
				(state) ? qtrue : qfalse);
		} else
			AIL_invalidparameter(1);
	}

	lua_pushboolean(L, AIL_ent->state & STATE_REACTION_ONCE);
	return 1;
}

/**
 * @brief Checks to see how many rounds the actor has left.
 */
static int AIL_roundsleft (lua_State *L)
{
	/* Right hand */
	if (RIGHT(AIL_ent) && RIGHT(AIL_ent)->item.t->reload)
		lua_pushnumber(L, RIGHT(AIL_ent)->item.a);
	else
		lua_pushnil(L);

	/* Left hand */
	if (LEFT(AIL_ent) && LEFT(AIL_ent)->item.t->reload)
		lua_pushnumber(L, LEFT(AIL_ent)->item.a);
	else
		lua_pushnil(L);
	return 2;
}

/**
 * @brief Checks to see if the actor can reload.
 */
static int AIL_canreload (lua_State *L)
{
	lua_pushboolean(L, G_ClientCanReload(AIL_player, AIL_ent->number, gi.csi->idRight));
	lua_pushboolean(L, G_ClientCanReload(AIL_player, AIL_ent->number, gi.csi->idLeft));
	return 2;
}

/**
 * @brief Actor reloads his weapons.
 */
static int AIL_reload (lua_State *L)
{
	shoot_types_t weap;

	if (lua_gettop(L) > 0) {
		if (lua_isstring(L, 1)) {
			const char *s = lua_tostring(L, 1);

			if (!strcmp(s, "right"))
				weap = gi.csi->idRight;
			else if (!strcmp(s, "left"))
				weap = gi.csi->idLeft;
			else
				return 0;
		} else {
			AIL_invalidparameter(1);
			return 0;
		}
	} else
		weap = gi.csi->idRight; /* Default to right hand. */

	G_ActorReload(AIL_ent->number, weap, QUIET);
	return 0;
}

/**
 * @brief Moves the actor into a position in which he can shoot his target.
 */
static int AIL_positionshoot (lua_State *L)
{
	pos3_t to, bestPos;
	vec3_t check;
	edict_t *ent;
	int dist;
	int xl, yl, xh, yh;
	int min_tu;
	aiActor_t *target;
	const byte crouchingState = AIL_ent->state & STATE_CROUCHED ? 1 : 0;

	/* We need a target. */
	assert(lua_isactor(L, 1));
	target = lua_toactor(L, 1);

	/* Make things more simple. */
	ent = AIL_ent;
	dist = ent->TU;

	/* Calculate move table. */
	G_MoveCalc(0, ent->pos, ent->fieldSize, crouchingState, ent->TU);
	gi.MoveStore(gi.pathingMap);

	/* set borders */
	xl = (int) ent->pos[0] - dist;
	if (xl < 0)
		xl = 0;
	yl = (int) ent->pos[1] - dist;
	if (yl < 0)
		yl = 0;
	xh = (int) ent->pos[0] + dist;
	if (xh > PATHFINDING_WIDTH)
		xl = PATHFINDING_WIDTH;
	yh = (int) ent->pos[1] + dist;
	if (yh > PATHFINDING_WIDTH)
		yh = PATHFINDING_WIDTH;

	/* evaluate moving to every possible location in the search area,
	 * including combat considerations */
	min_tu = INT_MAX;
	for (to[2] = 0; to[2] < PATHFINDING_HEIGHT; to[2]++)
		for (to[1] = yl; to[1] < yh; to[1]++)
			for (to[0] = xl; to[0] < xh; to[0]++) {
				/* Can we see the target? */
				gi.GridPosToVec(gi.routingMap, ent->fieldSize, to, check);
				if (G_ActorVis(check, target->ent, qtrue) > 0.3) {
					const pos_t tu = gi.MoveLength(gi.pathingMap, to,
							(ent->state & STATE_CROUCHED) ? 1 : 0, qtrue);

					if (tu > ent->TU || tu == ROUTING_NOT_REACHABLE)
						continue;

					/* Better spot (easier to get to). */
					if (tu < min_tu) {
						VectorCopy(to, bestPos);
						min_tu = tu;
					}
				}
			}

	/* No position found in range. */
	if (min_tu > ent->TU) {
		lua_pushboolean(L, 0);
		return 1;
	}

	/* Return the spot. */
	lua_pushpos3(L, &bestPos);
	return 1;
}

/**
 * @brief Moves the actor into a position in which he can hide.
 */
static int AIL_positionhide (lua_State *L)
{
	pos3_t to, bestPos;
	edict_t *ent;
	int dist;
	int xl, yl, xh, yh;
	int min_tu;
	const byte crouchingState = AIL_ent->state & STATE_CROUCHED ? 1 : 0;

	/* Make things more simple. */
	ent = AIL_ent;
	dist = ent->TU;

	/* Calculate move table. */
	G_MoveCalc(0, ent->pos, ent->fieldSize, crouchingState, ent->TU);
	gi.MoveStore(gi.pathingMap);

	/* set borders */
	xl = (int) ent->pos[0] - dist;
	if (xl < 0)
		xl = 0;
	yl = (int) ent->pos[1] - dist;
	if (yl < 0)
		yl = 0;
	xh = (int) ent->pos[0] + dist;
	if (xh > PATHFINDING_WIDTH)
		xl = PATHFINDING_WIDTH;
	yh = (int) ent->pos[1] + dist;
	if (yh > PATHFINDING_WIDTH)
		yh = PATHFINDING_WIDTH;

	/* evaluate moving to every possible location in the search area,
	 * including combat considerations */
	min_tu = INT_MAX;
	for (to[2] = 0; to[2] < PATHFINDING_HEIGHT; to[2]++)
		for (to[1] = yl; to[1] < yh; to[1]++)
			for (to[0] = xl; to[0] < xh; to[0]++) {
				const pos_t tu = gi.MoveLength(gi.pathingMap, to,
						(ent->state & STATE_CROUCHED) ? 1 : 0, qtrue);

				if (tu > ent->TU || tu == ROUTING_NOT_REACHABLE)
					continue;

				/* visibility */
				gi.GridPosToVec(gi.routingMap, ent->fieldSize, ent->pos, ent->origin);
				if (G_TestVis(-ent->team, ent, VT_PERISH | VT_NOFRUSTUM) & VIS_YES)
					continue;

				/* Better spot (easier to get to). */
				if (tu < min_tu) {
					VectorCopy(to, bestPos);
					min_tu = tu;
				}
			}

	/* No position found in range. */
	if (min_tu > ent->TU) {
		lua_pushboolean(L, 0);
		return 1;
	}

	/* Return the spot. */
	lua_pushpos3(L, &bestPos);
	return 1;
}

/**
 * @brief The think function for the ai controlled aliens
 * @param[in] player
 * @param[in] ent
 * @sa AI_FighterCalcBestAction
 * @sa AI_CivilianCalcBestAction
 * @sa G_ClientMove
 * @sa G_ClientShoot
 */
void AIL_ActorThink (player_t * player, edict_t * ent)
{
	lua_State *L;

	/* The Lua State we will work with. */
	L = ent->chr.AI.L;

	/* Set the global player and edict */
	AIL_ent = ent;
	AIL_player = player;

	/* Try to run the function. */
	lua_getglobal(L, "think");
	if (lua_pcall(L, 0, 0, 0)) { /* error has occured */
		gi.dprintf("Error while running Lua: %s\n",
			lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown Error");
	}

	/* Cleanup */
	AIL_ent = NULL;
	AIL_player = NULL;
}


/**
 * @brief Initializes the AI.
 * @param[in] ent Pointer to actor to initialize AI for.
 * @param[in] type Type of AI (Lua file name without .lua).
 * @param[in] subtype Subtype of the AI.
 * @return 0 on success.
 */
int AIL_InitActor (edict_t * ent, char *type, char *subtype)
{
	AI_t *AI;
	int size;
	char path[MAX_VAR];
	char *fbuf;

	/* Prepare the AI */
	AI = &ent->chr.AI;
	Q_strncpyz(AI->type, type, sizeof(AI->type));
	Q_strncpyz(AI->subtype, subtype, sizeof(AI->type));

	/* Create the new Lua state */
	AI->L = luaL_newstate();
	if (AI->L == NULL) {
		gi.dprintf("Unable to create Lua state.\n");
		return -1;
	}

	/* Register metatables. */
	actorL_register(AI->L);
	pos3L_register(AI->L);

	/* Register libraries. */
	luaL_register(AI->L, "ai", AIL_methods);

	/* Load the AI */
	Com_sprintf(path, sizeof(path), "ai/%s.lua", type);
	size = gi.FS_LoadFile(path, (byte **) &fbuf);
	if (size == 0) {
		gi.dprintf("Unable to load Lua file '%s'.\n", path);
		return -1;
	}
	if (luaL_dobuffer(AI->L, fbuf, size, path)) {
		gi.dprintf("Unable to parse Lua file '%s'\n", path);
		gi.FS_FreeFile(fbuf);
		return -1;
	}
	gi.FS_FreeFile(fbuf);

	return 0;
}

/**
 * @brief Cleans up the AI part of the actor.
 * @param[in] ent Pointer to actor to cleanup AI.
 */
static void AIL_CleanupActor (edict_t * ent)
{
	AI_t *AI;
	AI = &ent->chr.AI;

	/* Cleanup. */
	if (AI->L != NULL) {
		lua_close(AI->L);
		AI->L = NULL;
	}
}


/**
 * @brief Purges all the AI from the entities.
 */
void AIL_Cleanup (void)
{
	int i;

	for (i = 0; i < globals.num_edicts; i++) {
		edict_t *ent = g_edicts + i;
		if (ent->inuse && G_IsActor(ent))
			AIL_CleanupActor(ent);
	}
}
