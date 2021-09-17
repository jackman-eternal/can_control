#include "motor.h"
#include "delay.h"
#include "can.h"
#include "laser.h"
#include "gyro.h"
#include "io.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "can.h"
#include "ultrasonic.h"

#define light_left_on        PEin(5)==0   //�����
#define light_left_off       PEin(5)==1 
#define light_right_on       PEin(6)==0   //�Ҳ���   ��⵽���0����֮�����1
#define light_right_off      PEin(6)==1

#define getdist             Measure()    //������
#define getgyro            (float)stcAngle.Angle[2]/32768*180
#define setzero             sendcmd(YAWCMD)
	
#define limit_a      0.5   //����ĽǶ�ƫ��
#define coef1         90   //ϵ��
#define coef2         180
#define coef3         240

#define found      PBin(0)==1
#define notfound   PBin(0)==0
#define work       PBout(1)=1
#define sleep      PBout(1)=0

#define cd            50
#define speed       50000   

float standard_angle;

struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};

char SETZERO[5] = {0XFF,0XAA,0X76,0X00,0X00};
char YAWCMD[3] = {0XFF,0XAA,0X52};
char ACCCMD[3] = {0XFF,0XAA,0X67};
char SLEEPCMD[3] = {0XFF,0XAA,0X60};
char UARTMODECMD[3] = {0XFF,0XAA,0X61};
char IICMODECMD[3] = {0XFF,0XAA,0X62};

struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;

void gyro_init(void)
{
	   // sendcmd(UNLOCK);
			printf("���ڽ��м��ٶ�У׼\r\n");
			sendcmd(ACCCMD);//�ȴ�ģ���ڲ��Զ�У׼�ã�ģ���ڲ����Զ�������Ҫһ����ʱ��
			printf("���ٶ�У׼���\r\n");
			delay_ms(100);
			printf("����Z��Ƕ�����\r\n");
			sendcmd(YAWCMD);
			printf("Z��Ƕ��������\r\n");
	    delay_ms(100);
}
//�ô���2��JYģ�鷢��ָ��

 



void sendcmd(char cmd[])
{
	char i;
	for(i=0;i<5;i++)
		UART2_Put_Char(cmd[i]);
}

//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;

		}
		ucRxCnt=0;//��ջ�����
	}
}

void test(void)
{	
	int i;
	float dist;
  float L_dist,R_dist;
	while(1)
	{				
		i++;
		if(i>10)
		{
			i = 0;
			printf("���ڽ��м��ٶ�У׼\r\n");
			sendcmd(ACCCMD);//�ȴ�ģ���ڲ��Զ�У׼�ã�ģ���ڲ����Զ�������Ҫһ����ʱ��
			printf("���ٶ�У׼���\r\n");
			delay_ms(100);
			printf("����Z��Ƕ�����\r\n");
			sendcmd(SETZERO);
			printf("Z��Ƕ��������\r\n");
		}
		delay_ms(500);
		//���ڽ��ܵ��������Ѿ���������Ӧ�Ľṹ��ı������ˣ�����˵�����Э�飬�Լ��ٶ�Ϊ�� stcAcc.a[0]/32768*16����X��ļ��ٶȣ�
		//����Ƕ�
		printf("Angle:%.3f\r\n",(float)stcAngle.Angle[2]/32768*180);
		dist = getdist;
		printf("distance: %.2f\r\n",dist);
		L_dist=Read_dist(1);
		delay_ms(20);
		R_dist=Read_dist(3);
    printf("left: %.2f  rigt: %.2f \r\n",L_dist,R_dist);
		if(light_left_on) printf("������⵽\r\n"); else printf("�����δ��⵽\r\n");
		if(light_right_on) printf("�Ҳ����⵽\r\n"); else printf("�Ҳ���δ��⵽\r\n");
		printf("-----------------------------------\r\n");
		delay_ms(10);//�ȴ��������
	}//��ѭ��
}

//void adjust(void)
//{ 
//	  float dist_left[6],dist_rigt[6];
//	  float left,rigt;
//	  int i;
//	
//    Can_Send_speed(-10000,1);
//	  Can_Send_speed(10000,2);
//	  delay_ms(1000);
//	
//	  for(i=0;i<2;i++)
//	 {
//	  dist_left[i]=Read_dist(1);
//		delay_ms(50);
//	  dist_rigt[i]=Read_dist(3);	
//    delay_ms(50);		 
//	 }
//	 
//	  Can_Send_speed(0,1);
//	  Can_Send_speed(0,2);
//	  delay_ms(500);
//	 
//	 if((dist_left[1]<dist_left[0])&&(dist_rigt[1]>dist_rigt[0]))  //zuopian
//	 {
//	 	  Can_Send_speed(-10000,1);
//	    Can_Send_speed(-10000,2);
//		 
//		  while(1)
//			{
//			left=Read_dist(1);
//			delay_ms(50);
//			rigt=Read_dist(3);
//      delay_ms(50);	

