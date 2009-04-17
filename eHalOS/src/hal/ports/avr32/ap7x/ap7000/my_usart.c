/*
 * my_usart.c
 *
 *  Created on: 25.01.2009
 *      Author: Administrator
 */

#include <avr32/io.h>
#include <inttypes.h>
#include "compiler.h"

#include "my_usart.h"
#include "usart_isr.h"


#include "SerialBuffer.h"

volatile SerialBuffer *txBuffer;
volatile SerialBuffer *rxBuffer;

volatile avr32_usart_t *myusart = &AVR32_USART1;


void uart1_set_rx_and_tx_buffer(volatile SerialBuffer *txBuff, volatile SerialBuffer *rxBuff){
	ISR_DISABLE;
	txBuffer = txBuff;
	rxBuffer = rxBuff;
	ISR_ENABLE;
}

void uart1_init(volatile SerialBuffer *txBuff, volatile SerialBuffer *rxBuff){

	txBuffer = txBuff;
	rxBuffer = rxBuff;

	myusart->ier = AVR32_USART_IER_RXRDY_MASK; //Enable Receive Interrupt
}

void uart1_puts(const char *string) {
	while(*string)
		uart1_putch(*string++);
}

void uart1_putch(int32_t ch) {
	//myusart->idr = AVR32_USART_IDR_TXRDY_MASK; //Disable transmit interrupt
	SerialBuffer_add(txBuffer, ch);
	myusart->ier |= (AVR32_USART_IER_TXRDY_MASK); //Enable transmit interrupt
}

int32_t uart1_getch(void) {

	if (SerialBuffer_isEmpty(rxBuffer)) // check if character is available
		return -1;

	return SerialBuffer_remove(rxBuffer);
}
