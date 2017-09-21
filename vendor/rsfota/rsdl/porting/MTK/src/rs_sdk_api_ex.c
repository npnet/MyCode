#include "rs_sdk_api_ex.h"
#include "rs_param.h"
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_debug.h"


//#define EARLY_INVALD_VALUE_CHECK


/**
函数说明：对于sdk中提供的检测接口的一个扩展
参数说明：type 0 表示手动发起  1 表示自动发起
返回值：成功RS_ERR_OK（值为0），其它为失败
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
	
	// 非0值表示系统正忙，正在进行着请求 下载 或者升级
	return rs_sdk_check(type);
}

/**
函数说明：对于sdk中提供的下载接口的一个扩展，增加了空间的判断
参数说明：type 0 表示手动发起  1 表示自动发起
返回值：成功RS_ERR_OK（值为0），其它为失败
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
函数说明：安装已经下载的升级包
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
暴露给用户：是
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
	获取当前版本的长度
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
 * 获取新版本的版本号
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
 * 获取新版本的版本描述
*/
rs_s8* rs_sdk_new_version_description(void)
{
	const RS_FWDL_DD* dd = RS_NULL;
	
	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return RS_NULL;
		
	return (rs_s8*)dd->brief;
}
