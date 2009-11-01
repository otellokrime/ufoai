/**
 * @file cp_uforecovery.c
 * @brief UFO recovery and storing
 * @note UFO recovery functions with UR_*
 * @note UFO storing functions with US_*
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

#include "../client.h"
#include "../menu/m_main.h"
#include "cp_campaign.h"
#include "cp_ufo.h"
#include "cp_map.h"
#include "cp_uforecovery.h"
#include "cp_uforecovery_callbacks.h"
#include "cp_aircraft.h"

/**
 * @brief status names for savegames
 * @sa storedUFOStatus_t
 */
const char *ufostatus_strings[MAX_SUFO_STATUS] = {
	"recovered",
	"stored",
	"transfered"
};
CASSERT(lengthof(ufostatus_strings) == MAX_SUFO_STATUS);

/*==================================
Backend functions
==================================*/

/**
 * @brief Function to process active recoveries.
 * @sa CL_CampaignRun
 */
void UR_ProcessActive (void)
{
	int i;

	for (i = 0; i < ccs.numStoredUFOs; i++) {
		storedUFO_t *ufo = US_GetStoredUFOByIDX(i);
		if (!ufo)
			continue;

		assert(ufo->ufoTemplate);
		assert(ufo->ufoTemplate->tech);

		if (ufo->status == SUFO_STORED)
			continue;
		if (ufo->arrive.day > ccs.date.day || (ufo->arrive.day == ccs.date.day && ufo->arrive.sec > ccs.date.sec))
			continue;

		ufo->status = SUFO_STORED;

		Com_sprintf(cp_messageBuffer, sizeof(cp_messageBuffer), _("%s was transfered to %s."), UFO_TypeToName(ufo->ufoTemplate->ufotype), ufo->installation->name);
		MSO_CheckAddNewMessage(NT_TRANSFER_UFORECOVERY_FINISHED, _("UFO Recovered"), cp_messageBuffer, qfalse, MSG_TRANSFERFINISHED, NULL);
		
		if (!ufo->ufoTemplate->tech->statusCollected)
			RS_MarkCollected(ufo->ufoTemplate->tech);
	}
}

/* ==== UFO Storing stuff ==== */

/**
 * @brief Returns an ufostore place
 * @param[in] idx index of ufostore place
 * @return storedUFO_t Pointer
 */
storedUFO_t* US_GetStoredUFOPlaceByIDX (const int idx)
{
	if (idx < 0 || idx >= MAX_STOREDUFOS)
		return NULL;
	return &(ccs.storedUFOs[idx]);
}

/**
 * @brief Returns a stored ufo
 * @param[in] idx index of ufostore place
 * @return storedUFO_t Pointer
 */
storedUFO_t* US_GetStoredUFOByIDX (const int idx)
{
	if (idx < 0 || idx >= ccs.numStoredUFOs)
		return NULL;
	return &(ccs.storedUFOs[idx]);
}

/**
 * @brief Adds an UFO to the storage
 * @param[in] ufoTemplate Pointer to the aircraft(ufo)Template to add
 * @param[in,out] installation Pointer to the installation it should be added to
 * @param[in] date Date when UFO is arrives to the storage (recovery || transfer)
 * @param[in] condition Condition of the UFO to store (How much the UFO is damaged)
 * @return storedUFO_t pointer to the newly stored UFO (or NULL if failed)
 */
storedUFO_t *US_StoreUFO (const aircraft_t *ufoTemplate, installation_t *installation, date_t date, float condition)
{
	storedUFO_t *ufo;

	if (!ufoTemplate) {
		Com_DPrintf(DEBUG_CLIENT, "US_StoreUFO: Invalid aircraft (UFO) Template.\n");
		return NULL;
	}

	if (!installation) {
		Com_DPrintf(DEBUG_CLIENT, "US_StoreUFO: Invalid Installation\n");
		return NULL;
	}

	if (ccs.numStoredUFOs >= MAX_STOREDUFOS) {
		Com_DPrintf(DEBUG_CLIENT, "US_StoreUFO: stored UFOs array is full.\n");
		return NULL;
	}

	if (installation->ufoCapacity.cur >= installation->ufoCapacity.max) {
		Com_DPrintf(DEBUG_CLIENT, "US_StoreUFO: Installation is full with UFOs.\n");
		return NULL;
	}

	/* we can store it there */
	ufo = US_GetStoredUFOPlaceByIDX(ccs.numStoredUFOs);		/* Select first empty place */
	ufo->idx = ccs.numStoredUFOs;
	Q_strncpyz(ufo->id, ufoTemplate->id, sizeof(ufo->id));
	ufo->comp = CL_GetComponentsByID(ufo->id);
	assert(ufo->comp);

	ufo->installation = installation;
	installation->ufoCapacity.cur++;

	assert(ufoTemplate->tech);

	ufo->ufoTemplate = ufoTemplate;
	ufo->disassembly = NULL;

	ufo->arrive = date;
	if (date.day < ccs.date.day || (date.day == ccs.date.day && date.sec <= ccs.date.sec)) {
		ufo->status = SUFO_STORED;
		RS_MarkCollected(ufo->ufoTemplate->tech);
	} else {
		ufo->status = SUFO_RECOVERED;
	}
	ufo->condition = min(max(0, condition), 1);

	ccs.numStoredUFOs++;

	return ufo;
}

