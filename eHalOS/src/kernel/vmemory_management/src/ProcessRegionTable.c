/*
 * ProcessRegionTable.c
 *
 *  Created on: 02.02.2009
 *      Author: Christian Brändle
 */

#include "ProcessRegionTable.h"

#define MAXPROCESSES 32
#define RAMPAGERID 1
#define SWAPPAGERID 2

PageTable RamPageTable = {
		MAXHASHANCHORTABLESIZE,
		HashAnchorTable,
		MAXPAGETABLEENTRIES,
		InvertedPageTable
};

Pager RamPager = {
	RAMPAGERID,
	RamPagerType,
	&RamPageTable
};

Pager SwapPager = {
	SWAPPAGERID,
	SwapPagerType,
	NULL
};

PagerPtr Pagers[] =
{
	&RamPager,
	&SwapPager
};

ProcessRegionTable IllegalProcessRegionTable =
{
	0,

	0,
	NULL,

	0,
	NULL,
	NULL
};

ProcessRegionTablePtr ProcessRegionTables[MAXPROCESSES];

void initProcessRegionTables()
{
	int index = 0;
	for (index = 0; index < MAXPROCESSES; index++)
	{
		ProcessRegionTables[index] = &IllegalProcessRegionTable;
	}
}

ProcessRegionTablePtr reuseProcessRegionTable(int ASID, BOOL *autoUnload)
{
	if (autoUnload == NULL)
		return NULL;

	ProcessRegionTablePtr pProcessRegionTable = findProcessRegionTable(ASID);
	if (pProcessRegionTable != NULL)
		*autoUnload = unloadProcessRegionTable(ASID);

	return createProcessRegionTable(ASID);
}

ProcessRegionTablePtr createProcessRegionTable(int ASID)
{
	ProcessRegionTablePtr pProcessRegionTable = NULL;
	int index;

	for (index = 0; index < MAXPROCESSES; index++)
	{
		if (isIllegalProcessRegionTable(ProcessRegionTables[index]))
		{
			pProcessRegionTable =
				(ProcessRegionTablePtr) malloc(sizeof(ProcessRegionTable));

			pProcessRegionTable->ASID = ASID;
			pProcessRegionTable->pagersSize = 2;
			pProcessRegionTable->pPagers = (PagerHdl) malloc(sizeof(PagerPtr)*2);
			pProcessRegionTable->pPagers[0] = Pagers[0];
			pProcessRegionTable->pPagers[1] = Pagers[1];

			pProcessRegionTable->ProcessRegionEntrySize = 0;
			pProcessRegionTable->ProcessRegions = NULL;
			//pProcessRegionTable->UsedRegions = NULL;

			ProcessRegionTables[index] = pProcessRegionTable;
			return pProcessRegionTable;
		}
	}

	return NULL;
}

ProcessRegionTablePtr findProcessRegionTable(int ASID)
{
	int index = 0;
	for (index = 0; index < MAXPROCESSES; index++)
	{
		if (ProcessRegionTables[index]->ASID == ASID)
			return ProcessRegionTables[index];
	}

	return NULL;
}