//			printf("left: %.2f  rigt: %.2f\r\n",left,rigt);
//      if(200.0<left+rigt<205.0)			
//      break;				
//			}
//	 }
//	 
//	 	 else if((dist_left[1]>dist_left[0])&&(dist_rigt[1]<dist_rigt[0]))  //youpian
//	 {
//	 	  Can_Send_speed(10000,1);
//	    Can_Send_speed(10000,2);
//		 
//		  while(1)
//			{
//			left=Read_dist(1);
//			delay_ms(50);
//			rigt=Read_dist(3);
//      delay_ms(50);	

//			printf("left: %.2f  rigt: %.2f\r\n",left,rigt);
//      if(200.0<left+rigt<205.0)			
//      break;				
//			}
//	 }
//	 
//	 	 	Can_Send_speed(0,1);
//	    Can_Send_speed(0,2);
//}
void go_start(void)
{
	 float angle;
	
		printf("���ڽ��м��ٶ�У׼\r\n");
	  sendcmd(ACCCMD);//�ȴ�ģ���ڲ��Զ�У׼�ã�ģ���ڲ����Զ�������Ҫһ����ʱ��
		printf("���ٶ�У׼���\r\n");
		delay_ms(100);
	  printf("����Z��Ƕ�����\r\n");
		sendcmd(SETZERO);
		printf("Z��Ƕ��������\r\n");
	  delay_ms(100);
	
		Can_Send_speed(-10000,1);
	  Can_Send_speed(10000,2);
	  delay_ms(5);
		Can_Send_speed(-20000,1);
	  Can_Send_speed(20000,2);
	  delay_ms(5);
	  Can_Send_speed(-30000,1);
	  Can_Send_speed(30000,2);
	  while(light_left_off);
			
		delay_ms(760);
		
		Can_Send_speed(-30000,1);
	  Can_Send_speed(10000,2);
		while(1)
		{
		angle=getgyro;
    if(angle<-86.0)
		{
		Can_Send_speed(0,1);
	  Can_Send_speed(0,2);
			break;
		}
		}

}



void go_straight(void)
{   
	  int speed_left,speed_right;
    float left,rigt;
	  
	  while(1)
		{
		speed_left=speed_right=speed;
		rigt = Read_dist(3);
			
		while(rigt>300.0)
		{
		rigt = Read_dist(3);
		}	
		
		if(rigt>118.0)
		{
        if(rigt<121.0)
		{
				speed_left=speed+(rigt-118)*coef2;
			  speed_right=speed-(rigt-118)*coef2;
		}
		
//        else if((rigt>=108.0) && (rigt<112.0))
//		{
//				speed_left=50000+(rigt-105)*coef2;
//			  speed_right=50000-(rigt-105)*coef2;
//		}
		    else if(rigt>=121.0)
		{
				speed_left=speed+4500;
			  speed_right=speed-4500;
		}
	  }	
		
		else if(rigt<118.0)
		{
        if(rigt>115.0)
		{
				speed_left=speed+(118-rigt)*coef2;
			  speed_right=speed-(118-rigt)*coef2;
		}
		
//        else if((rigt>99.0) && (rigt<=103.0))
//		{
//				speed_left=50000+(105-rigt)*coef2;
//			  speed_right=50000-(105-rigt)*coef2;
//		}
		    else if(rigt<=115.0)
		{
				speed_left=speed-4500;
			  speed_right=speed+4500;
		}
	  }	
		
		Can_Send_speed(-speed_left,1);
	  Can_Send_speed(speed_right,2); 
		printf("speed: %d  %d\r\n",speed_left,speed_right);
		
		
		if(light_left_on) printf("������⵽\r\n"); else printf("�����δ��⵽\r\n");
		if(light_right_on) printf("�Ҳ����⵽\r\n"); else printf("�Ҳ���δ��⵽\r\n");
		printf("----------------------------\r\n");

			if((light_right_off) && (light_left_off) && ((rigt==0) || (rigt>300)))
			{
      delay_ms(10);
	   	if(light_right_off)
	  			break;
		  }
		   
		}
		 
}

