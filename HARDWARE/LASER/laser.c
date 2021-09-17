#include "sys.h"
#include "laser.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"


//���ڷ��ͽ��ջ�����
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0;

void USART3_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
    {
        res =USART_ReceiveData(USART3);
        if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
        {
            if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//�����Խ�������
            {
                USART3_RX_BUF[USART3_RX_STA++]=res;	//��¼���յ���ֵ
                if(USART3_RX_BUF[0]!=0x7e)
                    USART3_RX_STA=0;
            } 
            if((USART3_RX_STA&0X7FFF)==USART3_MAX_RECV_LEN)
            {
                USART3_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
            }
        }
    }
}


//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void usart3_init(u32 bound)
{

//    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

    USART_DeInit(USART3);  //��λ����3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10

    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

    USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������	3


    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���

    //ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��ʱ�ж�

//    //�����ж����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//    USART3_RX_STA=0;		//����
}

void Laser_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //�����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    USART3_RX_STA=0;		//����

}

void Laser_on(void)
{
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
void Laser_off(void)
{
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}

int Measure(void)
{
    int dist;
    char buf[4];

	  Laser_on();   //�򿪴���
	
    while(!(USART3_RX_STA&0X8000));
    buf[0]=(char)USART3_RX_BUF[9];
    buf[1]=(char)USART3_RX_BUF[10];
    buf[2]=(char)USART3_RX_BUF[11];
    buf[3]=(char)USART3_RX_BUF[12];
    dist=(int)strtol(buf,NULL,16);
    USART3_RX_STA=0;
	
	  Laser_off();  //�رմ���
	
    return dist;
}
