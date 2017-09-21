#include "rs_datatype.h"
#include "rs_flash_operate.h"
#include "rs_dev.h"
#include "rs_fs.h"
#include "rs_error.h"
#include "rs_checksum.h"
#include "rs_mem.h"
#include "rs_std_fun.h"
#include "rs_param.h"
#include "rs_dev.h"
#include "rs_debug.h"

// MTK header
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "ftl.h"



/***************************函数声明*****************************************/
extern FTL_FuncTbl ftlFuncTbl;
FTL_FuncTbl *g_ftlFuncTbl = &ftlFuncTbl;

/***************************局部函数*****************************************/

/**
 * transfer physical address to block/page
 *
 * @param
 * @param[addr]		: physical address to transfer
 * @param[vBlock]	: data ptr of transferred block index
 * @param[vPage]	: data ptr of transferred page index
 *
 * @remarks
 *
 * @return
 * rs_true	: transfer successfully
 * rs_false	: transfer failed
 */
static rs_bool rs_ua_AddrToBlockPage(rs_u32 addr, rs_u32 *vBlock, rs_u32 *vPage)
{
	rs_u32 status;

	status = g_ftlFuncTbl->FTL_AddrToBlockPage(addr, vBlock, vPage, NULL);
	if (status != FTL_SUCCESS) {
		RS_PORITNG_LOG("%s, transfer address[%x] to block/page failed, quit\n\r", __func__, addr);
		return RS_FALSE;
	}

	return RS_TRUE;
}

/***************************flash函数实现************************************/
/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_init()
{
	FTL_STATUS_CODE status;
	
	status = g_ftlFuncTbl->FTL_Init(NULL);
	if(status != FTL_SUCCESS) {
		RS_PORITNG_LOG("FTL Init failed, quit update\n");
		return RS_ERR_FLASH_INIT;
	}
	
  return RS_ERR_OK;
}

/**
函数说明：获取当前flash的一个page的大小 
参数说明：无
返回值：成功返回page的size，失败返回-1 
*/
rs_s32 rs_flash_getPageSize()
{
	return g_ftlFuncTbl->FTL_GetPageSize();
}

/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_getBlockSize()
{
	return g_ftlFuncTbl->FTL_GetBlockSize(0, NULL);
}

#if (!defined(RS_SUPPORT_UPDATE_INFO_FS) || !defined(RS_SUPPORT_UPDATE_DATA_FS))

