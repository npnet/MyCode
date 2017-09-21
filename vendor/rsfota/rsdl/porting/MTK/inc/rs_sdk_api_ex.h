#ifndef _RS_SDK_API_EX_H_
#define _RS_SDK_API_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_datatype.h"

/**
函数说明：对于sdk中提供的检测接口的一个扩展
参数说明：type 0 表示手动发起  1 表示自动发起
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_sdk_check_ex(rs_s32 type);


/**
函数说明：对于sdk中提供的下载接口的一个扩展，增加了空间的判断
参数说明：type 0 表示手动发起  1 表示自动发起
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_sdk_download_ex(rs_s32 type);

/**
函数说明：安装已经下载的升级包
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
暴露给用户：是
*/
rs_s32 rs_sdk_install_ex();

/*
	获取当前版本的长度
*/
rs_u32 rs_sdk_new_version_size(void);

/*
 * 获取新版本的版本号
*/
rs_s8* rs_sdk_new_version_name(void);

/*
 * 获取新版本的版本描述
*/
rs_s8* rs_sdk_new_version_description(void);

#ifdef __cplusplus
}
#endif

#endif