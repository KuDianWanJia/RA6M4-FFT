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

/*******Ƶ����ʾ********/
typedef enum
{
	Black,		 	//��
	Red,		 		//�� R 001
	Green,		 	//�� G 010
	Yellow,		 	//��
	Blue,		 		//�� B 100
	Purple,		 	//��
	Cyan,		 		//��
	White		 		//��
} Colour_Type;	   //��ɫ����ö��

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

