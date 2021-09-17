#ifndef __GYRO_H
#define __GYRO_H

void Initial_UART2(unsigned long baudrate);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);

void sendcmd(char cmd[]);
void CopeSerial2Data(unsigned char ucData);



#endif

//------------------End of File----------------------------


