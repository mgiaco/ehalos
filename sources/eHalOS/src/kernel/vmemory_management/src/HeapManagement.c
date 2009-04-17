/*
 * HeapManagement.c
 *
 *  Created on: 04.02.2009
 *      Author: Christian Brändle
 */

#include "HeapManagement.h"

void *hmalloc(size_t size)
{
	void *pMemory;
	pMemory = malloc(size);

	if (pMemory != NULL){
		// system call to update Process Region Table
		system_call2(ALLOCATE_IN_PROCESS_REGION_TABLE, size, pMemory);
		return pMemory;
	}
	return NULL;
}

void hfree(void *ptr)
{
	free(ptr);
	// system call to update Process Region Table
	system_call1(FREE_IN_PROCESS_REGION_TABLE, ptr);
}


