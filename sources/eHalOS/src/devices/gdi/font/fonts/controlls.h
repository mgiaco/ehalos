
/*
 *
 * controlls
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : controlls.h
 * Date                : 06.05.2007
 * Font size in bytes  : 349
 * Font width          : 8
 * Font height         : 8
 * Font first char     : 48
 * Font last char      : 55
 * Font used chars     : 7
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>

#ifndef CONTROLLS_H
#define CONTROLLS_H

#define CONTROLLS_WIDTH 8
#define CONTROLLS_HEIGHT 8

static const uint8_t Controlls[] = { 0x01, 0x5D, // size
		0x08, // width
		0x08, // height
		0x30, // first char
		0x07, // char count

		// char widths
		0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,

		// font data
		0x0C, 0x06, 0xFF, 0xFF, 0x06, 0x0C, // 48
		0x30, 0x60, 0xFF, 0xFF, 0x60, 0x30, // 49
		0xFE, 0x82, 0x83, 0x83, 0x82, 0xFE, // 50
		0x00, 0x78, 0x78, 0x78, 0x78, 0x00, // 51
		0x00, 0x70, 0x70, 0x70, 0x70, 0x00, // 52
		0x00, 0x60, 0x60, 0x60, 0x60, 0x00, // 53
		0x00, 0x40, 0x40, 0x40, 0x40, 0x00 // 54

		};

#endif
