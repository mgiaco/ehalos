/*
 ============================================================================
 Name        : shell.c
 Author      : zks
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * LIST OF BUGS:
 * -on sending of non-ascii chars (or wrong UART BAUD rate), colors + screenoffset get broken GDI BUG?
 * -on clear page, sometimes expected last line is drawn as first line
 *
 *
 *
 *
 *
 */

/**************/
//TODO: move to api
#define TASK_INITIALIZED        0
#define TASK_RUNNABLE			1
#define TASK_RUNNING            2
#define TASK_IOBLOCKED			3
#define TASK_SUSPENDED			4
#define TASK_STOPPED            5
#define TASK_TO_BE_DELETED      6
#define TASK_DELETE				7



/**************/

#include "shell.h"
#include "halosUtil.h"

const int8_t nr_output_devices = 4;
//static device_t* output_devices[nr_output_devices]; //verwendet, wenn Output über mehrere Schnittstellen ausgegeben werden soll
//static device_t* output_devices[nr_output_devices]; // ---"---  für Input

DEV_UID std_input;//input device
DEV_UID std_output; //output device
char* EXECUTABLE_PATH; //Ordner in dem Executables liegen
char** commandTable;//Tabelle, die alle geladenen Kommandos hält
char line[MAX_COMMAND_LENGTH];
const char halos_prompt[] = {"halos> "};
const char halos_space[] =  {" "};
const char halos_except[] =  {"COMMAND IGNORED: exceeded maximum command length"};
const char help[] = {"type --help after each command for info of usage"};
uint16_t screen_width = 0;
uint16_t screen_height = 0;
int32_t last_color = 0;
int16_t history_items = 0;
int8_t prompt_width = 0;

device_t *serial_device;
DEV_UID serial_device_uid;
int16_t x_pos = INITX;
int16_t y_pos = INITY;
int8_t drawCursor = 0;

COMMAND_NODE* command_list_head;
COMMAND_NODE* command_list_last;

CONSOLE_LINE* history_first;
CONSOLE_LINE* history_last;

//////////////////////////////////////////////
///										   ///
/// 	Implement and add commands here    ///
///										   ///
//////////////////////////////////////////////

int8_t display_help(int argc, char* argv[]){
	const char helpLine[] = {"following commands are available:\r\n"};

	COMMAND_NODE* tmp = command_list_head;

	draw_console_line(helpLine,TEXT,TEXTTYPE);
	new_line();

	set_x_pos(INITX+prompt_width);

	while(tmp != NULL){
		set_color(COMMAND);
		draw_string(tmp->command);
		write_line(tmp->command);
		set_x_pos(get_string_width(tmp->command)+INITX+prompt_width);
		draw_char(' ');
		draw_char('-');
		draw_char('>');
		draw_char(' ');
		write_line(" -> ");
		//draw_console_line(halos_space,COMMAND,COMMANDTYPE);
		set_color(TEXT);
		//console_draw_string(
		draw_string(tmp->short_description);
		write_line(tmp->short_description);
		new_line();
		write_byte(CR); write_byte(LF);
		tmp = tmp->nextCommand;
	}
	new_line();
	draw_console_line(help,TEXT,TEXTTYPE);

	return SUCCESS;
}

int8_t commandTemplate(int argc, char*argv[]){
	char* str = NULL;
	const char usage[] = {"this command is used this way...\r\n"};
	const char help[] = {"--help"};

	if(argc > 0){
		if (strcmp(help,str = argv[0])== 0){
			set_color(TEXT);
			console_draw_string(usage);
			new_line();
			str = NULL;
			return ERROR;
		}
	}
	draw_console_line("hello i\'m a template don\'t call me\r\n",TEXT,TEXTTYPE);
	new_line();
	//TODO loader
	//call loader to execute program
	return SUCCESS;
}

