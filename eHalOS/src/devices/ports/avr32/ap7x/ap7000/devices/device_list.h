#ifndef DEVICE_LIST_H_
#define DEVICE_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file device_list.h
 * @brief list of the devices on the board
 * @author Team Halos
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup DEVICE  Halos Devices
 */

/**
 *  @defgroup DEVICE_PORTS Hardware dependent
 *  @ingroup DEVICE
 */

/**
 *  @defgroup AP7 AP7000
 *  @ingroup DEVICE_PORTS
 *  @brief Hardware dependent (AP7000) functionality
 */

/**
 *  @defgroup ap7000_hardware_devices Hardware Devices
 *  @ingroup AP7
 *  @brief AP7000 device list
 *
 *  @note ...
 *  @author giaco, momo, andy
 *  @version 0.0.1
 */

/**@{*/


#include <stdint.h>
#include <stdlib.h>

#include "device_struct.h"

#include "device_types.h"


#include "uart_driver.h"
#include "leds_driver.h"
#include "dev_null_driver.h"


device_t actDevices[] = {

		{
				"Serial_Device",
				AP7000_UART_DEVICE,
				0,
				uart_driver_init,
				uart_driver_open,
				uart_driver_read_byte,
				uart_driver_write_byte,
				uart_driver_close
		},

		{
				"Leds_Device0",
				AP7000_LED_DEVICE,
				0,
				leds_driver_init,
				leds_driver_open,
				NULL,
				leds_driver_set_bit,
				leds_driver_close
		},

		{
				"Dev_NULL",
				AP7000_NULL_DEVICE,
				0,
				dev_null_driver_init,
				dev_null_driver_open,
				dev_null_driver_read_byte,
				dev_null_driver_write_byte,
				dev_null_driver_close
		},

		{
				"\0",
				0,
				0,
				0,
				0,
				0,
				0,
				0
		}

};


/**@}*/

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
#endif /*DEVICE_LIST_H_*/
