#ifndef MEMORYMANAGEMENT_H_
#define MEMORYMANAGEMENT_H_

/************************************************************************
 title:		Loader
 file:     	Loader.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2009 team halos
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup VMM  Virtual Memory Management
 *  @ingroup KERNEL
 *  @brief Virtual Memory Management. The central communication point to other modules.
 *  Especially Process Loading and Unloading is supported from the module.
 *  Only the Memory Exceptions undergo this module border.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/


/** Central Place for all Memory Interactions.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "PageTable.h"
#include "ProcessRegionTable.h"

#include "Utils.h"
#include "Loader.h"

	/**
	 * Initialize Page Table, set page table base register, reset and initialize MMU and MMU-Exceptions.
	 */
	extern int initMemoryManagement();

	/**
	 * State should be held in Process Management.
	 * Unused at the moment.
	 */
	void switchMemoryManagementToAppLoadMode();

	/**
	 * State should be held in Process Management.
	 * Unused at the moment.
	 */
	void switchMemoryManagementToAppRunMode();

	/**
	 * Set the given pid as ASID. Used especially for loading purposes.
	 * @param pid the process id that should be the ASID.
	 */
	void actualiseASID(uint32_t pid);

	/**
	 * Loads the Process Image in Memory.
	 * Actualise RegionTable, PageTable and TLB.
	 * @param ASID the PID of the process in charge
	 * @param pBinImage the Image of the process
	 * @return TRUE if successful, FALSE otherwise
	 */
	BOOL loadProcessInMemorySystem(int ASID, BinImagePtr pBinImage);

	/**
	 * Unloads process with given ASID from all Pagers i.e. from RamPage and SwapPage.
	 * @param ASID the PID of the process
	 * @return true if a complete remove of all Memory and Swap-Resources was successful, false otherwise
	 */
	BOOL unloadProcessFromMemorySystem(int ASID);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*MEMORYMANAGEMENT_H_*/
