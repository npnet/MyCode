#ifndef _RS_PARAM_H_
#define _RS_PARAM_H_

#include "rs_datatype.h"
#include "rs_porting_cfg.h"
#include "rs_dev.h"

/**
函数说明：获取设备属性
参数说明：【key】设备属性标识
		  【value】属性值
返回值：成功返回0，失败返回1
注意：value的长度为64个字节，主要不要超过63个字节
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value);

/**
函数说明：查询是否有用户界面
参数说明：
返回值：返回0，表示没有UI；返回1，表示有UI
场景说明：有没有UI在许多的场景还是不同的。比如上次正在下载过程中，由于某种特殊情况
导致手机重启，那么开机之后，如果有UI，由用户触发，如果没有UI则主动发起
*/
rs_s32 rs_cb_have_ui();

/**
函数说明：获取第一次自动检测距离开机的时间周期
参数说明：
返回值：返回值为距离开机的毫秒时间
*/
rs_u32 rs_cb_get_first_check_cycle();

/**
函数说明：第一次自动检测周期完成之后，每次自动检测的间隔的时间
参数说明：
返回值：返回值为每次自检测的周期的间隔时间
*/
rs_u32 rs_cb_get_auto_check_cycle();

/**
函数说明：有时候网络不好导致下载升级包失败，而离下次检测时间又还要很久，
		  所以需要临时把检测时间缩短
参数说明：
返回值：返回值为下次检测的的间隔时间（一次有效）
*/
rs_u32 rs_cb_get_once_check_cyle();


/**
函数说明：自动检测周期到达
参数说明：无
返回值：成功放回RS_ERR_OK, 失败返回其他值
*/
rs_s32 rs_cb_auto_check();

/**
函数说明：允许检测最大重试次数
参数说明：无
返回值：返回重试次数
*/
rs_s32 rs_cb_get_check_retry_count();

/**
函数说明：获取下载最大重试次数
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_retry_count();

/**
函数说明：获取上报过程中最大失败测试，rs_cb_get_report_retry_count是在上报过程中如果失败了会自动重试的次数。
		如果总次数超过了rs_cb_get_report_max_fail_count函数设定的上限，那么放弃上报。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_report_max_fail_count();

/**
函数说明：获取升级完成上报最大重试次数
参数说明：
返回值：返回重试次数
特别说明：此重试次数需要设置大一点，尽量让上报成功，否则对于后期判断造成影响
*/
rs_s32 rs_cb_get_report_retry_count();

/**
函数说明：获取下载过程中最大失败测试，rs_cb_get_download_retry_count是在下载过程中如果失败了会自动重试的次数
		如果总次数超过了此函数设定的上限，那么放弃此升级包。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_max_fail_count();

/**
函数说明：socket在运行过程中的各种超时设置
参数说明：type 代表不同超时类型
			0  gethostbyname的超时设置
			1  connect的超时设置
			2  send的超时设置
			3  recv的超时设置
返回值：返回超时的时间周期，单位为毫秒
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type);

/**
函数说明：获取当前电池剩余的量是否充足
参数说明：
返回值：电量大于等于33%,则返回RS_TRUE,否则RS_FALSE
*/
rs_bool rs_sys_bettery_electricity_enough();

/**
函数说明：获取当前可用的空间
参数说明：
返回值：返回当前可用的剩余空间
*/
rs_u32 rs_sys_available_space();

//定义日志等级配置文件路径
#define RS_CONFIG_DEBUGINFO_FILE "Z:\\debug.ini"

// 定义升级信息存储于文件路径或者flash的起始地址
#ifdef RS_SUPPORT_UPDATE_INFO_FS

#define RS_CONFIG_UPINFO_FILE1 "Z:\\updateinfo1"
#define RS_CONFIG_UPINFO_FILE2 "Z:\\updateinfo2"
#define RS_CONFIG_UPINFO_FILE1 "Z:\\@rsfota\\updateinfo1"
#define RS_CONFIG_UPINFO_FILE2 "Z:\\@rsfota\\updateinfo2"

/**
函数说明：获取升级包信息的存储路径
参数说明：
返回值：返回存储路径
*/
const rs_s8* rs_device_get_update_info1_file();
const rs_s8* rs_device_get_update_info2_file();

#endif

#define BLOCK_SIZE (0x8000) //32768
#define PAGE_SIZE 512


// 定义下载文件存储的文件路径或者flash中的起始地址
#ifdef RS_SUPPORT_UPDATE_DATA_FS

#define FLASH_UPDATE_FILE "Z:\\update.bin"
#define FLASH_UPDATE_FILE "Z:\\@rsfota\\update.bin"

/**
函数说明：获取升级包的下载路径
参数说明：
返回值：返回下载路径
*/
const rs_s8* rs_device_get_update_file();

#else

/*
如果升级包数据存储于flash中，那么升级信息数据也肯定是要在flash中存储的
因为bootloader需要读取
*/

// 差分包存储地址
#define FLASH_UPDATE_ADDR 0X580000

/**
函数说明：获取升级包的下载存放的flash的地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_addr();

// 这个值定义来源于custom_MemoryDevice.h
// 第一个值为fs的地址减去2个block，第二个为减去1个block
#define RS_CONFIG_UPINFO_ADDR1  0x6F8000
#define RS_CONFIG_UPINFO_ADDR2  0x6F0000
/**
函数说明：获取升级包信息的存储地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_info1_addr();
rs_u32 rs_device_get_update_info2_addr();

#endif


/**
函数说明：获取VIVA的起始地址
参数说明：无
返回值：返回VIVA的起始地址，如果没有VIAVA分区则返回0 
*/
rs_u32 rs_sys_getVIVAStartAddr();


#endif