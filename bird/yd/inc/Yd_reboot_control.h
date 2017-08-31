#ifndef YD_REBOOT_CONTROL_H
#define YD_REBOOT_CONTROL_H

#ifndef U8
typedef unsigned char U8;
#endif

void yd_init_reboot_control_param(void);
/*描述：对重启控制参数的初始化*/

U8 yd_tk001_get_reboot_flag(void);
/*描述：获取重启标志位*/

void yd_tk001_set_reboot_flag(U8 flag);
/*描述：设置重启标志位*/

void RJ_SYS_ReStart();
/*描述：做重启前的处理并通过驱动实现重启*/

void yd_send_reboot_control_msg();
/*描述：将重启消息发送到队列*/

void yd_set_reboot_control_msg();
/*描述：设置重启消息句柄*/

void Yd_24reset();
/*描述：24小时一次的2点重启函数*/

void yd_set_uart_reboot_msg();
/*描述：设置ECU重启消息句柄*/

#endif
