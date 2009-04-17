/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief NEWLIB_ADDONS system calls overloading file for AVR32.
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


#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "nlao_io.h"
//#include "../inc/nlao_usart.h"
#include "../interrupts/inc/interrupts.h"
#include "../exceptions/inc/exceptions.h"

#define AVR32_COUNT                                    0x00000108



int  __attribute__((weak))
  unimplemented_syscall(char *name){
#ifdef DEBUG_MSG_ON
  fprintf(stderr, "Error: Unimplemented syscall: %\n", name);
#endif
  return -1;
}

int  __attribute__((weak))
     _init_startup ()
{
  extern void _evba;
  void *handler_table;
  // Dynamically alloc the exception handlers table.
  handler_table = (void *)malloc(0x104);
  if (handler_table != (void *)0)
  {
    // Init the exception handlers table.
    init_exceptions((void *)&_evba, handler_table);
  }
  else
  {
    // The dynamic allocation failed: we might as well stay here forever.
    while (1);
  }
  return 1; // Use the same meaning as IAR: init the data segment when 1 is returned.
}

int __attribute__((weak))
     isatty (int desc){ return 0;}

int __attribute__((weak))
     _fstat (int filedes, void *ptr){ return -1;}

int __attribute__((weak))
     _stat (const char *filename, void *ptr){ return -1;}

int __attribute__((weak))
     _fcntl (){ return unimplemented_syscall("_fcntl");}

int __attribute__((weak))
     _link (){ return unimplemented_syscall("_link");}

int __attribute__((weak))
     _kill (){ return unimplemented_syscall("_kill");}

int __attribute__((weak))
     _getpid (){ return unimplemented_syscall("_getpid");}

int __attribute__((weak))
     _gettimeofday (){ return unimplemented_syscall("_gettimeofday");}

int __attribute__((weak))
     _unlink (){ return unimplemented_syscall("_unlink");}

int __attribute__((weak))
     _execve (){ return unimplemented_syscall("_execve");}

int __attribute__((weak))
     _fork (){ return unimplemented_syscall("_fork");}

int __attribute__((weak))
     _wait (){ return unimplemented_syscall("_wait");}

int __attribute__((weak))
     sigfillset (){ return unimplemented_syscall("sigfillset");}

int __attribute__((weak))
     sigprocmask (){ return unimplemented_syscall("sigprocmask");}


clock_t __attribute__((weak))
     _times( struct tms *tms )
{
  int tick;
  tick = __builtin_mfsr(AVR32_COUNT);
  tms->tms_utime = 0;
  tms->tms_stime = tick;
  tms->tms_cutime = 0;
  tms->tms_cstime = 0;
  return tick;
}

extern void __heap_start__, __heap_end__;


void * __attribute__((weak))
     _sbrk(int increment)
{
  static void *cur_heap_pos = 0;

  //Initialize cur_heap_pos
  if ( cur_heap_pos == 0 )
    cur_heap_pos = &__heap_start__;

  if ( (cur_heap_pos + increment) <= &__heap_end__ ){
    void *heap_pos = cur_heap_pos;
    cur_heap_pos += increment;
    return heap_pos;
  } else {
#ifdef DEBUG_MSG_ON
    fprintf(stderr, "Warning: Heap is running full trying to allocate %i bytes!!!!\n", increment);
    fprintf(stderr, "\tHeap start address\t= 0x%x\n", &__heap_start__);
    fprintf(stderr, "\tHeap end address\t= 0x%x\n", &__heap_end__);
    fprintf(stderr, "\tCurrent heap address\t= 0x%x\n", cur_heap_pos);
    fflush(stderr);
#endif
    errno = ENOMEM;
    return (void *)-1;
  }
  return (void *)-1;
}


/**
 * Low-level open operator.
 * Will return integer 3 if you open the "/dev/fb" framebuffer.
 * The user will never use this command, only call fopen("fb", "a") to get a
 * FILE handle.
 * This can in the future be extended to open MMC files.
 */
/*int __do_not_use_oscall_coproc__;
static void *do_not_use_oscall_coproc = &__do_not_use_oscall_coproc__;*/
extern void _init_argv_sim();
extern int _open_sim(char *pathname, int flags, int mode);
extern int _close_sim(int filedes);
extern int _lseek_sim(int filedes, int offset, int whence);
extern void _exit_sim(int code);
extern int _read_sim(int file, char * ptr, int len);
extern int _write_sim(int file, char * ptr, int len);

void __attribute__((weak))
     _init_argv ()
{
/*  if ( !do_not_use_oscall_coproc )
    _init_argv_sim();*/
}

int __attribute__((weak))
     _open (char *pathname, int flags, int mode)
{
/*  if ( !do_not_use_oscall_coproc )
    return _open_sim(pathname, flags, mode);*/

  /*
    if(strcmp(pathname, "/dev/fb" )==0){
    avr32fb_init();
    return 3;
    }
  */
  return unimplemented_syscall("_open");
}

int __attribute__((weak))
     _close (int file)
{
/*  if ( !do_not_use_oscall_coproc )
    return _close_sim(file);*/
  return unimplemented_syscall("_close");
}

int __attribute__((weak))
     _lseek( int filedes, int offset, int whence )
{
/*  if ( !do_not_use_oscall_coproc )
    return _lseek_sim(filedes, offset, whence);*/
  return unimplemented_syscall("_lseek");
}

void __attribute__((weak))
     _exit( int code )
{
/*  if ( !do_not_use_oscall_coproc )
    return _exit_sim(code);*/

  //Signal exit
  printf("\004%d", code);

  //flush all pending writes
  fflush(stdout);
  fflush(stderr);

  while (1);
}

int __attribute__((weak))
_read (int file,
       char * ptr,
       int len){
  int i;

/*  if ( !do_not_use_oscall_coproc )
    return _read_sim(file, ptr, len);*/

  if (file != 0)
    return unimplemented_syscall("_read with filedes != 0");

  for ( i = 0; i < len; i++ ){
    //ptr[i] = (char)usart_getc();
  }

  return len;
}

/**
 * Low-level write command.
 * When newlib buffer is full or fflush is called, this will output
 * data to correct location.
 * 1 and 2 is stdout and stderr which goes to usart
 * 3 is framebuffer
 */
int __attribute__((weak))
_write (int file,
        char * ptr,
        int len){
  int i;

/*  if ( !do_not_use_oscall_coproc )
    return _write_sim(file, ptr, len);*/

  if ( (file != 1)
       && (file != 2) && (file!=3))
    return unimplemented_syscall("_write with file != 1 or 2");
  /*  if(file==3){
      for(i = 0 ; i < len; i++){
      avr32fb_putchar(ptr[i]);
      }
      } else */{
    for ( i = 0; i < len; i++ ){
      //usart_putc(ptr[i]);
    }
  }

  return len;
}
