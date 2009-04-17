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

#include <avr32/io.h>
#include "mmu.h"
#include "macro.h"

#include "printf_P.h"

/* Prototypes for local functions */
int mmu_add_entry(long vpn, long pfn, int pos, int type);
int mmu_read_entry(long * vpn, long * pfn, int pos, int type);
int mmu_get_addrmask_from_page_size(long tlbelo, long *address_mask);


/*! \brief Changes mode on the MMU according to input
 *
 *  This function only changes the mode, it will not invalidate all the TLB
 *  entries. If this is needed use the \ref mmu_reset function to reset the
 *  MMU back to default setup.
 *
 *  \param enable enable or disable the MMU
 *    \arg MMU_ENABLE
 *    \arg MMU_DISABLE
 *  \param mode Set the MMU in private virtual mode or 1-shared virtual mode
 *    \arg MMU_PRIVATE_VIRTUAL_MODE
 *    \arg MMU_SHARED_VIRTUAL_MODE
 *  \param segmentation Turns on or off segmentation
 *    \arg MMU_SEGMENTATION_ON
 *    \arg MMU_SEGMENTATION_OFF
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR if unable to initialize the MMU
 */
int mmu_init(int enable, int mode, int segmentation)
{
	long regval = 0;
	long regval_read = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regval);

	regval = 	(		enable	<<AVR32_MMUCR_E_OFFSET)|
				(		mode	<<AVR32_MMUCR_M_OFFSET)|
				(segmentation	<<AVR32_MMUCR_S_OFFSET);

	AVR32_SET_SR_REG(AVR32_MMUCR, regval);

	AVR32_READ_SR_REG(AVR32_MMUCR, regval_read);

	if (regval != regval_read) {
		return MMU_ERROR;
	}

	return MMU_OK;
}


/*! \brief Resets the MMU and invalidates all TLB entries
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR if unable to reset the MMU
 */
int mmu_reset()
{
	long regval;
	int timer = 10;

	AVR32_SET_SR_REG(AVR32_MMUCR, AVR32_MMUCR_S_MASK|AVR32_MMUCR_I_MASK);

	/* Wait for invalidation of MMU entries */
	do {
		AVR32_READ_SR_REG(AVR32_MMUCR, regval);
		--timer;
	} while ((regval & AVR32_MMUCR_I_MASK) != 0 && timer > 0);

	if (regval != AVR32_MMUCR_S_MASK || timer == 0) {
		return MMU_ERROR;
	}

	//AVR32_SET_SR_REG(AVR32_TLBARHI, 0);
	//AVR32_SET_SR_REG(AVR32_TLBARLO, 0);

	AVR32_SET_SR_REG(AVR32_TLBARHI, 0xffffffff);
	AVR32_SET_SR_REG(AVR32_TLBARLO, 0xffffffff);

	return MMU_OK;
}


/*! \brief Search a virtual address and get the physical address from the
 *         TLB. The function searches the data TLBs first before searching
 *         the instruction TLBs.
 *
 *  \param virtual_ptr the virtual address to search for
 *  \param physical_addr pointer to a variable the physical address will
 *                       be written
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 *    \retval MMU_TLB_MISS on TLB search miss
 */
