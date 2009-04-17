/*
 ============================================================================
 Name        : resource_manager.c
 Author      : zks
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

/*
	serial_device = device_open(&serial_device_uid, AP7000_UART_DEVICE, 1);

	switch (serial_device_uid) {
	case DEV_NOT_FREE:
	case DEV_NOT_FOUND:
	case CHOOSE_NEXT_DEV:
		return ERROR;
	}

*/

#include "resource_manager.h"
#include "hashmap.h"
#include "device_driver.h"
#include "my_usart.h"
#include "sched.h"
#include "graphics.h"
#include "ap7000_lcd.h"
#include "hgdi_api.h"

PCtrlBlock_t* running_task;
//Virtual_UID_Entry* virtual_uid_head;
PCtrlBlock_t* foreground_process;
map_t device_to_real_uid_map;
map_t virt_uid_to_virt_dev_map;
map_t pid_to_virt_dev_map;

map_t pid_to_gdi_uid;

DEV_UID Virtual_UID_base = 1000;
DEV_UID gid_uid_base = 1;

DEV_UID ret_dev_null_uid;
device_t* dev_null;

static int32_t foreground_changed = 0;
// TODO:


/*
 * Mannnnn Karl scho wieder Code eingeckekt wo net goooooooooooooot
 * DEV_MAX ??
 * */
#define DEV_MAX  10

//es isch alls so a gschwür
static access_entry_t* access_matrix[DEV_MAX];


DEV_UID generate_gdi_UID(){
	return gid_uid_base++;
}

int8_t init_resource_manager(){

	foreground_changed = 0;

	dev_null = device_open(&ret_dev_null_uid,AP7000_NULL_DEVICE,0);

	device_to_real_uid_map = hashmap_new();
	virt_uid_to_virt_dev_map = hashmap_new();
	pid_to_virt_dev_map = hashmap_new();
	pid_to_gdi_uid = hashmap_new();

	return 0;
}


void check_ressource(PCtrlBlock_t* new_running_process) {

	if (new_running_process != ptrIdleTask) {
		virtual_dev_t* virtual_dev = NULL;

		hashmap_get(pid_to_virt_dev_map, new_running_process->pid, &virtual_dev);

		if (new_running_process == foreground_process) {

			setDevice(0);
			if (virtual_dev != NULL) {
				virtual_dev->current_device_type = virtual_dev->requested_device_type;
			}

			if (foreground_changed == 1) {
				foreground_changed = 0;

				//			setBackgroundColor(BLACK);
				//			setColor(WHITE);
				//			rectangle(0,0, GLCD_RIGHT, GLCD_BOTTOM);
				//			gotoXY(0,0);
			}
		} else {
			if (virtual_dev != NULL) {
				virtual_dev->current_device_type = dev_null;
			}
			setDevice(1);
		}

	}



//
//	virtual_dev_t* virtual_dev = NULL;
//
//	hashmap_get(pid_to_virt_dev_map,new_running_process->pid, &virtual_dev);
//
////	}else{
////		while(1){}
////	}
//
//	if(new_running_process == foreground_process){
//		if(virtual_dev!= NULL){
//			virtual_dev->current_device_type = virtual_dev->requested_device_type;
//		}
//		setDevice(0);
//	}else{
//		if(virtual_dev!= NULL){
//			virtual_dev->current_device_type = dev_null;
//		}
//		setDevice(1);
//	}


}

void rm_initGraph(void) {

	DEV_UID *gid_uid = (DEV_UID*)malloc(sizeof(DEV_UID));

	*gid_uid = generate_gdi_UID();

	//TODO: save pid to gdi_id
	hashmap_put(pid_to_gdi_uid, ptrRunningTask->pid, gid_uid);

	//SetDeviceContext()
	// init lcd for this pid
	// set device context for this pid

}

void swtich_to_next_running(){

	// TODO: maex

	// get head of running queue
	// search for ptr_running
	// try to choose the next for fore_ground

	//set_foreground_process();
}

void set_foreground_process(PCtrlBlock_t* fg_process){
	foreground_process = fg_process;
	foreground_changed = 1;

	check_ressource(fg_process);
	//rm_switch_focus();
}



int8_t init_device_list(device_t** available_devices){
	int i;

	if(available_devices == NULL){
		return ERROR;
	}

	for ( i=0; i < DEV_MAX; i++){
		access_matrix[i] = malloc(sizeof(access_entry_t));
		access_matrix[i]->device = available_devices[i];
	}

	return SUCCESS;
}

