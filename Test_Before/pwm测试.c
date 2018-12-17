#include <12c5a60s2.h>
#define uchar unsigned char
sbit djl=P2^0;  //分别为爪子的两个舵机
sbit djr=P2^1;
uchar aa;
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<248;++j);
}
void carry(){
if(aa<15) djl=djr=1;
else djr=djl=0;
if(aa>200)aa=0; 
}
void uncarry(){
if(aa<5) djl=djr=1;
else if(aa<10)djl=0;
else djr=djl=0;
if(aa>200)aa=0; 
}
void timer0()interrupt 1 //定时器1为0.1ms一个周期
{
TH0=(65536-100)/256;
TL0=(65536-100)%256;
aa++;
}
void init(){
aa=0;
djl=djr=0;
TMOD=0x11;
TH0=(65536-100)/256;
TL0=(65536-100)%256;
EA=1;
ET0=1;
TR0=1;
}
void main()
{
init();
while(1){
	carry();
	delay(2000);
	uncarry();
	delay(2000);
}
}





void initPCA()  
{
    // 初始化定时器T0为16位自动重装方式,其溢出脉冲作PCA计数器的时钟源
    TMOD=0x01;     // 设置T0为16位装载方式
    TH0=0xff;      // 定时时间78.125uS 决定了频率为50hz对应20ms
    TL0=0xb8;  
	ET0=1; 
    TR0=1;         // 启动定时器0
    // 初始化PCA模块0为PWM输出方式
    CMOD=0x04;     // #00000100B ,选择T0为PCA计数器时钟源
	CCON=0;
    CCAPM0=0x42		// 设置PCA模块0为8位PWM输出方式。脉冲在P1.4引脚输出，PWM无需中断支持。
	CL=0;			//高低8位初始化
    CCAP0L=0xf3;   // 设置脉冲宽度     
    EA=1;          // 开整个单片机所有中断共享的总中断控制位
    CR=1;          // 启动PCA计数器(CH,CL)计数
} 


#include <12c5a60s2.h>
#define uchar unsigned char
sbit djl=P2^0;  //分别为爪子的两个舵机
sbit djr=P2^1;

void PCA_Init(void)
{
	CCON = 0;				//PCA初始化
	CMOD = 0x00;			//空闲时不计数，不产生中断，时钟源为Sysclk/12，PWM频率大约为4KHz
	CL = 0x00;				//PCA低8位清零
	CH = 0x00;				//PCA高8位清零
 
	CCAPM0 = 0x42;			//8位PWM模式，无中断
	CCAP0H = 0xc0;			//PWM0占空比（调节此处值调节PWM占空比）
	CCAP0L = 0xc0;			//PWM0占空比（调节此处值调节PWM占空比）
 
	CCAPM1 = 0x42;			//8位PWM模式，无中断
	CCAP1H = 0x40;			//PWM1占空比（调节此处值调节PWM占空比）
	CCAP1L = 0x40;			//PWM1占空比（调节此处值调节PWM占空比）
 
	CR = 1;					//启动PCA计数器
}
 
/*PWM占空比设置*/
void PWM_Set(unsigned char x,unsigned char y)
{
  CCAP0H = y;				//设置比较值
  CCAP0L = y;	
  CCAP1H = x;				//设置比较值
  CCAP1L = x;
}
 
void main(void)
{
  P1M0 = 0xff;				//P1口推挽输出
  P1M1 = 0x00;
  PCA_Init();				//PCA初始化
  
  PWM_Set(0x40,0x40);       //设置占空比
}