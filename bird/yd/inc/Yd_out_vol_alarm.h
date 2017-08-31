#ifndef YD_OUT_VOL_ALARM_H
#define YD_OUT_VOL_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_power_off_alarm_param(kal_uint8 interval_time);
/*���������ⲿ��Դ�Ͽ����������ĳ�ʼ��*/

void bird_set_poft_value(U32 value);
/*�����������ⲿ��Դ�ϵ籨����ʱ�����ʱ��*/

U32 bird_get_poft_value(void);
/*��������ȡ�ⲿ��Դ�ϵ籨����ʱ�����ʱ��*/

void bird_set_external_power_alarm_flag(U8 flag);
/*�����������ⲿ��Դ�Ͽ�������־λ*/

U8 bird_get_power_dis_to_platform_flag();
/*��������ȡ�ⲿ��Դ�Ͽ�������־λ*/

void yd_set_out_vol_alarm_msg();
/*�����������ⲿ��Դ�Ͽ�������Ϣ���*/

#endif
