#include "rs_param.h"
#include "rs_system.h"
#include "rs_error.h"
#include "rs_datatype.h"
#include "rs_sdk_api.h"
#include "rs_state.h"
#include "rs_std_fun.h"
#include "rs_debug.h"
#include "rs_sdk_api_ex.h"


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
#include "CharBatSrvGprot.h"
#include "uem_utility.h"


/***********************************************************************************/
//extern kal_uint32 Image$$VIVA_GFH$$Base;
extern const sLanguageDetails mtk_gLanguageArray[];
extern srv_dtcnt_store_info_context g_srv_dtcnt_store_ctx;
extern char* release_verno();
/***********************************************************************************/

/*
 *	��ȡ�豸Ʒ��
 */
static void get_OEMBrand(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡƷ�ƵĽӿ�
#else // default
	rs_strcpy(buf, "BIRD");
	return ;
#endif
}

/*
 *	��ȡ�ͺ�
*/
static void get_OEMDevice(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡ�ͺŵĽӿ�
#else //default
	rs_strcpy(buf, "DW60_61A_OTA");
	return ;
#endif
}


/*
 * ��ȡ�豸��Ψһ��ʾ����
*/
static void get_OEMIMEI(char *buf)
{

	char imei[21] = {0};
	char imei_num[16] = {0};

	rs_strcpy(imei, "IMEI:");
	srv_imei_get_imei(MMI_SIM1, imei_num, 16);
	dbg_print("%s, imei_num[%s]\n\r", __func__, imei_num);
	kal_prompt_trace( MOD_SOC, "%s, imei_num[%s]\n\r", __func__, imei_num);
	if (imei_num[0] == '\0') {
		rs_strcpy(imei+5, "001068000000006");
	} else {
		rs_strcpy(imei+5, imei_num);
	}

	rs_strcpy(buf, imei);
	dbg_print("%s, IMEI[%s], buf[%s]\n\r", __func__, imei, buf);
	kal_prompt_trace( MOD_SOC, "%s, IMEI[%s], buf[%s]\n\r", __func__, imei, buf);
	

}

/*
 * ��ȡ��ǰ�豸������汾��
*/
static void get_OEMCurrentVersion(char *buf)
{

	char *ptr = NULL;

	ptr = "DW60_OTA_V1.00";//release_verno();
	dbg_print("%s, verno[%s]\n\r", __func__, ptr);
	kal_prompt_trace( MOD_SOC, "%s, verno[%s]\n\r", __func__, ptr);
	
	rs_strcpy(buf, ptr);

}


/*
 * ��ȡ��ǰ�豸����������
*/
static void get_OEMLanguage(char *buf)
{

	char language[32];
	kal_uint8 lang_index;
	char *lang_ptr;

	lang_index = srv_setting_get_language();
	lang_ptr = (char *)mtk_gLanguageArray[lang_index].aLangSSC;
	if (rs_strcmp(lang_ptr, SSC_ENGLISH) == 0)
		rs_strcpy(buf, "en");
	else if (rs_strcmp(lang_ptr, SSC_SCHINESE) == 0)
		rs_strcpy(buf, "zh-CN");
	else if (rs_strcmp(lang_ptr, SSC_THAI) == 0)
		rs_strcpy(buf, "zh-CN");
	else if (rs_strcmp(lang_ptr, SSC_INDONESIAN) == 0)
		rs_strcpy(buf, "zh-CN");
	else if (rs_strcmp(lang_ptr, SSC_ARABIC) == 0)
		rs_strcpy(buf, "zh-CN");
	else if (rs_strcmp(lang_ptr, SSC_TURKISH) == 0)
		rs_strcpy(buf, "zh-CN");
	else
		rs_strcpy(buf, "en");

}

/*
 * ��ȡ��Ӫ�̵�ID
*/
static void get_OEMCatoragy(char *buf)
{
	// get catoragy info

	kal_int8 i;
	char *ptr = NULL;

	for (i = 0; i < SRV_DTCNT_PROF_MAX_ACCOUNT_NUM; i++) {

		ptr = (char *)g_srv_dtcnt_store_ctx.acc_list[i].dest_name;

		dbg_print("%s, i[%d], %s\n\r", __func__, i, ptr);
		kal_prompt_trace( MOD_SOC, "%s, i[%d], %s\n\r", __func__, i, ptr);
		if (rs_strcmp(ptr, "cmnet") == 0 || rs_strcmp(ptr, "cmwap") == 0) {
			rs_strcpy(buf, "46000");//�ƶ�
			return ;
		} else if (rs_strcmp(ptr, "uninet") == 0 || rs_strcmp(ptr, "uniwap") == 0) {
			rs_strcpy(buf, "46001");//��ͨ
			return ;
		} else if (rs_strcmp(ptr, "ctnet") == 0) {
			rs_strcpy(buf, "46003");//����
			return ;
		}
	}
	rs_strcpy(buf, "UNKOWN");//δ֪

}

/*
 * ��ȡ��Ӫ�̵�ID
*/
static void get_OEMOSVersion(char *buf)
{
	rs_strcpy(buf, "MTK6261A");
}

