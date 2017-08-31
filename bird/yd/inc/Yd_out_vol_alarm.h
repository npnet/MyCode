#ifndef YD_OUT_VOL_ALARM_H
#define YD_OUT_VOL_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_power_off_alarm_param(kal_uint8 interval_time);
/*描述：对外部电源断开报警参数的初始化*/

void bird_set_poft_value(U32 value);
/*描述：设置外部电源断电报警延时处理的时间*/

U32 bird_get_poft_value(void);
/*描述：获取外部电源断电报警延时处理的时间*/

void bird_set_external_power_alarm_flag(U8 flag);
/*描述：设置外部电源断开报警标志位*/

U8 bird_get_power_dis_to_platform_flag();
/*描述：获取外部电源断开报警标志位*/

void yd_set_out_vol_alarm_msg();
/*描述：设置外部电源断开报警消息句柄*/

#endif
