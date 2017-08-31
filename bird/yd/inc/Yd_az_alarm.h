#ifndef YD_AZ_ALARM_H
#define YD_AZ_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_az_alarm_param(kal_uint8 interval_time);
/*描述：对鞍座报警参数的初始化*/

U8 yd_tk001_get_az_flag();
/*描述 ：获取鞍座标志位*/

void yd_tk001_set_az_flag(U8 flag);
/*描述：设置鞍座标志位*/

void yd_set_az_alarm_msg();
/*描述：设置鞍座报警消息句柄*/

void yd_defense_az_alarm();
/*描述：设防时根据鞍座标志位进行鞍座报警*/

#endif
