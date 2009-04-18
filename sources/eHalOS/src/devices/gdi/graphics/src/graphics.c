/**
 * @file graphics.c
 * @brief device independent graphic code
 * @author Team eHalOS
 * @version 0.0.1
 * @date    2008-2009
 * @note Hardware: NGW100, Software: AVR32-GCC
 * @license LGPLv2.1
 */

#include <math.h>

#include "graphis_hw_interface.h"
#include "graphics.h"

#include "font.h"

#include "ap7000_lcd.h"

// Zeiger auf das Interface der aktuellen Grafik
static VerticalPan_t CurrentVPan;
static Line_t CurrentLine;
static PutPixel_t CurrentPutPixel;
static ClrPixel_t CurrentClrPixel;
static SetColor_t CurrentSetColor;
static SetBGColor_t CurrentSetBackgroundColor;
static SetFontColor_t CurrentSetFontColor;
static PasteRawData_t CurrentPasteRawData;
static Update_t CurrentUpdate;

//static void fillBuffer(uint8_t pattern);

void initGraph(void)
{
	init_graphics2();
	//gotoXY(0,0);
}

void setDevice(char DeviceNo)
{
    CurrentDevice = DeviceNo;
    SetDeviceContext(&LCD[CurrentDevice],CurrentDevice);
    CurrentVPan		= LCD[CurrentDevice].Display.VerticalPan;
    CurrentSetColor = LCD[CurrentDevice].Display.SetColor;
    CurrentSetBackgroundColor = LCD[CurrentDevice].Display.SetBackgroundColor;
    CurrentPutPixel = LCD[CurrentDevice].Display.PutPixel;
    CurrentClrPixel = LCD[CurrentDevice].Display.ClrPixel;
    CurrentLine     = LCD[CurrentDevice].Display.Line;
    CurrentPasteRawData = LCD[CurrentDevice].Display.PasteRawData;
    CurrentUpdate   = LCD[CurrentDevice].Display.Update;
    //?.// weitere Funktionen

    //Init Font
    init_font(&LCD[CurrentDevice]);
}

void setColor(color_t Color)
{
    CurrentSetColor(Color);
}

color_t getColor() {
	return LCD[CurrentDevice].Color;
}

void setBackgroundColor(color_t color)
{
	CurrentSetBackgroundColor(color);
}

color_t getBackgroundColor() {
	return LCD[CurrentDevice].BackColor;
}

void setFontColor(uint32_t color)
{
	CurrentSetFontColor(color);
}

void verticalPan(int16_t yoffset) {
	CurrentVPan(yoffset);
}

void putPixel(uint16_t x1, uint16_t y1)
{
    CurrentPutPixel(x1,y1);
}

void clrPixel(uint16_t x1, uint16_t y1)
{
	CurrentClrPixel(x1,y1);
}
void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    CurrentLine(x1,y1,x2,y2);
}

void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    CurrentLine(x1,y1,x2,y1);
    CurrentLine(x1,y2,x2,y2);
    CurrentLine(x1,y1,x1,y2);
    CurrentLine(x2,y1,x2,y2);
}

void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t tmp, y;
	if(x1 > x2){
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if(y1 > y2){
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	for(y=y1;y<y2;y++){
		CurrentLine(x1,y,x2,y);
	}
}

void circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius)
{
    int x, y, r2;

    r2 = radius * radius;
    putPixel(xCenter, yCenter + radius);
    putPixel(xCenter, yCenter - radius);
    putPixel(xCenter + radius, yCenter);
    putPixel(xCenter - radius, yCenter);
    x = 1;
    y = (int) (sqrt((double)(r2 - 1)) + 0.5);

    while (x < y) {
        putPixel(xCenter + x, yCenter + y);
        putPixel(xCenter + x, yCenter - y);
        putPixel(xCenter - x, yCenter + y);
        putPixel(xCenter - x, yCenter - y);
        putPixel(xCenter + y, yCenter + x);
        putPixel(xCenter + y, yCenter - x);
        putPixel(xCenter - y, yCenter + x);
        putPixel(xCenter - y, yCenter - x);
        x += 1;
        y = (int) (sqrt((double)(r2 - x*x)) + 0.5);
    }
    if (x == y) {
        putPixel(xCenter + x, yCenter + y);
        putPixel(xCenter + x, yCenter - y);
        putPixel(xCenter - x, yCenter + y);
        putPixel(xCenter - x, yCenter - y);
    }
}