int8_t top_command(int argc, char*argv[]){
	//char* str;
	//char* usage = "sorry, this command does not take any parameters. glad to help you anyway.\r\n";
	//char* help = "--help";
	int i = 0;
	process_info_block_t* p_head = NULL;
	process_info_block_t* process_info  = NULL;

	int8_t pid = 0;
	int32_t state  = 0;
	char top_string[255];

//	if(argc > 0){
//		if (strcmp(help,str = argv[0])== 0){
//			set_color(TEXT);
//			console_draw_string(usage);
//			new_line();
//			return ERROR;
//		}
//	}
	draw_console_line("Running Processes: ",TEXT,COMMAND);
	new_line();
	draw_console_line("PID   |   NAME   |   STATE ",TEXT,COMMAND);
	new_line();
	int8_t nr_of_procs = get_nr_of_running_processes();
	//nr_of_procs += 3;

	p_head = malloc(sizeof(process_info_block_t));//*nr_of_procs);
	process_info = p_head;

	for (i =0; i < nr_of_procs; i++){
			process_info->cpu_time_running = i;
			process_info->cpu_time_waiting = i;
			process_info->pid = i;
			process_info->sched_priority = i;
			process_info->start_time = i;
			process_info->state = i;

			if(i != nr_of_procs-1){
				process_info->ptrNext = (process_info_block_t*)malloc(sizeof(process_info_block_t));
				process_info = process_info->ptrNext;
			}else{
				process_info->ptrNext = NULL;
			}
	}

	process_info = p_head;

	//p_head = malloc(sizeof(processInfoBlock_t)*nr_of_procs);

	get_running_processes(process_info);
	//process_info = p_head;

	process_info = p_head;

	do{
		if(process_info!=NULL){

			//memset(top_string, '\0', sizeof(top_string)/sizeof(char) );
			pid = process_info->pid;

			//TODO CPU TIME CALC

			strcpy(top_string,halos_itoa(pid));
			strcat(top_string," || ");
			strcat(top_string,process_info->processName);
			strcat(top_string," || ");


			switch(process_info->state){
				case(TASK_INITIALIZED):
					strcat(top_string,"init");
					break;
				case(TASK_DELETE):
					strcat(top_string,"deleted");
					break;
				case(TASK_IOBLOCKED):
					strcat(top_string,"iowaiting");
					break;
				case(TASK_RUNNABLE):
					strcat(top_string,"runnable");
					break;
				case(TASK_RUNNING):
					strcat(top_string,"running");
					break;
				case(TASK_STOPPED):
					strcat(top_string,"stopped");
					break;
				case(TASK_SUSPENDED):
					strcat(top_string,"suspended");
					break;
				case(TASK_TO_BE_DELETED):
					strcat(top_string,"zombie");
					break;
				default:
					break;

			}
			//state = process_info->state;

			draw_console_line(top_string,TEXT,TEXTTYPE);
			new_line();
		}
		process_info = process_info->ptrNext;
	}while(process_info != NULL);

	while(p_head->ptrNext!=NULL){
		process_info = p_head;
		p_head = p_head->ptrNext;
		free(process_info);
	}



	return SUCCESS;
}

int8_t kill_command(int argc, char*argv[]){
	char* str = NULL;
	const char usage[] = {"use PID as command\r\n"};
	const char help[] = {"--help"};
	int32_t pid = -666;

	if(argc > 0){
		if (strcmp(help,argv[0])== 0){
			set_color(TEXT);
			console_draw_string(usage);
			new_line();
			str = NULL;
			return ERROR;
		}else{
			console_draw_string("trying to kill process: ");
			console_draw_string(argv[0]);
			new_line();

			pid = kill_process(argv[0]);
		}
	}else{
		console_draw_string(usage);

		return ERROR;
	}
	console_draw_string("killed pid: ");
	console_draw_string(halos_itoa(pid));
	new_line();
	console_draw_string("killed argv[0]: ");
	console_draw_string(argv[0]);
	new_line();

	return SUCCESS;
}

int8_t start_command(int argc, char*argv[]){
	char* str = NULL;
	const char usage[] = {"sorry, this command does not take any parameters. glad to help you anyway.\r\n"};
	const char help[] = {"--help"};


	if(argc > 0){
		if (strcmp(help,str = argv[0])== 0){
			set_color(TEXT);
			console_draw_string(usage);
			new_line();
			str = NULL;
			return ERROR;
		}else{
			start_process(argv[0]);
		}
	}

	return SUCCESS;
}

