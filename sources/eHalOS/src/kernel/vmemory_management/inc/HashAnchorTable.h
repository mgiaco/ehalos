#ifndef HASHANCHORTABLE_H_
#define HASHANCHORTABLE_H_

/************************************************************************
 title:		Hash Anchor Table
 file:     	HashAnchorTable.h
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
 *  @defgroup VMM_HAT Hash Anchor Table
 *  @ingroup VMM
 *  @brief description of Hash Anchor Table
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief identifies the number of buckets i.e. entries in the Hash Anchor Table
 */
#define NROFBUCKETS 256
#define ASIDSHIFT 4
#define MAXHASHANCHORTABLESIZE NROFBUCKETS
#define MINASID 0
#define MAXASID MAXHASHANCHORTABLESIZE-1
#define ILLEGALHATINDEX -1
#define ILLEGALHATENTRY -1

#define OR |
#define AND &
#define NOT !

#include "PageTableEntry.h"

/**
 * hash anchor table entry
 * @typedef HashAnchorTableEntry
 */
typedef int32_t HashAnchorTableEntry;
typedef HashAnchorTableEntry *HashAnchorTableEntryPtr;
typedef HashAnchorTableEntryPtr *HashAnchorTableEntryHdl;

/**
 * @var HashAnchorTable Hash anchor table that contains all anchors for the hash chains.
 */
extern HashAnchorTableEntry HashAnchorTable[];

/**
 * Initialize the hash anchor table with ILLEGALHATENTRY's.
 */
void initHashAnchorTable();

/**
 * Hash-function that transforms the ASID and VPN to an anchor table index.
 * @param ASID the ASID to integrate in hash
 * @param VPN the VPN to integrate in hash
 */
int32_t getHashAnchorTableIndex(int32_t ASID, int32_t VPN);
// >> if index is EMPTY then do a updatePageTable ... done by other process, for Simple lookup an empty index is a page fault.

/**
 * Returns the hash anchor table entry of the given index in the hash anchor table.
 * The hash anchor table entry is the index in the inverted page table.
 * @param hatIndex the HAT index to be transformed
 */
HashAnchorTableEntry getHATEntry(int32_t hatIndex);

/**
 * Set HAT entry if HAT index and HAT entry are not illegal.
 * @param hatIndex the HAT index at which the HAT Entry is set
 * @param hatEntry the HAT entry that is be set
 */
BOOL setHATEntry(int32_t hatIndex, HashAnchorTableEntry hatEntry);

/**
 * Set an ILLEGAL HAT entry if the given HAT index is not illegal.
 * @param hatIndex the HAT index at which the ILLEGAL HAT Entry is set
 */
BOOL setIllegalHATEntry(int32_t hatIndex);

/**
 * Creates an hash anchor table entry out of an inverted page table index.
 * @param iptIndex the IPT index which determines the HAT Entry.
 */
HashAnchorTableEntry createHATEntry(int iptIndex);

/**
 * Checks if the hash anchor table index is illegal.
 * @param hatIndex the HAT index to be verified
 */
BOOL isIllegalHATIndex(int32_t hatIndex);

/**
 * Checks if the hash anchor table entry is illegal.
 * @param hatEntry the HAT Entry to be verified
 */
BOOL isIllegalHATEntry(HashAnchorTableEntry hatEntry);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*HASHANCHORTABLE_H_*/
