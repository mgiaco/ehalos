/**
 * @file hgdi_api.c
 * @brief Halos GDI API implementation
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */


#include "hgdi_api.h"

void rm_gdi_open(GDI_UID *gdi_uid, int32_t gdi_type) {
	system_call4(SCALL_RM_GDI_OPEN,gdi_type, 0, 0, gdi_uid);
}

void gdi_open(GDI_UID *gdi_uid, int32_t gdi_type) {
	system_call4(SCALL_GDI_OPEN,gdi_type, 0, 0, gdi_uid);
}

void goto_xy(uint16_t x, uint16_t y) {
	system_call2(SCALL_GDI_GOTOXY,x, y);
}

void set_x_pos(uint16_t x) {
	system_call1(SCALL_GDI_SET_X,x);
}

uint16_t get_x_pos(void) {
	uint16_t x;
	system_call0(SCALL_GDI_GET_X,&x);
	return x;
}

void set_y_pos(uint16_t y) {
	system_call1(SCALL_GDI_SET_Y,y);
}

uint16_t get_y_pos(void) {
	uint16_t y;
	system_call0(SCALL_GDI_GET_Y,&y);
	return y;
}

uint16_t get_screen_width(){
	uint16_t width;
	system_call0(SCALL_GDI_GET_SCREENWIDTH,&width);
	return width;
}
uint16_t get_screen_height(){
	uint16_t height;
	system_call0(SCALL_GDI_GET_SCREENHEIGHT,&height);
	return height;
}


void clr_scr(uint8_t bufferAndDisplay) {
	system_call1(SCALL_GDI_CLR_SCR,bufferAndDisplay);
}

void set_color(color_t color) {
	system_call1(SCALL_GDI_SET_COLOR,color);
}

void set_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t color = b;
	color |= g << 8;
	color |= r << 16;
	system_call1(SCALL_GDI_SET_COLOR,color);
}

// returns current color
color_t get_color() {
	uint32_t color;
	system_call0(SCALL_GDI_GET_COLOR,&color);
	return color;
}
void set_background_color(color_t color) {
	system_call1(SCALL_GDI_SET_BG_COLOR,color);
}

void set_background_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t color = b;
	color |= g << 8;
	color |= r << 16;
	system_call1(SCALL_GDI_SET_BG_COLOR,color);
}

// returns current background color
color_t get_background_color_rgb() {
	uint32_t color;
	system_call0(SCALL_GDI_GET_BG_COLOR,&color);
	return color;
}


//// TODO: not needed yet
//void set_font_color(uint32_t color) {
//	system_call1(SCALL_GDI_SET_FONT_COLOR,color);
//}
//
//// TODO: not needed yet
//void set_font_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
//	uint32_t color = b;
//	color |= g << 8;
//	color |= r << 16;
//	system_call1(SCALL_GDI_SET_FONT_COLOR,color);
//}

void put_pixel(uint16_t x1, uint16_t y1) {
	system_call2(SCALL_GDI_PUT_PIXEL,x1, y1);
}

void clr_pixel(uint16_t x1, uint16_t y1) {
	system_call2(SCALL_GDI_CLR_PIXEL,x1, y1);
}

void vertical_pan(int16_t yoffset) {
	system_call1(SCALL_GDI_VPAN, yoffset);
}
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	system_call4(SCALL_GDI_DRAW_LINE,x1, y1, x2, &y2);
}

void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	system_call4(SCALL_GDI_DRAW_RECTANGLE,x1, y1, x2, &y2);
}

void draw_circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius) {
	system_call4(SCALL_GDI_DRAW_CIRCLE,xCenter, yCenter, radius, 0);
}

void draw_sprite(uint8_t* data, uint16_t height, uint16_t width) {
	system_call4(SCALL_GDI_DRAW_SPRITE,height, width, 0, data);
}

void draw_sprite2(gdi_sprite_t* sprite){
	system_call4(SCALL_GDI_DRAW_SPRITE_2, 0, 0, 0, sprite);
}

void draw_color_sprite(gdi_sprite_t* sprite){
	system_call4(SCALL_GDI_DRAW_COLOR_SPRITE, 0, 0, 0, sprite);
}

void fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	system_call4(SCALL_GDI_FILL_RECTANGLE,x1, y1, x2, &y2);
}

/*TODO*/
void select_font(halos_font_t fonts) {
	system_call1(SCALL_GDI_FONT_SELECT,fonts);
}

uint8_t draw_char(const char c) {
	uint8_t ret;
	system_call4(SCALL_GDI_FONT_PUTCHAR,c, 0, 0, &ret);
	return ret;
}
void draw_string(const char *str) {
	system_call4(SCALL_GDI_FONT_PUTSTRING,0, 0, 0, (void *) str);
}

void set_text_justify(uint8_t horiz, uint8_t vert) {
	system_call2(SCALL_GDI_FONT_SETTEXTJUSTIFY,horiz, vert);
}

uint8_t get_char_width(char c) {
	uint8_t ret;
	system_call4(SCALL_GDI_FONT_GETCHARWIDTH,c, 0, 0, &ret);
	return ret;
}

uint16_t get_string_width(const char *str) {
	uint16_t ret;
	//TODO: void* for str
	system_call4(SCALL_GDI_FONT_GETSTRINGWIDTH,(char*)str,0,0, &ret);
	return ret;
}

uint8_t get_font_height() {
	uint8_t ret;
	system_call0(SCALL_GDI_FONT_GETFONTHEIGHT, &ret);
	return ret;
}

void set_line_thickness(uint8_t thickness) {
	system_call1(SCALL_GDI_SET_LINETHICKNESS,thickness);
}

void set_line_style(line_style_t linestyle) {
	system_call1(SCALL_GDI_SET_LINESTYLE,linestyle);
}
