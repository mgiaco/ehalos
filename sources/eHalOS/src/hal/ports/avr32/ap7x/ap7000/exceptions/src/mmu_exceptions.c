#include <stdint.h>
#include "exceptions.h"

#include <string.h>

#include "mmu.h"
#include "macro.h"
#include "PageTable.h"
#include "TransitionLookasideBuffer.h"


#include "halos.h"

static unsigned int mmu_exception(int evba_offset, int return_address);

BOOL isAppLoadMode(int ASID);

void init_mmu_exceptions(void){

	_register_exception_handler(&mmu_exception, EVBA_ITLB_MISS);
	_register_exception_handler(&mmu_exception, EVBA_DTLB_MISS_R);
	_register_exception_handler(&mmu_exception, EVBA_DTLB_MISS_W);
	_register_exception_handler(&mmu_exception, EVBA_DTLB_MODIFIED);
	_register_exception_handler(&mmu_exception, EVBA_TLB_MULTIPLE);

}


/*! \brief Exception handler for MMU exceptions.
 *
 *  \param evba_offset the offset vektor relevant to the EVBA address
 *  \param return_address memory pointer to where the exception handler
 *                        should return after executing
 */
static unsigned int mmu_exception(int evba_offset, int return_address){
	int retval;
	long bear;
	long tlbhi;
	long pfn;

	long physical_addr_base;
	long virtual_addr_base;
	struct mmu_tlb_t options;
	int type;
	int mmuStatus;


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

	TLBEHI tlbEntryHigh;
	PageTableEntryPtr pPTEntry;
	BOOL result;


	int ASID = activeASID;


	TLBELOPFNLESS tlbEntryLow;

	//printf("\r\n* Handling exception: ");


	switch (evba_offset) {
	case EVBA_ITLB_MISS:

		// get VPN to read PageTableEntry out of PageTable with actual ASID
		mmu_read_bear_and_tlbehi(&bear, &tlbhi);
		// with VPN and ASID search TLBLowEntry
		// CHB.B.: 15.01. change dependend o PageSize
		//tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi
		switch(PAGESIZE)
		{
			case 1:
				tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi; bit 31:10
				break;
			case 4:
				tlbEntryHigh.VPN = tlbhi >> 12; // to get  VPN out of tlbhi; bit 31:12
				break;
			case 64:
				tlbEntryHigh.VPN = tlbhi >> 16; // to get  VPN out of tlbhi; bit 31:16
				break;
			case 1024:
				tlbEntryHigh.VPN = tlbhi >> 20; // to get  VPN out of tlbhi; bit 31:20
				break;
		}
		tlbEntryHigh.ASID = ASID; // TODO: get from ProcessManagement

		pPTEntry = searchPageTableEntry(&tlbEntryHigh);
		result = updateTLB(pPTEntry);


		break;

	case EVBA_DTLB_MISS_W:

		// get VPN to read PageTableEntry out of PageTable with actual ASID
		mmu_read_bear_and_tlbehi(&bear, &tlbhi);
		// with VPN and ASID search TLBLowEntry
		// CHB.B.: 15.01. change dependend o PageSize
		//tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi
		switch(PAGESIZE)
		{
			case 1:
				tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi; bit 31:10
				break;
			case 4:
				tlbEntryHigh.VPN = tlbhi >> 12; // to get  VPN out of tlbhi; bit 31:12
				break;
			case 64:
				tlbEntryHigh.VPN = tlbhi >> 16; // to get  VPN out of tlbhi; bit 31:16
				break;
			case 1024:
				tlbEntryHigh.VPN = tlbhi >> 20; // to get  VPN out of tlbhi; bit 31:20
				break;
		}
		tlbEntryHigh.ASID = ASID; // TODO: get from ProcessManagement

		if (isAppLoadMode(ASID)) // TODO: should be in ProcessManager
		{

			pPTEntry = searchPageTableEntry(&tlbEntryHigh);
		}
		else
		{
			// ASSUMPTION: every write access is correct
			// make new Page Table entry
			tlbEntryHigh.instruction = FALSE;
			tlbEntryHigh.valid = TRUE;
			tlbEntryLow.accessPermissions = 7; // everything is allowed...
			tlbEntryLow.bufferable = FALSE;
			tlbEntryLow.cachable = FALSE;
			tlbEntryLow.dirty = FALSE;
			tlbEntryLow.global = FALSE;
			tlbEntryLow.pageSize = CODEDPAGESIZE;
			tlbEntryLow.writeThrough = FALSE; // ???

			pPTEntry = updatePageTableEntry(&tlbEntryHigh, &tlbEntryLow);
		}
		// enter tlb entries in TLB
		// ASSUMPTION: ALL misses are valid
		result = updateTLB(pPTEntry);

		break;

	case EVBA_DTLB_MODIFIED:

//		1. Examine the TLBEAR and TLBEHI registers in order to identify the page that caused
//		the fault. Use this to index the page table pointed to by PTBR and fetch the desired
//		page table entry.
//		2. Set the Dirty bit in the read page table entry and write this entry back to the page table
//		3. Use the fetched page table entry to update the necessary bits in TLBEHI and TLBELO.
//		The following bits must be updated: V, PFN, C, G, B, AP[2:0], SZ[1:0], W, D.
//		4. The TLBEHI[I] register is cleared by hardware to indicate that it was a data access, and
//		MMUCR[DRP] is updated to point to the matching TLB entry.
//		5. Execute the tlbw instruction in order to update the TLB entry.
//		6. Finish the exception handling and return to the application by executing the rete
//		instruction.

		//1.
		mmu_read_bear_and_tlbehi(&bear, &tlbhi);
		// with VPN and ASID search TLBLowEntry
		// CHB.B.: 15.01. change dependend o PageSize
		//tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi
		switch(PAGESIZE)
		{
			case 1:
				tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi; bit 31:10
				break;
			case 4:
				tlbEntryHigh.VPN = tlbhi >> 12; // to get  VPN out of tlbhi; bit 31:12
				break;
			case 64:
				tlbEntryHigh.VPN = tlbhi >> 16; // to get  VPN out of tlbhi; bit 31:16
				break;
			case 1024:
				tlbEntryHigh.VPN = tlbhi >> 20; // to get  VPN out of tlbhi; bit 31:20
				break;
		}
		tlbEntryHigh.ASID = ASID; // TODO: get from ProcessManagement
		pPTEntry = searchPageTableEntry(&tlbEntryHigh);
		// 2.
		pPTEntry->tlbEntryLow.dirty = TRUE;
		// 3.,4.,5.
		transformPTEntry(pPTEntry, &type, &physical_addr_base, &virtual_addr_base, &options, &mmuStatus);
		mmuStatus = mmu_add_tlb_entry(physical_addr_base, virtual_addr_base, &options);
		// update: V, PFN, C, G, B, AP[2:0], SZ[1:0], W, D via pPTEntry
		//6.
		break;


	case EVBA_TLB_MULTIPLE:


		while(1){}


		break;


	case EVBA_DTLB_MISS_R:

		// get VPN to read PageTableEntry out of PageTable with actual ASID
		mmu_read_bear_and_tlbehi(&bear, &tlbhi);
		// with VPN and ASID search TLBLowEntry
		// CHB.B.: 15.01. change dependend o PageSize
		//tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi
		switch(PAGESIZE)
		{
			case 1:
				tlbEntryHigh.VPN = tlbhi >> 10; // to get  VPN out of tlbhi; bit 31:10
				break;
			case 4:
				tlbEntryHigh.VPN = tlbhi >> 12; // to get  VPN out of tlbhi; bit 31:12
				break;
			case 64:
				tlbEntryHigh.VPN = tlbhi >> 16; // to get  VPN out of tlbhi; bit 31:16
				break;
			case 1024:
				tlbEntryHigh.VPN = tlbhi >> 20; // to get  VPN out of tlbhi; bit 31:20
				break;
		}
		tlbEntryHigh.ASID = ASID; // TODO: get from ProcessManagement

//		if (isAppLoadMode(ASID)) // TODO: should be in ProcessManager
//		{
//
//			pPTEntry = searchPageTableEntry(&tlbEntryHigh);
//		}
//		else
//		{
			// ASSUMPTION: every read access is correct
			// make new Page Table entry
			tlbEntryHigh.instruction = FALSE;
			tlbEntryHigh.valid = TRUE;
			tlbEntryLow.accessPermissions = 7; // everything is allowed...
			tlbEntryLow.bufferable = FALSE;
			tlbEntryLow.cachable = FALSE;
			tlbEntryLow.dirty = FALSE;
			tlbEntryLow.global = FALSE;
			tlbEntryLow.pageSize = CODEDPAGESIZE;
			tlbEntryLow.writeThrough = FALSE; // ???

			pPTEntry = updatePageTableEntry(&tlbEntryHigh, &tlbEntryLow);
//		}
		// enter tlb entries in TLB
		// ASSUMPTION: ALL misses are valid
		result = updateTLB(pPTEntry);
		break;

//		mmu_read_bear_and_tlbehi(&bear, &vpn);
		//printf("TLB miss at virtual address 0x%08lx\r\n", bear);
		//printf("  Bear is 0x%08lx\r\n", bear);
		//printf("  Vpn  is 0x%08lx\r\n", vpn);

		/* Add the page to TLB */
//		if (free_page_nr > MMU_DTLB_ENTRIES) {
//			free_page_nr = 16;
//		}

//		tlbOptions.position = free_page_nr++;
		//printf("  Adding page to TLB[%02d]...  ", tlbOptions.position);
		/* in SRAM segment */
//		if (((bear ^ 0x24000000) >> 24) == 0) {
//			pfn = 0x24000000;
//		} else {
//			pfn = 0;
//		}
//		retval = mmu_add_tlb_entry(pfn, bear, &tlbOptions);
//		if (retval != MMU_OK) {
//			//printf("[ FAILED ]\r\n");
//			goto out;
//		}
//		//printf("[ OK ]\r\n");
//		break;
	default:
		break;
		//printf("unhandled exception 0x%08x!\r\n", evba_offset);
	}

out:
	//printf("\n");
	return return_address;
}





