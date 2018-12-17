 #include<reg51.h>
#define uchar unsigned char
#define uint unnsigned int
unsigned char def = 0;
unsigned int i=100;//����ֻ��һ��ֹͣץȡԭ�ص�ͷ�Ĳ���
int nb=6;//��Ϊ�ж��Ƿ���Ҫת90����ı�־��
int nbn=8;
int abc=8000;
sbit LG = P1 ^ 4;//L������G����ǰ����B������ˣ�ENR�����ұ�ʹ��
sbit LB = P1 ^ 3;
sbit RB = P1 ^ 2;
sbit RG = P1 ^ 1;
sbit ENR = P1 ^ 0;
sbit ENL = P1 ^ 5;
sbit djl=P2^0;  //�ֱ�Ϊצ�ӵ��������
sbit djr=P2^1;
sbit R1 = P0 ^ 0;		//Ϊ6�����ѭ��ģ��
sbit L3 = P0 ^ 5;
sbit L1 = P0 ^ 1;
sbit R3 = P0 ^ 4;
sbit R2 = P0 ^ 2;
sbit L2 = P0 ^ 3;
sbit carry_led = P1 ^ 7;
void delay(unsigned int m){ //��ʱ������Ϊ0.5ms�ı���
	unsigned int j,i;
	for(i=0;i<m;++i)
		for(j=0;j<56;++j);
}
void point_go() { //ԭ����ת�Ժ�ָ�
	RB=0;
	LB=0;
	LG=1;
	RG=1;
	ENL=1;
	ENR=1;
	delay(10);
}
void stop() {//С��ָֹͣ��
	ENL=0;
	ENR=0;
}
void point_turn() { //ԭ�ص�ͷ
	RB=1;
	LG=1;
	RG=0;
	LB=0;
	ENL=1;
	ENR=1;
	delay(1530);
}
void qianjin() { //��ǰ��
	ENR=1;
	ENL=1;
	delay(10);
}
/*��ת����1*/
void turn_left1() {
	ENL = 0;
	ENR = 1;
	delay(111);
}
/*��ת����2*/
void turn_left2() {
	ENL = 0;
	ENR = 1;
	delay(10);
}
/*��ת����3*/
void turn_left3() {
	ENL = 0;
	ENR = 1;
	delay(500);
}
/*��ת����1*/
void turn_right1() {
	ENR=0;
	ENL = 1;
	delay(111);
}
/*��ת����2*/
void turn_right2() {

	ENL = 1;
	ENR = 0;
	delay(10);
}
/*��ת����3*/
void turn_right3() {
	ENL = 1;
	ENR = 0;
	delay(150);
}
/*��ת����4*/
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
	}//ֱ��
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 0)) {
		flag = 0;
	}//ֱ��
	else	if ((L1 == 1) && (R1 == 1) && (L2 == 0) && (R2 == 0)) {
		flag = 0;
	}//ֱ��
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 1)) {
		flag = 0;
	}//ֱ��
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 0) && (R2 == 0)) {
		flag = 4;
	}//��ƫ1����ת1
	else	if ((L1 == 1) && (R1 == 0) && (L2 == 1) && (R2 == 0)) {	
		flag = 5;
	}//��ƫ2����ת2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 1) && (R2 == 0)) {	
		flag = 6;
	}//��ƫ3����ת3
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 0)) {
		flag = 1;
	}//��ƫ1����ת1
	else	if ((L1 == 0) && (R1 == 1) && (L2 == 0) && (R2 == 1)) {
		flag = 2;
	}//��ƫ2����ת2
	else	if ((L1 == 0) && (R1 == 0) && (L2 == 0) && (R2 == 1)) {
		flag = 3;
	}//��ƫ3����ת3
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
	TL1 = 0xD7;		//���ö�ʱ��ֵ
	TH1 = 0xFD;		//���ö�ʱ��ֵ
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
	TL0 = 0xAE;		//���ö�ʱ��ֵ
	TH0 = 0xFB;		//���ö�ʱ��ֵ
}
void all_key(){//��ʱ���жϿ���
		EA=1;
        ET1=1;
        TMOD=0x11;
		TL1 = 0xD7;		//���ö�ʱ��ֵ
		TH1 = 0xFD;			//���ö�ʱ��ֵ
		TL0 = 0xAE;	 
		TH0 = 0xFB;
		TF0 = 0;
		TR0 = 1;
		TF1=0;
		TR1=1;
}
void main(){
	int nb_end=-11; //�˴�ָ�����L3�����ܴ������ﵽ��ʱ���ֹͣ���յ�
	    all_key();
		LB = 0;
		RB = 0;
		LG = 1;//������ӵ�����
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
