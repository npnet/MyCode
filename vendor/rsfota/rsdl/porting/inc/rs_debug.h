#ifndef __RSDEBUG_H__
#define __RSDEBUG_H__

#include "rs_datatype.h"
#include <stdarg.h>

#ifdef __cplusplus
  extern "C"
  {
#endif


/**
函数说明：打印log日志 
参数说明：参见printf的实现
返回值：VOID
*/
void rs_print_log(const rs_s8* modelTag, const rs_s8* line, rs_s8* fmt, va_list arglist);


void rs_platform_print_log(rs_s8* fmt, ...);

//void rs_platform_new_print_log(rs_s8* fmt, ...);



/*
1.DEBUG : 
DEBUG Level指出细粒度信息事件对调试应用程序是非常有帮助的。 
2. INFO 
INFO level表明 消息在粗粒度级别上突出强调应用程序的运行过程。 
3.WARN 
WARN level表明会出现潜在错误的情形。 
4.ERROR 
ERROR level指出虽然发生错误事件，但仍然不影响系统的继续运行。 
5.FATAL 
FATAL level指出每个严重的错误事件将会导致应用程序的退出。
*/

#define RS_LOG_FATAL  "<1>"
#define RS_LOG_ERROR  "<2>"
#define RS_LOG_WARN	  "<3>"
#define RS_LOG_INFO   "<4>"
#define RS_LOG_DEBUG  "<5>"

#define RSLOG_FATAL 1
#define RSLOG_ERROR 2
#define RSLOG_WARN  3
#define RSLOG_INFO  4
#define RSLOG_DEBUG 5

#define RS_LOG_DEFAULT "<4>" //默认级别为4

#define RS_DEFAULT_LEVEL 4


#define RS_PORITNG_LOG			rs_platform_print_log
//#define RS_PORITNG_LOG			/##/

#ifdef __cplusplus
  }
#endif

#endif
