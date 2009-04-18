#ifndef LEDS_DRIVER_H_
#define LEDS_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @file leds_driver.h
	 * @brief hardware dependent led driver code
	 * @author Team eHalOS
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
	 *  @defgroup LED_DRIVER Led Driver
	 *  @ingroup ap7000_hardware_devices
	 *  @brief AP7000 LED driver
	 *  @note ...
	 *  @author b83
	 *  @version 0.0.1
	 */

	/**@{*/
#include <inttypes.h>

#include "pio.h"

#include "device_driver.h"
#include "device_struct.h"
#include "device_types.h"


#define DEV_UID_NULL	0		/**< NULL UID */

/**
 * @brief initializes the led driver
 */
extern void leds_driver_init(void);

/**
 * @brief opens the led driver
 * @param device_type
 * @param device_number
 */
extern DEV_UID leds_driver_open(uint32_t device_type, uint32_t device_number);


/**
 * @brief sets a led on the board
 * @param dev_uid device UID
 * @param data bit to write
 */
extern void leds_driver_set_bit(DEV_UID dev_uid, void *data);

/**
 * @brief closes the led driver
 * @param dev_uid device UID
 */
extern int32_t leds_driver_close(DEV_UID dev_uid);

/**@}*/

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*LEDS_DRIVER_H_*/
