#ifndef SERIALBUFFER_H_
#define SERIALBUFFER_H_
/************************************************************************
 title:		Serial Buffer
 file:     	commBuffer.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2008 team halos
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 * 	@defgroup	haloshal	Hardware Abstraction Layer
 */

/**
 *  @defgroup 	serialbuffer	Serial Buffer for Halos UART
 *  @ingroup 	haloshal
 *  @code	#include <SerialBuffer.h> @endcode
 *  @brief	Routines for a circular Buffer
 *
 *  @note ...
 *  @author mgiaco
 *  @version 0.0.1
 */

/**@{*/
#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "compiler.h"


extern volatile avr32_usart_t *myusart;

#define ISR_DISABLE 	myusart->idr = (AVR32_USART_IDR_TXRDY_MASK | AVR32_USART_IDR_RXRDY_MASK); //Disable transmit and receive interrupt
#define ISR_ENABLE 		myusart->ier = (AVR32_USART_IER_TXRDY_MASK | AVR32_USART_IER_RXRDY_MASK); //Enable transmit and receive interrupt
//#define ISR_DISABLE 	myusart->idr = (AVR32_USART_IDR_TXRDY_MASK ); //Disable transmit and receive interrupt
//#define ISR_ENABLE    myusart->ier = (AVR32_USART_IER_TXRDY_MASK ); //Enable transmit and receive interrupt

/**
 *  @name constants, macros,
 */
#define SERIAL_BUFFSIZE 128 /**< SERIAL_BUFFSIZE in Bytes */


/**
 * Buffer Type
 * @typedef buffertype
 */
typedef uint8_t buffertype;

/**
 * Class SerialBuffer
 * @typedef SerialBuffer
 */
typedef struct SerialBufferTag SerialBuffer;

/**
 * Data struct for cirular serial buffer.
 * @struct SerialBufferTag
 */
struct SerialBufferTag{
	uint16_t fp; /**< front pointer */
	uint16_t bp; /**< back pointer */
	uint16_t count; /**< number of items in buffer */
	char fifo[SERIAL_BUFFSIZE]; /**< data array */
};



/**
 * Initialize the Circular Data Buffer.
 * @param me actual circular buffer pointer
 * @return none
 */
extern void SerialBuffer_ctor(volatile SerialBuffer *me);

/**
 * Check Is buffer empty.
 * @param me actual circular buffer pointer
 * @return true if empty or false if not
 */
extern uint16_t SerialBuffer_isEmpty(volatile SerialBuffer *me);

/**
 * Check Is buffer full.
 * @param me actual circular buffer pointer
 * @return true if full or false if not
 */
extern uint16_t SerialBuffer_isFull(volatile SerialBuffer *me);

/**
 * Insert a Data Object at rear.
 * @param me actual circular buffer pointer
 * @param c char to save(send)
 * @return none
 */
extern void SerialBuffer_add(volatile SerialBuffer *me, const char c);

/**
 * Remove a byte at front.
 * @param me actual circular buffer pointer
 * @return user data
 */
extern char SerialBuffer_remove(volatile SerialBuffer *me);

/**
 * Number of bytes in buffer.
 * @param me actual circular buffer pointer
 * @return number of saved elements
 */
extern uint16_t SerialBuffer_size(volatile SerialBuffer *me);

/**
 * Put data object to buffer
 * @see SerialBuffer_add
 */
#define SerialBuffer_put(a,b) SerialBuffer_add(a,b);

/**
 * Get data object from buffer.
 * @see SerialBuffer_remove
 */
#define SerialBuffer_get(a) SerialBuffer_remove(a);

/**
 * Get the number of free Buffer Space
 * @param a actual circular buffer pointer
 * @return number of free space
 */
#define SerialBuffer_getFreeSpace(a) (SERIAL_BUFFSIZE-SerialBuffer_size(a));

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
/**@}*/
#endif /* SERIALBUFFER_H_ */


