/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   The YD service message processing module header file
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#include "Bird_app.h"

#define YD_TK001_PNONE_NUM			16
#define YD_TK001_PASSWORD_LEN			16
#define RJ_GPS_SND_BUFFER_SIZE		1500
#define SMS_BUFFER_LEN 200

/* ∂Ã–≈÷∏¡Ó */
#define YD_TK001_SMS_TJGL                "tjgladmin"
#define YD_TK001_SMS_SCGL               "scgl"
#define YD_TK001_SMS_SZMM               "zc"
#define YD_TK001_SMS_IPDZ               "ipdz"
#define YD_TK001_SMS_HFSZ               "hfsz"
#define YD_TK001_SMS_SF             "sf"
#define YD_TK001_SMS_CF             "cf"
#define YD_TK001_SMS_CQ             "cq"
//lrf add
#define YD_TK001_SMS_KEY               "key" 
#define YD_TK001_SMS_SFMS               "sfms" 
#define YD_TK001_SMS_SCMS               "scms" 
#define YD_TK001_SMS_CQADMIN             "cqadmin"


#define YD_TK001_SMS_SCJG               "scjg"
#define YD_TK001_SMS_JWDW             "jwdw"
#define YD_TK001_SMS_DW               "dw"
#define YD_TK001_SMS_XMKG               "xmkg"
#define YD_TK001_SMS_DDXMKG               "ddxmkg"
#define YD_TK001_SMS_PTXM               "ptxm"
#define YD_TK001_SMS_DDXM               "ddxm"
#define YD_TK001_SMS_YWKG               "ywkg"
#define YD_TK001_SMS_YWJL               "ywjl"

#define YD_TK001_SMS_YDKG               "ydkg"
#define YD_TK001_SMS_YDBJ                "ydbj"
#define YD_TK001_SMS_CSKG               "cskg"
#define YD_TK001_SMS_CSBJ                "csbj"
#define YD_TK001_SMS_DDKG               "ddkg"
#define YD_TK001_SMS_JZSJ                 "jzsj"
#define YD_TK001_SMS_LXDW               "lxdw"
#define YD_TK001_SMS_ZDDJ               "zddj"
#define YD_TK001_SMS_YZDX               "ibdrdy"
#define YD_TK001_SMS_IMEI                "imeiadmin"
#define YD_TK001_SMS_FFTD                "ffkg"

//lrf add 2013-09-10
#define YD_TK001_SMS_DYKG                "dykg"
#define YD_TK001_SMS_YSXX                "ysxx"
#define YD_TK001_SMS_YSQC                "ysqc"
#define YD_TK001_SMS_STATUS	"statusadmin"
#define YD_TK001_SMS_TRACE	"trace"
#define YD_TK001_SMS_APN "apn"

#define    BIRD_COMMAND_CONTINUE_SEARCH  0x09
#define    BIRD_COMMAND_NORMAL_SLEEP_VALUE  0x0A
#define    BIRD_COMMAND_MAIN_INTERVAL_VALUE  0x0B
#define    BIRD_COMMAND_NORMAL_SLEEP_FLAG  0x0C    
#define    BIRD_COMMAND_ABNORMAL_SLEEP_VALUE  0x0D

#define    BIRD_COMMAND_DIS_SLEEP_FLAG  0x76
#define    BIRD_COMMAND_DEFENSE_DELAY_VALUE  0x0E         
#define    BIRD_COMMAND_ALARM_DELAY_VALUE  0x0F    

#define    BIRD_COMMAND_SET_OWNER_NUMBER  0x4A
#define    BIRD_COMMAND_SET_UTILITY_PW  0x4B
#define    BIRD_COMMAND_SET_SYS_ADDRESS  0x4C
#define    BIRD_COMMAND_ACTIVE_OWNER_NUMBER  0x4D
#define    BIRD_COMMAND_SET_UTILITY_PW_EXT  0x4E
    
#define    BIRD_COMMAND_MOVE_POSITION_ALARM  0x5B
#define    BIRD_COMMAND_MOVE_POSITION_DISTAENSENCE  0x5C
#define    BIRD_COMMAND_VIBRATION_ALARM  0x5D
#define    BIRD_COMMAND_VIBRATION_VALUE  0x5E
#define    BIRD_COMMAND_SPEED_ALARM  0x5F
#define    BIRD_COMMAND_SPEED_VALUE  0x60
#define    BIRD_COMMAND_EXTERNAL_POWER_OFF  0x61
#define    BIRD_COMMAND_DEFENSE_MODE  0x63

#define    BIRD_COMMAND_VIEW_SW_NO  0x64
#define    BIRD_COMMAND_VIEW_GPS_DETAIL  0x65
#define    BIRD_COMMAND_VIEW_GSM_DETAIL  0x66
#define    BIRD_COMMAND_VIEW_BATTERY_LEVEL  0x67

#define    BIRD_COMMAND_CONTROL_REBOOT  4
#define    BIRD_COMMAND_CONTROL_SET_FACTORY  5
#define    BIRD_COMMAND_CONTROL_DEFENSE_MODE_ON  8
#define    BIRD_COMMAND_CONTROL_DEFENSE_MODE_OFF  9
#define    BIRD_COMMAND_CONTROL_SMS_UPLOAD  10
#define    BIRD_COMMAND_CONTROL_CALL_UPLOAD  11
#define    BIRD_COMMAND_CONTROL_DYKG  13
#define    BIRD_COMMAND_CONTROL_KEY_LEARN  14

#define    BIRD_COMMAND_BATCH_NO  0x64 
#define    BIRD_COMMAND_ADMIN_NO  0x68 
#define    BIRD_COMMAND_ILLEGAL_POWER  0x69
#define    BIRD_COMMAND_EXTERNAL_STATUS  0x6A
#define    BIRD_COMMAND_SLEEP_STATUS  0x6B
#define    BIRD_COMMAND_DEFENSE_STATUS  0x6C
#define    BIRD_COMMAND_SENSCE_STATUS  0x6D
#define    BIRD_COMMAND_KEYLOW  0x6E
#define    BIRD_COMMAND_DYKG  0x77
#define    BIRD_COMMAND_SCHYE  0xFF

#define  BIRD_COMMAND_POSITION_ONCE   0x01
#define  BIRD_COMMAND_POSITION_SEVERAL    0x02
#define  BIRD_COMMAND_POSITION_JDWD   0x03

extern void yd_tk001_CallOwer();
extern void Yd_sendregres(U8 nflag);
extern void Yd_sendgetdwres(S8* add);
extern U8 yd_tk001_all_Msg(smslib_general_struct* data);