/**
函数说明：擦除block 
参数说明：addr需要擦除的block的地址
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_eraseBlock(rs_u32 addr)
{
	rs_u32 vBlock;
	rs_u32 vPage;
	rs_u32 vAddress = addr;

	// check block aligned or not
	if (vAddress & (BLOCK_SIZE - 1)) {
		RS_PORITNG_LOG("%s, failed causion:address is not aligned by Block, block size is %d, quit\n", __func__, BLOCK_SIZE);
		return RS_ERR_INVALID_PARAM;
	}
	
	// address to Block/Page
	if (!rs_ua_AddrToBlockPage(vAddress, &vBlock, &vPage)) {
		return RS_ERR_INVALID_PARAM;
	}

	// check erase block - page
	if (vPage != 0) {
		RS_PORITNG_LOG("%s, failed causion:we must erase an entire block!!!\n\r", __func__);
		return RS_ERR_INVALID_PARAM;
	}

	// erase
	if (g_ftlFuncTbl->FTL_EraseBlock(vBlock, NULL) != FTL_SUCCESS){
		RS_PORITNG_LOG("%s, failed causion:erase error at block[%d] page[%d] addr[%d], quit\n\r", __func__, vBlock, vPage, vAddress);
		return RS_ERR_FLASH_WRITE;
	}

	return RS_ERR_OK;
}




/**
函数说明：按照page读取数据 
参数说明：addr 需要读取的地址
		 buffer 存放要读取数据的缓冲区
		 len 需要写入buffer数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readPage(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{
	rs_u32 vBlock;
	rs_u32 vPage;
	rs_u32 vAddress = addr;
	rs_u8 pageBuf[PAGE_SIZE];
	
	if (buffer == RS_NULL || len > rs_flash_getPageSize())
	{
		RS_PORITNG_LOG("%s, failed causion: buffer == null or len != 512\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}
	
		// check page aligned or not
	if (vAddress & (PAGE_SIZE - 1)) {
		RS_PORITNG_LOG("%s, failed causion:address is not aligned by page[512], quit\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}
	
	if (!rs_ua_AddrToBlockPage(vAddress, &vBlock, &vPage)) {
		RS_PORITNG_LOG("%s No-aligned route, failed causion:transfer address to block/page failed, quit\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}

	if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)pageBuf, NULL) != FTL_SUCCESS) {
		RS_PORITNG_LOG("%s, Section A failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
		return RS_ERR_FLASH_READ;
	}
	
	rs_memcpy(buffer, pageBuf, len);
		
	return RS_ERR_OK;
}


/**
函数说明：按照page写入数据 
参数说明：addr 需要写入的地址
		 Buffer 需要写入的buffer数据
		 Len 需要写入的数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_writePage(rs_u32 addr, const rs_u8* buffer, rs_s32 len)
{
	rs_u32 vBlock;
	rs_u32 vPage;
	rs_u32 vAddress = addr;
	rs_u8 pageBuf[PAGE_SIZE];
	
	if (buffer == RS_NULL || len > rs_flash_getPageSize())
	{
		RS_PORITNG_LOG("%s, failed causion: buffer == null or len != 512\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}
	
		// check page aligned or not
	if (vAddress & (PAGE_SIZE - 1)) {
		RS_PORITNG_LOG("%s, failed causion:address is not aligned by page[512], quit\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}
	
	
	rs_memset(pageBuf, 0xff, PAGE_SIZE);
	rs_memcpy(pageBuf, buffer, len);
	
	if (!rs_ua_AddrToBlockPage(vAddress, &vBlock, &vPage)) {
		RS_PORITNG_LOG("%s No-aligned route, failed causion:transfer address to block/page failed, quit\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}

	if (g_ftlFuncTbl->FTL_WritePage(vBlock, vPage, (rs_u32 *)pageBuf, NULL) != FTL_SUCCESS) {
		RS_PORITNG_LOG("%s, Section A failed causion:write error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
		return RS_ERR_FLASH_READ;
	}
		
	return RS_ERR_OK;
}




/**
函数说明：按照block读取数据 
参数说明：addr 需要读取的地址,block对齐
		 Buffer 需要读取的buffer数据
		 Len 需要读取的数据的长度，必须为小于block的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readLessBlock(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{
	rs_u32 vBlock, vPage, j, vSizeCount, vSizeRemain, vPageSize;
	rs_u32 vAddress = addr;
	rs_u8 *p = buffer;
	rs_u8 data[PAGE_SIZE] = {0};

	if (buffer == RS_NULL)
	{
		RS_PORITNG_LOG("%s, failed causion: buffer == null\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}

	if ((vAddress & (BLOCK_SIZE - 1)) != 0)//
	{
		RS_PORITNG_LOG("%s failed causion: address is not an aligned by BLOCK address\n", __func__);
		return RS_ERR_INVALID_PARAM;
	}
	
	if (len < 0 || len > rs_flash_getBlockSize())
	{
		RS_PORITNG_LOG("%s failed causion: need read len = %d\n", __func__, len);
		return RS_ERR_INVALID_PARAM;		
	}

	if (!rs_ua_AddrToBlockPage(vAddress, &vBlock, &vPage))
	{
		RS_PORITNG_LOG("%s No-aligned route, failed causion:transfer address to block/page failed, quit\n\r", __func__);
		return RS_ERR_INVALID_PARAM;
	}

	// caculate size count/remain
	vPageSize = rs_flash_getPageSize();
	
	if (vPageSize != PAGE_SIZE)
	{
		RS_PORITNG_LOG("%s, vPageSize != PAGE_SIZE\n", __func__);
		return RS_ERR_INVALID_PARAM;		
	}
	
	vSizeCount = (len / vPageSize);
	vSizeRemain = (len % vPageSize);

	for (j = 0 ; j < vSizeCount; j++)
	{
		// read data
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			RS_PORITNG_LOG("%s, failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			
			return RS_ERR_FLASH_READ;
		}

		rs_memcpy(p, data, vPageSize);
		
		// next page
		vPage++;
		p += vPageSize;

		rs_memset(data, PAGE_SIZE, 0);
	}

	if (vSizeRemain != 0)
	{
		// read data
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			RS_PORITNG_LOG("%s, failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);;
			return RS_ERR_FLASH_READ;
		}
		
		// copy remain data to destination ptr
		rs_memcpy(p, data, vSizeRemain);
	}
	
	return RS_ERR_OK;
}

/*************************************扩展接口*************************************/

