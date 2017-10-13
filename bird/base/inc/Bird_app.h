/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      Bird_app.h
  Author:       Version:        Date: 2013-05-29
  Description: Global header files, bird module and other module reference  
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/


#ifndef RJ_GPS_APP_H
#define RJ_GPS_APP_H

#ifndef __MTK_TARGET__
#include <windows.h>    /* for soc_api.h */
#endif
#include "mmi_features.h"
#include "mmi_include.h"

#include "TimerEvents.h"
#include "MMIDataType.h"
#include "FileManagerGProt.h"
#include "DataAccountDef.h"
#include "soc_api.h"
#include "soc_struct.h"
#include "soc_consts.h"
#include "ProtocolEvents.h"
#include "EventsGprot.h"
#include "med_api.h"
#include "gui.h" 
#include "CommonScreens.h"
#include "stdarg.h"
#include "app_datetime.h"//ErrReport
#include "app_str.h"
#include "nvram_user_defs.h"

#include "kal_general_types.h"
#include "nvram_common_defs.h"
#include "common_nvram_editor_data_item.h"

#include "smslib_def.h"
#include "SmsSrvGprot.h"
#include "SimAccessSrvGprot.h"

#include "Conversions.h"

#include "CharBatSrvGprot.h"
#ifdef RJ_GPS_APP_AGPS
#include "nbr_public_struct.h"
#endif
#include "bird_std.h"

#define BIRD_GPS_DEBUG  1
#define YD_TK001_SW_SOFTKIND "1"
#define YD_TK001_SW_CJ "NBBD"
#define YD_TK001_SW_VERSION	"BD222"
#define RJ_GPS_UA				"SGT001b"
#define RJ_GPS_PLAT				"MTK6225_11B"

#if defined(BIRD_BT_SUPPORT)
#define YD_TK001_SW_FLAG "011"
#define YD_TK001_MODEL "UART_DW11BT"
#elif defined(BIRD_ECU_MMI_SUPPORT)
#define YD_TK001_SW_FLAG "011"
#define YD_TK001_MODEL "UART_DW11"
#elif defined(BIRD_DW60)
#define YD_TK001_SW_FLAG "TX"
#define YD_TK001_MODEL "DW60"
#else
#define YD_TK001_SW_FLAG "011"
#define YD_TK001_MODEL "DW11"
#endif

#define YD_TK001_BDLOG "BDLOG"

#define YD_TK001_T37_BD01 "BT01"

/*
实时数据T37里面厂家是BT01
故障码 BT02
电池组 BT03
报警 BT04
第三方设备参数设置上传 BT05
自检进程上传 BT06
硬件配置信息BT07
*/
#define YD_TK001_T37_realtime "BT01"
#define YD_TK001_T37_fault "BT02"
#define YD_TK001_T37_battery "BT08"
#define YD_TK001_T37_alarm "BT04"
#define YD_TK001_T37_param "BT05"
#define YD_TK001_T37_selfdefine "BT06"
#define YD_TK001_T37_hardware "BT07"

#define YD_TK001_T37_vendor "NBBD"

//#define ZK_YD	// add by lqy for dongkuan zhinan  beidou
#define BD_HDDZ    // add by lqy for beijing huada dianzi  beidou
//#define BD_BJHK    // add by lqy for beijing hangkong beidou
//#define BD_DGZN   // add by lqy for dongkuan zhinan  beidou
#if defined(BIRD_BT_SUPPORT)
	#define RJ_GPS_VERSION			"V1.13_NBBD_20170516_DW11BT_UART_MT6261"
	/*默认密码*/
	#define YD_TK001_PW "123456"
	/*默认服务器地址*/
	#define YD_TK001_HTTP "http://sk.gpslink.cn:8090/"
	#define YD_TK001_IP "139.129.87.204:8090"
	#define YD_TK001_YD_ADMIN_PSW "bdws"
	#define YD_NV_BATCH "Y1.1"
