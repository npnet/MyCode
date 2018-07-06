#ifndef YD_SPEED_ALARM_H
#define YD_SPEED_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif



void yd_init_speed_alarm_param(kal_uint8 interval_time);
/*�������Գ��ٱ���������ʼ��*/

U8 bird_get_speed_alarm_flag();
/*��������ȡ���ٱ�����־λ*/

void bird_speed_alarm_flag(U8 flag);
/*���������ó��ٱ�����־λ*/

void bird_speed_alarm_value(U32 value);
/*���������ó��ٱ����ٶȱ���ֵ*/

U32 bird_get_speed_alarm_value();
/*��������ȡ���ٱ����ٶȱ���ֵ*/

void yd_set_speed_alarm_msg();
/*���������ó��ٱ�����Ϣ���*/


#ifdef RJ_GPS_APP

void RJ_GPS_speed_handle(float gps_speed);
/*������GPS�ж��Ƿ�����Ϣ���з��ͳ��ٱ�����Ϣ����*/

float yd_get_speed();
/*��������ȡʵ���ٶ�ֵ*/

#ifdef BIRD_ECU_MMI_SUPPORT

void Bird_ecu_speed_handle();
/*������ECU�ж��Ƿ�����Ϣ���з��ͳ��ٱ�����Ϣ����*/

#endif

#endif

#endif
