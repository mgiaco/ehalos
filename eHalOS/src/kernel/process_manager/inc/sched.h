/**
 * @file sched.h
 * @brief Scheduler for Process Manager
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
 *  @defgroup SCHEDULER Scheduler
 *  @ingroup PROCESS_MANAGER
 *  @brief Task/process scheduler for Halos, implemented scheduling
 *  algorithm: simple Round Robin (not priority based)
 *
 *  @note ...
 *  @author maex.sp Markus Speckle
 *  @version 0.0.1
 */

/**@{*/

#ifndef SCHED_H_
#define SCHED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pcb.h"
#include "hsystem_api.h"
#include "MemoryManagement.h"

/**
 * @brief Constants for the different task states
 */
#define TASK_INITIALIZED        0
#define TASK_RUNNABLE			1
#define TASK_RUNNING            2
#define TASK_IOBLOCKED			3
#define TASK_SUSPENDED			4
#define TASK_STOPPED            5
#define TASK_TO_BE_DELETED      6
#define TASK_DELETE				7


/**
 * @brief Current Running Task - pointer to current running task (holds only one task!)
 */
extern volatile PCtrlBlock_t *ptrRunningTask;


/**
 * @brief Idle Task for Scheduler - pointer to idle task, is used if system has no other task to run and for performance metrics
 */
extern PCtrlBlock_t *ptrIdleTask;


//-----------------------------------------------------------------
// name public functions (extern)
//-----------------------------------------------------------------


/**
 * Initializing the scheduler at startup, needs idle task and task to run
 * 1.) inits idle task
 * 2.) puts first Task into runnable queue
 * 3.) schedules the tasks... should so far the ptrNewTask because its the first task to run
 * @param ptrIdleTask the idle task for the scheduler (empty task which is doing nothing, only performance counting)
 * @param ptrNewTask starting task ((this is normally the shell task but can also be NULL)
 * @return next task to be run (new current running task)
 */
extern PCtrlBlock_t* init_scheduler(PCtrlBlock_t* ptrIdleTask, PCtrlBlock_t* ptrNewTask);


/**
 * Puts idle task to the scheduler evironment, this task is only "executed" if there is no
 * other task in the runnable queue, idleTask should therefore also be used for performance
 * metrics analyses
 * @param ptrIdleT the idle Task
 */
extern void initIdleTask(PCtrlBlock_t *ptrIdleT);


/**
 * Puts task to the runnable queue, if we call schedule() -> one of the next task from this
 * runnalbe queue will be returned as the next running task
 * @param ptrNewTask the new task
 */
extern void putTask_to_RUNABLQ(PCtrlBlock_t *ptrNewTask);


/**
 * Puts task to the suspended-queue with a speciefied suspend-time, task will be hold in the
 * queue as long as the suspend time is not reached
 * @see updateAllSupendedTasks() for putting back tasks from suspended queue to runnable queue
 * @param ptrNewTask task to be suspended
 * @param suspendTime suspend time (the amount of systicks to wait in the suspend queue)
 */
extern void putTask_to_SUSPENDEDQ(PCtrlBlock_t *ptrNewTask, uint32_t suspendTime);


/**
 * Removes task from runnable queue, state will not be changed
 * @param ptrRemTask the task which should be removed
 */
extern void removeTask_from_RUNABLQ(PCtrlBlock_t *ptrRemTask);


/**
 * Schedules with round robin algorithmus through the runnable queue and returns
 * the next task which should be started for the next time quantum (time slice),
 * returns next task which schould be executed (depends if new runnable tasks available
 * and on priority of tasks (round robin), if no runnable task available the idle task
 * will become the next running task
 * @return the next task to be run (current runningtask)
 */
extern PCtrlBlock_t* schedule(void);


/**
 * Returns the current running task, after the schedule() operation this pointer holds
 * the new current task, this can also be the idle task if there was no other runnable task available
 */
extern PCtrlBlock_t* getRunningTask(void);


/**
 * Changes the state of the task and reorganizes the task queues, e.g. remove from BLOCKEDQ, change
 * state of task an put to RUNNABLEQ
 * @param ptrCurTask the task which should be changed
 * @param newState new state for this task
 */
