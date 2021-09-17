#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H	 
#include "sys.h"
  
	
	
void Ultrasonic_IO_Init(void);
void Ultrasonic_Send(int a);
float Read_dist(int a);
float Ultrasonic_mid(u8 a);



void EXTIX_Init(void); 
void TIM2_Int_Init(u16 arr,u16 psc);
#endif
