#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unnsigned int
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
sbit LG = P1 ^ 4;
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 1;
sbit ENR = P1 ^ 0;
sbit ENL = P1 ^ 5;
sbit duoji1=P2^0;
sbit duoji2=P2^1;
sbit Trig = P1^6;      //超声波模块的TRIG
sbit Echo = P1^7;      //超声波模块的ECHO
sbit L1 = P0 ^ 0;
sbit R1 = P0 ^ 5;
sbit L2 = P0 ^ 1;
sbit R2 = P0 ^ 4;
sbit L3 = P0 ^ 2;
sbit R3 = P0 ^ 3;
unsigned int sum;
float L;
void dealy(int z)
{
	unsigned int a, b;
	for (a = 0; a < 1000; a++);
	for (b = 0; b < z; b++);
}
/*左转函数1*/
void turn_left1() {
	zkb1 = 0;
	zkb2 = 5;
}
/*左转函数2*/
void turn_left2() {
	zkb1 = 0;
	zkb2 = 15;
}
/*左转函数3*/
void turn_left3() {
	zkb1 = 0;
	zkb2 = 60
		;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 5;
	zkb2 = 0;
}
/*右转函数2*/
void turn_right2() {
	zkb1 = 15;
	zkb2 = 0;
}
/*右转函数3*/
void turn_right3() {
	zkb1 = 60;
	zkb2 = 0;
}
/*原地掉头*/
void point_turn() {
	unsigned int m=0;
	while(m<970){
		ENL=1;
		ENR=1;
		LG=1;
		LB=0;
		RG=0;
		RB=1;
		m++;
	}
	RG=1;
	RB=0;
}
void qianjing(){
	ENL=1;
	ENR=1;
	zkb1=20;
	zkb2=20;
}
/*循迹函数*/
void xunji()
{
	uchar flag;
	if ((L1 == 1) && (R1 == 1) && (L2 == 1) && (R2 == 1) && (L3 == 1) && (R3 == 1)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 1) && (R1 == 1) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 1) && (R3 == 1)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 1) && (L3 == 0) && (R3 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 1) && (L3 == 1) && (R3 == 1)) {
		flag = 0;
	}//直行
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
		flag = 3;
	}//左偏1，左转1
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 1) && (R3 == 0)) {
		flag = 3;
	}//左偏2，左转1
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 1) && (R3 == 0)) {
		flag = 4;
	}//左偏3，左转2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
		flag = 6;
	}//左偏4，左转3
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
		flag = 1;
	}//右偏1，右转1
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 1)) {
		flag = 1;
	}//右偏2，右转1
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 1)) {
		flag = 2;
	}//右偏3，右转2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 1) && (L3 == 0) && (R3 == 0)) {
		flag = 5;
	}//右偏4，右转3
	switch (flag) {
	case 0:qianjing();
		break;
	case 1:turn_right1();
		break;
	case 2:turn_right2();
		break;
	case 3:turn_left1();
		break;
	case 4:turn_left2();
		break;
	case 5:turn_right3();
		break;
	case 6:turn_left3();
		break;
	default:break;
	}
}
void aa(unsigned char i){
	unsigned char k;
	for(;i>0;i--){
		for(k=245;k>0;k--);
	}
}
void openHc(){
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
uchar getDistance(){  //利用超声波模块判断有无障碍物
    TH0=0;
    TL0=0;
    openHc();
    while(!Echo);
    TR0=1;
    while(Echo);
    TR0=0;
    sum=TH0*256+TL0;
    L=(sum*1.78)/100;
    if(L<4){
        return 1;
    }
    else{
        return 0;
    }
}
void go() {
	ENL = 1;
	ENR = 1;
	zkb1 = 10;
	zkb2 = 10;
}
void stop() {
	ENL = 0;
	ENR = 0;
}
void timer0() interrupt 1
{
	if (t < zkb1)
		ENL = 1;
	else
		ENL = 0;
	if (t < zkb2)
		ENR = 1;
	else
		ENR = 0;
	t++;
	if (t >= 100)
	{
		t = 0;
	}
}
void main(){
	unsigned int i=1;
	    EA=1;
        ET0=1;
        ET1=1;
        TMOD=0x11;
		TH0 = (65536 - 100) / 256;
		TL0 = (65536 - 100) % 256;
		TR0 = 1;
		ENL = 1;
		ENR = 1;
		zkb1 = 10;
	zkb2 = 10;
	LG = 1;//给电机加电启动
		LB = 0;
		RG = 1;
		RB = 0;

	while(1){
            if (getDistance()){
				stop();
				while(1){
			duoji1=duoji2=1;
			aa(3);
			duoji1=duoji2=0;
			aa(37);
			if(i){
				point_turn();
				i--;
			}
			xunji();
				}
			}
			xunji();
	}
}
