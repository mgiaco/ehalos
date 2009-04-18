/**
 * @file colors24.h
 * @brief 24-Bit Colortable
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */
#ifndef COLORS24_H_
#define COLORS24_H_

/**
 *  @defgroup API  Halos API's
 */

/**
 *  @defgroup GDI Halos GDI
 *  @ingroup API
 */

/**
 *  @defgroup hgdi_colors24 24-bit Colors
 *  @ingroup GDI
 *  @brief 24-Bit Colors for GDI
 *
 *  @note ...
 *  @author momo
 *  @version 0.0.1
 */

/**@{*/

/** @brief typedef for color_t type */
typedef uint32_t color_t;

/*
 * 24-bit colortabelle
 */

//stdcolors

/** @brief white */
#define WHITE 		0xFFFFFF
/** @brief black */
#define BLACK		0x000000

/** @brief red */
#define RED			0xFF0000
/** @brief green */
#define GREEN		0x00FF00
/** @brief blue */
#define BLUE		0x0000FF

//some gray
/** @brief lightgray */
#define LIGHTGRAY	0xAAAAAA
/** @brief gray */
#define GRAY		0x888888
/** @brief darkgray */
#define DARKGRAY	0x555555

//cmy
/** @brief cyan */
#define	CYAN		0x00FFFF
/** @brief magenta */
#define MAGENTA		0xFF00FF
/** @brief yellow */
#define YELLOW		0xFFFF00

/**@}*/
#endif /*COLORS24_H_*/