int8_t commandClear(int argc, char*argv[]){
	char* str = NULL;
	const char usage[] = {"sorry, this command does not take any parameters. glad to help you anyway."};
	const char help[] = {"--help"};

	if(argc > 0){
		if (strcmp(help,str = argv[0])== 0){
			set_color(TEXT);
			console_draw_string(usage);
			new_line();
			str = NULL;
			return ERROR;
		}
	}
	clear_screen(0);
	clear_history();

	return SUCCESS;
}

//int8_t sendResponse1(int argc, char* argv[]){
//	int i=0;
//	int len= 0;
//	char* str;
//	const char usage[] = {"sorry, this command does not take any parameters. glad to help you anyway.\r\n"};
//	const char help[] = {"--help"};
//	const char response[] = {"start not yet implemented\r\n"};
//	x_pos += prompt_width;
//	set_x_pos(x_pos);
//
//	if(argc > 0){
//		if (strcmp(help,str = argv[0])== 0){
//			set_color(TEXT);
//			console_draw_string(usage);
//			new_line();
//			return SUCCESS;
//		}
//	}
//
//
//	draw_console_line(response,TEXT,TEXTTYPE);
//	new_line();
//	len = strlen(response);
////	for (i=0; i< len; i++){
////		write_byte(response[i]);
////	}
//
//	response = " parameter: ";
//	draw_console_line(response,TEXT,TEXTTYPE);
//	new_line();
//	len = strlen(response);
////	for (i=0; i< len; i++){
////		write_byte(response[i]);
////	}
//
//	for(i = 0; i < argc; i++){
//		str = argv[i];
//		strncat (str,halos_space, 1);
//		draw_console_line(str,TEXT,TEXTTYPE);
//		x_pos += get_string_width(str);
//		set_x_pos(x_pos);
//		len = strlen(str);
////		for (j=0; j< len; j++){
////			write_byte(str[j]);
////		}
////		write_byte('\t');
//	}
//	return SUCCESS;
//}

//int8_t sendResponse2(int argc, char* argv[]){
//	int i=0;
//	int len= 0;
//	char* str;
//	char* usage = "sorry, this command does not take any parameters. glad to help you anyway.\r\n";
//	char* help = "--help";
//	x_pos += prompt_width;
//	set_x_pos(x_pos);
//
//	if(argc > 0){
//		if (strcmp(help,str = argv[0])== 0){
//			set_color(TEXT);
//			console_draw_string(usage);
//			new_line();
//			return ERROR;
//		}
//	}
//	char* response = "top not yet implemented\r\n";
//	draw_console_line(response,TEXT,TEXTTYPE);
//	new_line();
//	len = strlen(response);
//	//	for (i=0; i< len; i++){
//	//		write_byte(response[i]);
//	//	}
//
//		response = " parameter: ";
//		draw_console_line(response,TEXT,TEXTTYPE);
//		new_line();
//		len = strlen(response);
//	//	for (i=0; i< len; i++){
//	//		write_byte(response[i]);
//	//	}
//
//		for(i = 0; i < argc; i++){
//			str = argv[i];
//			strncat (str,halos_space, 1);
//			draw_console_line(str,TEXT,TEXTTYPE);
//			x_pos += get_string_width(str);
//			set_x_pos(x_pos);
//			len = strlen(str);
//	//		for (j=0; j< len; j++){
//	//			write_byte(str[j]);
//	//		}
//	//		write_byte('\t');
//		}
//		return SUCCESS;
//}
//
//int8_t sendResponse3(int argc, char* argv[]){
//	int i=0;
//	int len= 0;
//	char* str;
//	char* usage = "sorry, this command does not take any parameters. glad to help you anyway.\r\n";
//	char* help = "--help";
//	x_pos += prompt_width;
//	set_x_pos(x_pos);
//
//	if(argc > 0){
//		if (strcmp(help,str = argv[0])== 0){
//			set_color(TEXT);
//			console_draw_string(usage);
//			new_line();
//			return SUCCESS;
//		}
//	}
//
//	char* response = "kill not yet implemented\r\n";
//	draw_console_line(response,TEXT,TEXTTYPE);
//	new_line();
//	len = strlen(response);
//	//	for (i=0; i< len; i++){
//	//		write_byte(response[i]);
//	//	}
//
//		response = " parameter: ";
//		draw_console_line(response,TEXT,TEXTTYPE);
//		new_line();
//		len = strlen(response);
//	//	for (i=0; i< len; i++){
//	//		write_byte(response[i]);
//	//	}
//
//		for(i = 0; i < argc; i++){
//			str = argv[i];
//			strncat (str,halos_space, 1);
//			draw_console_line(str,TEXT,TEXTTYPE);
//			x_pos += get_string_width(str);
//			set_x_pos(x_pos);
//			len = strlen(str);
//	//		for (j=0; j< len; j++){
//	//			write_byte(str[j]);
//	//		}
//	//		write_byte('\t');
//		}
//		return SUCCESS;
//}