void box(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t i;

    if (!width) return;

    width--;
    for (i=y;i<y+height;i++){
        line(x, i, x+width, i);
    }
}

void sprite(uint8_t* data, uint16_t height, uint16_t width)
{
	uint16_t x, y;
	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			//TODO check for some colors .. maybe later
			if(data[y*width+x]==0x01)
				CurrentPutPixel(getX()+x,getY()+y);
		}
	}
}

void sprite2(gdi_sprite_t* sprite)
{
	uint16_t x, y;
	uint16_t width=0;
	uint16_t height=0;
	uint16_t gx=0;
	uint16_t gy=0;
	uint8_t* data=0;
	gx = sprite->x;
	gy = sprite->y;

	width = sprite->width;
	height = sprite->height;

	data = sprite->data;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			//TODO check for some colors .. maybe later
			if(data[y*width+x]==0x01)
				CurrentPutPixel(gx+x,gy+y);
		}
	}
}

void color_sprite(gdi_sprite_t* sprite){
	uint16_t y;
	uint16_t width=0;
	uint16_t height=0;
	uint8_t* data=0;
	uint8_t* dataOffset=0;
	int offset = 0;
	width = sprite->width;
	height = sprite->height;

	data = sprite->data;

	for(y=0; y<height; y++)
	{
		offset = y*width*3;
		dataOffset = data+offset;
		CurrentPasteRawData(dataOffset,sprite->x,sprite->y+y,sprite->width);
	}
}


void update(uint16_t top, uint16_t bottom)
{
    CurrentUpdate(top,bottom);
}

void updateAll()
{
    update(0,LCD[CurrentDevice].Display.MaxY-1);
}

void gotoXY(uint16_t x, uint16_t y)
{
    LCD[CurrentDevice].Cursor.x = x;
    LCD[CurrentDevice].Cursor.y = y;
}

void setX(uint16_t x)
{
    LCD[CurrentDevice].Cursor.x = x;
}

uint16_t getX(void)
{
    return LCD[CurrentDevice].Cursor.x;
}

void setY(uint16_t y)
{
    LCD[CurrentDevice].Cursor.y = y;
}

uint16_t getY(void)
{
    return LCD[CurrentDevice].Cursor.y;
}

uint16_t getScreenWidth(){
	return LCD[CurrentDevice].Display.MaxX;
}

uint16_t getScreenHeight(){
	return LCD[CurrentDevice].Display.MaxY;
}

void clrScr(uint8_t bufferAndDisplay)
{
	CurrentSetColor(BLACK);
	CurrentSetBackgroundColor(BLACK);
	box(GLCD_LEFT, GLCD_TOP, GLCD_RIGHT,GLCD_BOTTOM );
    //fillBuffer(0x00);
    //if ( bufferAndDisplay ) update(GLCD_TOP,GLCD_BOTTOM);
}

//void fillBuffer(uint8_t pattern)
//{
//    uint16_t x,y, index;
//
//    index=0;
//
//    for (y=0;y<GLCD_Y_BYTES;y++)
//    {
//        for (x=0;x<GLCD_X_BYTES;x++)
//        {
//            index = x + (((y*8)>>3) << 7);
//            LCD[CurrentDevice].Display.Buffer[index] = pattern;
//        }
//    }
//}

extern void setLineThickness(uint8_t thickness) {
	LCD[CurrentDevice].LineSettings.thickness = thickness;
}
extern void setLineStyle(line_style_t linestyle) {
	LCD[CurrentDevice].LineSettings.linestyle = linestyle;
}