int8_t is_device_free(uint32_t dev_type, uint32_t dev_no, access_entry_t* access_entry){
	int i;

	for(i=0; i < DEV_MAX; i++) {
		if((access_matrix[i]->device->driver_type == dev_type)&&((access_matrix[i]->device->device_uid >> 0x18) == dev_no))
		{
			 if(access_matrix[i]->occupying_task == NULL){
				 access_entry = access_matrix[i];
				 return FREE;
			 }else{
				 access_entry = access_matrix[i];
				 return BUSY;
			 }
		}
	}

	access_entry = NULL;

	return NOT_AVAILABLE;
}


void rm_switch_focus(void){

	//TODO: scheduler prozessliste holen

	//aktuellen prozess auf fordergrund prüfen

	processPtrBlock_t* all_tasks = getTaskListCollection();
	PCtrlBlock_t* temp;
	foreground_changed = 1;

//	real_buffer_clear();
//	gotoXY(0,0);
	initGraph();   //change
	//setDevice(1);

	while(all_tasks!= NULL){
		all_tasks->ptrProcessPCB;
		if(all_tasks->ptrProcessPCB==NULL){
			return;
		}

		if(all_tasks->ptrProcessPCB == foreground_process){
			if(all_tasks->ptrNext!=NULL){
				all_tasks = all_tasks->ptrNext;
				foreground_process = all_tasks->ptrProcessPCB;

				return;
			}
			else{
				all_tasks = getTaskListCollection();
				foreground_process = all_tasks->ptrProcessPCB;
				return;
			}
		}
		all_tasks = all_tasks->ptrNext;
	}
}


//Virtual_UID_Entry* generateVirtualUID(DEV_UID dev_UID){
////	Virtual_UID_Entry* virt = malloc(sizeof(Virtual_UID_Entry));
////	Virtual_UID_Entry* current_node = NULL;
////	Virtual_UID_Entry* next_node = NULL;
////	//TODO
////	virt.virtual_UID= dev_UID;
////	virt.real_UID = dev_UID;
////	virt.next = NULL;
////
////	current_node = virtual_uid_head;
////	next_node = virtual_uid_head;
////
////	while(next_node != NULL){
////		virtual_uid_head = next_node;
////		next_node = next_node->next;
////	}
////	//current_node.next = virt;
//
//	return virt;
//}

DEV_UID generate_Virtual_UID(){
	return Virtual_UID_base++;
}

int8_t get_device(int32_t device_type, uint32_t dev_no, DEV_UID* ret_dev_id, device_t* ret_device){
	access_entry_t* entry;
	waiting_node_t* waiting_node;
	waiting_node_t* next_node;
	int8_t dev_state = is_device_free(device_type, dev_no, entry);

	if(dev_state == FREE){
		ret_device = entry->device;
		//TODO CHECK!!!!!
		entry->occupying_task = get_running_task();
		ret_dev_id = entry->device->device_uid;
		return SUCCESS;
	}else if(dev_state == BUSY){
		running_task = get_running_task();
		running_task->state = TASK_IOBLOCKED;

		waiting_node = entry->waiting_node_head;
		if(waiting_node == NULL){
			waiting_node = malloc(sizeof(waiting_node_t));
			waiting_node->next = NULL;
			waiting_node->watiting_task = running_task;
			return SUCCESS;
		}
		while(next_node != NULL){
			waiting_node = next_node;
			next_node = waiting_node->next;
		}

		waiting_node->next = malloc(sizeof(waiting_node_t));
		waiting_node->next->next = NULL;
		waiting_node->next->watiting_task = running_task;
		//TODO CALL SCHEDULER, SUSPEND PROCESS
		return SUCCESS;
	}
	return ERROR;
}

int8_t release_device(device_t* device){
	int i;
	access_entry_t* entry;
	waiting_node_t* waiting_node;
	waiting_node_t* release_node;

	running_task = get_running_task();

	for(i=0; i < DEV_MAX; i++) {
		if((access_matrix[i]->device->device_uid == device->device_uid))
		{
			entry = access_matrix[i];
			waiting_node = entry->waiting_node_head;

			if(waiting_node == NULL){
				return SUCCESS;
			}
			release_node = waiting_node;
			waiting_node->watiting_task->state = TASK_RUNNABLE;

			waiting_node = waiting_node->next;

			free(release_node);
			entry->occupying_task = waiting_node->watiting_task;
			//TODO CALL SCHEDULER
		}
	}

	return NOT_AVAILABLE;
}


int8_t get_uart(){


	return ERROR;
}

int8_t release_uart(){


	return ERROR;
}


int8_t get_display(){


	return ERROR;
}

int8_t release_display(){


	return ERROR;
}

