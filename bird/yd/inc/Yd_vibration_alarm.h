#ifndef YD_VIBRATION_ALARM_H
#define YD_VIBRATION_ALARM_H

#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U16
typedef unsigned short int U16;
#endif


U8 g_vibration_is_timer_count_start = 0;/*震动定时器开启标记*/
U8 g_vibration_count = 0;/*震动定时时间内震动次数*/
U8 g_vibration_is_timer_start = 0;/*震动延时报警定时器开启标记*/

void yd_init_vibration_alarm_param(kal_uint8 interval_time);
/*描述：对震动报警参数初始化*/

U16 bird_get_vibration_running_alarm_level();
/*描述：获取震动报警标准值*/

U16 bird_get_vibration_running_alarm_times_value();
/*描述：获取震动次数报警值*/

void bird_vibration_running_alarm_value(U16 argv1,U16 argv2);
/*描述：设定震动报警标准值与振动次数报警值*/

void bird_set_vibration_sensitivity_value(U8 level);
/*描述：设定震动灵敏度*/

U8 bird_get_vibration_sensitivity_value();
/*描述：获取震动灵敏度*/

void bird_set_vibration_running_alarm_flag(U8 flag);
/*描述：设置震动报警标志*/

U8 bird_get_vibration_running_alarm_flag();
/*描述：获取震动报警标志*/

void yd_tk001_post_alarm_vibration_ext();
/*描述：对震动报警的预处理*/

void yd_tk001_alarm_vibration_count();
/*描述：重置震动次数与震动定时器开启标志位*/

#ifdef BIRD_ECU_MMI_SUPPORT

void yd_set_uart_vibration_alarm_msg();
/*描述：设置串口震动报警消息句柄*/

#endif


#endif
