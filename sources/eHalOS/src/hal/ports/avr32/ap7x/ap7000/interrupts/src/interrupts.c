/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief NEWLIB_ADDONS interrupt handling file for AVR32.
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
#include <stdlib.h>
#include <stdarg.h>
#include "../../newlib/nlao_io.h"
#include "../inc/interrupts.h"

//#define AVR32_SR                                       0x00000000
//#define AVR32_SR_GM                                            16
//#define AVR32_SR_I0M                                           17


struct int_handler_elmt {
  __newlib_int_handler int_handler_func;
  void *user_handle;
};

static struct int_handler_elmt (*int_handler_table)[INT_GRPS][INT_LINES] = 0;

static void *__intc_base__ = (void *)0xFFF00400;

void set_interrupts_base(void *base){
  __intc_base__ = base;
}

static int int_initialized = 0;

void __attribute__((weak))
     unhandled_interrupt(int int_grp, void *user_handle){
#ifdef DEBUG_MSG_ON
  fprintf(stderr, "Unhandled Interrupt:\n\tGroup: %i\n", int_grp);
#endif

  while (1);  // Catch unregistered interrupts.
}


void __attribute__((weak))
     init_interrupts(){
  int int_grp, line, regval;
  extern void _evba;
  extern void _int0;
  int_initialized = 1;

  if ( int_handler_table )
    free(int_handler_table);

  int_handler_table = (void *)malloc(INT_GRPS*INT_LINES*sizeof(struct int_handler_elmt));

  regval = (INT0 << INTLEVEL) | (&_int0 - &_evba);

  // For all interrupt groups,
  for (int_grp = 0; int_grp < INT_GRPS; int_grp++) {
    // For all interrupt request lines of each group,
    for (line = 0; line < INT_LINES; line++) {
      // Assign unhandled_interrupt as default interrupt handler.
      (*int_handler_table)[int_grp][line].int_handler_func = unhandled_interrupt;
      (*int_handler_table)[int_grp][line].user_handle = (void *)0;
    }

    writel(regval, INTPR_BASE + (int_grp << 2));
  }
}


void __attribute__((weak))
     _handle_interrupt(int int_level){
  int group = readl(INTCAUSE_BASE + ((3 - int_level) << 2));
  int line = readl(INTREQ_BASE + (group << 2));
  line = __builtin_ctz(line);

  if(NULL == (*int_handler_table)[group][line].user_handle){
//	  while(1){
//
//	  }
  }else{
	  (*int_handler_table)[group][line].int_handler_func(group, (*int_handler_table)[group][line].user_handle);
  }
}


__newlib_int_handler __attribute__((weak))
     register_interrupt(__newlib_int_handler handler, int int_grp, int line, int priority,
                        ...){
  extern void _evba;
  extern void _int0;
  extern void _int1;
  extern void _int2;
  extern void _int3;
  va_list ap;

  va_start(ap, priority);

  if ( !int_initialized )
    init_interrupts();

  __newlib_int_handler old_handler = (*int_handler_table)[int_grp][line].int_handler_func;
  (*int_handler_table)[int_grp][line].int_handler_func = handler;
  (*int_handler_table)[int_grp][line].user_handle = va_arg(ap,void *);

  //Set priority
  priority &= 0x3; //Only two bits
  switch (priority){
  case 0:
    writel((INT0 << INTLEVEL) | (&_int0 - &_evba), INTPR_BASE + (int_grp << 2));
    break;
  case 1:
    writel((INT1 << INTLEVEL) | (&_int1 - &_evba), INTPR_BASE + (int_grp << 2));
    break;
  case 2:
    writel((INT2 << INTLEVEL) | (&_int2 - &_evba), INTPR_BASE + (int_grp << 2));
    break;
  case 3:
    writel((INT3 << INTLEVEL) | (&_int3 - &_evba), INTPR_BASE + (int_grp << 2));
    break;
  }

  //Enable interrupts for this priority
  //__builtin_mtsr(AVR32_SR, __builtin_mfsr(AVR32_SR) & ~(AVR32_SR_GM_MASK | (AVR32_SR_I0M_MASK << priority)));

  return old_handler;
}
