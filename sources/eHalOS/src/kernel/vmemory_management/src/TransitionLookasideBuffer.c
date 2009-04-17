/*
 * TransitionLookasideBuffer.c
 *
 *  Created on: Nov 30, 2008
 *      Author: tsi
 */

#include "TransitionLookasideBuffer.h"

//// second chance to skip page out of TLB
//// search an entry which is not accessed
//// and reset the accessed bit of visited entries
//BOOL updateTLB(PageTableEntryPtr pPTEntry)
//{
//	long physical_addr_base;
//	long virtual_addr_base;
//	struct mmu_tlb_t options;
//	int type;
//	static int index = 0;
//	int mmuStatus;
//
//	transformPTEntry(pPTEntry, &type, &physical_addr_base, &virtual_addr_base, &options, &mmuStatus);
//
//	while(TRUE){
//		if (!isAccessed(index, type))
//		{
//			// skip "unused" page and load new page
//			mmuStatus = mmu_invalidate_entry(index, type);
//			if (mmuStatus != MMU_OK )
//			{
//				if (isWiredDown(index, type))
//				{
//					// simply go to next index...
//					index = (index+1) % NROFTLBENTRIES;
//					continue;
//				}
//				else
//				{
//					return FALSE;
//				}
//			}
//			// use actual index as tlb position
//			options.position = index;
//
//			mmuStatus = mmu_add_tlb_entry(physical_addr_base, virtual_addr_base, &options);
//			if (mmuStatus != MMU_OK)
//				return FALSE;
//			return TRUE;
//		}
//		else
//		{
//			// reset accessed flag of tlb entry
//			mmuStatus = setNotAccessed(index, type);
//			if (mmuStatus != MMU_OK && !isWiredDown(index, type))
//				return FALSE;
//			index = (index+1) % NROFTLBENTRIES;
//		}
//
//	}
//
//}

// FIFO to skip page out of TLB
// bepends on inputs of __builtin_clz
BOOL updateTLB(PageTableEntryPtr pPTEntry)
{
	long physical_addr_base;
	long virtual_addr_base;
	struct mmu_tlb_t options;
	int type;
	uint32_t index = 0;
	uint32_t tlbarlo = 0;
	uint32_t firstUnusedTLBE = 0;
	int mmuStatus;

	AVR32_READ_SR_REG(AVR32_TLBARLO, tlbarlo);
	firstUnusedTLBE = __builtin_clz(tlbarlo);

	transformPTEntry(pPTEntry, &type, &physical_addr_base, &virtual_addr_base, &options, &mmuStatus);

	// if TLB is full use first index >> FIFO
	if (firstUnusedTLBE == MMU_DTLB_ENTRIES)
	{
		// set all entries to not accessed, resp. 1
		AVR32_SET_SR_REG(AVR32_TLBARLO, -1L);
	}
	index = firstUnusedTLBE % MMU_DTLB_ENTRIES;
	options.position = index;

	mmuStatus = mmu_add_tlb_entry(physical_addr_base, virtual_addr_base, &options);

	//mmu_dump_tlb();

	if (mmuStatus != MMU_OK)
		return FALSE;
	return TRUE;
}

