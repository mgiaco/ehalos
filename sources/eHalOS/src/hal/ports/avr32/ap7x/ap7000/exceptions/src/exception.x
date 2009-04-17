#include <avr32/io.h>
//#define AVR32_ECR                                      0x00000010
//#define AVR32_RAR_EX                                   0x00000048
//#define AVR32_RAR_DBG                                  0x00000050

	.section  .gcc_except_table, "ax"
	.align  9
	// Start of Exception Vector Table.
	.global _evba
	.weak _evba
_evba:
	.org	0x0
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x4
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x8
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0xc
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x10
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x14
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x18
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x1c
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_breakpoint
	.org	0x20
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x24
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x28
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x2c
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x30
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x34
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x38
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x3c
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x40
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x44
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x50
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x60
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x70
	pushm	r8-r9, r10, r11, r12, lr
	rjmp	_handle_exception
	.org	0x100
	//pushm	r4-r7, r8-r9, r10, r11, r12, lr
	pushm	r0-r12, lr
	rjmp	_handle_scall

	.align	2
	.global _int0
	.global _int1
	.global _int2
	.global _int3

//_int0:
  //pushm r8-r9, r10, r11, r12, lr

  //mov     r12, 0  // Pass the int_lev parameter to the _get_interrupt_handler function.
  //call    _handle_interrupt
  //call 	  rtc_int_handler

  //cp.w    r12, 0  // Get the pointer to the interrupt handler returned by the function.
  //movne   pc, r12 // If this was not a spurious interrupt (R12 != NULL), jump to the handler.
  //popm	r8-r9, r10, r11, r12, lr
  //rete            // If this was a spurious interrupt (R12 == NULL), return from event handler.


_int0:
    nop
  	pushm	r8-r9, r10, r11, r12, lr
  	//pushm	r0-r12, lr
  	//pushm	r8-r9, lr // r8-r9 wird beim rtc_int_handler
  	call    rtc_int_handler
  	//popm	r8-r9, r10, r11, r12, lr
  	rete


.irp    priority,1,2,3
_int\priority:
#ifdef __AVR32_AVR32A__
	pushm	lr
#else
	//pushm	r8-r9, r10, r11, r12, lr
	pushm	r0-r12, lr
#endif
	mov	r12, \priority
	call	_handle_interrupt
#ifdef __AVR32_AVR32A__
	popm	lr
#else
	//popm	r8-r9, r10, r11, r12, lr
	popm 	r0-r12, lr
#endif
	rete
.endr


_handle_breakpoint:
	lda.w	r10, handler_table
#ifdef __AVR32_AVR32A__
	lddsp	r11, sp[4+6*4]
#else
	mfsr	r11, AVR32_RAR_DBG
#endif
	ld.w	r10, r10[0]
	mcall	r10[0x1c]
	cp	r12, 0
	breq	_breakpoint_unhandled
#ifdef __AVR32_AVR32A__
	stdsp	sp[4+6*4], r12
#else
	mtsr	AVR32_RAR_DBG, r12
#endif
	popm	r8-r9, r10, r11, r12, lr
	retd
_breakpoint_unhandled:
	lda.w	r12, exit
#ifdef __AVR32_AVR32A__
	stdsp	sp[4+6*4], r12
#else
	mtsr	AVR32_RAR_DBG, r12
#endif
	popm	r8-r9, r10, r11, r12, lr
	retd

_handle_exception:
	lda.w	r10, handler_table
	mfsr	r12, AVR32_ECR
#ifdef __AVR32_AVR32A__
	lddsp	r11, sp[4+6*4]
#else
	mfsr	r11, AVR32_RAR_EX
#endif
	ld.w	r10, r10[0]
	lsl	r12, 2 /* Shift to get evba offset. */
	add	r10, r10, r12
	mcall	r10[0]
_exception_return:
	cp	r12, 0
	breq	_exception_unhandled
#ifdef __AVR32_AVR32A__
	stdsp	sp[4+6*4], r12
#else
	mtsr	AVR32_RAR_EX, r12
#endif
	popm	r8-r9, r10, r11, r12,lr
	rete

_exception_unhandled:
	lda.w	lr, exit
#ifdef __AVR32_AVR32A__
	stdsp	sp[4+6*4], lr
#else
	mtsr	AVR32_RAR_EX, lr
#endif
	popm	r8-r9, r10, r11, r12, lr
	rete

_handle_scall:
	call	_int_disable
	lda.w	r4, handle_scall
	icall	r4
//	call 	_int_enable
	//popm	r4-r7, r8-r9, r10, r11, r12, lr
	popm	r0-r12, lr
	//csrf	16 // clear global interrupt mask (enable interrupts)
	csrf	17 // clear global interrupt mask (enable interrupts)
	rets


_int_disable:
	//ssrf	16 //set global interrupt mask (disable interrupts)
	ssrf	17
    nop
    nop
    nop
    nop
    nop
    nop
    mov		pc, lr // Restore Program Counter (return)

_int_enable:
	//csrf	16 // clear global interrupt mask (enable interrupts)
	csrf	17
    mov		pc,lr // Restore Program Counter (return)


