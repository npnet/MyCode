#include "rs_notify_user.h"
#include "rs_system.h"
#include "rs_error.h"
#include "rs_debug.h"
#include "rs_std_fun.h"
#include "rs_param.h"

// MTK header
#include "MMIDataType.h" //for log
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "ImeiSrvGprot.h" //for device info
#include "kal_public_api.h"
#include "DtcntSrvIntStruct.h"
#include "DtcntSrvGprot.h"
#include "SSCStringHandle.h"
#include "Mmiapi_struct.h" //for message
#include "mmi_msg_struct.h"
#include "Mmi_frm_queue_gprot.h"
#include "Mmi_frm_events_gprot.h"
#include "Mmi_cb_mgr_gprot.h"
#include "FontRes.h"

/****************************全局变量或者函数申明**************************/
void* rs_malloc(unsigned int allocSize);
void rs_free(void* memBlock);
void rs_mem_info();
extern const sLanguageDetails mtk_gLanguageArray[];
extern srv_dtcnt_store_info_context g_srv_dtcnt_store_ctx;
extern char* release_verno();

/****************************局部函数*************************************/
typedef struct 
{
	rs_s32 param1;
	rs_s32 param2;
	rs_s32 exErrorCode;
} RS_EVENT_MSG;

void RSUA_Event_handler(mmi_event_struct* evt)
{
	rs_s32 ret = 0;
	rs_s32 param1 = 0;
	rs_s32 param2 = 0;
	rs_s32 exErrorCode = 0;
	RS_EVENT_MSG* userData = (RS_EVENT_MSG *)evt->user_data;
	rs_u32 errCode = rs_sdk_getLastError();
	rs_s32 initType = rs_sdk_getCheckInitType();
	
	if (userData == RS_NULL)
	{
		return;
	}
	else
	{
		param1 = userData->param1;
		param2 = userData->param2;
		exErrorCode = userData->exErrorCode;
		rs_free(userData);
	}
	
	RS_PORITNG_LOG("%s, evt->evt_id=%d, param1=%d, param2=%d, exErrorCode=%d\n\r", __func__, evt->evt_id, param1, param2, exErrorCode);


	switch(evt->evt_id)
	{
		case USER_CHANGE_STATE:
			if(param1 == RS_FWDL_STATE_CHECKED)
			{
				if (param2 != 0)
				{
					
					if (rs_cb_have_ui() == 1 || initType == 0)
					{
						// TODO:有UI界面，那么通知用户有新版；
					}
					else
					{
						// 没有界面直接进行下载
						ret = rs_sdk_download_ex(1);
						RS_PORITNG_LOG("%s, start to download ret = %d \n", __func__, ret);
					}
				}
				else
				{
					if (initType == 0)
					{
						//表示手动发起的，错误应该通知用户
						if (errCode == RS_ERR_OK)
						{
							// TODO:通知用户已经是最新版本
						}	
						else if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
						{
							// TODO:系统接口错误。比如读写flash等。
						} 
						else if (errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
						{
							// TODO:网络问题没有检测到升级版本
						}
						else
						{
							// TODO:其他未知错误
						}						
					}
				}
			}
			else if(param1 == RS_FWDL_STATE_DOWNLOADING)
			{
				// 表示下载处于暂停状态
				if (param2 == 1)
				{
					rs_mem_info();
					
					if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
					{
						// TODO:系统接口错误。比如读写flash等。
					}
					else if(errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
					{
						// TODO:网络问题导致下载中断了
					} 
					else if (errCode == RS_USER_CANCEL)
					{
						// TODO:用户取消了下载
					}
					else 
					{
						// TODO:其他未知错误
					}
					//add by liujw 20161101
					rs_sys_autocheck_timer_restart();//重新启动定时器快速进入下一次检测
					//add end 
				}
			}
			else if(param1 == RS_FWDL_STATE_DOWNLOADED)
			{
				rs_mem_info();
				
				RS_PORITNG_LOG("%s, download completed or fail,param2=%d \n", __func__, param2);

				
				if (param2 == 0)
				{
					
					if (rs_cb_have_ui() == 1  || initType == 0)
					{
						// TODO:如果有UI，那么提醒用户去升级。没有界面直接进行升级流程
					}
					else
					{
						// 没有界面直接进行升级
						ret = rs_sdk_install_ex();
						RS_PORITNG_LOG("%s, start to install ret = %d \n", __func__, ret);
					}
				}
				else if (param2 == 2)
				{
					// TODO:总失败的次数到达上限，底层放弃了升级包的下载。如果有UI，那么需要返回到用户检测界面，没有界面不用处理
				}
			}
			else if(param1 == RS_FWDL_STATE_INSTALLING)
			{
				if(param2 == 1)
				{
					// TODO:系统错误，不能写入命令到flash或者文件，或者调用重启接口错误
				}
			}
			else if(param1 == RS_FWDL_STATE_INSTALLED)
            {
                // 升级完成，一开机就会收到这个消息
                if (param2 == 0)
				{
					// TODO:安装成功，如果有UI需要通知用户
				}
				else
				{
					// TODO:安装失败，如果有UI需要通知用户
				}
            }
			break;
			
		case USER_DOWNLOAD_BREAK: // 如下消息都是开机之后发出的消息

			if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADING)
			{
				// TODO:如果用户上次关机之前正在进行下载任务，开机之后会有这个提示
			}
			else if (param1 == RS_FWDL_BREAK_STATE_RESUME)
			{
				// TODO:如果用户上次当前又处于暂停状态的下载升级包，开机之后会有这个提示，是否要继续
			}
			else if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADED)
			{
				// TODO:如果用户上次开机有下载完成的升级包，开机之后会有这个提示，是否进行升级	
			}
			break;
			
		case USER_DOWNLOAD_PROGRESS:
			{
				// TODO:在界面上显示进度，param1表示已经下载的数据的长度，param2表示总数据的长度
			}
			break;
			
		case USER_STARTED:
			{
				// OTA已经启动完成
			}
			break;
		default:
			break;
	}
}


