#ifndef YD_LOW_POWER_ALARM_H
#define YD_LOW_POWER_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif



void yd_init_low_power_alarm_param(kal_uint8 interval_time);
/*�������Ե͵籨�������ĳ�ʼ��*/

void bird_set_low_power_alarm_flag(U8 flag);
/*���������õ͵籨����־λ*/

U8 bird_get_low_power_alarm_flag();
/*��������ȡ�͵籨����־λ*/

void bird_set_bvl(U32 flag);
/*���������õ͵籨����ֵ*/

U32 bird_get_bvl();
/*��������ȡ�͵籨����ֵ*/

void yd_set_low_power_alarm_msg();
/*���������õ͵籨����Ϣ���*/


#endif
