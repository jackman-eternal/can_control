#include "sys.h"
#include "laser.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"


//串口发送接收缓存区
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART3_RX_STA=0;

void USART3_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
    {
        res =USART_ReceiveData(USART3);
        if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
        {
            if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//还可以接收数据
            {
                USART3_RX_BUF[USART3_RX_STA++]=res;	//记录接收到的值
                if(USART3_RX_BUF[0]!=0x7e)
                    USART3_RX_STA=0;
            } 
            if((USART3_RX_STA&0X7FFF)==USART3_MAX_RECV_LEN)
            {
                USART3_RX_STA|=1<<15;				//强制标记接收完成
            }
        }
    }
}


//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void usart3_init(u32 bound)
{

//    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

    USART_DeInit(USART3);  //复位串口3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10

    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11

    USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口	3


    USART_Cmd(USART3, ENABLE);                    //使能串口

    //使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//暂时中断

//    //设置中断优先级
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

//    USART3_RX_STA=0;		//清零
}

void Laser_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //设置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    USART3_RX_STA=0;		//清零

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

	  Laser_on();   //打开串口
	
    while(!(USART3_RX_STA&0X8000));
    buf[0]=(char)USART3_RX_BUF[9];
    buf[1]=(char)USART3_RX_BUF[10];
    buf[2]=(char)USART3_RX_BUF[11];
    buf[3]=(char)USART3_RX_BUF[12];
    dist=(int)strtol(buf,NULL,16);
    USART3_RX_STA=0;
	
	  Laser_off();  //关闭串口
	
    return dist;
}
