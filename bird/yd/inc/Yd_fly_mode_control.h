#ifdef YD_FLY_MODE_CONTROL_H
#define YD_FLY_MODE_CONTROL_H

#include "Kal_general_types.h"

#ifndef bool
typedef kal_bool bool;
#endif

void Yd_switch_fly(bool bfly);
/*����������ģʽ�л�����*/

void yd_send_fly_mode_change_msg();
/*���������ͷ���ģʽ�л���Ϣ����*/

void yd_set_fly_mode_change_msg();
/*���������÷���ģʽ�л���Ϣ�������*/

#endif
