#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
void LED4_Display (long sec);			// LED显示
void LED_OUT(uchar X);				// LED单字节串行移位函数
unsigned char code LED_0F[];		// LED字模表
void delay(unsigned int m);
void keyscan();
unsigned char getDistance();
void openHc();
void xunji();
int nb = 2;
unsigned char code fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
unsigned char  disbuf[4]={0,0,0,0};
unsigned char code LED_0F[] = {// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};
long sec=0;
unsigned int cnt=0;
unsigned int dj=0;
unsigned int sum;
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
unsigned char mod = 1;//定时器中断计数器
float L;
sbit key=P2^4;
sbit key_go=P2^1;
sbit key_mod=P2^7;
sbit LG = P1 ^ 4;
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 1;
sbit ENR = P1 ^ 0;
sbit ENL = P1 ^ 5;
sbit djl=P2^0;
sbit djr=P2^1;
sbit Trig = P0^6;      //超声波模块的TRIG
sbit Echo = P0^7;      //超声波模块的ECHO
sbit R1 = P0 ^ 0;
sbit L3 = P0 ^ 5;
sbit L1 = P0 ^ 1;
sbit R3 = P0 ^ 4;
sbit R2 = P0 ^ 2;
sbit L2 = P0 ^ 3;
sbit DIO = P2^5;				//串行数据输入
sbit RCLK  = P2^6;				//时钟脉冲信号——上升沿有效
sbit SCLK = P2^7;				//打入信号————上升沿有效
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
				nb += 1;
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
void openHc(){//发射声波
   Trig=1;
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   Trig=0;
}
unsigned char getDistance(){  //超声波测距，瓶子相距为4cm返回1
    TH1=0;
    TL1=0;
    openHc();
    while(!Echo);
    TR1=1;
    while(Echo);
    TR1=0;
    sum=TH1*256+TL1;
    L=(sum*1.78)/100;
    if(L<4){
        return 1;
    }
    else{
        return 0;
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
void LED4_Display (long sec){  //数显管的输出显示
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
	zkb1 = 25;
	zkb2 = 50;
}
/*左转函数2*/
void turn_left2() {
	zkb1 = 0;
	zkb2 = 60;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 50;
	zkb2 = 25;
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
}
void Turn_big() interrupt 2{
	turn_left4();
}
void init(){
	TMOD=0x01;
	AUXR=0xc1;
	TH0=0xff;
	TL0=0xf9;
	EA=1;
	ET1=1;
	ET0=1;
	EX1=1;
	TR0=1;
}
int main(){
	keyscan();
	int nb_end;
	unsigned char k=1;
	unsigned char m=1;
	init();
	ENL=1;
	ENR=1;
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	cnt=0;
	nb_end=-9+nb;
	while(1){
		if(cnt>=1000){
			cnt=0;
			sec=sec+1;
		}
		if (getDistance()) {//判断是否抓取并且使用pwm以1ms的时间，20ms的周期去驱动
			if(m){
			stop();
			m=0;}
			if(dj==2){
				djl=1;
				djr=1;
			}
			djl=djr=0;
			if(k){
			point_turn();
			point_go();
			k=0;}
		}
		xunji();
		if(nb==0){
			INT1=0;
			nb=1;//可能是2
		}
		if(nb==nb_end){
			zkb1=0;
			if(R3==1){
				zkb2=0;
				while(1){
					if(dj==2){//需要修改参数
						djl=1;
						djr=1;
				}
					djl=djr=0;
				}
			}
		}
	}
}