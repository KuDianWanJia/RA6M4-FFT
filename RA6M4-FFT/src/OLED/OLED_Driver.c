#include "OLED_Driver.h"

uint8_t color_byte[2]={0XFF,0X00},color_Damp_byte[2]={0XFF,0X00};
uint8_t OLED_GRAM[256][128];

//根据输入的RGB(5:6:5)参数生成颜色
u16 ConvertColor(u8 red,u8 green,u8 blue)
{
	return red<<11|green<<6|blue;
}

//生成随机颜色
uint16_t RandomColor(void){
	
	uint8_t red,green,blue;
	red = rand();
	green = rand();
	blue = rand();
	return (red<<11|green<<5|blue);
}

//控制颜色
void Set_DampColor(uint16_t color)  {
  
  color_Damp_byte[0] = (uint8_t)(color >> 8);
  color_Damp_byte[1] = (uint8_t)(color & 0x00ff);
}

//写命令
void Write_Command(uint8_t cmd)  {
	//OLED_A0=OLED_CMD;
    R_PORT4->PODR_b.PODR5=OLED_CMD;
    __ASM("NOP");__ASM("NOP");
    //OLED_CS=LOW;
    R_PORT4->PODR_b.PODR3=LOW;

	//DATAOUT(cmd);
	R_PORT1->PODR = (uint16_t)cmd;
	__ASM("NOP");__ASM("NOP");
	//OLED_CS=HIGH;
	R_PORT4->PODR_b.PODR3=HIGH;
}

//写一字节数据(8位并行)
void Write_Data(uint8_t dat) {
 
	//OLED_A0=OLED_DATA;
    R_PORT4->PODR_b.PODR5=OLED_DATA;
    __ASM("NOP");__ASM("NOP");
    //OLED_CS=LOW;
    R_PORT4->PODR_b.PODR3=LOW;

	//DATAOUT(dat);
    R_PORT1->PODR = (uint16_t)dat;
    __ASM("NOP");__ASM("NOP");
	//OLED_CS=HIGH;
    R_PORT4->PODR_b.PODR3=HIGH;
}
//写指定长度数据
void Write_Data2(uint8_t* dat_p, uint16_t length) {
	int i;
  for(i=0;i<length;i++)
		Write_Data(dat_p[i]);
}
//写半字数据(16位)-传入一个16位数据显示一个点
void Write_HalfWord(uint16_t dat) {
 
	u8 data[2] = {0};

  data[0] = dat >> 8;
  data[1] = dat;
	
	Write_Data(data[0]);
	Write_Data(data[1]);
}

//设置坐标-(x,y)坐标到(128,128)坐标,
void Set_Coordinate(uint16_t x, uint16_t y)  {

  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) 
    return;
  //Set x and y coordinate
  Write_Command(SSD1351_CMD_SETCOLUMN);
  Write_Data(x);
  Write_Data(SSD1351_WIDTH-1);
  Write_Command(SSD1351_CMD_SETROW);
  Write_Data(y);
  Write_Data(SSD1351_HEIGHT-1);
  Write_Command(SSD1351_CMD_WRITERAM);
}
//设置坐标-(x1,y1)坐标到(x2,y2)坐标
void Set_Coordinate2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)  {

  if ((x2 >= SSD1351_WIDTH) || (y2 >= SSD1351_HEIGHT)) 
    return;
  //Set x and y coordinate
  Write_Command(SSD1351_CMD_SETCOLUMN);
  Write_Data(x1);
  Write_Data(x2);
  Write_Command(SSD1351_CMD_SETROW);
  Write_Data(y1);
  Write_Data(y2);
  Write_Command(SSD1351_CMD_WRITERAM);
}
//设置GRAM刷新坐标-(0,0)坐标到(128,128)坐标
void RAM_Address(void)  {
  
  Write_Command(0x15);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75);
  Write_Data(0x00);
  Write_Data(0x7f);
}
//刷新屏幕-每循环一次(共循环128次)刷新2byte-16bit(对应RGB(5:6:5)一个像素点的16位)
void Refrash_Screen(void)  {
  
  int i,j;
  RAM_Address();
  Write_Command(0x5C);
  for(i=0;i<128;i++)  {
    for(j=0;j<128;j++)  {
      Write_Data(OLED_GRAM[j*2][i]);//RAM data clear
      Write_Data(OLED_GRAM[j*2+1][i]);//RAM data clear
    }
  }
}
//清屏
void Clear_Screen(void)  {
  
  int i,j;
  for(i=0;i<128;i++)  {
    for(j=0;j<128;j++)  {
      OLED_GRAM[2*i][j] = 0;
      OLED_GRAM[2*i+1][j] = 0;
    }
  }
}
 
