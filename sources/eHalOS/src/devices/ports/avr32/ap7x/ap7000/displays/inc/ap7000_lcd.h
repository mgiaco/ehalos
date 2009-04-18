#ifndef AP7000_LCD_H_
#define AP7000_LCD_H_
/**
 * @file ap7000_lcd.h
 * @brief AP7000 hardware dependet graphic/lcd header
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
 *  @defgroup DEVICE_PORTS Hardware dependent
 *  @ingroup DEVICE
 */

/**
 *  @defgroup AP7 AP7000
 *  @ingroup DEVICE_PORTS
 *  @brief Hardware dependent (AP7000) functionality
 */

/**
 *  @defgroup ap7000_lcd LCD
 *  @ingroup AP7
 *  @brief AP7000 dependent graphic code
 *
 *  @note ...
 *  @author momo, giaco
 *  @version 0.0.1
 */

/**@{*/
#include <inttypes.h>

#include "graphics.h"
#include "graphis_hw_interface.h"

#include "pio.h"
#include "pm_at32ap7000.h"

#define AP7000_TFT		0
#define AP7000_TFT_BG	1


//start of framebuffer
#define LCD_FRAMEBUFFER 0x90000000		/**< startaddress of framebuffer */

//define number of colors to use
#define COLORS BPP_24

/*! \brief Struct that defines the configuration of the LCD controller
 */
