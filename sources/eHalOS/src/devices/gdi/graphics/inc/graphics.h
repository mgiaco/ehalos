#ifndef _GRAPHICS_H
#define _GRAPHICS_H

/**
 * @file graphics.h
 * @brief device independent graphic code
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
 *  @defgroup graphic_device Graphic Device
 *  @ingroup GDI_DEVICE
 *  @brief device independent implementation of drawing functions. functions are called by GDI API.
 *  Hardware dependent code is called through the graphics hardware interface
 *
 *  @note ...
 *  @author momo, giaco
 *  @version 0.0.1
 */

/**@{*/

#include <stdint.h>
#include "gdi_types.h"


/**
 * @brief initalize graphic
 */
extern void initGraph(void);

/**
 * @brief switch current device
 * @param DeviceNo number of device
 */
extern void setDevice(char DeviceNo);

/**
 * @brief set new cursor position, called from GDI-API
 * @param x x-coordinate of new cursor position
 * @param y y-coordinate of new cursor position
 */
extern void gotoXY(uint16_t x, uint16_t y);

/**
 * @brief set new x-coordinate for cursor, called from GDI-API
 * @param x x-coordinate of new cursor position
 */
extern void setX(uint16_t x);

/**
 * @brief get x-coordinate of cursor, called from GDI-API
 * @return x-coordinate of cursor position
 */
extern uint16_t getX(void);

/**
 * @brief set new y-coordinate for cursor, called from GDI-API
 * @param y y-coordinate of new cursor position
 */
extern void setY(uint16_t y);

/**
 * @brief get y-coordinate of cursor, called from GDI-API
 * @return y-coordinate of cursor position
 */
extern uint16_t getY(void);

/**
 * @brief get current screenwidth, called from GDI-API
 * @return current screenwidth
 */
extern uint16_t getScreenWidth();

/**
 * @brief get current screenheight, called from GDI-API
 * @return current screenheight
 */
extern uint16_t getScreenHeight();

extern void clrScr(uint8_t bufferAndDisplay);

/**
 * @brief set new color, called from GDI-API
 * @param Color new color to be set
 */
extern void setColor(uint32_t Color);

/**
 * @brief get current color, called from GDI-API
 * @return color currently set
 */
extern uint32_t getColor();

/**
 * @brief set new backgroundcolor, called from GDI-API
 * @param color new backgroundcolor to be set
 */
extern void setBackgroundColor(uint32_t color);

/**
 * @brief get current backgroundcolor, called from GDI-API
 * @return backgroundcolor currently set
 */
extern uint32_t getBackgroundColor();

/**
 * @brief set font color, called from GDI-API
 * @param color new fontcolor
 */
extern void setFontColor(uint32_t color);
extern void update(uint16_t top, uint16_t bottom);
extern void updateAll(void);

/**
 * @brief scroll framebuffer up or down, called from GDI-API
 * @param yoffset value for framebuffer scrolling
 */
extern void verticalPan(int16_t yoffset);

/**
 * @brief draw pixel with current color at given position, called from GDI-API
 * @param x1 x-coordinate of pixel
 * @param y1 y-coordinate of pixel
 */
extern void putPixel(uint16_t x1, uint16_t y1);

/**
 * @brief draw pixel with current backgroundcolor at given position, called from GDI-API
 * @param x1 x-coordinate of pixel
 * @param y1 y-coordinate of pixel
 */
extern void clrPixel(uint16_t x1, uint16_t y1);

/**
 * @brief draws a line, called from GDI-API
 * @param x1 x-coordinate of startpoint
 * @param y1 y-coordinate of startpoint
 * @param x2 x-coordinate of endpoint
 * @param y2 y-coordinate of endpoint
 */
extern void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief draws a rectangle, called from GDI-API
 * @param x1 x-coordinate of first corner
 * @param y1 y-coordinate of first corner
 * @param x2 x-coordinate of second corner
 * @param y2 y-coordinate of second corner
 */
extern void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief draws a filled rectangle, called from GDI-API
 * @param x1 x-coordinate of first corner
 * @param y1 y-coordinate of first corner
 * @param x2 x-coordinate of second corner
 * @param y2 y-coordinate of second corner
 */
extern void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief draws a circle, called from GDI-API
 * @param xCenter x-coordinate of circle center
 * @param yCenter y-coordinate of circle center
 * @param radius radius of circle
 */
extern void circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius);

/**
 * @brief draws a box, called from GDI-API
 * @param x x-coordinate of first corner
 * @param y y-coordinate of first corner
 * @param width width of box
 * @param height height of box
 */
extern void box(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief draws a sprite, called from GDI-API
 * @param data sprite value array
 * @param height height of sprite
 * @param width width of sprite
 */
extern void sprite(uint8_t* data, uint16_t height, uint16_t width);

/**
 * @brief draws a sprite, called from GDI-API
 * @param sprite sprite-structure
 */
extern void sprite2(gdi_sprite_t* sprite);

/**
 * @brief draws a 24-bit colored sprite, called from GDI-API
 * @param sprite sprite-structure
 */
extern void color_sprite(gdi_sprite_t* sprite);

/**
 * @brief set line thickness, called from GDI-API
 * @param thickness value for line thickness
 */
extern void setLineThickness(uint8_t thickness);

/**
 * @brief set linestyle, called from GDI-API
 * @param linestyle value for new linestyle
 */
extern void setLineStyle(line_style_t linestyle);

/**@}*/

#endif
