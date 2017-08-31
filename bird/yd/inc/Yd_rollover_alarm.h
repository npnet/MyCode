#ifndef YD_ROLLOVER_ALARM_H
#define YD_ROLLOVER_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_rollover_alarm_param(kal_uint8 interval_time);
/*描述：对侧翻报警参数的初始化*/

U8 yd_tk001_get_rollover_alarm();
/*描述：获取侧翻报警标志位*/

void yd_tk001_set_rollover_alarm(U8 flag);
/*描述：设置侧翻报警标志位*/

void yd_tk001_post_alarm_static_rollover();
/*描述：静止侧翻报警消息处理函数*/

void yd_tk001_post_alarm_run_rollover();
/*描述：运动侧翻报警消息处理函数*/

#endif
