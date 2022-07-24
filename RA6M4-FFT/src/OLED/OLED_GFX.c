#include "OLED_GFX.h"

#include "OLED_Font.h"

uint8_t Row,Column;
FONT_SIZE Font_Size;

void OLED_GFX(void)  {
  Row = 0;
  Column = 0;
  Font_Size = FONT_5X8;
}

/*********************************����********************************/
//��������-�޸�GRAM
void Write_Line1(int16_t x0, int16_t y0, int16_t x1, int16_t y1)  {
    
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    
  if (steep)  {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1)  {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if(y0 < y1) {
    ystep = 1;
  } 
  else  {
    ystep = -1;
  }

  for(; x0<=x1; x0++) {
    if(steep)
      Draw_Pixel(y0, x0);
    else
      Draw_Pixel(x0, y0);
    err -= dy;
      
    if(err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
//��������-�޸�GRAM,����ɫ
void Write_Line2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color)  {
    
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    
  if (steep)  {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1)  {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if(y0 < y1) {
    ystep = 1;
  } 
  else  {
    ystep = -1;
  }

  for(; x0<=x1; x0++) {
    if(steep)
      Draw_Pixel2(y0, x0,color);
    else
      Draw_Pixel2(x0, y0,color);
    err -= dy;
      
    if(err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
//��������-�޸�GRAM
void Draw_Line1(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  // Update in subclasses if desired!
  if(x0 == x1)  {
    if(y0 > y1) swap(y0, y1);
    Draw_FastVLine(x0, y0, y1 - y0 );
  } 
  else if(y0 == y1) {
    if(x0 > x1)
      swap(x0, x1);
    Draw_FastHLine(x0, y0, x1 - x0 );
  }
  else
    Write_Line1(x0, y0, x1, y1);
}
//��������-�޸�GRAM,����ɫ
void Draw_Line2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {
  // Update in subclasses if desired!
  if(x0 == x1)  {
    if(y0 > y1) swap(y0, y1);
    Draw_FastVLine2(x0, y0, y1 - y0 ,color);
  } 
  else if(y0 == y1) {
    if(x0 > x1)
      swap(x0, x1);
    Draw_FastHLine2(x0, y0, x1 - x0 ,color);
  }
  else
    Write_Line2(x0, y0, x1, y1,color);
}
//��������-�޸�GRAM,����ɫ�Ϳ��
void  Draw_Line3(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8 with, uint16_t color)
{  int32   dx;						// ֱ��x���ֵ����
   int32   dy;          	// ֱ��y���ֵ����
   int16   dx_sym;				// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
   int16   dy_sym;				// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
   int32   dx_x2;					// dx*2ֵ���������ڼӿ������ٶ�
   int32   dy_x2;					// dy*2ֵ���������ڼӿ������ٶ�
   int32   di;						// ���߱���
   
   int32   wx, wy;				// �߿����
   int32   draw_a, draw_b;
   
   /* �������� */
   if(with==0) return;
   if(with>50) with = 50;
   
   dx = x1-x0;						// ��ȡ����֮��Ĳ�ֵ
   dy = y1-y0;
   
   wx = with/2;
   wy = with-wx-1;
   
   /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
   if(dx>0)							// �ж�x�᷽��
     dx_sym = 1;				// dx>0������dx_sym=1
   else
   {  
	    if(dx<0)  dx_sym = -1;	// dx<0������dx_sym=-1
      else
      {  /* dx==0������ֱ�ߣ���һ�� */
         wx = x0-wx;
         if(wx<0) wx = 0;
         wy = x0+wy;        
         while(1)
         {  
					 x0 = wx;
						if(y0 > y1) swap(y0, y1);
            Draw_FastVLine2(x0, y0, y1 - y0, color);
            if(wx>=wy) break;
            wx++;
         }   
      	 return;
      }
   }   
   if(dy>0)							// �ж�y�᷽��
     dy_sym = 1;					// dy>0������dy_sym=1
   else
   {  
		 if(dy<0)
       dy_sym = -1;				// dy<0������dy_sym=-1
     else
     {  /* dy==0����ˮƽ�ߣ���һ�� */
         wx = y0-wx;
         if(wx<0) wx = 0;
         wy = y0+wy;   
         while(1)
         {  
					 y0 = wx;
					 if(x0 > x1) swap(x0, x1);
					 Draw_FastHLine2(x0, y0, x1 - x0 ,color);
           if(wx>=wy) break;
           wx++;
         }
      	 return;
     }
   }    
   /* ��dx��dyȡ����ֵ */
   dx = dx_sym * dx;
   dy = dy_sym * dy; 
   /* ����2����dx��dyֵ */
   dx_x2 = dx*2;
   dy_x2 = dy*2;   
   /* ʹ��Bresenham�����л�ֱ�� */
   if(dx>=dy)			// ����dx>=dy����ʹ��x��Ϊ��׼
   {  
		 di = dy_x2 - dx;
     while(x0!=x1)
     {  /* x����������������y���򣬼�����ֱ�� */
        draw_a = y0-wx;
        if(draw_a<0) draw_a = 0;
        draw_b = y0+wy;
				if(draw_a > draw_b)  swap(draw_a, draw_b);
        Draw_FastVLine2(x0, draw_a, draw_b - draw_a, color);
         
        x0 += dx_sym;				
        if(di<0) di += dy_x2;			// �������һ���ľ���ֵ
        else
        {  
					di += dy_x2 - dx_x2;
          y0 += dy_sym;
        }
     }
     draw_a = y0-wx;
     if(draw_a<0) draw_a = 0;
     draw_b = y0+wy;
		 if(draw_a > draw_b) swap(draw_a, draw_b);
     Draw_FastVLine2(x0, draw_a, draw_b - draw_a, color);
   }
   else			// ����dx<dy����ʹ��y��Ϊ��׼
   {  
		 di = dx_x2 - dy;
     while(y0!=y1)
     {  /* y����������������x���򣬼���ˮƽ�� */
        draw_a = x0-wx;
        if(draw_a<0) draw_a = 0;
        draw_b = x0+wy;
				if(draw_a > draw_b) swap(draw_a, draw_b);
        Draw_FastHLine2(draw_a, y0, draw_b - draw_a, color);
         
        y0 += dy_sym;
        if(di<0)  di += dx_x2;
        else
        {  
					di += dx_x2 - dy_x2;
          x0 += dx_sym;
        }
     }
     draw_a = x0-wx;
     if(draw_a<0) draw_a = 0;
     draw_b = x0+wy;
		 if(draw_a > draw_b) swap(draw_a, draw_b);
     Draw_FastHLine2(draw_a, y0, draw_b - draw_a, color);
   }  
}
/*********************************************************************/

/********************************������*******************************/
//����������-�޸�GRAM
void Draw_Rect1(int16_t x, int16_t y, int16_t w, int16_t h) {
    
  Draw_FastHLine(x, y, w);
  Draw_FastHLine(x, y+h, w);
  Draw_FastVLine(x, y, h);
  Draw_FastVLine(x+w, y, h);
}
//����������-�޸�GRAM,����ɫ
void Draw_Rect2(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
    
  Draw_FastHLine2(x, y, w, color);
  Draw_FastHLine2(x, y+h, w, color);
  Draw_FastVLine2(x, y, h, color);
  Draw_FastVLine2(x+w, y, h, color);
}
/*
 *@brief ��������-�޸�GRAM
 */
void Fill_Rect1(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

	uint16_t i,j;
  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT))
  return;
  // Y bounds check
  if (y+h > SSD1351_HEIGHT) {
    h = SSD1351_HEIGHT - y;
  }
  // X bounds check
  if (x+w > SSD1351_WIDTH)  {
    w = SSD1351_WIDTH - x;
  }
	for(i=x;i<=w+x;i++)
		for(j=y;j<=h+y;j++)
			Draw_Pixel(i,j);
}
/*
 *@brief ��������-�޸�GRAM,����ɫ
 */
void Fill_Rect2(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color) {

	uint16_t i,j;
  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT))
  return;
  // Y bounds check
  if (y+h > SSD1351_HEIGHT) {
    h = SSD1351_HEIGHT - y ;
  }
  // X bounds check
  if (x+w > SSD1351_WIDTH)  {
    w = SSD1351_WIDTH - x ;
  }
	for(i=x;i<=w+x;i++)
		for(j=y;j<=h+y;j++)
			Draw_Pixel2(i,j, color);
}
/*********************************************************************/

/*********************************��Բ********************************/
//��һ��Բ����-�޸�GRAM
void Draw_Circle1(int16_t x0, int16_t y0, int16_t r)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  Draw_Pixel(x0  , y0+r);
  Draw_Pixel(x0  , y0-r);
  Draw_Pixel(x0+r, y0  );
  Draw_Pixel(x0-r, y0  );

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    Draw_Pixel(x0 + x, y0 + y);
    Draw_Pixel(x0 - x, y0 + y);
    Draw_Pixel(x0 + x, y0 - y);
    Draw_Pixel(x0 - x, y0 - y);
    Draw_Pixel(x0 + y, y0 + x);
    Draw_Pixel(x0 - y, y0 + x);
    Draw_Pixel(x0 + y, y0 - x);
    Draw_Pixel(x0 - y, y0 - x);
  }
}
//��һ��Բ����-�޸�GRAM,����ɫ
void Draw_Circle2(int16_t x0, int16_t y0, int16_t r,uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  Draw_Pixel2(x0  , y0+r, color);
  Draw_Pixel2(x0  , y0-r, color);
  Draw_Pixel2(x0+r, y0  , color);
  Draw_Pixel2(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    Draw_Pixel2(x0 + x, y0 + y, color);
    Draw_Pixel2(x0 - x, y0 + y, color);
    Draw_Pixel2(x0 + x, y0 - y, color);
    Draw_Pixel2(x0 - x, y0 - y, color);
    Draw_Pixel2(x0 + y, y0 + x, color);
    Draw_Pixel2(x0 - y, y0 + x, color);
    Draw_Pixel2(x0 + y, y0 - x, color);
    Draw_Pixel2(x0 - y, y0 - x, color);
  }
}
//��һ��Բ�������ķ�֮һ-�޸�GRAM,����ɫ
void Draw_Circle3(int16_t x0, int16_t y0, int16_t r,uint8 Type,uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
	switch(Type)
	{
		case 1:
				Draw_Pixel2(x0  , y0+r, color);
				Draw_Pixel2(x0+r, y0  , color);break;//����
		case 2:
				Draw_Pixel2(x0  , y0+r, color);
				Draw_Pixel2(x0-r, y0  , color);break;//����
		case 3:
				Draw_Pixel2(x0  , y0-r, color);
				Draw_Pixel2(x0+r, y0  , color);break;//����
		case 4:
				Draw_Pixel2(x0-r, y0  , color);
				Draw_Pixel2(x0  , y0-r, color);break;//����
	}
  while (x<y) 
	{
    if (f >= 0)
		{
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

	  switch(Type)
	  {
	  	case 1:
        Draw_Pixel2(x0 + x, y0 + y, color);
        Draw_Pixel2(x0 + y, y0 + x, color);break;
		  case 2:
        Draw_Pixel2(x0 - x, y0 + y, color);
        Draw_Pixel2(x0 - y, y0 + x, color);break;
		  case 3:
        Draw_Pixel2(x0 + x, y0 - y, color);
        Draw_Pixel2(x0 + y, y0 - x, color);break;
		  case 4:
        Draw_Pixel2(x0 - y, y0 - x, color);
        Draw_Pixel2(x0 - x, y0 - y, color);break;
	  }	
  }
}
/*����Բ����~�ķ�֮һԲ����-�޸�GRAM
 *corner����������λ��ÿһλ����Բ���ķ�֮һ,0x0fΪ��Բ
 */
void DrawCircle_Helper1( int16_t x0, int16_t y0, int16_t r, uint8_t corner)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (corner & 0x4) {
      Draw_Pixel(x0 + x, y0 + y);
      Draw_Pixel(x0 + y, y0 + x);
    }
    if (corner & 0x2) {
      Draw_Pixel(x0 + x, y0 - y);
      Draw_Pixel(x0 + y, y0 - x);
    }
    if (corner & 0x8) {
      Draw_Pixel(x0 - y, y0 + x);
      Draw_Pixel(x0 - x, y0 + y);
    }
    if (corner & 0x1) {
      Draw_Pixel(x0 - y, y0 - x);
      Draw_Pixel(x0 - x, y0 - y);
    }
  }
}
/*����Բ����~�ķ�֮һԲ����-�޸�GRAM,����ɫ
 *corner����������λ��ÿһλ����Բ���ķ�֮һ,0x0fΪ��Բ
 */
