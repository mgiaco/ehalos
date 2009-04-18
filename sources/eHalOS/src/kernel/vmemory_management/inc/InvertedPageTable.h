#ifndef INVERTEDPAGETABLE_H_
#define INVERTEDPAGETABLE_H_

/************************************************************************
 title:		Inverted Page Table
 file:     	InvertedPageTable.h
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
 *  @defgroup VMM_IPT	Inverted Page Table
 *  @ingroup VMM
 *  @brief Inverted Page Table. The basic idea of an inverted page table is to
 *  have only one page table for the complete memory and all loaded processes.
 *  The entries identify the physical frames rather than the virtual pages.
 *  The main advantage is that no TLB flush is necessary between process switches.
 *  Another advantage is that the memory usage of the page table is moderate.
 *  The search speed of a given Page Table Entry depends heavily on the hash-algorithm
 *  that uses ASID and VPN. With a goog hash-algorithm two to three lookups are
 *  necessary. A disadvantage is that no memory sharing is possible with the standard variant.
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
#include "PageTableEntry.h"

#ifdef PAGESIZE_64KB

#define PAGESIZE 64 //64 KB Pagesize
// TODO: redefine...
#define MAXPAGETABLEENTRIES 432 // 432 with 64KB // upper bound, above reserved for OS
// TODO: change to OS Needs...
#define PFNOFFSET 0x10500000/(PAGESIZE *1024) // lower bound, below reserved for OS

#else

// change to 64 KB
#define PAGESIZE 4 // 4 KB Pagesize
// TODO: redefine...
#define MAXPAGETABLEENTRIES 6912 // 432 with 64KB // upper bound, above reserved for OS
// TODO: change to OS Needs...
#define PFNOFFSET 0x10500000/(PAGESIZE *1024) // lower bound, below reserved for OS

#endif

// TODO: enter pointer into PTBR
// Index corresponds to physical frame number.
//
/**
 * @var InvertedPageTable Inverted Page Table that contains all Page Table Entries that can be accessed by the MMU.
 */
extern PageTableEntry InvertedPageTable[];

// TODO: Freelist to search free pages faster and ignore frames that access no RAM and Flash
/**
 * Initialize the inverted page table with IllegalTLBEntryHigh for the tlbEntryHigh-Parts to indicate possible usage and
 * appropriate page frame numbers for the tlbEntryLow-Parts to indicate the physical frame.
 * The page frame numbers start from PFNOFFSET to upper end.
 */
void initInvertedPageTable();

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the Inverted Page Table.
 * Also returns the corresponding Hash Anchor - Page Table Entry and the Page Table Entry before the searched Page Table Entry.
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @param hHashAnchorPTEntry the Page Table Entry that is identified by the Hash Anchor Table
 * @param hPrevPTEntry the Page Table Entry in the hash chain before the searched Page Table Entry
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryAndHashAnchorAndPreviousInIPT(TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hHashAnchorPTEntry, PageTableEntryHdl hPrevPTEntry);

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the Inverted Page Table.
 * Also returns the corresponding Hash Anchor - Page Table Entry.
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @param hHashAnchorPTEntry the Page Table Entry that is identified by the Hash Anchor Table
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryAndHashAnchorInIPT(TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hHashAnchorPTEntry);

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the Inverted Page Table.
 * Additional the Hash Anchor Table Entry of the actual hash chain must be delivered to improve search space.
 * Also returns the Page Table Entry before the searched Page Table Entry.
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @param hatEntry the Hash Anchor Table Entry that identified the hash chain
 * @param hPrevPTEntry the Page Table Entry in the hash chain before the searched Page Table Entry
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryAndPreviousInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry, PageTableEntryHdl hPrevPTEntry);

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the Inverted Page Table.
 * Additional the Hash Anchor Table Entry of the actual hash chain must be delivered to improve search space.
 * Also returns the Page Table Entry before the searched Page Table Entry.
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @param hatEntry the Hash Anchor Table Entry that identified the hash chain
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry);

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the hash chain of given Page Table Entry.
 * Additionally the Page Table Entry before the searched Page Table Entry is delivered.
 * @param pPTEntry the Page Table Entry that is the start entry of the hash chain
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @param hPrevPTEntry the Page Table Entry in the hash chain before the searched Page Table Entry
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryAndPreviousInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh, PageTableEntryHdl hPrevPTEntry);

/** Search the corresponding page table entry depending on ASID and VPN of given page table high part in the hash chain of given Page Table Entry.
 * @param pPTEntry the Page Table Entry that is the start entry of the hash chain
 * @param pTLBEntryHigh the Page Table Entry high part that delivers the ASID and VPN for the search
 * @return the found Page Table Entry pointer or NULL if not found
 */
PageTableEntryPtr searchPageTableEntryInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh);

// search TLBELO entry to given TLBEHI entry.
// PFN is the start index to search used by hash anchor table.

/** Search the low part of the Page Table Entry in the given hash chain identified by given Hash Anchor Table Entry.
 * The searched Page Table Entry is identified by the ASID and VPN of given high part of the searched Page Table Entry.
 * @param pTLBEntryHigh the high part of the searched Page Table Entry i.e. its low part
 * @param hatEntry the Hash Anchor Table Entry that identifies the hash chain of Page Table Entries
 * @return low part of found Page Table Entry or NULL otherwise
 */
TLBELOPtr searchPageTableEntryLowInIPT(TLBEHIPtr pTLBEntryHigh, HashAnchorTableEntry hatEntry);

/** Search the low part of the Page Table Entry in the given hash chain via the given Page Table Entry.
 * The searched Page Table Entry is identified by the ASID and VPN of given high part of the searched Page Table Entry.
 * @param pPTEntry the Page Table Entry that delivers the hash chain
 * @param pTLBEntryHigh the high part of the searched Page Table Entry i.e. its low part
 * @return low part of found Page Table Entry or NULL otherwise
 */
TLBELOPtr searchPageTableEntryLowInPTE(PageTableEntryPtr pPTEntry, TLBEHIPtr pTLBEntryHigh);

/**
 * Transforms an Hash Anchor Table Entry to an appropriate Inverted Page Table index.
 * @param hatEntry the HAT Entry to transform
 */
int32_t getIPTIndex(HashAnchorTableEntry hatEntry);

/**
 * Returns the last Page Table Entry of the chain where given Page Table Entry is in.
 * @param pPTEntry a Page Table Entry in the chain
 */
PageTableEntryPtr lastPageTableEntryOfPTEChain(PageTableEntryPtr pPTEntry);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*INVERTEDPAGETABLE_H_*/
