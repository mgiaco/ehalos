#include <stdint.h>
#include <string.h>
/**
 * @file ap7000_lcd.c
 * @brief AP7000 hardware dependet graphic/lcd implementation
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
 *  @author momo
 *  @version 0.0.0.1
 */

/**@{*/
#include "ap7000_lcd.h"
#include "ap7000_lcd_bg.h"



/* private prototypes */

/**
 * @brief Set current foreground color
 * @param Color new color
 */
static void SetColor(uint32_t Color);

/**
 * @brief Set current background color
 * @param Color new color
 */
static void SetBackgroundColor(uint32_t Color);

/**
 * @brief Set current font color
 * @param color new color
 */
static void SetFontColor(uint32_t color);

/**
 * @brief sets a pixel with current color at given position
 * @param x1 x-coordinate
 * @param y1 y-coordinate
 */
static void PutPixel(uint16_t x1, uint16_t y1);

/**
 * @brief sets a pixel with current backcolor at given position
 * @param x1 x-coordinate
 * @param y1 y-coordinate
 */
static void ClrPixel(uint16_t x1, uint16_t y1);

/**
 * @brief pastes raw data directly into framebuffer; using memset
 * @param data pointer to data-array
 * @param x x-position
 * @param y y-position
 * @param length length of data array
 */
static void PasteRawData(uint8_t* data, uint16_t x, uint16_t y, uint16_t length);

/**
 * @brief draws a line by manipulating the framebuffer
 * @param x1 x-coordinate of startpoint
 * @param y1 y-coordinate of startpoint
 * @param x2 x-coordinate of endpoint
 * @param y2 y-coordinate of endpoint
 */
static void Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief updates part of framebuffer
 * @param top startposition of framebufferupdate
 * @param bottom endposition of framebufferupdate
 */
static void Update(uint8_t top, uint8_t bottom);

/**
 * @brief vertical framebuffer scrolling
 * @param yoffset scrollvalue of framebuffer
 * @arg yoffset < 0: scroll up
 * @arg yoffset > 0: scroll down
 */
static void VerticalPan(int16_t yoffset);

/**
 * @brief indicator variable of lcd initalization
 */
static uint8_t initDone = FALSE;

void *lcd_console_address;
void *lcd_base;


DeviceContext_t* myDC; 	/**< pointer to current device context */
lcdc_conf_t *myLCD;		/**< pointer to lcd configuration structure */

lcdc_conf_t my_lcd_config ={

			dmabaddr1: 0x10000000,
			dmabaddr2: 0,
			burst_length: 8,
			xres: 480,
			yres: 272,
			set2dmode: MODE_2D_OFF,
			virtual_xres: 960,
			virtual_yres: 544,
			frame_rate: 60,
			lcdcclock: 0,
			guard_time: 2,
			memor: BIG_ENDIAN,
			ifwidth: 0,
			scanmod: SINGLE_SCAN,
			distype: TFT,
			invvd: NORMAL,
			invframe: INVERTED,
			invline: INVERTED,
			invclk: NORMAL,
			invdval: NORMAL,
			clkmod: ALWAYS_ACTIVE,
			pixelsize: BPP_24,
			ctrstval: 0x0f,
			ctrst_ena: DISABLED,
			ctrst_pol: NORMAL,
			ctrst_ps: PRE_HALF,
			mval: 0,
			mmode: EACH_FRAME,
			hpw: 41,
			hbp: 2,
			hfp: 2,
			vpw: 10,
			vbp: 12,
			vfp: 2,
			vhdly:0,
		};


void SetLCD_Conf(lcdc_conf_t *lcdconf)
{
	myLCD = lcdconf;
}

void SetColor(uint32_t Color)
{
    myDC->Color = Color;
}

void SetBackgroundColor(uint32_t color)
{
	myDC->BackColor = color;
}
void SetFontColor(uint32_t color)
{
	myDC->FontSettings.FontColor = color;
}

// TODO include linethickness in function

