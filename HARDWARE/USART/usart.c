//#include "usart.h"
//#include "stdarg.h"
//#include "stdio.h"
//#include "string.h"

////////////////////////////////////////////////////////////////////////////////////
////����3��������
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2015/3/29
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved
////////////////////////////////////////////////////////////////////////////////////

////���ڽ��ջ�����
//u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
//u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//u8 UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
//u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�



////ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
////���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
////�κ�����,���ʾ�˴ν������.
////���յ�������״̬
////[15]:0,û�н��յ�����;1,���յ���һ������.
////[14:0]:���յ������ݳ���
//vu16 USART3_RX_STA=0;
//vu16 UART5_RX_STA=0;

//void USART3_IRQHandler(void)
//{
//    u8 res;
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
//    {
//        res =USART_ReceiveData(USART3);
//        if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//        {
//            if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//�����Խ�������
//            {
//                USART3_RX_BUF[USART3_RX_STA++]=res;	//��¼���յ���ֵ
//                if(USART3_RX_BUF[0]!=0x55)
//                    USART3_RX_STA=0;
//            } 
//            if((USART3_RX_STA&0X7FFF)==USART3_MAX_RECV_LEN)
//            {
//                USART3_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
//            }
//        }
//    }
//}


////��ʼ��IO ����3
////pclk1:PCLK1ʱ��Ƶ��(Mhz)
////bound:������
//void usart3_init(u32 bound)
//{

//    NVIC_InitTypeDef NVIC_InitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

//    USART_DeInit(USART3);  //��λ����3
//    //USART3_TX   PB10
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10

//    //USART3_RX	  PB11
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

//    USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//    USART_Init(USART3, &USART_InitStructure); //��ʼ������	3


//    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���

//    //ʹ�ܽ����ж�
//    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��ʱ�ж�

//    //�����ж����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//    USART3_RX_STA=0;		//����
//}

//void Jy61_init()
//{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    //�����ж����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//    USART3_RX_STA=0;		//����

//}






////����5,printf ����
////ȷ��һ�η������ݲ�����UART5_MAX_SEND_LEN�ֽ�
//void u5_printf(char* fmt,...)
//{
//    u16 i,j;
//    va_list ap;
//    va_start(ap,fmt);
//    vsprintf((char*)UART5_TX_BUF,fmt,ap);
//    va_end(ap);
//    i=strlen((const char*)UART5_TX_BUF);		//�˴η������ݵĳ���
//    for(j=0; j<i; j++)							//ѭ����������
//    {
//        while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
//        USART_SendData(UART5,UART5_TX_BUF[j]);
//    }
//}

//void uart5_init(u32 bound)
//{

//    NVIC_InitTypeDef NVIC_InitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	// GPIOCʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);  //����5ʱ��ʹ��

//    USART_DeInit(UART5);  //��λ����3
//    //USART5_TX   PC12
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC10

//    //USART5_RX	  PD2
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//    GPIO_Init(GPIOD, &GPIO_InitStructure);  //��ʼ��PD2

//    USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//    USART_Init(UART5, &USART_InitStructure); //��ʼ������	5


//    USART_Cmd(UART5, ENABLE);                     //ʹ�ܴ���

//    //ʹ�ܽ����ж�
//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�

//    //�����ж����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//    UART5_RX_STA=0;		//����
//}


////ÿ�յ�һ���ֽڵ����ݣ��ͽ�һ���ж�
//void UART5_IRQHandler(void)
//{
//    u8 res;
//    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//���յ�����
//    {
//        res =USART_ReceiveData(UART5);
//        if((UART5_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//        {
//            if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//�����Խ�������
//            {
//                UART5_RX_BUF[UART5_RX_STA++]=res;	//��¼���յ���ֵ
//            }
//        }
//        if((UART5_RX_STA&0X7FFF)==UART5_MAX_RECV_LEN)
//        {
//            UART5_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
//        }
//    }
//}