//(DEV_UID *dev_uid, uint32_t device_type, uint32_t device_number)
device_t* ressource_open(DEV_UID *virt_dev_uid, int32_t device_type, int32_t device_number){
	//DEV_UID* real_UID = malloc(sizeof(DEV_UID));
	DEV_UID* temp_UID = malloc(sizeof(DEV_UID));
	virtual_dev_t *virtual_dev = (virtual_dev_t*)malloc(sizeof(virtual_dev_t));
	virtual_dev->real_uid = (DEV_UID*)malloc(sizeof(DEV_UID));
	virtual_dev_t *virtual_temp = NULL;
	device_t* device;
	any_t tmp;

	// virtual_devic zusammen bauen

	switch(device_type){
		case(AP7000_UART_DEVICE):

			device = open_driver(temp_UID, AP7000_UART_DEVICE, device_number);
			virtual_dev->requested_device_type = device;
			virtual_dev->vdev_rxbuf = malloc(sizeof(SerialBuffer));
			virtual_dev->vdev_txbuf = malloc(sizeof(SerialBuffer));
			SerialBuffer_ctor( (SerialBuffer*)(virtual_dev->vdev_rxbuf) );
			SerialBuffer_ctor( (SerialBuffer*)(virtual_dev->vdev_txbuf) );

			uart1_set_rx_and_tx_buffer((SerialBuffer*)virtual_dev->vdev_txbuf,(SerialBuffer*)virtual_dev->vdev_rxbuf);
			break;
		case(AP7000_LED_DEVICE):
			break;
		default:
			break;
	}

	switch (*temp_UID) {
		case DEV_NOT_FREE:
			//generate new virtual UID an add to map
			*virt_dev_uid = generate_Virtual_UID();
			//Find real uid in hashmap with key: device_type|device_number
			//hashmap_get(device_mapm)


			if(hashmap_get(device_to_real_uid_map,device_type|device_number,&tmp) == MAP_OK){
				*temp_UID = *((DEV_UID*)tmp);
				virtual_dev->real_uid = temp_UID;
			}else{
				while(1){}
			}

			if(hashmap_get(device_to_real_uid_map,device_type|device_number,&tmp) == MAP_OK){
				hashmap_put(virt_uid_to_virt_dev_map,*virt_dev_uid, virtual_dev);

			// save pid to virtual_dev
			hashmap_put(pid_to_virt_dev_map, ptrRunningTask->pid, virtual_dev);

			virtual_dev->current_device_type = dev_null;

			return device;

			}else{
				while(1){}
			}

		case DEV_NOT_FOUND:
			//TODO you're fucked...
			break;
	}

	*virt_dev_uid = generate_Virtual_UID();
	*virtual_dev->real_uid = *temp_UID;
	virtual_dev->current_device_type = virtual_dev->requested_device_type;
	//add as first element
	hashmap_put(device_to_real_uid_map,device_type|device_number,(any_t)temp_UID);

	if(hashmap_get(device_to_real_uid_map,device_type|device_number,&tmp) == MAP_OK){
		if(*temp_UID!=*(DEV_UID*)tmp){
			while(1){};
		}
		hashmap_put(virt_uid_to_virt_dev_map,*virt_dev_uid, virtual_dev);
	}else{
		while(1){}
	}


	// save pid to virtual_dev
	hashmap_put(pid_to_virt_dev_map, ptrRunningTask->pid, virtual_dev);

	return device;
}

int32_t ressource_close(device_t *dev, DEV_UID virt_dev_uid){
	//TODO free
//	if(hashmap_get(virutal_uids, virt_dev_uid, &tmp)==MAP_OK){
//		close_driver(*((DEV_UID*)tmp),dev);
//	}else{
//		while(1){}
//	}
	return -1;
}


int32_t ressource_read(DEV_UID virt_dev_uid, device_t *dev, void* data){
	any_t tmp;
	virtual_dev_t* virtual_dev = NULL;

	if(hashmap_get(virt_uid_to_virt_dev_map, virt_dev_uid, &tmp)==MAP_OK){
		virtual_dev = (virtual_dev_t*)tmp;
		read_driver(*virtual_dev->real_uid,virtual_dev->current_device_type,data);
		return 0;
	}else{
		while(1){}
	}
	return -1;
}

void ressource_write(DEV_UID virt_dev_uid, device_t *dev, void* data){
	any_t tmp;
	virtual_dev_t* virtual_dev = NULL;

	if(hashmap_get(virt_uid_to_virt_dev_map, virt_dev_uid, &tmp)==MAP_OK){
		virtual_dev = (virtual_dev_t*)tmp;
		write_driver(*virtual_dev->real_uid,virtual_dev->current_device_type,data);
	}else{
		while(1){}
	}
}



