#ifndef HGDI_API_H_
#define HGDI_API_H_
/**
 * @file hgdi_api.h
 * @brief Halos GDI API header
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 */

/**
 *  @defgroup API  Halos API's
 */

/**
 *  @defgroup GDI Halos GDI
 *  @ingroup API
 */

/**
 *  @defgroup hgdi_api	GDI API
 *  @ingroup GDI
 *  @brief API for GDI
 *
 *  @note ...
 *  @author momo
 *  @version 0.0.1
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#include "colors.h"
#include "fonts.h"

#include "hsystem_api.h"
#include "hsystem_call_table.h"
#include "gdi_types.h"

/**
 * @brief unique identifier for GDI
 * @typedef GDI_UID
 */
typedef int GDI_UID;

/**
* @brief open gdi device
* calls initalization of hardware dependent display
*/
extern void gdi_open(GDI_UID *gdi_uid, int32_t gdi_type);

/**
 * @brief set new cursor position
 * @param x x-coordinate of new cursor position
 * @param y y-coordinate of new cursor position
 */
extern void goto_xy(uint16_t x, uint16_t y);

/**
 * @brief set new x-coordinate for cursor
 * @param x x-coordinate of new cursor position
 */
extern void set_x_pos(uint16_t x);

/**
 * @brief get x-coordinate of cursor
 * @return x-coordinate of cursor position
 */
extern uint16_t get_x_pos(void);

/**
 * @brief set new y-coordinate for cursor
 * @param y y-coordinate of new cursor position
 */
extern void set_y_pos(uint16_t y);


/**
 * @brief get y-coordinate of cursor
 * @return y-coordinate of cursor position
 */
extern uint16_t get_y_pos(void);

/**
 * @brief get current screenwidth
 * @return current screenwidth
 */
extern uint16_t get_screen_width();

/**
 * @brief get current screenheight
 * @return current screenheight
 */
extern uint16_t get_screen_height();

extern void clr_scr(uint8_t bufferAndDisplay);

/**
 * @brief set new color
 * @param color new color to be set
 */
extern void set_color(color_t color);

/**
 * @brief set new color in RGB-style
 * @param r red pigment content of new color
 * @param g green pigment content of new color
 * @param b blue pigment content  of new color
 */
extern void set_color_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief get current color
 * @return color currently set
 */
extern color_t get_color();

/**
 * @brief set new backgroundcolor
 * @param color new backgroundcolor to be set
 */
extern void set_background_color(color_t color);

/**
 * @brief set new backgroundcolor in RGB-style
 * @param r red pigment content of new backgroundcolor
 * @param g green pigment content of new backgroundcolor
 * @param b blue pigment content  of new backgroundcolor
 */
extern void set_background_color_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief get current backgroundcolor
 * @return backgroundcolor currently set
 */
extern color_t get_background_color_rgb();

//extern void set_font_color(uint32_t color);
//extern void set_font_color_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief draw pixel with current color at given position
 * @param x1 x-coordinate of pixel
 * @param y1 y-coordinate of pixel
 */
extern void put_pixel(uint16_t x1, uint16_t y1);

/**
 * @brief draw pixel with current backgroundcolor at given position
 * @param x1 x-coordinate of pixel
 * @param y1 y-coordinate of pixel
 */
extern void clr_pixel(uint16_t x1, uint16_t y1);

/**
 * @brief scroll framebuffer up or down
 * @param yoffset value for framebuffer scrolling
 */
extern void vertical_pan(int16_t yoffset);

/**
 * @brief draws a line
 * @param x1 x-coordinate of startpoint
 * @param y1 y-coordinate of startpoint
 * @param x2 x-coordinate of endpoint
 * @param y2 y-coordinate of endpoint
 */
extern void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief draws a rectangle
 * @param x1 x-coordinate of first corner
 * @param y1 y-coordinate of first corner
 * @param x2 x-coordinate of second corner
 * @param y2 y-coordinate of second corner
 */
extern void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief draws a circle
 * @param xCenter x-coordinate of circle center
 * @param yCenter y-coordinate of circle center
 * @param radius radius of circle
 */
extern void draw_circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius);

/**
 * @brief draws a sprite
 * @param data sprite value array
 * @param height height of sprite
 * @param width width of sprite
 */
extern void draw_sprite(uint8_t* data, uint16_t height, uint16_t width);

/**
 * @brief draws a sprite
 * @param sprite sprite-structure
 */
extern void draw_sprite2(gdi_sprite_t* sprite);

/**
 * @brief draws a 24-bit colored sprite
 * @param sprite sprite-structure
 */
extern void draw_color_sprite(gdi_sprite_t* sprite);

/**
 * @brief draws a filled rectangle
 * @param x1 x-coordinate of first corner
 * @param y1 y-coordinate of first corner
 * @param x2 x-coordinate of second corner
 * @param y2 y-coordinate of second corner
 */
extern void fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief select font
 * @param fonts font to choose
 */
extern void select_font(halos_font_t fonts);

/**
 * @brief draws a character
 * @param c character
 */
extern uint8_t draw_char(const char c);

/**
 * @brief draws a string
 * @param str pointer to a string
 */
extern void draw_string(const char *str);

/**
 * @brief change alignment of text
 * @param horiz horizontal
 * @param vert vertical
 */
extern void set_text_justify(uint8_t horiz, uint8_t vert);

/**
 * @brief get width of a single char
 * @param c character
 * @return width of character
 */
extern uint8_t get_char_width(char c);

/**
 * @brief get width of a given string
 * @param str pointer to a string
 * @return width of string
 */
extern uint16_t get_string_width(const char *str);

/**
 * @brief get font height of current font
 * @return fontheight
 */
extern uint8_t get_font_height();

/**
 * @brief set line thickness
 * @param thickness value for line thickness
 */
extern void set_line_thickness(uint8_t thickness);

/**
 * @brief set linestyle
 * @param linestyle value for new linestyle
 */
extern void set_line_style(line_style_t linestyle);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/

#endif /*HGDI_API_H_*/
