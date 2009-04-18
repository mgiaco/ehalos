/************************************************************************
 title:		Serial Buffer
 file:     	uart_isr.c
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2008 Team eHalOS
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

#include  "usart.h"

#include "my_usart.h"
#include "usart_isr.h"
#include <inttypes.h>
#include "resource_manager.h"

#define SWITCH_BYTE	0x7c // '|'


void usart_int_handler() {

	unsigned int usart_status;
	unsigned int input;

	// check what causes the interrupt
	usart_status = myusart->csr & myusart->imr;

	if (usart_status & AVR32_USART_CSR_TXRDY_MASK) { // The interrupt was caused by the THR becoming empty. Are there any more characters to transmit?

		// check if more data to send
		if (!SerialBuffer_isEmpty(txBuffer)){
			// yes not empty
			myusart->thr = SerialBuffer_get(txBuffer);
		} else {
			// Buffer is empty disable transmit interrupt
			myusart->idr = AVR32_USART_IDR_TXRDY_MASK; //Disable transmit interrupt
		}
	}

	if (usart_status & AVR32_USART_CSR_RXRDY_MASK) { // The interrupt was caused by the receiver getting data.

		if(!SerialBuffer_isFull(rxBuffer)){
			input = (myusart->rhr & AVR32_USART_RHR_RXCHR_MASK) >> AVR32_USART_RHR_RXCHR_OFFSET;
			if( (char)input == SWITCH_BYTE ){
				rm_switch_focus();
			}else{
				SerialBuffer_put(rxBuffer, (buffertype) input);
			}
		}else{
			//TODO: Error Buffer Full
		}
	}
}
