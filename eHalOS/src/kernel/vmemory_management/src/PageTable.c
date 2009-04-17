#include "PageTable.h"

void initPageTable()
{
	initHashAnchorTable();
	initInvertedPageTable();
}

PageTableEntryPtr searchPageTableEntry(TLBEHIPtr pTLBEntryHigh)
{
	int32_t hatIndex = 0;
	HashAnchorTableEntry hatEntry;
	// 1. hash ASID and VPN to get HAT-Entry
	// 2. get IPT-Entry
	// 3. search IPT-Entry Chain
	hatIndex = getHashAnchorTableIndex(pTLBEntryHigh->ASID, pTLBEntryHigh->VPN);
	if (isIllegalHATIndex(hatIndex))
		return NULL;

	hatEntry = HashAnchorTable[hatIndex];
	if (isIllegalHATEntry(hatEntry))
		return NULL;

	return searchPageTableEntryInIPT(pTLBEntryHigh, hatEntry);
}

TLBELOPtr searchPageTableEntryLow(TLBEHIPtr pTLBEntryHigh)
{
	int32_t hatIndex = 0;
	HashAnchorTableEntry hatEntry;
	// 1. hash ASID and VPN to get HAT-Entry
	// 2. get IPT-Entry
	// 3. search IPT-Entry Chain
	hatIndex = getHashAnchorTableIndex(pTLBEntryHigh->ASID, pTLBEntryHigh->VPN);
	if (isIllegalHATIndex(hatIndex))
		return NULL;

	hatEntry = HashAnchorTable[hatIndex];
	if (isIllegalHATEntry(hatEntry))
		return NULL;

	return searchPageTableEntryLowInIPT(pTLBEntryHigh, hatEntry);
}
// ---


BOOL freePageTableEntry(TLBEHIPtr pTLBEntryHigh)
{
	PageTableEntryPtr pPTEntry = NULL;
	PageTableEntryPtr pHashAnchorPTEntry = NULL;
	PageTableEntryPtr pPrevPTEntry = NULL;
	uint32_t pfn;

	pPTEntry = searchPageTableEntryAndHashAnchorAndPreviousInIPT(pTLBEntryHigh, &pHashAnchorPTEntry, &pPrevPTEntry);

	if (pPTEntry == NULL)
		return FALSE;

	if (!dissolvePageTableEntry(pPTEntry, pHashAnchorPTEntry, pPrevPTEntry))
		return FALSE;

	pfn = pPTEntry->tlbEntryLow.PFN;
	*pPTEntry = IllegalPTEntry;
	pPTEntry->tlbEntryLow.PFN = pfn;

	return TRUE;
}

/* Dissovle given page table entry. Rechain page table chain and actualize HAT if necessary.
 * Do not delete contents of dissolved page table entries. This is the job of the caller.
 */
BOOL dissolvePageTableEntry(PageTableEntryPtr pPTEntry, PageTableEntryPtr pHashAnchorPTEntry, PageTableEntryPtr pPrevPTEntry)
{
	int32_t hatIndex;
	HashAnchorTableEntry hatEntry;

	if (pPTEntry == pHashAnchorPTEntry)
	{
		hatIndex = getHashAnchorTableIndex(pHashAnchorPTEntry->tlbEntryHigh.ASID, pHashAnchorPTEntry->tlbEntryHigh.VPN);
		if (isIllegalHATIndex(hatIndex))
			return FALSE;

		// actualize HAT
		if (pHashAnchorPTEntry->next != NULL )//&& isValid(pHashAnchorPTEntry->next->tlbEntryHigh) )
		{
			hatEntry = pHashAnchorPTEntry->next->tlbEntryLow.PFN-PFNOFFSET;
			return setHATEntry(hatIndex, hatEntry);
		}

		return setIllegalHATEntry(hatIndex);

	}
	else
	{
		// rechain
		pPrevPTEntry->next = pPTEntry->next;
	}

	return TRUE;
}

