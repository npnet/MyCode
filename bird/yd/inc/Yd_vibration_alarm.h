#ifndef YD_VIBRATION_ALARM_H
#define YD_VIBRATION_ALARM_H

#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U16
typedef unsigned short int U16;
#endif


U8 g_vibration_is_timer_count_start = 0;/*�𶯶�ʱ���������*/
U8 g_vibration_count = 0;/*�𶯶�ʱʱ�����𶯴���*/
U8 g_vibration_is_timer_start = 0;/*����ʱ������ʱ���������*/

void yd_init_vibration_alarm_param(kal_uint8 interval_time);
/*���������𶯱���������ʼ��*/

U16 bird_get_vibration_running_alarm_level();
/*��������ȡ�𶯱�����׼ֵ*/

U16 bird_get_vibration_running_alarm_times_value();
/*��������ȡ�𶯴�������ֵ*/

void bird_vibration_running_alarm_value(U16 argv1,U16 argv2);
/*�������趨�𶯱�����׼ֵ���񶯴�������ֵ*/

void bird_set_vibration_sensitivity_value(U8 level);
/*�������趨��������*/

U8 bird_get_vibration_sensitivity_value();
/*��������ȡ��������*/

void bird_set_vibration_running_alarm_flag(U8 flag);
/*�����������𶯱�����־*/

U8 bird_get_vibration_running_alarm_flag();
/*��������ȡ�𶯱�����־*/

void yd_tk001_post_alarm_vibration_ext();
/*���������𶯱�����Ԥ����*/

void yd_tk001_alarm_vibration_count();
/*�����������𶯴������𶯶�ʱ��������־λ*/

#ifdef BIRD_ECU_MMI_SUPPORT

void yd_set_uart_vibration_alarm_msg();
/*���������ô����𶯱�����Ϣ���*/

#endif


#endif
