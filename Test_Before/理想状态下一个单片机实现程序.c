#include <12c5a60s2.h>
#define uchar unsigned char
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
unsigned char mod = 1;//用作模式判断
unsigned char nb = 4;//用作判断是否掉头
unsigned char nbn = 13;//用作判断是否停车
unsigned char car_stop=1;//判断车是否最终停止
unsigned char mod_turn=4;
unsigned char code fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
unsigned char  disbuf[4]={0,0,0,0};
unsigned char code LED_0F[] = {// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};
unsigned int cnt=0;//用作叠加到1s
int sec=0;	//计时多少s；
sbit key=P2^4;
sbit key_go=P2^1;
sbit key_mod=P2^7;
sbit LG = P1 ^ 4;
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 0;
sbit ENR = P1 ^ 1;
sbit ENL = P1 ^ 5;
sbit R1 = P0 ^ 1;
sbit L1 = P0 ^ 2;
sbit C1 = P0 ^ 0;
sbit L2=P0^4;
sbit R2=P0^3;
sbit djl=P1^7;
sbit djr=P1^6;
sbit carrt_led=P3^2;
sbit DIO = P0^7;				//串行数据输入
sbit RCLK  = P0^6;				//时钟脉冲信号——上升沿有效
sbit SCLK = P0^5;				//打入信号————上升沿有效
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<248;++j);
}
void keyscan(){ //键盘扫描检测按键以及发出GO指令
	key=1;
	key_go=1;
	key_mod=1;
	while(1){
		if (key==0){
			delay(40); 
			if (key==0){
				nb++；
				while(key==0)delay(20);
			}
		if (key_mod==0){
			delay(40); 
			if (key_mod==0){
				mod=0;
				while(key_mod==0)delay(20);
			}	
		}
		if (key_go==0){
			delay(40); 
			if (key_go==0){
				delay(40);
				break;
			}
		}
	}		
}
void LED_OUT(uchar X){   //数显管的移位操作
	uchar i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO=1; else DIO=0;
		X<<=1;
		SCLK = 0;
		SCLK = 1;
	}
}
void LED4_Display (){  //数显管的输出显示
	unsigned char code *led_table;          // 查表指针
	uchar i;
	//显示第4位
	led_table = LED_0F +sec%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x08);		
	RCLK = 0;
	RCLK = 1;
	//显示第3位
	led_table = LED_0F + sec/10%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x04);	
	RCLK = 0;
	RCLK = 1;
	//显示第2位
	led_table = LED_0F + sec/100%10;
	i = *led_table;
	LED_OUT(i);		
	LED_OUT(0x02);		
	RCLK = 0;
	RCLK = 1;
	//显示第1位
	led_table = LED_0F + sec/1000%10;
	i = *led_table;
	LED_OUT(i);			
	LED_OUT(0x01);		
	RCLK = 0;
	RCLK = 1;
}
void Timer0() interrupt 1{
	if (t < zkb1)	ENL = 1;
	else			ENL = 0;
	if (t < zkb2)	ENR = 1;
	else			ENR = 0;
	t++;
	if (t > 100)	t = 0;
	TH0=0xff;
	TL0=0xf9;
}
void choose_stop() interrupt 3{
	if(nbn==1){
		zkb1=zkb2=0;
		car_stop=0;
		while(1){
			djr=djl=1;
			delay(1);
			djr=0;
			delay(1);
			djl=djr=0;
			delay(39); 
			}			
	}
	LED4_Display();
	cnt++;
	TL1 = 0xdb;		//设置定时初值
	TH1 = 0xfc;		//设置定时初值		
}				
void point_go() {
	RB=0;
	LG=1;
	RG=1;
	LB=0;
	zkb1=50;
	zkb2=50;
}
void stop() {
	zkb1=0;
	zkb2=0;
}
void point_turn() {
	RB=0;
	LG=0;
	RG=1;
	LB=1;
	zkb1=30;
	zkb2=30;
}
void qianjing() {
	zkb1 = 60;
	zkb2 = 60;
}
/*左转函数1*/
void turn_left1() {
	zkb1 = 35;
	zkb2 = 45;
}
/*左转函数2*/
void turn_left2() {
	zkb1 = 0;
	zkb2 = 60;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 45;
	zkb2 = 35;
}
/*右转函数2*/
void turn_right2() {
	zkb1 = 60;
	zkb2 = 0;
}
/*右转函数4*/
void turn_right3() {
	zkb1 = 10;
	zkb2 = 0;
}
/*左转函数4*/
void turn_left4() {
	zkb1 = 0;
	zkb2 = 10;
}
void xunji(){
	if ((L1 == 0) && (R1 == 0) && (C1 == 1))			qianjing();
	else	if ((L1 == 1) && (R1 == 0) && (C1 == 0))	turn_left1();
	else	if ((L1 == 0) && (R1 == 1) && (C1 == 0))	turn_right1();
	else	if ((L1 == 1) && (R1 == 0) && (C1 == 1))	qianjing();
	else	if ((L1 == 1) && (R1 == 1) && (C1 == 0))	qianjing();
	else	if ((L1 == 1) && (R1 == 1) && (C1 == 1))	qianjing(); 
	else	if ((L1 == 0) && (R1 == 0) && (C1 == 0))	qianjing();
	if (L2==1) nb--;
	if (R2==1) nbn--;
	if(nb==1){
		do{turn_left4;}while(C1==0);
		if(mod_turn==4)	
		{nb=4;
		mod_turn=3;}
		else if(mod_turn>0)
		{nb=2;
		mod_turn--}
		else if(mod_turn==0) nb = 100;
	}
}
void init(){
	TMOD=0x01;
	AUXR=0xc1;
	TH0=0xff;
	TL0=0xf9;
	TH1=0xdb;
	TL1=0xfc;
	EA=1;
	ET0=1;
	ET1=1;
	TR0=1;
}
int main () {
	keyscan();
	init();
	ENL=1;
	ENR=1;
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	if(mod){//任务抓取模式
		while(1){
			if(car_stop){
			if(cnt>1000)
			{cnt = 0; 
			sec++;}
			xunji();
			if(carrt_led){
				while(1){
					djr=djl=1;
					delay(1);
					djr=djl=0;
					delay();
					xunji();
				}
			}
			}
		}
	}
	else {//自主抓取模式
		while(1){
			xunji();
		}		
	}
}
