#ifndef YD_RUN_STATIC_EVENT_H
#define YD_RUN_STATIC_EVENT_H

#ifndef U8
typedef unsigned char U8;
#endif


void yd_set_static_event_msg();
/*描述：设置静止事件消息句柄*/

void yd_set_run_event_msg();
/*描述：设置运动事件消息句柄*/

U8 get_uart_run_status();
/*描述：获取uart动静状态标志*/

void yd_set_uart_static_event_msg();
/*描述：设置uart静止事件消息句柄*/

void yd_set_uart_run_event_msg();
/*描述：设置uart运动事件消息句柄*/


#endif
