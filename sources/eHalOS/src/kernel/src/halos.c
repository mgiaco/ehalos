

// Globale Variablen deklarieren, achtung Varibalen werden nicht initialiesiert!
#define EXTERN
#include "../halos_globals.h"

#include "halos.h"

#include "usart_isr.h"
#include "my_usart.h"


/**
 * ATTENTION: if TASK_SW active ==> linker file settings has to be "low" memory, otherwise "high" memory
 */
//#define TASK_SW

void rtc_int_handler() __attribute__ ((naked));

/**
 * @name PCBs for SIMPLE TASK_SWITCH_TESTING (no MMU and no scheduling)
 */
volatile PCtrlBlock_t *pcb1 = NULL;
volatile PCtrlBlock_t *pcb2 = NULL;

/**
 * @name Flags for SIMPLE SIMPLE TASK_SWITCH_TESTING (no MMU and no scheduling)
 */
volatile int flag = 0;
volatile int flag2 = 0;
volatile uint64_t sysTick;


uint64_t get_time(){
	return sysTick;

}

void _init_startup(void) {
	extern void _evba;
	void *handler_table;
	handler_table = (void *) malloc(0x104);
	init_exceptions((void *) &_evba, handler_table);
}


void rtc_int_handler() {

	asm volatile ("ssrf	16 \t\n" ::); //set global interrupt mask (disable interrupts)
	asm volatile ("nop \t\n" ::);
	asm volatile ("nop \t\n" ::);
	asm volatile ("nop \t\n" ::);
	asm volatile ("nop \t\n" ::);
	asm volatile ("nop \t\n" ::);
	asm volatile ("nop \t\n" ::);

	// if we just come from supervisor mode no switch has to be done
	if (!(__builtin_mfsr(SYSREG_RSR_INT0) & SR_M0)) {

		// storing register R0
		asm volatile ("st.w %0,r0 \t\n" :: "g" (&ptrRunningTask->regStack[REGISTER_0_OFFSET]) );
		asm volatile ("mov r0,%0 \t\n" :: "g" (ptrRunningTask->regStack) );

		asm volatile ("popm	r8-r9, r10, r11, r12, lr \t\n" ::);
		//asm volatile ("popm	r0-r12, lr \t\n" ::);

		// -4 cause register R0 is already on the register stack!
		asm volatile ("sub r0, -(16*4) \t\n" :: );

		// storeing r1 - r14
		asm volatile ("stmts --r0,r1-r14 \t\n" ::);

		// storing registers r0 - r14 to user stack
		// storing register rar_int0
		asm volatile ("mfsr r1,%0 \t\n" :: "g" (SYSREG_RAR_INT0));
		asm volatile ("stmts --r0,r1 \t\n" ::);

		// storing register rsr_int0
		asm volatile ("mfsr r1,%0 \t\n" :: "g" (SYSREG_RSR_INT0));
		asm volatile ("stmts --r0,r1 \t\n" ::);


#ifdef TASK_SW   // SIMPLE TASK_SWITCH_TESTING: without MMU and scheduler support

		schedule();

//		// change current running task between task1 and task2
//		if (flag == 0) {
//			ptrRunningTask = pcb1;
//			flag = 1;
//		} else if (flag == 1) {
//			ptrRunningTask = pcb2;
//			flag = 0;
//		} else {
//
//			// for debug reasons only, should never reached
//			while (1) {
//			}
//		}


#else  // TASK SWITCHING with MMU and scheduler support

		schedule();
		check_ressource(ptrRunningTask);
		actualiseASID(ptrRunningTask->pid);
		activeASID = ptrRunningTask->pid;

#endif

		sysTick++;

		// the flag can be cleared by writing TOPI in ICR to one
		AVR32_RTC.ICR.topi = 1;

		asm volatile ("mov r0,%0" :: "g" (ptrRunningTask->regStack));

		// get status register SR
		asm volatile ("ld.w r1,r0++ \t\n" ::);
		asm volatile ("mtsr %0,r1 \t\n" :: "g" (SYSREG_RSR_INT0));

		// get program counter PC
		asm volatile ("ld.w r1,r0++ \t\n" ::);
		asm volatile ("mtsr %0,r1 \t\n" :: "g" (SYSREG_RAR_INT0));

		asm volatile ("ldmts r0++, r0-r14 \t\n" ::);

	} else {

		sysTick++;
		AVR32_RTC.ICR.topi = 1;
		asm volatile ("popm	r8-r9, r10, r11, r12, lr \t\n" ::);
		//asm volatile ("popm	r0-r12, lr \t\n" ::);
	}

	asm volatile ("csrf	16 \t\n" ::); // clear global interrupt mask (enable interrupts)
	asm volatile ("rete \t\n" ::);
}



void init_rtc(unsigned int hz) {

	unsigned int Frtc = 16000;
	unsigned int top = Frtc / hz;

	// @see datasheet on page 120
	// fRTC = 2^-(PSEL+1) * 32KHz
	AVR32_RTC.CTRL.psel = 0; /* Set Frtc = 16 KHz */
	AVR32_RTC.CTRL.topen = 1; /* Count to TOP */
	// 32786Hz/2 => 1/f = T => 30usec/2 ==> *8000 => ~2Hz
	AVR32_RTC.top = top; /* Set F = Frtc / Hz */

	// Interrupt Setup
	AVR32_RTC.ICR.topi = 1; /* Clear pending interrupts */
	AVR32_RTC.IER.topi = 1; /* Enable RTC interrupt */

	// turn on RTC
	AVR32_RTC.CTRL.en = 1; /* Enable RTC */

}

static int TIMESLICE = 100;

static int data_a = 3;

int main(void) {

	int data_b = 3;

	if (data_a == data_b) {
		data_a = 5;
	}

	// setup processor frequency and SDRAM refresh timing
	low_level_init();

	// setup the interrupt handler
	set_interrupts_base((void*) AVR32_INTC_ADDRESS);

	// init_interrupts has to be called first, otherwise we are in a wrong mode
	init_interrupts();

	register_interrupt((rtc_int_handler), AVR32_RTC_IRQ/32, AVR32_RTC_IRQ % 32, INT0);

	_register_scall_handler((void*) (scall_Handler));

	// turn off interrupts
	Disable_global_interrupt();
	// init HALOS drivers
	init_all_drivers();


#ifdef TASK_SW  // SIMPLE TASK_SWITCH_TESTING: without MMU and scheduler support

	pcb1 = processCreate(shell_process, "PCB1", 200);
	pcb2 = processCreate(user_process2, "PCB2", 200);


//	ptrRunningTask = pcb1;

	init_scheduler(pcb2, NULL);
	// puts task into the scheduler, task will be executed on the next time-slice
	changeTaskStateAndReorg(pcb1, TASK_RUNNABLE);

	init_rtc(TIMESLICE);


	set_foreground_process(ptrRunningTask);
	start_first_process(ptrRunningTask);




#else  // TASK SWITCHING with MMU and scheduler support



	initMemoryManagement();

	setupFirstProcess();

	init_rtc(TIMESLICE);

	startFirstProcess();

#endif


	while (1) {
		// endless loop
	}

	// should never reached
	return 0;
}