/**
函数说明：把数据和这段数据对应的checkSum写入到某个block中
参数说明：addr 需要写入的地址,block对齐
		buffer 需要写入的数据的缓冲区
		len buffer数据的缓冲区
		checkSum 这段数据的校验值
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_writeDataWithCSToBlock(rs_u32 addr, const rs_u8* buffer, rs_s32 len, rs_u32 checkSum)
{
	rs_u8 page_buf[PAGE_SIZE] = {0};
	rs_s32 writeLen=0;
	rs_u32 w_addr = addr;
	rs_s32 needWriteLen = len;

	if(buffer == RS_NULL || len == 0)
	{
		return RS_ERR_WRITE_DATA_FAILE;
	}

	rs_flash_eraseBlock(addr);
	while(needWriteLen >= PAGE_SIZE)
	{
		if(rs_flash_writePage(w_addr, buffer+writeLen, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
		w_addr += PAGE_SIZE;
		writeLen += PAGE_SIZE;
		needWriteLen -= PAGE_SIZE;
	}

	if (needWriteLen + sizeof(rs_u32) > PAGE_SIZE)
	{
		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, PAGE_SIZE-needWriteLen);
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}

		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, &checkSum + (PAGE_SIZE-needWriteLen), sizeof(rs_u32) - (PAGE_SIZE-needWriteLen));
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}
	else
	{
		rs_memset(page_buf, 0xFF, PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, sizeof(rs_u32));
		if(rs_flash_writePage(w_addr, page_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}

	return RS_ERR_OK;
}

/**
函数说明：从指定地址读取出来unsigned long型的整型值
参数说明：addr 需要读取的地址
返回值：返回读出的整型值
*/
rs_u32 rs_flash_readU32(rs_u32 addr)
{
	rs_u8 read_buf[PAGE_SIZE];
	rs_s32 page_offset = 0;
	rs_u32 checkSum = 0;
	rs_bool readTwoPage = RS_FALSE;
	rs_u32 blockAddr = (addr / BLOCK_SIZE) * BLOCK_SIZE;
	rs_u32 offset = addr - blockAddr;

	if ((offset/PAGE_SIZE) != ((offset + sizeof(rs_u32) - 1)/PAGE_SIZE))
	{
		readTwoPage = RS_TRUE;
	}

	if (readTwoPage == RS_TRUE)
	{
		rs_s32 alreadyRead = 0;
		rs_u8 checkSumBuf[10] = {0};
		rs_u8* p = checkSumBuf;

		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset = (offset/PAGE_SIZE)*PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		alreadyRead = PAGE_SIZE - (offset%PAGE_SIZE);
		rs_memcpy(p, read_buf+(offset%PAGE_SIZE), alreadyRead);

		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset += PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(p + alreadyRead, read_buf, sizeof(rs_u32) - alreadyRead);

		rs_memcpy(&checkSum, checkSumBuf, 4);
	}
	else
	{
		rs_memset(read_buf, 0, PAGE_SIZE);
		page_offset = (offset/PAGE_SIZE)*PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(&checkSum, read_buf+(offset%PAGE_SIZE), sizeof(rs_u32));
	}
	
	return checkSum;
}

/*
函数说明：根据地址及长度，计算该段数据的checksum值
参数说明：addr 起始地址，page对齐
			len 数据长度
返回值：失败为-1，成功为校验值
*/
rs_u32 rs_flash_calculatCSByAddr(rs_u32 addr, rs_s32 len)
{
	rs_u32 checksum = 0;
	rs_u8 buff[PAGE_SIZE] = {0};
	rs_s32 pagecount;
	rs_s32 lessPageSize = len & (PAGE_SIZE - 1); //非page对齐长度数据大小
	rs_s32 i;

	if((addr & (PAGE_SIZE - 1)) != 0)
	{
		return -1;
	}

 	if (rs_open_checksum() != 1)
	{
		return -1;
	}
	pagecount = len/PAGE_SIZE ;
	if(lessPageSize > 0)
	{
		pagecount++;
	}

	for(i=0; i<pagecount; i++)
	{
		if(rs_flash_readPage(addr + i*PAGE_SIZE, buff, PAGE_SIZE) != RS_ERR_OK)
		{
			return -1;
		}

		if(i + 1 == pagecount)
		{
			if (lessPageSize != 0)
			{
				checksum = rs_get_checksum(checksum, buff, lessPageSize);
			}
			else
			{
				checksum = rs_get_checksum(checksum, buff, PAGE_SIZE);
			}
		}
		else
		{
			checksum = rs_get_checksum(checksum, buff, PAGE_SIZE);
		}
	}
	rs_close_checksum();

	return checksum;
}
#endif