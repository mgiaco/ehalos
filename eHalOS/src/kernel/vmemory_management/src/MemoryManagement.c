/*
 * MemoryManagement.c
 *
 *  Created on: Dec 4, 2008
 *      Author: tsi
 */

#include "mmu.h"
#include "mmu_exceptions.h"
#include "macro.h"

#include "MemoryManagement.h"

int initMemoryManagement()
{
	// init in memory before MMU is on...
	initPageTable();

	initProcessRegionTables();

	// TODO: should be done by ProcessManagement
	// PTBR
	AVR32_SET_SR_REG(AVR32_PTBR,InvertedPageTable);



	//init_mmu_segon_pageon:

	int retval = MMU_OK;
//	char * string_p = "MMU test segmentation on, paging on";
//	long string_p64 = (long) string_p | 0x64080000;
//	long string_pPA = 0;
//	long string_pVA = 0;
//
//	printf("\r\nDemonstrate MMU mode...\r\n");
//	printf("  Segmentation: ON\r\n");
//	printf("  Paging      : ON\r\n\n");
//
//	/* Reset MMU */
//	printf("  Reset MMU...               ");

	retval = mmu_reset();

	if (retval != MMU_OK) {
		//printf("[ FAILED ]\r\n");
		goto error;
	}

	//printf("[ OK ]\r\n");

	/* Set shared mode */
	//printf("  Set MMU in shared mode...  ");

//	retval = mmu_init(MMU_DISABLE, MMU_SHARED_VIRTUAL_MODE, MMU_SEGMENTATION_ON);
//
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	printf("[ OK ]\r\n");
//
//	struct mmu_tlb_t tlbOptions;
//	tlbOptions.valid = MMU_ENABLE;
//	tlbOptions.type = MMU_TLB_TYPE_DATA;
//	tlbOptions.asid = 0;
//	tlbOptions.cachable = MMU_DISABLE;
//	tlbOptions.global = MMU_DISABLE;
//	tlbOptions.bufferable = MMU_DISABLE;
//	tlbOptions.access = 0x03; /* read/write/execute */
//	tlbOptions.page_size = MMU_PAGE_SIZE_64KB;
//	tlbOptions.dirty = MMU_ENABLE;
//	tlbOptions.write_through = MMU_DISABLE;
//	tlbOptions.position = -1;
//
//	/* Add the segments to TLB */
//	printf("  Adding segments to TLB...  ");
//	tlbOptions.position = 12;
//	retval = mmu_add_tlb_entry(0, 0, &tlbOptions);
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	tlbOptions.position = 14;
//	retval = mmu_add_tlb_entry(0x24000000, 0x24000000, &tlbOptions);
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//
//	/* Add a memory address which maps to string_p */
//	tlbOptions.position = 11;
//	retval = mmu_add_tlb_entry((long)string_p, 0x64080000, &tlbOptions);
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	printf("[ OK ]\r\n");
//
//	/* Change mode */
//	printf("  Changing mode...           ");


	//MMU_ENABLE ==> Paging on
	retval = mmu_init(MMU_ENABLE, MMU_PRIVATE_VIRTUAL_MODE, MMU_SEGMENTATION_ON);

	if (retval != MMU_OK) {
		//printf("[ FAILED ]\r\n");
		goto error;
	}

	//printf("[ OK ]\r\n");

	/* Search and get the physical
	 * address to the pointer
	 */
//	printf("  Lookup physical address... ");
//	retval = mmu_search_virtual_ptr((void *)string_p64, &string_pPA);
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	printf("[ OK ]\r\n");

	/* Search and get the virtual
	 * address to the pointer
	 */
//	printf("  Lookup virtual address...  ");
//	retval = mmu_search_physical_ptr((void *)string_pPA, &string_pVA);
//	if (retval != MMU_OK) {
//		printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	printf("[ OK ]\r\n");
//
//	/* Find the index for the TLB entry */
//	long tlbIndex;
//	printf("  Get TLB index... ");
//	retval = mmu_find_index_to_entry(string_pPA, string_pVA, &tlbIndex);
//	if (retval != MMU_OK) {
//		printf("          [ FAILED ]\r\n");
//		goto error;
//	}
//	printf("[%02ld]      [ OK ]\r\n", tlbIndex);
//
//	printf("\r\n");
//	printf("  String   (0x%08lx): %s\r\n", (long)string_p, string_p);
//	printf("  String64 (0x%08lx): %s\r\n", (long)string_p64, (char *)string_p64);
//	printf("  StringPA (0x%08lx): %s\r\n", (long)string_pPA, (char *)string_pPA);
//	printf("  StringVA (0x%08lx): %s\r\n", (long)string_pVA, (char *)string_pVA);
//	printf("\r\n");
//	printf("  Test result...             ");
//
//	if (strcmp(string_p, (char *)string_pPA) == 0 &&
//			strcmp(string_p, (char *)string_pVA) == 0) {
//		printf("[ OK ]\r\n\n");
//	} else {
//		printf("[ FAILED ]\r\n\n");
//	}


	init_mmu_exceptions();

	/* Tests by mgiaco */
/*
	int tlbarlo;
	AVR32_READ_SR_REG(AVR32_TLBARLO, tlbarlo);

	int count_leading_zero = __builtin_clz(tlbarlo);

	struct mmu_tlb_t tlbOptions;
	tlbOptions.valid = MMU_ENABLE;
	tlbOptions.type = MMU_TLB_TYPE_DATA;
	tlbOptions.asid = 1;
	tlbOptions.cachable = MMU_DISABLE;
	tlbOptions.global = MMU_DISABLE;
	tlbOptions.bufferable = MMU_DISABLE;
	tlbOptions.access = 7; // read/write/execute
	tlbOptions.page_size = MMU_PAGE_SIZE_64KB;
	tlbOptions.dirty = MMU_ENABLE;
	tlbOptions.write_through = MMU_DISABLE;
	tlbOptions.position = -1;

	// Add the segments to TLB
	tlbOptions.position = count_leading_zero;

	retval = mmu_add_tlb_entry(0x10500000, 0x00000000 , &tlbOptions);
	if (retval != MMU_OK) {
		while(1){}
	}

	AVR32_READ_SR_REG(AVR32_TLBARLO, tlbarlo);
	count_leading_zero = __builtin_clz(tlbarlo);


	tlbOptions.valid = MMU_ENABLE;
	tlbOptions.type = MMU_TLB_TYPE_DATA;
	tlbOptions.asid = 2;
	tlbOptions.cachable = MMU_DISABLE;
	tlbOptions.global = MMU_DISABLE;
	tlbOptions.bufferable = MMU_DISABLE;
	tlbOptions.access = 7; // read/write/execute
	tlbOptions.page_size = MMU_PAGE_SIZE_64KB;
	tlbOptions.dirty = MMU_ENABLE;
	tlbOptions.write_through = MMU_DISABLE;
	tlbOptions.position = -1;

	// Add the segments to TLB
	tlbOptions.position = count_leading_zero;
	retval = mmu_add_tlb_entry(0x10510000, 0x00000000 , &tlbOptions);
	if (retval != MMU_OK) {
		while(1){}
	}

	AVR32_READ_SR_REG(AVR32_TLBARLO, tlbarlo);
	count_leading_zero = __builtin_clz(tlbarlo);


	tlbOptions.valid = MMU_ENABLE;
	tlbOptions.type = MMU_TLB_TYPE_DATA;
	tlbOptions.asid = 3;
	tlbOptions.cachable = MMU_DISABLE;
	tlbOptions.global = MMU_DISABLE;
	tlbOptions.bufferable = MMU_DISABLE;
	tlbOptions.access = 7; // read/write/execute
	tlbOptions.page_size = MMU_PAGE_SIZE_64KB;
	tlbOptions.dirty = MMU_ENABLE;
	tlbOptions.write_through = MMU_DISABLE;
	tlbOptions.position = -1;

	// Add the segments to TLB
	tlbOptions.position = count_leading_zero;
	retval = mmu_add_tlb_entry(0x10520000, 0x00000000 , &tlbOptions);
	if (retval != MMU_OK) {
		while(1){}
	}


	AVR32_READ_SR_REG(AVR32_TLBARLO, tlbarlo);
	count_leading_zero = __builtin_clz(tlbarlo);





	return retval;
*/

	//init_all_drivers();
	//mmu_dump_tlb();

error:
	return retval;
}

