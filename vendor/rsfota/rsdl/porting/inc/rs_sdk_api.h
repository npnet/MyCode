/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */
#ifndef _RS_SDK_API_H_
#define _RS_SDK_API_H_

#include "rs_datatype.h"
#include "rs_state.h"


#ifdef __cplusplus
extern "C" {
#endif

// 获取设备信息KEY
#define PROPERTY_KEY_DEVICE_DEVID		"product.devid"			//设备唯一ID
#define PROPERTY_KEY_DEVICE_MAN			"product.manu"			//设备厂商 
#define PROPERTY_KEY_DEVICE_MODEL		"product.model"			//设备型号
#define PROPERTY_KEY_DEVICE_FWV			"product.fwv"			//设备当前固件版本号
#define PROPERTY_KEY_DEVICE_LANG		"product.lang"			//设备使用语言
#define PROPERTY_KEY_DEVICE_OPERATORS	"product.operators"		//设备所属运营商，如46000
#define PROPERTY_KEY_DEVICE_OSV			"product.osv"			//获取系统的版本号
#define PROPERTY_KEY_DEVICE_LOC			"product.loc"			//获取位置信息
#define PROPERTY_KEY_DEVICE_UTDID		"product.utdid"			//设备唯一标识符
#define PROPERTY_KEY_DEVICE_APP_KEY		"product.appkey"		//app key
#define PROPERTY_KEY_DEVICE_CHANNEL		"product.channel"		//channel
#define PROPERTY_KEY_DEVICE_DEVMAC		"product.devmac"		//MAC

// 对于升级的描述信息
typedef struct _RS_FW_DD_ 
{
	rs_s8    key[64];				    //key值上限为32个字节，差分包id
	rs_s8    correlator[64];			//上限为32个字节，服务器端对应的唯一标识
	rs_s8    objectUri[516];			//url上限为512个字节，升级包下载地址
	rs_u32   objectSize;				//升级包大小
	rs_s8    toVersion[132];			//上限为128个字节，升级包目标版本
	rs_s8    hashValue[34];             //32位字符串
	rs_s8    dateLine[20];              //16为字符串，yyyyMMddTHHmmssZ
	rs_s8    brief[516];			    //上限为512个字节，升级包的描述信息
	rs_u32   autoDownload;				//是否自动下载，0非自动下载，1 WIFI下自动下载，2任意网络自动下载
	rs_u32	 forceUpdate;				//是否强制升级，0表示非强制升级，1表示强制升级
} RS_FWDL_DD;

/**
函数说明：启动TOTA服务
参数说明：无
返回值：成功返回RS_ERR_OK（值为0），其它为失败
特别说明：
*/
void rs_sdk_start();

/**
函数说明：检测固件升级
参数说明：type 0 表示手动发起  1 表示自动发起
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
*/
rs_s32 rs_sdk_check(rs_s32 type);

/**
函数说明：自动检测固件升级
参数说明：
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
暴露给用户：是
*/
rs_s32 rs_sdk_autoCheck();


/**
函数说明：下载升级包 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
*/
rs_s32 rs_sdk_download(rs_s32 type);

/**
函数说明：停止下载升级 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
*/
rs_s32 rs_sdk_stopDownload();


/**
函数说明：安装已经下载的升级包
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明：无
*/
rs_s32 rs_sdk_install();


/**
函数说明：获取到当前升级包的描述信息 
参数说明：无
返回值：成功返回DD信息的指针，否则返回为NULL。结构定义见RS_FW_DD
特别说明： 
*/
const RS_FWDL_DD* rs_sdk_getPkgDD();

/**
函数说明：获取已经下载升级包的大小
参数说明：无
返回值：返回已经下载升级包的大小
特别说明： 
暴露给用户：是
*/
rs_s32 rs_sdk_getAlreadyDownloadSize();

/**
函数说明：获取检测版本的发起类型，比如是自动发起  手动发起 
参数说明：无
返回值：返回rs_sdk_check发起的类型
特别说明： 
*/
rs_s32 rs_sdk_getCheckInitType();

/**
函数说明：放弃当前版本
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
特别说明： 如果检测到版本，或者已经下载完成部分，或者已经下载完成，都可以放弃此版本。如果正在下载，应该首先停止升级。
*/
rs_s32 rs_sdk_discardCurentPkg();

/**
函数说明：获取当前操作错误码
返回值：操作错误码，见rs_error.h定义
*/
rs_u32 rs_sdk_getLastError();

/**
函数说明：获取安装完成之后的错误码
参数说明：
返回值：返回错误码
*/
rs_s32 rs_sdk_getInstallErrCode();

/**
函数说明：获取当前的状态 
参数说明：无
返回值：返回当前的状态
暴露给用户：否
*/
RS_FWDL_STATE rs_sdk_getDLCurrentState();


/**
函数说明：判断是否需要做自动检测的功能
参数说明：
返回值：RS_TRUE表示需要 RS_FALSE表示不需要
*/
rs_bool rs_sdk_needAutoCheck();

#ifdef __cplusplus
}
#endif

#endif