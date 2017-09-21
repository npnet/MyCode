#include "ftl.h"
#include "fs_gprot.h"
#include <string.h>
#include <bl_types.h>
#include <bl_init.h>
#include <bl_common.h>
#include "wdt_hw.h"
#include "dcl.h"
#include <stdarg.h>

#include "rsplatform.h"
#include "rs_ua_update.h"
#include "rs_ua_display.h"
#include "rs_extbl_fs.h"
#include "rs_extbl_flash.h"
#include "rs_extbl_OEM.h"

#if defined(__GPS_FW_UPDATE_SUPPORT__)
#include "GPS_DL_api.h"
#endif

//ram buffer alloced start
#if defined(JIAWEI)
#pragma arm section rwdata = "EXT_BL_IOT_RS_FOTA_WORKING_BUF", zidata = "EXT_BL_IOT_RS_FOTA_WORKING_BUF"
rs_u8 rs_ram_buffer[RS_FOTA_RAM_BUFFER_SIZE];
rs_u8 block_data[RS_FOTA_BLOCK_BUFFER_SIZE];
#endif
#pragma arm section rwdata, zidata

#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT) || defined(CORE_TEK) || defined(NBBIRD)
extern rs_u32 Image$$EXT_BL_IOT_RS_FOTA_WORKING_BUF$$ZI$$Base;
rs_u8 * rs_ram_buffer = 0;
rs_u8 * block_data = 0;
#endif
//ram buffer end

// WatchDog macro
#define WDT_DISABLE_CMD             0x2200
#define WDT_RESTART_CMD             0x1971
#define WDT_SWRST_CMD               0x1209
extern void SetWDTNormalResetFlag(void);
void WatchDogResetAll(void);
extern rs_s32 rs_ua_update(void);
extern void WacthDogRestart(void);
/********************************************************************************/
/**
 * rs_ua_sdk_heap_size
 *
 *
 */
rs_u32 rs_ua_sdk_heap_size()
{
  return RS_FOTA_RAM_BUFFER_SIZE;
}

/**
 * rs_ua_postProgress
 *
 *
 */
void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total)
{

	rs_u32 present = current;
	if(total == PROGRESS_EXTEND_PRE)
	{
		present = (rs_u32)(PROGRESS_NORMAL_PRE + (current* total/100));
	}
	else if(total == PROGRESS_NORMAL_PRE)
	{
		present = (rs_u32)(current* total/100);
	}
	else 
	{
		
	}
	rs_trace("rsfota updating...,show progress: %d%% (step %d of 100)..\r\n",present , present);
	#ifdef __RS_FOTA_EXTBL_DISPLAY__
	rs_ua_showProgress(0, present);
	#endif
}

/**
 * rs_ua_kick_watchdog
 * 
 */
void rs_ua_kick_watchdog(void)
{
	//*(volatile rs_u32 *)(WDT_MODE) = WDT_DISABLE_CMD;
	WacthDogRestart();
}

rs_u32 rs_ua_sdk_heap_addr()
{
#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT) || defined(CORE_TEK) || defined(NBBIRD)
	block_data = (rs_s8 *)(&Image$$EXT_BL_IOT_RS_FOTA_WORKING_BUF$$ZI$$Base);
	rs_ram_buffer = block_data + RS_FOTA_BLOCK_BUFFER_SIZE;
#endif
	return (rs_u32 )rs_ram_buffer;
}

rs_u32 rs_ua_ram_block()
{
#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT) || defined(CORE_TEK) || defined(NBBIRD)
	block_data = (rs_s8 *)(&Image$$EXT_BL_IOT_RS_FOTA_WORKING_BUF$$ZI$$Base);
#endif
	return (rs_u32 )block_data;
}

rs_u32 rs_ua_ram_block_size()
{
	return RS_FOTA_BLOCK_BUFFER_SIZE;
}

rs_u32 rs_ua_getStartAddr(rs_u32 addr)
{
	return FOTA_UPGRADE_START;
}

rs_u32 rs_ua_getBlankSize()
{
	return BLANK_SIZE;
}

//#ifdef RS_DIFF_PACKAGE_ON_FLASH
/**
 * rs_ua_getPackageAddr
 * return address of fota diff package on flash
 */
rs_u32 rs_ua_getPackageAddr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return RS_FOTA_ADDRESS;
#else
	return 0;
#endif
}

/**
 * rs_ua_getUpdateInfo1Addr
 * return address of storing fota update status info, main block
 */
rs_u32 rs_ua_getUpdateInfo1Addr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return FOTA_UPDATE_INFO_ADDR;
#else
	return 0;
#endif
}

/**
 * rs_ua_getUpdateInfo2Addr
 * return address of storing fota update status info, second block
 */
rs_u32 rs_ua_getUpdateInfo2Addr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return FOTA_BACKUP_UPDATE_INFO_ADDR;
#else
	return 0;
#endif
}
//#else
rs_u8 rs_ua_getMainUpdateInfoFile()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return MAIN_UPDATE_FILE;
#endif
}

rs_u8 rs_ua_getSubUpdateInfoFile()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return SUB_UPDATE_FILE;
#endif
}
rs_u8 rs_ua_getExtendFile()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return EXTEND_FILE_INDEX;
#endif
}
rs_u8 rs_ua_getOTAPackageFile()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return OTA_PACKAGE_FILE;
#endif
}

rs_s32 rs_ua_openFileOnly(rs_u8 num)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (num == 1)
		return rs_fs_open_only((char *)UPINFO_A);
	else if (num == 2)
		return rs_fs_open_only((char *)UPINFO_B);
	else if (num == 3)
		return rs_fs_open_only((char *)RSDELTA_PATH);
	else if(num == 4)
		return rs_fs_open_only((char *)EXTEND_FILE);
