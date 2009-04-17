#include "main.h"
#include "title.h"
#include "space_invaders.h"



device_t *serial_device;
DEV_UID serial_device_uid;

volatile uint8_t quit = 0;

void process_input(){

		int16_t in = 0;

		in = rm_device_read_byte(serial_device, serial_device_uid);


		if(in == 'a'){
			player_move_left();
		}

		if(in == 'd'){
			player_move_right();
		}

		if(in == 's'){
			player_shoot();

		}

		if(in == 'o'){
			quit = 1;
		}


		if(in != -1){
			rm_device_write_byte(serial_device, serial_device_uid,(char)in);
		}
}


int main(void) {

	long ticks=0;

	//initialize the devices
	//device_init();

	//open the UART driver
	serial_device = rm_device_open(&serial_device_uid, AP7000_UART_DEVICE, 1);


	do{
		//init the game
		game_init();

		//draw the space invaders splash screen
		draw_splash();

		volatile uint32_t i;

		//insert a little break;
		for(i=0; i<1500000; i++){}

		//clear screen and switch to game
		set_color(BLACK);
		clr_scr(1);

		uint8_t running = 1;

		player_move_left();
		player_move_right();

		 do{
			ticks++;

			running = game_step();

			process_input();
		}while(running && !quit);


		set_color(BLACK);

		clr_scr(1);
	}while(!quit);

	//close the UART driver
	//rm_device_close(serial_device, serial_device_uid);


	system_call1(SCALL_KILL_RUNNING_PROCESS, 0);

	while(1){

	}

	return 0;
}