/*
 * ��ȡapp key
*/
static void get_AppKey(char *buf)
{
	rs_strcpy(buf, "pkkfjsxxalryhizzqva4kgca");
	//rs_strcpy(buf, "xa8usywxwy2evgwoti1kh8p7");
}

/*
 * ��ȡ������
*/
static void get_Channel(char *buf)
{
	rs_strcpy(buf, "bird");
}

/**
����˵������ȡ�豸����
����˵������key���豸���Ա�ʶ
		  ��value������ֵ
����ֵ���ɹ�����0��ʧ�ܷ���1
ע�⣺value�ĳ���Ϊ64���ֽڣ���Ҫ��Ҫ����63���ֽ�
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value)
{
	if (0 == key || 0 == value) 
		return 1;

	if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVID, key)) // ��ȡ�豸ID
	{
		get_OEMIMEI(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MAN, key)) // ��ȡƷ��
	{
		get_OEMBrand(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MODEL, key)) // ��ȡ�ͺ�
	{
		get_OEMDevice(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_FWV, key)) // ��ȡ��ǰ������汾
	{
		get_OEMCurrentVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LANG, key)) // ��ȡ��ǰ������
	{
		get_OEMLanguage(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OPERATORS, key)) // ��ȡ��Ӫ�̵�ID
	{
		get_OEMCatoragy(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OSV, key)) // ��ȡϵͳ�İ汾��
	{
		get_OEMOSVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_APP_KEY, key)) // ��ȡapp key
	{
		get_AppKey(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_CHANNEL, key)) // ��ȡ������
	{
		get_Channel(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVMAC, key)) // ��ȡ������
	{
		value[0] = 0;
	}
	else
	{
		return 1;
	}
	
	if (key != 0 && value != 0 && (rs_strlen(key) > 0) && (rs_strlen(value) > 0))
	{
		RS_PORITNG_LOG("get device atrr, key=%s, value = %s \r\n", key, value);
	}

	return 0;
}


/**
����˵������ѯ�Ƿ����û�����
����˵����
����ֵ������0����ʾû��UI������1����ʾ��UI
����˵������û��UI�����ĳ������ǲ�ͬ�ġ������ϴ��������ع����У�����ĳ���������
�����ֻ���������ô����֮�������UI�����û����������û��UI����������
*/
rs_s32 rs_cb_have_ui()
{
	return 0;
}

/**
����˵������ȡ��һ���Զ������뿪����ʱ������
����˵����
����ֵ������ֵΪ���뿪���ĺ���ʱ��
*/
rs_u32 rs_cb_get_first_check_cycle()
{
	//return (60*1000);
	return (60*1000);
}

/**
����˵������һ���Զ�����������֮��ÿ���Զ����ļ����ʱ��
����˵����
����ֵ������ֵΪÿ���Լ������ڵļ��ʱ��
*/
rs_u32 rs_cb_get_auto_check_cycle()
{
	//return (24*60*60*1000);
	return (1*60*1000);
}
/**
����˵������ʱ�����粻�õ�������������ʧ�ܣ������´μ��ʱ���ֻ�Ҫ�ܾã�
		  ������Ҫ��ʱ�Ѽ��ʱ������
����˵����
����ֵ������ֵΪ�´μ��ĵļ��ʱ�䣨һ����Ч��
*/
rs_u32 rs_cb_get_once_check_cyle()
{
	return (20*1000);
}

/**
����˵�����Զ�������ڵ���
����˵������
����ֵ���ɹ��Ż�RS_ERR_OK, ʧ�ܷ�������ֵ
*/
rs_s32 rs_cb_auto_check()
{
	rs_s32 ret = RS_ERR_OK;
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();

	if(rs_cb_have_ui() == 0)
	{
		// û��UI
		if (state == RS_FWDL_STATE_DOWNLOAD_PAUSE)
		{
			// ����ϳ���ʲô�쳣��������ֱ������
			ret = rs_sdk_download_ex(1);
			return ret;
		}
        else if(state == RS_FWDL_STATE_DOWNLOADED)
        {
            // ����Ѿ����ص�δ��װ�����ֱ�Ӱ�װ
            ret =  rs_sdk_install_ex();
            return ret;
        }
	}

	ret = rs_sdk_check_ex(1); // �����ڲ��ж�״̬���жϣ������ʱ�����ʽ��м�⣬�������˳�

	return ret;
}

/**
����˵����������������Դ���
����˵������
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_check_retry_count()
{
	return 2;
}

/**
����˵������ȡ����������Դ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_retry_count()
{
	return 15;
}

/**
����˵������ȡ��������ϱ�������Դ���
����˵����
����ֵ���������Դ���
�ر�˵���������Դ�����Ҫ���ô�һ�㣬�������ϱ��ɹ���������ں����ж����Ӱ��
*/
rs_s32 rs_cb_get_report_retry_count()
{
	return 3;
}