/**
 * loadCommands lädt Kommandos und erstellt eine Hash-map,
 * die später benutzt wird um Konsoleneingaben einem Kommando zuzuordnen
 *
 * soll in zukunft ein ein Verzeichnis parsen, executables parsen, als kommando adden und einer ausführbaren loader-FKT zuordnen
 */
int8_t load_commands(){
	//standardkommandos initialisieren

	add_command("help",display_help,"displays this help");
	add_command("history",display_history,"shows list of previous commands");
	add_command("clear",commandClear,"clears screen and history");
	add_command("start",start_command,"starts an application");
 	add_command("top",top_command,"shows all running processes");
	add_command("kill",kill_command,"kills a process with PID");
	add_command("template",commandTemplate,"shows how to make commands");

	return SUCCESS;
}



/////////////////////////////////////////////
///										  ///
/// 	       shell functions			  ///
///										  ///
/////////////////////////////////////////////

/**
 * initialisiert Shell und die erforderlichen Devices
 * UART, GDI
 *
 * jene Devices die verwendet werden sollen müssen hier angegeben werden
 *
 */
int8_t init_shell(void){
//	device_t *serial_device;
//	DEV_UID serial_device_uid;

	gdi_open(0,0);
	select_font(CONSOLE_FONT);
	set_background_color(BACKGROUND);
	set_color(TEXT);

	screen_width = get_screen_width();
	screen_height = get_screen_height();
	prompt_width = get_string_width(halos_prompt);


	//TODO von ressource manager anfordern
	serial_device = rm_device_open(&serial_device_uid, AP7000_UART_DEVICE, 1);

	switch (serial_device_uid) {
		case DEV_NOT_FREE:
		case DEV_NOT_FOUND:
		case CHOOSE_NEXT_DEV:
			return ERROR;
	}

	//input/output verbinden
	if(bind_input(serial_device_uid) == 0);
	if(bind_output(serial_device_uid) == 0);

	load_commands();

	return SUCCESS;
}

/**
 * verbindet shell mit einenm Output-gerät
 * (UART, Bildschirm..)
 *
 */
int8_t bind_output(DEV_UID output_device){

	std_output = output_device;
	if(std_output == NULL)
		return ERROR;
	return SUCCESS;
}

/**
 * verbindet shell mit einenm Input-gerät
 * (UART, Tastatur..)
 */
int8_t bind_input(DEV_UID input_device){

	std_input = input_device;
	if(std_input == NULL)
		return ERROR;
	return SUCCESS;
}

/**
 * Zeilenumbruch und automatisches Einrücken
 */
void new_line(){

	if((get_y_pos() + get_font_height()) >= screen_height){
		scroll_down(0);
	}

	x_pos = INITX;
	y_pos += (get_font_height());

	set_x_pos(x_pos);
	set_y_pos(y_pos);

	x_pos += prompt_width;
	set_x_pos(x_pos);
	set_y_pos(y_pos);
	write_byte(CR);	write_byte(LF); //for Devices/UART
}

/**
 * Zeilenumbruch und automatisch "halos>" Prompt zeichnen
 *
 */
void new_line_prompt(){
	set_text_justify(0,0);
	x_pos = INITX;
	y_pos += (get_font_height());
	set_x_pos(x_pos);
	set_y_pos(y_pos);
	set_color(PROMPT);
	if((get_y_pos() + get_font_height()) >= screen_height){
			//TODO SCROLL FUNCTION!!!!..
			scroll_down(1);
	}else{
	console_draw_string(halos_prompt);
	}
	write_byte(CR);	write_byte(LF); //for Devices/UART

	set_color(TEXT);
	x_pos = INITX + prompt_width;
	set_x_pos(x_pos);
}