static void _send_msg_to_ui(rs_u32 Msg, rs_s32 wParam, rs_s32 lParam, rs_s32 exErrorCode)
{
	rs_s32 ret = 0;
	mmi_event_struct dummy_evt;
	RS_EVENT_MSG * msg;
	
	RS_PORITNG_LOG("%s, Msg=%d, wParam=%d, lParam=%d\n\r", __func__, Msg, wParam, lParam);
	
	msg = (RS_EVENT_MSG*)rs_malloc(sizeof(RS_EVENT_MSG));
	if (msg == RS_NULL)
	{
		RS_PORITNG_LOG("%s, memery alloc fail\n\r", __func__);
		return;
	}
	
	msg->param1 = wParam;
	msg->param2 = lParam;
	msg->exErrorCode = exErrorCode;
	
	MMI_FRM_INIT_EVENT(&dummy_evt, Msg);
	MMI_FRM_POST_EVENT(&dummy_evt, RSUA_Event_handler, msg);
}


/********************************SDK需要实现的回调*********************************/

/**
函数说明：核心模块执行状态通知，比如开始检测的时候会发一个消息，开始下载的时候会发一个消息
参数说明：【state】表示当前正处于的一种状态
返回值：VOID
*/
void rs_cb_notify_state( RS_FWDL_STATE state, rs_s32 code)
{
	RS_PORITNG_LOG("recv rs_cb_notify_state msg, state = %d , code = %d\n", state, code);

	// 需要注意RS_FWDL_STATE_DOWNLOADED，code==2的时候，需要直接放弃当前pkg，因为sdk已经放弃了，应该直接回到检查界面

	_send_msg_to_ui((rs_u32)USER_CHANGE_STATE, (rs_s32)state, (rs_s32)code, 0);
}


/**
函数说明：这个回调函数是针对OTA的业务特点，额外增加的回调函数，开机之后,
		  如果上次开机正处于某种状态下面,那么开机之后提醒用户去处理，比如正在
		  下载中，那么上次关机之前正处于下载状态；下载完成，开机之后给用户一个提醒；
		  下载暂停，开机之后提醒用户继续下载。
		  
参数说明：【state】表示当前正处于的一种状态
返回值：VOID
*/
void rs_cb_notify_break_state( RS_FWDL_BREAK_STATE state)
{
	RS_PORITNG_LOG("recv rs_cb_notify_break_state msg, state = %d \n", state);


	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_BREAK, (rs_s32)state, 0, 0);
}


/**
函数说明：操作进度指示 
参数说明：【current】表示当前已经下载的总量
  【total】表示总量
返回值：VOID
*/
void rs_cb_progress(rs_s32 current, rs_s32 total)
{
	RS_PORITNG_LOG("recv rs_cb_progress msg, current = %d , total = %d\n", current, total);


	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_PROGRESS, (rs_s32)current, (rs_s32)total, 0);
}

/**
函数说明：表示OTA的核心模块已经启动完成
参数说明：
返回值：VOID
*/
void rs_cb_started()
{
	RS_PORITNG_LOG("ota started ------------^-^---------------\n");

	_send_msg_to_ui((rs_u32)USER_STARTED, 0, 0, 0);
}
