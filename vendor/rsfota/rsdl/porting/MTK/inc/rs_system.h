#ifndef _RS_SYSTEM_H_
#define _RS_SYSTEM_H_

#ifdef __cplusplus
  extern "C"
  {
#endif
#include "rs_datatype.h"

/*
函数说明：系统相关功能初始化
参数说明：无
返回值：成功返回RS_ERR_OK
*/
rs_s32 rs_sys_init();

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time(rs_s8* buf);

/*
函数说明：重启进入升级模式
特别说明：在其他的平台需要实现这个接口，windows模拟环境不需要实现
*/
void rs_sys_reboot();

/**
函数说明：输入日志到文件或者其他
参数说明：需要输出的日志内容
返回值：无
*/
void rs_cb_printLog(const rs_s8* msg);


/**
函数说明：消息接收模块收到消息之后，调用次毁掉 
参数说明：handle消息句柄
          uMsgEvent 消息事件
		 lParam1 消息参数1
		 lParam2 消息参数2
返回值： 
*/
typedef  void (*RS_MSG_HANDLER_CALLBACK)(void*handle, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2);

/**
函数说明：创建消息队列 
参数说明：userHandle 使用者的句柄
返回值：RS_NULL表示失败，否则为成功
*/
rs_s32 rs_sys_msg_queue_create(void* userHandle);

/**
函数说明：向消息接收模块发送消息
参数说明：nMsgQParam 消息句柄
          uMsgEvent 消息事件
		 lParam1 消息参数1
		 lParam2 消息参数2
		 callback 消息接收模块收到消息之后，调用此接口处理消息
返回值：0表示成功，-1表示失败 
*/
rs_bool rs_sys_msg_queue_send(rs_s32 nMsgQParam, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2, RS_MSG_HANDLER_CALLBACK callback);


/**
函数说明：定时器到达之后调用的回调函数 
参数说明：data 用户定义数据
返回值： 
*/
typedef  void (*RS_TIMER_CALLBACK)(void* handle);


/**
函数说明：启动定时器
参数说明：timerPeriod消息时间
返回值：返回定时器句柄，非0值表示成功，否则为失败 
*/
rs_s32 rs_sys_timer_start(rs_s32 timerPeriod, RS_TIMER_CALLBACK callback, void* data);

/**
函数说明：取消定时器
参数说明：nTimerParam定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_timer_cancel(rs_s32 handle);

/**
函数说明：启动主程序
参数说明：
返回值：rs_true表示成功，rs_false表示失败 
*/
/*根据不同的平台做不同的启动处理*/
rs_s32 rs_sys_autocheck_timer_start(rs_s32 timerPeriod);

/**
函数说明：取消自动检测定时器
参数说明：handle定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_cancel(rs_s32 handle);


/**
函数说明：当网络突发异常导致升级包下载失败，而下一次检测又太遥远（比如周期检测时间设置为3天），则
			应该作处理，让下一次检测快点到来
参数说明：无
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_restart();

/**
函数说明：判断SIM是否已经插入
参数说明：无
返回值：RS_TRUE表示有效，RS_FALSE表示无效 
*/
rs_bool rs_sys_simcard_insert();

/**
函数说明：判断SIM是否可以识别
参数说明：无
返回值：RS_TRUE表示可以识别，RS_FALSE表示不能识别
*/
rs_bool rs_sys_simcard_recognize();

/**
函数说明：判断imei是否已经写入
参数说明：无
返回值：RS_TRUE表示已经写入，RS_FALSE表示没有写入
*/
rs_bool rs_sys_imei_valid();

/**
函数说明：获取已经下载差分升级包的md5值
参数说明：deltaSize 升级包的长度
		  md5String 升级包的md5的值
返回值：RS_TRUE表示成功，RS_FALSE表示失败 
*/
rs_bool rs_sys_get_updatefile_md5_string(rs_s32 deltaSize, rs_s8* md5String);

/**
函数说明：读取外部配置文件debug.ini中的内容
		  返回结果作为系统的日志级别
		  文件地址在rs_param.h中RS_CONFIG_DEBUGINFO_FILE定义
参数说明：
返回值：返回数字0-3
*/
rs_u32 rs_sys_get_log_level();
#ifdef __cplusplus
  }
#endif

#endif
