#ifndef __OLED_GFX_H
#define __OLED_GFX_H

#include "stdlib.h"

#include "OLED_Driver.h"

typedef unsigned short u16;
typedef unsigned char  u8;

typedef enum  {
  FONT_5X8 = 0,
  FONT_8X16
}FONT_SIZE;

void OLED_GFX(void);
		
/*********************************»­Ïß********************************/
void Draw_Line1(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void Draw_Line2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void Draw_Line3(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8 with, uint16_t color);
		
/********************************»­¾ØÐÎ*******************************/
void Draw_Rect1(int16_t x, int16_t y, int16_t w, int16_t h);
void Draw_Rect2(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void Fill_Rect1(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void Fill_Rect2(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color);
		
/*********************************»­Ô²********************************/
void Draw_Circle1(int16_t x0, int16_t y0, int16_t r);
void Draw_Circle2(int16_t x0, int16_t y0, int16_t r,uint16_t color);
void Draw_Circle3(int16_t x0, int16_t y0, int16_t r,u8 Type,uint16_t color);
void Draw_RoundRect1(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void Draw_RoundRect2(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,uint16_t color);
void Fill_Circle1(int16_t x0, int16_t y0, int16_t r);
void Fill_Circle2(int16_t x0, int16_t y0, int16_t r,uint16_t color);
		
/*******************************»­Èý½ÇÐÎ******************************/
void Draw_Triangle1(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Draw_Triangle2(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color);
    
/*******************************×Ö·ûÏÔÊ¾******************************/
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size, u16 color);
void GRAM_ShowChar(u16 x, u16 y, char chr, u8 size, u16 color);
void GRAM_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p, u16 color);
void GRAM_ShowImage(u16 x, u16 y, u16 width, u16 height, const u8 *p);
		
//Î´Ê¹ÓÃ
void Set_FontSize(FONT_SIZE size);
void Set_TextCoordinate(uint8_t x, uint8_t y);
void Draw_LineS1(int16 *points, uint8 no);
void Draw_LineS2(int16 *points, uint8 no,uint16_t color);
void print_String(uint8_t x, uint8_t y, const uint8_t *text, FONT_SIZE size);
void Display_String_5x8(uint8_t x, uint8_t y, const uint8_t *text);
void Display_String_8x16(uint8_t x, uint8_t y, const uint8_t *text);


void Write_Line1(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void Write_Line2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
    
void DrawCircle_Helper1( int16_t x0, int16_t y0, int16_t r, uint8_t corner);
void DrawCircle_Helper2( int16_t x0, int16_t y0, int16_t r, uint8_t corner,uint16_t color);
void FillCircle_Helper1(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta);
void FillCircle_Helper2(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,uint16_t color);

#endif
