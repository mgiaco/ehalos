#ifndef PAGETABLEENTRY_H_
#define PAGETABLEENTRY_H_

/************************************************************************
 title:		Page Table Entry
 file:     	PageTableEntry.h
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
 *  @defgroup VMM_PTE	Page Table Entry
 *  @ingroup VMM
 *  @brief Page Table Entry. Holds the TLB High and Low part that could also be found in the TLB.
 *  Essentially holds virtual Addresses and ASID (High) and physical Addresses (Low) along with other data.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/


#ifdef __cplusplus
extern "C" {
#endif

/* The PTE contains:
 *
 * The process ID of the owning process.
 * The virtual page number.
 * A pointer to the next IPTE in the hash chain.
 * The normal protection, valid, modified bits referenced.
 */
#define ILLEGALVPN -1
#define ILLEGALASID -1

#include "stdlib.h"
#include "Utils.h"

#ifdef PAGESIZE_64KB
#define	CODEDPAGESIZE 2 // 2 .. 64KB
#else
#define	CODEDPAGESIZE 1 // 1 .. 4KB
#endif

/**
 * @struct TLBEHIVNPLESS the High Part of a TLB entry without an VPN
 */
#pragma pack(1)
typedef struct _TLBEHIVPNLESS
{
	BOOL valid : 1; 			// 1-bit
	BOOL instruction : 1; 		// 1-bit
	int32_t ASID : 8; 			// 8-bits

}TLBEHIVNPLESS;

typedef TLBEHIVNPLESS *TLBEHIVNPLESSPtr;
typedef TLBEHIVNPLESSPtr *TLBEHIVNPLESSHdl;

/**
 * @struct TLBEHI the high part of a Page Table Entry.
 * Can be entered in the high part of Transition Lookaside Buffer.
 */
#pragma pack(1)
typedef struct _TLBEHI
{
	int32_t VPN : 22;			// 22-bits for 1KB 16 for 64KB
	BOOL valid : 1; 			// 1-bit
	BOOL instruction : 1; 		// 1-bit
	int32_t ASID : 8; 			// 8-bits

}TLBEHI;

typedef TLBEHI *TLBEHIPtr;
typedef TLBEHIPtr *TLBEHIHdl;

extern TLBEHI IllegalTLBEntryHigh;

/**
 * @struct TLBELOPFNLESS the low part of an TLB Entry without an PFN
 */
#pragma pack(1)
typedef struct _TLBELOPFNLESS
{
	BOOL cachable : 1; 				// 1-bit
	BOOL global : 1; 				// 1-bit
	BOOL bufferable : 1; 			// 1-bit
	uint8_t accessPermissions : 3; 	// 3-bits
	uint8_t pageSize : 2;			// 2-bits
	BOOL dirty : 1;					// 1-bit
	BOOL writeThrough : 1;			// 1-bit
}TLBELOPFNLESS;

typedef TLBELOPFNLESS *TLBELOPFNLESSPtr;
typedef TLBELOPFNLESSPtr *TLBELOPFNLESSHdl;

/**
 * @struct TLBELO the low part of a Page Table Entry.
 * Can be entered in the low part of Transition Lookaside Buffer.
 */
#pragma pack(1)
typedef struct _TLBELO
{
	uint32_t PFN : 22; 				// 22-bits
	BOOL cachable : 1; 				// 1-bit
	BOOL global : 1; 				// 1-bit
	BOOL bufferable : 1; 			// 1-bit
	uint8_t accessPermissions : 3; 	// 3-bits
	uint8_t pageSize : 2;			// 2-bits
	BOOL dirty : 1;					// 1-bit
	BOOL writeThrough : 1;			// 1-bit
}TLBELO;

typedef TLBELO *TLBELOPtr;
typedef TLBELOPtr *TLBELOHdl;

/**
 * @var IllegalTLBEntryLow value identifies an illegal TLB low entry.
 * Used for identification of reusable Page Table Entries.
 */
extern TLBELO IllegalTLBEntryLow;

/**
 * @struct PageTableEntry the Page Table Entry that is used in the Inverted Page Table.
 */
typedef struct _PageTableEntry
{
	TLBEHI tlbEntryHigh;
	TLBELO tlbEntryLow;
	struct _PageTableEntry *next;
}PageTableEntry;

typedef PageTableEntry *PageTableEntryPtr;
typedef PageTableEntryPtr *PageTableEntryHdl;

/**
 * @var IllegalPTEntry value identifies an illegal Page Table Entry.
 * Used for identification of reusable Page Table Entries.
 */
extern PageTableEntry IllegalPTEntry;

/**
 * Determines full member-value equality of given TLB High components.
 * @param pEntry1 first TLB High to compare
 * @param pEntry2 second TLB High to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equalTLBEntryHigh(TLBEHIPtr pEntry1, TLBEHIPtr pEntry2);

/**
 * Determines full member-value equality of given TLB Low components.
 * @param pEntry1 first TLB Low to compare
 * @param pEntry2 second TLB Low to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equalTLBEntryLow(TLBELOPtr pEntry1, TLBELOPtr pEntry2);

/**
 * Determines member-value equality without PFN of given TLB Low components.
 * @param pEntry1 first TLB Low to compare
 * @param pEntry2 second TLB Low to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equalTLBEntryLowPFNLess(TLBELOPtr pEntry1, TLBELOPFNLESSPtr pEntry2);

/**
 * Determines member-value equality without VPN of given TLB High components.
 * @param pEntry1 first TLB High to compare
 * @param pEntry2 second TLB High to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equalTLBEntryHighVPNLess(TLBEHIPtr pEntry1, TLBEHIVNPLESSPtr pEntry2);

/**
 * Determines if ASID and VPN of given TLB High components are equal.
 * @param pEntry1 first TLB High to compare
 * @param pEntry2 second TLB High to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equal_ASIDVPN_TLBEntryHigh(TLBEHIPtr pEntry1, TLBEHIPtr pEntry2);

/**
 * Determines if PFN of given TLB Low components are equal.
 * @param pEntry1 first TLB Low to compare
 * @param pEntry2 second TLB Low to compare
 * @return TRUE if equal, FALSE otherwise
 */
BOOL equal_PFN_TLBEntryLow(TLBELOPtr pEntry1, TLBELOPtr pEntry2);

/**
 * Compares the given high component of an page table entry with IllegalTLBEntryHigh.
 * @return TRUE if same as IllegalTLBEntryHigh, FALSE otherwise
 */
BOOL isUnusedTLBEntryHigh(TLBEHIPtr pEntry);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*PAGETABLEENTRY_H_*/
