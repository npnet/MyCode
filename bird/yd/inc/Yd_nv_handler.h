#ifndef YD_NV_HANDLER_H
#define YD_NV_HANDLER_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

void yd_tk001_set_record_from_NV();
/*��������yd_tk001_info�ṹ�����ݱ�����NV*/

void yd_tk002_set_record_from_NV();
/*��������yd_tk002_info�ṹ�����ݱ�����NV*/

#ifdef BIRD_BT_SUPPORT
void yd_tk003_set_record_from_NV();
/*��������yd_tk003_info�ṹ�����ݱ�����NV*/
#endif

void yd_tk001_get_record_from_NV();
/*��������NV�ж�ȡ���ݴ���yd_tk001_info*/

void yd_tk002_get_record_from_NV();
/*��������NV�ж�ȡ���ݴ���yd_tk002_info*/

#ifdef BIRD_BT_SUPPORT
void yd_tk003_get_record_from_NV();
/*��������NV�ж�ȡ���ݴ���yd_tk003_info*/
#endif

void yd_tk004_get_record_from_NV();
/**/

void yd_tk001_set_default_record_to_NV();
/*����������ʼ�����yd_tk001_info�ṹ�����ݱ�����NV*/

void yd_send_save_nv_msg();
/*���������ͽ�yd_tk001_info������д��NV����Ϣ*/

void yd_set_save_nv_msg();
/*���������ý�yd_tk001_info������д��NV����Ϣ���*/

#endif
