#ifndef YD_RUN_STATIC_EVENT_H
#define YD_RUN_STATIC_EVENT_H

#ifndef U8
typedef unsigned char U8;
#endif


void yd_set_static_event_msg();
/*���������þ�ֹ�¼���Ϣ���*/

void yd_set_run_event_msg();
/*�����������˶��¼���Ϣ���*/

U8 get_uart_run_status();
/*��������ȡuart����״̬��־*/

void yd_set_uart_static_event_msg();
/*����������uart��ֹ�¼���Ϣ���*/

void yd_set_uart_run_event_msg();
/*����������uart�˶��¼���Ϣ���*/


#endif
