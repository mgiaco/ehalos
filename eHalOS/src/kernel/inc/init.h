/**
 * @file init.h
 * @brief Initializing of AVR AP7000
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
 *  @defgroup INIT Board Initialization
 *  @ingroup KERNEL
 *  @brief Initializing of AVR AP7000 frequency and SDRAM
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/

#ifndef INIT_H_
#define INIT_H_

#include <avr32/io.h>
#include <sys/sysregs.h>

#include <inttypes.h>

#include "pm_at32ap7000.h"
#include "sdramc_at32ap7000.h"


/**
 * Clock switch initialize
 */
void clk_switch_init();


/**
 * Delays system for given msec
 * @param msec milliseconds of delay
 */
void  delay_ms (uint32_t msec);


/**
 * Setup processor frequency and SDRAM refresh timing of embedded board AVR32 AP7000 NGW100
 * @return nothing
 */
void low_level_init();

/**@}*/

#endif /*INIT_H_*/
