#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define barrierDis 15      //距离障碍物的距离 
uint sum;
float L;
//sbit LED = P0^0;
sbit Trig = P1^1;      //超声波模块的TRIG
sbit Echo = P1^2;      //超声波模块的ECHO
/*sbit motorDriver_1 =P2^1;  //控制第一个电机，从单片机出来后接L298n模块的IN1
sbit motorDriver_2 =P2^2;  //控制第一个电机，从单片机出来后接L298n模块的IN2
sbit motorDriver_3 =P2^3;  //控制第二个电机，从单片机出来后接L298n模块的IN3
sbit motorDriver_4 =P2^4;  //控制第二个电机，从单片机出来后接L298n模块的IN4
sbit servorControl =P1^0;  //舵机的控制引脚*/
uchar flag=0;
uchar control=5;
uchar servorTime=0;
uchar lFlag=0;//左方向是否有障碍的标志
uchar rFlag=0;//右方向是否有障碍的标志
void delay(uchar time){  //延迟函数
    uchar i;
    for(;time>0;time--){
        for(i=0;i<255;i++);
    }
}
void stop(){     //小车停止
    motorDriver_1=0;
    motorDriver_2=0;
    motorDriver_3=0;
    motorDriver_4=0;
}
void turnLeft(){        //小车向左转
    motorDriver_1=1;
    motorDriver_2=0;
    motorDriver_3=0;
    motorDriver_4=0;
}                        //小车向右转
void turnRight(){
    motorDriver_1=0;
    motorDriver_2=0;
    motorDriver_3=1;
    motorDriver_4=0;
}
void turnUp(){            //小车向前走
    motorDriver_1=1;
    motorDriver_2=0;
    motorDriver_3=1;
    motorDriver_4=0;
}
void turnDown(){          //小车向后走
    motorDriver_1=0;
    motorDriver_2=1;
    motorDriver_3=0;
    motorDriver_4=1;
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
    if(L<barrierDis&&flag==0){
        LED=0;
        return 0;
    }
    else{
        LED=1;
        flag=0;
        return 1;
    }
}
void main(){
        //LED=1;
        EA=1;
        ET0=1;
        ET1=1;
        TMOD=0x11;
        TH1=0xff;
        TL1=0x9c;
        servorTime=0;
        while(1){
             stop();     
             control=15;//控制舵机使超声波模块正对前方
             servorTime=0;
             TR1=1;
             delay(200);
             TR1=0;
             turnUp();
             LED=0;
             delay(200);
             LED=1;
             while(getDistance()==1); //向前行驶，直到前方有障碍物
             do{
                stop();
                control=22;    //使舵机向左摆动
                servorTime=0;
                TR1=1;
                delay(200);
                TR1=0;
                rFlag=getDistance();
                control=5;       //使舵机向右摆动
                servorTime=0;
                TR1=1;
                delay(200);
                TR1=0;
                lFlag=getDistance();
                turnDown();
                delay(220);
                delay(220);
                delay(220);
            }while(lFlag==0&&rFlag==0);
             if(rFlag==1&&lFlag==0) //左侧没有障碍物
                turnLeft();
             else if(rFlag==0&&lFlag==1)//右侧没有障碍物
                turnRight();
             else if(rFlag==1&&lFlag==1)//两侧都没有障碍物，默认向左走
                turnLeft();
             delay(220);
             delay(220);
             delay(200);
        }
}
/*void T1_int(void) interrupt 3{    //产生舵机所需要的脉冲
        TH1=0xff;
        TL1= 0x9c;
        servorTime++;
        if(servorTime<=control)
            servorControl=1;
        else
            servorControl=0;
        if(servorTime>=200)
            servorTime=0;
}*/
void T0_int(void) interrupt 1{    //超声波超出测量范围
        flag=1;
}