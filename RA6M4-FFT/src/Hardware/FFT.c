#include "FFT.h"
#include "math.h"

#include "OLED_Driver.h"

#include "adc_hal.h"

#define M 7
#define N 128   //(1<<M) ���� N=2^M
#define PI2 6.28318530717959  // 2��

int32_t COS[N/2];						//�Ŵ�1024������ֵ��
int32_t SIN[N/2];						//�Ŵ�1024������ֵ��
uint32_t BitRO_tab[N];					//��λ���

COMPLEX_Type FFT_tab[N];				//���ڴ洢FFT�ĸ������
uint32_t FFT_Amp_tab[N/2];				//���ڴ洢FFT�ĸ��������ģ������ֵ�ױ�

/**/
int32_t fall_pot[N/2];					//Ư����߶ȱ�
Colour_Type fallt_colour_tab[N/2];	    //Ư������ɫ��
Colour_Type FFT_colour_tab[N/2]; 		//��������ɫ��
Colour_Type colour_tab[6] = {Red,Yellow,Green,Cyan,Blue,Purple};

/**/

void FFT_Init(void)     //�˺�����������FFT�������������ֵ������ֵ��͵�λ���
{
	uint32_t i,n,a;

	for(i=0; i < N/2 ;i++)
	{
		COS[i] = (int32_t)round(1024.0 * cos(i*PI2/N));  //���ɷŴ�1024��������ֵ������ֵ��
		SIN[i] = (int32_t)round(1024.0 * sin(i*PI2/N));
		/************/
		FFT_colour_tab[i] = colour_tab[i%6];             //��������ɫ��ʼ��
    fallt_colour_tab[i] = White;                         //Ư������ɫ��ʼ��
	  /************/
	}
	//���ɵ�λ���
	for(n=0; n<N ;n++)
	{
		a = 0;
		for(i=0;i < M;i++)
		{
			a|=((n>>i)&0x01)<<(M-1-i);   //������Ȼ���n���ɵ�λ�����a
		}
		BitRO_tab[n] = a;
	}	
}

extern uint16_t g_buffer_adc[];
void bit_reversed(void)   // ��ADC_Value[n]����λ����FFT_tab[n]��ʵ��
{
	uint32_t n;
	for(n=0; n<N ;n++)
	{
		FFT_tab[n].Re = (int32_t)(g_buffer_adc[BitRO_tab[n]]-2048);//��ADC����ֵȥ��ֱ��������
		FFT_tab[n].Im = 0;										   //ʵ��ֱ������Ϊ1.65V������Ϊ����ֵ��2048�����Լ�ȥ2048�󰴵�λ����FFT_tab[n]��ʵ����
	}
}

void FFT(void)   //FFT����
{
	uint32_t L,B,J,k,p;
	COMPLEX_Type temp;

	for(L=1 ; L<=M ; L++)    	//�������㼶��L
	{
		B = 1<<(L-1);						//������ת����W_N_p�ĸ��� B = 2^(L-1)
		for(J=0 ; J<B ; J++)
		{
			p = J<<(M-L);					//������ת����W_N_p��ָ�� p = J*2^(M-L)	
			for(k=J ; k<N ; k += 2*B)
			{
				temp.Re = FFT_tab[k+B].Re * COS[p] + FFT_tab[k+B].Im * SIN[p];
				temp.Im = FFT_tab[k+B].Im * COS[p] - FFT_tab[k+B].Re * SIN[p];	
				temp.Re /=1024;				//�������Һ�����ֵ����ԭ����1024�������ｫ����������1024�ָ�ԭ����ֵ
				temp.Im /=1024;				//�������Һ�����ֵ����ԭ����1024�������ｫ����������1024�ָ�ԭ����ֵ
				FFT_tab[k+B].Re = FFT_tab[k].Re - temp.Re;
				FFT_tab[k+B].Im = FFT_tab[k].Im - temp.Im;
				FFT_tab[k].Re = FFT_tab[k].Re + temp.Re;
				FFT_tab[k].Im = FFT_tab[k].Im + temp.Im;				
			}
		}	
	}	
}

void Quantization_of_FFT_results(void)   //��FFT����ķ�ֵ�׼����ֵ�׵�����
{
	uint32_t i,j;
	double Amp2;
	
	for(i=0; i<N/2 ;i++)
	{
		Amp2 = 1.0*FFT_tab[i+1].Re * FFT_tab[i+1].Re + 1.0*FFT_tab[i+1].Im * FFT_tab[i+1].Im;
		FFT_Amp_tab[i] = (uint32_t) sqrt( Amp2 );  //���ֵ��
		
		for(j=4 ; j<=62 ; j+=2)		//31�������Զ��κ����ͷ�ֵ�׵�����
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

void Display_refresh(void)    //���ݷ�ֵ�׵����������ˢ����ʾ������
{
	int16_t i,temp,color;
	/*
	for(i=0; i<N/2; i++)
	{
		//��������		
		temp = FFT_Amp_tab[i]*5;
		Draw_FastVLine2(2*i, 128-temp, 128-temp,RandomColor());
		Draw_FastVLine2(2*i+1, 128-temp, 128-temp,RandomColor());
	}
	*/

	for(i=0; i<N/4; i++)
	{
	    color = RandomColor();
	    //��������
	    temp = FFT_Amp_tab[i]*6;
	    Draw_FastVLine2(4*i, 128-temp, temp,color);
	    Draw_FastVLine2(4*i+1, 128-temp, temp,color);
	}
}

/*
void Spectrum_color_switching(void)			//Ƶ����ɫ�л�
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
