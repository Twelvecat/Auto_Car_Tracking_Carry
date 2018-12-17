#include <12c5a60s2.h>
unsigned char zkb1 = 0;//左边电机的占空比
unsigned char zkb2 = 0;//右边电机的占空比
unsigned char t = 0;//定时器中断计数器
float Kp = 10, Ki = 0.5, Kd = 0;					//PID参数调节
float p = 0, i = 0, d = 0, PID_value = 0;			//pid直道参数 
int decide = 0,error = 0;                           //元素判断
float previous_error = 0, previous_I = 0;           //误差值 
static int initial_motor_speed = 60;                //初始速度 
unsigned char mod = 1;//用作模式判断
unsigned char nb = 4;//用作判断是否掉头
unsigned char nbn = 13;//用作判断是否停车
sbit LG = P1 ^ 4;
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 0;
sbit ENR = P1 ^ 1;
sbit ENL = P1 ^ 5;
sbit R1 = P0 ^ 1;
sbit L1 = P0 ^ 2;
sbit C1 = P0 ^ 0;
sbit L2 = P0 ^ 4;
sbit R2 = P0 ^ 3;
sbit key=P2^4;
sbit key_go=P2^1;
sbit key_mod=P2^7;
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
				nb++;
				while(!key);
			}
		if (key_mod==0){
			delay(40); 
			if (key_mod==0){
				mod=0;
				while(!key_mod);
			}	
		}
		if (key_go==0){
			delay(50); 
			if (key_go==0){
				while(!key_go);
				break;
			}
		}
	}		
} }
void read_values()
{
  if ((L2 == 0) && (L1 == 0) && (C1 == 0) && (R1 == 0) && (R2 == 1)) {
      error = 2;//          0 0 0 0 1
    } else if ((L2 == 0) && (L1 == 0) && (C1 == 0) && (R1 == 1) && (R2 == 0)) {
      error = 1;//          0 0 0 1 0
    } else if ((L2 == 0) && (L1 == 0) && (C1 == 1) && (R1 == 0) && (R2 == 0)) {
      error = 0;//          0 0 1 0 0
    } else if ((L2 == 0) && (L1 == 1) && (C1 == 0) && (R1 == 0) && (R2 == 0)) {
      error = -1;//         0 1 0 0 0
    } else if ((L2 == 1) && (L1 == 0) && (C1 == 0) && (R1 == 0) && (R2 == 0)) {
      error = -2;//         1 0 0 0 0
    } else if ((L2 == 0) && (L1 == 0) && (C1 == 0) && (R1 == 0) && (R2 == 0)) {
      if (error == -2) {//  0 0 0 0 0
        error = -3;
      }else{
        error = 3;
      }
	}
}
void calc_pid()
{
  p = error;
  i = i + error;
  d = error - previous_error;
  PID_value = (Kp * p) + (Ki * i) + (Kd * d);
  previous_error = error;
}
//速度设定范围(-100,100)
void motorsWrite(int speedL, int speedR)
{
  if (speedR > 0) {
    zkb2=speedR;
	RG=1;
	RB=0;
  } else {
    zkb2=-speedR;
	RG=0;
	RB=1;
  }
  if (speedL > 0) {
    zkb1=speedL;
	LG=1;
	LB=0;
  } else {
    zkb1=-speedL;
	LG=0;
	LB=1;
  }
}
//速度设定范围(-100，100)
/*void motorsWritePct(int speedLpct, int speedRpct) {
  //speedLpct, speedRpct ranges from -100 to 100
  motorsWrite(speedLpct * 2.55, speedRpct * 2.55);
}*/
void motor_control()
{
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;
  
  if(left_motor_speed < -100){
    left_motor_speed = -100;
  }
  if(left_motor_speed > 100){
    left_motor_speed = 100;
  }
  motorsWrite(left_motor_speed,right_motor_speed);
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
void loop()
{
     read_values();  //获取5个循迹模块的数值情况
     calc_pid();            //pid计算出转向的pwm值
     motor_control();       //电机转动
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
	init();
	ENL=1;
	ENR=1;
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	while(1){
		loop();
		}		
	}