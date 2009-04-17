/**
 * @file font.c
 * @brief device independet graphic/font implementation
 * @author Team Halos
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

#include "font.h"

static FontReadCallback	FontRead;


DeviceContext_t *myDC; //actual Device Context


static void writeBuffer(uint8_t data, uint8_t mask);
static void calcXY(const char *str, uint16_t *x, uint16_t *y);


void init_font(DeviceContext_t *DC)
{
    myDC = DC;
}

uint8_t read_font_data(const uint8_t* ptr) {
	return (*ptr);
}

void select_new_font(halos_font_t fonts, FontReadCallback callback, uint32_t color) {

	uint8_t *font;

	switch (fonts) {
		case HANDEL_BOLD_14:
			font = (uint8_t*)handel_bold_14;
			break;

		case ARIAL_BOLD_14:
			font = (uint8_t*)arial_bold_14;
			break;
		default:
			break;
	}

	myDC->FontSettings.Font = font;
	FontRead = callback;
	myDC->FontSettings.FontColor = color;
    myDC->FontSettings.Height = FontRead(myDC->FontSettings.Font+FONT_HEIGHT);
}

void glcd_set_text_justify(uint8_t horiz, uint8_t vert)
{
    myDC->FontSettings.OrientHorizontal = horiz;
    myDC->FontSettings.OrientVertical = vert;
}


void calcXY(const char *str, uint16_t *x, uint16_t *y)
{
    uint16_t width, height;

    width = glcd_get_string_width(str);
    height = myDC->FontSettings.Height;

    switch (myDC->FontSettings.OrientHorizontal) {
        case LEFT_TEXT:

            (*x) = (*x);

            break;
        case CENTER_TEXT:

            //(*x) = (*x)-(width>>1);
            (*x) = (*x)-(width/2);

            break;

        case RIGHT_TEXT:

            (*x) = (*x)-width;

            break;

        default:

            (*x) = (*x);

            break;
    }

    switch (myDC->FontSettings.OrientVertical) {
        case BOTTOM_TEXT:

            (*y) = (*y);

            break;
        case CENTER_TEXT:

            //(*y) = (*y)-(height>>1);
            (*y) = (*y)-(height/2);

            break;

        case TOP_TEXT:

            (*y) = (*y)-(height);

            break;

        default:

            (*y) = (*y);

            break;
    }
}


uint8_t glcd_draw_char(const char a) {

	uint8_t data;
	uint8_t mask;

	uint8_t width = 0;
	uint8_t height = myDC->FontSettings.Height; //FontRead(myDC->FontSettings.Font+FONT_HEIGHT); //read actual font width
	uint8_t bytes = (height+7)/8; //anzahl zeilen

	uint8_t firstChar = FontRead(myDC->FontSettings.Font+FONT_FIRST_CHAR);
	uint8_t charCount = FontRead(myDC->FontSettings.Font+FONT_CHAR_COUNT);

	uint16_t index = 0;
	uint16_t x,y;
	char c = a;

	x = getX();
	y = getY();

	if(c < firstChar || c >= (firstChar+charCount)) { //see if the char is in the fonttable
		return 1;
	}

	c-= firstChar; //make zero based index I think, begins at firstChar

	// read width data, to get the index
        uint8_t i;
	for( i=0; i<c; i++) {
		index += FontRead(myDC->FontSettings.Font+FONT_WIDTH_TABLE+i);
	}
	index = index*bytes+charCount+FONT_WIDTH_TABLE;
	width = FontRead(myDC->FontSettings.Font+FONT_WIDTH_TABLE+c);

	// last but not least, draw the character
	//uint8_t i;
	for(i=0; i<bytes; i++) {
		uint8_t page = i*width; //select page for font
		uint8_t j;
		for(j=0; j<width; j++) {
			// 110
			data = FontRead(myDC->FontSettings.Font+index+page+j);

			if(height < (i+1)*8) {
				data >>= (i+1)*8-height;
			}

            if(i==bytes-1)
            {
                mask = height%8; //schrift kleiner 1 Byte
                //mask = 3;
            }
            else
            {
                mask = 0;
            }

			if(myDC->FontSettings.FontColor == BLACK)
			{
				//glcdWriteData(data);
				//glcdBuffer[y][GLCD_X_BYTES] = data;
				writeBuffer(data, mask);
                setX(x+j+1);

			} else
			{
				//glcdWriteData(~data);
				//glcdBuffer[y][GLCD_X_BYTES] = data;
				writeBuffer(~data, mask);
                setX(x+j+1);
			}
		}
		// 1px gap between chars
		if(myDC->FontSettings.FontColor == BLACK)
		{
			//glcdWriteData(0x00);
			//glcdBuffer[y][GLCD_X_BYTES] = 0x00;
			writeBuffer(0x00, mask);
            gotoXY(x+1,y);

		} else
		{
			//glcdWriteData(0xFF);
			//glcdBuffer[y][GLCD_X_BYTES] = 0xFF;
			writeBuffer(0xff, mask);
            gotoXY(x+1,y);
		}
		//y = getY();
		gotoXY(x,y+8);
	}

	gotoXY(x+width+1, y);

	return 0;
}

void glcd_draw_string(const char *str) {

    uint16_t x = getX(); //get actual X
    uint16_t y = getY(); //get actual Y
    uint16_t xtemp, ytemp;

    xtemp = x;
    ytemp = y;

    calcXY(str,&xtemp,&ytemp);

    gotoXY(xtemp,ytemp);

	while(*str != 0) {
		if(*str == '\n') {
			gotoXY(x, getY()+FontRead(myDC->FontSettings.Font+FONT_HEIGHT));
		} else {
			glcd_draw_char(*str);
		}
		str++;
	}

    gotoXY(x,y); //restore x, y position
}

uint8_t glcd_get_char_width(char c) {

    uint8_t width = 0;
    uint8_t firstChar = FontRead(myDC->FontSettings.Font+FONT_FIRST_CHAR);
    uint8_t charCount = FontRead(myDC->FontSettings.Font+FONT_CHAR_COUNT);

    // read width data
    if(c >= firstChar && c < (firstChar+charCount)) {
        c -= firstChar;
        width = FontRead(myDC->FontSettings.Font+FONT_WIDTH_TABLE+c)+1;
    }

    return width;
}

uint16_t glcd_get_string_width(const char *str) {
    uint16_t width = 0;

    while(*str != 0) {
        width += glcd_get_char_width(*str++);
    }

    return width;
}

uint8_t glcd_get_font_height() {
	return myDC->FontSettings.Height;
}

void writeBuffer(uint8_t data, uint8_t mask)
{
    //uint8_t temp;
    uint8_t yByte;
    uint8_t bitOffset;
    uint16_t x = myDC->Cursor.x;
    uint16_t y = myDC->Cursor.y;
    uint8_t i;
    uint8_t bitmask = 0x01; //Bei D0 anfangen

    if ( (x>GLCD_RIGHT) || (y>GLCD_BOTTOM) ) return;

    yByte = y >> 3; // welche page?

    bitOffset = y%8;

    if(bitOffset != 0)
    {
        if(mask == 0)
        {
            yByte = y; //yByte als Speicher

            for(i=0; i<8; i++)
            {
                if(data&bitmask)
                    putPixel(x,yByte+i);
                else
                    clrPixel(x,yByte+i);
                bitmask<<=1; //mask next bit
            } //for i
            gotoXY(x,y);
        }
        else
        {
            yByte = y;
            for(i=0; i<mask; i++)
            {
                if(data&bitmask)
                    putPixel(x,yByte+i);
                else
                    clrPixel(x,yByte+i);
                bitmask<<=1; //mask next bit
            } //for i
            gotoXY(x, y);
        }
    }
    else
    {
        if(mask == 0)
        {
            //glcdBuffer[yByte][x] = data;
            yByte = y;
            for(i=0; i<8; i++)
            {
                if(data&bitmask)
                    putPixel(x,yByte+i);
                else
                    clrPixel(x,yByte+i);
                bitmask<<=1; //mask next bit
            } //end for
            gotoXY(x, y);
        }
        else
        {
            yByte = y;
            for(i=0; i<mask; i++)
            {
                if(data&bitmask)
                    putPixel(x,yByte+i);
                else
                    clrPixel(x,yByte+i);
                bitmask<<=1; //mask next bit
            } //end for
            gotoXY(x, y);
        }
    }//end else
}