#elif defined(BIRD_ECU_MMI_SUPPORT)
	#define RJ_GPS_VERSION			"V1.24_NBBD_20170602_DW11_UART_MT6261"
	/*默认密码*/
	#define YD_TK001_PW "123456"
	/*默认服务器地址*/
	#define YD_TK001_HTTP "http://sk.gpslink.cn:8090/"
	#define YD_TK001_IP "139.129.87.204:8090"
	#define YD_TK001_YD_ADMIN_PSW "bdws"
	#define YD_NV_BATCH "Y2.1"
#elif defined(BIRD_DW60)
	#define RJ_GPS_VERSION			"V1.01"
	#define RJ_HW_VERSION			"V1.01"
	#define RJ_GPS_VERSION2			"V1.01_NBBD_20171013_DW60"
	/*默认密码*/
	#define YD_TK001_PW "123456"
	/*默认服务器地址*/
	#define YD_TK001_HTTP "http://118.190.126.192:9090/"//"http://118.190.145.224:11546/"
	#define YD_TK001_IP "118.190.126.192:9090"//"118.190.145.224:11546"
	#define YD_TK001_HTTP2 "http://sk.gpslink.cn:8090/"
	#define YD_TK001_IP2 "139.129.87.204:8090"
	#define YD_TK001_YD_ADMIN_PSW "bdws"
	#define YD_NV_BATCH "Y4.1"	
#else 
	#define RJ_GPS_VERSION			"V1.01_NBBD_20170727_DW11OTA_MT6261"
	/*默认密码*/
	#define YD_TK001_PW "123456"
	/*默认服务器地址*/
	#define YD_TK001_HTTP "http://sk.gpslink.cn:8090/"
	#define YD_TK001_IP "139.129.87.204:8090"
	#define YD_TK001_YD_ADMIN_PSW "bdws"
	#define YD_NV_BATCH "Y3.4"
#endif

/*********OTA Current Version*********/
#define BIRD_OTA_VRESION "BIRD_DW60_OTA_V1.02"

#define RJ_GPS_LOG_ON
#define RJ_GPS_LEN					32
#define RJ_GPS_PNONE_NUM			16
#define RJ_GPS_WORK_MODE		0               /* 运行模式，0 使用GPS 和AGPS 两种 ，1 单独使用GPS，2 单独使用AGPS */

#define RJ_GPS_STATUS_ON    1
#define RJ_GPS_STATUS_OFF    0

#define RJ_GPS_LED_LIGHT_TIME					100
#define RJ_GPS_LED_FAST_ON_TIME					150
#define RJ_GPS_LED_FAST_OFF_TIME					850
#define RJ_GPS_LED_SLOW_ON_TIME					750
#define RJ_GPS_LED_SLOW_OFF_TIME					750
#define RJ_GPS_LED_UNLIGHT_TIME				500
#define RJ_GPS_LED_SLOWLIGHT_TIME				1500
#define RJ_GPS_LED_ALWAYS_TIME				4000
#define RJ_GPS_LED_LONG_TIME				5000
#define YD_TK001_PHONE_LEN 16
#define YD_TK001_SERVER_ADDRESS_LEN 100

#define RJ_GPS_APP_5S                       5 * 1000
#define RJ_GPS_APP_10S                     10 * 1000
#define RJ_GPS_APP_15S                     15 * 1000
#define RJ_GPS_APP_1M                       1 * 60 * 1000
#define RJ_GPS_APP_30S                       30 * 1000
#define RJ_GPS_TIMER_5M                   5* 60 * 1000
#define RJ_GPS_TIMER_2H                   2* 60 * 60 * 1000
#define RJ_GPS_TIMER_24H                 (24* 60)


#define MMI_CARD_DRV_EXT   FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_V_REMOVABLE)
#define MMI_CARD_DRV   FS_GetDrive(FS_DRIVE_V_REMOVABLE, 2,  FS_DRIVE_V_REMOVABLE)//FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_V_REMOVABLE) 
#define MMI_CARD_DRV_1   FS_GetDrive(FS_DRIVE_V_REMOVABLE, 2,  FS_DRIVE_V_REMOVABLE)                               //E
#define BIRD_FILE_PATH_LEN 100//256
#define BIRD_LOG_LEN 256
#define BIRD_RTLOG_CLEAN_LOG 1

