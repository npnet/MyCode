#ifndef _RS_NOTIFY_USER_H_
#define _RS_NOTIFY_USER_H_

#include "rs_datatype.h"
#include "rs_sdk_api.h"
#include "rs_state.h"




#define USER_CHANGE_STATE			1
#define USER_DOWNLOAD_PROGRESS		2
#define USER_DOWNLOAD_BREAK			3
#define USER_STARTED				4

#ifdef __cplusplus
  extern "C"
  {
#endif


/**
函数说明：操作进度指示 
参数说明：【current】表示当前已经下载的总量
  【total】表示总量
返回值：VOID
*/
void rs_cb_progress(rs_s32 current, rs_s32 total);

/**
函数说明：开机之后,如果上次开机正处于某种状态下面,那么开机之后提醒用户去处理
参数说明：【state】表示当前正处于的一种状态
返回值：VOID
*/
void rs_cb_notify_break_state( RS_FWDL_BREAK_STATE state);

/**
函数说明：核心模块执行状态通知，比如开始检测的时候会发一个消息，开始下载的时候会发一个消息
参数说明：【state】表示当前正处于的一种状态
返回值：VOID
*/
void rs_cb_notify_state(RS_FWDL_STATE state, rs_s32 code);

/**
函数说明：表示OTA的核心模块已经启动完成
参数说明：
返回值：VOID
*/
void rs_cb_started();

#ifdef __cplusplus
  }
#endif

#endif