// TODO fix framebuffer issue (wrong drawing position)
void PutPixel(uint16_t x, uint16_t y){
	uint8_t* framePtr;
	uint32_t color = myDC->Color;
//	uint8_t thickness = myDC->LineSettings.thickness;
	framePtr = (unsigned char *) (0x90000000);
//	uint32_t offset = (myLCD->xres*y + x)*3;
	uint32_t offset = (myDC->Display.MaxX*y + x)*3;
//	uint32_t nextline = myDC->Display.MaxX*3;
	uint8_t* framePtrOffset =  framePtr+offset;
//	if( (thickness % 2) == 0)
//	{
//
//	}
	*(framePtrOffset)    = (color & 0x0000FF);
	*(framePtrOffset+1)  = (color & 0x00FF00) >> 8;
	*(framePtrOffset+2)  = (color & 0xFF0000) >> 16;
}



void ClrPixel(uint16_t x, uint16_t y){
	unsigned char * framePtr;
	uint32_t color = myDC->BackColor;
	framePtr = (unsigned char *) (0x90000000);
//	int offset = (myLCD->xres*y + x)*3;
	int offset = (myDC->Display.MaxX*y + x)*3;
	unsigned char *framePtrOffset =  framePtr+offset;
	*(framePtrOffset)    = (color & 0x0000FF);
	*(framePtrOffset+1)  = (color & 0x00FF00) >> 8;
	*(framePtrOffset+2)  = (color & 0xFF0000) >> 16;
}

//Paste raw data into framebuffer
void PasteRawData(uint8_t* data, uint16_t x, uint16_t y, uint16_t length) {
	unsigned char * framePtr;
	framePtr = (unsigned char *) (0x90000000);
	int offset = (myLCD->xres*y + x)*3;
	unsigned char *framePtrOffset =  framePtr+offset;
	memcpy(framePtrOffset,data,length*3);
}

void Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

    uint16_t length, xTmp, yTmp, i, y, yAlt;
    int16_t m;

    if(x1 == x2) {                                  // vertical line
        // x1|y1 must be the upper point
        if(y1 > y2) {
            xTmp = x1;
            yTmp = y1;
            x1 = x2;
            y1 = y2;
            x2 = xTmp;
            y2 = yTmp;
        }

        length = y2-y1;
        for(i=0; i<=length; i++) {
            //glcdDot(x1, y1+i, myDC->Color);
            PutPixel(x1,y1+i);
        }
    } else if(y1 == y2) {                           // horizontal line
        // x1|y1 must be the left point
        if(x1 > x2) {
            xTmp = x1;
            yTmp = y1;
            x1 = x2;
            y1 = y2;
            x2 = xTmp;
            y2 = yTmp;
        }

        length = x2-x1;
        for(i=0; i<=length; i++) {
            //glcdDot(x1+i, y1, myDC->Color);
            PutPixel(x1+i,y1);
        }
    } else {
        // x1 must be smaller than x2
        if(x1 > x2) {
            xTmp = x1;
            yTmp = y1;
            x1 = x2;
            y1 = y2;
            x2 = xTmp;
            y2 = yTmp;
        }

        if((y2-y1) >= (x2-x1) || (y1-y2) >= (x2-x1)) {  // angle larger or equal 45°
            length = x2-x1;                             // not really the length :)
            m = ((y2-y1)*200)/length;
            yAlt = y1;
            for(i=0; i<=length; i++) {
                y = ((m*i)/200)+y1;

                if((m*i)%200 >= 100)
                    y++;
                else if((m*i)%200 <= -100)
                    y--;

                Line(x1+i, yAlt, x1+i, y); /* wuff wuff recurs. */

                if(length<=(y2-y1) && y1 < y2)
                    yAlt = y+1;
                else if(length<=(y1-y2) && y1 > y2)
                    yAlt = y-1;
                else
                    yAlt = y;
            }
        } else {                                        // angle smaller 45°
            // y1 must be smaller than y2
            if(y1 > y2) {
                xTmp = x1;
                yTmp = y1;
                x1 = x2;
                y1 = y2;
                x2 = xTmp;
                y2 = yTmp;
            }
            length = y2-y1;
            m = ((x2-x1)*200)/length;
            yAlt = x1;
            for(i=0; i<=length; i++) {
                y = ((m*i)/200)+x1;

                if((m*i)%200 >= 100)
                    y++;
                else if((m*i)%200 <= -100)
                    y--;

                Line(yAlt, y1+i, y, y1+i); /* wuff */

                if(length<=(x2-x1) && x1 < x2)
                    yAlt = y+1;
                else if(length<=(x1-x2) && x1 > x2)
                    yAlt = y-1;
                else
                    yAlt = y;
            }
        }
    }
}


