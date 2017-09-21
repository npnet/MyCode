#ifndef __RSUAUPDATE_H__
#define __RSUAUPDATE_H__
#include "rsplatform.h"
#define PROGRESS_EXTEND_PRE  (20)
#define PROGRESS_NORMAL_PRE (100- PROGRESS_EXTEND_PRE)
/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_bool rs_ua_flash_write_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_bool rs_ua_flash_read_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getPageSize();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_bool rs_ua_flash_erase_ext(rs_u32 vBlock);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getBlockSize();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_ram_block_size();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getStartAddr(rs_u32 addr);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_trace(const char *aFormat,...);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_ram_block();

/*
* ƽ̨�ӿڣ�ƽ̨����
*/
rs_bool rs_ua_flash_init(void);

/*
* rsua����ӿڣ�ƽ̨����
*/
rs_s32 rs_ua_initUpdateInfo();
/*
* rsua����ӿڣ�ƽ̨����
*/
rs_s32 rs_ua_update(void);

/*
* ƽ̨�ӿڣ�rsua�������
*/
void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_sdk_heap_size();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_sdk_heap_addr();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_ua_get_backupBlock(rs_u32 *blockStart, rs_u32 *blockSize);

void rs_save_log(rs_u8* msg,rs_u32 msgLen);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getPackageAddr();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getUpdateInfo1Addr();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getUpdateInfo2Addr();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u8 rs_ua_getMainUpdateInfoFile();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u8 rs_ua_getSubUpdateInfoFile();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u8 rs_ua_getOTAPackageFile();
rs_u8 rs_ua_getExtendFile();
/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_ua_openFileOnly(rs_u8 num);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_ua_openFileCreate(rs_u8 num);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_ua_openFileRW(rs_u8 num);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_removeFile(rs_u8 num);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_fs_exists(const char* fileName);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_fs_getFileSize(rs_s32 handle,rs_s32* size);

/*
* ƽ̨�ӿڣ�rsua�������
*/
void rs_fs_close(rs_s32 handle);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_fs_read(rs_s32 handle,void *buffer, rs_u32 len, rs_u32 *read);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_fs_write(rs_s32 handle, const void* buffer, int size, rs_u32 *written);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_s32 rs_fs_seek(rs_s32 handle,rs_u32 offset, rs_s32 origin);

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u8 rs_ua_packageStore();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_determineProject();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getBlankSize();

/*
* ƽ̨�ӿڣ�rsua�������
*/
rs_u32 rs_ua_getBackUpAddr();

rs_bool rs_ua_package_read_metafile(rs_u8 *buffer, rs_u32 size);
rs_u32 rs_ua_package_getLeftDeltaLen();
rs_u32 rs_ua_processExtendFile();
void rs_ua_cleanExtendFile();
rs_u32 rs_ua_saveExtendDataToFile(rs_u8* buff,rs_u32 size, rs_u32 offset,rs_bool isFinal);
rs_bool rs_ua_writeDataToFile(rs_u8* buffer, rs_s32 bufferSize, rs_u8 destFile);
#endif

