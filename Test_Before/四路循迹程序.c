#include <reg51.h>
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
sbit L1 = P0 ^ 0;
sbit R1 = P0 ^ 5;
sbit L2 = P0 ^ 1;
sbit R2 = P0 ^ 4;
sbit L3 = P0 ^ 2;
sbit R3 = P0 ^ 3;
void dealy(int z)
{
	unsigned int a, b;
	for (a = 0; a < 1000; a++);
	for (b = 0; b < z; b++);
}
/*初始化定时器，中断*/
void init()
{
	EA = 1;
	ET0 = 1;
	TMOD = 0x01;
	TH0 = (65536 - 100) / 256;
	TL0 = (65536 - 100) % 256;
	TR0 = 1;
}
/*中断函数+脉宽调制*/
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
void qianjin() {
	zkb1 = 10;
	zkb2 = 10;
}
/*左转函数1*/
void turn_left1() {
	zkb1 = 0;
	zkb2 = 15;
}
/*左转函数2*/
void turn_left2() {
	zkb1 = 0;
	zkb2 = 45;
}
/*左转函数3*/
void turn_left3() {
	zkb1 = 0;
	zkb2 = 80
		;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 15;
	zkb2 = 0;
}
/*右转函数2*/
void turn_right2() {
	zkb1 = 45;
	zkb2 = 0;
}
/*右转函数3*/
void turn_right3() {
	zkb1 = 80;
	zkb2 = 0;
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
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 1) && (L3 == 0) && (R3 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 1) && (R3 == 1)) {
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
/*
	else	if((L2==1)&&(R2==0)&&(L3==1)&&(R3==0)){
	flag = 4;}//经过路口转弯
	else	if((L2==1)&&(R2==1)&&(L3==1)&&(R3==1)){
	flag = 0;}//经过路口直行
	else	if((L2==0)&&(R2==1)&&(L3==0)&&(R3==1)){
	flag = 2;}//经过路口转弯
	*/
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
void main() {
	dealy(10);
	init();
	zkb1 = 10;
	zkb2 = 10;
	while (1) {
		LG = 1;//给电机加电启动
		LB = 0;
		RG = 1;
		RB = 0;
		ENL = 1;
		ENR = 1;
		//while(1){
			//if(IN6==0&&IN7==0)STOP(IN1=0;IN2=0;IN3=0;IN4=0;)
			//else GO(IN1=1;IN2=0;IN3=1;IN4=0;)
		//}
		while (1) {
			xunji();
			if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0) && (L3 == 0) && (R3 == 0)) {
				while (1) {
					ENL = 0;
					ENR = 0;
				}
			}
		}
	}
}
