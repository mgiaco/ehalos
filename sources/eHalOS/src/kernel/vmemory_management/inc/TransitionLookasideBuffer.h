#ifndef TRANSITIONLOOKASIDEBUFFER_H_
#define TRANSITIONLOOKASIDEBUFFER_H_

/************************************************************************
 title:		Transition Lookaside Buffer
 file:     	TransitionLookasideBuffer.h
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
 *  @defgroup VMM_TLB	Transition Lookaside Buffer
 *  @ingroup VMM
 *  @brief Transition Lookaside Buffer. Manages the 32 data entries of the AVR32 AP7000.
 *  Used by memory exception handler.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "PageTableEntry.h"
#include "InvertedPageTable.h"

#include "mmu.h"
#include "macro.h"

/**
 * @brief the possible number of loaded Processes as far Memory Management is concerned.
 */
#define	NROFTLBENTRIES 32 // only 32 data entries (64 data and 64 instruction entries)


/**
 * Updates the TLB with the given Page Table Entry.
 * At the moment FIFO is used to skip entries.
 * Later Second Chance to skip page out of TLB
 * search an entry which is not accessed
 * and reset the accessed bit of visited entries.
 * @param pPageTableEntry the Page Table Entry that must be added in the TLB
 * @return TRUE id successful, FALSE otherwise
 */
BOOL updateTLB(PageTableEntryPtr pPageTableEntry);

/**
 * Transforms own Page Table Entry structure into data structure of mmu.c code.
 * @param pPTEntry the Page Table Entry that should be transformed
 * @param pType MMU_TLB_TYPE_DATA or MMU_TLB_TYPE_INSTRUCTION
 * @param pPhysical_addr_base the physical base address in mmu.c format
 * @param pVirtual_addr_base the virtual base address in mmu.c format
 * @param pOptions defines options for operating on TLB entries
 * @param mmuStatus MMU_OK if success, MMU_TLB_MISS otherwise
 */
void transformPTEntry(PageTableEntryPtr pPTEntry, int *pType, long *pPhysical_addr_base, long *pVirtual_addr_base, struct mmu_tlb_t * pOptions,  int *mmuStatus);

/**
 * Check if the TLB Entry is wired down.
 * @param index identifies the TLB Entry
 * @param type MMU_TLB_TYPE_DATA or MMU_TLB_TYPE_INSTRUCTION
 * @return TRUE if successful, FALSE otherwise
 */
BOOL isWiredDown(int index,int type);

/**
 * Check if the TLB Entry is accessed.
 * @param index identifies the TLB Entry
 * @param type MMU_TLB_TYPE_DATA or MMU_TLB_TYPE_INSTRUCTION
 * @return TRUE if successful, FALSE otherwise
 */
BOOL isAccessed(int index,int type);

/**
 * Set the identified TLB Entry to not accessed.
 * @param index identifies the TLB Entry
 * @param type MMU_TLB_TYPE_DATA or MMU_TLB_TYPE_INSTRUCTION
 * @return MMU_OK if successful
 */
int setNotAccessed(int index, int type);

/**
 * Invalidates the Transition Lookaside Buffer of the corresponding Page Table Entry.
 * @param pPTEntry the Page Table Entry in charge
 * @return TRUE if successful, FALSE otherwise
 */
BOOL InvalidateTLBEntry(PageTableEntryPtr pPTEntry);

/**
 * Invalidates the complete Transition Lookaside Buffer.
 * @return TRUE if successful, FALSE otherwise
 */
BOOL InvalidateTLB();

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*TRANSITIONLOOKASIDEBUFFER_H_*/
