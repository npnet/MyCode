#ifndef BIRD_OILCUT_H
#define BIRD_OILCUT_H


#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif


void bird_init_oilcut_param(void);
/*�������Զ���ָ������ĳ�ʼ��*/

void bird_set_ocin_state(U8 flag);
/*���������ö�����Ϣ��ʶ(0λ�ָ���·��1Ϊ�Ͽ���·)*/

U8 bird_get_ocin_state(void);
/*��������ȡ������Ϣ��ʶ*/

void bird_set_oc_instruct(U32 instruct);
/*���������ö���ָ��ID*/

U32 bird_get_oc_instruct(void);
/*��������ȡ����ָ��ID*/

void bird_set_oc_exec_state(U8 flag);
/*���������ö�����Ϣִ��״̬*/

U8 bird_get_oc_exec_state(void);
/*��������ȡ������Ϣִ��״̬*/

void bird_set_oc_main_heart_time(U8 flag);
/*���������ö���ָ���ѯ����ʱ����ʱ��*/

U8 bird_get_oc_main_heart_time(void);
/*��������ȡ����ָ���ѯ����ʱ����ʱ��*/

void bird_ocin_init(void);
/*��������ʼ����������״̬*/

void bird_set_dy_state(U8 flag);
/*������������������״̬*/

void Yd_oc_heart_handle(void);
/*����������ָ���ѯ����*/

void bird_oilcut_accsleep_handler(void);
/*������ACC����ʱ�Զ���ָ��Ĵ�����*/

void bird_oc_logic_res(void);
/*������������ϢԤ������*/

void bird_ocin_state_res(U8 flag);
/*����������ָ�����ö�����Ϣ��ʶ�ĺ���*/

#endif
