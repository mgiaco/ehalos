/************************************************************************
 title:		Serial Buffer
 file:     	commBuffer.c
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2008 team halos
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

#include <inttypes.h>
#include "SerialBuffer.h"

// initialize
void SerialBuffer_ctor(volatile SerialBuffer *me) {
	ISR_DISABLE
	me->fp = 0;
	me->bp = 0;
	me->count = 0;
	ISR_ENABLE
}

// is buffer empty
uint16_t SerialBuffer_isEmpty(volatile SerialBuffer *me) {
	uint16_t ret;
	ISR_DISABLE
	ret = (me->count == 0);
	ISR_ENABLE
	return ret;
}

// is buffer full
uint16_t SerialBuffer_isFull(volatile SerialBuffer *me) {
	uint16_t ret;
	ISR_DISABLE
	ret = (me->count == SERIAL_BUFFSIZE);
	ISR_ENABLE
	return ret;
}

// insert a byte at rear
void SerialBuffer_add(volatile SerialBuffer *me, const char c) {
	if (me->bp >= SERIAL_BUFFSIZE)
		me->bp = 0;

	me->fifo[me->bp] = c;
	me->bp++;
	ISR_DISABLE
	me->count++;
	ISR_ENABLE
}

// remove a byte at front
char SerialBuffer_remove(volatile SerialBuffer *me) {
	char c;
	if (me->fp >= SERIAL_BUFFSIZE)
		me->fp = 0;

	c = me->fifo[me->fp];
	me->fp++;
	ISR_DISABLE
	me->count--;
	ISR_ENABLE
	return c;

}

// return number of bytes in buffer (size)
uint16_t SerialBuffer_size(volatile SerialBuffer *me) {
	uint16_t ret;
	ISR_DISABLE
	ret = me->count;
	ISR_ENABLE
	return ret;
}