BOOL addBinImageRegionToRamPager(int ASID, BinImagePtr pBinImage)
{
	// simplest variant
	// give all pages of bin image all access rights and add them to region as ram

	// search corresponding ProcessRegionTable
	ProcessRegionPtr pProcessRegion;
	ProcessRegionTablePtr pProcessRegionTable = findProcessRegionTable(ASID);

	if (pProcessRegionTable == NULL)
		return FALSE;

	// add a ProcessRegion and initialize it with size of BinImage and full access

	pProcessRegionTable->ProcessRegionEntrySize++;
	if (pProcessRegionTable->ProcessRegions == NULL)
		pProcessRegionTable->ProcessRegions = malloc(sizeof(ProcessRegionPtr)*pProcessRegionTable->ProcessRegionEntrySize);
	else
		pProcessRegionTable->ProcessRegions = realloc(pProcessRegionTable->ProcessRegions, sizeof(ProcessRegionPtr)*pProcessRegionTable->ProcessRegionEntrySize);

	pProcessRegionTable->ProcessRegions[pProcessRegionTable->ProcessRegionEntrySize-1] = (ProcessRegionPtr) malloc(sizeof(ProcessRegion));

	pProcessRegion = pProcessRegionTable->ProcessRegions[pProcessRegionTable->ProcessRegionEntrySize-1];
	//pProcessRegion->StartVPN = pBinImage->baseAddress / (PAGESIZE * 1024);
	// HACK - App must be linked in second page
	pProcessRegion->StartVPN = pBinImage->baseAddress / (PAGESIZE * 1024) - 1;
	pProcessRegion->NrOfVPNs = nrOfPagesForBinImage(pBinImage);

	pProcessRegion->AccessRightsSize = 3;
	pProcessRegion->AccessRights = malloc(sizeof(AccessRight)*3);

	pProcessRegion->AccessRights[0] = Read;
	pProcessRegion->AccessRights[1] = Write;
	pProcessRegion->AccessRights[2] = Execute;

	pProcessRegion->TypeSize = 4;
	pProcessRegion->ProcessRegionType = (char *) calloc(sizeof("data")+1, sizeof(char));
	pProcessRegion->ProcessRegionType[0]='d';
	pProcessRegion->ProcessRegionType[1]='a';
	pProcessRegion->ProcessRegionType[2]='t';
	pProcessRegion->ProcessRegionType[3]='a';

	pProcessRegion->PagerID = RAMPAGERID;

	return TRUE;
}

BOOL addRegion(int ASID, ProcessRegionPtr pProcessRegion)
{
	ProcessRegionTablePtr pProcessRegionTable = findProcessRegionTable(ASID);

	if (pProcessRegionTable == NULL)
		return FALSE;

	// add a ProcessRegion and return

	pProcessRegionTable->ProcessRegionEntrySize++;
	pProcessRegionTable->ProcessRegions = realloc(pProcessRegionTable->ProcessRegions, sizeof(ProcessRegionPtr)*pProcessRegionTable->ProcessRegionEntrySize);

	pProcessRegionTable->ProcessRegions[pProcessRegionTable->ProcessRegionEntrySize-1] = pProcessRegion;

	return TRUE;
}

BOOL isIllegalProcessRegionTable(ProcessRegionTablePtr pProcessRegionTable)
{
	if (pProcessRegionTable == NULL)
		return FALSE;

	if (pProcessRegionTable->ASID == IllegalProcessRegionTable.ASID)
		return TRUE;

	return FALSE;
}

BOOL unloadProcessRegionTable(int ASID)
{
	// TODO: knowledge of all pages of app is necessary
	// this is job of vm_space and vnode_object

	// search infos of regiontable and call frees from pagers

	PageTableEntryPtr pPTEntry = NULL;

	int nrOfActRegion;
	ProcessRegionPtr pActProcessRegion;
	TLBEHI tlbEntryHigh;
	int nrOfActConsecPage;
	BOOL succ = TRUE;
	ProcessRegionTablePtr pProcessRegionTable = findProcessRegionTable(ASID);

	if (pProcessRegionTable == NULL)
		return FALSE;

	// QUICK HACK - because InvalidateTLBEntry do not work properly...
	InvalidateTLB();

	// go through all regions
	for (nrOfActRegion=0; nrOfActRegion<pProcessRegionTable->ProcessRegionEntrySize; nrOfActRegion++ )
	{
		pActProcessRegion = pProcessRegionTable->ProcessRegions[nrOfActRegion];

		// determine all pages and free them
		tlbEntryHigh.ASID = pProcessRegionTable->ASID;
		tlbEntryHigh.VPN = pActProcessRegion->StartVPN;
		for (nrOfActConsecPage = 1; nrOfActConsecPage<=pActProcessRegion->NrOfVPNs; nrOfActConsecPage++ )
		{
			pPTEntry = searchPageTableEntry(&tlbEntryHigh);
			//InvalidateTLBEntry(pPTEntry);
			if(freePageTableEntry(&tlbEntryHigh) == FALSE)
				succ = FALSE;
			tlbEntryHigh.VPN++;
		}
	}

	//make complete free of all ProcessRegionTable resources
	freeProcessRegionTableContents(pProcessRegionTable);

	*pProcessRegionTable = IllegalProcessRegionTable;

	return succ;
}

