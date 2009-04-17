#ifndef AP7000_LCD_BG_H_
#define AP7000_LCD_BG_H_

#include <stdint.h>

void VerticalPan_BG(int16_t yoffset);
void SetColor_BG(uint32_t Color);
void SetBackgroundColor_BG(uint32_t Color);
void SetFontColor_BG(uint32_t color);
void PutPixel_BG(uint16_t x1, uint16_t y1);
void ClrPixel_BG(uint16_t x1, uint16_t y1);
void Line_BG(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void PasteRawData_BG(uint8_t* data, uint16_t x, uint16_t y, uint16_t length);
void Update_BG(uint8_t top, uint8_t bottom);

#endif /*AP7000_LCD_BG_H_*/