int mmu_search_virtual_ptr(void *virtual_ptr, long *physical_addr)
{
	long addr_mask;
	long vpn;
	long pfn;
	int retval;
	int i;

	for (i = 0; i < MMU_DTLB_ENTRIES; i++) {
		/* Read the TLB entry */
		retval = mmu_read_entry(&vpn, &pfn, i, MMU_TLB_TYPE_DATA);
		if (retval != MMU_OK) {
			return retval;
		}

		retval = mmu_get_addrmask_from_page_size(pfn, &addr_mask);
		if (retval == MMU_OK) {
			if (((long)virtual_ptr & addr_mask) == (vpn & addr_mask)
					&& (vpn & AVR32_TLBEHI_V_MASK)) {
				*physical_addr = (pfn & addr_mask)
					|((long)virtual_ptr & ~addr_mask);
				return MMU_OK;
			}
		}
	}

	for (i = 0; i < MMU_ITLB_ENTRIES; i++) {
		/* Read the TLB entry */
		retval = mmu_read_entry(&vpn, &pfn, i, MMU_TLB_TYPE_INSTRUCTION);
		if (retval != MMU_OK) {
			return retval;
		}

		retval = mmu_get_addrmask_from_page_size(pfn, &addr_mask);
		if (retval == MMU_OK) {
			if (((long)virtual_ptr & addr_mask) == (vpn & addr_mask)
					&& (vpn & AVR32_TLBEHI_V_MASK)) {
				*physical_addr = (pfn & addr_mask)
					|((long)virtual_ptr & ~addr_mask);
				return MMU_OK;
			}
		}
	}



	return MMU_TLB_MISS;
}


/*! \brief Search a physical address and get the virtual address from the
 *         TLB. The function searches the data TLBs first before searching
 *         the instruction TLBs.
 *
 *  \param physical_ptr the physical address to search for
 *  \param virtual_addr pointer to a variable the virtual address will
 *                      be written
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 *    \retval MMU_TLB_MISS on TLB search miss
 */
int mmu_search_physical_ptr(void *physical_ptr, long *virtual_addr)
{
	long addr_mask;
	long vpn;
	long pfn;
	int retval;
	int i;

	for (i = 0; i < MMU_DTLB_ENTRIES; i++) {
		/* Read the TLB entry */
		retval = mmu_read_entry(&vpn, &pfn, i, MMU_TLB_TYPE_DATA);
		if (retval != MMU_OK) {
			return retval;
		}

		retval = mmu_get_addrmask_from_page_size(pfn, &addr_mask);
		if (retval == MMU_OK) {
			if (((long)physical_ptr & addr_mask) == (pfn & addr_mask)
					&& (vpn & AVR32_TLBEHI_V_MASK)) {
				*virtual_addr = (vpn & addr_mask)
					|((long)physical_ptr & ~addr_mask);
				return MMU_OK;
			}
		}
	}

	for (i = 0; i < MMU_ITLB_ENTRIES; i++) {
		/* Read the TLB entry */
		retval = mmu_read_entry(&vpn, &pfn, i, MMU_TLB_TYPE_INSTRUCTION);
		if (retval != MMU_OK) {
			return retval;
		}

		retval = mmu_get_addrmask_from_page_size(pfn, &addr_mask);
		if (retval == MMU_OK) {
			if (((long)physical_ptr & addr_mask) == (pfn & addr_mask)
					&& (vpn & AVR32_TLBEHI_V_MASK)) {
				*virtual_addr = (vpn & addr_mask)
					|((long)physical_ptr & ~addr_mask);
				return MMU_OK;
			}
		}
	}


	return MMU_TLB_MISS;
}


/*! \brief Locks a specified number of entries
 *
 *  \param entries number of entries to lock
 *  \param type data or instruction entries to lock
 *    \arg MMU_TLB_TYPE_DATA
 *    \arg MMU_TLB_TYPE_INSTRUCTION
 */
int mmu_lock_entries(int entries, int type)
{
	if (entries < 0 ||
			((type == MMU_TLB_TYPE_DATA &&
			  entries >= MMU_DTLB_ENTRIES) ||
			 (type == MMU_TLB_TYPE_INSTRUCTION &&
			  entries >= MMU_ITLB_ENTRIES))) {
		return MMU_ERROR_ARGUMENT;
	}

	long regValue = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	if (type == MMU_TLB_TYPE_INSTRUCTION) {
		//regValue &= ~(AVR32_MMUCR_ILA_MASK);
		//regValue |= (entries<<AVR32_MMUCR_ILA_OFFSET);
	} else {
		regValue &= ~(AVR32_MMUCR_DLA_MASK);
		regValue |= (entries<<AVR32_MMUCR_DLA_OFFSET);
	}

	AVR32_SET_SR_REG(AVR32_MMUCR, regValue);

	return MMU_OK;
}