typedef enum   //all timer
{
	BIRD_GPRS_TIMER_START = BIRD_TIMER_START,                  
	BIRD_GPRS_TIMEOUT_TIMER,
	BIRD_AGPS_WRITE_TIMEOUT_TIMER,
       Bird_login_Timer,
       Bird_main_Timer,
       //Bird_postinfo_Timer,
       //Bird_revdeal_Timer,
       Bird_sleep_dinisocket_Timer,
       BIRD_GPS_CHECK_TIMER,  
       BIRD_GPS_RESET_TIMER,  
	//BIRD_GPS_APP_TIMER,
	//BIRD_GPS_APP_TIMER_CLEAR_MSG,
       BIRD_SOCKET_SEND,
       BIRD_SOCKET_RECON,
	RJ_GPS_TIMER_REDLIGHT,
	RJ_GPS_TIMER_YELLOLIGHT,
	RJ_GPS_TIMER_BLUELIGHT,
       //RJ_GPRS_APP_SYNCH_TIMER,    
	//RJ_GPS_APP_DAY_RESET_TIMER,  	
       //BIRD_YD_3D_SENSOR_TIMER,
       //BIRD_YD_UPLOAD_CALLER_TIMER,
       //BIRD_YD_NORMAL_SLEEP_TIMER,
       //BIRD_YD_ABNORMAL_SLEEP_TIMER,   
       BIRD_SET_DEFENSE_DELAY_TIMER,
       //BIRD_YD_WAKEUP_THEN_CHECK_TIMER,    
       //BIRD_YD_POST_INIT_PARA_TIMER,           
	RJ_GPS_RESET_TIMER, 
	BIRD_GPS_CLOSE_TIMER, 
	BIRD_GPS_RESTART_TIMER, 
	BIRD_GPS_OPEN_TIMER, 
	BIRD_GPS_COLD_RESTART_TIMER, 
	BIRD_GPS_COLD_RESTART_CHECK_TIMER, 
	BIRD_AGPS_WRITE_CHECK_TIMER, 
	BIRD_AGPS_WRITE_TIMER, 
	BIRD_VIBRATION_TIMER,
	//BIRD_SET_KEYON_DELAY_TIMER,
	BIRD_SET_MOVE_DELAY_TIMER,
	//BIRD_SET_LOW_DELAY_TIMER,
	BIRD_VIBRATION_TIMER_COUNT,	
	//BIRD_LOGIN_TRY_TIMER,
	//BIRD_ACTIVE_MSG_TIMER,
	//BIRD_WAKEUP_TIMER,
	BIRD_POWER_OFF_DELAY,
	BIRD_YD_KEY_DET_DELAY,
	BIRD_YD_KEY_LEARN,
	BIRD_LED_ENABLE_DELAY,
	BIRD_READ_FILE,
	Bird_LOG_WRITE,
	BIRD_FLY_MODE_CHANGE,
	RJ_ANTITHEFT_TIMER_GPS_LVYUAN_UART,
	BIRD_BDREAD_UART,  //add by lqy
	BIRD_VOLT_CONNECT,  //add by wtl for volt get timer
	BIRD_LOCK_TIMER,
	BIRD_LOCK_RESP,  //add by wtl for lock response 
	BIRD_ECU_LOCK_RESP,  //add by wtl for ecu lock response 
	BIRD_ECU_DEFEND_RESP,  //add by wtl for ecu defend response 
	BIRD_ECU_FINDCAR_RESP,  //add by wtl for ecu findcar response 
	BIRD_ECU_LIMITSPEED_RESP,  //add by wtl for ecu limit speed response 
	BIRD_ECU_OPENAZ_RESP,  //add by wtl for ecu openaz response 
	BIRD_ECU_CARCHECK_RESP,  //add by wtl for ecu car check response 
	BIRD_ECU_CARSTART_RESP,  //add by wtl for ecu car start response 
	BIRD_ECU_BATTERY_RESP,  //add by wtl for ecu car battery response 
	BIRD_ECU_CONNECT_TIMER,  //add by wtl for ecu connect timer
	BIRD_ECU_BATTERY_TIMER,  //add by wtl for ecu send battery timer
	BIRD_ECU_SPEED_TIMER,  //add by wtl for ecu speed timer
	BIRD_ACC_ECU_CHECK_TIMER,
       BIRD_SOS_STATE_timer,       
       BIRD_SOUND_RECORD_TIMER,
       BIRD_SOUND_SEND_TIMER,
       Bird_read_gpsnum_timer,//add by jiazhaoran	
       BIRD_POWEROFF_TIMER,
	   BIRD_SPI_TEST,
       BIRD_WDG_timer,
       BIRD_ATTEST_timer,
       BIRD_WDG_timer_TEST,
       BIRD_AT_TEST_TIMER,
       BIRD_CAN_SIMULATE_SEND_TIMER,
       BIRD_COUNTOR_TIMEOUT_TIMER,
       BIRD_DW23_RUNSTATE_timer,
       BIRD_POWEROFF_VOLTAGE_TIMER,
       BIRD_REMOTER_TIMER,
       BIRD_REMOTER_INT_TIMER,
       BIRD_PWM_CONTROL_TIMER,
       BIRD_TRACECAR_TIMER,
       BIRD_LEARN_TIMER,
       BIRD_DISCON_SOCK_TIMER,       
       BIRD_DELETE_FILE_TIMER,
 
	Bird_task_login_Timer,
	Bird_task_logintxbox_Timer,
	Bird_task_connlogin_Timer,
	Bird_task_calibtime_Timer,
	Bird_task_24calib_Timer,
       Bird_task_main_Timer,
       Bird_task_savedata_Timer,
       Bird_task_heart_Timer,
       BIRD_TASK_GPRS_TIMER_START,
       BIRD_TASK_VIBRATION_TIMER_COUNT,
       BIRD_TASK_YD_NORMAL_SLEEP_TIMER,
       BIRD_TASK_SOCKET_SEND,
       BIRD_TASK_SOCKET_RECON,
       RJ_TASK_GPS_RESET_TIMER,
       BIRD_TASK_SET_DEFENSE_DELAY_TIMER,
       BIRD_TASK_POWER_OFF_DELAY,
       BIRD_TASK_DW_TIMER,
       BIRD_TASK_YD_DEEP_SLEEP_TIMER,
       Bird_task_sleep_dinisocket_Timer,
       BIRD_TASK_REST_TIMER,
       BIRD_TASK_24REST_TIMER,
       BIRD_TASK_LC_TJ_TIMER,
       BIRD_TASK_POS_WAIT_TIMER,
       BIRD_TASK_SLEEPPOS_WAIT_TIMER,
       BIRD_TASK_ALARM_LOW_WAIT_TIMER,
       BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER,
       BIRD_TASK_ALARM_MOVE_WAIT_TIMER,
       BIRD_TASK_ALARM_RUN_WAIT_TIMER,
       BIRD_TASK_SEAL_COUNT_WAIT_TIMER,
       BIRD_TASK_HEART_WAIT_TIMER,
       Bird_task_ringoff_timer,
       BIRD_TASK_DELAY_SENDLC_TIMER,
       BIRD_TASK_OC_MAIN_HEART_TIMER,
       BIRD_TASK_OC_RUN_TIMER,

	BIRD_GPRS_TIMER_MAX                  
}Lima_plat_timer_id;

