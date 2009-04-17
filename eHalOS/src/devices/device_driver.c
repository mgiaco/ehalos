#include "device_driver.h"

#include "Halos_Config.h"

#include "device_list.h"
#include "ap7000_lcd.h"
#include "resource_manager.h"

static uint32_t new_dev_id = DEV_UID_START;

DEV_UID generate_dev_uid(void){
	//return next free UID
	return new_dev_id++;
}

void init_all_drivers(void) {

	init_resource_manager();

	device_t *driver = actDevices;
	while (*driver->init != NULL) {
		driver->init();
		driver ++;
	}

	initGraph();
}

device_t *open_driver(DEV_UID *dev_uid, uint32_t device_type, uint32_t device_number) {
	device_t *driver = actDevices;
	while (*driver->init != 0) {
		//driver->device_uid = driver->open(device_type, device_number);
		//*dev_uid = driver->device_uid;
		*dev_uid = driver->open(device_type, device_number);

		if (*dev_uid == DEV_NOT_FREE) {
			return driver;
		}else if (*dev_uid>0) {
			return driver;
		}

		driver++;
	}
	// no driver is found
	return (device_t *)0;
}

int32_t read_driver(DEV_UID dev_uid, device_t *dev, void* data) {
	if (dev==(device_t *)0) {
		return 0;
	}
	return dev->read(dev_uid, data);
}

void write_driver(DEV_UID dev_uid, device_t *dev, void* data) {
	if (dev==(device_t *)0) {
		return;
	}
	dev->write(dev_uid, data);
}

int32_t close_driver(device_t *dev, DEV_UID dev_uid) {
	if (dev==(device_t *)0) {
		return -1;
	}
	return dev->close(dev_uid);
}

