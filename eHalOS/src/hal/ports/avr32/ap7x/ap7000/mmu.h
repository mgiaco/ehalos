/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief MMU example application.
 *
 * This file gives an example of using the MMU to manage the memory
 * on AVR32 MCUs.
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with MMU.
 * - AppNote:            AVR32113 Configuration and Use of the Memory
 *			 Management Unit
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Name$
 * $Revision: 679 $
 * $RCSfile$
 * $Date: 2007-10-12 09:44:30 +0200 (Fri, 12 Oct 2007) $
 *****************************************************************************/

/* Copyright (c) 2006, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MMU_H_
#define _MMU_H_

#include <avr32/io.h>
#include <stdint.h>

#ifdef __AVR32_AP7000__
#define MMU_DTLB_ENTRIES	32
#define MMU_ITLB_ENTRIES	0
#elif
#error Unknown CPU, MMU configuration must be added to mmu.h
#endif

/*! \brief Struct which defines options for operating on TLB entries
 */
struct mmu_tlb_t {
	/*! \brief Enable if the TLB entry is valid */
	int valid;

	/*! \brief The type of the TLB entry
	 *    \arg MMU_TLB_TYPE_DATA
	 *    \arg MMU_TLB_TYPE_INSTRUCTION
	 */
	int type;

	/*! \brief Application space identifier. Used by operating systems
	 *         to give an unique id to each process.
	 */
	int asid;

	/*! \brief Enable if the page is cachable */
	int cachable;

	/*! \brief Enable if the MMU is in private virtual memory mode and a
	 *         global lookup is to be beformed.
	 */
	int global;

	/*! \brief Enable if the page is bufferable */
	int bufferable;

	/*! \brief Access permissions specifying the requirements to access
	 *         the page.
	 */
	int access;

	/*! \brief Enable if the page has been written to. */
	int dirty;

	/*! \brief Page size for the entries */
	int page_size;

	/*! \brief Enable if the cache should use write-through as update
	 *         policy. Disable for write-back update.
	 */
	int write_through;

	/*! \brief The TLB entry to modify the entry to */
	int position;
};


/*! \brief MMU defines for readable setup */
enum {
	MMU_ERROR = -1,
	MMU_OK = 0,
	MMU_ERROR_ARGUMENT = 1,
	MMU_TLB_MISS = 2,

	MMU_DISABLE = 0,
	MMU_ENABLE = 1,
	MMU_PRIVATE_VIRTUAL_MODE = 0,
	MMU_SHARED_VIRTUAL_MODE = 1,
	MMU_SEGMENTATION_OFF = 0,
	MMU_SEGMENTATION_ON = 1,
	MMU_TLB_TYPE_DATA = 0,
	MMU_TLB_TYPE_INSTRUCTION = 1,

	MMU_PAGE_SIZE_1KB = 0,
	MMU_PAGE_SIZE_4KB = 1,
	MMU_PAGE_SIZE_64KB = 2,
	MMU_PAGE_SIZE_1MB = 3,
};

int mmu_init(int enable, int mode, int segmentation);

int mmu_reset();

int mmu_search_virtual_ptr(void * virtual_ptr, long * physical_addr);

int mmu_search_physical_ptr(void * physical_ptr, long * virtual_addr);

int mmu_lock_entries(int entries, int type);

int mmu_add_tlb_entry(long physical_addr_base,
		long virtual_addr_base, struct mmu_tlb_t * options);

void mmu_read_bear_and_tlbehi(long *bear, long *vpn);

int mmu_find_index_to_entry(long physical_addr, long virtual_addr, long *index);

// own code
int mmu_find_index_to_entry_with_ASID(long physical_addr, long virtual_addr, uint8_t ASID, long *index);

int mmu_invalidate_entry(int index, int type);


extern void mmu_dump_tlb(void);
extern void mmu_show_tlb_entry(uint32_t index);


#endif

