
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "port.h"
#include "sched.h"
#include "pcb.h"
#include "Loader.h"
#include "sys/sysregs.h"
#include "MemoryManagement.h"

PCtrlBlock_t *allocatePCBAndStack(unsigned int stackSize);
PCtrlBlock_t *allocatePCB();

static int32_t cnt = 'a';
static int32_t nextPid = 1;


uint32_t activeASID = -1;


BinImage BinImages[] =
{
	//128K Speicher
	{		0xA0500000, 	// load address on flash
			0x00010000,	 	// base address in RAM - must be at start of second page
			131071,		    // size in bytes
			0,   			// virtual address of first data, before text
			0xA0500000, 	// pointer to start of image data
			"IdleProcess" 	// name of app image - must call shell and loop for ever...
	},

	//128K Speicher
	{ 		0xA0520000, 	// load address on flash
			0x00010000,	 	// base address in RAM - must be at start of second page
			131071,		    // size in bytes
			0,   			// virtual address of first data, before text
			0xA0520000, 	// pointer to start of image data
			"Shell" 		// name of app image
	},

	// 128K Speicher
	{ 		0xA0540000, 	// load address on flash
			0x00010000,	 	// base address in RAM - must be at start of second page
			131071,			// size in bytes (128k - 1 Byte )
			0,   			// virtual address of first data, before text
			0xA0540000, 	// pointer to start of image data
			"SpaceInvaders" // name of app image
	},

	// 2M Speicher
	{ 		0xA0560000, 	// load address on flash
			0x00010000,	 	// base address in RAM - must be at start of second page
			2097151,		// size in bytes (2M - 1 Byte )
			0,   			// virtual address of first data, before text
			0xA0560000, 	// pointer to start of image data
			"ImageShow"		// name of app image
	}
};

uint32_t nrOfBinImages = (sizeof(BinImages)/sizeof(BinImage));

PCtrlBlock_t *mmuProcessCreate(uint32_t virtualLoadAddress, const char *pName) {

	PCtrlBlock_t * ptrNewPCB;

	// Allocate the memory required by the PCB and stack for the new process.
	// checking that the allocation was successful
	ptrNewPCB = allocatePCB();

	if (ptrNewPCB != NULL) {

		unsigned int *ptrTopOfStack;

		strcpy(ptrNewPCB->processName, pName);

		/* Calculate the top of stack address.  This depends on whether the
		 stack grows from high memory to low (as per the 80x86) or visa versa.
		 portSTACK_GROWTH is used to make the result positive or negative as
		 required by the port. */

		//TODO: check if  bla bla bla
		ptrTopOfStack = (unsigned int *)0x0;//ptrNewPCB->ptrToStack + ( stackSize - 1 );

		// init Register Stack
		port_init_stack2(ptrNewPCB->regStack, ptrTopOfStack, virtualLoadAddress, ptrNewPCB->pid);

	} else {

		// do nothing yet, memory overload

	}

	return ptrNewPCB;
}


PCtrlBlock_t *processCreate(processFkt pFkt, const char *pName, uint32_t stackSize) {

	PCtrlBlock_t * ptrNewPCB;

	// Allocate the memory required by the PCB and stack for the new process.
	// checking that the allocation was successful
	ptrNewPCB = allocatePCBAndStack(stackSize);

	if (ptrNewPCB != NULL) {

		uint32_t *ptrTopOfStack;

		///* Setup the newly allocated TCB with the initial state of the task. */
		//prvInitialiseTCBVariables(ptrNewPCB, pcName, uxPriority);
		strcpy(ptrNewPCB->processName, pName);

		/* Calculate the top of stack address.  This depends on whether the
		 stack grows from high memory to low (as per the 80x86) or visa versa.
		 portSTACK_GROWTH is used to make the result positive or negative as
		 required by the port. */

		ptrTopOfStack = ptrNewPCB->ptrToStack + ( stackSize - 1 );

		// init Register Stack
		port_init_stack(ptrNewPCB->regStack, ptrTopOfStack, pFkt);

	} else {

		// do nothing yet, memory overload

	}

	return ptrNewPCB;
}


PCtrlBlock_t *allocatePCB(){

	PCtrlBlock_t *ptrNewPCB;

	// allocate space for the PCB.
	ptrNewPCB = ( PCtrlBlock_t * ) malloc(sizeof(PCtrlBlock_t));

	if (ptrNewPCB != NULL) {

		// generate new PID
		ptrNewPCB->pid = nextPid++;

		// priority is 0 on startup, is only updated by scheduler for standard round robing, never
		// change it from outside, use instead sched_realtime_deadline (but not yet implemented) :)
		ptrNewPCB->sched_priority = 0;

		// standard state after creation of task
		ptrNewPCB->state = TASK_INITIALIZED;

		// set performance measuring members to 0
		ptrNewPCB->cpu_time_running = 0;
		ptrNewPCB->cpu_time_waiting = 0;

	}
	return ptrNewPCB;
}