//镜像显示
void  Invert(bool v) {
  
  if (v)
    Write_Command(SSD1351_CMD_INVERTDISPLAY);
  else
    Write_Command(SSD1351_CMD_NORMALDISPLAY);
}

//画像素点-修改GRAM
void Draw_Pixel(int16_t x, int16_t y)
{
  // Bounds check.
  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) return;
  if ((x < 0) || (y < 0)) return;

	OLED_GRAM[2*x][y] = color_byte[0];
	OLED_GRAM[2*x+1][y] = color_byte[1];
}

//画像素点-修改GRAM,带颜色
void Draw_Pixel2(int16_t x, int16_t y,uint16_t color)
{
  // Bounds check.
  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) return;
  if ((x < 0) || (y < 0)) return;

	OLED_GRAM[2*x][y] = (uint8_t)(color >> 8);
	OLED_GRAM[2*x+1][y] = (uint8_t)(color & 0x00ff); 
} 
 
//画一条水平线，忽略任何屏幕旋转-通过画点修改GRAM
void Draw_FastHLine(int16_t x, int16_t y, int16_t length) {
  //Bounds check
	int16_t x0=x;
  do
  {
    Draw_Pixel(x, y);     // 逐点显示，描出垂直线
    x++;
  }
  while(x0+length>=x);
}

//画一条水平线，忽略任何屏幕旋转-通过画点修改GRAM,带颜色
void Draw_FastHLine2(int16_t x, int16_t y, int16_t length,uint16_t color) {
  //Bounds check
	int16_t x0=x;
  do
  {
    Draw_Pixel2(x, y,color);// 逐点显示，描出垂直线
    x++;
  }
  while(x0+length>=x);
}
  
//画一条竖线，忽略任何屏幕旋转-通过画点修改GRAM
void Draw_FastVLine(int16_t x, int16_t y, int16_t length)  {
  //Bounds check
	int16_t y0=y;
  do
  {
    Draw_Pixel(x, y);   // 逐点显示，描出垂直线
    y++;
  }
  while(y0+length>=y);
}

//画一条竖线，忽略任何屏幕旋转-通过画点修改GRAM,带颜色
void Draw_FastVLine2(int16_t x, int16_t y, int16_t length,uint16_t color)  {
  //Bounds check
	int16_t y0=y;
  do
  {
    Draw_Pixel2(x, y,color);   // 逐点显示，描出垂直线
    y++;
  }
  while(y0+length>=y);
}

void Device_Init(void) {

  //OLED_CS = LOW;
  R_PORT4->PODR_b.PODR3 = LOW;
  __ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");
  //OLED_WR=LOW;
  R_PORT4->PODR_b.PODR6=LOW;
  __ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");

  //OLED_RST = LOW;
  R_PORT4->PODR_b.PODR4 = LOW;
  rt_thread_mdelay(40);
  //OLED_RST = HIGH;
  R_PORT4->PODR_b.PODR4 = HIGH;
  rt_thread_mdelay(20);

  Write_Command(0xfd);	// command lock
  Write_Data(0x12);
  Write_Command(0xfd);	// command lock
  Write_Data(0xB1);

  Write_Command(0xae);	// display off
  Write_Command(0xa4); 	// Normal Display mode

  Write_Command(0x15);	//set column address
  Write_Data(0x00);     //column address start 00
  Write_Data(0x7f);     //column address end 95
  Write_Command(0x75);	//set row address
  Write_Data(0x00);     //row address start 00
  Write_Data(0x7f);     //row address end 63	

  Write_Command(0xB3);
  Write_Data(0xF1);

  Write_Command(0xCA);	
  Write_Data(0x7F);

  Write_Command(0xa0);  //set re-map & data format
  Write_Data(0x74);     //Horizontal address increment

  Write_Command(0xa1);  //set display start line
  Write_Data(0x00);     //start 00 line

  Write_Command(0xa2);  //set display offset
  Write_Data(0x00);

  Write_Command(0xAB);	
  Write_Command(0x01);	

  Write_Command(0xB4);	
  Write_Data(0xA0);	  
  Write_Data(0xB5);  
  Write_Data(0x55);    

  Write_Command(0xC1);	
  Write_Data(0xC8);	
  Write_Data(0x80);
  Write_Data(0xC0);

  Write_Command(0xC7);	
  Write_Data(0x0F);

  Write_Command(0xB1);	
  Write_Data(0x32);

  Write_Command(0xB2);	
  Write_Data(0xA4);
  Write_Data(0x00);
  Write_Data(0x00);

  Write_Command(0xBB);	
  Write_Data(0x17);

  Write_Command(0xB6);
  Write_Data(0x01);

  Write_Command(0xBE);
  Write_Data(0x05);

  Write_Command(0xA6);

  Clear_Screen();
  Refrash_Screen();
  Write_Command(0xaf);	 //display on
}