/**
����˵������ȡ���ع��������ʧ�ܲ��ԣ�rs_cb_get_download_retry_count�������ع��������ʧ���˻��Զ����ԵĴ���
		����ܴ��������˴˺����趨�����ޣ���ô��������������
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_max_fail_count()
{
	return 10;
}

/**
����˵������ȡ�ϱ����������ʧ�ܲ��ԣ�rs_cb_get_report_retry_count�����ϱ����������ʧ���˻��Զ����ԵĴ�����
		����ܴ���������rs_cb_get_report_max_fail_count�����趨�����ޣ���ô�����ϱ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_report_max_fail_count()
{
	return 5;
}

/**
����˵����socket�����й����еĸ��ֳ�ʱ����
����˵����type ����ͬ��ʱ����
			0  gethostbyname�ĳ�ʱ����
			1  connect�ĳ�ʱ����
			2  send�ĳ�ʱ����
			3  recv�ĳ�ʱ����
����ֵ�����س�ʱ��ʱ�����ڣ���λΪ����
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type)
{
	rs_s32 timeout = 0;

	switch(type)
	{
	case 0: // gethostbyname ��ʱʱ��
		timeout = 30*1000;
		break;
	case 1: // connect ��ʱʱ��
		timeout = 30*1000;
		break;
	case 2: // send ��ʱʱ��
		timeout = 30*1000;
		break;
	case 3: // recv ��ʱʱ��
		timeout = 30*1000;
		break;
	case 4: // session ���ʱ��
		timeout = 10*1000;
		break;
	}

	return timeout;
}


/**
����˵������ȡ��ǰ���ʣ������Ƿ����
����˵����
����ֵ���������ڵ���33%,�򷵻�RS_TRUE,����RS_FALSE
�ر�˵��:bmt_status.volt��ص�ѹ�������п��ܸ��ݵ�ѹ�жϵ�ǰ���Ӿ�ȷ��
����ʣ�������������Ϊ���Ϊ3593500,���Ϊ4253900���ֳ�100�֣����ǰ�volt
��ӡ������Ϊ�˽�
������ע�⣬��MMITask.c��init�����ϵ������½���ǲ��еģ��п��ܳ���û�г�ʼ��
*/
rs_bool rs_sys_bettery_electricity_enough()
{
	uem_bmt_struct bmt_status;
	srv_charbat_battery_level_enum level = srv_charbat_get_battery_level();
	uem_get_bat_status(&bmt_status);

	RS_PORITNG_LOG("*****current bettery level = %d, volt=%d\n",  level, bmt_status.volt);

	if (level >= BATTERY_LEVEL_1)
	{
		return RS_TRUE;
	}
	else
	{
		return RS_FALSE;
	}
}


/**
����˵������ȡ��ǰ���õĿռ�
����˵����
����ֵ�����ص�ǰ���õ�ʣ��ռ�,��λΪbyte
*/
rs_u32 rs_sys_available_space()
{
	U32 disk_free_space = 0;
#ifdef RS_SUPPORT_UPDATE_DATA_FS
    FS_DiskInfo disk_info;
    int result = 0;

    result = FS_GetDiskInfo(L"C:", &disk_info, FS_DI_BASIC_INFO | FS_DI_FREE_SPACE);

    if (result >= 0)
    {        
        disk_free_space = disk_info.FreeClusters * 
                          disk_info.SectorsPerCluster * 
                          disk_info.BytesPerSector;
        
    }
#else
	kal_uint32 fs_start_address = custom_get_FAT_StartAddr();

	disk_free_space = (U32)((fs_start_address&0x0FFFFFFF) - FLASH_UPDATE_ADDR - 3 * BLOCK_SIZE);
	RS_PORITNG_LOG("jintao fs start address[%x], free space[%x]\n", fs_start_address, disk_free_space);
#endif

	return disk_free_space;
}


#ifdef RS_SUPPORT_UPDATE_DATA_FS

/**
����˵������ȡ������������·��
����˵����
����ֵ����������·��
*/
const rs_s8* rs_device_get_update_file()
{
	return FLASH_UPDATE_FILE;
}

#else

/*
������������ݴ洢��flash�У���ô������Ϣ����Ҳ�϶���Ҫ��flash�д洢��
��Ϊbootloader��Ҫ��ȡ
*/

/**
����˵������ȡ�����������ش�ŵ�flash�ĵ�ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_addr()
{
	return FLASH_UPDATE_ADDR;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info1_addr()
{
	return RS_CONFIG_UPINFO_ADDR1;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info2_addr()
{
	return RS_CONFIG_UPINFO_ADDR2;
}

#endif

#ifdef RS_SUPPORT_UPDATE_INFO_FS

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info1_file()
{
	return RS_CONFIG_UPINFO_FILE1;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info2_file()
{
	return RS_CONFIG_UPINFO_FILE2;
}

#endif



/**
����˵������ȡVIVA����ʼ��ַ
����˵������
����ֵ������VIVA����ʼ��ַ�����û��VIAVA�����򷵻�0 
*/
rs_u32 rs_sys_getVIVAStartAddr()
{
	//RS_PORITNG_LOG("viva address is 0x%x\n\r", ((rs_u32)&Image$$VIVA_GFH$$Base) & 0x00FFFFFF);
	//return (((rs_u32)&Image$$VIVA_GFH$$Base) & 0x00FFFFFF); 
	return 0;
}