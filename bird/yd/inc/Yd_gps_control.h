#ifndef YD_GPS_CONTROL_H
#define YD_GPS_CONTROL_H



void yd_restart_gps(void);
/*�������ȹرպ�����GPS�ĺ���*/

void yd_set_gps_open_msg();
/*���������ô�GPS��Ϣ���*/

void yd_set_gps_close_msg();
/*���������ùر�GPS��Ϣ���*/

void yd_send_gps_open_msg();
/*����������Ϣ���з��ʹ�GPS��Ϣ*/

void yd_send_gps_close_msg();
/*����������Ϣ���з��͹ر�GPS��Ϣ*/


#endif
