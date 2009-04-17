/*
 * Loader.c
 *
 *  Created on: Dec 4, 2008
 *      Author: tsi
 */

#include "Loader.h"


/** Load raw process image into memory.
 *  Stack and heap allocation is done by application.
 *  Problem: intentionally causes page faults.
 *  How to separate from illegal memory-access?
 *  Done by state of process.
 *  At the moment loading copies complete process image.
 *  That means that the right number of pages are reserved
 *  to appropriate process image size and copying is done afterwords.
 */
BOOL loadBinApplication(BinImagePtr pBinImage, int ASID)
{
	// simply writes data into ram

	// because Page Table Entries are now (maybe) of type data
	// reassign them to type instruction
	// To handle Page-Faults differently,
	//forces interrupts other than mmu to be ignored...

	// switchMemoryManagementToAppLoadMode(); TODO: done outside...

	if (!reservePagesForBinImage(pBinImage, ASID))
		return FALSE;

	if (!writeBinImageToVirtualRam(pBinImage, ASID)){
		// TODO: free allocated Pages
		// switchMemoryManagementToAppRunMode(); TODO: done outside...
		return FALSE;
	}

	// switchMemoryManagementToAppRunMode(); TODO: done outside ...
	return TRUE;
}
/*
BOOL loadIHexApplication(IHexImagePtr pIHexImage)
{
	// parse iHex and write it into memory
	// variant a.) MMU is on
	// variant b.) MMU is off

	return TRUE;
}
*/

/** Starts raw binary application.
 * BinImage is nec if e.g. startaddress is not 0x0000
 */
void startBinApplication(BinImagePtr pBinImage, int ASID)
{
	// TODO: set application PC to 0x0000 and run

}

void stopBinApplication(BinImagePtr pBinImage, int ASID)
{
	// TODO
}

BOOL unloadBinApplication(BinImagePtr pBinImage, int ASID)
{
	// TODO: knowledge of all pages of app is necessary
	// this is job of vm_space and vnode_object
	return TRUE;
}

// --------------------

BOOL reservePagesForBinImage(BinImagePtr pBinImage, int ASID)
{
	// no page faults because all is entered in page table
	// (actualise TLB only for the first page to avoid nested exceptions at process start)
	int nrOfImagePages = 0;
	TLBELOPFNLESS tlbEntryLow;
	TLBEHI tlbEntryHigh;
	PageTableEntryPtr pPageTableEntry;
	int actPage = 0;
	nrOfImagePages = nrOfPagesForBinImage(pBinImage);

	// Do not separate access permissions between text and data...
	tlbEntryLow.accessPermissions = 7; // 0b111 - everything is allowed...
	tlbEntryLow.bufferable = FALSE;
	tlbEntryLow.cachable = FALSE;
	tlbEntryLow.dirty = FALSE;
	tlbEntryLow.global = FALSE;
	tlbEntryLow.pageSize = CODEDPAGESIZE;
	tlbEntryLow.writeThrough = FALSE;// TRUE; // TODO: ???

	tlbEntryHigh.ASID = ASID;
	tlbEntryHigh.instruction = TRUE;
	tlbEntryHigh.valid = TRUE;

	for (actPage=0; actPage<nrOfImagePages; actPage++)
	{
		//tlbEntryHigh.VPN = actPage * PAGESIZE * 1024;
		// CH.B.: 15.01. VPN is simply actPage number
		tlbEntryHigh.VPN = actPage;
		if (tlbEntryHigh.VPN < pBinImage->dataOffset)
			tlbEntryHigh.instruction = TRUE;
		else
			tlbEntryHigh.instruction = FALSE;

		pPageTableEntry = updatePageTableEntry(&tlbEntryHigh, &tlbEntryLow);
		if (pPageTableEntry == NULL)
			return FALSE;
	}

	// HACK
	// first page do not get any access rights - app must be linked in second page
	tlbEntryHigh.VPN = 0;
	tlbEntryLow.accessPermissions = 0; // 0b000 no access rights
	pPageTableEntry = updatePageTableEntry(&tlbEntryHigh, &tlbEntryLow);
	if (pPageTableEntry == NULL)
		return FALSE;

	return TRUE;
	//besser wŠre: getContinuousPageTableEntries(ASID, nrOfImagePages);
}

/** Writes raw binary image to ram.
 * During writing multiple page faults will occur.
 * Problem: Context is the context of OS NOT of application!
 * It is necessary to use the actual ASID also in OS Mode.
 * Alternatively write to Physical RAM via direct mapped memory and offset 0xA000000.
 */
BOOL writeBinImageToVirtualRam(BinImagePtr pBinImage, int ASID)
{
	char *pRAM = (char *)pBinImage->baseAddress;
	char *pData = pBinImage->pData;
	int index=0;

	char *pFirstPage = (char *)( pBinImage->baseAddress / (PAGESIZE*1024) );

/*	for(index=0; index < pBinImage->size; index++)
	{
		// cause Page-Fault - MMU Code will supply appropriate Page and
		// hardware will restart instruction
		// >> not possible anymore because of updatePageTableWithBinImage
		// only TLB misses should occour, if nested exceptions are ok this should work...
		// in the TLB miss-handler the actual ASID must be got from Process Manager
		pRAM[index] = *pData;
		pData++;
	}*/

//	// HACK
//	// fill first page with zeros - should be page where baseAddress is in
//	for (index=0; index<PAGESIZE*1024; index++)
//	{
//		pFirstPage[index] = 0;
//	}

	pData = pData+pBinImage->size-1;

	for(index=pBinImage->size - 1; index >=0 ; index--)
	{
		// cause Page-Fault - MMU Code will supply appropriate Page and
		// hardware will restart instruction
		// >> not possible anymore because of updatePageTableWithBinImage
		// only TLB misses should occour, if nested exceptions are ok this should work...
		// in the TLB miss-handler the actual ASID must be got from Process Manager
		pRAM[index] = *pData;
		pData--;
	}

	return TRUE;
}
/*
BOOL writeBinImageToPhysicalRam(BinImagePtr pBinImage)
{
	int nrOfPages = 0;

	// switch MMU off
	switchVirtualMemoryManagementOff();

	nrOfPages = nrOfPagesForBinImage(pBinImage);
	PageTableEntryPtr pPageTableList = getContinuousPageTableEntries(ASID, nrOfPages);

	void *pRAM = pBinImage->baseAddress;

	int index=0;
	for(index=0; index < count; index++)
	{
		// cause Page-Fault - MMU Code will supply appropriate Page and
		// hardware will restart instruction

		pRAM[baseaddress+index] = *pData;
		pData++;
	}
	// switch MMU off
	switchVirtualMemoryManagementOn();


	return TRUE;
}


BOOL writeDataToVirtualRam(unsigned char* pData, int vitualBaseAddress,  int count)
{
	void *pRAM = vitualBaseDddress;

	int index=0;
	for(index=0; index < count; index++)
	{
		// cause Page-Fault - MMU Code will supply appropriate Page and
		// hardware will restart instruction
		pRAM[vitualBaseAddress+index] = *pData;
		pData++;
	}

	return TRUE;
}

BOOL writeDataToPhysicalRam(unsigned char* pData, int physicalBaseAddress,  int count)
{
	return FALSE;
}
*/
// ---
int nrOfPagesForBinImage(BinImagePtr pBinImage)
{
//	return pBinImage->size / (PAGESIZE * 1024) + 1;
	// HACK
	// add a Page to have an empty zone in the first page for NULL pointer
	int nr =  pBinImage->size / (PAGESIZE * 1024) + 1;
	nr++;
	return nr;
}
// ---