void Update(uint8_t top, uint8_t bottom)
{
//    uint8_t x, y, i;
//    uint8_t yt, yb;
//    uint16_t xoffset, index;
//    uint8_t data;
//
//    index=0;
//
//    yt = top >> 3;  //select page top
//    yb = bottom >> 3;   //select page bottom
//
//    for (i=0;i<GLCD_NUMCONTROLLER;i++) {
//        glcdSelectChip(i);
//        for (y=yt;y<=yb;y++) {
//            glcdSetPage(y);
//            glcdSetAddress(0);
//            xoffset = i * GLCD_MAXADDRESS;
//            for (x=0;x<GLCD_MAXADDRESS;x++) {
//                index = (xoffset+x) + (((y*8)>>3) << 7);
//                data = myDC->Display.Buffer[index];
//                //if(inv) data =~ data;
//                glcdWriteData(data);
//            }
//        }
//    }
//    glcdSelectChip(GLCD_SELECTNONE);
}
void VerticalPan(int16_t yoffset){
	unsigned char * framePtr;
	unsigned char *framePtrOffset;
	int offset;
	int length;
	framePtr = (unsigned char *) (0x90000000);

	// scroll framebuffer up
	if(yoffset<=0){
		offset = yoffset*myLCD->xres*3;
		framePtrOffset = framePtr - offset;
		length = ((myLCD->yres+1+yoffset)*myLCD->xres*3);
		memcpy(framePtr, framePtrOffset, length);
	}

	// scroll framebuffer down
	else {
		offset = yoffset*myLCD->xres*3;
		framePtrOffset = framePtr + offset;
		length = (myLCD->yres-yoffset)*myLCD->xres*3;
		memcpy(framePtrOffset,  framePtr, length);
	}
}
void SetDeviceContext(DeviceContext_t *DC, uint8_t deviceNumber)
{
    //TODO change LCD Controller for actual Device!! 2 LCD´s


    switch(CurrentDevice)
    {
        case AP7000_TFT:
        	DC->Display.VerticalPan = VerticalPan;
            DC->Display.SetColor = SetColor;
            DC->Display.SetBackgroundColor = SetBackgroundColor;
            DC->Display.SetFontColor = SetFontColor;
            DC->Display.Line = Line;
            DC->Display.PutPixel = PutPixel;
            DC->Display.ClrPixel = ClrPixel;
            DC->Display.PasteRawData = PasteRawData;
            DC->Display.Update = Update; //Funktionen den Funktionspointern zuweisen
            break;
        case AP7000_TFT_BG:
        	DC->Display.VerticalPan = VerticalPan_BG;
            DC->Display.SetColor = SetColor_BG;
            DC->Display.SetBackgroundColor = SetBackgroundColor_BG;
            DC->Display.SetFontColor = SetFontColor_BG;
            DC->Display.Line = Line_BG;
            DC->Display.PutPixel = PutPixel_BG;
            DC->Display.ClrPixel = ClrPixel_BG;
            DC->Display.PasteRawData = PasteRawData_BG;
            DC->Display.Update = Update_BG; //Funktionen den Funktionspointern zuweisen
            break;
        default: break;
    }

    DC->Display.MaxColors = 1;
    DC->Display.MaxX = 480;
    DC->Display.MaxY = 272;


    myDC = DC;
}

void InitDriver(void)
{
    //SetDeviceContext(DC,deviceNumber);

    if(!initDone)
    {
        //glcdInit();
        initDone = TRUE;
    }

}



#define PRERIPHERAL_A 0
#define PRERIPHERAL_B 1

