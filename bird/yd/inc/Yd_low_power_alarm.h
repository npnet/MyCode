#ifndef YD_LOW_POWER_ALARM_H
#define YD_LOW_POWER_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif



void yd_init_low_power_alarm_param(kal_uint8 interval_time);
/*描述：对低电报警参数的初始化*/

void bird_set_low_power_alarm_flag(U8 flag);
/*描述：设置低电报警标志位*/

U8 bird_get_low_power_alarm_flag();
/*描述：获取低电报警标志位*/

void bird_set_bvl(U32 flag);
/*描述：设置低电报警阀值*/

U32 bird_get_bvl();
/*描述：获取低电报警阀值*/

void yd_set_low_power_alarm_msg();
/*描述：设置低电报警消息句柄*/


#endif
