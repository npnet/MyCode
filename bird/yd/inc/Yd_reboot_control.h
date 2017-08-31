#ifndef YD_REBOOT_CONTROL_H
#define YD_REBOOT_CONTROL_H

#ifndef U8
typedef unsigned char U8;
#endif

void yd_init_reboot_control_param(void);
/*���������������Ʋ����ĳ�ʼ��*/

U8 yd_tk001_get_reboot_flag(void);
/*��������ȡ������־λ*/

void yd_tk001_set_reboot_flag(U8 flag);
/*����������������־λ*/

void RJ_SYS_ReStart();
/*������������ǰ�Ĵ���ͨ������ʵ������*/

void yd_send_reboot_control_msg();
/*��������������Ϣ���͵�����*/

void yd_set_reboot_control_msg();
/*����������������Ϣ���*/

void Yd_24reset();
/*������24Сʱһ�ε�2����������*/

void yd_set_uart_reboot_msg();
/*����������ECU������Ϣ���*/

#endif
