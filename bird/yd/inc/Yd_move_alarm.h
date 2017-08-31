#ifndef YD_MOVE_ALARM_H
#define YD_MOVE_ALARM_H

#include "Bird_gpscell.h"

#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U8
typedef unsigned char U8;
#endif


void yd_init_move_alarm_param(kal_uint8 interval_time);
/*��������λ�Ʊ��������ĳ�ʼ��*/

void bird_set_move_position_alarm_value(U32 distance);
/*����������λ�Ʊ����ı�������*/

U32 bird_get_move_position_alarm_value();
/*��������ȡλ�Ʊ����ı�������*/

void bird_set_move_position_alarm_flag(U8 flag);
/*����������λ�Ʊ�����־λ*/

U8 bird_get_enable_move_to_platform_flag();
/*��������ȡλ�Ʊ�����־λ*/

void yd_set_move_alarm_msg();
/*����������λ�Ʊ�����Ϣ���*/

void RJ_GPS_distance_handle(mdi_gps_nmea_gga_struct rmc_data);
/*������λ�Ʊ��������жϺ���*/

#endif
