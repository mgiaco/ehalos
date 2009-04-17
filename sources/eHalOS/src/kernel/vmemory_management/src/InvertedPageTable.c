#include "InvertedPageTable.h"

// ---

PageTableEntry InvertedPageTable[MAXPAGETABLEENTRIES];

// ---

void initInvertedPageTable()
{
	uint32_t i=0;
	for(i=0;i<MAXPAGETABLEENTRIES; i++)
	{
		InvertedPageTable[i].tlbEntryHigh = IllegalTLBEntryHigh;
		InvertedPageTable[i].tlbEntryLow.PFN = i + PFNOFFSET;
	}
}

// ---

PageTableEntryPtr searchPageTableEntryAndHashAnchorAndPreviousInIPT(TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hHashAnchorPTEntry, PageTableEntryHdl hPrevPTEntry)
{
	int32_t hatIndex;
	HashAnchorTableEntry hatEntry;

	// determine HATIndex and HATEntry to start search
	// -----------------------------------------------
	// determine hash of VPN & ASID for HAT-Entry
	// cannot be an illegal index
	hatIndex = getHashAnchorTableIndex(pTLBEntryHigh->ASID, pTLBEntryHigh->VPN);

	// get HAT IPT-Entry to which HAT-Entry points
	// if illegal hatEntry search will return NULL
	hatEntry = getHATEntry(hatIndex);

	if (isIllegalHATEntry(hatEntry))
		return NULL;

	int iptIndex = getIPTIndex(hatEntry);

	if (isUnusedTLBEntryHigh( &(InvertedPageTable[iptIndex].tlbEntryHigh) ) )//(IsIllegalPTEntry(InvertedPageTable[iptIndex]))
		return NULL;

	// at least a valid HashAnchorPTEntry exists
	*hHashAnchorPTEntry = &(InvertedPageTable[iptIndex]);

	return searchPageTableEntryAndPreviousInIPT(pTLBEntryHigh, hatEntry, hPrevPTEntry);
}

PageTableEntryPtr searchPageTableEntryAndHashAnchorInIPT(TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hHashAnchorPTEntry)
{
	int32_t hatIndex;
	HashAnchorTableEntry hatEntry;

	// determine HATIndex and HATEntry to start search
	// -----------------------------------------------
	// determine hash of VPN & ASID for HAT-Entry
	// cannot be an illegal index
	hatIndex = getHashAnchorTableIndex(pTLBEntryHigh->ASID, pTLBEntryHigh->VPN);

	// get HAT IPT-Entry to which HAT-Entry points
	// if illegal hatEntry search will return NULL
	hatEntry = getHATEntry(hatIndex);

	if (isIllegalHATEntry(hatEntry))
		return NULL;

	int iptIndex = getIPTIndex(hatEntry);

	if (isUnusedTLBEntryHigh( &(InvertedPageTable[iptIndex].tlbEntryHigh) ) )//(IsIllegalPTEntry(InvertedPageTable[iptIndex]))
		return NULL;

	// at least a valid HashAnchorPTEntry exists
	*hHashAnchorPTEntry = &(InvertedPageTable[iptIndex]);

	return searchPageTableEntryInIPT(pTLBEntryHigh, hatEntry);
}

PageTableEntryPtr searchPageTableEntryAndPreviousInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry, PageTableEntryHdl hPrevPTEntry)
{
	int iptIndex = getIPTIndex(hatEntry);
	if (InvertedPageTable[iptIndex].tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			InvertedPageTable[iptIndex].tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return &(InvertedPageTable[iptIndex]);

	if (InvertedPageTable[iptIndex].next == NULL)
		return NULL;

	*hPrevPTEntry = &(InvertedPageTable[iptIndex]);

	return searchPageTableEntryAndPreviousInPTE(InvertedPageTable[iptIndex].next, pTLBEntryHigh, hPrevPTEntry);
}


PageTableEntryPtr searchPageTableEntryInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry)
{
	int iptIndex = getIPTIndex(hatEntry);
	if (InvertedPageTable[iptIndex].tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			InvertedPageTable[iptIndex].tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return &(InvertedPageTable[iptIndex]);

	if (InvertedPageTable[iptIndex].next == NULL)
		return NULL;

	return searchPageTableEntryInPTE(InvertedPageTable[iptIndex].next, pTLBEntryHigh);
}

PageTableEntryPtr searchPageTableEntryAndPreviousInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hPrevPTEntry)
{
	if (pPTEntry->tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			pPTEntry->tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return pPTEntry;

	if (pPTEntry->next == NULL)
		return NULL;

	*hPrevPTEntry = pPTEntry;

	return searchPageTableEntryAndPreviousInPTE(pPTEntry->next, pTLBEntryHigh, hPrevPTEntry);
}

PageTableEntryPtr searchPageTableEntryInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh)
{
	if (pPTEntry->tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			pPTEntry->tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return pPTEntry;

	if (pPTEntry->next == NULL)
		return NULL;

	return searchPageTableEntryInPTE(pPTEntry->next, pTLBEntryHigh);
}

PageTableEntryPtr lastPageTableEntryOfPTEChain(PageTableEntryPtr pPTEntry)
{
	if (pPTEntry->next == NULL)
		return pPTEntry;

	return lastPageTableEntryOfPTEChain(pPTEntry->next);
}


// ---

TLBELOPtr searchPageTableEntryLowInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry)
{
	if (isIllegalHATEntry(hatEntry))
		return NULL;

	int iptIndex = getIPTIndex(hatEntry);
	if (InvertedPageTable[iptIndex].tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			InvertedPageTable[iptIndex].tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return &(InvertedPageTable[iptIndex].tlbEntryLow);

	if (InvertedPageTable[iptIndex].next == NULL)
		return NULL;

	return searchPageTableEntryLowInPTE(InvertedPageTable[iptIndex].next, pTLBEntryHigh);
}

TLBELOPtr searchPageTableEntryLowInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh)
{
	if (pPTEntry->tlbEntryHigh.ASID == pTLBEntryHigh->ASID &&
			pPTEntry->tlbEntryHigh.VPN == pTLBEntryHigh->VPN )
			return &(pPTEntry->tlbEntryLow);

	if (pPTEntry->next == NULL)
		return NULL;

	return searchPageTableEntryLowInPTE(pPTEntry->next, pTLBEntryHigh);
}

int32_t getIPTIndex(HashAnchorTableEntry hatEntry)
{
	return (int32_t)hatEntry;// + PFNOFFSET;
}

// ---