typedef struct
{
	kal_bool  b_SIM_IS_SERACHING;
    	kal_bool  b_GSM_IS_SERACHING;
       kal_bool  b_GPS_IS_SERACHING;	
	kal_bool b_SERVER_IS_LOGGING;
	kal_bool b_GPS_IS_CLOSE;
	kal_bool b_SOC_IS_CONNECTING;
	kal_bool b_SOC_IS_CONNECTED;
	kal_bool b_GSM_IS_CLOSE;
	kal_bool b_SIM_IS_CLOSE;
	kal_bool b_LOGSERVER_IS_CLOSE;
}RJ_POWER_GSM_GPRS_status_Info;



typedef enum{
    TK001_ALARM_ENUM_moved,
    TK001_ALARM_ENUM_vibration,
    TK001_ALARM_ENUM_speed,
    TK001_ALARM_ENUM_power_off,
    TK001_ALARM_ENUM_low_power,
    TK001_ALARM_ENUM_ydcall,
    TK001_ALARM_ENUM_sos,
    TK001_ALARM_ENUM_az,
    TK001_ALARM_ENUM_collision,
    TK001_ALARM_ENUM_static_rollover,
    TK001_ALARM_ENUM_run_rollover,
    TK001_ALARM_ENUM_max
}TK001_ALARM_ENUM;



