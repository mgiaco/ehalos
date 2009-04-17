#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sched.h"
#include "halos.h"
#include "resource_manager.h"


/**
 * @name -------- THE TASK QUEUES -------
 */

/**
 * @name Pointer first task of our linked list
 */
PCtrlBlock_t *ptrTasks_RUNABLQ;

/**
 * @name Pointer first task of blocked tasks (for io-devices)
 */
PCtrlBlock_t *ptrTasks_BLOCKEDQ;

/**
 * @name Pointer first task of stopped tasks (finished tasks)
 */
PCtrlBlock_t *ptrTasks_STOPPEDQ;

/**
 * @name Pointer first task of suspended tasks - holds all tasks which are suspended
 */
PCtrlBlock_t *ptrTasks_SUSPENDEDQ;

/**
 * @name Pointer first task of to be deleted tasks
 */
PCtrlBlock_t *ptrTasks_DELETEQ;

/**
 * @name Pointer to current running task (holds only one task!)
 */
volatile PCtrlBlock_t *ptrRunningTask;

/**
 * @name Pointer to idle task, is used if system has no other task to run and for performance metrics
 */
PCtrlBlock_t *ptrIdleTask;


/**
 * @name Pointer to head of task list collection, this list holds all suitable tasks of the scheduler
 * for the resource manager, this list is needed for the background/foreground task selection
 */
processPtrBlock_t* ptrTaskListCollection;




PCtrlBlock_t* init_scheduler(PCtrlBlock_t* ptrIdleTask, PCtrlBlock_t* ptrNewTask){

	initIdleTask(ptrIdleTask);

	changeTaskStateAndReorg(ptrNewTask, TASK_RUNNABLE);

	PCtrlBlock_t* ptrTaskToRun = NULL;

	ptrTaskToRun = schedule();

	return ptrTaskToRun;
}


void initIdleTask(PCtrlBlock_t *ptrIdleT){
	if(ptrIdleT == NULL){
		return;
	}
	ptrIdleT->ptrNextTask = NULL;
	ptrIdleTask = ptrIdleT;
}


void putTask_to_RUNABLQ(PCtrlBlock_t *ptrNewTask){

	if(ptrNewTask == NULL){
		return;
	}

	ptrNewTask->ptrNextTask = NULL;
	ptrNewTask->state = TASK_RUNNABLE;

	// do not update priority automatically, it was maybe before a blocked task
	// and so the priority will be quite high now

	// if list is null so far
	if(ptrTasks_RUNABLQ == NULL){
		ptrTasks_RUNABLQ = ptrNewTask;
		ptrTasks_RUNABLQ->ptrNextTask = NULL;
		return;
	}

	// find end of runablequeue
	PCtrlBlock_t *ptrTmpTask = ptrTasks_RUNABLQ;
	while(ptrTmpTask->ptrNextTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	}

	// put task to the runablequeue
	ptrTmpTask->ptrNextTask = ptrNewTask;

	return;
}


void putTask_to_BLOCKEDQ(PCtrlBlock_t *ptrNewTask){

	if(ptrNewTask == NULL){
		return;
	}

	ptrNewTask->ptrNextTask = NULL;
	ptrNewTask->state = TASK_IOBLOCKED;

	// do not update priority automaticly, it was maybe before a blocked task
	// and so the priority will be quite high now

	// if list is null so far
	if(ptrTasks_BLOCKEDQ == NULL){
		ptrTasks_BLOCKEDQ = ptrNewTask;
		ptrTasks_BLOCKEDQ->ptrNextTask = NULL;
		return;
	}

	// find end of IOBLOCKED queue
	PCtrlBlock_t *ptrTmpTask = ptrTasks_BLOCKEDQ;
	while(ptrTmpTask->ptrNextTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	}

	// put task to the IOBLOCKED queue
	ptrTmpTask->ptrNextTask = ptrNewTask;

	return;
}


