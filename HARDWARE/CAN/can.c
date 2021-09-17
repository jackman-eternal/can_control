#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.1 
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.1�޸�˵�� 20150528
//������CAN��ʼ�����������ע�ͣ������˲����ʼ��㹫ʽ
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //jie TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;        //jie RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART=ENABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode= mode;	        //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 
	//���ò�����
	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//��ʼ��CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber=0;	//������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ�� 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0

	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1, 0, &RxMessage);
	for(i=0;i<8;i++)
	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X140,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send1_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x140;			// ��׼��ʶ�� 
	TxMessage.ExtId=1;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=len;				// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}

//can����һ������(�̶���ʽ:IDΪ0X140,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send2_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x140;			// ��׼��ʶ�� 
	TxMessage.ExtId=2;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=len;				// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}





u8 Can_Send_angle(uint8_t SpinDirection,uint16_t angle,uint16_t max_speed,uint16_t motor_id)
{
    u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
    TxMessage.StdId=0x140+motor_id;			// ��׼��ʶ�� 
	TxMessage.ExtId=0;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
    TxMessage.DLC=8;
    TxMessage.Data[0]=0xa6;
    TxMessage.Data[1]=SpinDirection;
    TxMessage.Data[2]=max_speed;
    TxMessage.Data[3]=max_speed>>8;
    TxMessage.Data[4]=angle;
    TxMessage.Data[5]=angle>>8;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	
}

u8 Can_Send_torque(int16_t motor_current,uint16_t motor_id)
{
    u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
    TxMessage.StdId=0x140+motor_id;			// ��׼��ʶ�� 
	TxMessage.ExtId=0;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
    TxMessage.DLC=8;
    TxMessage.Data[0]=0xa1;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=motor_current;
    TxMessage.Data[5]=motor_current>>8;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;
}

u8 Can_Send_speed(int32_t motor_speed,uint16_t motor_id)
{
    u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
    TxMessage.StdId=0x140+motor_id;			// ��׼��ʶ�� 
	TxMessage.ExtId=0;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
    TxMessage.DLC=8;
    TxMessage.Data[0]=0xa2;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=motor_speed;
    TxMessage.Data[5]=motor_speed>>8;
    TxMessage.Data[6]=motor_speed>>16;
    TxMessage.Data[7]=motor_speed>>24;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;
}

u8 Can_Send_angles(uint16_t speed,uint32_t angle,uint16_t motor_id)
{
    u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
    TxMessage.StdId=0x140+motor_id;			// ��׼��ʶ�� 
	TxMessage.ExtId=0;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
    TxMessage.DLC=8;
    TxMessage.Data[0]=0xa4;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=speed;
    TxMessage.Data[3]=speed>>8;
    TxMessage.Data[4]=angle;
    TxMessage.Data[5]=angle>>8;
    TxMessage.Data[6]=angle>>16;
    TxMessage.Data[7]=angle>>24;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	
}

u8 Can_Stop(uint16_t motor_id)
{
    u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
    TxMessage.StdId=0x140+motor_id;			// ��׼��ʶ�� 
	TxMessage.ExtId=0;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
    TxMessage.DLC=8;
    TxMessage.Data[0]=0x81;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=0;
    TxMessage.Data[5]=0;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;
}

u8 Can_Run(uint16_t motor_id)
{
  u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
  TxMessage.StdId=0x140+motor_id;		
	TxMessage.ExtId=0;			
	TxMessage.IDE=CAN_Id_Standard; 
	TxMessage.RTR=CAN_RTR_Data;		
    TxMessage.DLC=8;
    TxMessage.Data[0]=0x88;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=0;
    TxMessage.Data[5]=0;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//??????
	if(i>=0XFFF)return 1;
	return 0;	
}

u8 Can_Shutdown(uint16_t motor_id)
{
  u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
  TxMessage.StdId=0x140+motor_id;		
	TxMessage.ExtId=0;			
	TxMessage.IDE=CAN_Id_Standard; 
	TxMessage.RTR=CAN_RTR_Data;		
    TxMessage.DLC=8;
    TxMessage.Data[0]=0x80;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=0;
    TxMessage.Data[5]=0;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//??????
	if(i>=0XFFF)return 1;
	return 0;	
}

u8 Can_Read_Angles(uint16_t motor_id)
{
  u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
  TxMessage.StdId=0x140+motor_id;		
	TxMessage.ExtId=0;			
	TxMessage.IDE=CAN_Id_Standard; 
	TxMessage.RTR=CAN_RTR_Data;		
    TxMessage.DLC=8;
    TxMessage.Data[0]=0x92;
    TxMessage.Data[1]=0;
    TxMessage.Data[2]=0;
    TxMessage.Data[3]=0;
    TxMessage.Data[4]=0;
    TxMessage.Data[5]=0;
    TxMessage.Data[6]=0;
    TxMessage.Data[7]=0;
    mbox= CAN_Transmit(CAN1, &TxMessage); 
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//??????
	if(i>=0XFFF)return 1;
	return 0;	
}

u32 get_angle(void)
{
   u8 *buf;
	 u32 angle;
   Can_Receive_Msg(buf);
	 angle=((buf[0])+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24));
	 return angle;
}