///*! \brief Use MMU in segmentet mode and with paging to generate a exception
// *
// *  \return Status or error code
// *    \retval MMU_OK on success
// */
//int mmuex_exceptions()
//{
//	int retval = MMU_OK;
//	char * string_p = "this test string is used to generate exceptions";
//	char * string_p61 = (char *)((long) string_p | 0x61800000);
//
//	//printf("\r\nDemonstrate MMU mode...\r\n");
//	//printf("  Segmentation: ON\r\n");
//	//printf("  Paging      : ON\r\n");
//	//printf("  Exceptions  : ON\r\n\n");
//
//	/* Reset MMU */
//	//printf("  Reset MMU...               ");
//	retval = mmu_reset();
//	if (retval != MMU_OK) {
//		//printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	//printf("[ OK ]\r\n");
//
//	/* Set shared mode */
//	//printf("  Set MMU in shared mode...  ");
//	retval = mmu_init(MMU_DISABLE,
//			MMU_SHARED_VIRTUAL_MODE,
//			MMU_SEGMENTATION_ON);
//	if (retval != MMU_OK) {
//		//printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	//printf("[ OK ]\r\n");
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
//
//	/* Inset NOR flash at address 0 */
//	//printf("  Adding segments to TLB...  ");
//	tlbOptions.position = 12;
//	retval = mmu_add_tlb_entry(0, 0, &tlbOptions);
//	if (retval != MMU_OK) {
//		//printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	/* Insert the internal SRAM */
//	tlbOptions.position = 14;
//	retval = mmu_add_tlb_entry(0x24000000, 0x24000000, &tlbOptions);
//	if (retval != MMU_OK) {
//		//printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	//printf("[ OK ]\r\n");
//
//	/* Change mode */
//	//printf("  Changing mode...           ");
//	retval = mmu_init(MMU_ENABLE,
//			MMU_SHARED_VIRTUAL_MODE,
//			MMU_SEGMENTATION_ON);
//	if (retval != MMU_OK) {
//		//printf("[ FAILED ]\r\n");
//		goto error;
//	}
//	//printf("[ OK ]\r\n");
//
//	//printf("\r\n");
//	//printf("  String   (0x%08lx): %s\r\n", (long)string_p, string_p);
//	/* Use the pointer in the 0x6180xxxx segment without adding the page.
//	 * This will generate an exception */
//	strcpy(string_p61, string_p);
//
//	/* Print out the string which should now be available */
//	//printf("  String61 (0x%08lx): %s\r\n", (long)string_p61, string_p61);
//
//	//printf("\r\n");
//	//printf("  Test result...             ");
//
//	if (strcmp(string_p, string_p61) == 0) {
//		//printf("[ OK ]\r\n\n");
//	} else {
//		//printf("[ FAILED ]\r\n\n");
//	}
//
//	return retval;
//error:
//	//printf("\r\n");
//	//printf("  Test result... (jumped)    [ FAILED ]\r\n\n");
//	return retval;
//}

BOOL isAppLoadMode(int ASID)
{
	return TRUE; // TODO: should be done in ProcessManager...
}

