/**
 * @file resource_manager.h
 * @brief Hashmap
 * @author Team Halos
 * @version 0.0.1
 * @date 2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup RESOURCE_MANAGER Resource Manager
 *  @ingroup KERNEL
 */

/**
 *  @defgroup RESOURCE_M Resource Manager for device access
 *  @ingroup RESOURCE_MANAGER
 *  @brief Manages the exclusive usage of devices (GDI, UART, ...), used for foreground and background processes
 *
 *  @note ...
 *  @author karl, giaco, momo
 *  @version 0.0.1
 */

/**@{*/


#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>

//#include "../../../halos_apis/halos_device_api/inc/hdevice_api.h"
#include "device_types.h"
#include "device_struct.h"
#include "pcb.h"
#include "sched.h"
#include "error_types.h"

#include "SerialBuffer.h"


#define BUSY 1
#define FREE 0
#define NOT_AVAILABLE -1
#define NOT_LOADED -2

/**
 * @brief virtual device structure, which keeps track of the requested physical device. RXBuffer and TXbuffer are kept seperately for each virtual device
 *
 */
typedef struct virtual_dev_struct{
	volatile void *vdev_txbuf;
	volatile void *vdev_rxbuf;
	device_t* requested_device_type;
	device_t* current_device_type;
	DEV_UID* real_uid;
}virtual_dev_t;

/**
 * @brief planned to implement a waiting queue for ressources
 */
typedef struct waiting_node{
	PCtrlBlock_t* watiting_task;
	struct waiting_node* next;
}waiting_node_t;

/**
 * @brief planned to implement access entries to handle accesses from processes to devices
*/
typedef struct access_entry{
	device_t* device;
	PCtrlBlock_t* occupying_task;
	waiting_node_t* waiting_node_head;
}access_entry_t;

///**
// * @brief planned mapping from virtual UIDs to real UIDs
//*/
//typedef struct virtual_UID_entry{
//	DEV_UID virtual_UID;
//	DEV_UID real_UID;
//	struct virtual_UID_entry* next;
//	//int DEV_NO;
//}Virtual_UID_Entry;

/**
 * @brief initializes a graphics context for the process which is currently running
*/
void rm_initGraph(void);

/**
 * @brief checks if new_running_process is a foreground process and has therefor the focus of the screen and other shared devices. has to be executed after each scheduling step.
 * @param new_running_process process to assign ressources to
 */
void check_ressource(PCtrlBlock_t* new_running_process);

/**
 * @brief sets the current foreground process, which gains the focus of LCD and shared devices
 * @param fg_process the process to become foreground
 */
void set_foreground_process(PCtrlBlock_t* fg_process);

/**
 * @brief switches the focus to the next process in the process queue. gets the current runnable processes from the scheduler
 */
void rm_switch_focus(void);

/**
 * @brief planned but not yet implemented
 */
int8_t is_device_free(uint32_t dev_type, uint32_t dev_no, access_entry_t* access_entry);

/**
 * @brief planned but not yet implemented
 */
int8_t get_device(int32_t device_type, uint32_t dev_no, DEV_UID* ret_dev_id, device_t* device);

/**
 * @brief planned but not yet implemented
 */
int8_t get_display();

/**
 * @brief writes a unit of data to the device
 * @param dev_uid unique device number
 * @param dev device independent structure
 * @param  pointer to data
 */
int8_t release_device(device_t* device);

/**
 * @brief initializes the ressouce manager; initializes the used hash maps, initializes the null device
 * @param dev_uid unique device number
 * @param dev device independent structure
 * @param  pointer to data
 */
int8_t init_resource_manager();

/**
 * @brief opens the requested ressource and initializes a virtual_dev_t for the request. if a ressource is not yet used, the returned virtual device uid enables to write to the real device. if the ressource is currently used, the virtual_dev_t points to the null device
 * @param virt_dev_uid return value, the virtual device uid
 * @param device_type the requested device, e.g. AP7000_UART_DEVICE
 * @param device_number eg. 1 for COM1
 */
device_t* ressource_open(DEV_UID *virt_dev_uid, int32_t device_type, int32_t device_number);

/**
 * @brief reads data from a device using the virutal_device_uid
 * @param dev_uid virtual device uid
 * @param dev pointer to device struct
 * @param data pointer to data buffer
 */
extern int32_t ressource_read(DEV_UID dev_uid, device_t *dev, void* data);


/**
 * @brief writes a unit of data to the device using the virtual device uid
 * @param dev_uid virtual device uid
 * @param dev device independent structure
 * @param  pointer to data
 */
extern void ressource_write(DEV_UID dev_uid, device_t *dev, void* data);

/**
 * @brief closes a device
 * @param dev pointer to the device struct
 * @param dev_uid virtual device UID
 */
extern int32_t ressource_close(device_t *dev, DEV_UID dev_uid);


/**@}*/

#endif /*RESOURCE_MANAGER_H_*/


