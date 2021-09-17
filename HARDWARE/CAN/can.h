#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
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
 
//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 Can_Send1_Msg(u8* msg,u8 len);
//��������
u8 Can_Send2_Msg(u8* msg,u8 len);

u8 Can_Send_angle(uint8_t SpinDirection,uint16_t angle,uint16_t max_speed,uint16_t motor_id);

u8 Can_Send_torque(int16_t motor_current,uint16_t motor_id);

u8 Can_Receive_Msg(u8 *buf);							//��������

u8 Can_Send_speed(int32_t motor_speed,uint16_t motor_id);

u8 Can_Stop(uint16_t motor_id);

u8 Can_Send_angles(uint16_t SpinDirection,uint32_t angle,uint16_t motor_id);

u8 Can_Run(uint16_t motor_id);

u8 Can_Shutdown(uint16_t motor_id);

u8 Can_Read_Angles(uint16_t motor_id);

u32 get_angle(void);

#endif

















