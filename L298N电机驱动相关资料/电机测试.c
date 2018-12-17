#include<reg52.h>
sbit IN1=P1^4;
sbit IN2=P1^3;
sbit IN3=P1^2;
sbit IN4=P1^1;
sbit ENB=P1^0;
sbit ENA=P1^5;
void delay(unsigned int z);
void delay_us(unsigned int aa);
void main(){
	while(1){
		unsigned int i,cycle=0,T=2048;
		IN4=IN1=1;
		IN3=IN2=0;
		ENA=ENB=1;
		for(i=0;i<2000;i++){
			delay(10);
			
			
		}
		IN4=IN1=0;
		IN3=IN2=1;
		ENA=ENB=1;
		for(i=0;i<2000;i++){
			delay(20);
			
			
		}
		IN4=IN1=1;
		IN3=IN2=0;
		while(cycle!=T){
			ENB=ENA=1;
			delay_us(cycle++);
			ENB=ENA=0;
			delay_us(T-cycle);
		}
		cycle=0;
		IN4=IN1=0;
		IN3=IN2=1;
		while(cycle!=T){
			ENB=ENA=1;
			delay_us(cycle++);
			ENB=ENA=0;
			delay_us(T-cycle);
		}
	}
}
void delay(unsigned int z){
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void delay_us(unsigned int aa){
	while(aa--);
}
