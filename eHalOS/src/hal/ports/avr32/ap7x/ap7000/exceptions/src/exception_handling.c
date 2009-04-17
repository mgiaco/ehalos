/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief NEWLIB_ADDONS exception handling file for AVR32.
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


#include <avr32/io.h>
#include <stdio.h>
#include "../inc/exceptions.h"
#include "graphics.h"
#include "font.h"
#include "halosUtil.h"

//#define AVR32_SR                                       0x00000000
//#define AVR32_SR_EM                                            21
//#define AVR32_EVBA                                     0x00000004

__exception_handler *handler_table = 0;


void __attribute__((weak))
     _register_exception_handler(__exception_handler handler, int evba_offset){
  int index = evba_offset >> 2;
  if ( index < 0 ){
#ifdef DEBUG_MSG_ON
    fprintf(stderr, "Illegal EVBA-Offset given to _register_exception_handler()\n");
#endif
    return;
  }

  if ( !handler_table ){
#ifdef DEBUG_MSG_ON
    fprintf(stderr, "Error: init_exceptions() must be called before registering\n" \
            "exception handlers!\n");
#endif
    return;
  }

  handler_table[index] = handler;

}

void __attribute__((weak))
     _register_scall_handler(__scall_handler handler){
  handler_table[EVBA_SCALL >> 2] = (__exception_handler)handler;
}


void __attribute__((weak))
     handle_scall(int code, int param1, int param2, int param3, int param4){
  ((__scall_handler)handler_table[EVBA_SCALL >> 2])(code, param1, param2, param3, param4);
}

/*
   Function for handling exception which have got no designated handler. Takes evba_offset
   and lr value as input parameters. Should return the address to return from exception to
   or 0 if the exception was unhandled.
*/
unsigned int __attribute__((weak)) unhandled_exception(int evba_offset, int return_address){

	int tmp;

//#ifdef DEBUG_MSG_ON
  //fprintf(stderr, "Unhandled Exception:\n\t");
  switch ( evba_offset ){
  case EVBA_UNRECOVERABLE:
	  tmp = 1;
    //fprintf(stderr, "Unrecoverable");
    break;
  case EVBA_TLB_MULTIPLE:
	  tmp = 2;
    //fprintf(stderr, "TLB Multiple Hit");
    break;
  case EVBA_BUS_ERROR_DATA:
	  tmp = 3;
    //fprintf(stderr, "Bus Error Data Fetch BEAR=0X%08X", __builtin_mfsr(AVR32_BEAR));
    break;
  case EVBA_BUS_ERROR_INSTR:
	  tmp = 4;
    //fprintf(stderr, "Bus Error instruction Fetch");
    break;
  case EVBA_NMI:
	  tmp = 5;
    //fprintf(stderr, "NMI");
    break;
  case EVBA_INSTR_ADDR:
	  tmp = 6;
    //fprintf(stderr, "Instruction Address");
    break;
  case EVBA_ITLB_PROT:
	  tmp = 7;
    //fprintf(stderr, "ITLB Protection");
    break;
  case EVBA_BREAKPOINT:
	  tmp = 8;
    //fprintf(stderr, "Breakpoint");
    break;
  case EVBA_ILLEGAL_OPCODE:
	  tmp = 9;
    //fprintf(stderr, "Illegal Opcode");
    break;
  case EVBA_UNIMPLEMENTED:
	  tmp = 10;
    //fprintf(stderr, "Unimplemented Instruction");
    break;
  case EVBA_PRIVILEGE_VIOL:
	  tmp = 11;
    //fprintf(stderr, "Privilege Violation");
    break;
  case EVBA_FLOATING_POINT:
	  tmp = 12;
    //fprintf(stderr, "Floating Point");
    break;
  case EVBA_COP_ABSENT:
	  tmp = 13;
    //fprintf(stderr, "Coprocessor Absent");
    break;
  case EVBA_DATA_ADDR_R:
	  tmp = 14;
    //fprintf(stderr, "Data Address (Read)");
    break;
  case EVBA_DATA_ADDR_W:
	  tmp = 15;
    //fprintf(stderr, "Data Address (Write)");
    break;
  case EVBA_DTLB_PROT_R:
	  tmp = 16;
    //fprintf(stderr, "DLTB Protection (Read)");
    break;
  case EVBA_DTLB_PROT_W:
	  tmp = 17;
    //fprintf(stderr, "DTLB Protection (Write)");
    break;
  case EVBA_DTLB_MODIFIED:
	  tmp = 18;
    //fprintf(stderr, "DTLB Modified");
    break;
  case EVBA_ITLB_MISS:
	  tmp = 19;
    //fprintf(stderr, "ITLB Miss");
    break;
  case EVBA_DTLB_MISS_R:
	  tmp = 20;
    //fprintf(stderr, "DTLB Miss (Read)");
    break;
  case EVBA_DTLB_MISS_W:
	  tmp = 21;
    //fprintf(stderr, "DTLB Miss (Write)");
    break;
  case EVBA_SCALL:
	  tmp = 22;
    //fprintf(stderr, "Scall");
    break;
  default:
    //fprintf(stderr, "Unknown EVBA offset!");
	break;
  }

  //fprintf(stderr, "\n\tat pc = 0x%x\n", return_address);
//#endif

  draw_kernel_panic(tmp);
  while (1);  // Catch unregistered exceptions.

  return 0;
}

void draw_kernel_panic(int tmp){
	char buffer[] = {"KERNEL PANIC\r\n"};

	setBackgroundColor(0xFF0000);
	fillRectangle(0,0, 480, 272);
	gotoXY(240, 136);
	glcd_set_text_justify(1,1);

	setColor(0xFFFFFF);
	glcd_draw_string(buffer);

}


void __attribute__((weak))
  init_exceptions(void *evba, void *_handler_table){

  int sr;
  int handlers = (0x104 / sizeof(__exception_handler));
  int  i;
  //Init EVBA
  __builtin_mtsr(AVR32_EVBA, (int)evba);

  //Enable Exceptions
  sr = __builtin_mfsr(AVR32_SR);
  sr &= ~AVR32_SR_EM_MASK;
  __builtin_mtsr(AVR32_SR, sr);

  //Initialize the exception handlers to default
  handler_table = (__exception_handler *)_handler_table;

  for ( i = 0; i < handlers; i++ ){
    handler_table[i] = unhandled_exception;
  }
}
