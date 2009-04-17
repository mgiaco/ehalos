

#include "dev_null_driver.h"

#include "compiler.h"

void dev_null_driver_init(void){

}

/**
 * @brief opens the UART driver
 * @param device_type the type of device which should be opened(USART, BIT, ...)
 * @param device_number unique device number
 */
DEV_UID dev_null_driver_open(uint32_t device_type,uint32_t device_number){

	if (device_type==AP7000_NULL_DEVICE) {
		return DEV_NULL_UID;
	}
	return CHOOSE_NEXT_DEV;
}

/**
 * @brief reads a byte from the UART
 * @param dev_uid unique device number
 * @param data pointer to data buffer
 */
int32_t  dev_null_driver_read_byte(DEV_UID dev_uid,  void *data){

}

/**
 * @brief writes a byte to the UART
 * @param dev_uid unique device number
 * @param data  pointer to data
 */
void dev_null_driver_write_byte(DEV_UID dev_uid, void *data){

}

/**
 * closes the UART driver
 * @param dev_uid unique device number
 */
int32_t dev_null_driver_close(DEV_UID dev_uid){

}
