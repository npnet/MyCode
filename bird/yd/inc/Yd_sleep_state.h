#ifndef YD_SLEEP_STATE_H
#define YD_SLEEP_STATE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif


void yd_init_sleep_param();
/*�����������߱�־λ���г�ʼ���ĺ���*/

U8 bird_get_normal_sleep_state();
/*��������ȡ����״̬�ĺ���*/

void bird_set_normal_sleep_state(U8 flag);
/*��������������״̬�ĺ���*/

U8 bird_is_normal_sleep_on(void);
/*��������ȡ������߱�־λ�ĺ���*/

void bird_set_normal_sleep_flag(U8 flag);
/*���������ò�����߱�־λ�ĺ���*/

U32 bird_get_normal_sleep_value();
/*��������ȡ�����ʱ����ʱ��ĺ���*/

void bird_set_normal_sleep_value(U32 time);
/*���������ò����ʱ����ʱ��ĺ���*/

U8 bird_is_abnormal_sleep_on(void);
/*��������ȡ�ϵ����߱�־λ�ĺ���*/

void bird_set_abnormal_sleep_flag(U8 flag);
/*���������öϵ����߱�־λ�ĺ���*/

U32 bird_get_abnormal_sleep_value();
/*��������ȡ�ϵ���ʱ����ʱ��ĺ���*/

void bird_set_abnormal_sleep_value(U32 time);
/*���������öϵ���ʱ����ʱ��ĺ���*/

U8 Yd_is_sleep_on(void);
/*��������ȡ���߱�־λ(����������߱�־���ϵ���ϵ����߱�־)*/

void Yd_sleeppos_rs(void);
/*�������������ɵ����ߵĴ�����*/

void Yd_sleep_sendheart();
/*���������߷��������ĺ���*/

void Yd_stop_sleepTimer();
/*�������ر����߶�ʱ���ĺ���*/

void Yd_normal_sleep();
/*���������ߴ�����*/

#endif
