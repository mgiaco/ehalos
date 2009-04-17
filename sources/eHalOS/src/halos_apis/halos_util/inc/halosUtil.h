/**
 * @file halosUtil.h
 * @brief Utilitys for Halos
 * @author Team Halos
 * @version 0.0.1
 * @date 2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup API Halos API's
 */

/**
 *  @defgroup UTIL Utilitys
 *  @ingroup API
 */


/**
 *  @defgroup UTILITYS Utils for HalOS
 *  @ingroup UTIL
 *  @brief Utils for the halOS
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/

#ifndef HALOSUTIL_H_
#define HALOSUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>


/**
 * Converts a integer into a string (char array)
 * @param i integer to convert
 * @return char array
 */
extern char* halos_itoa(int32_t i);


/**
 * Converts a integer into a string (char array)
 * @param val integer to convert
 * @param base other integer :)
 * @return char array
 */
extern char* halos_itoa_base(int32_t val, int32_t base);


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*HALOSUTIL_H_*/
