
#ifndef MY_USART_H_
#define MY_USART_H_


/**
 * @file my_usart.h
 * @brief interrupt driven UART
 * @author Team Halos
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */

/**
 *  @defgroup haloshal	Hardware Abstraction Layer
 */

/**
 *  @defgroup UART  halos interrupt driven UART
 *  @ingroup haloshal
 *  @brief halos interrupt driven UART
 *
 *  @note ...
 *  @author b83, giaco
 *  @version 0.0.1
 */

/**@{*/


#include <inttypes.h>
#include "SerialBuffer.h"


/**
 * @brief Sets the receive and transmit buffers
 * @param txBuff the transmit buffer
 * @param rxBuff the receive buffer
 */
extern void uart1_set_rx_and_tx_buffer(volatile SerialBuffer *txBuff, volatile SerialBuffer *rxBuff);

/**
 * @brief Initializes the UART
 * @param txBuff the transmit buffer
 * @param rxBuff the receive buffer
 */
extern void uart1_init(volatile SerialBuffer *txBuff, volatile SerialBuffer *rxBuff);


/**
 * @brief writes a character into the write buffer
 * @param ch the character to write
 */
extern void uart1_putch(int32_t ch);

/**
 * @brief reads a character from the read buffer
 */
extern int32_t uart1_getch(void);

/**
 * @brief writes a string into the write buffer
 * @param string the string to write
 */
extern void uart1_puts(const char *string);

/**
 * @brief reference to the write buffer
 */
extern volatile SerialBuffer *txBuffer;

/**
 * @brief reference to the read buffer
 */
extern volatile SerialBuffer *rxBuffer;

/**
 * @brief reference to the UART
 */
extern volatile avr32_usart_t *myusart;


/**@}*/

#endif /* MY_USART_H_ */
