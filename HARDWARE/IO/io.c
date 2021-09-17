#include "io.h"
#include "sys.h"
#include "delay.h"


void BEEP_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//�����趨������ʼ��
}
void beep_start(void)
{
    PBout(8)=1;
	  delay_ms(500);
	  PBout(8)=0;
}
void IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;				 //PE6 PE5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //    ��⵽���0����֮���1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
  GPIO_SetBits(GPIOA,GPIO_Pin_6);						 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	  				
  GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //�������    ��⵽���0����֮���1	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	    	//SHURU	          1-found   0-notfound
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //XIALA
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	  				
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    	//shuchu           1-work   0-sleep
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	  				
}

