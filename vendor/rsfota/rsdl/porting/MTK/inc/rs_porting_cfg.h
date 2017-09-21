#ifndef _RS_PORTING_CFG_H_
#define _RS_PORTING_CFG_H_

/*
 * RS_SUPPORT_UPDATE_INFO_FS
 *
 * 下载过程中是否支持把下载信息记录于文件系统
 * 在不支持的情况下,升级信息会存储于flash中，那么此时也表示能够支持flash的操作
 */
//#define RS_SUPPORT_UPDATE_INFO_FS

/*
 * RS_SUPPORT_UPDATE_DATA_FS
 *
 * 下载过程中是否支持把升级包存储于文件系统中
 * 如果bootloader阶段或者recovery模式能够识别文件系统，那么应该支持此选项
 * 在不支持的情况下,升级包会存储于flash中，那么此时也表示能够支持flash的操作
 * 
 * 备注：如果能够支持RS_SUPPORT_UPDATE_DATA_FS，那么RS_SUPPORT_UPDATE_INFO_FS是一定能够支持。
 * 如果支持RS_SUPPORT_UPDATE_INFO_FS，但是RS_SUPPORT_UPDATE_INFO_FS不一定可以支持，MTK6216就是一个例子， 
 * 虽然有文件系统，但是bootloader不支持。
 */
//#define RS_SUPPORT_UPDATE_DATA_FS

#endif