/*! \brief Adds an entry to the TLB
 *
 *  \param physical_addr_base base address of the physical placement
 *  \param virtual_addr_base base address of the virtual placement
 *  \param options options to the entry
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 */
int mmu_add_tlb_entry(long physical_addr_base, long virtual_addr_base, struct mmu_tlb_t *options)
{
	if (!(options->valid == MMU_ENABLE
			|| options->valid == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->type == MMU_TLB_TYPE_INSTRUCTION
			|| options->type == MMU_TLB_TYPE_DATA)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (options->asid > 0xFF
			|| options->asid < 0) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->cachable == MMU_ENABLE
			|| options->cachable == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->global == MMU_ENABLE
			|| options->global == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->bufferable == MMU_ENABLE
			|| options->bufferable == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (options->access > 0x0D
			|| options->access < 0) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->page_size == MMU_PAGE_SIZE_1KB
			|| options->page_size == MMU_PAGE_SIZE_4KB
			|| options->page_size == MMU_PAGE_SIZE_64KB
			|| options->page_size == MMU_PAGE_SIZE_1MB)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->dirty == MMU_ENABLE
			|| options->dirty == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (!(options->write_through == MMU_ENABLE
			|| options->write_through == MMU_DISABLE)) {
		return MMU_ERROR_ARGUMENT;
	}
	else if (options->position < 0 ||
			((options->type == MMU_TLB_TYPE_DATA &&
			  options->position >= MMU_DTLB_ENTRIES) ||
			 (options->type == MMU_TLB_TYPE_INSTRUCTION &&
			  options->position >= MMU_ITLB_ENTRIES))) {
		return MMU_ERROR_ARGUMENT;
	}

	unsigned long vpn;
	unsigned long pfn;
	unsigned long addr_mask;

	if (options->page_size == MMU_PAGE_SIZE_1KB) {
		addr_mask = 0xFFFFFC00;
	}
	else if (options->page_size == MMU_PAGE_SIZE_4KB) {
		addr_mask = 0xFFFFF000;
	}
	else if (options->page_size == MMU_PAGE_SIZE_64KB) {
		addr_mask = 0xFFFF0000;
	}
	else if (options->page_size == MMU_PAGE_SIZE_1MB) {
		addr_mask = 0xFFF00000;
	} else {
		return MMU_ERROR_ARGUMENT;
	}

	vpn =
		(virtual_addr_base & addr_mask)|
		(options->valid<<AVR32_TLBEHI_V_OFFSET)|
		(options->type<<AVR32_TLBEHI_I_OFFSET)|
		(options->asid<<AVR32_TLBEHI_ASID_OFFSET);

	pfn =
		(physical_addr_base & addr_mask)|
		(options->cachable<<AVR32_TLBELO_C_OFFSET)|
		(options->global<<AVR32_TLBELO_G_OFFSET)|
		(options->bufferable<<AVR32_TLBELO_B_OFFSET)|
		(options->access<<AVR32_TLBELO_AP_OFFSET)|
		(options->page_size<<AVR32_TLBELO_SZ_OFFSET)|
		(options->dirty<<AVR32_TLBELO_D_OFFSET)|
		(options->write_through<<AVR32_TLBELO_W_OFFSET);

	int retVal = MMU_OK;

	retVal = mmu_add_entry(vpn, pfn, options->position, options->type);

	return retVal;
}


/*! \brief Reads the TLBEAR and TLBEHI registers and place the contents in
 *         \c bear and \c vpn
 *
 *  \param bear pointer to where to put the TLBEAR data
 *  \param vpn pointer to where to put the TLBEHI data
 */