void go_straight_long(int a,int b)
{
	  int speed_left,speed_right;
	  int i=0;
	  float angle_z,distance;
    float left,rigt;
	  float pre_left,pre_rigt;
	  int warning1=0,warning2=0;
	  float error=0; 
		
//			printf("���ڽ��м��ٶ�У׼\r\n");
//			sendcmd(ACCCMD);//�ȴ�ģ���ڲ��Զ�У׼�ã�ģ���ڲ����Զ�������Ҫһ����ʱ��
//			printf("���ٶ�У׼���\r\n");
//			delay_ms(100);
//			printf("����Z��Ƕ�����\r\n");
//			sendcmd(SETZERO);
//			printf("Z��Ƕ��������\r\n");
	
 //  	Can_Send_speed(-10000,1);
//	  Can_Send_speed(10000,2);
//	  left=Read_dist(1);
//	  rigt=Read_dist(3);
	  
	  
	
	
	  while(1)
		{
//		angle_z=getgyro;
//		if(angle_z<standard_angle-20.0)	//��ƫ  �Ƕ�ƫ�ƽϴ�
//		{
//		    Can_Send_speed(20000,1);
//	      Can_Send_speed(20000,2);
//			  while(1)
//				{
//				angle_z=getgyro;
//				if(angle_z>standard_angle-5.0)
//					break;
//				}
//		}
//		else if(angle_z>standard_angle+20.0)	//zuoƫ  �Ƕ�ƫ�ƽϴ�
//		{
//		    Can_Send_speed(-20000,1);
//	      Can_Send_speed(-20000,2);
//			  while(1)
//				{
//				angle_z=getgyro;
//				if(angle_z<standard_angle+5.0)
//					break;
//				}
//		}
		
	  speed_left=speed_right=speed;
		left = Read_dist(1);
		rigt = Read_dist(3);
		while(left>300.0)
		{
		left = Read_dist(1);
		}	
		while(rigt>300.0)
		{
		rigt = Read_dist(3);
		}	
		if(i==0)
		{
		  pre_left=Ultrasonic_mid(1);
			pre_rigt=Ultrasonic_mid(3);
		}
		while(abs(left-pre_left)>50.0)   //ultrasonic error
		{
		  left=Read_dist(1);
			pre_left=left;
		}
		while(abs(rigt-pre_rigt)>50.0)
		{
		  rigt=Read_dist(3);
			pre_rigt=rigt;		
		}
		i++;
    if(rigt>145) {warning2=1;warning1=0;}
    else if(left>145)	{warning1=1;warning2=0;}
  	else  {warning1=0;warning2=0;}	

		if(warning1==0 && warning2==0)
	{
    if(rigt>left)   //��ƫ
		{
			if(rigt-left<5.0)
			{
				speed_left=speed+(rigt-left)*coef1-error*cd;
			  speed_right=speed-(rigt-left)*coef1+error*cd;
			}
			else if((5.0<=rigt-left) && (rigt-left<10.0))
			{
				speed_left=speed+(rigt-left)*coef2-error*cd;
			  speed_right=speed-(rigt-left)*coef2+error*cd;
			}
			else if(rigt-left>=10.0)
			{
	 	  speed_left=speed+5000;
			speed_right=speed-5000;
			}
				  error=left-rigt;
		}	
		else  if(rigt<left)   //youƫ
		{
			if(left-rigt<5.0)
			{
				speed_left=speed-(left-rigt)*coef1-error*cd;
			  speed_right=speed+(left-rigt)*coef1+error*cd;
			}
			else if((5.0<=left-rigt) && (left-rigt<10.0))
			{
				speed_left=speed-(left-rigt)*coef2-error*cd;
			  speed_right=speed+(left-rigt)*coef2+error*cd;
			}
			else if(left-rigt>=10.0)
			{
	      speed_left=speed-5000;
			  speed_right=speed+5000;
			}
				  error=left-rigt;
		}	
	 }
   else if(warning1) 
   {
	    speed_left=speed-6000;
			speed_right=speed+6000;
		 	  error=0;
	 }		
   else if(warning2) 
   {
	 	  speed_left=speed+6000;
			speed_right=speed-6000;
		 	  error=0;
	 }	

	 
    Can_Send_speed(-speed_left,1);
	  Can_Send_speed(speed_right,2);	 
		printf("speed: %d  %d\r\n",speed_left,speed_right);
		
		
		if(light_left_on) printf("������⵽\r\n"); else printf("�����δ��⵽\r\n");
		if(light_right_on) printf("�Ҳ����⵽\r\n"); else printf("�Ҳ���δ��⵽\r\n");
		printf("----------------------------\r\n");
       			
	    if(a==2)
			{
			 if(found)
			 {
				 sleep;
				 break;
			 }
			}
					
	    if(b==0)
			{
			if(light_left_off || light_right_off)
			 {
      delay_ms(5);
	   	if(light_left_off || light_right_off)
			  {
				delay_ms(150);
	  			break;
			  }
		   }
		  }
		  else if(b==1)
			{
			if(light_left_off)
			 {
      delay_ms(5);
	   	if(light_left_off)
			  {
				delay_ms(150);
	  			break;
			  }
		   }
		  }	

		}
		   	
	}