typedef struct lcdc_configuration_s {
	/*! \brief Base address for the upper panel (in dual scan mode) or complete frame */
	unsigned int dmabaddr1;
	/*! \brief Base address of lower panel (dual scan mode only)*/
	unsigned int dmabaddr2;
	/*! \brief Burst length of DMA controller */
	unsigned char burst_length;
	/*! \brief Number of columns on the display (in pixels) */
	unsigned short	xres;
	/*! \brief Number of rows on the display (in pixels) */
	unsigned short	yres;

	/*! \brief Enables or disables the 2D addressing mode
	 *  If 2D addressing is activated the values in \ref virtual_xres and \ref virtual_yres
	 *  must be set according to the virtual frame size.
	 *  \arg MODE_2D_ON
	 *  \arg MODE_2D_OFF
	 */
	unsigned char set2dmode;

	/*! \brief Virtual horizontal size of the display (in pixels)
	 *  Use this in 2D addressing mode to set the size of the frame buffer
	 */
	unsigned int virtual_xres;

	/*! \brief Virtual vertical size of the display (in pixels)
	 *  Use this value in 2D addressing mode to set the size of the frame buffer
	 */
	unsigned int virtual_yres;

	/*! \brief Frame rate of the display */
	unsigned short frame_rate;

	/*! \brief LCD controller clock
	 *  Frequency in MHz at which the LCD module runs.
	 *  This can be set in the generic clock setup.
	 */
	unsigned int lcdcclock;

	/*! \brief Delay in frame periods between applying control signals to the LCD module and setting PWR high, and between setting PWR low and removing control signals from LCD module */
	unsigned short guard_time;

	/* LCD configuration register 2 */
	/*! \brief Memory organization
	 *  \arg BIG_ENDIAN
	 *  \arg LITTLE_ENDIAN
	 *  \arg WIN_CE
	 */
	unsigned char	memor;

	/*! \brief Interface width (only valid for STN mode)
	 *  \arg IF_WIDTH4 (only valid in STN single scan mode)
	 *  \arg IF_WIDTH8
	 *  \arg IF_WIDTH16 (only valid in dual scan mode)
	 */
	unsigned char	ifwidth;
	/*! \brief Scan mode
	 *  \arg SINGLE_SCAN
	 *  \arg DUAL_SCAN
	 */
	unsigned char	scanmod;
	/*! \brief Display type
	 *  \arg STN_MONO
	 *  \arg STN_COLOR
	 *  \arg TFT
	 */
	unsigned char	distype;
	/*! \brief Data polarity
	 *  \arg NORMAL
	 *  \arg INVERTED
	 */
	unsigned char 	invvd;

	/*! \brief Vertical sync polarity
	 *  \arg NORMAL (active high)
	 *  \arg INVERTED (active low)
	 */
	unsigned char 	invframe;

	/*! \brief Horizontal sync polarity
	 *  \arg NORMAL (active high)
	 *  \arg INVERTED (active low)
	 */
	unsigned char 	invline;
	/*! \brief Pixel clock polarity
	 *  \arg NORMAL (data fetched at falling edge)
	 *  \arg INVERTED (data fetched at rising edge)
	 */
	unsigned char	invclk;

	/*! \brief Data valid polarity
	 *  \arg NORMAL (active high)
	 *  \arg INVERTED (active low)
	 */
	unsigned char invdval;

	/*! \brief Pixel clock mode
	 *  \arg PARTLY_ACTIVE (only active during display period)
	 *  \arg ALWAYS_ACTIVE (needed for TFT mode)
	 */
	unsigned char	clkmod;

	/*! \brief Bits per pixel
	 *  \arg BPP_1
	 *  \arg BPP_2
	 *  \arg BPP_4
	 *  \arg BPP_8
	 *  \arg BPP_16
	 *  \arg BPP_24 (packed 24bpp)
	 *  \arg BPP_32 (unpacked 24bpp)
	 */
	unsigned char pixelsize;

	/*! \brief Contrast value
	 *  PWM compare value used to adjust the analog value obtained after an external
	 *  filter to control the contrast of the display.
	 */
	unsigned char ctrstval;

	/*! \brief Contrast PWM prescaler
	 *  This Prescaler divides the LCD controller clock for the contrast PWM generator
	 *  \arg PRE_NONE No prescaling
	 *  \arg PRE_HALF LCD controller clock divided by 1/2
	 *  \arg PRE_FOURTH LCD controller clock divided by 1/4
	 *  \arg PRE_EIGTH LCD controller clock divided by 1/8
	 */
	unsigned char ctrst_ps;

	/*! \brief Contrast PWM polarity
	 *  This value defines the polarity of the contrast PWM output. If NORMAL, the output pulses are high level
	 *  (the output will be high whenever the value in the counter is less than the value in
	 *  the compare register CONTRAST_VAL \ref ctrstval ). If INVERTED, the output pulses are low level.
	 *  \arg NORMAL
	 *  \arg INVERTED
	 */
	unsigned char ctrst_pol;

	/*! \brief Contrast PWM generator enable
	 *  \arg ENABLED
	 *  \arg DISABLED
	 */
	unsigned char ctrst_ena;

	/*! \brief Toggle rate
	 *  Toggle the polarity after each frame (EACH_FRAME) or by a specified value (MVAL_DEFINED).
	 *  \arg EACH_FRAME
	 *  \arg MVAL_DEFINED
	 */
	unsigned char mmode;

	/*! \brief Toggle rate value.
	 *  If Toggle rate is set to MVAL_DEFINED this value sets toggle rate to mval + 1 line periods.
	 *  \arg MVAL_DEFINED
	 *  \arg EACH_FRAME
	 */
	unsigned char mval;

	/*! \brief Horizontal sync pulse width
	 *  Width of the HSYNC pulse, given in pixel clock cycles. Width is (HPW+1) PCLK cycles.
	 */
	unsigned short	hpw;

	/*! \brief Horizontal back porch
	 *  Number of idle pixel clock cycles at the beginning of the line. Idle period is (HBP+1) pixel clock cycles.
	 */
	unsigned char hbp;		/* Wait before of line */

	/*! \brief Horizontal front porch
	 *  Number of idle pixel clock cycles at the end of the line. Idle period is (HFP+1) pixel clock cycles.
	 */
	unsigned char	hfp;		/* Wait end of line */

	/*! \brief Vertical sync pulse width
	 *  In TFT mode, these bits equal the vertical synchronization pulse width, given in number of lines.
	 *  VSYNC width is equal to (VPW+1) lines. In STN mode, these bits should be set to 0.
	 */
	unsigned char	vpw;

	/*! \brief Vertical back porch
	 *  In TFT mode, these bits equal the number of idle lines at the beginning of the frame. In STN mode, these bits should be set to 0.
	 */
	unsigned char	vbp;		/* Wait before of frame */

	/*! \brief Vertical front porch
	 *  In TFT mode, these bits equal the number of idle lines at the end of the frame. In STN mode, these bits should be set to 0.
	 */
	unsigned char	vfp;		/* Wait end of frame */

	/*! \brief Vertical to horizontal delay
	 *  In TFT mode, this is the delay between VSYNC rising or falling edge and HSYNC rising edge.
	 *  Delay is (VHDLY+1) pixel clock cycles. In STN mode, these bits should be set to 0.
	 */
	unsigned char vhdly;

} lcdc_conf_t;