BOOL freeProcessRegionTableContents(ProcessRegionTablePtr pProcessRegionTable)
{
	int  index;

	if (pProcessRegionTable == NULL)
		return FALSE;

	for (index = 0; index < pProcessRegionTable->ProcessRegionEntrySize; index++)
	{
		ProcessRegionPtr pProcessRegion = pProcessRegionTable->ProcessRegions[index];
		freeProcessRegionContents(pProcessRegion);
	}

	if (pProcessRegionTable->ProcessRegions != NULL)
		free(pProcessRegionTable->ProcessRegions);

	pProcessRegionTable->ProcessRegions = NULL;

	return TRUE;
}

BOOL freeProcessRegionContents(ProcessRegionPtr pProcessRegion){
	if (pProcessRegion == NULL )
			return FALSE;

	if (pProcessRegion->AccessRights != NULL)
		free(pProcessRegion->AccessRights);
	pProcessRegion->AccessRights = NULL;
	if (pProcessRegion->ProcessRegionType != NULL)
		free(pProcessRegion->ProcessRegionType);
	pProcessRegion->ProcessRegionType = NULL;
	return TRUE;
}

ProcessRegionPtr findProcessRegionByMemoryRegion(int ASID, size_t size, void *pMemory)
{
	int32_t nrOfVPNs;
	int32_t startVPN;
	int32_t index;

	ProcessRegionTablePtr pProcessRegionTable = findProcessRegionTable(ASID);

	if (pProcessRegionTable == NULL)
		return NULL;

	// calculate Page-Range
	nrOfVPNs = trunc((size+0.0) /(PAGESIZE * 1024)+1.0/(PAGESIZE * 1024));
	startVPN = ((int)pMemory) /(PAGESIZE * 1024);

	for (index=0; index<pProcessRegionTable->ProcessRegionEntrySize; index++)
	{
		ProcessRegionPtr pProcessRegion = pProcessRegionTable->ProcessRegions[index];

		if (pProcessRegion != NULL && pProcessRegion->StartVPN <= startVPN && pProcessRegion->NrOfVPNs >= nrOfVPNs)
			return pProcessRegion;
	}

	return NULL;
}

BOOL addRightToProcessRegion(ProcessRegionPtr pProcessRegion, AccessRight accessRight){
	// simply add the given AccessRight if it is not existent
	int32_t index;
	if (pProcessRegion == NULL)
		return FALSE;

	for (index=0; index<pProcessRegion->AccessRightsSize; index++)
		if (pProcessRegion->AccessRights[index] == accessRight)
			return FALSE;

	pProcessRegion->AccessRightsSize++;
	if (pProcessRegion->AccessRightsSize == 1)
		pProcessRegion->AccessRights = malloc(sizeof(AccessRight)*pProcessRegion->AccessRightsSize);
	else
		pProcessRegion->AccessRights = realloc(pProcessRegion->AccessRights, sizeof(AccessRight)*pProcessRegion->AccessRightsSize);

	pProcessRegion->AccessRights[pProcessRegion->AccessRightsSize-1] = accessRight;
	return TRUE;
}
/*
ProcessRegionPtr createProcessRegion(int pagerID, int startVPN, int nrOfVPNs, char *pProcessRegionType, int32_t accessRightsSize, AccessRight []accessRights)
{
	ProcessRegionPtr pProcessRegion = (ProcessRegionPtr) malloc(sizeof(ProcessRegion));
	pProcessRegion->PagerID = pagerID;
	pProcessRegion->StartVPN = startVPN;
	pProcessRegion->NrOfVPNs = nrOfVPNs;
	if (pProcessRegionType != NULL)
	{
		int_32_t len = strlen(pProcessRegionType);
		pProcessRegion->ProcessRegionType = (char *) calloc(sizeof(char),len+1);
		pProcessRegion->TypeSize = len+1;
		memcpy(pProcessRegion->ProcessRegionType, pProcessRegionType, len);
	}

	for (index=0; index<AccessRightsSize; index++)
		addRightToProcessRegion(pProcessRegion, accessRights[index]);

	return pProcessRegion;
}
*/