extern void changeTaskStateAndReorg(PCtrlBlock_t* ptrCurTask, uint8_t newState);


/**
 * Returns current info for all processes in the OS, processInfoBlock_t
 * is a special datatyp which is only used for the top() command, parameter allocatedListOfInfBlocks
 * has to be a already allocated list, length of list is defined by function return value get_nr_of_processes()
 * @param allocatedListOfInfBlocks allocated list of process info blocks (linked list)
 * @return 0 if fine, otherwise -1 (E.g. if length of allocated)
 */
//extern int8_t top(process_info_block_t* allocatedListOfInfBlks);
extern int8_t top(process_info_block_t* allocatedListOfInfBlks);

/**
 * Gets the number of current tasks/processes in the scheduler
 * @return number of tasks
 */
extern uint8_t get_nr_of_processes();


/**
 * Gets the number of tasks (length) in a task-queue
 * @param ptrTaskList queue list for counting
 * @return length of task queue
 */
uint8_t count_queue(PCtrlBlock_t *ptrTaskList);


/**
 * Cleans the scheduler pointer, free´s alls queues and set pointer to NULL (mainly used
 * for testing issues)
 */
extern void cleanupScheduler(void);


/**
 * Updates/reorganizes the SUSPENDED tasks queue, all suspended tasks will be
 * checked if they can be put back to runnable queue
 * @see putTask_to_SUSPENDEDQ()
 */
extern void updateAllSupendedTasks(void);





extern processPtrBlock_t* getTaskListCollection();




//-----------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------


void removeTask_from_TaskListCollection(PCtrlBlock_t* ptrPCBRemTask);


void putTask_to_TaskListCollection(PCtrlBlock_t* ptrCurTask);

/**
 * Put tasks to the blocked queue, here are all tasks hold which are waiting for any IO operations
 * @param ptrNewTask the task for the blocked-queue
 */
void putTask_to_BLOCKEDQ(PCtrlBlock_t *ptrNewTask);


/**
 * Put tasks to the stopped queue, here are all tasks which ended and can be deleted afterwards
 * @param ptrNewTask the task for the stopped-queue
 */
void putTask_to_STOPPEDQ(PCtrlBlock_t *ptrNewTask);


void putTask_to_DELETEQ(PCtrlBlock_t *ptrNewTask);

/**
 * Removes task from blocked-IO-queue, state will not be changed
 * @param ptrRemTask task to be removed from queue
 */
void removeTask_from_BLOCKEDQ(PCtrlBlock_t *ptrRemTask);


/**
 * Removes task from stopped queue, state will not be changed
 * @param ptrRemTask task to be removed from queue
 */
void removeTask_from_STOPPEDQ(PCtrlBlock_t *ptrRemTask);


/**
 * Removes task from suspended-queue, state will not be changed
 * @param ptrRemTask task to be removed from queue
 */
void removeTask_from_SUSPENDEDQ(PCtrlBlock_t *ptrRemTask);

void removeTask_from_DELETEQ(PCtrlBlock_t *ptrRemTask);

void deleteAllTasks();

/**
 * Free´s the memory and the task itselfe from the HALOS system, after
 * this function call the task is completly deleted from the system and scheduler
 * @param ptrCurTask the task to be deleted from HALOS
 */
void deleteTask(PCtrlBlock_t* ptrCurTask);


/**
 * Updates all ticks for process they are in runnable, iowaiting or running state, also the
 * IdleTask has counted time ticks which should indicate the usability of the system
 * Each CPU_TIME_... shows the ticks of running or waiting. Multiply these ticks with
 * the scheduling time-slice size !
 */
void updateCpuTimes(void);


/**
 * Reorganizes the SUSPENDED tasks queue, checks if there are tasks available which can be put
 * to the runnable queue, call this function until return 1 is received, this is needed if there are
 * more than 1 task have to be moved to runnable queue
 * @return: 0 = one task has been found and reorganized, maybe one or more other tasks left to
 * reorganize, return: 1 = all tasks have been checked (and if
 * necessary also moved to runnable queue)
 */
uint8_t reorganizeSuspendedTasks(void);

PCtrlBlock_t* findProcess(uint32_t pid);


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*SCHED_H_*/
