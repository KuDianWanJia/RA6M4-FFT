#ifndef __FFT_H
#define	__FFT_H

#include "hal_data.h"

typedef struct
{
	int32_t Re;
	int32_t Im;	
} COMPLEX_Type;

void FFT_Init(void);
void bit_reversed(void);
void FFT(void);
void Quantization_of_FFT_results(void);

/*******频谱显示********/
typedef enum
{
	Black,		 	//黑
	Red,		 		//红 R 001
	Green,		 	//绿 G 010
	Yellow,		 	//黄
	Blue,		 		//蓝 B 100
	Purple,		 	//紫
	Cyan,		 		//青
	White		 		//白
} Colour_Type;	   //颜色种类枚举

typedef struct
{
	uint32_t RDbuf[64];
	uint32_t GDbuf[64];
	uint32_t BDbuf[64];
} DISBUF_Type;

void Display_refresh(void);
void Spectrum_color_switching(void);
/***********************/

#endif /* __FFT_H */

