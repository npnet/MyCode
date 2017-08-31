#ifndef YD_ROLLOVER_ALARM_H
#define YD_ROLLOVER_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_rollover_alarm_param(kal_uint8 interval_time);
/*�������Բ෭���������ĳ�ʼ��*/

U8 yd_tk001_get_rollover_alarm();
/*��������ȡ�෭������־λ*/

void yd_tk001_set_rollover_alarm(U8 flag);
/*���������ò෭������־λ*/

void yd_tk001_post_alarm_static_rollover();
/*��������ֹ�෭������Ϣ������*/

void yd_tk001_post_alarm_run_rollover();
/*�������˶��෭������Ϣ������*/

#endif
