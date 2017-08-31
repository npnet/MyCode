#ifndef YD_MOVE_ALARM_H
#define YD_MOVE_ALARM_H

#include "Bird_gpscell.h"

#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_move_alarm_param(kal_uint8 interval_time);
/*描述：对位移报警参数的初始化*/

void bird_set_move_position_alarm_value(U32 distance);
/*描述：设置位移报警的报警距离*/

U32 bird_get_move_position_alarm_value();
/*描述：获取位移报警的报警距离*/

void bird_set_move_position_alarm_flag(U8 flag);
/*描述：设置位移报警标志位*/

U8 bird_get_enable_move_to_platform_flag();
/*描述：获取位移报警标志位*/

void yd_set_move_alarm_msg();
/*描述：设置位移报警消息句柄*/

void RJ_GPS_distance_handle(mdi_gps_nmea_gga_struct rmc_data);
/*描述：位移报警触发判断函数*/

#endif
