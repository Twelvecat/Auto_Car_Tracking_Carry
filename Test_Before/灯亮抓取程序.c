#include <reg51.h>
sbit carry_led = P1 ^ 7;
sbit djl=P2^0;  //分别为爪子的两个舵机
sbit djr=P2^1;
void delay(unsigned int m){ //延时函数，为0.5ms的倍数
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<56;++j);
}
int main(){
	while(1){
	if(carry_led==1){
				while(1){
					djr=djl=1;
					delay(3);
					djl=djr=0;
					delay(38); 
					}
					}	}
	
}