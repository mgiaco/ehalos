#ifndef DEVICE_DRIVER_H_
#define DEVICE_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file device_driver.h
 * @brief Device Hardware Interface
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */

/**
 *  @defgroup DEVICE  Halos Devices
 */

/**
 *  @defgroup device_driver Device independend structure
 *  @ingroup DEVICE
 *  @brief Device independend structure
 *
 *  @note ...
 *  @author andy, giaco
 *  @version 0.0.1
 */

/**@{*/


#include <stdlib.h>
#include <stdint.h>


#include "device_struct.h"

#define DEV_UID_START  99

/**
 * @brief generates an unique id for a device
 * @return unique id
 */
extern DEV_UID generate_dev_uid(void);


/**
 * @brief closes a device
 */
extern void init_all_drivers(void);


/**
 * @brief opens a device
 * @param dev_uid unique device number
 * @param device_type the type of device which should be opened(USART, BIT, ...)
 * @param device_number the index of the device, because there could be multiple of the same type
 */
extern device_t *open_driver(DEV_UID *dev_uid, uint32_t device_type, uint32_t device_number);


/**
 * @brief reads data from a device
 * @param dev_uid unique device number
 * @param dev pointer to device struct
 * @param data pointer to data buffer
 */
extern int32_t read_driver(DEV_UID dev_uid, device_t *dev, void* data);


/**
 * @brief writes a unit of data to the device
 * @param dev_uid unique device number
 * @param dev device independent structure
 * @param data pointer to data
 */
extern void write_driver(DEV_UID dev_uid, device_t *dev, void* data);

/**
 * @brief closes a device
 * @param dev pointer to device struct
 * @param dev_uid unique device number
 */
extern int32_t close_driver(device_t *dev, DEV_UID dev_uid);

/**@}*/

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
#endif /*DEVICE_DRIVER_H_*/
