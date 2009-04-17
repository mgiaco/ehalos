/* This file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief NEWLIB_ADDONS system calls (sim) overloading file for AVR32.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
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


	.global _init_argv_sim
	.weak _init_argv_sim

	.text
_init_argv_sim:
	pushm   lr
	cop	CP6, CR12, CR11, CR10, 0x7
	mvcr.w	CP6, R12, CR12
	mcall	.Lmalloc
	MOV	R11, R12
	mvrc.w	CP6, CR12, R12
	cop	CP6, CR12, CR11, CR10, 0x8
	mvcr.w	CP6, R12, CR12
	popm    pc

	.align 2
.Lmalloc:
	.word malloc

	.global	_open_sim
	.weak _open_sim

	.text
_open_sim:
	mvrc.w	CP6, CR12, R12
	mvrc.w	CP6, CR11, R11
	mvrc.w	CP6, CR10, R10
	cop	CP6, CR12, CR11, CR10, 0x0
	mvcr.w	CP6, R12, CR12
	retal	r12

	.global	_close_sim
	.weak _close_sim
_close_sim:
	mvrc.w	CP6, CR12, R12
	cop	CP6, CR12, CR11, CR10, 0x1
	mvcr.w	CP6, R12, CR12
	retal	r12

	.global	_read_sim
	.weak _read_sim
_read_sim:
	mvrc.w	CP6, CR12, R12
	mvrc.w	CP6, CR11, R11
	mvrc.w	CP6, CR10, R10
	cop	CP6, CR12, CR11, CR10, 0x2
	mvcr.w	CP6, R12, CR12
	retal	r12



	.global	_write_sim
	.weak _write_sim
_write_sim:
	mvrc.w	CP6, CR12, R12
	mvrc.w	CP6, CR11, R11
	mvrc.w	CP6, CR10, R10
	cop	CP6, CR12, CR11, CR10, 0x3
	mvcr.w	CP6, R12, CR12
	retal	r12


	.global	_lseek_sim
	.weak _lseek_sim
_lseek_sim:
	mvrc.w	CP6, CR12, R12
	mvrc.w	CP6, CR11, R11
	mvrc.w	CP6, CR10, R10
	cop	CP6, CR12, CR11, CR10, 0x4
	mvcr.w	CP6, R12, CR12
	retal	r12


	.global	_exit_sim
	.weak _exit_sim
_exit_sim:
	mvrc.w	CP6, CR12, R12
	cop	CP6, CR12, CR11, CR10, 0x6
loop:   rjmp loop;
