#include "rs_datatype.h"
#include "rs_system.h"
#include "rs_state.h"
#include "rs_sdk_api.h"
#include "rs_mem.h"
#include "rs_error.h"
#include "rs_dev.h"
#include "rs_debug.h"
#include "rs_std_fun.h"
#include "rs_param.h"
#include "rs_porting_cfg.h"
#include "rs_fs.h"
// MTK header
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "kal_public_api.h"
#include "Mmiapi_struct.h"
#include "mmi_msg_struct.h"
#include "Mmi_frm_queue_gprot.h"
#include "Mmi_frm_events_gprot.h"
#include "Mmi_cb_mgr_gprot.h"
#include "TimerEvents.h"
#include "app_datetime.h"
#include "DateTimeGprot.h"
#include "CharBatSrvGprot.h"
#include "uem_utility.h"
#include "app_md5.h"
#include "Unicodexdcl.h"

static const rs_s8 dec2hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void * g_msg_handle = RS_NULL;

typedef struct 
{
	RS_MSG_HANDLER_CALLBACK callback;
	rs_u32 uMsgEvent;
	rs_s32 lParam1;
	rs_s32 lParam2;
} RS_MMI_Msg;


void rs_sys_msg_queue_callback(mmi_event_struct* evt)
{
	if(evt != 0)
	{
		RS_MMI_Msg* data;
		
		data = (RS_MMI_Msg *)evt->user_data;
		if (data != RS_NULL && g_msg_handle != RS_NULL)
		{
			data->callback(g_msg_handle, data->uMsgEvent, data->lParam1, data->lParam2);
			rs_free(data);
		}
	}
}

/******************************************************************************************************/
/*
函数说明：系统相关功能初始化
参数说明：无
返回值：成功返回RS_ERR_OK
*/
rs_s32 rs_sys_init()
{
	int errCode = RS_ERR_OK;



	return errCode;
}

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time(rs_s8* buf)
{
	MYTIME local_time;
	rs_memset(&local_time, 0, sizeof(MYTIME));
	DTGetRTCTime(&local_time);
	rs_sprintf(buf, "%d-%d-%d %d:%d:%d ", local_time.nYear, local_time.nMonth, local_time.nDay, local_time.nHour, local_time.nMin, local_time.nSec);

}

void rs_sys_get_time_ex(rs_s8* buf)
{
	MYTIME local_time;
	rs_u32 time_addr = (rs_u32)&local_time;
	rs_memset(&local_time, 0, sizeof(MYTIME));
	DTGetRTCTime(&local_time);
	rs_sprintf(buf, "%d-%d-%d %d:%d:%d %d ", local_time.nYear, local_time.nMonth, local_time.nDay, local_time.nHour, local_time.nMin, local_time.nSec, time_addr);
}
/*
函数说明：重启进入升级模式
特别说明：在其他的平台需要实现这个接口，windows模拟环境不需要实现
*/
void rs_sys_reboot()
{
	// 重启进入升级模式
	mmi_eq_power_on_req_struct *myMsgPtr;

	RS_PORITNG_LOG("boot system\n");

	myMsgPtr = (mmi_eq_power_on_req_struct*) OslConstructDataPtr(sizeof(mmi_eq_power_on_req_struct));
	myMsgPtr->fun = NORMAL_RESET;
	myMsgPtr->rst = 0;
//    MMI_TRACE(MMI_COMMON_TRC_G8_DEV, TRC_MMI_GPIO_PMIC_CHARGER_POWERKEY_ON);
	mmi_frm_send_ilm(MOD_L4C, MSG_ID_MMI_EQ_POWER_ON_REQ, 
                    (oslParaType*) myMsgPtr, NULL);
                    
	RS_PORITNG_LOG("boot system end\n");
}




/**
函数说明：向消息接收模块发送消息
参数说明：nMsgQParam 消息句柄
          uMsgEvent 消息事件
		 lParam1 消息参数1
		 lParam2 消息参数2
		 callback 消息接收模块收到消息之后，调用此接口处理消息
返回值：0表示成功，-1表示失败 
*/
rs_bool rs_sys_msg_queue_send(rs_s32 nMsgQParam, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2, RS_MSG_HANDLER_CALLBACK callback)
{
	rs_bool ret = RS_TRUE;
	mmi_event_struct dummy_evt;
	RS_MMI_Msg * msg;
	
	msg = (RS_MMI_Msg*)rs_malloc(sizeof(RS_MMI_Msg));
	if (msg == RS_NULL)
	{
		RS_PORITNG_LOG("rs_sys_msg_queue_send malloc fail\n");
		return RS_FALSE;
	}
	
	msg->callback = callback;
	msg->uMsgEvent = uMsgEvent;
	msg->lParam1 = lParam1;
	msg->lParam2 = lParam2;
	
	MMI_FRM_INIT_EVENT(&dummy_evt, 0);
	MMI_FRM_POST_EVENT(&dummy_evt, rs_sys_msg_queue_callback, msg);
	
	return ret;
}