/**
 * byte von gerät einlesen
 */
int16_t read_byte(){
	return rm_device_read_byte(serial_device, std_input);
}

/**
 *  byte an gerät senden
 */
int8_t write_byte(int16_t byte_to_write ){
	//TODO Fix device_write_byte -> return value
	rm_device_write_byte(serial_device, std_input, byte_to_write);
	return SUCCESS;
}

/**
 *  byte an gerät senden
 */
int8_t write_line(char* line){
	//TODO Fix device_write_byte -> return value
	int len = 0;
	int i=0;

	len = strlen(line);
	for(i = 0; i < len; i++){
		write_byte(line[i]);
	}

	return SUCCESS;
}


/**
 * stellt fest, ob ein Kommando exisiert und
 * interpretiert gegebenfalls dieses Kommando
 *
 */
int8_t interpret_command(char* command){
	int8_t argc = 0;
	int8_t cmd_ret_val = -1;
	char* tokens;
	char* cmd;
	char** parameters= malloc(8*sizeof(char*));
	char* tmp;
	int8_t cmdlen = 0;
	//int8_t retval=-1;
	uint32_t hash;
	COMMAND_NODE* command_node = command_list_head;

	cmd = strtok (command," ");
	tokens = cmd;
	cmdlen = strlen(command);

	hash = RSHash(cmd,strlen(cmd));//hash für kommando erzeugen

    while ((tokens = strtok (NULL," ")))//parameter parsen
    {
    	parameters[argc] = tokens;
    	cmdlen += strlen(parameters[argc]);
    	cmdlen += 1; //for space
    	argc++;
    }

	if(cmdlen == MAX_COMMAND_LENGTH-1){
		set_x_pos(INITX+prompt_width);
		add_to_history(halos_except,get_x_pos(),(int8_t)ERRORTYPE);
		command_node = NULL;
		return NO_SUCH_COMMAND;
	}

    command_node = find_command(hash);

    //TODO
    //new_line();

	if(command_node != NULL){
		set_x_pos(INITX+prompt_width);
		set_color(COMMAND);
		console_draw_string(command);
		tmp = malloc(strlen(command)*sizeof(char));
		strcpy(tmp,command);

		add_to_history(tmp,get_x_pos(),COMMANDTYPE);
		set_color(TEXT);
		new_line();
		cmd_ret_val =command_node->execute(argc,parameters);
		if(cmd_ret_val < 0){
			return cmd_ret_val;
		}
	}else{
		tmp = malloc(strlen(command)*sizeof(char));
		strcpy(tmp,command);
		add_to_history(tmp,get_x_pos(),(int8_t)UNKNOWNCOMMAND);

		command_node = NULL;
		return NO_SUCH_COMMAND;
	}

	return SUCCESS;
}

/**
 * Bildschirm löschen
 */
void clear_screen(int8_t draw_prompt){
	//clr_scr(0);
	set_color(BACKGROUND);
	fill_rectangle(0,0,screen_width,screen_height);
	set_color(TEXT);

	x_pos = INITX;
	y_pos = INITY;

	//TODO evtl bug in :
	//goto_xy(x_pos,y_pos);
	set_x_pos(x_pos);
	set_y_pos(y_pos);

	if(draw_prompt> 0){
		set_color(PROMPT);
		console_draw_string(halos_prompt);
		set_color(TEXT);
	}

	x_pos += prompt_width;
	set_x_pos(x_pos);
}

/**
 * Bildschirm scrollen
 * NOT YET IMPLEMENTED
 * ruft nur clearScreen auf
 */
void scroll_down(int8_t draw_prompt){
	clear_screen(draw_prompt);
	//vertical_pan(-1*(get_font_height()*3));
	//	set_color(BACKGROUND);
//	fill_rectangle(0,0,screen_width,screen_height);
//	set_color(TEXT);
	//clear_screen(int8_t draw_prompt);
}

