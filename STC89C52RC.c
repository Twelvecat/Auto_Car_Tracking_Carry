#include <reg51.h>
#define uchar unsigned char
unsigned char code fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
unsigned char  disbuf[4]={0,0,0,0};
unsigned char code LED_0F[] = {// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};
unsigned int cnt=0;//�������ӵ�1s
unsigned char count=0;//�������ӵ�1s
unsigned char dj1=0;//�������ӵ�1s
unsigned char dj2=0;//�������ӵ�1s
int sec=0;	//��ʱ����s��
sbit djl=P1^2;
sbit djr=P1^1;
sbit Go=P1^0;
sbit carrt_led=P1^3;
sbit DIO = P2^2;				//������������
sbit RCLK  = P2^1;				//ʱ�������źš�����������Ч
sbit SCLK = P2^0;				//�����źš���������������Ч
void LED_OUT(uchar X){   //���Թܵ���λ����
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO=1; else DIO=0;
		X<<=1;
		SCLK = 0;
		SCLK = 1;
	}
}
void LED4_Display (){  //���Թܵ������ʾ
	unsigned char code *led_table;          // ���ָ��
	uchar i;
	//��ʾ��4λ
	led_table = LED_0F +sec/1000%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x08);		
	RCLK = 0;
	RCLK = 1;
	//��ʾ��3λ
	led_table = LED_0F + sec/100%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x04);	
	RCLK = 0;
	RCLK = 1;
	//��ʾ��2λ
	led_table = LED_0F + sec/10%10;
	i = *led_table;
	LED_OUT(i);		
	LED_OUT(0x02);		
	RCLK = 0;
	RCLK = 1;
	//��ʾ��1λ
	led_table = LED_0F + sec%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x01);		
	RCLK = 0;
	RCLK = 1;
}
void Timer1() interrupt 1{
	LED4_Display();
	cnt++;
	TL0 = 0xdb;		//���ö�ʱ��ֵ
	TH0 = 0xfc;		//���ö�ʱ��ֵ		
}
void Timer0() interrupt 3{
    if(count < dj2) 
    {djl = 1;}
    else
    {djl = 0;}
    if(count < dj2) //����ռ�ձ�����
    {djr = 1;}
    else
    {djr = 0;}
    count++;
    if (count >= 138) //T = 20ms��ʱ������������0
    {count = 0;}
	TL1 = 0x2e;		//���ö�ʱ��ֵ
	TH1 = 0xfF;		//���ö�ʱ��ֵ		
}
int main () {
	TMOD=0x01;
	TH0=0xdb;
	TL0=0xfc;
	TH1=0x2e;
	TL1=0xff;
	EA=1;
	ET1=1;
	TF1=0;
	TR1=1;
	ET0=1;
	TF0=0;
	TR0=1;
	djl=djr=0;
	while (1){
		if (carrt_led==1&&Go==0){
		dj1=dj2=7;
		}
		if(cnt>500&&Go==0){
			sec++;
			cnt=0;
		}
			
	}
}
