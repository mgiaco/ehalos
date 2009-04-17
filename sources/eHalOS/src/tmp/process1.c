#include "process1.h"

void user_process1(void) {

	int32_t lflag = 0;
	int32_t oldTick = 0;
	int32_t time = 0;

	processStarted = 1;

	Time myTime;
	Time_ctor(&myTime, 0, 0, 0);

	device_t *serial_device;
	DEV_UID serial_device_uid;

	//device_init();

	serial_device = rm_device_open(&serial_device_uid, AP7000_UART_DEVICE, 1);




	device_t *led1_device;
	DEV_UID led1_device_uid;

	led1_device = device_open(&led1_device_uid, AP7000_LED_DEVICE, LED1);

	if (serial_device == 0 && led1_device == 0) {
		while (1) {
		}
	}

	switch (serial_device_uid) {
	case DEV_NOT_FREE:
		while (1) {}
	case DEV_NOT_FOUND:
		while (1) {}
	}

	switch (led1_device_uid) {
	case DEV_NOT_FREE:
		while (1) {}
	case DEV_NOT_FOUND:
		while (1) {}
	}

	while (1) {

		int16_t tmp = rm_device_read_byte(serial_device, serial_device_uid);

		if (tmp >= 0)
			rm_device_write_byte(serial_device, serial_device_uid, tmp);


		time = sysTick;

		if ((time - oldTick) >= 400) {
			oldTick = time;
			Time_increment(&myTime);

			rm_device_write_string(serial_device, serial_device_uid,
			Time_toString(&myTime));
			rm_device_write_string(serial_device, serial_device_uid, "\n");

			if (lflag) {
				device_led_On(led1_device, led1_device_uid);
				lflag = 0;
			} else {
				device_led_Off(led1_device, led1_device_uid);
				lflag = 1;
			}

		}

	}
}