void console_draw_string(const char* line){
	int i = 0;
	int str_len=0;

	set_text_justify(0,0);

	if((get_y_pos() + get_font_height()) >= screen_height){
		//clear_screen()
		set_y_pos(get_y_pos()+get_screen_height()-get_font_height());
		//TODO SCROLL FUNCTION!!!!..
		scroll_down(0);
	}

	if((get_x_pos()+ get_string_width(line)) >= screen_width){
		str_len = strlen(line);
		for(i = 0; i < str_len; i++){
			while((get_x_pos() < screen_width)&& i<str_len){
				draw_char(line[i]);
				i++;
			}
			new_line();
		}
	}else{
		draw_string(line);
	}

	write_line(line);
}

/**
 * History anzeigen
 */
int8_t display_history(int argc, char* argv[]){
	CONSOLE_LINE* tmp = NULL;
	int8_t command_mode = 0;

	if((argc > 0)&&(strcmp("-a",argv[0])>0)){
		command_mode = 1;
	}

	//reset screen
	set_color(BACKGROUND);
	fill_rectangle(0,0,screen_width,screen_height);
	set_color(TEXT);

	x_pos = INITX;
	y_pos = INITY;

	set_x_pos(x_pos);
	set_y_pos(y_pos);

	//y_pos += (get_font_height());
	tmp = history_first;

	while(tmp!=NULL && tmp != history_last){
		//if line was a command, draw prompt
		if(tmp->type == COMMANDTYPE){
			x_pos = INITX;

			set_x_pos(x_pos);
			//set_y_pos(y_pos);
			set_color(HISTORY);
			draw_string(halos_prompt);
			//x_pos += prompt_width;
			//set_x_pos(x_pos);
			set_color(COMMAND);
		}else if(tmp->type == (int8_t)UNKNOWNCOMMAND){
			x_pos = INITX;
			set_x_pos(x_pos);
			set_color(HISTORY);
			draw_string(halos_prompt);
			tmp->x_offset= (tmp->x_offset = INITX+prompt_width);
			set_color(UNKNOWNCOMMAND);
		}else if(tmp->type == (int8_t)ERRORTYPE){
			x_pos = INITX;
			set_x_pos(x_pos);
			set_color(HISTORY);
			draw_string(halos_prompt);
			tmp->x_offset= (tmp->x_offset = INITX+prompt_width);
			set_color(ERRORTYPE);
		}else{
			set_color(HISTORY);
		}

		if (command_mode == 1){
					continue;
		}

		//before reaching last line of console, prompt for return, then clear screen
		if((get_y_pos() + (get_font_height()*4)) >= screen_height){
			console_draw_string("Press Enter to Continue..");
			write_byte(CR);	write_byte(LF); //for Devices/UART

			while(read_byte()!= CR){ }
			//scroll_down(0);//or clear screen if preferable
			clear_screen(0);
		}
		//set x pos to the offset set in the history, to indent the line accurately
		set_x_pos(tmp->x_offset);

		if(get_x_pos() >= screen_width){
			//reached only under obscure circumstances... font changes, manual changes of X_pos
			new_line();
		}

		console_draw_string(tmp->line);

		new_line();
		tmp = tmp->next_line;
	}

	return SUCCESS;
}

/**
 * not yet implemented
 */
void cursorBlink(){
	if(drawCursor == 1){
		//set_color(TEXT);
		draw_console_line("_",TEXT,0);
		//set_color(BACKGROUND);
	}else{
		//set_color(BACKGROUND);
		draw_console_line("_",BACKGROUND,0);
		//set_color(TEXT);
	}
}

/**
 * schreibt eine fehlermeldung
 * !!!fügt die fehlermeldung in history ein!!!
 * wird über alle schnittstellen gesendet
 */
void drawError(char* error_msg){
	x_pos = INITX;
	x_pos += prompt_width;
	set_x_pos(x_pos);
	draw_console_line(error_msg,ERROR,ERRORTYPE);
	write_line(error_msg);
}

/**
 * schreibt einen string in die konsole, automatischer "Zeilenumbruch" bei strings
 * die über displaygröße hinausgehen
 *
 * !!!fügt kommandos in die "history" ein!!!!
 * wird über alle schnittstellen gesendet
 *
 */
