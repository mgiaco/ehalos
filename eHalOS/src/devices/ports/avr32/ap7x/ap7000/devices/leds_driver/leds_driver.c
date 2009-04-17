

#include "leds_driver.h"



static uint32_t device_uid[2];

void leds_driver_init(void) {

	device_uid[LED0] = DEV_UID_NULL;
	device_uid[LED1] = DEV_UID_NULL;

	// PIO Enable Register
	AVR32_PIOA.per = (1 << 19) | (1<<16);

	// PIO Output Enable Register
	AVR32_PIOA.oer = (1 << 19) | (1<<16);

	//clear leds
	AVR32_PIOA.sodr = (1 << 19) | (1<<16);
}

DEV_UID leds_driver_open(uint32_t device_type, uint32_t device_number) {

	if (device_type==AP7000_LED_DEVICE) {
		if (device_number==LED0) {
			if (device_uid[device_number]!=DEV_UID_NULL) {
				return DEV_NOT_FREE;
			} else {
				device_uid[device_number] = generate_dev_uid();
				return device_uid[device_number];
			}
		}

		if (device_number==LED1) {
			if (device_uid[device_number]!=DEV_UID_NULL) {
				return DEV_NOT_FREE;
			} else {
				device_uid[device_number] = generate_dev_uid();
				return device_uid[device_number];
			}
		}
	}
	return CHOOSE_NEXT_DEV;
}

void leds_driver_set_bit(DEV_UID dev_uid, void *data) {

	if (*((uint32_t*)data)) {
		if (device_uid[LED0]==dev_uid) {
			AVR32_PIOA.codr = (1<<16);
		}
		if (device_uid[LED1]==dev_uid) {
			AVR32_PIOA.codr = (1 << 19);
		}
	} else {
		if (device_uid[LED0]==dev_uid) {
			AVR32_PIOA.sodr = (1<<16);
		}
		if (device_uid[LED1]==dev_uid) {
			AVR32_PIOA.sodr = (1 << 19);
		}
	}

}

int32_t leds_driver_close(DEV_UID dev_uid) {

	if (device_uid[LED0]==dev_uid) {
		device_uid[LED0] = DEV_UID_NULL;
		return DEV_CLOSED;
	}

	if (device_uid[LED1]==dev_uid) {
		device_uid[LED1] = DEV_UID_NULL;
		return DEV_CLOSED;
	}

	return DEV_NOT_FOUND;
}
