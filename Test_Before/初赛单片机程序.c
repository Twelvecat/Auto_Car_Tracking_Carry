 #include<reg51.h>
#define uchar unsigned char
#define uint unnsigned int
unsigned char def = 0;
unsigned int i=100;//用于只做一次停止抓取原地掉头的操作
int nb=6;//此为判断是否需要转90°弯的标志。
int nbn=8;
int abc=8000;
sbit LG = P1 ^ 4;//L代表左，G代表前进，B代表后退，ENR代表右边使能
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 1;
sbit ENR = P1 ^ 0;
sbit ENL = P1 ^ 5;
sbit djl=P2^0;  //分别为爪子的两个舵机
sbit djr=P2^1;
sbit R1 = P0 ^ 0;		//为6个光电循迹模块
sbit L3 = P0 ^ 5;
sbit L1 = P0 ^ 1;
sbit R3 = P0 ^ 4;
sbit R2 = P0 ^ 2;
sbit L2 = P0 ^ 3;
sbit carry_led = P1 ^ 7;
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<56;++j);
}
void point_go() { //原地旋转以后恢复
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	ENL=1;
	ENR=1;
	delay(10);
}
void stop() {//小车停止指令
	ENL=0;
	ENR=0;
}
void point_turn() { //原地掉头
	RB=1;
	LG=1;
	RG=0;
	LB=0;
	ENL=1;
	ENR=1;
	delay(1530);
}
void qianjin() { //往前走
	ENR=1;
	ENL=1;
	delay(10);
}
/*左转函数1*/
void turn_left1() {
	ENL = 0;
	ENR = 1;
	delay(111);
}
/*左转函数2*/
void turn_left2() {
	ENL = 0;
	ENR = 1;
	delay(10);
}
/*左转函数3*/
void turn_left3() {
	ENL = 0;
	ENR = 1;
	delay(500);
}
/*右转函数1*/
void turn_right1() {
	ENR=0;
	ENL = 1;
	delay(111);
}
/*右转函数2*/
void turn_right2() {

	ENL = 1;
	ENR = 0;
	delay(10);
}
/*右转函数3*/
void turn_right3() {
	ENL = 1;
	ENR = 0;
	delay(150);
}
/*左转函数4*/
void turn_left4() {
	RG=0;
	LB=0;
	LG=1;
	RB=1;
	ENR=1;
	ENL=1;
	delay(174
	8);

}
void xunji(){
	uchar flag;
	if(L3==1){
		delay(250);
		if(L3==0){
		nb=nb-1;
		}		
	}
	if(R3==1){
		delay(100);
		if(R3==0){
		flag = 7;
		}		
	}
	else if ((L1 == 1) && (R1 == 1) && (L2 == 1) && (R2 == 1)) {
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
	case 7:nbn=nbn-1;
		break;
	default:break;
	}	
}
void xunji_interrupt() interrupt 3{
	if(def==0){
	xunji();}				
	TL1 = 0xD7;		//设置定时初值
	TH1 = 0xFD;		//设置定时初值
}
void choose_stop() interrupt 1{
	if(nbn==1){
		while(1){
		ENR=0;
			if(L3==1){
				ENL=0;
				djr=djl=1;
				delay(1);
				djr=0;
				delay(1);
				djl=djr=0;
				delay(39); 
			}
		}
			
	}				
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
}
void all_key(){//定时器中断开关
		EA=1;
        ET1=1;
        TMOD=0x11;
		TL1 = 0xD7;		//设置定时初值
		TH1 = 0xFD;			//设置定时初值
		TL0 = 0xAE;	 
		TH0 = 0xFB;
		TF0 = 0;
		TR0 = 1;
		TF1=0;
		TR1=1;
}
void main(){
	int nb_end=-11; //此处指令代表L3检测的总次数，达到的时候就停止在终点
	    all_key();
		LB = 0;
		RB = 0;
		LG = 1;//给电机加电启动
		RG = 1;
		ENL = 1;
		ENR = 1;
		L2=0;
		while(1){
			if(nb==1){
				def=1;
				turn_left4();
				point_go();
				nb=5;
				def=0;
			}  
			if(carry_led==1){
				while(abc){
					djr=djl=1;
					delay(2);
					djl=djr=0;
					delay(39); 
					abc--;
					}
					i=i-1;
					if(i>0){
						def=1;
						point_turn();
						point_go();
						i=0;
						abc=8000;
						def=0;
					}	
			}
		}						
}
