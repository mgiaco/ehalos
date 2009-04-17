#include "../inc/hsystem_api.h"
#include "../../halos_gdi/inc/hgdi_api.h"
#include <stdlib.h>
void system_call0(int32_t scall_number, void* ret_param){
	system_call4(scall_number,0,0,0,ret_param);
}

void system_call1(int32_t scall_number, int32_t param1){
	system_call4(scall_number,param1,0,0,0);
}

void system_call2(int32_t scall_number, int32_t param1, int32_t param2){
	system_call4(scall_number,param1,param2,0,0);
}

void system_call4(int32_t scall_number, int32_t param1, int32_t param2, int32_t param3, void* ret_param) {
	asm volatile("scall \n\t" ::);
}

void start_process(char *processName) {
	system_call0(SCALL_START_PROCESS, (void*)processName);
}

int32_t kill_process(char *pidName){
	// parse an int out of pidName
	uint32_t pid = 0;// = atoi(pidName);
	//uint32_t *pPID = malloc(sizeof(uint32_t));

	draw_string(pidName);
	draw_char('X');

	switch(pidName[0]){
		case '1':
			pid = 1;
			break;
		case '2':
			pid = 2;
			break;
		case '3':
			pid = 3;
			break;
		case '4':
			pid = 4;
			break;
		case '5':
			pid = 5;
			break;
		case '6':
			pid = 6;
			break;
		case '7':
			pid = 7;
			break;
		case '8':
			pid = 8;
			break;
		case '9':
			pid = 9;
			break;
	}

	if (pid != 0){
		system_call0(SCALL_KILL_PROCESS, (void *)pid);
	}
	return pid;
}

uint64_t get_kernel_time(){
	uint64_t time;
	system_call0(SCALL_SYSTEM_TIME, (void*)&time);
	return time;
}

int8_t get_nr_of_running_processes(){
	int8_t nr;
	system_call0(SCALL_NR_PROCESSES,(void*)&nr);
	return nr;
}

void get_running_processes(process_info_block_t *ret_parm){
	system_call4(SCALL_SHOW_PROCESSES,0,0,0,ret_parm);
}
