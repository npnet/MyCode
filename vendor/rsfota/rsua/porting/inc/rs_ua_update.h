#ifndef __RSUAUPDATE_H__
#define __RSUAUPDATE_H__
#include "rsplatform.h"
#define PROGRESS_EXTEND_PRE  (20)
#define PROGRESS_NORMAL_PRE (100- PROGRESS_EXTEND_PRE)
/*
* 平台接口，rsua库里调用
*/
rs_bool rs_ua_flash_write_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

/*
* 平台接口，rsua库里调用
*/
rs_bool rs_ua_flash_read_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getPageSize();

/*
* 平台接口，rsua库里调用
*/
rs_bool rs_ua_flash_erase_ext(rs_u32 vBlock);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getBlockSize();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_ram_block_size();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getStartAddr(rs_u32 addr);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_trace(const char *aFormat,...);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_ram_block();

/*
* 平台接口，平台调用
*/
rs_bool rs_ua_flash_init(void);

/*
* rsua库里接口，平台调用
*/
rs_s32 rs_ua_initUpdateInfo();
/*
* rsua库里接口，平台调用
*/
rs_s32 rs_ua_update(void);

/*
* 平台接口，rsua库里调用
*/
void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_sdk_heap_size();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_sdk_heap_addr();

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_ua_get_backupBlock(rs_u32 *blockStart, rs_u32 *blockSize);

void rs_save_log(rs_u8* msg,rs_u32 msgLen);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getPackageAddr();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getUpdateInfo1Addr();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getUpdateInfo2Addr();

/*
* 平台接口，rsua库里调用
*/
rs_u8 rs_ua_getMainUpdateInfoFile();

/*
* 平台接口，rsua库里调用
*/
rs_u8 rs_ua_getSubUpdateInfoFile();

/*
* 平台接口，rsua库里调用
*/
rs_u8 rs_ua_getOTAPackageFile();
rs_u8 rs_ua_getExtendFile();
/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_ua_openFileOnly(rs_u8 num);

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_ua_openFileCreate(rs_u8 num);

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_ua_openFileRW(rs_u8 num);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_removeFile(rs_u8 num);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_fs_exists(const char* fileName);

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_fs_getFileSize(rs_s32 handle,rs_s32* size);

/*
* 平台接口，rsua库里调用
*/
void rs_fs_close(rs_s32 handle);

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_fs_read(rs_s32 handle,void *buffer, rs_u32 len, rs_u32 *read);

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_fs_write(rs_s32 handle, const void* buffer, int size, rs_u32 *written);

/*
* 平台接口，rsua库里调用
*/
rs_s32 rs_fs_seek(rs_s32 handle,rs_u32 offset, rs_s32 origin);

/*
* 平台接口，rsua库里调用
*/
rs_u8 rs_ua_packageStore();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_determineProject();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getBlankSize();

/*
* 平台接口，rsua库里调用
*/
rs_u32 rs_ua_getBackUpAddr();

rs_bool rs_ua_package_read_metafile(rs_u8 *buffer, rs_u32 size);
rs_u32 rs_ua_package_getLeftDeltaLen();
rs_u32 rs_ua_processExtendFile();
void rs_ua_cleanExtendFile();
rs_u32 rs_ua_saveExtendDataToFile(rs_u8* buff,rs_u32 size, rs_u32 offset,rs_bool isFinal);
rs_bool rs_ua_writeDataToFile(rs_u8* buffer, rs_s32 bufferSize, rs_u8 destFile);
#endif

