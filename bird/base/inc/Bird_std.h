/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD standard function header file for reference, BIRD module
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifndef PSS_TYPES_H
#define PSS_TYPES_H

#ifndef U8
#define U8 unsigned char
#endif

#ifndef U16
#define U16 unsigned short
#endif

#ifndef U32
#define U32 unsigned int
#endif

#ifndef S8
#define S8 char 
#endif

#ifndef S16
#define S16 short
#endif

#ifndef S32
#define S32 int
#endif

#ifndef F32
#define F32 float
#endif

#ifndef F64
#define F64 double
#endif

#ifndef gdi_handle
typedef U32 gdi_handle;
#endif

#ifndef handle_pss
typedef S32 handle_pss;
#endif


#define RJ_GPS_MSG_MAX_LENGTH      (256)    //定义短信最大处理长度
#define BIRD_SAVE_LOG_LENGTH    8192
#define BIRD_LOG_LENGTH    256
#define BIRD_START_WRITE_LOG_LENGTH    1//128
#define BIRD_DT_LENGTH   25
#define BIRD_TIME_STAMP_MAX   0//100

#ifndef __cplusplus
#define true	1
#define false	0
#define bool U8
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

//定义矩形类型
typedef struct
{
	U16	x;
	U16	y;
	U16	width;
	U16	height;
} rect_t;



typedef struct
{
    U8 Log_type;
    U8 Log_data_len;
    kal_char Log_buffer[512];
}Bird_log_type;

#define BIRD_LOG_FILE_PATH "%c:\\log.dat"    //by lqy 
#define BIRD_LOG_FILE_PATH2 "%c:\\Lima\\log2.dat" 
#define BIRD_POS_FILE_PATH "%c:\\posdata.dat" 
#define BIRD_RTLOG_DIR "%c:\\Rtlog"
#define BIRD_RTLOG_FILE_PATH "%c:\\Rtlog\\log_%04d-%02d-%02d-%02d-%02d-%02d.txt"
#define BIRD_RTLOG_FILE "%c:\\Rtlog\\log_*.txt"
#define BIRD_SOUND_RECORD_FOLDER "%c:\\RECORD"
#define BIRD_SOUND_RECORD_PATH "%c:\\RECORD\\REC_%s.amr"
#define BIRD_TBOXDATA_DIR "%c:\\Tboxdata" 
#define BIRD_TBOXDATA_PATH "%c:\\Tboxdata\\rec_%04d-%02d-%02d-%02d.txt" 
#define BIRD_TBOXDATA_FILE "%c:\\Tboxdata\\rec_*.txt" 
#define BIRD_TBOXINFO_DIR "%c:\\Tboxinfo" 
#define BIRD_TBOXINFO_PATH "%c:\\Tboxinfo\\rec_%04d-%02d-%02d-%02d.txt" 
#define BIRD_TBOXINFO_FILE "%c:\\Tboxinfo\\rec_*.txt" 

typedef void (*void_fn_t)(void);
typedef void (* timer_fn_t)(void);
typedef void(* hl_fn_t)(S32 index);
typedef bool (*call_back_fn_t)(S32 p1,void* p2);
typedef void (*void_func)(void*);
typedef U8 (*pss_ext_func) (void *, S32);
#endif
extern void Lima_Soc_Itoa(S8* out, S32 num);
extern void yd_tk001_set_utility_time(U8 * data);