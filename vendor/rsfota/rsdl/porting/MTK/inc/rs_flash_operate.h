#ifndef __RSFLASHOPERATE_H__
#define __RSFLASHOPERATE_H__


#ifdef __cplusplus
  extern "C"
  {
#endif

#include "rs_porting_cfg.h"

#include "rs_datatype.h"


/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_init();

/**
函数说明：获取当前flash的一个page的大小 
参数说明：无
返回值：成功返回page的size，失败返回-1 
*/
rs_s32 rs_flash_getPageSize();

/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_getBlockSize();

/**
函数说明：擦除block 
参数说明：addr需要擦除的block的地址
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_eraseBlock(rs_u32 addr);

/**
函数说明：按照page读取数据 
参数说明：addr 需要读取的地址
		 buffer 存放要读取数据的缓冲区
		 len 需要写入buffer数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readPage(rs_u32 addr, rs_u8* buffer, rs_s32 len);


/**
函数说明：按照page写入数据 
参数说明：addr 需要写入的地址
		 Buffer 需要写入的buffer数据
		 Len 需要写入的数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_writePage(rs_u32 addr, const rs_u8* buffer, rs_s32 len);

/**
函数说明：按照block读取数据 
参数说明：addr 需要读取的地址,block对齐
		 Buffer 需要读取的buffer数据
		 Len 需要读取的数据的长度，必须为小于block的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readLessBlock(rs_u32 addr, rs_u8* buffer, rs_s32 len);

/**
函数说明：把数据和这段数据对应的checkSum写入到某个block中
参数说明：addr 需要写入的地址,block对齐
		buffer 需要写入的数据的缓冲区
		len buffer数据的缓冲区
		checkSum 这段数据的校验值
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_writeDataWithCSToBlock(rs_u32 addr, const rs_u8* buffer, rs_s32 len, rs_u32 checkSum);

/**
函数说明：从指定地址读取出来unsigned long型的整型值
参数说明：addr 需要读取的地址
返回值：返回读出的整型值
*/
rs_u32 rs_flash_readU32(rs_u32 addr);

/*
函数说明：根据地址及长度，计算该段数据的checksum值
参数说明：addr 起始地址，page对齐
			len 数据长度
返回值：失败为-1，成功为校验值
*/
rs_u32 rs_flash_calculatCSByAddr(rs_u32 addr, rs_s32 len);

#ifdef __cplusplus
  }
#endif

#endif