void lcd_pio_config(void){
	pio_setup_pin(AVR32_LCDC_HSYNC_0_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_VSYNC_0_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_PCLK_0_PIN, PRERIPHERAL_A);

	pio_setup_pin(AVR32_LCDC_DATA_0_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_1_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_2_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_3_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_4_1_PIN, PRERIPHERAL_B);

	pio_setup_pin(AVR32_LCDC_DATA_5_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_DATA_6_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_DATA_7_PIN, PRERIPHERAL_A);

	pio_setup_pin(AVR32_LCDC_DATA_8_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_9_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_10_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_11_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_12_1_PIN, PRERIPHERAL_B);

	pio_setup_pin(AVR32_LCDC_DATA_13_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_DATA_14_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_DATA_15_PIN, PRERIPHERAL_A);

	pio_setup_pin(AVR32_LCDC_DATA_16_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_17_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_18_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_19_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_20_1_PIN, PRERIPHERAL_B);
	pio_setup_pin(AVR32_LCDC_DATA_21_1_PIN, PRERIPHERAL_B);

	pio_setup_pin(AVR32_LCDC_DATA_22_PIN, PRERIPHERAL_A);
	pio_setup_pin(AVR32_LCDC_DATA_23_PIN, PRERIPHERAL_A);
}


void lcd_on(void)
{
	volatile avr32_pio_t *pioa = &AVR32_PIOA;
	volatile avr32_pm_t *ppm = &AVR32_PM;

	lcd_base = (void *)(0x90000000);
	lcd_console_address = (void *)(lcd_base);


	 // Enable CLOCK for LCDC in HSBMASK
	  pm_enable_module(PM_HSB_LCDC);

	  // Enable generic clock PLL0 for LCD controller pixel clock
	  pm_gen_clk_opt_t gen_clk_opt = {
	    .clock_source = PM_PLL0,
	    .divider = 2,
	  };
	  pm_start_generic_clock(7, &gen_clk_opt);

	//ppm->hsbmask |= 0x00000080;
	/* Enable generic clock PLL0 for LCD controller pixel clock */
	//ppm->gcctrl[7] |= 0x00000006;


	pioa->per  |= LCD_ON; // PIO Controller PIO Enable Register
	pioa->oer  |= LCD_ON; // PIO Controller Output Enable Register
	pioa->idr  |= LCD_ON; // PIO Controller Interrupt Disable Register
	pioa->puer |= LCD_ON; // PIO Pull Up Enable Register
	pioa->sodr |= LCD_ON;
	lcd_pio_config();

};

//extern int burst_length;