void go_after_turn(int a)
{
	  int speed_left,speed_right;
	  float angle_z,distance;
    float left,rigt;
		float error;
	  
	  while(1)
		{	 
		speed_left=speed_right=40000;
		
		left = Read_dist(1);
		rigt = Read_dist(3);	
		while(left>300.0)
		{
		left = Read_dist(1);
		}	
		while(rigt>300.0)
		{
		rigt = Read_dist(3);
		}	
		
    if(rigt>left)   //��ƫ
		{
			if(rigt-left<5.0)
			{
				speed_left=40000+(rigt-left)*coef1-error*cd;
			  speed_right=40000-(rigt-left)*coef1+error*cd;
			}
			else if((5.0<=rigt-left) && (rigt-left<10.0))
			{
				speed_left=40000+(rigt-left)*coef2-error*cd;
			  speed_right=40000-(rigt-left)*coef2+error*cd;
			}
			else if(rigt-left>=10.0)
			{
					speed_left=43500;
					speed_right=36500;
			}
		}	
		else  if(rigt<left)   //youƫ
		{
			if(left-rigt<5.0)
			{
				speed_left=40000-(left-rigt)*coef1-error*cd;
			  speed_right=40000+(left-rigt)*coef1+error*cd;
			}
			else if((5.0<=left-rigt) && (left-rigt<10.0))
			{
				speed_left=40000-(left-rigt)*coef2-error*cd;
			  speed_right=40000+(left-rigt)*coef2+error*cd;
			}
			else if(left-rigt>=10.0)
			{
					speed_left=36500;
					speed_right=43500;
			}
		}
		error = left - rigt;
		Can_Send_speed(-speed_left,1);
	  Can_Send_speed(speed_right,2);  
		printf("speed: %d  %d\r\n",speed_left,speed_right);
		
		
		if(light_left_on) printf("������⵽\r\n"); else printf("�����δ��⵽\r\n");
		if(light_right_on) printf("�Ҳ����⵽\r\n"); else printf("�Ҳ���δ��⵽\r\n");
		printf("----------------------------\r\n");
		
    if(a==1)
		{
			if(light_left_off)
      {
				delay_ms(5);
	   	  if(light_left_off)
				{
					delay_ms(160);
	  	  break;
				}
		  }
		} 	
		else if(a==3)
		{
			if(light_right_off)
      {
				delay_ms(5);
	   	  if(light_right_off)
	  	  {
					delay_ms(160);
	  	    break;
				}
		  }
		} 	

   }
}



	void turn_left(void)
			{
				float angle_z;

			 Can_Send_speed(-10000,1);
	     Can_Send_speed(30000,2); 
			 while(1)
			 {	
				    angle_z=getgyro; 
				 	  printf("Z��Ƕȣ�%.2f\r\n",angle_z);	
					  if(standard_angle+86.0<angle_z)
						{
							 Can_Send_speed(0,1);
	             Can_Send_speed(0,2); 
							  break;
						}
			 }
        
			}
			
	void turn_left_last(void)
			{
				float angle_z;

			 Can_Send_speed(-10000,1);
	     Can_Send_speed(30000,2); 
			 while(1)
			 {	
				    angle_z=getgyro; 
				 	  printf("Z��Ƕȣ�%.2f\r\n",angle_z);	
					  if((angle_z>176.0) || (angle_z<-176.0))
						{
								Can_Send_speed(0,1);
	              Can_Send_speed(0,2);   
							  break;
						}
			 }
        
			}			

			void turn_right(void)
			{
					 	float angle_z;			

			 Can_Send_speed(-30000,1);
	     Can_Send_speed(10000,2); 
			 while(1)
			 {	
				    angle_z=getgyro; 
				 	  printf("Z��Ƕȣ�%.2f\r\n",angle_z);	
					  if(angle_z<standard_angle-86.0)
						{
							  Can_Send_speed(0,1);
	              Can_Send_speed(0,2); 
						  	break;
						}			  
			 } 
			}				
				  
		

void stop(void)
{
    Can_Stop(1);
	  Can_Stop(2);
}

void run(void)
{
    Can_Run(1);
	  Can_Run(2);
}

void shutdown(void)
{
    Can_Shutdown(1);
    Can_Shutdown(2);
}

