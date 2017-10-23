/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      Yd_main.h
  Author:       Version:        Date: 
  Description:   YD main function header files, including running state is defined, the YD user information structure
  Others:         
  Function List:  
  History:       
    1. Date:2013-07-25
       Author:
       Modification:
1.�𶯸�Ӧ�����ȿ�ƽ̨����
2.����SZMM��������У��6λ������
3.soc����bug�޸�
4.����������л�
****************************************************************************/


#include "Bird_app.h"

#define YD_NV_BATCHLEN 6

/* ����״̬ö�� */
typedef enum
{
        BIRD_STATUS_NULL = 0,
        BIRD_STATUS_RUN,  /*������ʻ״̬*/
        BIRD_STATUS_HEART, /*��ֹ״̬���跢��������Ϣ��������*/
        BIRD_STATUS_HEARTBEFORE, /*��ʻ״̬������״̬�Ĺ��ɣ����ȷ���ǰλ�ã�֮��ת��Ϊ����״̬*/
        BIRD_STATUS_SLEEPBEFORE, /*��ֹ������״̬�Ĺ��ɣ����ȷ���ǰλ�ã�֮��ת��Ϊ����״̬*/
        BIRD_STATUS_SLEEP, /*����״̬���������κ�����*/
        BIRD_STATUS_DEEPSLEEP, /*�������*/
        BIRD_STATUS_SLEEPSENDHEART

}rj_gps_stage_type_enum;

typedef struct
{
    unsigned char format_flag;     
    volatile unsigned char defense_flag;  // defense true or false   ���
    unsigned char auto_defense_flag;  // weather the external circuit enable defense
    
    volatile unsigned char speed_alarm;/*���ٿ���*/
    U32 speed_int;/*/�ٶ�ֵ*/
    volatile unsigned char negative_voltage_alarm;/*/Ƿѹ����*/
    volatile unsigned char external_power_alarm;  /*/�ⲿ��Դ�ϵ籨��*/
    volatile unsigned char move_position_alarm;   /*/��λ����*/
    U32 move_position_distance;/*/��λ����*/
    volatile unsigned char vibration_running_alarm; /* �˶����� */
    YD_TK001_VIBRATION_VALUE vibration_value;
    unsigned char illegal_stream_alarm;

    YD_TK001_CONTINUE_SEARCH continue_search;/*/������λ*/
    YD_TK001_NORMAL_SLEEP normal_sleep;/*/��ͨ����*/
    YD_TK001_ABNORMAL_SLEEP abnormal_sleep; /*/�ϵ�����*/

    U32 main_interval;
    U32 defense_delay;

    U32 alarm_delay;
    unsigned char authorize_number[YD_TK001_PHONE_LEN]; // owner number
    unsigned char utility_number[YD_TK001_PHONE_LEN];    //utility number
    unsigned char sys_password[YD_TK001_PHONE_LEN];
    //unsigned char server_address[YD_TK001_SERVER_ADDRESS_LEN];
    unsigned char abd_address[YD_TK001_SERVER_ADDRESS_LEN];
    unsigned char agps_address[YD_TK001_SERVER_ADDRESS_LEN];

    unsigned char sim_iccid [25];

    unsigned char admin_pw[YD_TK001_PHONE_LEN];
    unsigned char admin_number[YD_TK001_PHONE_LEN];

    unsigned char sensitivity_level;
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*����NV�汾��*/
    U8 nrestarttimes;//����ʱ��
    U8 nresresion; //����ԭ�� lrf20130922
    U8 ntrace;//�Ƿ�������
    U32 heart_interval;
    U32 nlbv; //�ϵ籨����ֵ
    U8 nreg;//����
    U8 ndwtimes;//dw 
    U8 nzctimes;//zc
    U8 CALLLOCK;
    U8 CALLDISP;
    U8 VIBCALL;
    U8 SMS;
    unsigned char CID[10];
    U32 POFT;
    U8 WAKEUP;
    U32 WAKEUPT;
    U8 ACCLOCK;
    unsigned char	ipadd[40];
    unsigned char    resttime[24];
    kal_uint32 agps_pos_lati;
    kal_uint32 agps_pos_long;
    kal_uint32 agps_pos_alti;		
    kal_uint32 agps_llapos_lati;
    kal_uint32 agps_llapos_long;
    kal_uint32 agps_llapos_alti;	
    U8 Corner;
    U8 reboot;
    U8 server_defense_flag;
    U8 az_flag;
    U8 rollover_alarm;
    U8 hitlvalue;
    U8 hitldelaytime;
    U8 hitllatchtime;
    OC_Info oc_info;
    U8 acc_state;		//add by wtl 20160928
    U8 motor_state;		//���״̬
    U8 equmode;		//add by wtl 20161024
    U8 oc_main_heart_time;
    U8 lock_state;  //��������״̬
    U8 limit_speed;		//add by wtl 20161221
    U8 uart_bat_interval;
    U8 main_stage_status;
    U8 car_status;
}YD_TK001_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*����NV�汾��*/
    unsigned char server_address[YD_TK001_SERVER_ADDRESS_LEN];
}YD_TK002_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*����NV�汾��*/
    U8 bt_mac[20];
}YD_TK003_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*����NV�汾��*/
    U16 savedata_ival;	
    U16 nmal_main_ival;		
    U16 alarm_main_ival;		
    U8 heart_ival;
    U16 ter_res_time;		
    U16 ser_res_time;		
    U8 conn_interval;
    U16 lixian_time;		
}YD_TK005_COMMAND_Info;

extern YD_TK001_COMMAND_Info yd_tk001_info;
extern YD_TK002_COMMAND_Info yd_tk002_info;
extern YD_TK003_COMMAND_Info yd_tk003_info;
extern YD_TK005_COMMAND_Info yd_tk005_info;

extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

extern U8 g_vibration_is_timer_count_start;/*�𶯶�ʱ���������*/
extern U8 g_vibration_count;/*�𶯶�ʱʱ�����𶯴���*/
extern U8 g_vibration_is_timer_start;/*����ʱ������ʱ���������*/
extern U8 g_n_equstatue_count;

extern void yd_set_gps_open_msg();
extern void yd_set_gps_close_msg();
extern void yd_send_gps_open_msg();
extern void yd_send_gps_close_msg();
extern void yd_tk001_set_record_from_NV();
extern void yd_init_rollover_alarm_param(kal_uint8 interval_time);
extern void yd_init_az_alarm_param(kal_uint8 interval_time);
extern void yd_init_move_alarm_param(kal_uint8 interval_time);
extern void yd_init_vibration_alarm_param(kal_uint8 interval_time);
extern void yd_init_speed_alarm_param(kal_uint8 interval_time);
extern void yd_init_power_off_alarm_param(kal_uint8 interval_time);
extern void yd_init_low_power_alarm_param(kal_uint8 interval_time);
extern void  Yd_switch_fly(bool bfly);

