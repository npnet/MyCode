#ifndef YD_SPEED_ALARM_H
#define YD_SPEED_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif



void yd_init_speed_alarm_param(kal_uint8 interval_time);
/*描述：对超速报警参数初始化*/

U8 bird_get_speed_alarm_flag();
/*描述：获取超速报警标志位*/

void bird_speed_alarm_flag(U8 flag);
/*描述：设置超速报警标志位*/

void bird_speed_alarm_value(U32 value);
/*描述：设置超速报警速度报警值*/

U32 bird_get_speed_alarm_value();
/*描述：获取超速报警速度报警值*/

void yd_set_speed_alarm_msg();
/*描述：设置超速报警消息句柄*/


#ifdef RJ_GPS_APP

void RJ_GPS_speed_handle(float gps_speed);
/*描述：GPS判断是否向消息队列发送超速报警消息函数*/

float yd_get_speed();
/*描述：获取实际速度值*/

#ifdef BIRD_ECU_MMI_SUPPORT

void Bird_ecu_speed_handle();
/*描述：ECU判断是否向消息队列发送超速报警消息函数*/

#endif

#endif

#endif
