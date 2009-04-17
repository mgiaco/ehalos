#ifndef DEVICE_STRUCT_H_
#define DEVICE_STRUCT_H_
/**
 * @file device_struct.h
 * @brief Device Structure
 * @author Team Halos
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

#include <stdint.h>

/** @brief typedef for unique device identifier */
typedef int32_t DEV_UID;

/** @brief typedef for driver type */
typedef int32_t DRV_ID;

typedef struct {
	/** @brief devicename */
	char deviceName[20];

	/** @brief ID to identify driver type */
	DRV_ID driver_type;

	/** @brief unique identifier for device */
	DEV_UID device_uid;

	/** @brief function pointer to initialize device */
	void (*init)(void);

	/** @brief function pointer to open device */
	DEV_UID (*open)(uint32_t, uint32_t);

	/** @brief function pointer to read from device */
	int32_t (*read)(DEV_UID, void*);

	/** @brief function pointer to write to device */
	void (*write)(DEV_UID, void*);

	/** @brief function pointer to close device */
	int32_t (*close)(DEV_UID);
	/* TODO: int32_t (*ioctl) (DEV_UID, void*, void*); */
} device_t;

/** @brief array for all available devices */
extern device_t actDevices[];

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/
#endif /*DEVICE_STRUCT_H_*/
