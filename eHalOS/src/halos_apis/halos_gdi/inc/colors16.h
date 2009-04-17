/**
 * @file colors16.h
 * @brief 16-Bit Colortable
 * @author Team Halos
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */
#ifndef COLORS16_H_
#define COLORS16_H_

/**
 *  @defgroup API  Halos API's
 */

/**
 *  @defgroup GDI Halos GDI
 *  @ingroup API
 */

/**
 *  @defgroup hgdi_colors16 16-bit Colors
 *  @ingroup GDI
 *  @brief 16-Bit Colors for GDI
 *
 *  @note ...
 *  @author momo
 *  @version 0.0.1
 */

/**@{*/

/** @brief typedef for color_t type */
typedef uint16_t color_t;

/*
 * 16-bit colortabelle
 */

//stdcolors

/** @brief white */
#define WHITE 		0xFFFF
/** @brief black */
#define BLACK		0x0000

/**@}*/
#endif /* COLORS16_H_ */
