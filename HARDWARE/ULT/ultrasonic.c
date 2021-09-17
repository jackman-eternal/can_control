#include "ultrasonic.h"
#include "usart.h"
#include "sys.h"
#include "delay.h"

/*Front    	PE.0接收   	PE.1发送
	Left			PD.1				PD.0
	Right			PC.2				PC.1
 */		


u32 Now_tim=0;
float Left_dist=0;
float  Left_tim=0;


float Right_dist=0;
float Right_tim=0;	



void Ultrasonic_IO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
		
		//PE.1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
		GPIO_Init(GPIOE, &GPIO_InitStructure);					 
		GPIO_ResetBits(GPIOE,GPIO_Pin_1);						 
		//PD.0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    		
		GPIO_Init(GPIOD, &GPIO_InitStructure);	  				
		GPIO_ResetBits(GPIOD,GPIO_Pin_0); 						 
		//PC.1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		
		GPIO_Init(GPIOC, &GPIO_InitStructure);	  				
		GPIO_ResetBits(GPIOC,GPIO_Pin_1); 	
    //PE.4	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		
		GPIO_Init(GPIOE, &GPIO_InitStructure);	  				
		GPIO_ResetBits(GPIOE,GPIO_Pin_4); 	
}
 void Ultrasonic_Send(int a)
{
		Now_tim=0;
	
	if(a==1)       PDout(0)=1;
	else if(a==3)  PCout(1)=1;

	delay_us(30);
	PDout(0)=0;
	PCout(1)=0;	

}

	
void EXTIX_Init(void)     //PF.0作为检测端
{
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;  //结构体声明
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
		GPIO_Init(GPIOE, &GPIO_InitStructure);   //前侧超声波
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
		GPIO_Init(GPIOD, &GPIO_InitStructure); //左侧超声波
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);   //右侧超声波
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
		GPIO_Init(GPIOE, &GPIO_InitStructure);   //左侧超声波2
		//初始化I/O口
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		//外部时钟必须使能
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
				//中断映射端口
		//PE.0
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;								
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //事件OR中断（中断）
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //上升沿和下降沿沿中断
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		//PD.1
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource1);
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;			
		EXTI_Init(&EXTI_InitStructure);
		
		//PC.2
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);
		EXTI_InitStructure.EXTI_Line=EXTI_Line2;			
		EXTI_Init(&EXTI_InitStructure);
	
		//PE.3
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
		EXTI_InitStructure.EXTI_Line=EXTI_Line3;			
		EXTI_Init(&EXTI_InitStructure);

		//初始化外部中断
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
		NVIC_Init(&NVIC_InitStructure); 
		//优先级设置为0，0	         PE.0	
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
		NVIC_Init(&NVIC_InitStructure); 
		//优先级设置为0，1

		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
		NVIC_Init(&NVIC_InitStructure); 
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
		NVIC_Init(&NVIC_InitStructure); 
		
}	

float Front_dist;
float  Front_tim=0;
								//前侧超声波PE.0
//外部中断函数
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		if(PEin(0)==0)    //最高位第8位为1  捕获到下降沿
		{
			Front_tim=Now_tim+(TIM2->CNT/1000.0)-Front_tim;
			Front_dist=Front_tim*170;
		}
		else   //捕获到上升沿
		{
			Front_tim=Now_tim+(TIM2->CNT/1000.0);
			
		}		
		EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
	}
}


								//左侧超声波PD.1

//PD.1   Left
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		if(PDin(1)==0)    // 捕获到下降沿
		{
			Left_tim=Now_tim+(TIM2->CNT/1000.0)-Left_tim;
			Left_dist=Left_tim*170;				
		}
		else   //捕获到上升沿
		{
			Left_tim=Now_tim+(TIM2->CNT/1000.0);			
		}		
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位  
	}
}


float LEft_dist=0;
float LEft_tim=0;
								//左侧超声波PD.1

//PE.3   Left2
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==SET)
	{
		if(PEin(3)==0)    // 捕获到下降沿
		{
			LEft_tim=Now_tim+(TIM2->CNT/1000.0)-LEft_tim;
			LEft_dist=LEft_tim*170;				
		}
		else   //捕获到上升沿
		{
			LEft_tim=Now_tim+(TIM2->CNT/1000.0);			
		}		
	EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE3上的中断标志位  
	}
}


								//右侧超声波PC.2
//PC.2  Right
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
			if(PCin(2)==0)    //最高位第8位为1  捕获到下降沿
			{
				Right_tim=Now_tim+(TIM2->CNT/1000.0)-Right_tim;
				Right_dist=Right_tim*170;
			}
			else   //捕获到上升沿
			{
				Right_tim=Now_tim+(TIM2->CNT/1000.0);
			}	
	EXTI_ClearITPendingBit(EXTI_Line2); //清除LINE0上的中断标志位  
	}		
}
void TIM2_Int_Init(u16 arr,u16 psc)   //计时
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 //定时器初始化
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 
	//允许更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	//中断优先级

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设


		EXTIX_Init();
		Ultrasonic_IO_Init();
}
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{	
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
				Now_tim++;
			
		}
}




float Read_dist(int a)
{
	float val;
	
	while(1)
	{
		Ultrasonic_Send(a);
		
		if(a==1)
		{
    while(1)
				{
					if(Left_dist)
					{
						val=Left_dist;
						Left_dist=0;
						if(val>25)
						return val;
					}
					else
					{
						if(Now_tim>20) return 0;
					}
				}
			
	  }
		
   else if(a==2)
		{
      while(!Front_dist);
			val=Front_dist;
			Front_dist=0;
			if(val>25)
			return val; 
			
	  }
		
		else if(a==3)
		{
      while(1)
				{
					if(Right_dist)
					{
						val=Right_dist;
						Right_dist=0;
						if(val>25)
						return val;
					}
					else
					{
						if(Now_tim>20) return 0;
					}
				}
			
	  }
		
		else if(a==4)
		{
      while(!LEft_dist);
			val=LEft_dist;
			LEft_dist=0;
			if(val>25)
			return val; 
			
	  }
		
		else return 0;
	}
}

float Ultrasonic_mid(u8 a)
{
	float dist[5][5];
  u8 i;
	float max=0,min=0,temp=0;
	
	for(i=0;i<3;i++)
	{
	dist[a][i]=Read_dist(a);
//	delay_ms(20);
	}
	
 	min = dist[a][0];

	for(i=0;i<3;i++)
	{	
	if(dist[a][i]<min)
		min = dist[a][i];
	
	if(dist[a][i]>max)
		max = dist[a][i];
	
	temp+=dist[a][i];
	}

  return (temp-(max+min));
}
