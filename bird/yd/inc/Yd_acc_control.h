#ifndef YD_ACC_CONTROL_H
#define YD_ACC_CONTROL_H

#include "Kal_general_types.h"

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef MMI_BOOL
#define MMI_BOOL kal_bool
#endif



void yd_init_acc_control_param();
/*描述：对ACC控制参数的初始化*/

void bird_set_acc_state(U8 flag);
/*描述：设置ACC状态标志位*/

U8 bird_get_acc_state();
/*描述：获取ACC状态标志位*/

U8 bird_get_acclock_value();
/*描述：获取ACCLOCK状态标志位*/

void bird_set_acclock_value(U8 mvalue);
/*描述：设置ACCLOCK状态标志位*/

MMI_BOOL get_Acc_onoff();
/*描述：判断ACC状态标志位是否为1*/

void yd_tk001_post_alarm_key_on();
/*描述：无ECU时，ACC连接消息处理函数*/

void yd_tk001_post_alarm_key_off();
/*描述：无ECU时，ACC断开消息处理函数*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_set_ECU_command_acc_on_msg();
/*描述：设置ECU发送ACC打开命令消息句柄*/

void yd_set_ECU_command_acc_off_msg();
/*描述：设置ECU发送ACC关闭命令处理消息句柄*/

void yd_set_ECU_key_on_msg();
/*描述：带ECU时，设置ACC连接消息句柄*/

void yd_set_ECU_key_off_msg();
/*描述：带ECU时，设置ACC断开消息句柄*/

#endif

void yd_set_aux_key_on_msg();
/*描述：设置ACC连接消息句柄*/

void yd_set_aux_key_off_msg();
/*描述：设置ACC断开消息句柄*/

void yd_set_acc_key_closed_msg();
/**/

void Yd_acc_close_gps();
/*描述：终端断电心跳且ACC断开时关闭GPS的处理函数*/

#endif
