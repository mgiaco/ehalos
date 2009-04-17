#ifndef DEVICE_TYPES_H_
#define DEVICE_TYPES_H_
/**
 * @file device_types.h
 * @brief AP7000 specific Device types
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
 */

/**
 *  @defgroup AP7000_DEV AP7000 specific devices
 *  @ingroup DEVICE_API
 *  @brief AP7000 specific device types
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif


#define DEV_CLOSED   				 0		/**< closed device */
#define DEV_NOT_FREE    			-1		/**< device is not free */
#define DEV_NOT_FOUND   			-2		/**< device not found */
#define CHOOSE_NEXT_DEV				-3		/**< choose next device */



#define AP7000_LED_DEVICE			100		/**< led device */
#define AP7000_TFT_DEVICE			200		/**< tft device */
#define AP7000_UART_DEVICE			300		/**< uart device */
#define	AP7000_NULL_DEVICE			999		/**< null device */

#define DEV_NULL_UID				23		/**< UID for NULL device */

//#define COM0	0
#define COM1	1							/**< COM1 */
//#define COM2	2
//#define COM3	3

#define LED0	0							/**< first led */
#define LED1	1							/**< second led */


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
/**@}*/
#endif /*DEVICE_TYPES_H_*/