// TODO: rework, may ignore full table at the moment ...
// >> topic of vitual memory, demand paging and swapping
PageTableEntryPtr updatePageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow)
{
	PageTableEntryPtr pPTEntry = NULL;
	PageTableEntryPtr pHashAnchorPTEntry = NULL;

	pPTEntry = searchPageTableEntryAndHashAnchorInIPT(pTLBEntryHigh, &pHashAnchorPTEntry);
	// if PTEntry is already existent
	if (pPTEntry != NULL)
	{
		// update this entry & return
		pPTEntry->tlbEntryHigh = *pTLBEntryHigh;
		// change Permissions of tlbLow...
		pPTEntry->tlbEntryLow.accessPermissions = pTLBEntryLow->accessPermissions;
		pPTEntry->tlbEntryLow.bufferable = pTLBEntryLow->bufferable;
		pPTEntry->tlbEntryLow.cachable = pTLBEntryLow->cachable;
		pPTEntry->tlbEntryLow.global = pTLBEntryLow->global;

		pPTEntry->tlbEntryLow.pageSize = pTLBEntryLow->pageSize;
		pPTEntry->tlbEntryLow.dirty = pTLBEntryLow->dirty;
		pPTEntry->tlbEntryLow.writeThrough = pTLBEntryLow->writeThrough;

		return pPTEntry;
	}
	// if at least a HashAnchor is existent
	else if (pHashAnchorPTEntry != NULL)
	{
		return createPageTableEntry(pTLBEntryHigh, pTLBEntryLow, pHashAnchorPTEntry);
	}
	// if nothing is existent
	return createHashAnchorPageTableEntry(pTLBEntryHigh, pTLBEntryLow);
}

// at the moment only free PageTableEntries are returned ...
// later victim is found via second change alg
// dissovling includes the hash-chain as well as the HashAnchor
PageTableEntryPtr getDissolvedVictimPTEntry(TLBEHIPtr pTLBEntryHigh, int *pIPTIndex)
{
	int32_t index = 0;
	for (index=0; index<MAXPAGETABLEENTRIES; index++)
	{
		if ( isUnusedTLBEntryHigh(&(InvertedPageTable[index].tlbEntryHigh)))
		{
			*pIPTIndex = index;
			return &InvertedPageTable[index];
		}
	}
	return NULL;
}

PageTableEntryPtr createPageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow, PageTableEntryPtr pHashAnchorPTEntry)
{
	int iptIndex;
	PageTableEntryPtr pAfterHashAnchorPTEntry = NULL;

	// get free PTEntry
	PageTableEntryPtr pVictimPTEntry = getDissolvedVictimPTEntry(pTLBEntryHigh, &iptIndex);
	if (pVictimPTEntry == NULL)
		return NULL;

	// initialise new PTEntry
	pVictimPTEntry->tlbEntryHigh = *pTLBEntryHigh;
	// change Permissions
	pVictimPTEntry->tlbEntryLow.accessPermissions = pTLBEntryLow->accessPermissions;
	pVictimPTEntry->tlbEntryLow.bufferable = pTLBEntryLow->bufferable;
	pVictimPTEntry->tlbEntryLow.cachable = pTLBEntryLow->cachable;
	pVictimPTEntry->tlbEntryLow.global = pTLBEntryLow->global;

	pVictimPTEntry->tlbEntryLow.pageSize = pTLBEntryLow->pageSize;
	pVictimPTEntry->tlbEntryLow.dirty = pTLBEntryLow->dirty;
	pVictimPTEntry->tlbEntryLow.writeThrough = pTLBEntryLow->writeThrough;

	// rechain
	pAfterHashAnchorPTEntry = pHashAnchorPTEntry->next;
	pHashAnchorPTEntry->next = pVictimPTEntry;

	if (pAfterHashAnchorPTEntry != NULL)
	{
		pHashAnchorPTEntry->next = pVictimPTEntry;
		pVictimPTEntry->next = pAfterHashAnchorPTEntry;
	}
	return pVictimPTEntry;
}

PageTableEntryPtr createHashAnchorPageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow)
{
	int iptIndex;
	HashAnchorTableEntry hatEntry;

	// get free PTEntry
	PageTableEntryPtr pVictimPTEntry = getDissolvedVictimPTEntry(pTLBEntryHigh, &iptIndex);
	// initialise new PTEntry
	pVictimPTEntry->tlbEntryHigh = *pTLBEntryHigh;
	// change Permissions
	pVictimPTEntry->tlbEntryLow.accessPermissions = pTLBEntryLow->accessPermissions;
	pVictimPTEntry->tlbEntryLow.bufferable = pTLBEntryLow->bufferable;
	pVictimPTEntry->tlbEntryLow.cachable = pTLBEntryLow->cachable;
	pVictimPTEntry->tlbEntryLow.global = pTLBEntryLow->global;

	pVictimPTEntry->tlbEntryLow.pageSize = pTLBEntryLow->pageSize;
	pVictimPTEntry->tlbEntryLow.dirty = pTLBEntryLow->dirty;
	pVictimPTEntry->tlbEntryLow.writeThrough = pTLBEntryLow->writeThrough;

	// actualize HAT
	hatEntry = createHATEntry(iptIndex);
	int hatIndex = getHashAnchorTableIndex(pTLBEntryHigh->ASID, pTLBEntryHigh->VPN);
	HashAnchorTable[hatIndex]=hatEntry;

	return pVictimPTEntry;

}
