/**
 * @file hdevice_api.c
 * @brief Device API implementation
 * @author Team Halos
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */

#include "hdevice_api.h"

//static int8_t init = 0;

void device_init(void) {
	int32_t tmp;
//	if (!init) {
//		init = 1;
		system_call4(SCALL_DEVICE_INIT, 0, 0, 0, (void*)&tmp);
		if (tmp != 999) {
			while (1) { }
		}
//	}
}

device_t *device_open(DEV_UID *dev_uid, int32_t device_type, int32_t device_number) {
	uint32_t tmp;
	system_call4(SCALL_DEVICE_OPEN, (uint32_t) dev_uid, device_type, device_number, (void*) &tmp);
	return (device_t *) tmp;
}

int32_t device_close(device_t *dev, DEV_UID dev_uid) {
	uint32_t tmp;
	system_call4(SCALL_DEVICE_CLOSE, (uint32_t) dev, dev_uid, 0, (void*) &tmp);
	return *((int*) tmp);
}

void device_write_byte(device_t *dev, DEV_UID dev_uid, char data) {
	system_call4(SCALL_DEVICE_WRITE, (uint32_t) dev, dev_uid, data, 0);
}

int16_t device_read_byte(device_t *dev, DEV_UID dev_uid) {
	static uint32_t tmp = -1;
	system_call4(SCALL_DEVICE_READ, (uint32_t) dev, dev_uid, 0, (void*) &tmp);
	return (int16_t) tmp;
}

void device_write_string(device_t* dev, DEV_UID dev_uid, const char *str) {
	while (*str != 0) {
		device_write_byte(dev, dev_uid, *str);
		str++;
	}
}


device_t *rm_device_open(DEV_UID *dev_uid, int32_t device_type, int32_t device_number) {
	uint32_t tmp;
	system_call4(SCALL_RM_DEVICE_OPEN, (uint32_t) dev_uid, device_type, device_number, (void*) &tmp);
	return (device_t *) tmp;
}

int32_t rm_device_close(device_t *dev, DEV_UID dev_uid) {
	uint32_t tmp;
	system_call4(SCALL_RM_DEVICE_CLOSE, (uint32_t) dev, dev_uid, 0, (void*) &tmp);
	return *((int*) tmp);
}

void rm_device_write_byte(device_t *dev, DEV_UID dev_uid, char data) {
	system_call4(SCALL_RM_DEVICE_WRITE, (uint32_t) dev, dev_uid, data, 0);
}

int16_t rm_device_read_byte(device_t *dev, DEV_UID dev_uid) {
	static uint32_t tmp = -1;
	system_call4(SCALL_RM_DEVICE_READ, (uint32_t) dev, dev_uid, 0, (void*) &tmp);
	return (int16_t) tmp;
}

void rm_device_write_string(device_t* dev, DEV_UID dev_uid, const char *str) {
	while (*str != 0) {
		rm_device_write_byte(dev, dev_uid, *str);
		str++;
	}
}





void device_led_On(device_t *dev, DEV_UID dev_uid) {
	static uint32_t tmp = 0;
	device_write_byte(dev, dev_uid, tmp);
}

void device_led_Off(device_t *dev, DEV_UID dev_uid) {
	static uint32_t tmp = 1;
	device_write_byte(dev, dev_uid, tmp);
}
