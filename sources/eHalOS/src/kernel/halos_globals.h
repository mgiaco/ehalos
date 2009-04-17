/**
 * @file halos_globals.h
 * @brief Globals for Halos
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
 *  @defgroup GLOBALS Halos Globals
 *  @ingroup KERNEL
 *  @brief Holds globals for Halos
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/


#ifndef HALOS_GLOBALS_H_
#define HALOS_GLOBALS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/**
 * Globale Variabeln braucht man in C hin und wieder, hier sollten alle angeführt werden
 * dadruch gibt es kein durcheinander!
 *
 * Verwendung im main dieser File includieren und zwar so (path-anpassen!) :
 * #define EXTERN
 * #include "path/globals.h"
 *
 *
 * in den File wo man die Globalen Variablen verwenden will einfach nur eine
 * #include "path/globals.h"
 *
 */

#ifndef EXTERN
#define EXTERN extern
#endif


EXTERN int32_t globalVar1;
EXTERN int32_t globalVar2;

EXTERN int32_t globalI;
EXTERN int32_t processStarted;



#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*HALOS_GLOBALS_H_*/
