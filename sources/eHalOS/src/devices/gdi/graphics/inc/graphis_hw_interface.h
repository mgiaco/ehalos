
#ifndef GRAPHIS_HW_INTERFACE_H_
#define GRAPHIS_HW_INTERFACE_H_
/**
 * @file graphis_hw_interface.h
 * @brief Graphics Hardware Interface
 * @author Team Halos
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
 *  @defgroup graphic_hw_interface Graphic Hardware Interface
 *  @ingroup GDI_DEVICE
 *  @brief Hardware Interface for Graphic device
 *
 *  @note ...
 *  @author momo, giaco
 *  @version 0.0.1
 */

/**@{*/

#include <inttypes.h>

#define MAXLCDS 3 			/**< number of different device contexts */

/*
 *  @name Callbacks
 */

// hier werden die Elementarfunktionen für das Graphics Interface definiert
typedef void (*VerticalPan_t)(int16_t yoffset);											/**< callback for VerticalPan */
typedef void (*Line_t)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); 			/**< callback for Line */
typedef void (*PutPixel_t)(uint16_t x, uint16_t y); 									/**< callback for PutPixel */
typedef void (*ClrPixel_t)(uint16_t x, uint16_t y);										/**< callback for ClrPixel */
typedef void (*SetColor_t)(uint32_t Color); 											/**< callback for SetColor */
typedef void (*SetBGColor_t)(uint32_t color);											/**< callback for SetBGColor */
typedef void (*SetFontColor_t)(uint32_t color);											/**< callback for SetFontColor */
typedef void (*PasteRawData_t)(uint8_t* data, uint16_t x, uint16_t y, uint16_t length);	/**< callback for PasteRawData */
typedef void (*Update_t)(uint16_t top, uint16_t bottom);									/**< callback for Update */

/**
 * @brief Struct that defines Display information
 */
typedef struct
{
	/** @brief max x resolution of display */
    uint16_t MaxX;

    /** @brief max y resolution of display */
    uint16_t MaxY;

    /** @brief max colors of display*/
    uint16_t MaxColors;

    char *Buffer; // Zeiger auf einen Datenpuffer, falls für das entsprechende Lcd erforderlich
    uint8_t* page_buffer;

    /** @brief  callback for hardware dependent VerticalPan */
    VerticalPan_t VerticalPan;

    /** @brief  callback for hardware dependent linedrawing */
    Line_t Line;

    /** @brief  callback for hardware dependent PutPixel */
    PutPixel_t PutPixel;

    /** @brief  callback for hardware dependent ClrPixel */
    ClrPixel_t ClrPixel;

    /** @brief  callback for hardware dependent SetColor */
    SetColor_t SetColor;

    /** @brief  callback for hardware dependent SetBackgroundColor */
    SetBGColor_t SetBackgroundColor;

    /** @brief  callback for hardware dependent SetFontColor */
    SetFontColor_t SetFontColor;

    /** @brief  callback for hardware dependent PasteRawData */
    PasteRawData_t PasteRawData;

    /** @brief  callback for hardware dependent Update */
    Update_t Update;
}DisplayInfo_t;

/**
 * @brief Struct that defines the position of the cursor
 */
typedef struct
{
	/** @brief x-postition of cursor */
    uint16_t x;

    /** @brief y-position of cursor */
    uint16_t y;
}Cursor_t;

/**
 * @brief Struct that defines the linestyle
 */
typedef struct
{
	/** @brief linestyle for line */
   uint8_t linestyle;

   /** @brief pattern */
   uint8_t upattern;

   /** @brief linethickness */
   uint8_t thickness;
}LineSettings_t;

/**
 * @brief Struct that defines the font settings
 */
typedef struct
{
	/** @brief pointer to font */
    const uint8_t  *Font;

    /** @brief height of font */
    uint8_t Height;

    /** @brief font color */
    uint32_t FontColor;

    /** @brief vertical alignment of font */
    uint8_t OrientVertical;

    /** @brief horizontal alignment of font */
    uint8_t OrientHorizontal;
    //...weitere parameter
}FontSettings_t;

/**
 * @brief Struct that defines the device context
 */
typedef struct
{
	/** @brief Cursor information */
    Cursor_t Cursor;

    /** @brief color */
    uint32_t Color;

    /** @brief background color */
    uint32_t BackColor;

    /** @brief maximal possible colors */
    uint32_t MaxColors;

    /** @brief information about font settings */
    FontSettings_t FontSettings;

    /** @brief information about line settings */
    LineSettings_t LineSettings;
    // andere Strukturen

    /** @brief information about the display itself */
    DisplayInfo_t Display; // Info zum entsprechenden Display

}DeviceContext_t;

/*
 * @name Variables
 */
uint8_t CurrentDevice;				/**< device currently active */
DeviceContext_t LCD[MAXLCDS];		/**< device context array */

/**@}*/

#endif /* GRAPHIS_HARDWARE_LAYER_H_ */
