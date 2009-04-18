#ifndef HDEVICE_API_H_
#define HDEVICE_API_H_
/**
 * @file hdevice_api.h
 * @brief Device API header
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */

/**
 *  @defgroup API  Halos API's
 */

/**
 *  @defgroup DEVICE_API Halos Device Api
 *  @ingroup API
 *  @brief Halos Device API
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#include "device_struct.h"
#include "hsystem_call_table.h"
#include "hsystem_api.h"

/**
 * @brief initalize all devices for use with Halos
 */
extern void device_init(void);

/**
 * @brief open device
 * @param dev_uid		unique device identifier
 * @param device_type	device type which should be opened
 * @param device_number device number to open
 */
extern device_t *device_open(DEV_UID *dev_uid, int32_t device_type, int32_t device_number);

/**
 * @brief write byte to device
 * @param dev device
 * @param dev_uid unique device identifier
 * @param data databyte which is written to device
 */
extern void device_write_byte(device_t *dev, DEV_UID dev_uid, char data);

/**
 * @brief read byte from device
 * @param dev device
 * @param dev_uid unique device identifier
 * @return databyte
 */
extern int16_t device_read_byte(device_t *dev, DEV_UID dev_uid);

/**
 * @brief close device
 * @param dev device
 * @param dev_uid unique device identifier
 * @return Status
 */
extern int32_t device_close(device_t *dev, DEV_UID dev_uid);

/* Ressource manager functions */

/**
 * @brief open device (ressource manager)
 * @param dev_uid		unique device identifier
 * @param device_type	device type which should be opened
 * @param device_number device number to open
 */
extern device_t *rm_device_open(DEV_UID *dev_uid, int32_t device_type, int32_t device_number);

/**
 * @brief write byte to device (ressource manager)
 * @param dev device
 * @param dev_uid unique device identifier
 * @param data databyte which is written to device
 */
extern void rm_device_write_byte(device_t *dev, DEV_UID dev_uid, char data);

/**
 * @brief read byte from device (ressource manager)
 * @param dev device
 * @param dev_uid unique device identifier
 * @return databyte
 */
extern int16_t rm_device_read_byte(device_t *dev, DEV_UID dev_uid);

/**
 * @brief close device (ressource manager)
 * @param dev device
 * @param dev_uid unique device identifier
 * @return Status
 */extern int32_t rm_device_close(device_t *dev, DEV_UID dev_uid);



// --public helper functions
extern void device_write_string(device_t* dev, DEV_UID dev_uid, const char *str);

extern void device_led_On(device_t *dev, DEV_UID dev_uid);
extern void device_led_Off(device_t *dev, DEV_UID dev_uid);

extern int16_t switch_foreground_process();

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/
#endif /*HDEVICE_API_H_*/