int init_gdi_driver(lcdc_conf_t* lcdc_conf)
{
	volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
	unsigned char valid_data_lines = 0;
	unsigned char pixel_size = 0;
	unsigned char clkval = 0;
	unsigned int pixel_clock_theo = 0;
	unsigned short lineval, hozval;


	/* Turn off LCD Controller (core first then DMA) */
	plcdc->pwrcon &= ~(1 << AVR32_LCDC_PWRCON_PWR_OFFSET);
	plcdc->dmacon &= ~(1 << AVR32_LCDC_DMACON_DMAEN_OFFSET);

	/* LCDFRCFG */
	if(lcdc_conf->distype != TFT){
		switch(lcdc_conf->scanmod){
		case SINGLE_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH4:
				valid_data_lines = 4;
				break;
			case IF_WIDTH8:
				valid_data_lines = 8;
				break;
			default:
				return -1;
		}

		case DUAL_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH8:
				valid_data_lines = 4;
				break;
			case IF_WIDTH16:
				valid_data_lines = 8;
				break;
			default:
				return -1;
			}
		default:
			return -1;
		}
	}

	lineval = lcdc_conf->yres - 1;
	switch(lcdc_conf->distype){
	case STN_MONO:
		hozval = (lcdc_conf->xres / valid_data_lines) - 1;
		break;
	case STN_COLOR:
		hozval = (lcdc_conf->xres * 3 / valid_data_lines) - 1;
		break;
	case TFT:
		hozval = lcdc_conf->xres - 1;
		break;
	default:
		return -1;
	}
	plcdc->lcdfrmcfg = (lineval & AVR32_LCDC_LINEVAL_MASK) |((hozval << AVR32_LCDC_HOZVAL) & AVR32_LCDC_HOZVAL_MASK);

	/* Calculation of theoretical pixel clock */
	switch(lcdc_conf->distype){
		case STN_MONO:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres / valid_data_lines;
			break;
		case STN_COLOR:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres * 3 / valid_data_lines;
			break;
		case TFT:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres;
			break;
		default:
			return -1;
	}
	clkval = (pm_read_module_freq_hz(PM_HSB_LCDC) / (2 * pixel_clock_theo));

	if (clkval == 0) {
		plcdc->lcdcon1 = 1; /* bypass pixel clock */
	} else {
		plcdc->lcdcon1 = ((clkval - 1) << AVR32_LCDC_LCDCON1_CLKVAL) & AVR32_LCDC_LCDCON1_CLKVAL_MASK;
	}

	/* LCDCON2 */
	switch(lcdc_conf->pixelsize){
		case 1: pixel_size = 0;break;
		case 2: pixel_size = 1;break;
		case 4: pixel_size = 2;break;
		case 8: pixel_size = 3;break;
		case 16: pixel_size = 4;break;
		case 24: pixel_size = 5;break;
		case 32: pixel_size = 6;break;
		default:
			return -1;
	}

	plcdc->lcdcon2 = (lcdc_conf->distype & AVR32_LCDC_LCDCON2_DISTYPE_MASK) |
			((lcdc_conf->scanmod << AVR32_LCDC_LCDCON2_SCANMOD) & AVR32_LCDC_LCDCON2_SCANMOD_MASK) |
			((lcdc_conf->ifwidth << AVR32_LCDC_LCDCON2_IFWIDTH) & AVR32_LCDC_LCDCON2_IFWIDTH_MASK) |
			((lcdc_conf->invvd << AVR32_LCDC_LCDCON2_INVVD) & AVR32_LCDC_LCDCON2_INVVD_MASK) |
			((lcdc_conf->invframe << AVR32_LCDC_LCDCON2_INVFRAME) & AVR32_LCDC_LCDCON2_INVFRAME_MASK) |
			((pixel_size << AVR32_LCDC_LCDCON2_PIXELSIZE) & AVR32_LCDC_LCDCON2_PIXELSIZE_MASK) |
			((lcdc_conf->invline << AVR32_LCDC_LCDCON2_INVLINE) & AVR32_LCDC_LCDCON2_INVLINE_MASK) |
			((lcdc_conf->invclk << AVR32_LCDC_LCDCON2_INVCLK) & AVR32_LCDC_LCDCON2_INVCLK_MASK) |
			((lcdc_conf->invdval << AVR32_LCDC_LCDCON2_INVDVAL) & AVR32_LCDC_LCDCON2_INVDVAL_MASK) |
			((lcdc_conf->clkmod << AVR32_LCDC_LCDCON2_CLKMOD) & AVR32_LCDC_LCDCON2_CLKMOD_MASK) |
			((lcdc_conf->memor << AVR32_LCDC_LCDCON2_MEMOR) & AVR32_LCDC_LCDCON2_MEMOR_MASK);

	/* Timings */
	plcdc->lcdtim1 = (lcdc_conf->vfp & AVR32_LCDC_LCDTIM1_VFP_MASK) |
			((lcdc_conf->vbp << AVR32_LCDC_LCDTIM1_VBP) & AVR32_LCDC_LCDTIM1_VBP_MASK) |
			(((lcdc_conf->vpw - 1) << AVR32_LCDC_LCDTIM1_VPW) & AVR32_LCDC_LCDTIM1_VPW_MASK) |
			((lcdc_conf->vhdly << AVR32_LCDC_LCDTIM1_VHDLY) & AVR32_LCDC_LCDTIM1_VHDLY_MASK);
	plcdc->lcdtim2 = (lcdc_conf->hbp & AVR32_LCDC_HBP_MASK) |
			(((lcdc_conf->hpw - 1) << AVR32_LCDC_LCDTIM2_HPW) & AVR32_LCDC_LCDTIM2_HPW_MASK) |
			((lcdc_conf->hfp << AVR32_LCDC_LCDTIM2_HFP) & AVR32_LCDC_LCDTIM2_HFP_MASK);

	/* Interrupts */
	plcdc->idr = 0xFFFFFFFF;

	/* Toggle rate */
	plcdc->lcdmval = (lcdc_conf->mval & AVR32_LCDC_LCDMVAL_MVAL_MASK) |
			((lcdc_conf->mmode << AVR32_LCDC_LCDMVAL_MMODE_OFFSET) & AVR32_LCDC_LCDMVAL_MMODE_MASK);

	/* Contrast */
	plcdc->contrast_val = lcdc_conf->ctrstval;
	plcdc->contrast_ctr = (lcdc_conf->ctrst_ps & AVR32_LCDC_CONTRAST_CTR_PS_MASK) |
			((lcdc_conf->ctrst_pol << AVR32_LCDC_CONTRAST_CTR_POL_OFFSET) & AVR32_LCDC_CONTRAST_CTR_POL_MASK) |
			((lcdc_conf->ctrst_ena << AVR32_LCDC_CONTRAST_CTR_ENA_OFFSET) & AVR32_LCDC_CONTRAST_CTR_ENA_MASK);

	/* Setup FIFO */
	int lcd_fifo_size = lcdc_conf->scanmod ? 256 : 512;
	plcdc->lcdfifo = lcd_fifo_size - (2 * lcdc_conf->burst_length + 3);

	/* DMA base address */
	plcdc->dmabaddr1 = lcdc_conf->dmabaddr1;
	if(lcdc_conf->scanmod == DUAL_SCAN){
		plcdc->dmabaddr2 = lcdc_conf->dmabaddr2;
	}

	/* DMA frame configuration
	* The frame size is measured in words
	*/
	plcdc->dmafrmcfg = ((((lcdc_conf->xres * lcdc_conf->yres * lcdc_conf->pixelsize) + 31 )/ 32) & AVR32_LCDC_DMAFRMCFG_FRMSIZE_MASK) |
			(((lcdc_conf->burst_length - 1) << AVR32_LCDC_DMAFRMCFG_BRSTLEN) & AVR32_LCDC_DMAFRMCFG_BRSTLEN_MASK);

	/* 2D configuration */
	if(lcdc_conf->set2dmode){
		/* Assumed is that the frame starts at a word boundary -> no pixel offset needed */
		plcdc->dma2dcfg = ((lcdc_conf->virtual_xres - lcdc_conf->xres) * (lcdc_conf->pixelsize / 8)) & AVR32_LCDC_DMA2DCFG_ADDRINC_MASK;
	}

	/* wait for DMA engine to become idle */
	while (plcdc->dmacon & AVR32_LCDC_DMACON_DMABUSY);

	/* and enable DMA with updated configuration */
	if(lcdc_conf->set2dmode){
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMA2DEN_OFFSET);
	}
	else{
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
	}


	/* enable LCD */
	plcdc->pwrcon |= (lcdc_conf->guard_time << AVR32_LCDC_PWRCON_GUARD_TIME_OFFSET) & AVR32_LCDC_PWRCON_GUARD_TIME_MASK;

	/* Wait for the LCDC core to become idle and enable it */
	while (plcdc->PWRCON.busy == 1);
	plcdc->PWRCON.pwr = 1;

	return 0;
};

