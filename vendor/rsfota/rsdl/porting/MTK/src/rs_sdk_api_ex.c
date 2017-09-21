#include "rs_sdk_api_ex.h"
#include "rs_param.h"
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_debug.h"


//#define EARLY_INVALD_VALUE_CHECK


/**
����˵��������sdk���ṩ�ļ��ӿڵ�һ����չ
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_check_ex(rs_s32 type)
{
	rs_s32 ret = 0;

#ifdef EARLY_INVALD_VALUE_CHECK
	if(rs_sys_simcard_insert() == RS_FALSE)
	{
		return RS_CARD_NOT_INSERT;
	}

	if (rs_sys_simcard_recognize() == RS_FALSE)
	{
		return RS_CARD_NOT_RECOGNIZE;
	}

	if (rs_sys_imei_valid() == RS_FALSE)
	{
		return RS_IMEI_NOT_WRITTEN;
	}
#endif
	
	// ��0ֵ��ʾϵͳ��æ�����ڽ��������� ���� ��������
	return rs_sdk_check(type);
}

/**
����˵��������sdk���ṩ�����ؽӿڵ�һ����չ�������˿ռ���ж�
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_download_ex(rs_s32 type)
{
	rs_u32 availableSpace = rs_sys_available_space();
	rs_s32 alreadyDown = rs_sdk_getAlreadyDownloadSize();
	const RS_FWDL_DD*dd = rs_sdk_getPkgDD();
	rs_u32 needSpace = 0;

#ifdef EARLY_INVALD_VALUE_CHECK
	if(rs_sys_simcard_insert() == RS_FALSE)
	{
		return RS_CARD_NOT_INSERT;
	}

	if (rs_sys_simcard_recognize() == RS_FALSE)
	{
		return RS_CARD_NOT_RECOGNIZE;
	}

	if (rs_sys_imei_valid() == RS_FALSE)
	{
		return RS_IMEI_NOT_WRITTEN;
	}
#endif

	needSpace = dd->objectSize - alreadyDown;

	if (availableSpace < needSpace)
	{
		return RS_ERR_SPACE_NOT_ENGHOU;
	}
	else
	{
		return rs_sdk_download(type);
	}
}


/**
����˵������װ�Ѿ����ص�������
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
��¶���û�����
*/
rs_s32 rs_sdk_install_ex()
{
	if (rs_sys_bettery_electricity_enough() == RS_FALSE)
	{
		return RS_ERR_BATTERY_LEVEL_LOW;
	}
	
	return rs_sdk_install();
}

/*
	��ȡ��ǰ�汾�ĳ���
*/
rs_u32 rs_sdk_new_version_size(void)
{
	const RS_FWDL_DD* dd = RS_NULL;
	
	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return -1;
		
	return dd->objectSize;
}

/*
 * ��ȡ�°汾�İ汾��
*/
rs_s8* rs_sdk_new_version_name(void)
{
	const RS_FWDL_DD* dd = RS_NULL;
	
	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return RS_NULL;
		
	return (rs_s8*)dd->toVersion;
}

/*
 * ��ȡ�°汾�İ汾����
*/
rs_s8* rs_sdk_new_version_description(void)
{
	const RS_FWDL_DD* dd = RS_NULL;
	
	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return RS_NULL;
		
	return (rs_s8*)dd->brief;
}
