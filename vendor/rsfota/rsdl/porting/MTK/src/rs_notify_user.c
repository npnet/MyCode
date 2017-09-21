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

/****************************ȫ�ֱ������ߺ�������**************************/
void* rs_malloc(unsigned int allocSize);
void rs_free(void* memBlock);
void rs_mem_info();
extern const sLanguageDetails mtk_gLanguageArray[];
extern srv_dtcnt_store_info_context g_srv_dtcnt_store_ctx;
extern char* release_verno();

/****************************�ֲ�����*************************************/
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
						// TODO:��UI���棬��ô֪ͨ�û����°棻
					}
					else
					{
						// û�н���ֱ�ӽ�������
						ret = rs_sdk_download_ex(1);
						RS_PORITNG_LOG("%s, start to download ret = %d \n", __func__, ret);
					}
				}
				else
				{
					if (initType == 0)
					{
						//��ʾ�ֶ�����ģ�����Ӧ��֪ͨ�û�
						if (errCode == RS_ERR_OK)
						{
							// TODO:֪ͨ�û��Ѿ������°汾
						}	
						else if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
						{
							// TODO:ϵͳ�ӿڴ��󡣱����дflash�ȡ�
						} 
						else if (errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
						{
							// TODO:��������û�м�⵽�����汾
						}
						else
						{
							// TODO:����δ֪����
						}						
					}
				}
			}
			else if(param1 == RS_FWDL_STATE_DOWNLOADING)
			{
				// ��ʾ���ش�����ͣ״̬
				if (param2 == 1)
				{
					rs_mem_info();
					
					if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
					{
						// TODO:ϵͳ�ӿڴ��󡣱����дflash�ȡ�
					}
					else if(errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
					{
						// TODO:�������⵼�������ж���
					} 
					else if (errCode == RS_USER_CANCEL)
					{
						// TODO:�û�ȡ��������
					}
					else 
					{
						// TODO:����δ֪����
					}
					//add by liujw 20161101
					rs_sys_autocheck_timer_restart();//����������ʱ�����ٽ�����һ�μ��
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
						// TODO:�����UI����ô�����û�ȥ������û�н���ֱ�ӽ�����������
					}
					else
					{
						// û�н���ֱ�ӽ�������
						ret = rs_sdk_install_ex();
						RS_PORITNG_LOG("%s, start to install ret = %d \n", __func__, ret);
					}
				}
				else if (param2 == 2)
				{
					// TODO:��ʧ�ܵĴ����������ޣ��ײ�����������������ء������UI����ô��Ҫ���ص��û������棬û�н��治�ô���
				}
			}
			else if(param1 == RS_FWDL_STATE_INSTALLING)
			{
				if(param2 == 1)
				{
					// TODO:ϵͳ���󣬲���д�����flash�����ļ������ߵ��������ӿڴ���
				}
			}
			else if(param1 == RS_FWDL_STATE_INSTALLED)
            {
                // ������ɣ�һ�����ͻ��յ������Ϣ
                if (param2 == 0)
				{
					// TODO:��װ�ɹ��������UI��Ҫ֪ͨ�û�
				}
				else
				{
					// TODO:��װʧ�ܣ������UI��Ҫ֪ͨ�û�
				}
            }
			break;
			
		case USER_DOWNLOAD_BREAK: // ������Ϣ���ǿ���֮�󷢳�����Ϣ

			if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADING)
			{
				// TODO:����û��ϴιػ�֮ǰ���ڽ����������񣬿���֮����������ʾ
			}
			else if (param1 == RS_FWDL_BREAK_STATE_RESUME)
			{
				// TODO:����û��ϴε�ǰ�ִ�����ͣ״̬������������������֮����������ʾ���Ƿ�Ҫ����
			}
			else if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADED)
			{
				// TODO:����û��ϴο�����������ɵ�������������֮����������ʾ���Ƿ��������	
			}
			break;
			
		case USER_DOWNLOAD_PROGRESS:
			{
				// TODO:�ڽ�������ʾ���ȣ�param1��ʾ�Ѿ����ص����ݵĳ��ȣ�param2��ʾ�����ݵĳ���
			}
			break;
			
		case USER_STARTED:
			{
				// OTA�Ѿ��������
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


/********************************SDK��Ҫʵ�ֵĻص�*********************************/

/**
����˵��������ģ��ִ��״̬֪ͨ�����翪ʼ����ʱ��ᷢһ����Ϣ����ʼ���ص�ʱ��ᷢһ����Ϣ
����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_state( RS_FWDL_STATE state, rs_s32 code)
{
	RS_PORITNG_LOG("recv rs_cb_notify_state msg, state = %d , code = %d\n", state, code);

	// ��Ҫע��RS_FWDL_STATE_DOWNLOADED��code==2��ʱ����Ҫֱ�ӷ�����ǰpkg����Ϊsdk�Ѿ������ˣ�Ӧ��ֱ�ӻص�������

	_send_msg_to_ui((rs_u32)USER_CHANGE_STATE, (rs_s32)state, (rs_s32)code, 0);
}


/**
����˵��������ص����������OTA��ҵ���ص㣬�������ӵĻص�����������֮��,
		  ����ϴο���������ĳ��״̬����,��ô����֮�������û�ȥ������������
		  �����У���ô�ϴιػ�֮ǰ����������״̬��������ɣ�����֮����û�һ�����ѣ�
		  ������ͣ������֮�������û��������ء�
		  
����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_break_state( RS_FWDL_BREAK_STATE state)
{
	RS_PORITNG_LOG("recv rs_cb_notify_break_state msg, state = %d \n", state);


	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_BREAK, (rs_s32)state, 0, 0);
}


/**
����˵������������ָʾ 
����˵������current����ʾ��ǰ�Ѿ����ص�����
  ��total����ʾ����
����ֵ��VOID
*/
void rs_cb_progress(rs_s32 current, rs_s32 total)
{
	RS_PORITNG_LOG("recv rs_cb_progress msg, current = %d , total = %d\n", current, total);


	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_PROGRESS, (rs_s32)current, (rs_s32)total, 0);
}

/**
����˵������ʾOTA�ĺ���ģ���Ѿ��������
����˵����
����ֵ��VOID
*/
void rs_cb_started()
{
	RS_PORITNG_LOG("ota started ------------^-^---------------\n");

	_send_msg_to_ui((rs_u32)USER_STARTED, 0, 0, 0);
}