/**
 * @brief Removes an UFO from the storage
 * @param[in,out] ufo stored UFO to remove
 */
void US_RemoveStoredUFO (storedUFO_t *ufo)
{
	int ufoCount;
	int i;

	assert(ufo);

	/* Stop disassembling */
	if (ufo->disassembly) {
		base_t *prodBase = PR_ProductionBase(ufo->disassembly);

		assert(prodBase);

		if (ufo->disassembly->idx == 0)
			PR_QueueNext(prodBase);
		else
			PR_QueueDelete(prodBase, &ccs.productions[prodBase->idx], ufo->disassembly->idx);
	}

	/* Stop running research if this is the only UFO from this type
	 * also clear collected status */
	assert(ufo->ufoTemplate);
	ufoCount = US_UFOsInStorage(ufo->ufoTemplate, NULL);
	if (ufoCount <= 1 && ufo->ufoTemplate->tech->statusResearch == RS_RUNNING) {
		RS_StopResearch(ufo->ufoTemplate->tech);
		ufo->ufoTemplate->tech->statusCollected = qfalse;
	}

	/* remove ufo */
	ufo->installation->ufoCapacity.cur--;
	REMOVE_ELEM_ADJUST_IDX(ccs.storedUFOs, ufo->idx, ccs.numStoredUFOs);

	/* Adjust UFO pointer of other disassemblies */
	for (i = 0; i < ccs.numBases; i++) {
		int j;
		for (j = 0; j < ccs.productions[i].numItems; j++) {
			production_t *prod = &(ccs.productions[i].items[j]);
			if (!prod->ufo)
				continue;

			if (prod->ufo > ufo)
				prod->ufo--;
		}
	}
}


/**
 * @brief Returns the number of UFOs stored (on an installation or anywhere)
 * @param[in] ufoTemplate aircraftTemplate of the ufo
 * @param[in] installation Pointer to the installation to count at
 * @returns the number of ufos stored of the given ufotype at given installation or overall
 * @note installation == NULL means count on every ufoyards
 */
int US_UFOsInStorage (const aircraft_t *ufoTemplate, const installation_t *installation)
{
	int i;
	int count = 0;

	for (i = 0; i < ccs.numStoredUFOs; i++) {
		const storedUFO_t *ufo = US_GetStoredUFOByIDX(i);
		if (!ufo)
			continue;

		if (ufo->ufoTemplate != ufoTemplate)
			continue;
		if (installation && ufo->installation != installation)
			continue;
		if (ufo->status != SUFO_STORED)
		 	continue;

	 	count++;
	}

	return count;
}

/**
 * @brief Removes ufos which are over the storing capacity
 * @param[in] installation pointer to the ufoyard the ufos are stored in
 */
void US_RemoveUFOsExceedingCapacity (installation_t *installation)
{
	int i;
	const capacities_t *ufoCap;

	if (!installation)
		Com_Error(ERR_DROP, "US_RemoveUFOsExceedingCapacity: No installation given!\n");

	ufoCap = &(installation->ufoCapacity);

	/* loop ufos backwards */
	for (i = ccs.numStoredUFOs - 1; i >= 0; i--) {
		storedUFO_t *ufo = US_GetStoredUFOByIDX(i);

		assert(ufo);

		if (ufo->installation != installation)
			continue;

		if (ufoCap->cur > ufoCap->max)
			US_RemoveStoredUFO(ufo);
	}
}