/**
函数说明：输入日志到文件或者其他
参数说明：需要输出的日志内容
返回值：无
*/
void rs_cb_printLog(const rs_s8* msg)
{
	dbg_print((char*)msg);
	kal_prompt_trace( MOD_SOC, (char*)msg);
}

/**
函数说明：创建消息队列 
参数说明：userHandle 使用者的句柄
返回值：RS_NULL表示失败，否则为成功
*/
rs_s32 rs_sys_msg_queue_create(void* userHandle)
{
	g_msg_handle = userHandle;
	return 0;
}

/**
函数说明：启动定时器
参数说明：timerPeriod消息时间
返回值：返回定时器句柄，非0值表示成功，否则为失败 
*/
rs_s32 rs_sys_timer_start(rs_s32 timerPeriod, RS_TIMER_CALLBACK callback, void* data)
{
	StartTimerEx(TIMER_ID_RS_FOTA_TIMEOUT_TIMER, timerPeriod, callback, data);
	
	return TIMER_ID_RS_FOTA_TIMEOUT_TIMER;
}

/**
函数说明：取消定时器
参数说明：nTimerParam定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_timer_cancel(rs_s32 handle)
{
	rs_bool ret = RS_TRUE;
	if(handle != 0)
	{
		StopTimer(handle);
		handle = 0;
	}
	
	return ret;
}

void rs_sys_autocheck_callback(void* data)
{
	rs_s32 interCycle = rs_reg_get_internal_cycle();
	rs_u32 autoCheckCycle = rs_cb_get_auto_check_cycle();

	if (interCycle != 0)
		autoCheckCycle = interCycle;

	rs_sys_autocheck_timer_start(autoCheckCycle);

	rs_sdk_autoCheck();
}



/**
函数说明：启动主程序
参数说明：
返回值：rs_true表示成功，rs_false表示失败 
*/
/*根据不同的平台做不同的启动处理*/
rs_s32 rs_sys_autocheck_timer_start(rs_s32 timerPeriod)
{

	
	StartTimerEx(TIMER_ID_RS_FOTA_TIMER, timerPeriod, rs_sys_autocheck_callback, 0);
	
	return TIMER_ID_RS_FOTA_TIMER;
}

/**
函数说明：取消自动检测定时器
参数说明：handle定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_cancel(rs_s32 handle)
{
	rs_bool ret = RS_TRUE;
	if(handle != 0)
	{
		StopTimer(handle);
		handle = 0;
	}
	
	return ret;

}


/**
函数说明：当网络突发异常导致升级包下载失败，而下一次检测又太遥远（比如周期检测时间设置为3天），则
			应该作处理，让下一次检测快点到来
参数说明：无
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_restart()
{
	
	rs_u32 timerPeriod = rs_cb_get_once_check_cyle();
	if(rs_cb_get_auto_check_cycle()<= timerPeriod)//若本来设置的周期检测时间就比较短，
												 //就没必要作处理重新启动定时器，因为定时器计时很快就会快到了
	{
		return RS_FALSE;
	}
	//first stop timer
	if(!rs_sys_autocheck_timer_cancel(TIMER_ID_RS_FOTA_TIMER))
	{
		return RS_FALSE;
	}
	//start timer use shorten timePeriod
	StartTimerEx(TIMER_ID_RS_FOTA_TIMER, timerPeriod, rs_sys_autocheck_callback, 0);
	
	return RS_TRUE;
}



/**
函数说明：判断SIM是否已经插入
参数说明：无
返回值：RS_TRUE表示有效，RS_FALSE表示无效 
*/
rs_bool rs_sys_simcard_insert()
{
	if (!IsSATPresent())
	{
		RS_PORITNG_LOG("IsSATPresent return false\n");
		return RS_FALSE;
	}

	return RS_TRUE;
}

/**
函数说明：判断SIM是否可以识别
参数说明：无
返回值：RS_TRUE表示可以识别，RS_FALSE表示不能识别
*/
rs_bool rs_sys_simcard_recognize()
{
	if (!mmi_bootup_is_sim_valid())
	{
		RS_PORITNG_LOG("mmi_bootup_is_sim_valid return false\n");
		return RS_FALSE;
	}

	return RS_TRUE;
}