int lcdc_init(lcdc_conf_t* lcdc_conf)
{
	volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
	unsigned char valid_data_lines = 0;
	unsigned char pixel_size = 0;
	unsigned char clkval = 0;
	unsigned int pixel_clock_theo = 0;
	unsigned short lineval, hozval;

	/* Turn off LCD Controller (core first then DMA) */
	plcdc->pwrcon &= ~(1 << AVR32_LCDC_PWRCON_PWR_OFFSET);
	plcdc->dmacon &= ~(1 << AVR32_LCDC_DMACON_DMAEN_OFFSET);

	/* LCDFRCFG */
	if(lcdc_conf->distype != TFT){
		switch(lcdc_conf->scanmod){
		case SINGLE_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH4:
				valid_data_lines = 4;
				break;
			case IF_WIDTH8:
				valid_data_lines = 8;
				break;
			default:
				return -1;
		}

		case DUAL_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH8:
				valid_data_lines = 4;
				break;
			case IF_WIDTH16:
				valid_data_lines = 8;
				break;
			default:
				return -1;
			}
		default:
			return -1;
		}
	}

	lineval = lcdc_conf->yres - 1;
	switch(lcdc_conf->distype){
	case STN_MONO:
		hozval = (lcdc_conf->xres / valid_data_lines) - 1;
		break;
	case STN_COLOR:
		hozval = (lcdc_conf->xres * 3 / valid_data_lines) - 1;
		break;
	case TFT:
		hozval = lcdc_conf->xres - 1;
		break;
	default:
		return -1;
	}
	plcdc->lcdfrmcfg = (lineval & AVR32_LCDC_LINEVAL_MASK) |((hozval << AVR32_LCDC_HOZVAL) & AVR32_LCDC_HOZVAL_MASK);

	/* Calculation of theoretical pixel clock */
	switch(lcdc_conf->distype){
		case STN_MONO:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres / valid_data_lines;
			break;
		case STN_COLOR:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres * 3 / valid_data_lines;
			break;
		case TFT:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres;
			break;
		default:
			return -1;
	}
	clkval = (pm_read_module_frequency(PM_HSB_LCDC) / (2 * pixel_clock_theo));

	if (clkval == 0) {
		plcdc->lcdcon1 = 1; /* bypass pixel clock */
	} else {
		plcdc->lcdcon1 = ((clkval - 1) << AVR32_LCDC_LCDCON1_CLKVAL) & AVR32_LCDC_LCDCON1_CLKVAL_MASK;
	}
	/* LCDCON2 */
	switch(lcdc_conf->pixelsize){
		case 1: pixel_size = 0;break;
		case 2: pixel_size = 1;break;
		case 4: pixel_size = 2;break;
		case 8: pixel_size = 3;break;
		case 16: pixel_size = 4;break;
		case 24: pixel_size = 5;break;
		case 32: pixel_size = 6;break;
		default:
			return -1;
	}

	plcdc->lcdcon2 = (lcdc_conf->distype & AVR32_LCDC_LCDCON2_DISTYPE_MASK) |
			((lcdc_conf->scanmod << AVR32_LCDC_LCDCON2_SCANMOD) & AVR32_LCDC_LCDCON2_SCANMOD_MASK) |
			((lcdc_conf->ifwidth << AVR32_LCDC_LCDCON2_IFWIDTH) & AVR32_LCDC_LCDCON2_IFWIDTH_MASK) |
			((lcdc_conf->invvd << AVR32_LCDC_LCDCON2_INVVD) & AVR32_LCDC_LCDCON2_INVVD_MASK) |
			((lcdc_conf->invframe << AVR32_LCDC_LCDCON2_INVFRAME) & AVR32_LCDC_LCDCON2_INVFRAME_MASK) |
			((pixel_size << AVR32_LCDC_LCDCON2_PIXELSIZE) & AVR32_LCDC_LCDCON2_PIXELSIZE_MASK) |
			((lcdc_conf->invline << AVR32_LCDC_LCDCON2_INVLINE) & AVR32_LCDC_LCDCON2_INVLINE_MASK) |
			((lcdc_conf->invclk << AVR32_LCDC_LCDCON2_INVCLK) & AVR32_LCDC_LCDCON2_INVCLK_MASK) |
			((lcdc_conf->invdval << AVR32_LCDC_LCDCON2_INVDVAL) & AVR32_LCDC_LCDCON2_INVDVAL_MASK) |
			((lcdc_conf->clkmod << AVR32_LCDC_LCDCON2_CLKMOD) & AVR32_LCDC_LCDCON2_CLKMOD_MASK) |
			((lcdc_conf->memor << AVR32_LCDC_LCDCON2_MEMOR) & AVR32_LCDC_LCDCON2_MEMOR_MASK);

	/* Timings */
	plcdc->lcdtim1 = (lcdc_conf->vfp & AVR32_LCDC_LCDTIM1_VFP_MASK) |
			((lcdc_conf->vbp << AVR32_LCDC_LCDTIM1_VBP) & AVR32_LCDC_LCDTIM1_VBP_MASK) |
			(((lcdc_conf->vpw - 1) << AVR32_LCDC_LCDTIM1_VPW) & AVR32_LCDC_LCDTIM1_VPW_MASK) |
			((lcdc_conf->vhdly << AVR32_LCDC_LCDTIM1_VHDLY) & AVR32_LCDC_LCDTIM1_VHDLY_MASK);

	plcdc->lcdtim2 = (lcdc_conf->hbp & AVR32_LCDC_HBP_MASK) |
			(((lcdc_conf->hpw - 1) << AVR32_LCDC_LCDTIM2_HPW) & AVR32_LCDC_LCDTIM2_HPW_MASK) |
			((lcdc_conf->hfp << AVR32_LCDC_LCDTIM2_HFP) & AVR32_LCDC_LCDTIM2_HFP_MASK);

	/* Interrupts */
	plcdc->idr = 0xFFFFFFFF;

	/* Toggle rate */
	plcdc->lcdmval = (lcdc_conf->mval & AVR32_LCDC_LCDMVAL_MVAL_MASK) |
			((lcdc_conf->mmode << AVR32_LCDC_LCDMVAL_MMODE_OFFSET) & AVR32_LCDC_LCDMVAL_MMODE_MASK);

	/* Contrast */
	plcdc->contrast_val = lcdc_conf->ctrstval;
	plcdc->contrast_ctr = (lcdc_conf->ctrst_ps & AVR32_LCDC_CONTRAST_CTR_PS_MASK) |
			((lcdc_conf->ctrst_pol << AVR32_LCDC_CONTRAST_CTR_POL_OFFSET) & AVR32_LCDC_CONTRAST_CTR_POL_MASK) |
			((lcdc_conf->ctrst_ena << AVR32_LCDC_CONTRAST_CTR_ENA_OFFSET) & AVR32_LCDC_CONTRAST_CTR_ENA_MASK);

	/* Setup FIFO */
	int lcd_fifo_size = lcdc_conf->scanmod ? 256 : 512;
	plcdc->lcdfifo = lcd_fifo_size - (2 * lcdc_conf->burst_length + 3);

	/* DMA base address */
	plcdc->dmabaddr1 = lcdc_conf->dmabaddr1;
	if(lcdc_conf->scanmod == DUAL_SCAN){
		plcdc->dmabaddr2 = lcdc_conf->dmabaddr2;
	}

	/* DMA frame configuration
	* The frame size is measured in words
	*/
	plcdc->dmafrmcfg = ((((lcdc_conf->xres * lcdc_conf->yres * lcdc_conf->pixelsize) + 31 )/ 32) & AVR32_LCDC_DMAFRMCFG_FRMSIZE_MASK) |
			(((lcdc_conf->burst_length - 1) << AVR32_LCDC_DMAFRMCFG_BRSTLEN) & AVR32_LCDC_DMAFRMCFG_BRSTLEN_MASK);

	/* 2D configuration */
	if(lcdc_conf->set2dmode){
		/* Assumed is that the frame starts at a word boundary -> no pixel offset needed */
		plcdc->dma2dcfg = ((lcdc_conf->virtual_xres - lcdc_conf->xres) * (lcdc_conf->pixelsize / 8)) & AVR32_LCDC_DMA2DCFG_ADDRINC_MASK;
	}

	/* wait for DMA engine to become idle */
	while (plcdc->dmacon & AVR32_LCDC_DMACON_DMABUSY);

	/* and enable DMA with updated configuration */
	if(lcdc_conf->set2dmode){
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMA2DEN_OFFSET);
	}
	else{
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) |
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
	}

	/* enable LCD */
	plcdc->pwrcon |= (lcdc_conf->guard_time << AVR32_LCDC_PWRCON_GUARD_TIME_OFFSET) & AVR32_LCDC_PWRCON_GUARD_TIME_MASK;

	/* Wait for the LCDC core to become idle and enable it */
	while (plcdc->PWRCON.busy == 1);
	plcdc->PWRCON.pwr = 1;

	return 0;
}


void real_buffer_clear(void){

	memset((void *)(0x90000000), 0, my_lcd_config.virtual_xres * my_lcd_config.virtual_yres * my_lcd_config.pixelsize / 8);

}


//void init_graphics(lcdc_conf_t* lcdconf) {
//	memset((void *)(0x90000000), 0, lcdconf->virtual_xres * lcdconf->virtual_yres * lcdconf->pixelsize / 8);
//
//	lcd_on();
//	lcdc_init(lcdconf);
//	SetLCD_Conf(lcdconf);
//    InitDriver();
//    setDevice(0);
//};

void init_graphics2() {

	memset((void *)(0x90000000), 0, my_lcd_config.virtual_xres * my_lcd_config.virtual_yres * my_lcd_config.pixelsize / 8);
	lcd_on();
	init_gdi_driver(&my_lcd_config);
	SetLCD_Conf(&my_lcd_config);
    InitDriver();
    setDevice(1); //change
    gotoXY(0,0);
};

/**@}*/
