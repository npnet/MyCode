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


#include <stdio.h>
#include "rs_fs.h"
#include "rs_error.h"
#include "rs_debug.h"
#include "rs_std_fun.h"

// MTK header
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "kal_public_api.h"
#include "Mmiapi_struct.h"
#include "mmi_msg_struct.h"
#include "Mmi_frm_queue_gprot.h"
#include "Mmi_frm_events_gprot.h"
#include "Mmi_cb_mgr_gprot.h"
#include "TimerEvents.h"

#define MAX_FILE_PATH	256

/*
����˵�������ļ�
����˵����fileName  �ļ���·��
		  openMode  �򿪵�ģʽ���μ�rs_fs_openMode
����ֵ���ɹ������ļ������ʧ�ܷ���RS_FS_INVALID
*/
RS_FILE rs_fs_open(const rs_s8* fileName, rs_fs_openMode openMode)
{
	RS_FILE fh = RS_FS_INVALID;
	int lpOpenMode;
	FS_HANDLE fd = -1;
	WCHAR wfilePath[MAX_FILE_PATH];

	if(fileName == RS_NULL || rs_strlen(fileName) > (MAX_FILE_PATH - 1))
	{
		return RS_FS_INVALID;
	}

	if(openMode == RS_FS_OPEN_CREATE)
	{
		lpOpenMode = FS_CREATE_ALWAYS | FS_READ_WRITE;
	}
	else if(openMode == RS_FS_OPEN_READ)
	{
		lpOpenMode = FS_READ_ONLY;
	}
	else if(openMode == RS_FS_OPEN_WRITE)
	{
		lpOpenMode = FS_READ_WRITE;
	}

	rs_memset(wfilePath, 0, sizeof(wfilePath));
	kal_wsprintf(wfilePath, "%s", fileName);

	fd = (FS_Open(wfilePath, lpOpenMode));
	if (fd < FS_NO_ERROR)
	{
		RS_PORITNG_LOG("open file fail, fileName = %s, rs open mode = %d, current os mode=0x%x\n\r", fileName, openMode, lpOpenMode);
		fd = RS_FS_INVALID;
	}
	else
	{
		RS_PORITNG_LOG("open file success, fileName = %s, rs open mode = %d, current os mode=0x%x\n\r", fileName, openMode, lpOpenMode);
	}
	fh = (RS_FILE)fd;

	return fh;
}

/*
����˵�����ر��ļ�
����˵����fh  �ļ��򿪺�ľ��
����ֵ���ɹ�����0��ʧ��Ϊ-1
*/
rs_s32 rs_fs_close(RS_FILE fh)
{
	FS_Close((FS_HANDLE)fh);
	return 0;
}

/*
����˵���������ݶ�ȡ����������
����˵����fh  �ļ��򿪺�ľ��
		  buffer  ��Ҫ��������ݵĻ�����
		  size  ��Ҫд������ݵĳ���
����ֵ���ɹ���ȡ�ļ����ݵĳ��ȣ�ʧ�ܷ���-1
*/
rs_s32 rs_fs_read(RS_FILE fh, void* buffer, rs_s32 size)
{ 
	rs_s32 read;
	rs_s32 fs_status;

	fs_status = FS_Read((FS_HANDLE)fh, buffer, size, &read);
	if (fs_status < FS_NO_ERROR || read != size) {
		RS_PORITNG_LOG("read error: status[%x], size[%d], read[%d]\r\n", fs_status, size, read);
		return -1;
	}
	
	return read;
}

/*
����˵����������д�뵽�ļ���
����˵����fh  �ļ��򿪺�ľ��
		  buffer  ��Ҫд�������
		  size  ��Ҫд������ݵĳ���
����ֵ���ɹ�Ϊд���ļ��ĳ��ȣ�ʧ�ܷ���-1
*/
rs_s32 rs_fs_write(RS_FILE fh, const void* buffer, rs_s32 size)
{
	rs_s32 written;
	rs_s32 fs_status;

	fs_status = FS_Write((FS_HANDLE)fh, (void*)buffer, size, &written);
	if (fs_status != FS_NO_ERROR || written != size) {
		RS_PORITNG_LOG("write error: status[%x], size[%d], written[%d]\r\n", fs_status, size, written);
		return -1;
	}
	
	return written;
}

/*
����˵���������ļ�ָ���λ��
����˵����fh  �ļ��򿪺�ľ��
		  offset  �����origin��ƫ����
		  origin  �ļ�ͷ β ��ǰλ��
����ֵ������0��ʧ�ܷ���-1
*/
rs_s32 rs_fs_seek(RS_FILE fh, rs_s32 offset, rs_s32 origin)
{
	rs_s32 fs_status;

	fs_status = FS_Seek((FS_HANDLE)fh, offset, origin);
	if(fs_status < FS_NO_ERROR) {
		RS_PORITNG_LOG("seek error: status[%x]\n\r", fs_status);
		return -1;
	}
	
	return 0;
}

/*
����˵������ȡ�ļ��Ĵ�С
����˵����fh  �ļ��򿪺�ľ��
����ֵ���ļ��Ĵ�С
*/
rs_s32 rs_fs_size(RS_FILE fh)
{ 
	rs_s32 fs_status;
	rs_s32 filesize;

	fs_status = FS_GetFileSize((FS_HANDLE)fh, &filesize);
	if (fs_status < FS_NO_ERROR) {
		RS_PORITNG_LOG("get file size error: status[%x]\n\r", fs_status);
		return -1;
	}
	
	return filesize;
}

/*
����˵�����ж��ļ�ϵͳ�е��ļ��Ƿ����
����˵����fileName  �ļ���·��
����ֵ������Ϊ1������Ϊ0
*/
rs_s32 rs_fs_exists(const rs_s8* fileName)
{
	RS_FILE f;
	f = rs_fs_open(fileName, RS_FS_OPEN_READ);
	if (f != RS_FS_INVALID)
	{
		// it existes
		rs_fs_close(f);
		RS_PORITNG_LOG("file[%s] is exist\n\r", fileName);

		return 1;
	}
	else
	{
		// not exist
		RS_PORITNG_LOG("file[%s] is not exist\n\r", fileName);

		return 0;
	}
}


/*
����˵����ɾ���ļ�ϵͳ�е��ļ�
����˵����fileName  �ļ���·��
����ֵ���ɹ�Ϊ0������Ϊʧ��
*/
rs_s32 rs_fs_remove(const rs_s8* fileName)
{
	rs_s32 fs_status;
	WCHAR wfilePath[MAX_FILE_PATH];

	rs_memset(wfilePath, 0, sizeof(wfilePath));
	kal_wsprintf(wfilePath, "%s", fileName);

	fs_status = FS_Delete(wfilePath);
	if (fs_status < FS_NO_ERROR) {
		RS_PORITNG_LOG("remove error: status[%x]\n\r", fs_status);
		return -1;
	}
	
	return 0;
}