void mmu_read_bear_and_tlbehi(long *bear, long *vpn)
{
	AVR32_READ_SR_REG(AVR32_TLBEAR, *bear);
	AVR32_READ_SR_REG(AVR32_TLBEHI, *vpn);
}


/*! \brief Search the TLB for an entry matching the given physical and
 *         virtual address and return the index.
 *
 *  \param physical_addr the physical address to find
 *  \param virtual_addr the virtual address to find
 *  \param index the index of the entry from the search
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_TLB_MISS on TLB search miss
 */
int mmu_find_index_to_entry(long physical_addr, long virtual_addr, long *index)
{
	long vpn = virtual_addr & 0xFFFFfc00;
	long pfn = physical_addr & 0xFFFFfc00;

	AVR32_SET_SR_REG(AVR32_TLBEHI, vpn);
	AVR32_SET_SR_REG(AVR32_TLBELO, pfn);

#ifdef __GNUC__
	__builtin_tlbs();
#elif __ICCAVR32__
	__search_TLB_entry();
#else
#error No known compiler used
#endif

	long regValue = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	if ((regValue & AVR32_MMUCR_N_MASK) != 0) {
		return MMU_TLB_MISS;
	}

	/* Check for instruction or data TBL */
	if ((vpn & AVR32_TLBEHI_I_MASK) != 0) {
		//*index = 0x3F & (regValue>>AVR32_MMUCR_IRP_OFFSET);
	} else {
		*index = 0x3F & (regValue>>AVR32_MMUCR_DRP_OFFSET);
	}

	return MMU_OK;
}

int mmu_find_index_to_entry_with_ASID(long physical_addr, long virtual_addr, uint8_t ASID, long *index)
{
	long vpn = virtual_addr & 0xFFFFfc00;
	long pfn = physical_addr & 0xFFFFfc00;

	vpn |= (ASID << AVR32_TLBEHI_ASID_OFFSET);

	AVR32_SET_SR_REG(AVR32_TLBEHI, vpn);
	AVR32_SET_SR_REG(AVR32_TLBELO, pfn);

#ifdef __GNUC__
	__builtin_tlbs();
#elif __ICCAVR32__
	__search_TLB_entry();
#else
#error No known compiler used
#endif

	long regValue = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	if ((regValue & AVR32_MMUCR_N_MASK) != 0) {
		return MMU_TLB_MISS;
	}

	/* Check for instruction or data TBL */
	if ((vpn & AVR32_TLBEHI_I_MASK) != 0) {
		//*index = 0x3F & (regValue>>AVR32_MMUCR_IRP_OFFSET);
	} else {
		*index = 0x3F & (regValue>>AVR32_MMUCR_DRP_OFFSET);
	}

	return MMU_OK;
}


/*! \brief Invalidates an entry in the TLB
 *
 *  \param index the TLB index of the entry to be invalidated
 *  \param type data or instruction entries to invalidate
 *    \arg MMU_TLB_TYPE_INSTRUCTION
 *    \arg MMU_TLB_TYPE_DATA
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid argument
 */
int mmu_invalidate_entry(int index, int type)
{
	if (index < 0 ||
			((type == MMU_TLB_TYPE_DATA &&
			  index >= MMU_DTLB_ENTRIES) ||
			 (type == MMU_TLB_TYPE_INSTRUCTION &&
			  index >= MMU_ITLB_ENTRIES))) {
		return MMU_ERROR_ARGUMENT;
	}

	long regValue = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	if (type == MMU_TLB_TYPE_INSTRUCTION) {
		//regValue &= ~(0x3F<<AVR32_MMUCR_IRP_OFFSET);
		//regValue |= (index<<AVR32_MMUCR_IRP_OFFSET);
	} else {
		regValue &= ~(0x3F<<AVR32_MMUCR_DRP_OFFSET);
		regValue |= (index<<AVR32_MMUCR_DRP_OFFSET);
	}

	AVR32_SET_SR_REG(AVR32_MMUCR, regValue);

#ifdef __GNUC__
	__builtin_tlbr();
#elif __ICCAVR32__
	__read_TLB_entry();
#else
#error No known compiler used
#endif

	//AVR32_CLEAR_SR_BIT(AVR32_TLBEHI, AVR32_TLBEHI_V_OFFSET); => falsch sorry aber ATMEL, ATMEL..
	AVR32_CLEAR_SR_BIT(AVR32_TLBEHI, AVR32_TLBEHI_V_MASK);

#ifdef __GNUC__
	__builtin_tlbw();
#elif __ICCAVR32__
	__write_TLB_entry();
#else
#error No known compiler used
#endif

	return MMU_OK;
}


