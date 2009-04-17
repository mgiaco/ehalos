#ifndef PAGETABLE_H_
#define PAGETABLE_H_

/************************************************************************
 title:		Page Table
 file:     	PageTable.h
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
 *  @defgroup VMM Virtual Memory Management
 *  @ingroup KERNEL
 */

/**
 *  @defgroup VMM_PT	Page Table
 *  @ingroup VMM
 *  @brief The Page Table consists of two parts, namely the Hash Anchor Table and the Inverted Page Table.
 *  This is to improve the search speed via a hashing algorithm for given ASID-VPN combinations.
 *  The init-, search-, update- and free-methods should be used from Memory Management to handle the Page Table,
 *  other methods are for internal use only.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/


#ifdef __cplusplus
extern "C" {
#endif

#include "HashAnchorTable.h"
#include "InvertedPageTable.h"

/**
 * Initialize all components of the Page Table.
 * That means Hash Anchor Table and Inverted Page Table.
 */
void initPageTable();

/**
 * Search a complete Page Table Entry via ASID and VPN of the given TLBEntryHigh part of the Page Table Entry.
 * @param pTLBEntryHigh high part with relevant ASID and VPN to search.
 * @return found Page Table Entry pointer, NULL otherwise
 */
PageTableEntryPtr searchPageTableEntry(TLBEHIPtr pTLBEntryHigh);

/**
 * Search the TLBEntryLow Part via ASID and VPN of the given TLBEntryHigh part of the Page Table Entry.
 * @param pTLBEntryHigh high part with relevant ASID and VPN to search.
 * @return found TLB Low pointer, NULL otherwise
 */
TLBELOPtr searchPageTableEntryLow(TLBEHIPtr pTLBEntryHigh);

// insert VPN & AISD in a victim entry and update HAT and IPT.

/** Update the Inverted Page Table Entry identified by tlbEntryHigh.
 * If existent it is replaced, if not it is inserted in front of bucket-chain.
 * @param pTLBEntryHigh delivers ASID and VPN for search or all parameters of the high part for creation of entry
 * @param pTLBEntryLow identifies all parameters except the page frame number of the low part
 * @return created or actualized Page Table Entry pointer if successful, NULL otherwise
 */
PageTableEntryPtr updatePageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow);

/**
 * Dissolves the found Page Table Entry from all chains and set it to IllegalPTEntry except the page frame number.
 * @param pTLBEntryHigh the TLBEntryHigh Part that delivers the ASID and VPN
 * @return true if successful, false otherwise
 */
BOOL freePageTableEntry(TLBEHIPtr pTLBEntryHigh);

/**
 * Dissolve a Page Table Entry from the Inverted Page Table and returns its index.
 * At the moment only free PageTableEntries are returned.
 * Later victim should be found via second change algorithm.
 * Dissovling includes the hash-chain as well as the HashAnchor.
 * @param pTLBEntryHigh to identify if entry is unused via complete Inverted Page Table search
 * @param pIPTIndex the inverted page table index of the dissolved page table entry
 * @return the dissolved Page Table Entry pointer if successful, or NULL otherwise
 */
PageTableEntryPtr getDissolvedVictimPTEntry(TLBEHIPtr pTLBEntryHigh, int *pIPTIndex);

/** Dissovle given page table entry. Rechain page table chain and actualize HAT if necessary.
 *  Do not delete contents of dissolved page table entries. This is the job of the caller.
 *  @param pPTEntry  the Page Table Entry that should be dissolved
 *  @param pHashAnchorPTEntry the Page Table Entry identified by the Hash Anchor Table, i.e. the start of the hash chain
 *  @param pPrevPTEntry the Page Table Entry that is before the entry to dissolve in the hash chain
 *  @return TRUE if successful, FALSE otherwise
 */
BOOL dissolvePageTableEntry(PageTableEntryPtr pPTEntry, PageTableEntryPtr pHashAnchorPTEntry, PageTableEntryPtr pPrevPTEntry);

/** Creates a new Page Table Entry with the high and low parameters. The hash chain is given.
 * @param pTLBEntryHigh identifies all parameters of the high part
 * @param pTLBEntryLow identifies all parameters except the page frame number of the low part
 * @param pHashAnchorPTEntry identifies the hash chain in which the entry is inserted
 * @return created Page Table Entry pointer if successful, NULL otherwise
 */
PageTableEntryPtr createPageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow, PageTableEntryPtr pHashAnchorPTEntry);

/** Creates a new Hash Anchor Page Table Entry i.e. the first entry in the hash chain that is referenced by the Hash Anchor Table.
 * @param pTLBEntryHigh identifies all parameters of the high part
 * @param pTLBEntryLow identifies all parameters except the page frame number of the low part
 * @return created Page Table Entry pointer if successful, NULL otherwise
 */
PageTableEntryPtr createHashAnchorPageTableEntry(TLBEHIPtr pTLBEntryHigh, TLBELOPFNLESSPtr pTLBEntryLow);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*PAGETABLE_H_*/
