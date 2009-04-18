/*
 * HeapManagement.h
 *
 *  Created on: 04.02.2009
 *      Author: Christian Brändle
 */

#ifndef HEAPMANAGEMENT_H_
#define HEAPMANAGEMENT_H_

/************************************************************************
 title:		Heap Management
 file:     	HeapManagement.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2009 Team eHalOS
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup VMM Virtual Memory Management
 *  @ingroup KERNEL
 */

/**
 *  @defgroup VMM_HEAP Heap Management
 *  @ingroup VMM
 *  @brief Heap Management to add dynamically allocated regions to Process Region Table.
 *  Benefits:
 *  + Free all Pages at process termination
 *  + Memory Protection at Page Level
 *
 *  Not complete at the moment.
 *  System Calls must be implemented.
 *
 *	TODO:
 *	+ own malloc functions for speed and better control
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/


#include "hsystem_api.h"
#include "hsystem_call_table.h"
#include <stdlib.h>

/**
 * Uses malloc of std lib and additionally updates Process Region Table.
 * Not used at the moment.
 * @param size size in bytes of allocated region
 * @return pointer to memory region
 */
void *hmalloc(size_t size);

/**
 * Uses free of std lib and additionally updates Process Region Table.
 * Not used at the moment.
 * @param ptr pointer to memory to be freed
 */
void hfree(void *ptr);


#endif /* HEAPMANAGEMENT_H_ */
