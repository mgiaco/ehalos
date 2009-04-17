#include "PageTableEntry.h"

// ---
TLBEHI IllegalTLBEntryHigh = {
	ILLEGALVPN, FALSE, FALSE, ILLEGALASID
};

TLBELO IllegalTLBEntryLow = {
	-1,
	FALSE,
	FALSE,
	FALSE,
	0, // TODO: may redefine...
	CODEDPAGESIZE,
	0,
	0
};

PageTableEntry IllegalPTEntry = {
		{
			ILLEGALVPN,
			FALSE,
			FALSE,
			ILLEGALASID
		},
		{
			-1,
			FALSE,
			FALSE,
			FALSE,
			0,
			CODEDPAGESIZE,
			0,
			0
		},
		NULL
};
// ---

BOOL equalTLBEntryLow(TLBELOPtr pEntry1, TLBELOPtr pEntry2)
{
	if (pEntry1->PFN != pEntry2->PFN)
		return FALSE;
	if (pEntry1->accessPermissions != pEntry2->accessPermissions)
		return FALSE;
	if (pEntry1->bufferable != pEntry2->bufferable)
		return FALSE;
	if(pEntry1->cachable != pEntry2->cachable)
		return FALSE;
	if (pEntry1->global != pEntry2->global)
		return FALSE;

	if (pEntry1->pageSize != pEntry2->pageSize)
		return FALSE;
	if (pEntry1->dirty != pEntry2->dirty)
		return FALSE;
	if (pEntry1->writeThrough != pEntry2->writeThrough)
		return FALSE;

	return TRUE;
}

BOOL equalTLBEntryHigh(TLBEHIPtr pEntry1, TLBEHIPtr pEntry2)
{
	if (pEntry1->ASID != pEntry2->ASID)
		return FALSE;
	if (pEntry1->VPN != pEntry2->VPN)
		return FALSE;
	if (pEntry1->instruction != pEntry2->instruction)
		return FALSE;
	if (pEntry1->valid != pEntry2->valid)
		return FALSE;

	return TRUE;
}

BOOL equal_PFN_TLBEntryLow(TLBELOPtr pEntry1, TLBELOPtr pEntry2)
{
	if (pEntry1->PFN == pEntry2->PFN)
			return TRUE;

	return FALSE;
}

BOOL equal_ASIDVPN_TLBEntryHigh(TLBEHIPtr pEntry1, TLBEHIPtr pEntry2)
{
	if (pEntry1->VPN == pEntry2->VPN &&
		pEntry1->ASID == pEntry2->ASID)
			return TRUE;

	return FALSE;
}

BOOL equalTLBEntryLowPFNLess(TLBELOPtr pEntry1, TLBELOPFNLESSPtr pEntry2)
{
	if (pEntry1->accessPermissions != pEntry2->accessPermissions)
		return FALSE;
	if (pEntry1->bufferable != pEntry2->bufferable)
		return FALSE;
	if(pEntry1->cachable != pEntry2->cachable)
		return FALSE;
	if (pEntry1->global != pEntry2->global)
		return FALSE;

	if (pEntry1->pageSize != pEntry2->pageSize)
		return FALSE;
	if (pEntry1->dirty != pEntry2->dirty)
		return FALSE;
	if (pEntry1->writeThrough != pEntry2->writeThrough)
		return FALSE;

	return TRUE;
}

BOOL equalTLBEntryHighVPNLess(TLBEHIPtr pEntry1, TLBEHIVNPLESSPtr pEntry2)
{
	if (pEntry1->ASID != pEntry2->ASID)
		return FALSE;
	if (pEntry1->instruction != pEntry2->instruction)
		return FALSE;
	if (pEntry1->valid != pEntry2->valid)
		return FALSE;

	return TRUE;
}

BOOL isUnusedTLBEntryHigh(TLBEHIPtr pEntry)
{
	return equal_ASIDVPN_TLBEntryHigh(pEntry, &IllegalTLBEntryHigh);
}