#define STN_MONO 0
#define STN_COLOR 1
#define TFT 2

#define SINGLE_SCAN 0
#define DUAL_SCAN 1

#define IF_WIDTH4 0
#define IF_WIDTH8 1
#define IF_WIDTH16 2

#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1
#define WIN_CE 2

#define MODE_2D_ON 1
#define MODE_2D_OFF 0

#define BPP_1 1
#define BPP_2 2
#define BPP_4 4
#define BPP_8 8
#define BPP_16 16
#define BPP_24 24
#define BPP_32 32

#define NORMAL 0
#define INVERTED 1

#define ENABLED 1
#define DISABLED 0

#define PRE_NONE 0
#define PRE_HALF 1
#define PRE_FOURTH 2
#define PRE_EIGTH 3

#define PARTLY_ACTIVE 0
#define ALWAYS_ACTIVE 1

#define EACH_FRAME 0
#define MVAL_DEFINED 1


#define DISP_ON  (1<<31)				// pin for display on
#define DISP_OFF (1<<31)
#define BL_ON    (1<<28)				// pin for backlight
#define BL_OFF   (1<<28)
#define LCD_ON ((BL_ON) | (DISP_ON))
#define LCD_OFF ((BL_ON) | (DISP_ON))

/**
 * @brief Configures the LCD module
 *
 * @param lcdc_conf Pointer to LCD controller configuration structure
 *
 * @return Status
 *  @retval 0 = success
 *  @retval -1 = failure
 *
 */
int init_gdi_driver (lcdc_conf_t* lcdc_conf);

/**
 * @brief Initalization of graphic device
 *
 * @param lcdc_conf Pointer to LCD controller configuration structure
 *
 */
//void init_graphics(lcdc_conf_t* lcdc_conf);

/**
 * @brief Initalization of graphic device
 */
void init_graphics2();

/**
 * @brief real frame buffer clean
 */
void real_buffer_clear(void);

/** @brief typedef enum {False, True}  boolean */
typedef uint8_t boolean;

// some handy DEFINES
#ifndef FALSE
    #define FALSE               0
    #ifndef TRUE
        #define TRUE                !FALSE
    #endif
#endif

/* display definition */
#define GLCD_LEFT			0		/**< minimal vertical position */
#define GLCD_TOP			0		/**< minimal horizontal position*/
#define GLCD_RIGHT			480 	/**< maximal vertical position */
#define GLCD_BOTTOM			272 	/**< maximal horizontal position */

/**
 * @brief Configures the LCD module
 *
 * @param lcdconf Pointer to LCD controller configuration structure
 *
 * @return Status
 *  @retval 0 = success
 *  @retval -1 = failure
 *
 */
extern void SetLCD_Conf(lcdc_conf_t *lcdconf);

/**
 * @brief Initalize Driver
 */
extern void InitDriver(void);

/**
 * @brief Sets the device context
 *
 * @param DC pointer to device context
 * @param deviceNumber Number of device
 *
 */
extern void SetDeviceContext(DeviceContext_t *DC, uint8_t deviceNumber);

/**
 * @brief Configures PIO
 */
void lcd_pio_config(void);

/**@}*/
#endif /*AP7000_LCD_H_*/
