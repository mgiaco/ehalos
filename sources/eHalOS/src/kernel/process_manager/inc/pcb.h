/**
 * @file pcb.h
 * @brief Process Control Block for Scheduler
 * @author Team Halos
 * @version 0.0.1
 * @date 2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup PROCESS_MANAGER Process Manager
 *  @ingroup KERNEL
 */


/**
 *  @defgroup PCB Process Control Block (PCB)
 *  @ingroup PROCESS_MANAGER
 *  @brief Process Control Block (PCB), structs and routines for creating tasks (PCB´s),
 *  MMU initializing and starting tasks
 *
 *  @note ...
 *  @author maex.sp Markus Speckle, christian, giaco
 *  @version 0.0.1
 */

/**@{*/

#ifndef PCB_H_
#define PCB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "port.h"
#include "Loader.h"


/**
 * @brief constant for stack size
 */
#define REGISTER_STACK_SIZE 	20

/**
 * @brief constant for register 0 offset
 */
#define REGISTER_0_OFFSET 		16

/**
 * @brief Binary images for the tasks we want to load (holds all the memory adresses to our
 * applications like idle-task, shell, space invaders, ...)
 * [0] = idle process
 * [1] = shell process
 * [2] = space invaders game
 */
extern BinImage BinImages[];

/**
 * @brief current active ASID for MMU
 */
extern uint32_t activeASID;

/**
 * @struct processControlBlock
 * @brief Control Block for holding all the task informations, used mainly by the schuduler
 */
typedef struct processControlBlock {

	/** @brief Register Stack Array */
	unsigned int regStack[REGISTER_STACK_SIZE];

	// Points to the location of the last item placed
	// on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE STRUCT.
	// volatile unsigned int *ptrTopOfStack;

	/** @brief Points to the start of the stack */
	uint32_t *ptrToStack;

	/** @brief Descriptive name given to the task when created.  Facilitates debugging only. */
	char processName[20];

	/** @brief Pagetable, pointer to the Page Global Directory of the task, every task has exactly one Page Global Directory. */
	unsigned long pgd;

	/** @brief For defined states scheduler in @file sched.h */
	uint32_t pid;

	/** @brief used for task states, @see sched.h - Constants for the different task states */
	volatile long state;

	/** @brief creation timestamp */
    unsigned long start_time;

    /** @brief cputime (in running mode)... jiffies / how long already running / for performance counting */
    unsigned long cpu_time_running;

    /** @brief cputime (in runnalbe or iowaiting mode)..... jiffies/ how long already running / for performance counting */
    unsigned long cpu_time_waiting;

    /** @brief priority for standard round robin (priority changes every scheduler-run) */
	int32_t sched_priority;

	/** @brief for realtime scheduling, EDF: Earliest Deadline First (NOT USED YET) */
	int32_t sched_realtime_deadline;

	/** @brief how long task have to be suspended */
	int32_t suspend_time;

	/** @brief process function pointer */
	processFkt pfkt;

	/** @brief pointer to next task */
    struct processControlBlock *ptrNextTask;

} PCtrlBlock_t;


/**
 * TASK_SWITCH_TESTING function: Creates a process with defined name and stacksize, this method is only
 * used for simple task switching (without nay MMU/memory management support!)
 * @see mmuProcessCreate(...)
 * @param pFkt process function pointer
 * @param pName name of the process (shell, space invaders, picture frame application, ...)
 * @param stackSize stack size
 * @return pointer to the new created process
 */
extern PCtrlBlock_t *processCreate(processFkt pFkt, const char *pName, uint32_t stackSize);


/**
 * Creates a process with MMU support and with defined name and stacksize
 * @param virtualLoadAddress load address for MMU
 * @param pName name of the process (shell, space invaders, picture frame application, ...)
 * @return pointer to the new created process
 */
extern PCtrlBlock_t *mmuProcessCreate(uint32_t virtualLoadAddress, const char *pName);


/**
 * Loads the given process into the the current registers, PC, SP, LR, R0 - R12, ...
 * @param pcb process control block of the process we want to load
 * @return nothing
 */
extern void start_first_process(PCtrlBlock_t *pcb) __attribute__ ((naked));


/**
 * Loads the given process from the defined BinImages, used for SYSTEMCALL E.g. shell starts new process like
 * space invaders, function creates a new Process Control Block (PCB), creates process in the MMU,
 * ASID MMU management and puts PCB to the scheduler as a new runnable task
 * @param processName name of the process to init
 * @return nothing
 */
extern void setup_and_start_new_process(char *processName);


/**
 * Create and load first process, first process is the Idle process ==> this process is
 * used for measurement and runs only if no other runnable process is available, idle task
 * starts directly the shell as first real applications (so the user can interact via GUI with HALOS)
 * @return nothing
 */
extern void setupFirstProcess();


/**
 * Starts first process ==> Idle Task, this task automatically starts the shell as a
 * the first real user application (GUI interface)
 * @return nothing
 */
extern void startFirstProcess();

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*PCB_H_*/
