#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file uart_driver.h
 * @brief hardware dependent UART code
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
 */

/**
 *  @defgroup UART_DRIVER UART Driver
 *  @ingroup ap7000_hardware_devices
 *  @brief AP7000 UART driver
 */


 /**
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/


#include <inttypes.h>

#include "pio.h"
#include "usart.h"
#include "pm_at32ap7000.h"

#include "device_driver.h"
#include "device_types.h"

#define DEV_NULL	0


/**
 * @brief initializes the UART driver
 */
extern void uart_driver_init(void);

/**
 * @brief opens the UART driver
 * @param device_type the type of device which should be opened(USART, BIT, ...)
 * @param device_number unique device number
 */
extern DEV_UID uart_driver_open(uint32_t device_type,uint32_t device_number);

/**
 * @brief reads a byte from the UART
 * @param dev_uid unique device number
 * @param data pointer to data buffer
 */
extern int32_t  uart_driver_read_byte(DEV_UID dev_uid,  void *data);

/**
 * @brief writes a byte to the UART
 * @param dev_uid unique device number
 * @param data  pointer to data
 */
extern void uart_driver_write_byte(DEV_UID dev_uid, void *data);

/**
 * closes the UART driver
 * @param dev_uid unique device number
 */
extern int32_t uart_driver_close(DEV_UID dev_uid);


/**@}*/

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif //_UART_DRIVER_H_


