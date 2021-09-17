#include "delay.h"
#include "sys.h"
#include "can.h"
#include "motor.h"
#include "laser.h"
#include "gyro.h"
#include "io.h"
#include "string.h"
#include "usart.h"
#include "ultrasonic.h"
#include "timer.h"
#include "steer.h"

/************************************************
           	echo   	    trig
	Left			PD.1				PD.0
	Right			PC.2				PC.1
	
**********************舵机赋值**********************/

#define steer1(A) TIM4->CCR1=A;   //PB6
#define steer2(A) TIM4->CCR2=A;   //PB7

#define high TIM4->CCR2=93
#define low  TIM4->CCR2=60

#define point_at_left  TIM4->CCR1=110
#define point_at_right TIM4->CCR1=42

#define found      PBin(0)==1
#define notfound   PBin(0)==0
#define work       PBout(1)=1
#define sleep      PBout(1)=0

#define light_left_on        PEin(5)==0   //左侧光电
#define light_left_off       PEin(5)==1 
#define light_right_on       PEin(6)==0   //右侧光电   检测到输出0，反之则输出1
#define light_right_off      PEin(6)==1

#define getdist             Measure()    //激光测距

char UNLOCK[5] = {0XFF,0XAA,0X69,0X88,0XB5};
char RATE[5] = {0XFF,0XAA,0X03,0X08,0X00};

int a=0;
void bumiao(int b)
{
	    Can_Send_speed(0,1);
			Can_Send_speed(0,2);
	    low;
	    if(b==1)    //zuo
	    steer1(112)
	    if(b==2)    //you
	    steer1(40)
			
	    SetpMotor_SetSpeed(0,4);
	    if(a==0)
			{
			 SetpMotor_SetStep(0,-1600);
			 a=1;
			}
			else 	if(a==1) SetpMotor_SetStep(0,-750);
      delay_ms(1000);
			delay_ms(1000);
			high;
			delay_ms(1000);
			if(b==1)    //zuo
      point_at_left;	    
			else if(b==2)    //you
			point_at_right;
			delay_ms(1000);
}

int main(void)
{
	
	while(1)
		{
		
  float ceshi=Read_dist(2);
		printf ("s=10%f\r\n", ceshi);
	delay_ms (1000);
	
	
	}
	
	
//	  int i=0;
//	  float left,rigt;
//    delay_init();	    	 //延时函数初始化
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);
//    //CAN初始化正常模式,波特率1000Kbps
//	  Initial_UART2(115200);//接JY-61模块的串口
//	  uart_init(9600);
////	  usart3_init(115200);
////	  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
////	  Laser_init();
//	  IO_Init();   //初始化IO口
//	  gyro_init();
//    TIM2_Int_Init(999,71);
//	  TIM3_Int_Init(9,719);
//	  TIM4_PWM_Init(999,1439);
//	  delay_ms(1000);
//	  sendcmd(UNLOCK);
//	//  sendcmd(RATE);
//	  delay_ms(1000);
//	  printf("start\r\n");
////  test();
////  while(1);
//    
////		while(1)
////		{
////		  left=Read_dist(1);
////			delay_ms(50);
////			rigt=Read_dist(3);
////			delay_ms(50);
////		 	printf("left: %.2f  rigt: %.2f\r\n",left,rigt);
////		}

//    
////		SetpMotor_SetStep(0,16000);
////		SetpMotor_SetSpeed(0,4);
////	  while(1);
////  		SetpMotor_SetStep(0,-750);    //-1600   -750
////  		SetpMotor_SetSpeed(0,4);
////			SetpMotor_SetStep(0,-750);

