#ifndef YD_WAKEUP_HANDLE_H
#define YD_WAKEUP_HANDLE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void yd_init_wakeup_param();
/*�������Ի��Ѳ����ĳ�ʼ��*/

void bird_set_wakeup_value(U8 value);
/*���������û��ѿ���λ*/

U8 bird_get_wakeup_value(void);
/*��������ȡ���ѿ���λ*/

void bird_set_wakeupt_value(U32 value);
/*�������������߷�����������ֵ*/

U32 bird_get_wakeupt_value(void);
/*��������ȡ���߷�����������ֵ*/

void Yd_run_for_wakeup_device();
/*�������豸���Ѻ���*/

void yd_tk001_wakeup_function();
/*���������Ѵ�����*/


#endif
