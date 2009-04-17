/**
 * @file hsystem_api.h
 * @brief HalOS System API
 * @author Team Halos
 * @version 0.0.1
 * @date 2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup API Halos API's
 */

/**
 *  @defgroup SYSTEM_CALL_API System Call API
 *  @ingroup API
 */

/**
 *  @defgroup HSYSTEM_API System API
 *  @ingroup SYSTEM_CALL_API
 *  @brief System Call API manages all system calls from usermode to halOS (supervisormode)
 *  @note ...
 *  @author giaco, momo
 *  @version 0.0.1
 */

/**@{*/

#ifndef HSYSTEM_API_H_
#define HSYSTEM_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#include "hsystem_call_table.h"


/**
 * @struct processInfoBlock
 * @typedef process_info_block_t
 * @brief Used for top() comand, presents current process information in a new datastructure
 */
#pragma pack(4)
typedef struct processInfoBlock {

	/** @brief unique ID of the process */
	uint32_t pid;

	/** @brief Descriptive name given to the task when created.  Facilitates debugging only. */
	char processName[20];

	/** @brief For defined states scheduler in @file sched.h */
	uint32_t state;

	/** @brief creationtimestamp */
	uint64_t start_time;

	/** @brief cputime (in running mode)... jiffies/ how long already running / for performance counting */
	uint64_t cpu_time_running;

	/** @brief cputime (in runnalbe or iowaiting mode)..... jiffies/ how long already running / for performance counting */
	uint64_t cpu_time_waiting;

	/** @brief priority for standard round robin */
	int32_t sched_priority;

	/** @brief pointer to next task */
	struct processInfoBlock* ptrNext;

} process_info_block_t;



/**
 * @struct Process Pointer Block
 * @typedef processPtrBlock_t
 * @brief Used for the Ressource Manager in the halOS kernel for determining the foreground/background process and
 * for switching trough all processes (task switch through user), ATTENTION: this should only accessed for reading, otherwise
 * the scheduler will get in troubles
 */
typedef struct processPtrBlock {

	/** @brief Pointer to a current PCB ot the process */
	struct processControlBlock* ptrProcessPCB;

	/** @brief Pointer to next processPtrBlock */
	struct processPtrBlock* ptrNext;

} processPtrBlock_t;




// ------------------------------------------------------------------------------------------
// public members Standard read, write issues a system call with the given parameter
// ------------------------------------------------------------------------------------------


/**
 * System call up to 4 parameters (including ret_parameter as void pointer)
 * @param scall_number specifys the number of the systemcall, see also hsystem_call_table in hsystem_call_table.h
 * @param param1 first parameter
 * @param param2 second parameter
 * @param param3 third parameter
 * @param ret_param can be used as void pointer for everthing or as return parameter
 */
extern void system_call4(int32_t scall_number, int32_t param1, int32_t param2, int32_t param3, void* ret_param) __attribute__((always_inline));


/**
 * System call without any parameter
 * @param scall_number specifys the number of the systemcall, see also hsystem_call_table in hsystem_call_table.h
 * @param ret_param can be used as void pointer for everthing or as return parameter
 */
extern void system_call0(int32_t scall_number, void* ret_param);


/**
 * System call with one parameter
 * @param scall_number specifys the number of the systemcall, see also hsystem_call_table in hsystem_call_table.h
 * @param param1 first parameter
 */
extern void system_call1(int32_t scall_number, int32_t param1);


/**
 * System call with two parameter
 * @param scall_number specifys the number of the systemcall, see also hsystem_call_table in hsystem_call_table.h
 * @param param1 first parameter
 * @param param2 second parameter
 */
extern void system_call2(int32_t scall_number, int32_t param1, int32_t param2);


/**
 * System call for starting a process by the process name, @see BinImages[] in pcb.c for available processes/applications to start
 * @param processName name of the process to start
 */
extern void start_process(char *processName);


/**
 * Kills and deletes a process
 * @param processName the process to kill
 */
extern int32_t kill_process(char *pidName);


/**
 * Gets the current system kernel time of halOS
 * @return kernel time
 */
extern uint64_t get_kernel_time();


/**
 * Gets the number of running processes, this is often used by the top() comand to determine the size of
 * memory which have to be allocated
 * @return number of all processes in the system (including the running and idle process)
 */
extern int8_t get_nr_of_running_processes();


/**
 * Gets information about the current running process, info: running process can change very quickly because
 * the scheduler switches the tasks very often (depending on TIMESLICE and available possible RUNNABLE processes)
 * @param ret_parm
 */
extern void get_running_processes(process_info_block_t* ret_parm);


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /* HSYSTEM_API_H_ */
