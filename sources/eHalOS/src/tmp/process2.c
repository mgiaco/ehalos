#include "process2.h"

void user_process2(void) {

	int32_t testdata = 0;
	int32_t lflag = 0;
	int32_t oldTick = 0;
	int32_t time = 0;

	Time myTime;
	Time_ctor(&myTime, 0, 0, 0);

	//device_init();

	device_t *led0_device;
	DEV_UID led0_device_uid;

	led0_device = device_open(&led0_device_uid, AP7000_LED_DEVICE, LED0);

	if (led0_device == 0) {
		while (1) {}
	}

	switch (led0_device_uid) {
	case DEV_NOT_FREE:
		while (1) {}
	case DEV_NOT_FOUND:
		while (1) {}
	}



	/* Start Shell via SysCAll */


	while (1) {

		testdata++;

		if ((testdata >= 50000)) {
			testdata = 0;
		}

		time = sysTick;

		if ((time - oldTick) >= 200) {
			oldTick = time;
			Time_increment(&myTime);

			if (lflag) {
				device_led_On(led0_device, led0_device_uid);
				lflag = 0;
			} else {
				device_led_Off(led0_device, led0_device_uid);
				lflag = 1;
			}

		}
	}
}

