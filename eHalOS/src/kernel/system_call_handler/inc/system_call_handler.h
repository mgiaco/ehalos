/**
 * @file system_call_handler.h
 * @brief System Call Hanlder
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
 *  @defgroup SYSTEM_CALL_HANDLER System Call Handler
 *  @ingroup KERNEL
 *  @brief System Call Handler manages all system calls from usermode to halOS (supervisormode)
 *  @note ...
 *  @author giaco, andy, momo, karl, christian
 *  @version 0.0.1
 */

/**@{*/

#ifndef SYSTEM_CALL_HANDLER_H_
#define SYSTEM_CALL_HANDLER_H_

#include "hsystem_call_table.h"
#include "device_driver.h"
#include "graphics.h"
#include "font.h"

/**
 * Manages system calls from usermode (applications) to the halOS kernel (supervisormode), system calls
 * can have max 4 parameters for a syscall (including the ret_params as void pointer)
 * @param scall_number specifys the number of the systemcall, see also @see hsystem_call_table in @file hsystem_call_table.h
 * @param param1 first parameter
 * @param param2 second parameter
 * @param param3 third parameter
 * @param ret_param can be used as void pointer for everthing or as return parameter
 */
extern void scall_Handler(int32_t scall_number, int32_t param1, int32_t param2, int32_t param3, void* ret_param);

/**@}*/

#endif /*SYSTEM_CALL_HANDLER_H_*/
