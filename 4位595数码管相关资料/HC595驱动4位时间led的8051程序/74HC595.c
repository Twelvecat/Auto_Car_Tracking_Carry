#include <AT89X51.H>
 
 unsigned char code fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 unsigned char code segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 unsigned char  disbuf[4]={0,0,0,0};


 //-----------------------------------------------------------------------------
// ����ԭ�ζ���
#define uchar unsigned char

void main (void);					// ������
void LED4_Display (void);			// LED��ʾ
void LED_OUT(uchar X);				// LED���ֽڴ�����λ����

unsigned char code LED_0F[];		// LED��ģ��

sbit DIO = P1^0;				//������������
sbit RCLK  = P1^1;				//ʱ�������źš�����������Ч
sbit SCLK = P1^2;				//�����źš���������������Ч

//-----------------------------------------------------------------------------
// ȫ�ֱ���//

//uchar LED[8];	//����LED��8λ��ʾ����


//*****************************************************************************
// ������
//
void main (void) 
{
		 	TMOD = 0X01;
	TH0 = 0xFC;
	TL0 = 0x18;
	TR0 = 1;
/*	LED[0]=1;
	LED[1]=2;
	LED[2]=3;
	LED[3]=4;
		   */
	while(1)
	{
		LED4_Display ();
	} 
} 

void LED4_Display (void)
{
	unsigned char code *led_table;          // ���ָ��
	uchar i;
	//��ʾ��1λ
	led_table = LED_0F;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x01);		

	RCLK = 0;
	RCLK = 1;
	//��ʾ��2λ
	led_table = LED_0F ;
	i = *led_table;

	LED_OUT(i);		
	LED_OUT(0x02);		

	RCLK = 0;
	RCLK = 1;
	//��ʾ��3λ
	led_table = LED_0F ;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK = 0;
	RCLK = 1;
	//��ʾ��4λ
	led_table = LED_0F;
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x08);		

	RCLK = 0;
	RCLK = 1;
}

void LED_OUT(uchar X)
{
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO=1; else DIO=0;
		X<<=1;
		SCLK = 0;
		SCLK = 1;
	}
}

unsigned char code LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};



