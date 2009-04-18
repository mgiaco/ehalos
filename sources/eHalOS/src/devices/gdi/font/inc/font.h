#ifndef FONT_H_
#define FONT_H_

/**
 * @file font.h
 * @brief device independet graphic/font header
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

/**
 *  @defgroup DEVICE  Halos Devices
 */

/**
 *  @defgroup GDI_DEVICE Halos GDI
 *  @ingroup DEVICE
 */

/**
 *  @defgroup FONTS Halos Font
 *  @ingroup GDI_DEVICE
 *  @brief Font drawing functions and Font definitions for Halos GDI Device
 */

/**
 *  @defgroup graphic_device_font Font
 *  @ingroup FONTS
 *  @brief Font functions for Graphic device
 *
 *  @note ...
 *  @author momo, giaco
 *  @version 0.0.1
 */

/**@{*/

#include <inttypes.h>

#include "ap7000_lcd.h"

#include "graphics.h"
#include "graphis_hw_interface.h"

#include "colors.h"
#include "fonts.h"

#include "handel_bold_14.h"
#include "arial_bold_14.h"

/**
 *  @name Constants
 */

//font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

//text settings
#define LEFT_TEXT       0			/**< left align text */
#define CENTER_TEXT     1			/**< center text */
#define RIGHT_TEXT      2			/**< right align text */
#define BOTTOM_TEXT     0			/**< bottom align text */
#define TOP_TEXT        2			/**< top align text */


/**
 *  @name Callback
 */

typedef uint8_t (*FontReadCallback)(const uint8_t *); /**< callback for FontRead */


/**
 *  @name Functions
 */

/**
 * @brief initialize font
 * @param DC DeviceContext
 */
extern void init_font(DeviceContext_t *DC); //Init Font

/**
 * @brief read font data
 * @param ptr pointer on font data
 * @return font data
 */
extern uint8_t read_font_data(const uint8_t *ptr); //Standard Read Callback

/**
 * @brief select font, called from GDI-API
 * @param fonts HalosFont to select
 * @param callback FontReadCallback
 * @param color color
 */
extern void select_new_font(halos_font_t fonts, FontReadCallback callback, uint32_t color);

/**
 * @brief draw character, called from GDI-API
 * @param c character to draw
 */
extern uint8_t glcd_draw_char(const char c);

/**
 * draw string, called from GDI-API
 * @param str pointer to string to draw
 */
extern void glcd_draw_string(const char *str);

/**
 * @brief set text alignment, called from GDI-API
 * @param horiz	horizontal alignment
 * @param vert	vertical alignment
 */
extern void glcd_set_text_justify(uint8_t horiz, uint8_t vert);

/**
 * @brief get character width, called from GDI-API
 * @param c character
 * @return width of character
 */
extern uint8_t glcd_get_char_width(char c);

/**
 * @brief get string width, called from GDI-API
 * @param str pointer to string
 * @return width of string
 */
extern uint16_t glcd_get_string_width(const char *str);

/**
 * @brief get font height, called from GDI-API
 * @return height of current font
 */
extern uint8_t glcd_get_font_height();
/**@}*/
#endif

