/**
 * @file g_spawn.c
 * @brief Brings new objects into the world.
 */

/*
All original materal Copyright (C) 2002-2006 UFO: Alien Invasion team.

26/06/06, Eddy Cullen (ScreamingWithNoSound):
	Reformatted to agreed style.
	Added doxygen file comment.
	Updated copyright notice.

Original file from Quake 2 v3.21: quake2-2.31/game/g_spawn.c
Copyright (C) 1997-2001 Id Software, Inc.

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

spawn_temp_t st;

static void SP_light(edict_t * ent);
static void SP_misc_dummy(edict_t * ent);
static void SP_player_start(edict_t * ent);
static void SP_human_start(edict_t * ent);
static void SP_alien_start(edict_t * ent);
static void SP_civilian_start(edict_t * ent);
static void SP_func_breakable(edict_t * ent);
static void SP_worldspawn(edict_t * ent);
static void SP_ugv_start(edict_t * ent);
static void SP_civilian_target(edict_t * ent);
static void SP_misc_mission(edict_t * ent);
static void SP_misc_mission_aliens(edict_t * ent);

typedef struct {
	char *name;
	void (*spawn) (edict_t * ent);
} spawn_t;

static spawn_t spawns[] = {
	{"worldspawn", SP_worldspawn},
	{"light", SP_light},
	{"misc_model", SP_misc_dummy},
	{"misc_particle", SP_misc_dummy},
	{"misc_mission", SP_misc_mission},
	{"misc_mission_aliens", SP_misc_mission_aliens},
	{"info_player_start", SP_player_start},
	{"info_human_start", SP_human_start},
	{"info_alien_start", SP_alien_start},
	{"info_civilian_start", SP_civilian_start},
	{"info_civilian_target", SP_civilian_target},
	{"info_ugv_start", SP_ugv_start},
	{"func_breakable", SP_func_breakable},

	{NULL, NULL}
};

typedef enum {
	F_INT,
	F_FLOAT,
	F_LSTRING,					/* string on disk, pointer in memory, TAG_LEVEL */
	F_GSTRING,					/* string on disk, pointer in memory, TAG_GAME */
	F_VECTOR,
	F_ANGLEHACK,
	F_EDICT,					/* index on disk, pointer in memory */
/*	F_ITEM,				// index on disk, pointer in memory */
	F_CLIENT,					/* index on disk, pointer in memory */
	F_FUNCTION,
	F_IGNORE
} fieldtype_t;

typedef struct {
	char *name;
	size_t ofs;
	fieldtype_t type;
	int flags;
} field_t;