/*! \brief Adds an entry to the TLB
 *
 *  \param vpn the value to be written to TLBEHI
 *  \param pfn the value to be written to TLBELO
 *  \param pos the TLB index to add an entry to
 *  \param type data or instruction entry to add
 *    \arg MMU_TLB_TYPE_DATA
 *    \arg MMU_TLB_TYPE_INSTRUCTION
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 */
int mmu_add_entry(long vpn, long pfn, int pos, int type)
{

	if (pos < 0 ||
			((type == MMU_TLB_TYPE_DATA &&
			  pos >= MMU_DTLB_ENTRIES) ||
			 (type == MMU_TLB_TYPE_INSTRUCTION &&
			  pos >= MMU_ITLB_ENTRIES))) {
		return MMU_ERROR_ARGUMENT;
	}

	long regValue = 0;

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	regValue &= 0x0000001F;

	if (type == MMU_TLB_TYPE_INSTRUCTION) {
		//regValue |= (pos<<AVR32_MMUCR_IRP_OFFSET);
	}
	else if (type == MMU_TLB_TYPE_DATA) {
		regValue |= (pos<<AVR32_MMUCR_DRP_OFFSET);
	} else {
		return MMU_ERROR_ARGUMENT;
	}

	//regValue = 26;


	AVR32_SET_SR_REG(AVR32_MMUCR, regValue);
	AVR32_SET_SR_REG(AVR32_TLBEHI, vpn);
	AVR32_SET_SR_REG(AVR32_TLBELO, pfn);

#ifdef __GNUC__
	__builtin_tlbw();
#elif __ICCAVR32__
	__write_TLB_entry();
#else
#error No known compiler used
#endif

	return MMU_OK;
}


/*! \brief Reads an entry from the TLB and place the contents in
 *         \c vpn and \c pfn.
 *
 *  \param vpn address to the result position, MSB
 *  \param pfn address to the result position, LSB
 *  \param pos the TLB index to read an entry from
 *  \param type data or instruction entry to read
 *    \arg MMU_TLB_TYPE_DATA
 *    \arg MMU_TLB_TYPE_INSTRUCTION
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 */
int mmu_read_entry(long *vpn, long *pfn, int pos, int type)
{
	long regValue = 0;
	if (pos < 0 ||
			((type == MMU_TLB_TYPE_DATA &&
			  pos >= MMU_DTLB_ENTRIES) ||
			 (type == MMU_TLB_TYPE_INSTRUCTION &&
			  pos >= MMU_ITLB_ENTRIES))) {
		return MMU_ERROR_ARGUMENT;
	}

	AVR32_READ_SR_REG(AVR32_MMUCR, regValue);

	if (type== MMU_TLB_TYPE_INSTRUCTION) {
		//regValue &= ~(0x3F<<AVR32_MMUCR_IRP_OFFSET);
		//regValue |= (pos<<AVR32_MMUCR_IRP_OFFSET);
	} else {
		regValue &= ~(0x3F<<AVR32_MMUCR_DRP_OFFSET);
		regValue |= (pos<<AVR32_MMUCR_DRP_OFFSET);
	}

	AVR32_SET_SR_REG(AVR32_MMUCR, regValue);

#ifdef __GNUC__
	__builtin_tlbr();
#elif __ICCAVR32__
	__read_TLB_entry();
#else
#error No known compiler used
#endif

	AVR32_READ_SR_REG(AVR32_TLBEHI, *vpn);
	AVR32_READ_SR_REG(AVR32_TLBELO, *pfn);

	return MMU_OK;
}