void putTask_to_STOPPEDQ(PCtrlBlock_t *ptrNewTask){

	if(ptrNewTask == NULL){
		return;
	}

	ptrNewTask->ptrNextTask = NULL;
	ptrNewTask->state = TASK_STOPPED;

	// if list is null so far
	if(ptrTasks_STOPPEDQ == NULL){
		ptrTasks_STOPPEDQ = ptrNewTask;
		ptrTasks_STOPPEDQ->ptrNextTask = NULL;
		return;
	}

	// find end of IOBLOCKED queue
	PCtrlBlock_t *ptrTmpTask = ptrTasks_STOPPEDQ;
	while(ptrTmpTask->ptrNextTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	}

	// put task to the IOBLOCKED queue
	ptrTmpTask->ptrNextTask = ptrNewTask;

	return;
}


void putTask_to_SUSPENDEDQ(PCtrlBlock_t *ptrNewTask, uint32_t suspendTime){

	if(ptrNewTask == NULL){
		return;
	}

	// check it task is currently a running task or somewhere else stored, remove from old queue or ptrRunning
	changeTaskStateAndReorg(ptrNewTask, TASK_SUSPENDED);

	ptrNewTask->ptrNextTask = NULL;
	ptrNewTask->state = TASK_SUSPENDED;
	ptrNewTask->suspend_time = sysTick + suspendTime;   // suspend time:  current time + new suspend time

	// if list is null so far
	if(ptrTasks_SUSPENDEDQ == NULL){
		ptrTasks_SUSPENDEDQ = ptrNewTask;
		ptrTasks_SUSPENDEDQ->ptrNextTask = NULL;
		return;
	}

	// find end of SUSPENDED queue
	PCtrlBlock_t *ptrTmpTask = ptrTasks_SUSPENDEDQ;
	while(ptrTmpTask->ptrNextTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	}

	// put task to the SUSPENDED queue
	ptrTmpTask->ptrNextTask = ptrNewTask;

	return;
}

void putTask_to_DELETEQ(PCtrlBlock_t *ptrNewTask){

	if(ptrNewTask == NULL){
		return;
	}

	ptrNewTask->ptrNextTask = NULL;
	ptrNewTask->state = TASK_DELETE;

	// do not update priority automaticly, it was maybe before a DELETE task
	// and so the priority will be quite high now

	// if list is null so far
	if(ptrTasks_DELETEQ == NULL){
		ptrTasks_DELETEQ = ptrNewTask;
		ptrTasks_DELETEQ->ptrNextTask = NULL;
		return;
	}

	// find end of DELETE queue
	PCtrlBlock_t *ptrTmpTask = ptrTasks_DELETEQ;
	while(ptrTmpTask->ptrNextTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	}

	// put task to the DELETE queue
	ptrTmpTask->ptrNextTask = ptrNewTask;

	return;
}


void putTask_to_TaskListCollection(PCtrlBlock_t* ptrCurTask){

	if(ptrCurTask == NULL){
		return;
	}

	processPtrBlock_t* ptrNewProcessTask = (processPtrBlock_t*) malloc(sizeof(processPtrBlock_t));

	if(ptrNewProcessTask != NULL){

		ptrNewProcessTask->ptrProcessPCB = ptrCurTask;
		ptrNewProcessTask->ptrNext = NULL;

		// list is empty first time
		if(ptrTaskListCollection == NULL){
			ptrTaskListCollection = ptrNewProcessTask;
			return;
		}

		// list has already some tasks
		processPtrBlock_t* ptrTmpProcessTask = ptrTaskListCollection;
		while(ptrTmpProcessTask->ptrNext != NULL){

			// process PCB already in TaskListCollection
			if(ptrTmpProcessTask->ptrProcessPCB == ptrNewProcessTask->ptrProcessPCB){
				return;
			}

			ptrTmpProcessTask = ptrTmpProcessTask->ptrNext;
		}

		// add task to list
		ptrTmpProcessTask->ptrNext = ptrNewProcessTask;

	} else {
		// for debug only
		while(1){}
	}

	return;
}


