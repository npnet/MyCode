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
/*��������ACC���Ʋ����ĳ�ʼ��*/

void bird_set_acc_state(U8 flag);
/*����������ACC״̬��־λ*/

U8 bird_get_acc_state();
/*��������ȡACC״̬��־λ*/

U8 bird_get_acclock_value();
/*��������ȡACCLOCK״̬��־λ*/

void bird_set_acclock_value(U8 mvalue);
/*����������ACCLOCK״̬��־λ*/

MMI_BOOL get_Acc_onoff();
/*�������ж�ACC״̬��־λ�Ƿ�Ϊ1*/

void yd_tk001_post_alarm_key_on();
/*��������ECUʱ��ACC������Ϣ������*/

void yd_tk001_post_alarm_key_off();
/*��������ECUʱ��ACC�Ͽ���Ϣ������*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_set_ECU_command_acc_on_msg();
/*����������ECU����ACC��������Ϣ���*/

void yd_set_ECU_command_acc_off_msg();
/*����������ECU����ACC�ر��������Ϣ���*/

void yd_set_ECU_key_on_msg();
/*��������ECUʱ������ACC������Ϣ���*/

void yd_set_ECU_key_off_msg();
/*��������ECUʱ������ACC�Ͽ���Ϣ���*/

#endif

void yd_set_aux_key_on_msg();
/*����������ACC������Ϣ���*/

void yd_set_aux_key_off_msg();
/*����������ACC�Ͽ���Ϣ���*/

void yd_set_acc_key_closed_msg();
/**/

void Yd_acc_close_gps();
/*�������ն˶ϵ�������ACC�Ͽ�ʱ�ر�GPS�Ĵ�����*/

#endif
