#ifndef YD_AZ_ALARM_H
#define YD_AZ_ALARM_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_az_alarm_param(kal_uint8 interval_time);
/*�������԰������������ĳ�ʼ��*/

U8 yd_tk001_get_az_flag();
/*���� ����ȡ������־λ*/

void yd_tk001_set_az_flag(U8 flag);
/*���������ð�����־λ*/

void yd_set_az_alarm_msg();
/*���������ð���������Ϣ���*/

void yd_defense_az_alarm();
/*���������ʱ���ݰ�����־λ���а�������*/

#endif
