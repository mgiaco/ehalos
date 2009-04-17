
#include "uart_driver.h"

#include "interrupts.h"

#include "printf_P.h"

#include "usart_isr.h"
#include "my_usart.h"

#include "compiler.h"

//static uint32_t device_uid[4];
static uint32_t device_uid[4];

//static volatile struct avr32_usart_t *usart0 = &AVR32_USART1;
static volatile struct avr32_usart_t *usart1 = &AVR32_USART1;
//static volatile struct avr32_usart_t *usart2 = &AVR32_USART1;
//static volatile struct avr32_usart_t *usart3 = &AVR32_USART1;

static struct usart_options_t opt;
//static int32_t cpu_hz = 20000000;
static int32_t cpu_hz = 25000000;
//static int32_t cpu_hz = 37500000; //150MHZ

static volatile SerialBuffer transmitBuffer;
static volatile SerialBuffer recieveBuffer;


void uart_driver_init(void) {

	// Only UART1 !!
	avr32_piomap_t usart_piomap = { { AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION },
																	{ AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION } };

	// Set options for the USART
	opt.baudrate = 115200;
	opt.charlength = 8;
	opt.paritytype = USART_NO_PARITY;
	opt.stopbits = USART_1_STOPBIT;
	opt.channelmode = USART_NORMAL_CHMODE;

	cpu_hz = pm_read_module_freq_hz(PM_PBA_USART1);

	// Initialize it in RS232 mode
	//usart_init_rs232(usart0, &opt, cpu_hz);
	usart_init_rs232(usart1, &opt, cpu_hz);
	//usart_init_rs232(usart2, &opt, cpu_hz);
	//usart_init_rs232(usart3, &opt, cpu_hz);

	// Setup pio for USART1
	pio_enable_module(usart_piomap, 2);



	SerialBuffer_ctor(&transmitBuffer);
	SerialBuffer_ctor(&recieveBuffer);

//	Disable_global_interrupt();
//	Enable_global_interrupt();
//	Disable_interrupt_level(INT0);
	Disable_interrupt_level(INT1);
//	Disable_interrupt_level(INT2);
//	Disable_interrupt_level(INT3);

	//Setup Interrupts
	register_interrupt((__newlib_int_handler)usart_int_handler, AVR32_USART1_IRQ/32, AVR32_USART1_IRQ % 32, INT1);
	uart1_init(&transmitBuffer, &recieveBuffer);
	Enable_interrupt_level(INT1);
//	Enable_interrupt_level(INT3);


	//dev_uid[COM0] = DEV_NULL;
	device_uid[COM1] = DEV_NULL;
	//dev_uid[COM2] = DEV_NULL;
	//dev_uid[COM3] = DEV_NULL;


//	printf("Board running at ");
//	printf(": %d",pm_get_mclk_freq_hz()/1000000);
//	printf(" MHz\n");
//	printf("SDRAM running at ");
//	printf(": %d", pm_read_module_freq_hz(PM_PBB_HSDRAMC) );


}

DEV_UID uart_driver_open(uint32_t device_type, uint32_t device_number) {

	if (device_type==AP7000_UART_DEVICE) {
		if (device_number==COM1) {
			if (device_uid[device_number]!=DEV_NULL) {
				//device is used
				return DEV_NOT_FREE;
			} else {
				device_uid[device_number] = generate_dev_uid();
				return device_uid[device_number];
			}
		}
	}
	return CHOOSE_NEXT_DEV;
}

int32_t uart_driver_read_byte(DEV_UID dev_uid, void *data) {
	int32_t ret;

//	if (device_uid[COM1]==dev_uid) {
//		//None blocked reading
//		ret = usart_read_char(usart1, (int*)data);
//		if (ret != SUCCESS) {
//			*((int32_t*)data) = -1;
//			return -1;
//		}
//		return 1;
//	}

	if (device_uid[COM1]==dev_uid) {
		//None blocked reading
		ret = uart1_getch();
		*((int32_t*)data) = ret;

		return ret;
	}


//	if (device_uid[COM1]==dev_uid) {
//			//None blocked reading
//			ret = usart_getchar(usart1);
//			*((int32_t*)data) = ret;
//
//			return ret;
//		}


	return DEV_NOT_FOUND;
}

void uart_driver_write_byte(DEV_UID dev_uid, void *data) {
	if (device_uid[COM1]==dev_uid) {

		//old
		//usart_putchar(usart1, *(int*)data);
		uart1_putch(*(int*)data);

		return;
	}
}

int32_t uart_driver_close(DEV_UID dev_uid) {
	if (device_uid[COM1]==dev_uid) {
		device_uid[COM1] = DEV_NULL;
		return DEV_CLOSED;
	}
	return DEV_NOT_FOUND;
}
