#ifndef SHELL_H_
#define SHELL_H_

#include <avr32/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>



#include "hsystem_api.h"
#include "hdevice_api.h"
#include "device_types.h"
#include "hgdi_api.h"


#define MAX_COMMAND_LENGTH  30
#define HISTORY_SIZE 25

//#define IS_COMMAND 1

/*ERROR CODES*/
#define SUCCESS  0
#define ERROR  -1
#define COMMAND_ALREADY_DEFINED  -2
#define NO_SUCH_COMMAND -77

/*types*/
#define COMMANDTYPE -5
#define ERRORTYPE -6
#define TEXTTYPE	-7

/*ASCII*/
#define CR 13
#define LF 10
#define BACKSPACE 8

/*graphics*/
#define CONSOLE_FONT   ARIAL_BOLD_14
#define VSPACE 4
#define HSPACE 4
#define INITX 10
#define INITY 10

#define BACKGROUND BLACK
#define HISTORY GRAY
#define TEXT GREEN
#define PROMPT YELLOW
#define UNKNOWNCOMMAND RED
#define COMMAND BLUE


 typedef struct command_node{
	uint32_t command_hash;
 	char* command;
 	char* parameters;
 	char* short_description;
 	int8_t (*execute)(int8_t,char*[]); //äquivalent zu execute(int argc, char*argv[],int8_t retparm)
 	struct command_node* nextCommand;
 }COMMAND_NODE;

 typedef struct console_line{
	int16_t x_offset;
	int8_t type;
 	char* line;
 	struct console_line* prev_line;
 	struct console_line* next_line;
 }CONSOLE_LINE;

 int8_t init_shell(void);

 int8_t bind_output(DEV_UID output_device);

 int8_t bind_input(DEV_UID input_device);

 int16_t read_byte();

 int8_t write_byte(int16_t byte_to_write );

 int8_t write_line(char* line);

 uint32_t RSHash(char* str, uint16_t len);

//void shell_process(void);

 int8_t load_commands();

 int8_t add_command(char* cmd, int8_t (*execute)(int,char*[]),char* description);

COMMAND_NODE* find_command(int hash);

int8_t display_help(int argc, char* argv[]);

/*screen functions*/

void new_line();

void new_line_prompt();

void console_draw_string(const char* line);

void draw_console_line(const char* msg, int32_t color, int8_t type);

void clear_screen(int8_t is_command);

void scroll_down(int8_t is_command);

void new_line();

void cursorBlink();

void clear_string(char* c);

void add_to_history(char* line, int16_t x_pos,int8_t type);

void clear_history();

int8_t display_history(int argc, char* argv[]);
#endif /*SHELL_H_*/