void draw_console_line(const char* msg, int32_t color, int8_t type){
	set_text_justify(0,0);
	set_color(color);

	if((get_y_pos() + get_font_height()) >= screen_height){
		if(type == COMMANDTYPE){
			//draw_string(msg);
			scroll_down(1);
		}else{
			scroll_down(0);
		}
	}

//	if(get_x_pos() >= screen_width){
//		new_line();
//	}

//	if((get_x_pos()+ get_string_width(msg)) >= screen_width){
//		add_to_history(msg,get_x_pos(),type);
//		str_len = strlen(msg);
//		for(i = 0; i < str_len; i++){
//			while((get_x_pos() < screen_width)){
//				draw_char(msg[i]);
//				i++;
//			}
//			new_line();
//		}
//	}else{
		add_to_history(msg,get_x_pos(),type);
		console_draw_string(msg);
		set_x_pos(get_string_width(msg));
//	}
}

//TODO rename clear_string
/**
 * letztes auf screen geschriebenes Zeichen löschen
 */
void clear_string(char* c){

	int16_t width = 0;
	x_pos = get_x_pos();
	width = get_string_width(c);
	set_x_pos(x_pos-width);

	set_color(BACKGROUND);
	draw_string(c);
	x_pos = get_x_pos();
	//y_pos = get_y_pos();
	set_color(TEXT);

	//überzeichnen
	//cursor blinken
}

/**
 * liest Zeichen ein bis '\n'
 * gibt pointer auf diese Zeile zurück
 *
 */
char* read_line(){
	//char line[MAX_COMMAND_LENGTH];
	memset(line, '\0', sizeof(line) );
	int16_t pos = 0;
	int8_t tmp = -1;

	do {
		while (tmp == -1){
			tmp = (char)read_byte();
		}

		if(tmp!=BACKSPACE ){
			line[pos++]=tmp;
			x_pos = get_x_pos();
			if(x_pos >= screen_width){
				new_line();
			}
			draw_char((char)tmp);
		}else{
			if(pos >0){
				clear_string(&line[pos-1]);
				line[pos-1] = '\0';
				pos--;
			}
		}
		tmp = -1;
	}while(line[pos-1]!=CR && pos < MAX_COMMAND_LENGTH) ;

	if(pos>1){
		line[pos-1] = '\0';
	}else{
		line[pos] = '\0';
	}

	return line;
}

/**
 * not yet implemented
 */
void display_available_commands(){


}

/**
 * Message Of The Day anzeigen
 */
void motd(){
	int i = 0;
	uint64_t seed = 0;

	seed = get_kernel_time();
	srand ( (unsigned int)seed );

	i = rand() % 10 + 1;
	set_color(BLUE);
	if(i >= 7){
		console_draw_string("viel Spass mit HalOS..");
	}else if( i >= 4){
		console_draw_string("heute schon HalOS gesehen?");
	}else if( i < 4){
		console_draw_string("ein Tag ohne HalOS ist ein verlorener Tag..");
	}
	y_pos = get_y_pos()+ get_font_height();
	set_y_pos(y_pos);
	y_pos = y_pos*2;
	set_y_pos(y_pos);
	display_available_commands();
}

//void shell_process(void){
int main(void) {
	int err = 0;
	char* info = "type \"help\" for list of commands\r\n";

	//device_init();
	init_shell();
    // int tmp;

	write_byte(CR);	write_byte(LF); //for Devices/UART

	x_pos = INITX;
	y_pos = INITY;

	//goto_xy(x_pos,y_pos);
	set_x_pos(x_pos);
	set_y_pos(y_pos);
	motd();
	write_byte(CR);	write_byte(LF); //for Devices/UART
	set_color(TEXT);
	console_draw_string(info);
	y_pos = get_y_pos()+ get_font_height();
	set_y_pos(y_pos);
	set_color(PROMPT);
	console_draw_string(halos_prompt);
	set_color(TEXT);
	last_color = TEXT;

	x_pos += prompt_width;
	set_x_pos(x_pos);

	//set_y_pos(get_y_pos()+ get_font_height());

    while (1) {
		if((err = interpret_command(read_line())) < 0){
			switch(err){
				case NO_SUCH_COMMAND:
					x_pos += prompt_width;
					set_x_pos(x_pos);
					new_line();
					drawError("unbekanntes Kommando\r\n");
					new_line_prompt();
					break;
			}
		}else{
			//draw_console_line("Kommando erfolgreich ausgeführt.");
			new_line_prompt();
		}
	}
    return 0;
}