static field_t fields[] = {
	{"classname", offsetof(edict_t, classname), F_LSTRING, 0},
	{"model", offsetof(edict_t, model), F_LSTRING, 0},
	{"spawnflags", offsetof(edict_t, spawnflags), F_INT, 0},
	{"speed", offsetof(edict_t, speed), F_FLOAT, 0},
	{"accel", offsetof(edict_t, accel), F_FLOAT, 0},
	{"decel", offsetof(edict_t, decel), F_FLOAT, 0},
	{"target", offsetof(edict_t, target), F_LSTRING, 0},
	{"targetname", offsetof(edict_t, targetname), F_LSTRING, 0},
	{"particle", offsetof(edict_t, particle), F_LSTRING, 0},
	{"team", offsetof(edict_t, team), F_INT, 0},
	{"size", offsetof(edict_t, fieldSize), F_INT, 0},
	{"wait", offsetof(edict_t, wait), F_FLOAT, 0},
	{"delay", offsetof(edict_t, delay), F_FLOAT, 0},
	{"random", offsetof(edict_t, random), F_FLOAT, 0},
	{"style", offsetof(edict_t, style), F_INT, 0},
	{"count", offsetof(edict_t, count), F_INT, 0},
	{"health", offsetof(edict_t, HP), F_INT, 0},
	{"sounds", offsetof(edict_t, sounds), F_INT, 0},
	{"light", 0, F_IGNORE, 0},
	{"dmg", offsetof(edict_t, dmg), F_INT, 0},
	{"origin", offsetof(edict_t, origin), F_VECTOR, 0},
	{"angles", offsetof(edict_t, angles), F_VECTOR, 0},
	{"angle", offsetof(edict_t, angle), F_FLOAT, 0},
	{"maxteams", offsetof(spawn_temp_t, maxteams), F_INT, 0},

	/*need for item field in edict struct, FFL_SPAWNTEMP item will be skipped on saves */
	{"gravity", offsetof(spawn_temp_t, gravity), F_LSTRING, FFL_SPAWNTEMP},
	{"minyaw", offsetof(spawn_temp_t, minyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"maxyaw", offsetof(spawn_temp_t, maxyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"minpitch", offsetof(spawn_temp_t, minpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"maxpitch", offsetof(spawn_temp_t, maxpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"nextmap", offsetof(spawn_temp_t, nextmap), F_LSTRING, FFL_SPAWNTEMP},

	{0, 0, 0, 0}
};

/**
 * @brief Finds the spawn function for the entity and calls it
 */
static void ED_CallSpawn(edict_t * ent)
{
	spawn_t *s;

	if (!ent->classname) {
		gi.dprintf("ED_CallSpawn: NULL classname\n");
		return;
	}

	/* check normal spawn functions */
	for (s = spawns; s->name; s++) {
		/* found it */
		if (!Q_strcmp(s->name, ent->classname)) {
			s->spawn(ent);
			return;
		}
	}

	gi.dprintf("%s doesn't have a spawn function\n", ent->classname);
	ent->inuse = qfalse;
}

/**
 * @brief
 */
static char *ED_NewString(char *string)
{
	char *newb, *new_p;
	int i;
	size_t l;

	l = strlen(string) + 1;

	newb = gi.TagMalloc(l, TAG_LEVEL);

	new_p = newb;

	for (i = 0; i < l; i++) {
		if (string[i] == '\\' && i < l - 1) {
			i++;
			if (string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		} else
			*new_p++ = string[i];
	}

	return newb;
}

/**
 * @brief Takes a key/value pair and sets the binary values in an edict
 */
static void ED_ParseField(char *key, char *value, edict_t * ent)
{
	field_t *f;
	byte *b;
	float v;
	vec3_t vec;

	for (f = fields; f->name; f++) {
		if (!(f->flags & FFL_NOSPAWN) && !Q_stricmp(f->name, key)) {
			/* found it */
			if (f->flags & FFL_SPAWNTEMP)
				b = (byte *) & st;
			else
				b = (byte *) ent;

			switch (f->type) {
			case F_LSTRING:
				*(char **) (b + f->ofs) = ED_NewString(value);
				break;
			case F_VECTOR:
				sscanf(value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
				((float *) (b + f->ofs))[0] = vec[0];
				((float *) (b + f->ofs))[1] = vec[1];
				((float *) (b + f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int *) (b + f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(float *) (b + f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((float *) (b + f->ofs))[0] = 0;
				((float *) (b + f->ofs))[1] = v;
				((float *) (b + f->ofs))[2] = 0;
				break;
			case F_IGNORE:
				break;
			case F_GSTRING:
				break;
			case F_EDICT:
				break;
			case F_CLIENT:
				break;
			case F_FUNCTION:
				break;
			}
			return;
		}
	}
/*	gi.dprintf ("ED_ParseField: %s is not a valid field\n", key);*/
}

/**
 * @brief Parses an edict out of the given string, returning the new position
 * @param[in] ent should be a properly initialized empty edict.
 */
static char *ED_ParseEdict(char *data, edict_t * ent)
{
	qboolean init;
	char keyname[256];
	char *com_token;

	init = qfalse;
	memset(&st, 0, sizeof(st));

	/* go through all the dictionary pairs */
	while (1) {
		/* parse key */
		com_token = COM_Parse(&data);
		if (com_token[0] == '}')
			break;
		if (!data)
			gi.error("ED_ParseEntity: EOF without closing brace");

		Q_strncpyz(keyname, com_token, sizeof(keyname));

		/* parse value */
		com_token = COM_Parse(&data);
		if (!data)
			gi.error("ED_ParseEntity: EOF without closing brace");

		if (com_token[0] == '}')
			gi.error("ED_ParseEntity: closing brace without data");

		init = qtrue;

		/* keynames with a leading underscore are used for utility comments, */
		/* and are immediately discarded by quake */
		if (keyname[0] == '_')
			continue;

		ED_ParseField(keyname, com_token, ent);
	}

	if (!init)
		memset(ent, 0, sizeof(*ent));

	return data;
}

/**
 * @brief Creates a server's entity / program execution context
 * by parsing textual entity definitions out of an ent file.
 */
void SpawnEntities(char *mapname, char *entities)
{
	edict_t *ent;
	int entnum;
	char *com_token;

	gi.FreeTags(TAG_LEVEL);

	memset(&level, 0, sizeof(level));
	memset(g_edicts, 0, game.maxentities * sizeof(edict_t));

	Q_strncpyz(level.mapname, mapname, sizeof(level.mapname));

	ent = NULL;
	level.activeTeam = -1;

	/* parse ents */
	entnum = 0;
	while (1) {
		/* parse the opening brace */
		com_token = COM_Parse(&entities);
		if (!entities)
			break;
		if (com_token[0] != '{')
			gi.error("ED_LoadFromFile: found %s when expecting {", com_token);

		if (!ent)
			ent = g_edicts;
		else
			ent = G_Spawn();
		entities = ED_ParseEdict(entities, ent);

		VecToPos(ent->origin, ent->pos);
		gi.GridPosToVec(gi.map, ent->pos, ent->origin);

		ent->mapNum = entnum++;
		ED_CallSpawn(ent);
	}

	/* spawn ai players, if needed */
	if (level.num_spawnpoints[TEAM_CIVILIAN]) {
		if (AI_CreatePlayer(TEAM_CIVILIAN) == NULL)
			Com_Printf("Could not create civilian\n");
	}
#ifdef DEBUG
	else
		Com_Printf("No civilian spawn points in this map or civlians deactivated\n");
#endif

	if (((int) sv_maxclients->value == 1 || ai_numactors->value) && level.num_spawnpoints[TEAM_ALIEN]) {
		if (AI_CreatePlayer(TEAM_ALIEN) == NULL)
			Com_Printf("Could not create alien\n");
#ifdef DEBUG
	} else {
		Com_Printf("No alien spawn points in this map or aliens are deactivated for multiplayer\n");
		Com_Printf("sv_maxclients %.0f, ai_numactors: %.0f, alien spawnpoints: %i)\n", sv_maxclients->value, ai_numactors->value, level.num_spawnpoints[TEAM_ALIEN]);
#endif
	}
}


/**
 * @brief QUAKED light (0 1 0) (-8 -8 -8) (8 8 8)
 */
static void SP_light(edict_t * self)
{
	/* lights aren't client-server communicated items */
	/* they are completely client side */
	G_FreeEdict(self);
}

/**
 * @brief
 */
static void G_ActorSpawn(edict_t * ent)
{
	/* set properties */
	level.num_spawnpoints[ent->team]++;
	ent->classname = "actor";
	ent->type = ET_ACTORSPAWN;
	ent->fieldSize = ACTOR_SIZE_NORMAL;

	/* fall to ground */
	if (ent->pos[2] >= HEIGHT)
		ent->pos[2] = HEIGHT - 1;
	ent->pos[2] = gi.GridFall(gi.map, ent->pos);
	gi.GridPosToVec(gi.map, ent->pos, ent->origin);

	/* link it for collision detection */
	ent->dir = AngleToDV(ent->angle);
	ent->solid = SOLID_BBOX;
	/* maybe this is already set in one of the spawn functions */
	if (ent->maxs[0] == 0)
		VectorSet(ent->maxs, PLAYER_WIDTH, PLAYER_WIDTH, PLAYER_STAND);
	if (ent->mins[0] == 0)
		VectorSet(ent->mins, -PLAYER_WIDTH, -PLAYER_WIDTH, PLAYER_MIN);
}

/**
 * @brief Spawn an singleplayer UGV
 */
static void G_UGVSpawn(edict_t * ent)
{
	/* set properties */
	level.num_ugvspawnpoints[ent->team]++;
	ent->classname = "ugv";
	ent->type = ET_UGVSPAWN;
	ent->fieldSize = ACTOR_SIZE_UGV;

	/* fall to ground */
	if (ent->pos[2] >= HEIGHT)
		ent->pos[2] = HEIGHT - 1;
	ent->pos[2] = gi.GridFall(gi.map, ent->pos);
	gi.GridPosToVec(gi.map, ent->pos, ent->origin);

	/* link it for collision detection */
	ent->dir = AngleToDV(ent->angle);
	ent->solid = SOLID_BBOX;
}

/**
 * @brief QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
 * Starting point for a player.
 * "team"	the number of the team for this player starting point
 * "0" is reserved for civilians and critters (use info_civilian_start instead)
 */
static void SP_player_start(edict_t * ent)
{
	static int soldierCount = 0;

	/* only used in multi player */
	if (sv_maxclients->value == 1) {
		G_FreeEdict(ent);
		return;
	}

	/* mapchange? */
	if (!level.num_spawnpoints[ent->team])
		soldierCount = 0;

	/* in teamplay mode check whether the player has reached */
	/* the max allowed soldiers per player */
	if ((int) maxsoldiersperplayer->value && soldierCount >= (int) maxsoldiersperplayer->value && (int) sv_teamplay->value) {
		gi.dprintf("Only %i/%i soldiers per player allowed - don't use this starting point for team %i\n", (int) maxsoldiersperplayer->value, soldierCount, ent->team);
		G_FreeEdict(ent);
		return;
	}

	/* maybe there are already the max soldiers allowed per team connected */
	if ((int) (maxsoldiers->value) > level.num_spawnpoints[ent->team]) {
		ent->STUN = 0;
		ent->HP = MAX_HP;
		ent->AP = 100;
		G_ActorSpawn(ent);
		soldierCount++;
	} else if (soldierCount <= 0) {
		gi.dprintf("No free soldier slots available - please choose another team\n");
		G_FreeEdict(ent);
	} else
		G_FreeEdict(ent);
}

/**
 * @brief QUAKED info_human_start (1 0 0) (-16 -16 -24) (16 16 32)
 * Starting point for a single player human.
 */
static void SP_human_start(edict_t * ent)
{
	/* only used in single player */
	if (sv_maxclients->value > 1) {
		G_FreeEdict(ent);
		return;
	}
	ent->team = TEAM_PHALANX;
	ent->STUN = 0;
	ent->HP = MAX_HP;
	ent->AP = 100;
	G_ActorSpawn(ent);
}


/**
 * @brief QUAKED info_ugv_start (1 1 0) (-32 -32 -24) (32 32 32)
 * Starting point for a ugv.
 */
static void SP_ugv_start(edict_t * ent)
{
	/* no ugv in multiplayer */
	if (sv_maxclients->value > 1) {
		G_FreeEdict(ent);
		return;
	}
	/* set stats */
	ent->STUN = 0;
	ent->HP = MAX_HP;
	ent->AP = 100;

	/* these units are bigger */
	VectorSet(ent->maxs, PLAYER_WIDTH * 2, PLAYER_WIDTH * 2, PLAYER_STAND);
	VectorSet(ent->mins, -(PLAYER_WIDTH * 2), -(PLAYER_WIDTH * 2), PLAYER_MIN);

	/* spawn singleplayer ugv */
	G_UGVSpawn(ent);
}

/**
 * @brief QUAKED info_alien_start (1 0 0) (-16 -16 -24) (16 16 32)
 * Starting point for a single player alien.
 */
static void SP_alien_start(edict_t * ent)
{
	/* deactivateable in multiplayer */
	if ((int)sv_maxclients->value > 1 && !ai_numactors->value) {
		G_FreeEdict(ent);
		return;
	}
	ent->team = TEAM_ALIEN;
	/* set stats */
	ent->STUN = 0;
	ent->HP = MAX_HP;
	ent->AP = 100;

	G_ActorSpawn(ent);
}


/**
 * @brief QUAKED info_civilian_start (0 1 1) (-16 -16 -24) (16 16 32)
 * Starting point for a civilian.
 */
static void SP_civilian_start(edict_t * ent)
{
	/* deactivateable in multiplayer */
	if ((int)sv_maxclients->value > 1 && !ai_numcivilians->value) {
		G_FreeEdict(ent);
		return;
	}
	ent->team = TEAM_CIVILIAN;
	/* set stats */
	ent->STUN = 99;
	ent->HP = MAX_HP;
	ent->AP = 100;
	ent->count = 100; /* current waypoint */
	G_ActorSpawn(ent);
}

/**
 * @brief QUAKED info_civilian_start (0 1 1) (-16 -16 -24) (16 16 32)
 * Way point for a civilian.
 * @sa SP_civilian_start
 */
static void SP_civilian_target(edict_t * ent)
{
	/* target point for which team */
	ent->team = TEAM_CIVILIAN;
	ent->classname = "civtarget";
	ent->type = ET_CIVILIANTARGET;

	/* fall to ground */
	if (ent->pos[2] >= HEIGHT)
		ent->pos[2] = HEIGHT - 1;
	ent->pos[2] = gi.GridFall(gi.map, ent->pos);
	gi.GridPosToVec(gi.map, ent->pos, ent->origin);
}

/**
 * @brief Init the human/phalanx mission entity
 */
static void SP_misc_mission(edict_t * ent)
{
	ent->classname = "mission";
	ent->type = ET_MISSION;
	ent->team = TEAM_PHALANX;

	/* fall to ground */
	if (ent->pos[2] >= HEIGHT)
		ent->pos[2] = HEIGHT - 1;
	ent->pos[2] = gi.GridFall(gi.map, ent->pos);
	gi.GridPosToVec(gi.map, ent->pos, ent->origin);
}

/**
 * @brief Init the alien mission entity
 */
static void SP_misc_mission_aliens(edict_t * ent)
{
	ent->classname = "mission";
	ent->type = ET_MISSION;
	ent->team = TEAM_ALIEN;

	/* fall to ground */
	if (ent->pos[2] >= HEIGHT)
		ent->pos[2] = HEIGHT - 1;
	ent->pos[2] = gi.GridFall(gi.map, ent->pos);
	gi.GridPosToVec(gi.map, ent->pos, ent->origin);
}

/**
 * @brief a dummy to get rid of local entities
 */
static void SP_misc_dummy(edict_t * self)
{
	/* models and particles aren't client-server communicated items */
	/* they are completely client side */
	G_FreeEdict(self);
}


/**
 * @brief QUAKED func_breakable (0.3 0.3 0.3) ?
 * Used for breakable objects.
 */
static void SP_func_breakable(edict_t * self)
{
	self->type = ET_BREAKABLE;

	VectorSet(self->origin, 0, 0, 0);
	gi.setmodel(self, self->model);

#if 0
	Com_Printf( "model (%s) num: %i mins: %i %i %i maxs: %i %i %i\n",
		self->model, self->mapNum, (int)self->mins[0], (int)self->mins[1], (int)self->mins[2],
		(int)self->maxs[0], (int)self->maxs[1], (int)self->maxs[2] );
#endif
}


/**
 * @brief QUAKED worldspawn (0 0 0) ?
 *
 * Only used for the world.
 * "sounds"	music cd track number
 * "gravity"	800 is default gravity
 * "message"	text to print at user logon
 * "maxlevel"	max. level to use in the map
 * "maxteams"	max team amount for multiplayergames for the current map
 */
static void SP_worldspawn(edict_t * ent)
{
	ent->solid = SOLID_BSP;
	/* since the world doesn't use G_Spawn() */
	ent->inuse = qtrue;

	if (st.nextmap)
		Q_strncpyz(level.nextmap, st.nextmap, MAX_QPATH);

	/* default value */
	if (st.maxteams < 2)
		st.maxteams = 2;

	/* make some data visible to the server */
	if (ent->message && ent->message[0]) {
		gi.configstring(CS_NAME, ent->message);
		Q_strncpyz(level.level_name, ent->message, MAX_QPATH);
	} else
		Q_strncpyz(level.level_name, level.mapname, MAX_QPATH);

	gi.configstring(CS_CDTRACK, va("%i", ent->sounds));

	gi.configstring(CS_MAXCLIENTS, va("%i", (int) (maxplayers->value)));

	/* only used in multi player */
	if (sv_maxclients->value >= 2) {
		gi.configstring(CS_MAXSOLDIERS, va("%i", (int) (maxsoldiers->value)));
		gi.configstring(CS_MAXSOLDIERSPERPLAYER, va("%i", (int) (maxsoldiersperplayer->value)));
		gi.configstring(CS_ENABLEMORALE, va("%i", (int) (sv_enablemorale->value)));
		gi.configstring(CS_MAXTEAMS, va("%i", st.maxteams));
		gi.cvar_set("sv_maxteams", va("%i", st.maxteams));
	}

	if (!st.gravity)
		gi.cvar_set("sv_gravity", "800");
	else
		gi.cvar_set("sv_gravity", st.gravity);

	/* Setup light animation tables. 'a' is total darkness, 'z' is doublebright. */

	/* 0 normal */
	gi.configstring(CS_LIGHTS + 0, "m");

	/* 1 FLICKER (first variety) */
	gi.configstring(CS_LIGHTS + 1, "mmnmmommommnonmmonqnmmo");

	/* 2 SLOW STRONG PULSE */
	gi.configstring(CS_LIGHTS + 2, "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");

	/* 3 CANDLE (first variety) */
	gi.configstring(CS_LIGHTS + 3, "mmmmmaaaaammmmmaaaaaabcdefgabcdefg");

	/* 4 FAST STROBE */
	gi.configstring(CS_LIGHTS + 4, "mamamamamama");

	/* 5 GENTLE PULSE 1 */
	gi.configstring(CS_LIGHTS + 5, "jklmnopqrstuvwxyzyxwvutsrqponmlkj");

	/* 6 FLICKER (second variety) */
	gi.configstring(CS_LIGHTS + 6, "nmonqnmomnmomomno");

	/* 7 CANDLE (second variety) */
	gi.configstring(CS_LIGHTS + 7, "mmmaaaabcdefgmmmmaaaammmaamm");

	/* 8 CANDLE (third variety) */
	gi.configstring(CS_LIGHTS + 8, "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");

	/* 9 SLOW STROBE (fourth variety) */
	gi.configstring(CS_LIGHTS + 9, "aaaaaaaazzzzzzzz");

	/* 10 FLUORESCENT FLICKER */
	gi.configstring(CS_LIGHTS + 10, "mmamammmmammamamaaamammma");

	/* 11 SLOW PULSE NOT FADE TO BLACK */
	gi.configstring(CS_LIGHTS + 11, "abcdefghijklmnopqrrqponmlkjihgfedcba");

	/* styles 32-62 are assigned by the light program for switchable lights */

	/* 63 testing */
	gi.configstring(CS_LIGHTS + 63, "a");
}
