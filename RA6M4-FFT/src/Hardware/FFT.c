#include "FFT.h"
#include "math.h"

#include "OLED_Driver.h"

#include "adc_hal.h"

#define M 7
#define N 128   //(1<<M) 点数 N=2^M
#define PI2 6.28318530717959  // 2π

int32_t COS[N/2];						//放大1024倍余弦值表
int32_t SIN[N/2];						//放大1024倍正弦值表
uint32_t BitRO_tab[N];					//倒位序表

COMPLEX_Type FFT_tab[N];				//用于存储FFT的复数结果
uint32_t FFT_Amp_tab[N/2];				//用于存储FFT的复数结果的模，即幅值谱表

/**/
int32_t fall_pot[N/2];					//漂浮点高度表
Colour_Type fallt_colour_tab[N/2];	    //漂浮点颜色表
Colour_Type FFT_colour_tab[N/2]; 		//谱线柱颜色表
Colour_Type colour_tab[6] = {Red,Yellow,Green,Cyan,Blue,Purple};

/**/

void FFT_Init(void)     //此函数用于生成FFT运算所需的余弦值表、正弦值表和倒位序表
{
	uint32_t i,n,a;

	for(i=0; i < N/2 ;i++)
	{
		COS[i] = (int32_t)round(1024.0 * cos(i*PI2/N));  //生成放大1024倍的余弦值表、正弦值表
		SIN[i] = (int32_t)round(1024.0 * sin(i*PI2/N));
		/************/
		FFT_colour_tab[i] = colour_tab[i%6];             //谱线柱颜色初始化
    fallt_colour_tab[i] = White;                         //漂浮点颜色初始化
	  /************/
	}
	//生成倒位序表
	for(n=0; n<N ;n++)
	{
		a = 0;
		for(i=0;i < M;i++)
		{
			a|=((n>>i)&0x01)<<(M-1-i);   //根据自然序号n生成倒位序序号a
		}
		BitRO_tab[n] = a;
	}	
}

extern uint16_t g_buffer_adc[];
void bit_reversed(void)   // 将ADC_Value[n]按倒位序存进FFT_tab[n]的实部
{
	uint32_t n;
	for(n=0; n<N ;n++)
	{
		FFT_tab[n].Re = (int32_t)(g_buffer_adc[BitRO_tab[n]]-2048);//将ADC采样值去除直流分量，
		FFT_tab[n].Im = 0;										   //实测直流分量为1.65V，换算为采样值是2048，所以减去2048后按倒位序存进FFT_tab[n]的实部。
	}
}

void FFT(void)   //FFT运算
{
	uint32_t L,B,J,k,p;
	COMPLEX_Type temp;

	for(L=1 ; L<=M ; L++)    	//蝶形运算级数L
	{
		B = 1<<(L-1);						//计算旋转因子W_N_p的个数 B = 2^(L-1)
		for(J=0 ; J<B ; J++)
		{
			p = J<<(M-L);					//计算旋转因子W_N_p的指数 p = J*2^(M-L)	
			for(k=J ; k<N ; k += 2*B)
			{
				temp.Re = FFT_tab[k+B].Re * COS[p] + FFT_tab[k+B].Im * SIN[p];
				temp.Im = FFT_tab[k+B].Im * COS[p] - FFT_tab[k+B].Re * SIN[p];	
				temp.Re /=1024;				//由于余弦和正弦值都是原来的1024倍，这里将运算结果除以1024恢复原来的值
				temp.Im /=1024;				//由于余弦和正弦值都是原来的1024倍，这里将运算结果除以1024恢复原来的值
				FFT_tab[k+B].Re = FFT_tab[k].Re - temp.Re;
				FFT_tab[k+B].Im = FFT_tab[k].Im - temp.Im;
				FFT_tab[k].Re = FFT_tab[k].Re + temp.Re;
				FFT_tab[k].Im = FFT_tab[k].Im + temp.Im;				
			}
		}	
	}	
}

void Quantization_of_FFT_results(void)   //求FFT结果的幅值谱及其幅值谱的量化
{
	uint32_t i,j;
	double Amp2;
	
	for(i=0; i<N/2 ;i++)
	{
		Amp2 = 1.0*FFT_tab[i+1].Re * FFT_tab[i+1].Re + 1.0*FFT_tab[i+1].Im * FFT_tab[i+1].Im;
		FFT_Amp_tab[i] = (uint32_t) sqrt( Amp2 );  //求幅值谱
		
		for(j=4 ; j<=62 ; j+=2)		//31级非线性二次函数型幅值谱的量化
		{
			if(FFT_Amp_tab[i]<400)
			{
				FFT_Amp_tab[i]=1;
				break;
			}				
			else if( FFT_Amp_tab[i] >= (j*j*25) && FFT_Amp_tab[i] < ((j+2)*(j+2)*25) )
			{
				FFT_Amp_tab[i] = j/2;
				break;
			}
			else if(j == 62)
			{
				FFT_Amp_tab[i] = 31;
			}
		}
	}	
}

void Display_refresh(void)    //根据幅值谱的量化结果，刷新显示缓冲区
{
	int16_t i,temp,color;
	/*
	for(i=0; i<N/2; i++)
	{
		//画谱线柱		
		temp = FFT_Amp_tab[i]*5;
		Draw_FastVLine2(2*i, 128-temp, 128-temp,RandomColor());
		Draw_FastVLine2(2*i+1, 128-temp, 128-temp,RandomColor());
	}
	*/

	for(i=0; i<N/4; i++)
	{
	    color = RandomColor();
	    //画谱线柱
	    temp = FFT_Amp_tab[i]*6;
	    Draw_FastVLine2(4*i, 128-temp, temp,color);
	    Draw_FastVLine2(4*i+1, 128-temp, temp,color);
	}
}

/*
void Spectrum_color_switching(void)			//频谱颜色切换
{
	uint32_t i;
	static uint32_t n = 0;
	static Colour_Type colour_tab[6] = {Red,Yellow,Green,Cyan,Blue,Purple};
	n++;
	if(n>8) n = 1;
	switch(n)
	{
		case 1:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Red;
							fallt_colour_tab[i] = Cyan;
						} break;
		case 2:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Green;
							fallt_colour_tab[i] = Purple;
						} break;
		case 3:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Yellow;
							fallt_colour_tab[i] = Blue;
						} break;			
		case 4:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Blue;
							fallt_colour_tab[i] = Yellow;
						} break;
		case 5:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Purple;
							fallt_colour_tab[i] = Green;
						} break;
		case 6:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = Cyan;
							fallt_colour_tab[i] = Red;
						} break;
		case 7:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = White;
							fallt_colour_tab[i] = colour_tab[i%6];
						} break;
		case 8:
						for(i=0; i < N/2 ;i++)
						{
							FFT_colour_tab[i] = colour_tab[i%6];
							fallt_colour_tab[i] = White;
						} break;
	}
}
*/
