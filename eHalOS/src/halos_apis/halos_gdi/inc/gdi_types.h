
#ifndef GDI_TYPES_H_
#define GDI_TYPES_H_

/**
 * @file gdi_types.h
 * @brief some special types for GDI
 * @author Team Halos
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
 *  @defgroup hgdi_types GDI Types
 *  @ingroup GDI
 *  @brief GDI-Types for GDI API
 *
 *  @note ...
 *  @author momo
 *  @version 0.0.1
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that defines a gdi sprite
 */
typedef struct _gdi_sprite{
	/** @brief  byte-array for sprite
	 * @arg 24-bit colored sprites: *data = { 8bit blue, 8bit green, 8bit red }
	 * @arg uncolored sprites:	*data = 0 or 1   to draw a pixel with current color
	 * */
	uint8_t* data;

	/** @brief  width of gdi sprite*/
	uint16_t width;

	/** @brief  height of gdi sprite*/
	uint16_t height;

	/** @brief  x-position of gdi sprite*/
	uint16_t x;

	/** @brief  y-position of gdi sprite*/
	uint16_t y;
}gdi_sprite_t;

/**
 * @brief Enumeration of diffrent linestyles
 */
typedef enum _line_style_t {
	/** @brief  linestyle for solid line */
	SOLID_LINE = 0,
	/** @brief  linestyle for dashed line */
	DASHED_LINE,
	/** @brief  linestyle for dotted line */
	DOTTED_LINE,
}line_style_t;

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

/**@}*/
#endif /* GDI_TYPES_H_ */