/*! \brief Get the address mask from the page size of a TLBELO entry.
 *
 *  \param tlbelo the tlbelo to get the page size off
 *  \param address_mask pointer to a variable the address mask will
 *                      be written
 *
 *  \return Status
 *    \retval MMU_OK on success
 *    \retval MMU_ERROR_ARGUMENT on invalid arguments
 */
int mmu_get_addrmask_from_page_size(long tlbelo, long *address_mask)
{
	int page_size = (tlbelo & AVR32_TLBELO_SZ_MASK) >> AVR32_TLBELO_SZ_OFFSET;

	if (page_size == MMU_PAGE_SIZE_1KB) {
		*address_mask = 0xFFFFfc00;
	}
	else if (page_size == MMU_PAGE_SIZE_4KB) {
		*address_mask = 0xFFFFf000;
	}
	else if (page_size == MMU_PAGE_SIZE_64KB) {
		*address_mask = 0xFFFF0000;
	}
	else if (page_size == MMU_PAGE_SIZE_1MB) {
		*address_mask = 0xFFF00000;
	} else {
		return MMU_ERROR_ARGUMENT;
	}

	return MMU_OK;
}

void mmu_show_tlb_entry(uint32_t index) {
	uint32_t tlbehi, tlbehi_save, tlbelo, mmucr, mmucr_save;
	uint64_t flags;

	//	local_irq_save(flags);
	//	mmucr_save = sysreg_read(MMUCR);
	//	tlbehi_save = sysreg_read(TLBEHI);
	//	mmucr = mmucr_save & 0x13;
	//	mmucr |= index << 14;
	//	sysreg_write(MMUCR, mmucr);
	//
	//	asm volatile("tlbr" : : : "memory");
	//	cpu_sync_pipeline();
	//
	//	tlbehi = sysreg_read(TLBEHI);
	//	tlbelo = sysreg_read(TLBELO);

	//	mmucr_save = sysreg_read(MMUCR);
	//	tlbehi_save = sysreg_read(TLBEHI);

	AVR32_READ_SR_REG(AVR32_MMUCR, mmucr_save);
	AVR32_READ_SR_REG(AVR32_TLBEHI, tlbehi_save);
	mmu_read_entry(&tlbehi,&tlbelo, index, MMU_TLB_TYPE_DATA);

	printf("%2u: %c %c %02x   %05x %05x %u  %u  %c %c %c %c\n",
			index,
            (tlbehi & 0x200)?'1':'0',
            (tlbelo & 0x100)?'1':'0',
            (tlbehi & 0xff),
            (tlbehi >> 12), (tlbelo >> 12),
            (tlbelo >> 4) & 7, (tlbelo >> 2) & 3,
            (tlbelo & 0x200)?'1':'0',
            (tlbelo & 0x080)?'1':'0',
            (tlbelo & 0x001)?'1':'0',
            (tlbelo & 0x002)?'1':'0');

	//sysreg_write(MMUCR, mmucr_save);
	//sysreg_write(TLBEHI, tlbehi_save);

	AVR32_SET_SR_REG(AVR32_MMUCR, mmucr_save);
	AVR32_SET_SR_REG(AVR32_TLBEHI, tlbehi_save);

	//cpu_sync_pipeline();
    //local_irq_restore(flags);
}

void mmu_dump_tlb(void) {
	uint32_t i;

	printf("\n");
	printf("\n");
	printf("ID  V G ASID VPN   PFN   AP SZ C B W D\n");

	for (i = 0; i < MMU_DTLB_ENTRIES; i++) {
		mmu_show_tlb_entry(i);
	}
}


