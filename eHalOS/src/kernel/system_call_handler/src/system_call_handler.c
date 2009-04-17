#include "system_call_handler.h"
#include "fonts.h"
#include "sched.h"
#include "gdi_types.h"
#include "pcb.h"
#include "resource_manager.h"
#include "halos.h"

static int32_t var_device_init = 0;

void scall_Handler(int32_t scall_number, int32_t param1, int32_t param2, int32_t param3, void* ret_param) {

	PCtrlBlock_t *ptrTask = NULL;
	ProcessRegionPtr pProcessRegion = NULL;

	switch (scall_number) {

	case SCALL_DEVICE_INIT:

		if (!var_device_init) {
			var_device_init = 1;
			init_all_drivers();
		}
		*((int32_t*)ret_param) = 999;

		break;

	case SCALL_DEVICE_OPEN:
		*((uint32_t*)ret_param) = (uint32_t)open_driver((DEV_UID*)param1, param2, param3);
		break;

	case SCALL_DEVICE_CLOSE:
		*((int32_t*)ret_param) = (int32_t) close_driver((device_t *)param1, (DEV_UID)param2);
		break;

	case SCALL_DEVICE_WRITE:
		write_driver(param2, (device_t *)param1, (void*)&param3);
		break;

	case SCALL_DEVICE_READ:
		read_driver(param2, (device_t *)param1, ret_param);
		break;

	case SCALL_RM_DEVICE_OPEN:
		*((uint32_t*)ret_param) = (uint32_t)ressource_open((DEV_UID*)param1, param2, param3);
		break;

	case SCALL_RM_DEVICE_CLOSE:
		*((int32_t*)ret_param) = (int32_t) ressource_close((device_t *)param1, (DEV_UID)param2);
		break;

	case SCALL_RM_DEVICE_WRITE:
		ressource_write(param2, (device_t *)param1, (void*)&param3);
		break;

	case SCALL_RM_DEVICE_READ:
		ressource_read(param2, (device_t *)param1, ret_param);
		break;

	case SCALL_RM_GDI_OPEN:
		rm_initGraph();
		break;

	case SCALL_GDI_OPEN:
		initGraph();
		break;

	case SCALL_GDI_GOTOXY:
		gotoXY(param1,param2);
		break;

	case SCALL_GDI_SET_X:
		setX(param1);
		break;

	case SCALL_GDI_GET_X:
		*((uint16_t*)ret_param) = (uint16_t)getX();
		break;

	case SCALL_GDI_SET_Y:
		setY(param1);
		break;

	case SCALL_GDI_GET_Y:
		*((uint16_t*)ret_param) = (uint16_t)getY();
		break;

	case SCALL_GDI_GET_SCREENWIDTH:
		*((uint16_t*)ret_param) = (uint16_t)getScreenWidth();
		break;

	case SCALL_GDI_GET_SCREENHEIGHT:
		*((uint16_t*)ret_param) = (uint16_t)getScreenHeight();
		break;

	case SCALL_GDI_CLR_SCR:
		clrScr(param1);
		break;

	case SCALL_GDI_SET_COLOR:
		setColor(param1);
		break;

	case SCALL_GDI_GET_COLOR:
		*((uint32_t*)ret_param) = (uint32_t)getColor();
		break;

	case SCALL_GDI_SET_BG_COLOR:
		setBackgroundColor(param1);
		break;

	case SCALL_GDI_GET_BG_COLOR:
		*((uint32_t*)ret_param) = (uint32_t)getBackgroundColor();
		break;

//	case SCALL_GDI_SET_FONT_COLOR:
//		setBackgroundColor(param1);
//		break;

	case SCALL_GDI_VPAN:
		verticalPan(param1);
		break;

	case SCALL_GDI_PUT_PIXEL:
		putPixel(param1, param2);
		break;

	case SCALL_GDI_CLR_PIXEL:
		clrPixel(param1, param2);
		break;

	case SCALL_GDI_DRAW_LINE:
		line(param1,param2,param3,*((int16_t*)ret_param));
		break;

	case SCALL_GDI_DRAW_RECTANGLE:
		rectangle(param1,param2,param3,*((int16_t*)ret_param));
		break;

	case SCALL_GDI_DRAW_CIRCLE:
		circle(param1,param2,param3);
		break;

	case SCALL_GDI_DRAW_SPRITE:
		sprite( (uint8_t*)ret_param, param1, param2);
		break;
	case SCALL_GDI_DRAW_SPRITE_2:
		sprite2((gdi_sprite_t*)ret_param);
		break;

	case SCALL_GDI_DRAW_COLOR_SPRITE:
		color_sprite((gdi_sprite_t*)ret_param);
		break;

	case SCALL_GDI_FILL_RECTANGLE:
		fillRectangle(param1,param2,param3,*((uint16_t*)ret_param));
		break;

	case SCALL_GDI_FONT_SELECT:
		select_new_font((halos_font_t)param1,read_font_data,0x000000);
		break;

	case SCALL_GDI_FONT_PUTCHAR:
		*((uint8_t*)ret_param) = glcd_draw_char((char)param1);
		break;

	case SCALL_GDI_FONT_PUTSTRING:
		glcd_draw_string((char*)ret_param);
		break;

	case SCALL_GDI_FONT_SETTEXTJUSTIFY:
		glcd_set_text_justify(param1,param2);
		break;

	case SCALL_GDI_FONT_GETCHARWIDTH:
		*((uint16_t*)ret_param) = glcd_get_char_width((char)param1);
		break;

	case SCALL_GDI_FONT_GETSTRINGWIDTH:
		*((uint16_t*)ret_param) = glcd_get_string_width((char*)param1);
		break;

	case SCALL_GDI_FONT_GETFONTHEIGHT:
		*((uint8_t*)ret_param) = glcd_get_font_height();
		break;

	case SCALL_GDI_SET_LINETHICKNESS:
		setLineThickness(param1);
		break;

	case SCALL_GDI_SET_LINESTYLE:
		setLineStyle(param1);
		break;

	case SCALL_START_PROCESS:
		setup_and_start_new_process((char*)ret_param);
		break;

	case SCALL_NR_PROCESSES:
		*((int8_t*)ret_param) = get_nr_of_processes();
		break;

	case SCALL_SHOW_PROCESSES:

	//	temp = ((process_info_block_t*)param1);

		top((process_info_block_t*)ret_param);
		//ret_param = temp;
		break;

	case SCALL_KILL_RUNNING_PROCESS:

		// HACK, shoud have different TASK State
		rm_switch_focus();

		//Kill running process
		changeTaskStateAndReorg(ptrRunningTask, TASK_TO_BE_DELETED);

		break;

	case SCALL_SYSTEM_TIME:
		*((uint64_t*)ret_param) = get_time();
		break;

	case SCALL_KILL_PROCESS:

			//Kill process with given PID
			ptrTask = findProcess((uint32_t)ret_param);
			if (ptrTask != NULL){
				changeTaskStateAndReorg(ptrTask, TASK_TO_BE_DELETED);
			}else{
				*((uint32_t*)ret_param) = 99999;
			}
		break;

	case ALLOCATE_IN_PROCESS_REGION_TABLE:
		// allocation in actual process
/*
		// TODO: read size and pMemory from return Args
		pProcessRegion = findRegion(activeASID, size, pMemory);
		if (pProcessRegion == NULL){
			// TODO: complete Code
			// ProcessRegionPtr createProcessRegion(RAMPAGERID, startVPN, nrOfVPNs, "data", accessRightsSize, accessRights);
			// BOOL succ = addRegion(int ASID, pProcessRegion);
		}
		else
		{
			// update Rights of Region...
			addRightToProcessRegion(pProcessRegion, Read);
			addRightToProcessRegion(pProcessRegion, Write);
		}
*/
		break;

	case FREE_IN_PROCESS_REGION_TABLE:
		// TODO free only complete Process Region if access rights are read and write
		break;
	default:
		while(1){}
		break;

	}
}