typedef struct{
	U8 upload_onoff;			/*  上传允许位*/
	U8 call_onoff;		/*  拨号允许位*/
	U8 alarm_times;			/*  报警次数*/
	kal_uint8 interval_time;		/*  报警时间间隔*/
	applib_time_struct compare_time;	/*  上次报警时间+  报警时间间隔*/
}ALARM_msg_param;



typedef struct {
	LOCAL_PARA_HDR
	TK001_ALARM_ENUM   msg_id;
       U8 para_setting_id;
       U8 Is_init_post;
} BIRD_id_struct;

typedef struct
{
	kal_int8 year;
	kal_int8 month;
	kal_int8 day;
	kal_int8 hour;
	kal_int8 min;
	kal_int8 sec;
/*
	S8 year[RJ_GPS_LEN];
	S8 month[RJ_GPS_LEN];
	S8 day[RJ_GPS_LEN];
	S8 hour[RJ_GPS_LEN];
	S8 min[RJ_GPS_LEN];
	S8 sec[RJ_GPS_LEN];
*/
}RJ_Gps_Date;

typedef struct
{
    	volatile S8 satellite_num;
	RJ_Gps_Date time;
       volatile	double la;
       volatile	double lg;
#ifdef HD8020_GPS_SUPPORT
       volatile	double al;
#endif
	float sd;
	float di;
	volatile S8 satellite_num2;
       kal_int8      north_south;  /*N or S*/
       kal_int8      east_west;    /*E or W*/	
	S8 ishavedate;

/*
	RJ_Gps_Date time;
	U8 la[RJ_GPS_LEN];
	U8 lg[RJ_GPS_LEN];
	U8 sd[RJ_GPS_LEN];
	U8 di[RJ_GPS_LEN];
*/
}RJ_Gps_Position;


typedef struct
{
	U8 u_imei[RJ_GPS_LEN];
	U8 u_iccid[RJ_GPS_LEN];
	U8 ua[RJ_GPS_LEN];
	U8 version[RJ_GPS_LEN*2];
	U8 sos1[RJ_GPS_PNONE_NUM];
	U8 sos2[RJ_GPS_PNONE_NUM];
	U8 sos3[RJ_GPS_PNONE_NUM];
	U8 sc[RJ_GPS_LEN];
	U8 plat[RJ_GPS_LEN];
	U8 gps_plat[RJ_GPS_LEN];
	U8 u_imsi[RJ_GPS_LEN];
	U8 u_id[RJ_GPS_LEN];
}RJ_Gps_User_Info;

typedef struct
{
    U16 search_times;
    U16 search_interval;
}YD_TK001_CONTINUE_SEARCH;

typedef struct
{
    unsigned char sleep_flag;
//    unsigned char sleep_state;    
    U32 sleep_time;
}YD_TK001_NORMAL_SLEEP;

typedef struct
{
    U16 vibration_level;
    U16 vibration_times;    
}YD_TK001_VIBRATION_VALUE;

typedef struct
{
    unsigned char sleep_flag;
    U32 sleep_time;
}YD_TK001_ABNORMAL_SLEEP;

typedef enum
{
	RJ_GPS_NO_ERROR = 0,
	RJ_GPS_MSGINFO_ERROR,
	RJ_GPS_MSGINFO_WRONG_NUM,
	RJ_GPS_OUTOFTIME
	
} rj_gps_wrong_msg_enum;


