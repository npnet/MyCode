#ifndef YD_GPS_CONTROL_H
#define YD_GPS_CONTROL_H



void yd_restart_gps(void);
/*描述：先关闭后重启GPS的函数*/

void yd_set_gps_open_msg();
/*描述：设置打开GPS消息句柄*/

void yd_set_gps_close_msg();
/*描述：设置关闭GPS消息句柄*/

void yd_send_gps_open_msg();
/*描述：向消息队列发送打开GPS消息*/

void yd_send_gps_close_msg();
/*描述：向消息队列发送关闭GPS消息*/


#endif
