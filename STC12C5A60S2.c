#include <12c5a60s2.h>
#define uchar unsigned char
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
unsigned char nb = 11;//用作判断是否掉头
unsigned char nbn = 0;//用作判断是否停车
unsigned char def = 1;//用作判断是否停车
void xunji();
sbit key=P2^4;
sbit key_go=P2^1;
sbit LG = P1 ^ 4;
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 0;
sbit ENR = P1 ^ 1;
sbit ENL = P1 ^ 5;
sbit R1 = P0 ^ 1;
sbit L1 = P0 ^ 0;
sbit C1 = P0 ^ 4;
sbit L2=P0^2;
sbit R2=P0^3;
sbit Go=P0^7;
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<248;++j);
}
void keyscan(){ //键盘扫描检测按键以及发出GO指令
	key=1;
	key_go=1;
	while(1){
		if (key==0){
			delay(40); 
			if (key==0){
				nb++;
				while(!key);
			} }
		if (key_go==0){
			delay(50); 
			if (key_go==0){
				while(!key_go);
				Go=0;
				break;
			}
		}
	}		
} 
void Timer0() interrupt 3{
	if (t < zkb1)	ENL = 1;
	else			ENL = 0;
	if (t < zkb2)	ENR = 1;
	else			ENR = 0;
	t++;
	if (t > 100)	t = 0;
	TH1=0xff;
	TL1=0xf9;
}	
void Timer1() interrupt 1{
	if(def){
	xunji();}
	TL0 = 0xdb;		//设置定时初值
	TH0 = 0xfc;		//设置定时初值		
}
void qianjing() {
	zkb1 = 25;
	zkb2 = 24;
}
/*左转函数1*/
void turn_left1() {
	zkb1 = 18;
	zkb2 = 40;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 39;
	zkb2 = 14;		   
}
/*左转函数4*/
void turn_left4() {
	LB=1;LG=0;
	do{
	zkb1 = 22;
	zkb2 = 23;
	if(L1==1)break;
	}while(1);
	LB=0;LG=1;
	turn_right1();
	delay(29);
}
void xunji(){
	if ((L1 == 1) && (R1 == 0))	turn_left1();
	else if ((L1 == 0) && (R1 == 1))	  turn_right1();
	else if ((L1 == 0) && (R1 == 0))	qianjing();
	else if ((L1 == 1) && (R1 == 1))	qianjing();
}
void init(){
	TMOD=0x01;
	AUXR=0xc1;
	TH1=0xff;
	TL1=0xf9;
	TH0=0xdb;
	TL0=0xfc;
	EA=1;
	ET0=1;
	ET1=1;
	TF0=0;
	TF1=0;
	TR0=1;
	TR1=1;
}
int main () {
	Go=1;
	keyscan();
	init();
	ENL=1;
	ENR=1;
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	zkb1=40;
	zkb2=40;
	while(1){
		if (C1==1){
	delay(15);
		if(C1==0) nb--;}

	if (R2==1){
	delay(80);
		if(R2==0)nbn++;}
		if(nb==8){
			def = 0;
			turn_left4();
			nb=7;

			def =1;
		}
		else if(nb==4||nb==2||nb==0){
			def = 0;
			turn_left4();
			nb--;
			def =1;
		}
		if (nbn ==9){
			def=0;
			zkb1=zkb2=0;
			Go=1;
		}
	}
	
}