#ifndef BIRD_MILEAGE_H
#define BIRD_MILEAGE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef S32
typedef signed int S32;
#endif

U8 Yd_get_lc_static_count();
/*��������ȡ����ϱ�����ʱ������ֹ�Ĵ���*/

U8 Yd_get_lc_static_time();
/*�������ж�����ϱ�����ʱ������ֹʱ���Ƿ񳬹�4����*/

U8 Yd_get_lc_station_count();
/*��������ȡ����ϱ�����ʱ������״̬�����ǳ��������ô���*/

void bird_set_lc_tj();
/*������ͳ�������*/

S32 bird_get_lc_tj();
/*��������ȡ�����*/

void bird_ini_lc_tj();
/*��������ʼ�����λ����Ϣ�ṹ��*/

void Yd_lc();
/*����������ϱ����ƺ���*/

#endif