/**
 * @brief get the closest stored ufo (of a type) from a base
 * @param[in] ufoTemplate Pointer to the aircraft (ufo) template to look for (@c NULL for any type)
 * @param[in] base Pointer to the base. If it's @c NULL the function simply return the first stored UFO of type
 * @return Pointer to the first stored UFO matches the conditions
 */
storedUFO_t *US_GetClosestStoredUFO (const aircraft_t *ufoTemplate, const base_t *base)
{
	int i;
	float minDistance = -1;
	storedUFO_t *closestUFO = NULL;

	for (i = 0; i < ccs.numStoredUFOs; i++) {
		storedUFO_t *ufo = US_GetStoredUFOByIDX(i);
		float distance = 0;

		if (!ufo)
			continue;

		if (ufoTemplate && ufo->ufoTemplate != ufoTemplate)
			continue;

		if (ufo->status != SUFO_STORED)
		 	continue;

		assert(ufo->installation);
		if (base)
			distance = GetDistanceOnGlobe(ufo->installation->pos, base->pos);

		if (minDistance < 0 || minDistance > distance) {
			minDistance = distance;
			closestUFO = ufo;
		}	
	}
	return closestUFO;
}

/**
 * @brief returns ufostatus from stringid
 * @param[in] id identifier string to look for
 * @returns storedUFOStatus_t Stored UFO status
 */
storedUFOStatus_t US_UFOStatusByID (const char *id)
{
	int i;

	for (i = 0; i < MAX_SUFO_STATUS; i++) {
		if (!strcmp(id, ufostatus_strings[i]))
			return i;
	}
	return MAX_SUFO_STATUS;
}

/**
 * @brief Save callback for savegames in XML Format
 * @sa US_LoadXML
 * @sa SAV_GameSaveXML
 */
qboolean US_SaveXML (mxml_node_t *p)
{
	int i;
	mxml_node_t *node = mxml_AddNode(p, "storedufos");
	for (i = 0; i < ccs.numStoredUFOs; i++) {
		const storedUFO_t *ufo = US_GetStoredUFOByIDX(i);
		mxml_node_t * snode = mxml_AddNode(node, "ufo");

		mxml_AddString(snode, "ufoid", ufo->id);
		mxml_AddInt(snode, "day", ufo->arrive.day);
		mxml_AddInt(snode, "sec", ufo->arrive.sec);
		mxml_AddString(snode, "status", ufostatus_strings[ufo->status]);
		mxml_AddFloat(snode, "condition", ufo->condition);

		if (ufo->installation)
			mxml_AddInt(snode, "installationidx", ufo->installation->idx);
	}
	return qtrue;
}

/**
 * @brief Load callback for xml savegames
 * @sa US_SaveXML
 * @sa SAV_GameLoadXML
 */
qboolean US_LoadXML (mxml_node_t *p)
{
	int i;
	mxml_node_t *node, *snode;

	node = mxml_GetNode(p, "storedufos");

	for (i = 0, snode = mxml_GetNode(node, "ufo"); i < MAX_STOREDUFOS && snode;
			i++, snode = mxml_GetNextNode(snode, node, "ufo")) {

		aircraft_t *ufoTemplate = AIR_GetAircraft(mxml_GetString(snode, "ufoid"));
		installation_t *inst = INS_GetFoundedInstallationByIDX(mxml_GetInt(snode, "installationidx", MAX_INSTALLATIONS));
		storedUFOStatus_t status = US_UFOStatusByID(mxml_GetString(snode, "status"));
		date_t arrive;
		storedUFO_t *ufo;
		float condition = mxml_GetFloat(snode, "condition", 1.0f);

		arrive.day = mxml_GetInt(snode, "day", 0);
		arrive.sec = mxml_GetInt(snode, "sec", 0);

		if (!ufoTemplate)
			return qfalse;

		if (!inst)
			return qfalse;

		ufo = US_StoreUFO(ufoTemplate, inst, arrive, condition);
		if (!ufo)
			Com_Printf("Cannot store ufo %s at installation idx=%i.\n", ufoTemplate->id, inst->idx);
		else {
			if (status != MAX_SUFO_STATUS) {
				ufo->status = status;
			} else {
				/* falback code for compatibility.
				 * @todo remove this before release */
				if (arrive.day < ccs.date.day || (arrive.day == ccs.date.day && arrive.sec < ccs.date.sec))
					ufo->status = SUFO_STORED;
			}
		}
	}
	return qtrue;
}