/**
函数说明：判断imei是否已经写入
参数说明：无
返回值：RS_TRUE表示已经写入，RS_FALSE表示没有写入
*/
rs_bool rs_sys_imei_valid()
{
	char imei_num[20] = {0};

	srv_imei_get_imei(MMI_SIM1, imei_num, 16);

	if (imei_num[0] == '\0') 
	{
		RS_PORITNG_LOG("srv_imei_get_imei can not get imei\n");
		return RS_FALSE;
	}

	RS_PORITNG_LOG("%s, imei_num[%s]\n\r", __func__, imei_num);

	return RS_TRUE;
}

/**
函数说明：获取已经下载差分升级包的md5值
参数说明：deltaSize 升级包的长度
		  md5String 升级包的md5的值
返回值：RS_TRUE表示成功，RS_FALSE表示失败 
*/
rs_bool rs_sys_get_updatefile_md5_string(rs_s32 deltaSize, rs_s8* md5String)
{
	applib_md5_ctx md5_context;
	rs_u8 buf[PAGE_SIZE];
	rs_s32 readSize = 0;
	rs_u8 md5_dec[16];
	rs_s32 i = 0;

#ifdef RS_SUPPORT_UPDATE_DATA_FS
	const rs_s8* filePath = rs_device_get_update_file();
	WCHAR fname_wstr[256];

	mmi_asc_to_wcs(fname_wstr, (CHAR*)filePath);
	applib_md5_init(&md5_context);
	applib_md5_file_update(&md5_context, fname_wstr);
	applib_md5_final(md5_dec, &md5_context);


	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5_dec[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5_dec[i] & 0x0F];
	}
	md5String[32] = 0;

	RS_PORITNG_LOG("MD5 string: %s\n", md5String);

	return RS_TRUE;
#else
	rs_u32 flashAddr = 0;
	
	applib_md5_init(&md5_context);
	flashAddr = rs_device_get_update_addr();

	while(readSize < deltaSize)
	{
		rs_s32 ret = 0;
		ret = rs_flash_readPage(flashAddr + readSize, buf, PAGE_SIZE);
		if (ret != RS_ERR_OK)
			return RS_FALSE;

		if (deltaSize - readSize > PAGE_SIZE)
		{
			applib_md5_update (&md5_context, buf, PAGE_SIZE);
			readSize += PAGE_SIZE;
		}
		else
		{
			applib_md5_update (&md5_context, buf, deltaSize - readSize);
			readSize = deltaSize;
		}
	}

	applib_md5_final(md5_dec, &md5_context);

	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5_dec[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5_dec[i] & 0x0F];
	}
	md5String[32] = 0;

	RS_PORITNG_LOG("MD5 string: %s\n", md5String);

	return RS_TRUE;
#endif	
}
rs_bool rs_sys_get_data_md5_string(rs_u8* src_data, rs_s32 data_size, rs_s8* md5String)
{
	applib_md5_ctx md5_context;
	rs_u8 md5_dec[16];
	rs_s32 i = 0;

	applib_md5_init(&md5_context);
	applib_md5_update (&md5_context, src_data, data_size);
	applib_md5_final(md5_dec, &md5_context);

	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5_dec[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5_dec[i] & 0x0F];
	}
	md5String[32] = 0;

	return RS_TRUE;	
}
rs_u32 rs_sys_get_pid()
{
	return 0;
}

/**
函数说明：读取外部配置文件debug.ini中的内容
		  返回结果作为系统的日志级别
		  文件地址在rs_param.h中RS_CONFIG_DEBUGINFO_FILE定义
参数说明：
返回值：返回数字0-3
*/
rs_u32 rs_sys_get_log_level()
{
	rs_u8 buf[10];
	RS_FILE rs_file = -1;
	static rs_u32 log_level = RS_DEFAULT_LEVEL;
	static int log_level_unknow = 1;
	if(log_level_unknow == 0){
		return log_level;
	}
	log_level_unknow = 0; //目标是为了以下语句当函数rs_sys_get_log_level第一次被调用时才会执行
	if(rs_fs_exists(RS_CONFIG_DEBUGINFO_FILE))
	{
		rs_file = rs_fs_open(RS_CONFIG_DEBUGINFO_FILE,RS_FS_OPEN_READ);
		if(rs_file != RS_FS_INVALID){//打开成功
			//到文件开始位置
			rs_fs_seek(rs_file,0,RS_FS_SEEK_SET);
			if(rs_fs_read(rs_file,buf,1)>0){
				if(buf[0]>='0' && buf[0]<='9'){					
					log_level = buf[0] - 0x30;
				}
			}
			rs_fs_close(rs_file);
			
		}
	}
	//printf("log_level=%d\n",log_level);
	
	return log_level;//数字越高可能的打印的log越多
		
	

}