void removeTask_from_TaskListCollection(PCtrlBlock_t* ptrPCBRemTask){

	if(ptrPCBRemTask == NULL || ptrTaskListCollection == NULL){
		return;
	}


	// find task and remove it from list (may task is the first in list)
	if(ptrTaskListCollection->ptrProcessPCB == ptrPCBRemTask){
		processPtrBlock_t* ptrDeleteProcessTask = ptrTaskListCollection;
		ptrTaskListCollection = ptrTaskListCollection->ptrNext;
		// do not: ptrRemTask = NULL;
		free(ptrDeleteProcessTask);
		ptrDeleteProcessTask = NULL;
		return;
	}

	// other tasks
	processPtrBlock_t *ptrTmpProcessTask = ptrTaskListCollection;
	do{
		if(ptrTmpProcessTask->ptrNext != NULL && ptrTmpProcessTask->ptrNext->ptrProcessPCB == ptrPCBRemTask){
			processPtrBlock_t* ptrDeleteProcessTask = ptrTmpProcessTask->ptrNext;
			ptrTmpProcessTask->ptrNext = ptrTmpProcessTask->ptrNext->ptrNext;
			free(ptrDeleteProcessTask);
			ptrDeleteProcessTask = NULL;
			return;
		}
		ptrTmpProcessTask = ptrTmpProcessTask->ptrNext;
	} while(ptrTmpProcessTask != NULL);


	/*
	processPtrBlock_t* ptrTmpProcessTask = ptrTaskListCollection;
	processPtrBlock_t* ptrPrevProcessTask = ptrTaskListCollection;
	do{
		if(ptrTmpProcessTask->ptrProcessPCB == ptrPCBRemTask){
			processPtrBlock_t* ptrDeleteProcessTask = ptrTmpProcessTask->ptrNext;
			ptrTmpProcessTask = ptrTmpProcessTask->ptrNext;
			// do not: ptrRemTask = NULL;
			free(ptrDeleteProcessTask);
			ptrDeleteProcessTask = NULL;
			ptrPrevProcessTask->ptrNext = ptrTmpProcessTask;
			return;
		}
		ptrPrevProcessTask = ptrPrevProcessTask->ptrNext;
		ptrTmpProcessTask = ptrTmpProcessTask->ptrNext;
	} while(ptrTmpProcessTask != NULL);
	*/

}


