#include<12c5a60s2.h>
sbit duoji1=P1^4;
sbit duoji2=P1^5;
void aa(unsigned char i){
	unsigned char k;
	for(;i>0;i--){
		for(k=245;k>0;k--);
	}
}
void main(){
	int i=0;
	while(1){
		while(i<=500){
			duoji1=duoji2=1;
			aa(2);
			duoji1=0;
			aa(2);
			duoji2=0;
			aa(36);
			i=i+1;
		}
		i=0;
		while(i<=500){
			duoji1=duoji2=1;
			aa(3);
			duoji1=duoji2=0;
			aa(37);
			i=i+1;
		}
		i=0;
	}
}