void switchMemoryManagementToAppLoadMode()
{
	return;
}

void switchMemoryManagementToAppRunMode()
{
	return;
}


#define MMU_CONTEXT_ASID_MASK           0x000000ff
#define cpu_sync_pipeline()     asm volatile("sub pc, -2" : : : "memory")


void actualiseASID(uint32_t pid)
{

// vom linux
//	 static inline void set_asid(unsigned long asid)
//	 {
//	         /* XXX: We're destroying TLBEHI[8:31] */
//			sysreg_write(TLBEHI, asid & MMU_CONTEXT_ASID_MASK);
//	         cpu_sync_pipeline();
//	}


//	AVR32_SET_SR_REG(AVR32_TLBEHI, pid & MMU_CONTEXT_ASID_MASK);



	// set ASID of current Process in TLBHI
	uint32_t tlbehi;

	AVR32_READ_SR_REG(AVR32_TLBEHI, tlbehi);
	tlbehi &= ~( (uint32_t)AVR32_TLBEHI_ASID_MASK << AVR32_TLBEHI_ASID_OFFSET);
	tlbehi |= (pid << AVR32_TLBEHI_ASID_OFFSET);
	AVR32_SET_SR_REG(AVR32_TLBEHI, tlbehi);

	// TODO: gehört hinein.
	cpu_sync_pipeline();

}


BOOL loadProcessInMemorySystem(int ASID, BinImagePtr pBinImage)
{
	BOOL autoUnload;
	BOOL succ1;
	BOOL succ2;
	ProcessRegionTablePtr pProcessRegionTable;

	pProcessRegionTable = reuseProcessRegionTable(ASID, &autoUnload);
	succ1 = addBinImageRegionToRamPager(ASID, pBinImage);

	succ2 = loadBinApplication(pBinImage, ASID);

	if (pProcessRegionTable != NULL && succ1 == TRUE && succ2 == TRUE)
		return TRUE;

	return FALSE;
}

BOOL unloadProcessFromMemorySystem(int ASID)
{
	return unloadProcessRegionTable(ASID);
}