typedef enum{
    BIRD_NORMAL = 0,
    BIRD_CONN_ERR, 
    BIRD_SIM_GET_ERR,
    BIRD_TIMER_ERR,
    BIRD_BUT_LOW,
    BIRD_SEND_ERR,
    BIRD_CHARGE_ERR,
    BIRD_RESON_max
}RESET_RESON_ENUM;

typedef struct
{
	U8 oc_state;
	U32 instru_id;
	U8 exec_state;
}OC_Info;

typedef struct
{
    U8 lockcar;
    U8 findcar;
    U8 defend;
    U8 limitspeed;
    U8 selfcheck;
    U8 startup;
    U8 anzuo;
    U8 reserved;
    U8 lockcar_flag;
    U8 findcar_flag;
    U8 defend_flag;
    U8 limitspeed_flag;
    U8 selfcheck_flag;
    U8 startup_flag;
    U8 anzuo_flag;
    U8 reserved_flag;
}BIRD_UART_A0_INFO;

typedef enum{
	ECU_status_log=3,
	Move_status_log,
	Vibrate_status_log,
	Power_status_log,
	ACC_status_log,
	Defense_status_log,
	Server_receiver_log
}SOC_LOG_INFO;
typedef struct
{
    U8 lockcar;
    U8 findcar;
    U8 defend;
    U8 limitspeed;
    U8 limitspeed_value;	
    U8 selfcheck;
    U8 startup;
    U8 anzuo;
    U8 reserved;
    U8 lockcar_flag;
    U8 findcar_flag;
    U8 defend_flag;
    U8 limitspeed_flag;
    U8 selfcheck_flag;
    U8 startup_flag;
    U8 anzuo_flag;
    U8 reserved_flag;
}BIRD_UART_C9_INFO;

typedef struct
{
U8 vibrate_status;
U8 btconnect_status;
U8 btKey_induction_status;
U8 gps_status;
U8 gsm_status;
U8 sim_status;
}BIRD_UART_C9_04_INFO;

typedef enum{
    BIRD_URAT_DATA_realtime,
    BIRD_URAT_DATA_fault,
    BIRD_URAT_DATA_battery,
    BIRD_URAT_DATA_alarm,
    BIRD_URAT_DATA_param,
    BIRD_URAT_DATA_selfdefine,
    BIRD_URAT_DATA_hardware,
    BIRD_URAT_DATA_max
}BIRD_URAT_DATA_ENUM;
/*
1---设置成功
2---当前状态执行断油有风险，等后续执行,
3---断油指令已执行成功,
4---终端异常退出断油状态，等待后续执行, 
*/
#define OC_SET_SUCCESS 1
#define OC_TO_BE_EXEC 2
#define OC_EXECUTED 3
#define OC_ABNORMAL 4

typedef struct{
	U32 offset;
	applib_time_struct data_time;
	applib_time_struct cur_time;
}BIRD_READPOS_STRUCT;

extern RJ_Gps_User_Info rj_user_info;
extern kal_bool  b_is_no_motion;                    /* GPS 静止 */
extern kal_bool b_motion;                              /* GPS 运动 */
extern kal_int8 rj_gps_error_code;
extern kal_bool b_SIM_IS_OK;
extern kal_bool b_charge_usb_in;
extern volatile U8 rj_gps_gps_module;
extern U8 rj_gps_service_status;
extern U16 RJ_Gps_wrong_info;
extern U8 rj_gps_stage;
extern S8 rj_gps_search_mark[4];

extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
extern RJ_Gps_Position rj_position;
extern RJ_Gps_Position gps_position;
extern RJ_Gps_Position heart_position;
extern RJ_Gps_Position alarm_position;
extern U8 bird_sim_signal_strength_in_percentage;

extern void Bird_start();
extern void Bird_soc_send();
extern void RJ_GPS_record_date(const WCHAR  *filename , S8 *date , BOOL isRecover);
extern void RJ_GPS_log(kal_char *fmt,...);
extern void RJ_SYS_ReStart();
extern void RJ_Gps_Init();
extern void RJ_GPS_Stop(void);
extern kal_bool is_ac_charger_in(void);
extern void Bird_Log_Init(void);
extern void Bird_Log_print(module_type mod_id, const kal_char *fmt,...);
#endif

