#ifdef YD_FLY_MODE_CONTROL_H
#define YD_FLY_MODE_CONTROL_H

#include "Kal_general_types.h"

#ifndef bool
typedef kal_bool bool;
#endif

void Yd_switch_fly(bool bfly);
/*描述：飞行模式切换函数*/

void yd_send_fly_mode_change_msg();
/*描述：发送飞行模式切换消息函数*/

void yd_set_fly_mode_change_msg();
/*描述：设置飞行模式切换消息句柄函数*/

#endif