////		work;
////    while(1);
////      if(PEin(12))
////			Can_Send_speed(-30000,1);
////			else if(PEin(14))
////			Can_Send_speed(30000,2);
////    bumiao(1);
////		delay_ms(3000);
////		bumiao(1);
////		delay_ms(3000);
////		bumiao(1);
////		delay_ms(3000);
////		bumiao(1);
////		delay_ms(3000);
////		bumiao(1);
////		while(1);
//		
//    steer1(76)
//		if(PEin(12))
//		{
//		  high;
//		
//		while(1)
//		{ 
//      go_start();
//			standard_angle=-90.0;
//				
//      point_at_left;
//      delay_ms(1000);
//      delay_ms(1000);						
//			
//	     //1						
//			go_straight_long(1,1);		
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);	
//			turn_left();
//			standard_angle=90.0;
//			
//			point_at_right;
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      //2
//			 go_straight_long(1,0);
//			turn_right();
//			standard_angle=0.0;
//      go_after_turn(3);			
//			turn_right();
//			standard_angle=-90.0;
//			
//      point_at_left;
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      //3
//			go_straight_long(1,0);
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);			
//			turn_left();
//			standard_angle=90.0;
//			
//			point_at_right;
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      //4	
//			go_straight_long(1,0);
//			turn_right();
//			standard_angle=0.0;
//      go_after_turn(3);			
//			turn_right();
//			standard_angle=-90.0;
//			
//      point_at_left;
//      delay_ms(1000);
//      delay_ms(1000);						
//			
//      //5	
//			 go_straight_long(1,0);
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);			
//			turn_left();
//			standard_angle=90.0;
//			
//			go_straight_long(1,1);
//			turn_left_last();
// 	    steer1(76)					

//	    go_straight();
//			
//			Can_Send_speed(-30000,1);
//			Can_Send_speed(30000,2);	
//      delay_ms(1000);
//			Can_Send_speed(0,1);
//			Can_Send_speed(0,2);		
//			while(1);
//			
//		}
//	}	
//		
//	
//	//----------------------------------------------
//	else if(PEin(14))
//		{
//			high;
//		while(1)
//		{ 
//      go_start();
//			standard_angle=-90.0;
//				
//      point_at_left;
//      work;
//      delay_ms(1000);
//      delay_ms(1000);						
//			
//      while(1)	     //1
//			{		
//       work;				
//			 go_straight_long(2,1);
//			 if(light_left_on) bumiao(1); 
//			 else break;			
//			}
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);	
//			turn_left();
//			standard_angle=90.0;
//			
//			point_at_right;
//			high;
//      work;	
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      while(1)	     //2
//			{		
//       work;				
//			 go_straight_long(2,0);
//			 if(light_left_on) bumiao(2);
//			 else break;		
//			}
//			turn_right();
//			standard_angle=0.0;
//      go_after_turn(3);			
//			turn_right();
//			standard_angle=-90.0;
//			
//      point_at_left;
//			high;
//      work;
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      while(1)	     //3
//			{			
//				       work;				
//			 go_straight_long(2,0);
//			 if(light_left_on) bumiao(1);
//			 else break;
//			}
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);			
//			turn_left();
//			standard_angle=90.0;
//			
//			point_at_right;
//			high;	
//			work;	
//      delay_ms(1000);	
//      delay_ms(1000);						
//			
//      while(1)	     //4
//			{			
//			       work;					
//			 go_straight_long(2,0);
//       if(light_left_on) bumiao(2);
//			 else break;
//				
//			}
//			turn_right();
//			standard_angle=0.0;
//      go_after_turn(3);			
//			turn_right();
//			standard_angle=-90.0;
//			
//      point_at_left;
//			high;
//      work;	
//      delay_ms(1000);
//      delay_ms(1000);						
//			
//      while(1)	     //5
//			{			
//				       work;				
//			 go_straight_long(2,0);
//			 if(light_right_on) bumiao(1);
//			 else break;
//				
//			}
//			turn_left();
//			standard_angle=0.0;
//      go_after_turn(1);			
//			turn_left();
//			standard_angle=90.0;
//			
//			go_straight_long(1,1);
//			turn_left_last();
// 	    steer1(76)	
//      low;			

//	    go_straight();
//			
//			Can_Send_speed(-30000,1);
//			Can_Send_speed(30000,2);	
//      delay_ms(1000);
//			Can_Send_speed(0,1);
//			Can_Send_speed(0,2);		
//			while(1);
//			
//		}
//	}	
//		
//	else 
//	{point_at_right;
//		while(1);
//	}
}


