/**
 * @file port.h
 * @brief Porting of AVR AP7000 stuff
 * @author Team eHalOS
 * @version 0.0.1
 * @date 2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup PROCESS_MANAGER Process Manager
 *  @ingroup KERNEL
 */

/**
 *  @defgroup PORTINGSAP7000 Halos Portings for AVR AP7000
 *  @ingroup PROCESS_MANAGER
 *  @brief Porting of AVR AP7000 implementations, register/stack initializing (process manager stuff)
 *
 *  @note ...
 *  @author giaco
 *  @version 0.0.1
 */

/**@{*/

#ifndef PORT_H_
#define PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/**
 * @typedef processFkt process function pointer
 * @brief Defines to prototype to which task functions must conform. (if taskswitcher is used)
 */
typedef void (*processFkt)(void);


/**
 * @note TCB layout
 * R0
 * R1
 * ...
 * SP
 * LR
 * PC
 * SR
 * ulCriticalNesting
 */


/**
 * Init the stack of a process to look exactly as if a call to portSAVE_CONTEXT had been called.
 * @param ptrTopOfRegStack pointer to top of register stack
 * @param ptrTopOfStack pointer to top of stack
 * @param ptrTaskCode pointer to task/process code
 */
extern void port_init_stack(unsigned int *ptrTopOfRegStack,unsigned int *ptrTopOfStack, processFkt ptrTaskCode);


/**
 * Init the stack of a process to look exactly as if a call to portSAVE_CONTEXT had been called.
 * @param ptrTopOfRegStack pointer to top of register stack
 * @param ptrTopOfStack pointer to top of stack
 * @param virtualLoadAddress virtual load address
 * @param pid
 */
extern void port_init_stack2(unsigned int *ptrTopOfRegStack,unsigned int *ptrTopOfStack, uint32_t virtualLoadAddress, uint32_t pid);




#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*PORT_H_*/

