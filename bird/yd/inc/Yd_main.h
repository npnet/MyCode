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
1.震动感应灵敏度可平台设置
2.短信SZMM增加密码校验6位纯数字
3.soc发送bug修改
4.电门设防可切换
****************************************************************************/


#include "Bird_app.h"

#define YD_NV_BATCHLEN 6

/* 运行状态枚举 */
typedef enum
{
        BIRD_STATUS_NULL = 0,
        BIRD_STATUS_RUN,  /*正常行驶状态*/
        BIRD_STATUS_HEART, /*静止状态，需发送心跳信息到服务器*/
        BIRD_STATUS_HEARTBEFORE, /*行驶状态到心跳状态的过渡，需先发当前位置，之后转换为心跳状态*/
        BIRD_STATUS_SLEEPBEFORE, /*静止到休眠状态的过渡，需先发当前位置，之后转换为休眠状态*/
        BIRD_STATUS_SLEEP, /*休眠状态，不发送任何数据*/
        BIRD_STATUS_DEEPSLEEP, /*深度休眠*/
        BIRD_STATUS_SLEEPSENDHEART

}rj_gps_stage_type_enum;

typedef struct
{
    unsigned char format_flag;     
    volatile unsigned char defense_flag;  // defense true or false   设防
    unsigned char auto_defense_flag;  // weather the external circuit enable defense
    
    volatile unsigned char speed_alarm;/*超速开关*/
    U32 speed_int;/*/速度值*/
    volatile unsigned char negative_voltage_alarm;/*/欠压开关*/
    volatile unsigned char external_power_alarm;  /*/外部电源断电报警*/
    volatile unsigned char move_position_alarm;   /*/移位报警*/
    U32 move_position_distance;/*/移位距离*/
    volatile unsigned char vibration_running_alarm; /* 运动报警 */
    YD_TK001_VIBRATION_VALUE vibration_value;
    unsigned char illegal_stream_alarm;

    YD_TK001_CONTINUE_SEARCH continue_search;/*/连续定位*/
    YD_TK001_NORMAL_SLEEP normal_sleep;/*/普通休眠*/
    YD_TK001_ABNORMAL_SLEEP abnormal_sleep; /*/断电休眠*/

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
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*保存NV版本号*/
    U8 nrestarttimes;//重启时间
    U8 nresresion; //重启原因 lrf20130922
    U8 ntrace;//是否开启跟踪
    U32 heart_interval;
    U32 nlbv; //断电报警阀值
    U8 nreg;//激活
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
    U8 motor_state;		//马达状态
    U8 equmode;		//add by wtl 20161024
    U8 oc_main_heart_time;
    U8 lock_state;  //解锁上锁状态
    U8 limit_speed;		//add by wtl 20161221
    U8 uart_bat_interval;
    U8 main_stage_status;
    U8 car_status;
}YD_TK001_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*保存NV版本号*/
    unsigned char server_address[YD_TK001_SERVER_ADDRESS_LEN];
}YD_TK002_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*保存NV版本号*/
    U8 bt_mac[20];
}YD_TK003_COMMAND_Info;

typedef struct
{
    unsigned char format_flag;     
    unsigned char nvbatch[YD_NV_BATCHLEN]; 	/*保存NV版本号*/
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

extern U8 g_vibration_is_timer_count_start;/*震动定时器开启标记*/
extern U8 g_vibration_count;/*震动定时时间内震动次数*/
extern U8 g_vibration_is_timer_start;/*震动延时报警定时器开启标记*/
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

