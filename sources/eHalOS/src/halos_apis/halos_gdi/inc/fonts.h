#ifndef FONTS_H_
#define FONTS_H_

/**
 * @file fonts.h
 * @brief Font enumeration for Halos
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */


/**
 *  @defgroup API  Halos API's
 */

/**
 *  @defgroup GDI Halos GDI
 *  @ingroup API
 */

/**
 *  @defgroup hgdi_fonts Fonts
 *  @ingroup GDI
 *  @brief Halos Fonts for GDI API
 *
 *  @note ...
 *  @author momo
 *  @version 0.0.1
 */

/**@{*/

/**
 * @brief Enumeration of Halos Fonts
 */
typedef enum _halos_font_t {

	/** @brief  Handel bold 14 font */
	HANDEL_BOLD_14 = 0,
	/** @brief  Arial bold 14 font */
	ARIAL_BOLD_14,
} halos_font_t;

/**@}*/

#endif /* FONTS_H_ */
