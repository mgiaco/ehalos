#ifndef PROCESSREGIONTABLE_H_
#define PROCESSREGIONTABLE_H_

/************************************************************************
 title:		Process Region Table
 file:     	ProcessRegionTable.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2009 Team eHalOS
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup VMM Virtual Memory Management
 *  @ingroup KERNEL
 */

/**
 *  @defgroup VMM_PRT	Process Region Table
 *  @ingroup VMM
 *  @brief The Process Region Table should manage the regions and access-rights for a
 *  given process. Swapping and Demand-Paging links to different Page Tables.
 *  At the moment no swapping is supported and the Process Region Table is simply used
 *  for unloading the process from the virtual memory system.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif


#define MAXPROCESSREGIONS 8
#define NUMOFPAGERS 2
#define PROCESSREGIONTYPESIZE 8


#include "HashAnchorTable.h"
#include "PageTableEntry.h"
#include "Loader.h"
#include "Utils.h"
#include "TransitionLookasideBuffer.h"
#include <math.h>

/**
 * @struct PageTable that holds dynamically a Hash Anchor Table and an Inverted Page Table
 */
typedef struct pageTable{

	int32_t HashAnchorTableSize;
	HashAnchorTableEntryPtr pHashAnchorTable;
	int32_t InvertedPageTableSize;
	PageTableEntryPtr pInvertedPageTable;

}PageTable;

typedef PageTable *PageTablePtr;
typedef PageTablePtr *PageTableHdl;

/**
 * @enum PagerType identifies the job of the pager.
 */
typedef enum pagerType{
	SwapPagerType, RamPagerType
}PagerType;

/**
 * @struct Pager defines pager type and corresponding Page Table
 */
typedef struct pager
{
	int ID;
	PagerType Type;
	PageTablePtr pPageTable;
}Pager;

typedef Pager *PagerPtr;
typedef PagerPtr *PagerHdl;

/**
 * @var Pagers the available Pagers.
 * At the moment only RamPagers and SwapPagers are available.
 * RamPager is programmed, SwapPager need to be done.
 */
extern PagerPtr Pagers[];

/**
 * @enum AccessRight defines the AccessRight of a Process Region
 */
typedef enum accessRight{
	Read, Write, Execute
}AccessRight;

typedef AccessRight *AccessRightPtr;
/**
 * @struct ProcessRegion holds all access rights and VPN-range of the consecutive process-region.
 */
typedef struct processRegion{

	int32_t TypeSize;
	char *ProcessRegionType; // text, data, stack, ...

	int32_t AccessRightsSize;
	AccessRightPtr AccessRights;

	int32_t StartVPN; // lies at Page-Boundaries
	int32_t NrOfVPNs;

	int PagerID;

}ProcessRegion;

typedef ProcessRegion *ProcessRegionPtr;
typedef ProcessRegionPtr *ProcessRegionHdl;

/**
 * @struct Process Region Table holds the ASID of corresponding process,
 * all necessary Pagers and all existent Process Regions.
 */
typedef struct processRegionTable
{
	int32_t ASID;

	int32_t pagersSize;
	PagerHdl pPagers;

	int32_t ProcessRegionEntrySize;
	ProcessRegionHdl ProcessRegions;
	//BOOL *UsedRegions;

}ProcessRegionTable;

typedef ProcessRegionTable *ProcessRegionTablePtr;
typedef ProcessRegionTablePtr *ProcessRegionTableHdl;

//extern ProcessRegionTableHdl ProcessRegionTables;

/**
 * Initialise the available Process Region Table Pool.
 */
void initProcessRegionTables();

/**
 * If Process Region Table with given ASID it is unloaded automatically.
 * Otherwise a new Process Region Table for the given ASID is created.
 * @param ASID the PID of the process in charge
 * @param autoUnload TRUE if successful aunload happens, FALSE otherwise
 * @return new Process Region Table with default initialisation
 */
ProcessRegionTablePtr reuseProcessRegionTable(int ASID, BOOL *autoUnload);

/**
 * Dynamically creates a new Process Region Table for a Process with a given ASID.
 * @param ASID the PID of the process in charge
 * @return the table if successful, false otherwise
 */
ProcessRegionTablePtr createProcessRegionTable(int ASID);

/**
 * Returns a Process Region Table for the process with the given ASID.
 * @param ASID the PID of the process in charge
 * @return the found Process Region Table pointer, NULL otherwise
 */
ProcessRegionTablePtr findProcessRegionTable(int ASID);

/**
 * Adds all pages of image to appropriate region of region table.
 * Only existent to have an intermediate Solution with the Loader.
 * With full demand paging this is not necessary anymore.
 * @param ASID the PID of the process in charge
 * @param pBinImage the process image that will be fully loaded in RAM
 * @return true if Regions could be added, false otherwise
 */
BOOL addBinImageRegionToRamPager(int ASID, BinImagePtr pBinImage);

/**
 * Add a given Region to the Process Region Table of the process in charge.
 * @param ASID the PID of the process in charge
 * @param pProcessRegion the process region that should be added
 * @return true if adding was successful, false otherwise
 */
BOOL addRegion(int ASID, ProcessRegionPtr pProcessRegion);

/**
 * Checks if the given Process Region Table is illegal, i.e. could be overwritten.
 * @param pProcessRegionTable the table in charge
 * @return true if illegal, false otherwise
 */
BOOL isIllegalProcessRegionTable(ProcessRegionTablePtr pProcessRegionTable);

/**
 * Unloads process with given ASID from all Pagers i.e. from RamPage and SwapPage.
 * @param ASID the PID of the process
 * @return true if a complete remove of all Memory and Swap-Resources was successful, false otherwise
 */
BOOL unloadProcessRegionTable(int ASID);

/**
 * Frees contents in given Process Region Table.
 * @param pProcessRegionTable the Process Region Table which contents should be freed
 * @return TRUE if successful, FALSE otherwise
 */
BOOL freeProcessRegionTableContents(ProcessRegionTablePtr pProcessRegionTable);

/**
 * Frees contents in given Process Region.
 * @param pProcessRegion the Process Region which contents should be freed
 * @return TRUE if successful, FALSE otherwise
 */
BOOL freeProcessRegionContents(ProcessRegionPtr pProcessRegion);

/**
 * Find Process Region according to defined Memory Region.
 * @param ASID the PID of the process in charge
 * @param size the size in byte of allocated memory
 * @param pMemory the pointer to allocated memory
 * @return the found ProcessRegion or NULL otherwise
 */
ProcessRegionPtr findProcessRegionByMemoryRegion(int ASID, size_t size, void *pMemory);

/**
 * Accumulates a new Right to existing AccessRights of the given Process Region.
 * To Be Implemented.
 * @param pProcessRegion the Process Region to be updated
 * @param accessRight the Access Right to be added
 * @return TRUE if add was successful, FALSE otherwise
 */
BOOL addRightToProcessRegion(ProcessRegionPtr pProcessRegion, AccessRight accessRight);

//ProcessRegionPtr createProcessRegion(int pagerID, int startVPN, int nrOfVPNs, char *pProcessRegionType, int32_t accessRightsSize, AccessRight []accessRights);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* PROCESSREGIONTABLE_H_ */