PCtrlBlock_t *allocatePCBAndStack(unsigned int stackSize){

	PCtrlBlock_t *ptrNewPCB;

	// allocate space for the PCB
	ptrNewPCB = ( PCtrlBlock_t * ) malloc(sizeof(PCtrlBlock_t));

	if (ptrNewPCB != NULL) {

		// generate new PID
		ptrNewPCB->pid = nextPid++;

		// priority is 0 on startup, is only updated by scheduler for standard round robing, never
		// change it from outside, use instead sched_realtime_deadline (but not yet implemented) :)
		ptrNewPCB->sched_priority = 0;

		// standard state after creation of task
		ptrNewPCB->state = TASK_INITIALIZED;

		// set performance measuring members to 0
		ptrNewPCB->cpu_time_running = 0;
		ptrNewPCB->cpu_time_waiting = 0;

		// memory for stack of application
		ptrNewPCB->ptrToStack = ( uint32_t * ) malloc( ( ( size_t )stackSize ) * sizeof(uint32_t));

		if ( ptrNewPCB->ptrToStack == NULL  ) {

			// Could not allocate the stack.  Delete the allocated TCB.
			free(ptrNewPCB);
			ptrNewPCB = NULL;

		} else {

			// fill up stack for debgging only
			memset(ptrNewPCB->ptrToStack, cnt++, stackSize * sizeof(uint32_t));

			// for Debugging fill up Register Stack
			// memset(ptrNewPCB->regStack, 'r' , REGISTER_STACK_SIZE * sizeof(unsigned int));
		}
	}
	return ptrNewPCB;
}


void setupFirstProcess() {

	PCtrlBlock_t *pPCB_idle = mmuProcessCreate(BinImages[0].baseAddress, BinImages[0].pImageName);

	pPCB_idle->state = TASK_RUNNABLE;

	init_scheduler(pPCB_idle, NULL);

	actualiseASID(pPCB_idle->pid);
	activeASID = pPCB_idle->pid;

	// loads Idle task as first process in HALOS (BinImages[0])
	//loadBinApplication(&(BinImages[0]), pPCB_idle->pid);
	loadProcessInMemorySystem(pPCB_idle->pid, &(BinImages[0]));

	ptrIdleTask = pPCB_idle;
}


void startFirstProcess() {

	start_first_process(ptrIdleTask);
}


void start_first_process(PCtrlBlock_t *pcb) {

	// we are currently in supervisor mode
	asm volatile ("mov r1,%0" : : "g" (pcb->regStack));

	asm volatile ("ld.w r0,r1++");
	asm volatile ("mtsr %0,r0" :: "g" (SYSREG_RSR_SUP));

	// r0 = PC from stack
	asm volatile ("ld.w r0,r1++");
	asm volatile ("mtsr %0,r0" :: "g" (SYSREG_RAR_SUP));

	// restore R0 - R12 + LR + SP
	asm volatile ("ldmts r1++, r0-r14 \t\n" ::);

	asm volatile ("rets \t" ::);
}



void setup_and_start_new_process(char *processName) {

	BinImage *pBinImage = NULL;
	PCtrlBlock_t *pPCB_new = NULL;
	int8_t index;

	uart1_puts(processName);

	// search through all BinImages
		for(index=0; index<nrOfBinImages; index++){

		if (!strcmp(BinImages[index].pImageName, processName))
			pBinImage = &(BinImages[index]);
	}

	if (pBinImage == NULL){
		uart1_puts("processName not found - use SpaceInvaders instead");
		pBinImage = &(BinImages[2]);
	}

	pPCB_new = mmuProcessCreate(pBinImage->baseAddress, pBinImage->pImageName);

	// for correct loading of TLB, updating
	actualiseASID(pPCB_new->pid);
	activeASID = pPCB_new->pid;

	//loadBinApplication(pBinImage, pPCB_new->pid);
	loadProcessInMemorySystem(pPCB_new->pid, pBinImage);

	// to guarantee that the new process is executed next... (to refresh TLB with first memory-access)
	pPCB_new->sched_priority = 200000;

	// puts task into the scheduler, task will be executed on the next time-slice
	changeTaskStateAndReorg(pPCB_new, TASK_RUNNABLE);

	//Set new process as forground always the same
	set_foreground_process(pPCB_new);

	// return to old state
	actualiseASID(ptrRunningTask->pid);
	activeASID = ptrRunningTask->pid;

	//mmu_dump_tlb();
}