#endif
	return -1;
}
void rs_ua_cleanExtendFile()
{
	
	//rs_fs_remove((char *)EXTEND_FILE);
	
}

rs_u32 rs_ua_saveExtendDataToFile(rs_u8* buff,rs_u32 size, rs_u32 offset,rs_bool isFinal)
{
	rs_u8* externDataBuff = (rs_u8*)(rs_ua_sdk_heap_addr() + rs_ua_sdk_heap_size());

	memcpy(externDataBuff + offset, buff, size);

	if (isFinal == rs_true)
	{
		if(rs_ua_writeDataToFile(externDataBuff, offset + size, 4) != rs_true)
		{
			return -1;
		}
	}

	return 0;
}

rs_s32 rs_ua_openFileCreate(rs_u8 num)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (num == 1)
		return rs_fs_open_create((char *)UPINFO_A);
	else if (num == 2)
		return rs_fs_open_create((char *)UPINFO_B);
	else if (num == 3)
		return rs_fs_open_create((char *)RSDELTA_PATH);
	else if(num == 4)
	{
		/*
		if(rs_fs_exists((char *)EXTEND_FILE) == 1)
		{
			rs_fs_remove((char *)EXTEND_FILE);
		}
		*/	
		return rs_fs_open_create((char *)EXTEND_FILE);
	}
#endif
	return -1;
}

rs_s32 rs_ua_openFileRW(rs_u8 num)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (num == 1)
		return rs_fs_open_rw((char *)UPINFO_A);
	else if (num == 2)
		return rs_fs_open_rw((char *)UPINFO_B);
	else if (num == 3)
		return rs_fs_open_rw((char *)RSDELTA_PATH);
	else if(num == 4)
		return rs_fs_open_rw((char *)EXTEND_FILE);
#endif
	return -1;
}

rs_u32 rs_ua_removeFile(rs_u8 num)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (num == 1)
		return rs_fs_remove((char *)UPINFO_A);
	else if (num == 2)
		return rs_fs_remove((char *)UPINFO_B);
	else if (num == 3)
		return rs_fs_remove((char *)RSDELTA_PATH);
#endif
	return -1;
}

rs_s32 rs_ua_getFileSize(rs_u8 num)
{
	rs_s32 fileHandle;
	rs_s32 filesize,fs_status;
	fileHandle = rs_ua_openFileOnly(num);
	
	if (fileHandle < 0)
		return 0;
	fs_status = rs_fs_getFileSize(fileHandle, &filesize);
	if (fs_status < 0)
	{
		rs_trace("%s, get file size error: status[%x]\n\r", __func__, fs_status);
		rs_fs_close(fileHandle);
		return 0;
	}
	rs_fs_close(fileHandle);
	return filesize;
}
//#endif

rs_u32 rs_ua_getBackUpAddr()
{
	return PARITY_ADDR;
}

rs_s32 rs_ua_get_backupBlock(rs_u32 *blockStart, rs_u32 *blockSize)
{
    *blockStart = (rs_u32)PARITY_ADDR;
    *blockSize = (rs_u32)rs_ua_getBlockSize();
    return 0;
}

/**
 * rs_ua_packageStore
 * return value for SDK using
 */
rs_u8 rs_ua_packageStore()
{
#if defined(RS_DIFF_PACKAGE_ON_FLASH)
	return 1;
#else
	return 0;
#endif
}

/**
 * rs_ua_determineProject
 * return value for SDK using to handle blank size
 */
rs_u32 rs_ua_determineProject()
{
#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT)
	return 0;
#elif defined(JIAWEI)
	return 1;
#endif
}

rs_u32 rs_trace(const char *aFormat,...)
{
   va_list ap;
   va_start (ap, aFormat);

   bl_print_internal(LOG_INFO, aFormat, ap);

   va_end (ap);

   bl_print(LOG_INFO, "\r"); 
   
   return 0;
}

int rs_ua_execute(void)
{
	rs_s32 ret = 0;

	//WacthDogDisable();

#ifndef RS_DIFF_PACKAGE_ON_FLASH
	ret = rs_FS_SanityCheck();//FS_SanityCheck();
	rs_trace("%s, FS_SanityCheck ret = %d,\n\r", __func__, ret);
#endif

	if (!rs_ua_flash_init()) {
		rs_trace("%s, FTL Init failed\n\r", __func__);
		return 1;
	}
	rs_trace("%s, flash init complete!\r\n", __func__);

	if(rs_ua_initUpdateInfo() !=0)
	{
		rs_trace("%s, init update info return value != 0\r\n");
		return 2;
	}

#ifndef RS_DIFF_PACKAGE_ON_FLASH
	//if(rs_fs_exists((char *)EXTEND_FILE) == 1)
	{
		rs_fs_remove((char *)EXTEND_FILE);
		rs_trace("%s, deltet extend file\n\r", __func__);
	}
#endif
	
	rs_trace("%s, init update info finish.. \r\n", __func__);
	WacthDogDisable();
	ret = rs_ua_update();
	rs_trace("%s, rs_ua_update return %d\r\n", __func__, ret);
	if (ret == 0)
	{
		#ifdef __RS_FOTA_EXTBL_DISPLAY__
		rs_ua_showSuccess();//update success, display
		#endif
		SetWDTNormalResetFlag();
		WatchDogResetAll();   
	}
	else
	{
		#ifdef __RS_FOTA_EXTBL_DISPLAY__
		rs_ua_showError();//go to normal reboot, otherwise under not-good ota-package, circle reboot forever
						//because of updating flag is valid.
		#endif
		SetWDTNormalResetFlag();
		WatchDogResetAll();
	}

	return ret;
}
