#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unnsigned int
unsigned int sum;   //用于超声波测距的中间变量
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
float L;//测量距离物品的距离
unsigned int i=2;//用于只做一次停止抓取原地掉头的操作
int nb=3;//此为判断是否需要转90°弯的标志。
sbit LG = P1 ^ 4;//L代表左，G代表前进，B代表后退，ENR代表右边使能
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 1;
sbit ENR = P1 ^ 0;
sbit ENL = P1 ^ 5;
sbit djl=P2^0;  //分别为爪子的两个舵机
sbit djr=P2^1;
sbit Trig = P0^6;      //超声波模块的TRIG
sbit Echo = P0^7;      //超声波模块的ECHO
sbit R1 = P0 ^ 0;		//为6个光电循迹模块
sbit L3 = P0 ^ 5;
sbit L1 = P0 ^ 1;
sbit R3 = P0 ^ 4;
sbit R2 = P0 ^ 2;
sbit L2 = P0 ^ 3;
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<245;++j);
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
void timer0() interrupt 1//中断用来模拟pwm，制作小车的占空比控速
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
void point_go() { //原地旋转以后恢复
	RB=0;
	LG=1;
	RG=1;
	LB=0;
	zkb1=20;
	zkb2=20;
}
void stop() {//小车停止指令
	zkb1=0;
	zkb2=0;
}
void point_turn() { //原地掉头
	ENR=1;
	ENL=1;
	RB=0;
	LG=0;
	RG=1;
	LB=1;
	zkb1=90;
	zkb2=90;
}
void qianjin() { //往前走
	zkb1 = 100;
	zkb2 = 100;
}
/*左转函数1*/
void turn_left1() {
	zkb1 = 0;
	zkb2 = 30;
}
/*左转函数2*/
void turn_left2() {
	zkb1 = 0;
	zkb2 = 40;
}
/*左转函数3*/
void turn_left3() {
	zkb1 = 0;
	zkb2 = 45;
}
/*右转函数1*/
void turn_right1() {
	zkb1 = 30;
	zkb2 = 0;
}
/*右转函数2*/
void turn_right2() {
	zkb1 = 40;
	zkb2 = 0;
}
/*右转函数3*/
void turn_right3() {
	zkb1 = 45;
	zkb2 = 0;
}
/*左转函数4*/
void turn_left4() {
	zkb1 = 0;
	zkb2 = 90;
}
void xunji(){
	uchar flag;
	if(nb==0){
		INT1=0;
		nb=1;//可能是2
	}
	if ((L1 == 1) && (R1 == 1) && (L2 == 1) && (R2 == 1)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 1) && (R1 == 1) && (L2 == 0) && (R2 == 0)) {
		flag = 0;
	}//直行
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 1)) {
		flag = 0;
	}//直行
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 0) && (R2 == 0)) {
		flag = 4;
	}//左偏1，左转1
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 1) && (R2 == 0)) {	
		flag = 5;
	}//左偏2，左转2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 0)) {	
		flag = 6;
	}//左偏3，左转3
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 0)) {
		flag = 1;
	}//右偏1，右转1
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 1)) {
		flag = 2;
	}//右偏2，右转2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 1)) {
		flag = 3;
	}//右偏3，右转3
	switch (flag) {
	case 0:qianjin();
		break;
	case 1:turn_right1();
		break;
	case 2:turn_right2();
		break;
	case 3:turn_right3();
		break;
	case 4:turn_left1();
		break;
	case 5:turn_left2();
		break;
	case 6:turn_left3();
		break;
	default:break;
	}	
}
void Turn_big() interrupt 2{
	turn_left4();
}
void xunji_interrupt() interrupt 3{
	xunji();
	TL1 = 0x90;		//设置定时初值
	TH1 = 0xE8;		//设置定时初值
}
void carry() interrupt 0{
	if(i==2){
	stop();
	i=1;
	}
	djr=djl=1;
	delay(3);
	djl=djr=0;
	delay(37);
	if(i==1){
	point_turn();
	point_go();
	i=0;
	}
}
void all_key(){//定时器中断开关
		EA=1;
        ET0=1;
        ET1=1;
        TMOD=0x11;
		TL0 = 0x50;		//设置定时初值
		TH0 = 0xFB;		//设置定时初值
		TL1 = 0x90;		//设置定时初值
		TH1 = 0xE8;		//设置定时初值
		TF0=0;
		TF1=0;
		TR1=1;
		TR0 = 1;
		EX0=1;
		EX1=1;
		IT1=1;
		IT0=1;
}
void main(){
	int nb_end=-11; //此处指令代表L3检测的总次数，达到的时候就停止在终点
	    all_key();
		zkb1 = 10;
		zkb2 = 10;
		LB = 0;
		RB = 0;
		LG = 1;//给电机加电启动
		RG = 1;
		ENL = 1;
		ENR = 1;
		while(1){
			if(L3==1){
				nb=nb-1;
			}
			if (getDistance()){
				INT0=0;				
				if(nb==nb_end){
					zkb1=0;
					if(R3==1){
						zkb2=0;
						while(1){
							djr=djl=1;
							delay(2);
							djr=0;
							delay(2);
							djl=djr=0;
							delay(36);
			//放开瓶子
						}
					}
				}
			}
		}		
}
		
