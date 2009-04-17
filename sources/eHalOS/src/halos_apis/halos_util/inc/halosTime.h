/**
 * @file halosTime.h
 * @brief System Time API
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
 *  @defgroup TIMEAPI System Time API
 *  @ingroup UTIL
 *  @brief System Time routines
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/

#ifndef HALOSTIME_H_
#define HALOSTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>


/**
 * @typedef Time
 */
typedef struct TimeTag Time;


/**
 * @struct TimeTag
 * Struct for Time Info (h, min, sec)
 */
struct TimeTag {
	int8_t hour;
	int8_t minute;
	int8_t second;
};


/**
 * Time constructor
 * @param me pointer of Time
 * @param hour
 * @param minute
 * @param second
 */
extern void Time_ctor(Time *me, int8_t hour, int8_t minute, int8_t second);


/**
 * Increments time
 * @param me
 */
extern void Time_increment(Time *me);


/**
 * Converts Time into a char[]
 * @param me
 * @return time as a char array
 */
extern char* Time_toString(Time *me);


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*HALOSTIME_H_*/
