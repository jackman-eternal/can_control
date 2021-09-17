#include "sys.h"

extern float standard_angle;


void go_straight(void);
void turn_left(void);
void turn_right(void);
void stop(void);
void run(void);
void shutdown(void);
void gyro_init(void);
void sendcmd(char cmd[]);
void CopeSerial2Data(unsigned char ucData);
void go_straight_long(int a,int b);
void go_after_turn(int a);
void test(void);
void adjust(void);
void go_start(void);
void turn_left_last(void);