/**
 * erzeugt Hash-Wert für einen String
 * sdbm-hash
 */
uint32_t RSHash(char* str, uint16_t len)
{
	uint32_t hash = 0;
	//uint32_t c	=	0;
    uint32_t i    = 0;

    for(i = 0; i < len; str++, i++){
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

COMMAND_NODE* find_command(int hash){
	COMMAND_NODE* command = command_list_head;
	while (command != NULL){
		if(hash == command->command_hash){
			return command;
		}
		command = command->nextCommand;
	}
	return NULL;
}

/*
 * return -1 if cmd was null
 * return -2 if command already defined
 */
int8_t add_command(char* cmd, int8_t (*execute)(int,char*[]),char* description){
	COMMAND_NODE* new_command;
	uint32_t hash =0;
	if(cmd == NULL){
		return ERROR;
	}
	hash = RSHash(cmd,strlen(cmd));

	if(find_command(hash)!=NULL){
		return COMMAND_ALREADY_DEFINED;
	}

	new_command = (COMMAND_NODE*)malloc(sizeof(COMMAND_NODE)); //allocate space for node
	new_command->command = cmd;
	new_command->command_hash = hash;
	new_command->short_description = description;

	new_command->execute = (void*)(*execute);
	new_command->nextCommand = NULL;

	if(command_list_head == NULL){
		command_list_head = new_command;
		command_list_last = new_command;
	}else{
		command_list_last->nextCommand = new_command;
		command_list_last = new_command;
	}

	return SUCCESS;
}

/**
 * Kommando löschen
 * NOT YET IMPLEMENTED
 */
int8_t remove_command(char* cmd){
	//hash erstellen
	//kommando holen, löschen

	//für alle kommandos durchführen
	return ERROR;
}

void add_to_history(char* line, int16_t x_pos,int8_t type){
//	CONSOLE_LINE* tmp = NULL;
//
//	if(history_first == NULL){
//		history_first =  (CONSOLE_LINE*)malloc(sizeof(CONSOLE_LINE));
//		history_first->line = line;
//		history_first->x_offset = x_pos;
//		history_first->type = type;
//		history_first->prev_line = NULL;
//		history_first->next_line = NULL;
//		history_last = history_first;
//
//		history_items++;
//		//return;
//	}else{
//		tmp = history_first;
//
//		if(history_items >= HISTORY_SIZE){
//			history_first = history_first->next_line;
//			free( history_first->prev_line);
//			history_first->prev_line = NULL;
//			history_items--;
//		}
//		history_last->next_line = malloc(sizeof(CONSOLE_LINE));
//		history_last->next_line->prev_line = history_last;
//		history_last = history_last->next_line;
//		history_last->type = type;
//		history_last->line = line;
//		history_last->next_line = NULL;
//		history_last->x_offset = x_pos;
//		history_items++;
//	}
////
////	tmp->next_line = malloc(sizeof(CONSOLE_LINE));
////	tmp->next_line->prev_line = tmp;
////	tmp->next_line->next_line = NULL;
////	tmp->next_line->prompt = is_command;
////	strcpy(tmp->next_line->line,line);
////	tmp->next_line->x_offset = x_pos;
////	history_last = tmp->next_line;
}

void clear_history(){
//	CONSOLE_LINE* tmp = NULL;
//	//CONSOLE_LINE* destr = NULL;
//	tmp = history_last;
//
//	if((history_first == NULL)&&(history_last == NULL)){
//		return;
//	}
//	if(history_first == history_last){
//		history_items = 0;
//		free(history_first);
//		history_first = NULL;
//		history_last = NULL;
//	}
//
//	while(tmp!=NULL){
//		tmp = tmp->prev_line;
//		if(tmp == NULL)
//			continue;
//		free(tmp->next_line);
//		tmp->next_line = NULL;
//		history_items--;
//	}
//
//	free(history_first);
//	history_items = 0;
//	history_first = NULL;
//	history_last = NULL;

}

//TODO DESTRUCT?

