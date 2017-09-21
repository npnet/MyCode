/*
 *
 * ��Ȩ(c) 2015 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2015 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */


#include "fs_gprot.h"
#include "kal_general_types.h"
#include <stdio.h>
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"

#include "rs_extbl_fs.h"
#include "rs_extbl_OEM.h"

extern rs_u32 rs_trace(const char *aFormat,...);

rs_s32 rs_fs_open_only(const char* fileName)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	return FS_Open((WCHAR*)fileName, FS_READ_ONLY);
#else
	return -1;
#endif
}

rs_s32 rs_fs_open_create(const char* fileName)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	return FS_Open((WCHAR*)fileName, FS_CREATE_ALWAYS | FS_READ_WRITE);
#else
	return -1;
#endif
}

rs_s32 rs_fs_open_rw(const char* fileName)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	return FS_Open((WCHAR*)fileName, FS_READ_WRITE);
#else
	return -1;
#endif
}

void rs_fs_close(rs_s32 handle)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	FS_Close(handle);
#else
	return ;
#endif
}

rs_u32 rs_fs_read(rs_s32 handle,void *buffer, rs_u32 len, rs_u32 *read)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	rs_s32  status;

	status = FS_Read(handle, buffer, len, read);
	//Check the result of FS API, and convert it to BL error code
	if(status >= 0)
	{
		return *read;
	}
	else
	{
		rs_trace("%s failed!\n\r", __func__);
		return -1;
   }
#else
	return -1;
#endif
}

rs_s32 rs_fs_write(rs_s32 handle, const void* buffer, int size, rs_u32 *written)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	int fs_status;

	fs_status = FS_Write(handle, (void*)buffer, size, written);
	if (fs_status != FS_NO_ERROR || *written != size) {
		rs_trace("%s, write error: status[%x], size[%d], written[%d]\n\r", __func__, fs_status, size, *written);
	}
	return *written;
#else
	return -1;
#endif
}

rs_s32 rs_fs_seek(rs_s32 handle,rs_u32 offset, rs_s32 origin)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	rs_u32 offset_p;

	ASSERT(origin == 0);
	offset_p =  FS_Seek(handle, offset, 0);
	return offset_p;
#else
	return -1;
#endif
}

rs_s32 rs_fs_getFileSize(rs_s32 handle,rs_s32* size)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	FS_GetFileSize(handle,size);
	return *size;
#else
	return -1;
#endif
}

rs_u32 rs_fs_exists(const char* fileName)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	rs_s32 f;
	f = rs_fs_open_only(fileName);
	if (f != RS_FS_INVALID)
	{
		// it existes
		rs_fs_close(f);
		rs_trace("%s, file[%s] is exist\n\r", __func__, fileName);
		return 1;
	}
	else
	{
		// not exist
		rs_trace("%s, file[%s] is not exist\n\r", __func__, fileName);
		return 0;
	}
#else
	return -1;
#endif
}

rs_u32 rs_fs_remove(const char* fileName)
{
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
	int fs_status;

	fs_status = FS_Delete((WCHAR*)fileName);
	if (fs_status < FS_NO_ERROR) {
		rs_trace("%s, remove error: status[%x]\n\r", __func__, fs_status);
		return 0;
	}
	return 1;
#else
	return 0;
#endif
}
