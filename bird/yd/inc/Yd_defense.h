#ifndef YD_DEFENSE_H
#define YD_DEFENSE_H

#include "Kal_general_types.h"

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef MMI_BOOL
#define MMI_BOOL kal_bool
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void yd_init_defense_param();
/*����������������ĳ�ʼ��*/

U8 yd_tk001_get_defense();
/*��������ȡ�����־λ*/

void bird_set_defense_delay_value(U32 time);
/*���������������ʱʱ��*/

U32 bird_get_defense_delay_value();
/*��������ȡ�����ʱʱ��*/

void yd_tk001_defense_ini();
/*���������ݵ�ǰ���״̬ɾ�����λ����Ϣ���������λ����Ϣ���¿���λ*/

void yd_tk001_set_defense(MMI_BOOL flag);
/*���������ݵ�ǰ���״̬���������־λ�����λ����Ϣ�Ϳ���λ*/

void yd_tk001_set_server_defense(U8 flag);
/*���������÷����������־λ*/

MMI_BOOL yd_tk001_defense_is_set_by_server();
/*�������ж��Ƿ�Ϊ�������������*/

void yd_tk001_post_soft_sf();
/*�������������*/

void bird_set_cf_res();
/*��������ECU����DW23ʱ������������ָ��Ĵ�����*/

void bird_set_sf_res();
/*��������ECU����DW23ʱ�����������ָ��Ĵ�����*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_ecu_defend_to_uart();
/*������ECU֪ͨuart�����������*/

void yd_ecu_defend_to_server(U8 flag);
/*������ECU�����������������������ĺ���*/

void yd_ecu_defend_deal();
/*������ECU�������������*/

void bird_ecu_defend_res(U8 defend);
/*��������ECU����DW23ʱ�����������������Ԥ������*/
#endif

void bird_set_defense_res(U8 rtn);
/*�������������������������*/

void yd_set_defend_on_msg();
/*������DW23���������������Ϣ���*/

void yd_set_defend_off_msg();
/*������DW23����������������Ϣ���*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_set_uart_defend_on_msg();
/*�������������������Ϣ���*/

void yd_set_uart_defend_off_msg();
/*��������������������Ϣ���*/
#endif

void yd_mute_sf();
/*�������������������*/

void yd_set_mute_defend_on_msg();
/*���������þ��������Ϣ���*/

#endif
