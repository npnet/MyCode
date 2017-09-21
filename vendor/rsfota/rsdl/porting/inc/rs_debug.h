#ifndef __RSDEBUG_H__
#define __RSDEBUG_H__

#include "rs_datatype.h"
#include <stdarg.h>

#ifdef __cplusplus
  extern "C"
  {
#endif


/**
����˵������ӡlog��־ 
����˵�����μ�printf��ʵ��
����ֵ��VOID
*/
void rs_print_log(const rs_s8* modelTag, const rs_s8* line, rs_s8* fmt, va_list arglist);


void rs_platform_print_log(rs_s8* fmt, ...);

//void rs_platform_new_print_log(rs_s8* fmt, ...);



/*
1.DEBUG : 
DEBUG Levelָ��ϸ������Ϣ�¼��Ե���Ӧ�ó����Ƿǳ��а����ġ� 
2. INFO 
INFO level���� ��Ϣ�ڴ����ȼ�����ͻ��ǿ��Ӧ�ó�������й��̡� 
3.WARN 
WARN level���������Ǳ�ڴ�������Ρ� 
4.ERROR 
ERROR levelָ����Ȼ���������¼�������Ȼ��Ӱ��ϵͳ�ļ������С� 
5.FATAL 
FATAL levelָ��ÿ�����صĴ����¼����ᵼ��Ӧ�ó�����˳���
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

#define RS_LOG_DEFAULT "<4>" //Ĭ�ϼ���Ϊ4

#define RS_DEFAULT_LEVEL 4


#define RS_PORITNG_LOG			rs_platform_print_log
//#define RS_PORITNG_LOG			/##/

#ifdef __cplusplus
  }
#endif

#endif
