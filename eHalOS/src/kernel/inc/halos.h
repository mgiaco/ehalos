/**
 * @file halos.h
 * @brief Kernel Time of Halos
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
 *  @defgroup TIME Time - Halos System Ticks
 *  @ingroup KERNEL
 *  @brief Manages Kernel Time of Halos (sysTick)
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/



#ifndef HALOS_H_
#define HALOS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <inttypes.h>

#include <avr32/io.h>
#include <sys/sysregs.h>

#include <stdint.h>
#include <stdlib.h>

#include "pcb.h"
#include "port.h"
#include "compiler.h"
#include "sched.h"

#include "interrupts.h"
#include "exceptions.h"

#include "MemoryManagement.h"
#include "loader.h"

#include "../../apps/process1.h"
#include "../../apps/process2.h"
#include "../../apps/shell.h"

#include "macro.h"
#include "init.h"

#include "system_call_handler.h"

/**
 * @brief sysTick counts the current time (system ticks)
 */
extern volatile uint64_t sysTick;

/**
 * Gets the current kernel time of Halos
 * @return sysTick of Halos
 */
extern uint64_t get_time();


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*HALOS_H_*/