void transformPTEntry(PageTableEntryPtr pPTEntry, int *pType, long *pPhysical_addr_base, long *pVirtual_addr_base, struct mmu_tlb_t * pOptions, int *mmuStatus)
{
	long index;

	*pType = pPTEntry->tlbEntryHigh.instruction;
	*pVirtual_addr_base = (pPTEntry->tlbEntryHigh.VPN * (PAGESIZE * 1024));
	*pVirtual_addr_base &= 0x00000000FFFFFFFFL; // to clear high word
	*pPhysical_addr_base = (pPTEntry->tlbEntryLow.PFN * (PAGESIZE * 1024));
	*pPhysical_addr_base &= 0x00000000FFFFFFFFL; // to clear high word

	//mmuStatus = mmu_find_index_to_entry(*pPhysical_addr, *pVirtual_addr, &index);
	*mmuStatus = mmu_find_index_to_entry_with_ASID(*pPhysical_addr_base, *pVirtual_addr_base, pPTEntry->tlbEntryHigh.ASID, &index);

	pOptions->access = pPTEntry->tlbEntryLow.accessPermissions;// isAccessed(index,*pType);
	pOptions->asid = pPTEntry->tlbEntryHigh.ASID;
	pOptions->bufferable = pPTEntry->tlbEntryLow.bufferable;
	pOptions->cachable = pPTEntry->tlbEntryLow.cachable;
	pOptions->dirty = pPTEntry->tlbEntryLow.dirty;
	pOptions->global = pPTEntry->tlbEntryLow.global;
	pOptions->page_size = pPTEntry->tlbEntryLow.pageSize;
	pOptions->position = index; // TODO: ???
	pOptions->type = *pType;
	pOptions->valid = pPTEntry->tlbEntryHigh.valid;
	pOptions->write_through = pPTEntry->tlbEntryLow.writeThrough;

}


BOOL isWiredDown(int index,int type)
{
	// TODO: access instruction TLB lockdown amount
	return FALSE;
}

BOOL isAccessed(int index,int type)
{
	// TODO: access TLB access register, S. 37
	//TLBARHI & TLBARLO - HI is not used?
	long regValue;

	if (index < 32)
	{
		AVR32_READ_SR_REG(AVR32_TLBARLO, regValue);
		// search access bit

		return !(regValue & (1 << index));
		//return (regValue & (1 << index));   // Test Logik umdrehen ??
	}

	AVR32_READ_SR_REG(AVR32_TLBARHI, regValue);
	// search access bit
	return !(regValue & (1 << (index-32)));
}

int setNotAccessed(int index, int type)
{
	// TODO: change TLB access register, S. 37

	if (index < 32)
	{
		//AVR32_CLEAR_SR_BIT(AVR32_TLBARLO, index);
		//AVR32_TOGGLE_SR_BIT(AVR32_TLBARLO, index); ==> falsch

		AVR32_SET_SR_BIT(AVR32_TLBARLO, 1<<index);
		//AVR32_CLEAR_SR_BIT(AVR32_TLBARLO, 1<<index); // Test Logik umdrehen ??

	}
	else
	{
		//AVR32_CLEAR_SR_BIT(AVR32_TLBARHI, index-32);
		//AVR32_TOGGLE_SR_BIT(AVR32_TLBARHI, index-32); ==> falsch

		AVR32_SET_SR_BIT(AVR32_TLBARHI, ( 1<<(index-32) ));
	}

	return MMU_OK;
}

BOOL InvalidateTLBEntry(PageTableEntryPtr pPTEntry){
	long index=0;

	int type;
	long physical_addr_base;
	long virtual_addr_base;
	struct mmu_tlb_t options;
	int mmuStatus;

	int succ;

	if (pPTEntry == NULL)
		return FALSE;

	transformPTEntry(pPTEntry, &type, &physical_addr_base, &virtual_addr_base, &options, &mmuStatus);

	succ = mmu_find_index_to_entry_with_ASID(physical_addr_base, virtual_addr_base, pPTEntry->tlbEntryHigh.ASID, &index);

	if (succ == FALSE)
		return FALSE;

	if (index < 0)
		return FALSE;
	if (index > MMU_DTLB_ENTRIES)
		return FALSE;

	if (mmu_invalidate_entry(index, type) != MMU_OK)
		return FALSE;

	return TRUE;

}

BOOL InvalidateTLB(){

	// QUICK HACK...
	int index=0;
	for (index=0; index<MMU_DTLB_ENTRIES; index++)
		mmu_invalidate_entry(index, MMU_TLB_TYPE_DATA);

	return TRUE;

}
