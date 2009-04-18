
#ifndef USART_ISR_H_
#define USART_ISR_H_


/**
 * @file usart_isr.h
 * @brief interrupt driven UART handler code
 * @author Team eHalOS
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



/**
 * @brief UART interrupt handler; initiatzes focus switch when '|' received
 */
extern void usart_int_handler();


/**@}*/

#endif /* USART_ISR_H_ */
