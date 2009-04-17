#include "port.h"


/**
 * @def HALOS_SR_M0_MASK
 * Status Register, Supervisor Execution Mode Mask
 */
#define  HALOS_SR_M0_MASK              0x00000000


void port_init_stack(unsigned int *ptrTopOfRegStack,unsigned int *ptrTopOfStack, processFkt ptrTaskCode) {

	//Setup the initial stack of the task.  The stack is set exactly as
	//expected by the portRESTORE_CONTEXT() macro.

	//The last thing onto the stack is the status register, which is set for
	//system mode, with interrupts enabled.

	*ptrTopOfRegStack++ = ( unsigned int ) HALOS_SR_M0_MASK; // SR
	*ptrTopOfRegStack++ = ( unsigned int ) ptrTaskCode; // R15 (PC)

	*ptrTopOfRegStack++ = ( unsigned int ) 14; // R14 (LR)
	*ptrTopOfRegStack++ = ( unsigned int ) ptrTopOfStack; // R13 (SP)
	*ptrTopOfRegStack++ = ( unsigned int ) 12; // R12
	*ptrTopOfRegStack++ = ( unsigned int ) 11; // R11
	*ptrTopOfRegStack++ = ( unsigned int ) 10; // R10
	*ptrTopOfRegStack++ = ( unsigned int ) 9; // R9
	*ptrTopOfRegStack++ = ( unsigned int ) 8; // R8
	*ptrTopOfRegStack++ = ( unsigned int ) 7; // R7
	*ptrTopOfRegStack++ = ( unsigned int ) 6; // R6
	*ptrTopOfRegStack++ = ( unsigned int ) 5; // R5
	*ptrTopOfRegStack++ = ( unsigned int ) 4; // R4
	*ptrTopOfRegStack++ = ( unsigned int ) 3; // R3
	*ptrTopOfRegStack++ = ( unsigned int ) 2; // R2
	*ptrTopOfRegStack++ = ( unsigned int ) 1; // R1
	*ptrTopOfRegStack =   ( unsigned int ) 0; // R0
}


void port_init_stack2(unsigned int *ptrTopOfRegStack,unsigned int *ptrTopOfStack, uint32_t virtualLoadAddress, uint32_t pid) {

	//Setup the initial stack of the task.  The stack is set exactly as
	//expected by the portRESTORE_CONTEXT() macro.

	//The last thing onto the stack is the status register, which is set for
	//system mode, with interrupts enabled.

	*ptrTopOfRegStack++ = ( unsigned int ) HALOS_SR_M0_MASK; // SR
	*ptrTopOfRegStack++ = ( unsigned int ) virtualLoadAddress; // R15 (PC)

	*ptrTopOfRegStack++ = ( unsigned int ) 14; // R14 (LR)
	*ptrTopOfRegStack++ = ( unsigned int ) ptrTopOfStack; // R13 (SP)
	*ptrTopOfRegStack++ = ( unsigned int ) 12; // R12
	*ptrTopOfRegStack++ = ( unsigned int ) 11; // R11
	*ptrTopOfRegStack++ = ( unsigned int ) 10; // R10
	*ptrTopOfRegStack++ = ( unsigned int ) 9; // R9
	*ptrTopOfRegStack++ = ( unsigned int ) 8; // R8
	*ptrTopOfRegStack++ = ( unsigned int ) 7; // R7
	*ptrTopOfRegStack++ = ( unsigned int ) 6; // R6
	*ptrTopOfRegStack++ = ( unsigned int ) 5; // R5
	*ptrTopOfRegStack++ = ( unsigned int ) 4; // R4
	*ptrTopOfRegStack++ = ( unsigned int ) pid; // R3
	*ptrTopOfRegStack++ = ( unsigned int ) 2; // R2
	*ptrTopOfRegStack++ = ( unsigned int ) 1; // R1
	*ptrTopOfRegStack =   ( unsigned int ) 0; // R0
}