void removeTask_from_RUNABLQ(PCtrlBlock_t *ptrRemTask){

	if(ptrRemTask == NULL || ptrTasks_RUNABLQ == NULL){
		return;
	}

	// find task and remove it from list
	if(ptrTasks_RUNABLQ == ptrRemTask){       // if it is the first
		ptrTasks_RUNABLQ = ptrTasks_RUNABLQ->ptrNextTask;
		ptrRemTask = NULL;
		return;
	}

	PCtrlBlock_t *ptrTmpTask = ptrTasks_RUNABLQ;     // if it is one of the following tasks
	do{
		if(ptrTmpTask->ptrNextTask == ptrRemTask){
			ptrTmpTask->ptrNextTask = ptrRemTask->ptrNextTask;
			ptrRemTask = NULL;
			return;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);


}


void removeTask_from_BLOCKEDQ(PCtrlBlock_t *ptrRemTask){

	if(ptrRemTask == NULL || ptrTasks_BLOCKEDQ == NULL){
		return;
	}

	// find task and remove it from list
	PCtrlBlock_t *ptrTmpTask = ptrTasks_BLOCKEDQ;
	do{
		if(ptrTmpTask == ptrRemTask){
			ptrTmpTask->ptrNextTask = ptrRemTask->ptrNextTask;
			ptrRemTask->ptrNextTask = NULL;
			return;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);
}


void removeTask_from_STOPPEDQ(PCtrlBlock_t *ptrRemTask){

	if(ptrRemTask == NULL || ptrTasks_STOPPEDQ == NULL){
		return;
	}

	// find task and remove it from list
	PCtrlBlock_t *ptrTmpTask = ptrTasks_STOPPEDQ;
	do{
		if(ptrTmpTask == ptrRemTask){
			ptrTmpTask->ptrNextTask = ptrRemTask->ptrNextTask;
			ptrRemTask->ptrNextTask = NULL;
			return;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);
}


void removeTask_from_SUSPENDEDQ(PCtrlBlock_t *ptrRemTask){

	if(ptrRemTask == NULL || ptrTasks_SUSPENDEDQ == NULL){
		return;
	}

	// find task and remove it from list
	PCtrlBlock_t *ptrTmpTask = ptrTasks_SUSPENDEDQ;
	do{
		if(ptrTmpTask == ptrRemTask){
			ptrTmpTask->ptrNextTask = ptrRemTask->ptrNextTask;
			ptrRemTask->ptrNextTask = NULL;
			return;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);
}


void removeTask_from_DELETEQ(PCtrlBlock_t *ptrRemTask){

	if(ptrRemTask == NULL || ptrTasks_DELETEQ == NULL){
		return;
	}

	// find task and remove it from list
	PCtrlBlock_t *ptrTmpTask = ptrTasks_DELETEQ;
	do{
		if(ptrTmpTask == ptrRemTask){
			ptrTmpTask->ptrNextTask = ptrRemTask->ptrNextTask;
			ptrRemTask->ptrNextTask = NULL;
			return;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);
}


void updateAllSupendedTasks(void){

	uint8_t state = 0;

	state = reorganizeSuspendedTasks();

	// function reorganizeSupsendedTasks has to be called as often as return 0 is received (to update all tasks)
	while(state == 0){
		state = reorganizeSuspendedTasks();
	}
}


uint8_t reorganizeSuspendedTasks(void){

	if(ptrTasks_SUSPENDEDQ == NULL){
		return 1;
	}

	// find task which has already reached suspend_time and remove it from list
	PCtrlBlock_t *ptrTmpTask = ptrTasks_SUSPENDEDQ;
	do{
		if(ptrTmpTask->suspend_time <= sysTick){
			// removes task from suspended queue and puts it back to runnable queue for next schedule call
			changeTaskStateAndReorg(ptrTmpTask, TASK_RUNNABLE);
			return 0;
		}
		ptrTmpTask = ptrTmpTask->ptrNextTask;
	} while(ptrTmpTask != NULL);

	return 1;   // no tasks left for reorganizing
}


uint8_t get_nr_of_processes(){

	uint8_t count = 0;

	count = count_queue(ptrTasks_RUNABLQ);

	count+= count_queue(ptrTasks_BLOCKEDQ);

	count+= count_queue(ptrTasks_STOPPEDQ);

	count+= count_queue(ptrTasks_SUSPENDEDQ);

	if(ptrRunningTask != NULL){
		count++;
	}

	// if runnTask is already the idleTask we should not count it twice
	//if(ptrRunningTask != ptrIdleTask){
	if(ptrIdleTask != NULL){
		count++;
	}

	return count;
}



uint8_t count_queue(PCtrlBlock_t *ptrTaskList){

	uint8_t listCount = 0;

	PCtrlBlock_t *ptrTmpTask = ptrTaskList;

	while(ptrTmpTask != NULL){
		ptrTmpTask = ptrTmpTask->ptrNextTask;
		listCount++;
	}

	return listCount;
}



int8_t top(process_info_block_t* allocatedListOfInfBlks){
	// ret_param contains allocated space for processinfoblocks cause we cannot allocate space in
	// the scheduler (supervisormode) and return it afterwards to the shell application (usermode)
	// check if allocated space is OK (in case of changed tasks in the meantime)
	uint8_t count = get_nr_of_processes();
	uint8_t allocCount = 0;
	process_info_block_t* tmpBlocks;

//	tmpBlocks = allocatedListOfInfBlks;

	//uint32_t test_size = (sizeof((processInfoBlock_t*) allocatedListOfInfBlocks)/sizeof(processInfoBlock_t));

//	while(tmpBlocks->ptrNext!=NULL){
//		tmpBlocks = tmpBlocks->ptrNext;
//		allocCount++;
//	}
//
//	//uint8_t allocCount = sizeof((processInfoBlock_t*) allocatedListOfInfBlocks) ;
//	if(count <= allocCount){
//		return -1;  // wrong number of tasks-memory allocated
//	}

	process_info_block_t* ptrNewInfBlk = allocatedListOfInfBlks;


	// --------------------------------------------+------------------------------------
	// get RUNNABLE queue process infos
	// --------------------------------------------------------------------------------
	PCtrlBlock_t *ptrTmpTask = ptrTasks_RUNABLQ;

	while(ptrTmpTask != NULL){
		ptrNewInfBlk->pid = ptrTmpTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrTmpTask->processName, strlen(ptrTmpTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrTmpTask->processName);
		ptrNewInfBlk->state = ptrTmpTask->state;
		ptrNewInfBlk->start_time = ptrTmpTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrTmpTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrTmpTask->cpu_time_waiting;

		// next task
		ptrTmpTask = ptrTmpTask->ptrNextTask;
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}


	// --------------------------------------------------------------------------------
	// get BLOCKED (IOWAINTING) queue process infos
	// --------------------------------------------------------------------------------
	ptrTmpTask = ptrTasks_BLOCKEDQ;

	while(ptrTmpTask != NULL){
		ptrNewInfBlk->pid = ptrTmpTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrTmpTask->processName, strlen(ptrTmpTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrTmpTask->processName);
		ptrNewInfBlk->state = ptrTmpTask->state;
		ptrNewInfBlk->start_time = ptrTmpTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrTmpTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrTmpTask->cpu_time_waiting;

		// next task
		ptrTmpTask = ptrTmpTask->ptrNextTask;
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}


	// --------------------------------------------------------------------------------
	// get STOPPED queue process infos
	// --------------------------------------------------------------------------------
	ptrTmpTask = ptrTasks_STOPPEDQ;

	while(ptrTmpTask != NULL){
		ptrNewInfBlk->pid = ptrTmpTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrTmpTask->processName, strlen(ptrTmpTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrTmpTask->processName);
		ptrNewInfBlk->state = ptrTmpTask->state;
		ptrNewInfBlk->start_time = ptrTmpTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrTmpTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrTmpTask->cpu_time_waiting;

		// next task
		ptrTmpTask = ptrTmpTask->ptrNextTask;
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}


	// --------------------------------------------------------------------------------
	// get SUSPENDED queue process infos
	// --------------------------------------------------------------------------------
	ptrTmpTask = ptrTasks_SUSPENDEDQ;

	while(ptrTmpTask != NULL){
		ptrNewInfBlk->pid = ptrTmpTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrTmpTask->processName, strlen(ptrTmpTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrTmpTask->processName);
		ptrNewInfBlk->state = ptrTmpTask->state;
		ptrNewInfBlk->start_time = ptrTmpTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrTmpTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrTmpTask->cpu_time_waiting;

		// next task
		ptrTmpTask = ptrTmpTask->ptrNextTask;
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}


	// --------------------------------------------------------------------------------
	// get RUNNING task process infos
	// --------------------------------------------------------------------------------

	if(ptrIdleTask != NULL && ptrRunningTask != NULL){
		ptrNewInfBlk->pid = ptrRunningTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrRunningTask->processName, strlen(ptrRunningTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrRunningTask->processName);
		ptrNewInfBlk->state = ptrRunningTask->state;
		ptrNewInfBlk->start_time = ptrRunningTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrRunningTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrRunningTask->cpu_time_waiting;

		// no next task
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}


	// --------------------------------------------------------------------------------
	// get IDLE task process infos
	// --------------------------------------------------------------------------------
	if(ptrIdleTask != NULL && ptrIdleTask != ptrRunningTask){
		ptrNewInfBlk->pid = ptrIdleTask->pid;
		// memcpy(ptrNewInfBlk->processName, ptrIdleTask->processName, strlen(ptrIdleTask->processName));
		strcpy(ptrNewInfBlk->processName, ptrIdleTask->processName);
		ptrNewInfBlk->state = ptrIdleTask->state;
		ptrNewInfBlk->start_time = ptrIdleTask->start_time;
		ptrNewInfBlk->cpu_time_running = ptrIdleTask->cpu_time_running;
		ptrNewInfBlk->cpu_time_waiting = ptrIdleTask->cpu_time_waiting;

		// no next task
		ptrNewInfBlk = ptrNewInfBlk->ptrNext;
	}

	return 0;
}

void deleteAllTasks(){
	PCtrlBlock_t* ptrTmpTask = ptrTasks_DELETEQ;
	PCtrlBlock_t* ptrTmp2Task;

	while(ptrTmpTask != NULL){
		ptrTmp2Task = ptrTmpTask->ptrNextTask;
		deleteTask(ptrTmpTask);
		ptrTmpTask = ptrTmp2Task;
	}

	// TODO free whole list
	//free(ptrTasks_DELETEQ);
	ptrTasks_DELETEQ = NULL;
}


PCtrlBlock_t * schedule(void){

	updateAllSupendedTasks();

	deleteAllTasks();

	// if runnable list empty ... and no previous running task present
	if(ptrTasks_RUNABLQ == NULL && ptrRunningTask == NULL){
		ptrIdleTask->state = TASK_RUNNING;
		ptrRunningTask = ptrIdleTask;
		updateCpuTimes();
		return ptrRunningTask;
	}

	// if runnable list empty ... and running task present
	if(ptrTasks_RUNABLQ == NULL){
		// check if running task is available and if state is not blocked or something else
		if(ptrRunningTask != NULL && ptrRunningTask->state == TASK_RUNNING){
			// return the current running task (could be idle or other task)
			updateCpuTimes();
			return ptrRunningTask;

		} else if(ptrRunningTask != NULL){
			changeTaskStateAndReorg(ptrRunningTask, ptrRunningTask->state);
			ptrRunningTask = ptrIdleTask;
			ptrIdleTask->state = TASK_RUNNING;
			updateCpuTimes();
			return ptrIdleTask;
		}
	}

	// ptrTasks_RUNABLQ != null ... now new runnable tasks available
	PCtrlBlock_t* tmpLastRunnTask = ptrRunningTask;

	// update current (old) running job
	if(ptrRunningTask != NULL){

		if(ptrRunningTask->state == TASK_RUNNING){
			// set only from running to runnable if task is not blocked or stopped
			// and if it should therefore be continued on next quantums
			tmpLastRunnTask->sched_priority = 0;
			tmpLastRunnTask->state = TASK_RUNNABLE;
			ptrRunningTask = NULL;

			// put task earliest at the end of "schedule" back to the runnable list !

		} // else: do nothing, leave state and priority as it is (e.g. io-blocked or stopped)
	}

	// if only one new runnable task is available (only 1 task in list)
	if(ptrTasks_RUNABLQ->ptrNextTask == NULL){

		ptrRunningTask = ptrTasks_RUNABLQ;    // set new as current running task
		ptrRunningTask->state = TASK_RUNNING;

		// remove task from RUNABLQ, there was only 1 task
		ptrTasks_RUNABLQ = NULL;

	} else {

		// start round robin, look for the best task (with highest priority)
		PCtrlBlock_t *ptr_BestRR_Task = ptrTasks_RUNABLQ;

		PCtrlBlock_t *ptrTmpTask = ptrTasks_RUNABLQ;
		while(ptrTmpTask != NULL){
			if(ptrTmpTask->sched_priority > ptr_BestRR_Task->sched_priority){
				ptr_BestRR_Task = ptrTmpTask;
			} else {
				ptrTmpTask->sched_priority++;
			}

			ptrTmpTask = ptrTmpTask->ptrNextTask;
		}

		// save and update best RR task
		ptrRunningTask = ptr_BestRR_Task;

		changeTaskStateAndReorg(ptr_BestRR_Task, TASK_RUNNING);
	}

	// put old task back into runnable queue ... do this after we choosing next running-task
	if(tmpLastRunnTask != ptrIdleTask){
		putTask_to_RUNABLQ(tmpLastRunnTask);   // put into runnable queue if it was not idle task
	}

	updateCpuTimes();
	return ptrRunningTask;
}


void updateCpuTimes(void){

	// RUNNING TASK
	if(ptrRunningTask != NULL){
		ptrRunningTask->cpu_time_running++;
	}

	// IDLE TASK (only if it is also IOBLOCKED)
	if(ptrIdleTask != NULL && ptrIdleTask->state == TASK_IOBLOCKED){
		ptrIdleTask->cpu_time_waiting++;
	}

	// RUNNABLE QUEUE
	PCtrlBlock_t *ptrTmpTask1 = ptrTasks_RUNABLQ;
	while(ptrTmpTask1 != NULL){
		ptrTmpTask1->cpu_time_running++;
		ptrTmpTask1 = ptrTmpTask1->ptrNextTask;
	}

	// IOWAINTING QUEUE
	PCtrlBlock_t *ptrTmpTask2 = ptrTasks_BLOCKEDQ;
	while(ptrTmpTask2 != NULL){
		ptrTmpTask2->cpu_time_waiting++;
		ptrTmpTask2 = ptrTmpTask2->ptrNextTask;
	}

	// SUSPENDED QUEUE
	// do nothing at the moment
}


void cleanupScheduler(void){
	// ptrTasks_MAINQ = NULL;
	ptrTasks_RUNABLQ = NULL;
	ptrTasks_BLOCKEDQ = NULL;
	ptrRunningTask = NULL;
	ptrIdleTask = NULL;
}


PCtrlBlock_t* getRunningTask(void){
	return ptrRunningTask;
}


void deleteTask(PCtrlBlock_t* ptrCurTask){

	BOOL succ;

	// TEST
	//rm_switch_focus();

	removeTask_from_TaskListCollection(ptrCurTask);

	//free the memory of this task !!!
	succ = unloadProcessFromMemorySystem(ptrCurTask->pid);

	// TODO: properly free ptrCurTask

	ptrCurTask = NULL;
}


void changeTaskStateAndReorg(PCtrlBlock_t* ptrCurTask, uint8_t newState){

	if(ptrCurTask == NULL){
		return;
	}

	uint8_t oldState = ptrCurTask->state;

	switch(oldState){
		case TASK_INITIALIZED:
			putTask_to_TaskListCollection(ptrCurTask);
			break;
		case TASK_RUNNABLE:
			// remove from runnable queue
			removeTask_from_RUNABLQ(ptrCurTask);
			break;
		case TASK_RUNNING:
			// task is running
			ptrRunningTask = NULL;
			break;
		case TASK_SUSPENDED:
			removeTask_from_SUSPENDEDQ(ptrCurTask);
			break;
		case TASK_IOBLOCKED:
			// remove from IOBLOCKED queue
			removeTask_from_BLOCKEDQ(ptrCurTask);
			ptrCurTask->sched_priority = 100000;       // task comes back from IOBLOCKED and should run as early as possible
			break;
		case TASK_STOPPED:
			removeTask_from_STOPPEDQ(ptrCurTask);
			break;
		case TASK_TO_BE_DELETED:
			// do nothing
			break;
		default:
			break;
	}

	ptrCurTask->state = newState;

	switch(newState){

		case TASK_INITIALIZED:
			// do nothing
			break;

		case TASK_RUNNABLE:
			// put to runnable queue
			putTask_to_RUNABLQ(ptrCurTask);
			break;

		case TASK_RUNNING:
			if(ptrRunningTask != NULL){
				ptrRunningTask->ptrNextTask = NULL;
				ptrRunningTask->sched_priority = 0;
			}
			// this should never happen! only schedule() can do this
			break;

		case TASK_SUSPENDED:
			//
			// INFO: this is not possible, putTask_to_SUSPENDEDQ(... ) needs also suspend time
			//       never use TASK_SUSPENDED as new state,
			//		 instead use: changeTaskAndReorg(task, TASK_SUSPENDED) + putTask_to_SUSPENDQ(task, suspend_time)
			break;

		case TASK_IOBLOCKED:
			// put to IOBLOCKED queue
			putTask_to_BLOCKEDQ(ptrCurTask);
			break;

		case TASK_STOPPED:
			putTask_to_STOPPEDQ(ptrCurTask);
			break;

		case TASK_TO_BE_DELETED:
			putTask_to_DELETEQ(ptrCurTask);
			break;

		case TASK_DELETE:
			// used by schedule() method
			break;


		default:
			break;
	}
}



processPtrBlock_t* getTaskListCollection(){

	return ptrTaskListCollection;
}

PCtrlBlock_t* findProcess(uint32_t pid)
{
	processPtrBlock_t* ptrCurProcTask = ptrTaskListCollection;
	while(ptrCurProcTask != NULL){
		if(ptrCurProcTask->ptrProcessPCB->pid == pid){
			return ptrCurProcTask->ptrProcessPCB;   // return current PCB
		}
		ptrCurProcTask = ptrCurProcTask->ptrNext;
	}
	return NULL;
}