void DrawCircle_Helper2( int16_t x0, int16_t y0, int16_t r, uint8_t corner,uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (corner & 0x4) {
      Draw_Pixel2(x0 + x, y0 + y, color);
      Draw_Pixel2(x0 + y, y0 + x, color);
    }
    if (corner & 0x2) {
      Draw_Pixel2(x0 + x, y0 - y, color);
      Draw_Pixel2(x0 + y, y0 - x, color);
    }
    if (corner & 0x8) {
      Draw_Pixel2(x0 - y, y0 + x, color);
      Draw_Pixel2(x0 - x, y0 + y, color);
    }
    if (corner & 0x1) {
      Draw_Pixel2(x0 - y, y0 - x, color);
      Draw_Pixel2(x0 - x, y0 - y, color);
    }
  }
}
//��һ��Բ�Ǿ�������
void Draw_RoundRect1(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) {
    // smarter version

    Draw_FastHLine(x+r  , y    , w-2*r); // Top
    Draw_FastHLine(x+r  , y+h-1, w-2*r); // Bottom
    Draw_FastVLine(x    , y+r  , h-2*r); // Left
    Draw_FastVLine(x+w-1, y+r  , h-2*r); // Right
    // draw four corners
    DrawCircle_Helper1(x+r    , y+r    , r, 1);
    DrawCircle_Helper1(x+w-r-1, y+r    , r, 2);
    DrawCircle_Helper1(x+w-r-1, y+h-r-1, r, 4);
    DrawCircle_Helper1(x+r    , y+h-r-1, r, 8);
}
//��һ��Բ�Ǿ�������-����ɫ
void Draw_RoundRect2(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,uint16_t color)
{
    // smarter version
    Draw_FastHLine2(x+r  , y    , w-2*r, color); // Top
    Draw_FastHLine2(x+r  , y+h-1, w-2*r, color); // Bottom
    Draw_FastVLine2(x    , y+r  , h-2*r, color); // Left
    Draw_FastVLine2(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    DrawCircle_Helper2(x+r    , y+r    , r, 1, color);
    DrawCircle_Helper2(x+w-r-1, y+r    , r, 2, color);
    DrawCircle_Helper2(x+w-r-1, y+h-r-1, r, 4, color);
    DrawCircle_Helper2(x+r    , y+h-r-1, r, 8, color);
}

/*��Բ�λ�Բ��
 *corner����: 1-ֻ��ʾ�Ұ�� 2-ֻ��ʾ���� 3-���Ҷ���ʾ
 *delta���� ��0-Բ��  >0-Բ����������
 */
void FillCircle_Helper1(int16_t x0, int16_t y0, int16_t r, uint8_t corner, int16_t delta)  {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while(x<y)  {
    if(f >= 0)  {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if(corner & 0x1)  {
      Draw_FastVLine(x0+x, y0-y, 2*y+delta);
      Draw_FastVLine(x0+y, y0-x, 2*x+delta);
    }
    if(corner & 0x2)  {
      Draw_FastVLine(x0-x, y0-y, 2*y+delta);
      Draw_FastVLine(x0-y, y0-x, 2*x+delta);
    }
  }
}
/*��Բ�λ�Բ��-����ɫ
 *corner����: 1-ֻ��ʾ�Ұ�� 2-ֻ��ʾ���� 3-���Ҷ���ʾ
 *delta���� ��0-Բ��  >0-Բ����������
 */
void FillCircle_Helper2(int16_t x0, int16_t y0, int16_t r, uint8_t corner, int16_t delta,uint16_t color)  {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while(x<y)  {
    if(f >= 0)  {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if(corner & 0x1)  {
      Draw_FastVLine2(x0+x, y0-y, 2*y+delta, color);
      Draw_FastVLine2(x0+y, y0-x, 2*x+delta, color);
    }
    if(corner & 0x2)  {
      Draw_FastVLine2(x0-x, y0-y, 2*y+delta, color);
      Draw_FastVLine2(x0-y, y0-x, 2*x+delta, color);
    }
  }
}

//�����Բ
void Fill_Circle1(int16_t x0, int16_t y0, int16_t r) {
  
  Draw_FastVLine(x0, y0-r, 2*r);
  FillCircle_Helper1(x0, y0, r, 3, 0);
}
//�����Բ-����ɫ
void Fill_Circle2(int16_t x0, int16_t y0, int16_t r,uint16_t color) {
  
  Draw_FastVLine2(x0, y0-r, 2*r, color);
  FillCircle_Helper2(x0, y0, r, 3, 0, color);
}
/*********************************************************************/

/*******************************��������******************************/
//������������-
void Draw_Triangle1(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  Draw_Line1(x0, y0, x1, y1);
  Draw_Line1(x1, y1, x2, y2);
  Draw_Line1(x2, y2, x0, y0);
}

//������������-����ɫ
void Draw_Triangle2(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color)
{
  Draw_Line2(x0, y0, x1, y1, color);
  Draw_Line2(x1, y1, x2, y2, color);
  Draw_Line2(x2, y2, x0, y0, color);
}

/**
 * @brief	��ʾһ��ASCII���ַ�-ֱ����ʾ����ɫ
 *
 * @param   x,y		��ʾ��ʼ����
 * @param   chr		��Ҫ��ʾ���ַ�
 * @param   size	�����С(֧��16/24/32������)
 *
 * @return  void
 */
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size, u16 color)
{
    u8 temp, t1, t;
    u8 csize;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    u16 colortemp;
    u8 sta;

    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

    if((x > (SSD1351_WIDTH - size / 2)) || (y > (SSD1351_WIDTH - size)))	return;

    Set_Coordinate2(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == 16) || (size == 32) )	//16��32������
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
        for(t = 0; t < csize; t++)
        {
            if(size == 16)temp = asc2_1608[(u8)chr][t];	//����1608����
            else if(size == 32)temp = asc2_3216[(u8)chr][t];	//����3216����
            else return;			//û�е��ֿ�

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = 0xff00;
                else colortemp = 0x0000;

                Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
	  else if  (size == 12)	//12������
	  {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[(u8)chr][t];
            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = 0xff00;
                else colortemp = 0xff00;

                Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
    else if(size == 24)		//24������
    {
        csize = (size * 16) / 8;
        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[(u8)chr][t];

            if(t % 2 == 0)sta = 8;
            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) colortemp = 0xff00;
                else colortemp = 0xff00;

                Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief	��ʾһ��ASCII���ַ�-GRAM��ʾ����ɫ
 *
 * @param   x,y		��ʾ��ʼ����
 * @param   chr		��Ҫ��ʾ���ַ�
 * @param   size	�����С(֧��16/24/32������)
 *
 * @return  void
 */
void GRAM_ShowChar(u16 x, u16 y, char chr, u8 size, u16 color)
{
  u8 temp, t1, t;
  u8 csize;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
  u8 sta;

  chr = chr - ' '; //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
  if((x > (SSD1351_WIDTH - size / 2)) || (y > (SSD1351_WIDTH - size)))	return;

  if((size == 16) || (size == 32) )	//16��32������
  {
    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    for(t = 0; t < csize; t++)
    {
      if(size == 16) temp = asc2_1608[(u8)chr][t];	//����1608����
      else if(size == 32) temp = asc2_3216[(u8)chr][t];	//����3216����
      else return;			//û�е��ֿ�

      for(t1 = 0; t1 < 8; t1++)
      {
        if(temp & 0x80) 
				{		
					if(size == 16) Draw_Pixel2(x+t1,y+t,color);
					if(size == 32 && t%2)  Draw_Pixel2(x+t1+8,y+t/2,color);
					else  Draw_Pixel2(x+t1,y+t/2,color);
				}					
				
        temp <<= 1;
      }
    }
  }
	else if  (size == 12)	//12������
	{
    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    for(t = 0; t < csize; t++)
    {
      temp = asc2_1206[(u8)chr][t];
      for(t1 = 0; t1 < 6; t1++)
      {
        if(temp & 0x80) Draw_Pixel2(x+t1,y+t,color);
        temp <<= 1;
      }
    }
  }
  else if(size == 24)		//24������
  {
    csize = (size * 16) / 8;
    for(t = 0; t < csize; t++)
    {
      temp = asc2_2412[(u8)chr][t];
      if(t % 2 == 0) sta = 8;
      else sta = 4;
			
      for(t1 = 0; t1 < sta; t1++)
      {
        if(temp & 0x80) Draw_Pixel2(x+t1,y+t,color);
        temp <<= 1;
      }
    }
  }
}

/**
 * @brief	��ʾ�ַ���-GRAM��ʾ����ɫ
 *
 * @param   x,y		�������
 * @param   width	�ַ���ʾ������
 * @param   height	�ַ���ʾ����߶�
 * @param   size	�����С
 * @param   p		�ַ�����ʼ��ַ
 *
 * @return  void
 */
void GRAM_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p, u16 color)
{
  u8 x0 = x;
  width += x;
  height += y;

  while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
  {
    if(x >= width)          //������ʾ
    {
      x = x0;
      y += size;
    }
    if(y >= height)  break; //�޶�����д���˳�

    GRAM_ShowChar(x, y, *p, size, color);
    x += size / 2;
    p++;
  }
}

/**
 * @brief	 ��ʾͼƬ-GRAM��ʾ
 *
 * @remark Image2Lcdȡģ��ʽ��	C��������/ˮƽɨ��/16λ���ɫ(RGB565)/��λ��ǰ		�����Ĳ�Ҫѡ
 *
 * @param  x,y		�������
 * @param  width	ͼƬ���
 * @param  height	ͼƬ�߶�
 * @param  p		  ͼƬ����������ʼ��ַ
 *
 * @return  void
 */
void GRAM_ShowImage(u16 x, u16 y, u16 width, u16 height, const u8 *p)
{
	u8 t1, t;
	
  if(x + width > SSD1351_WIDTH || y + height > SSD1351_HEIGHT) return;
	
	for(t = 0; t < height; t++)
  {
    for(t1 = 0; t1 < width; t1++)
    {
        OLED_GRAM[x+2*t1][y+t] = p[t*2*width+2*t1]; 
			  OLED_GRAM[x+(2*t1+1)][y+t] = p[t*2*width+(2*t1+1)];
			
			  OLED_GRAM[2*t1][t] = p[t*2*(width+1)+2*t1]; 
			  OLED_GRAM[(2*t1+1)][t] = p[t*2*(width+1)+(2*t1+1)];
    }
  }
}
