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
函数说明：打开文件
参数说明：fileName  文件的路径
		  openMode  打开的模式，参见rs_fs_openMode
返回值：成功返回文件句柄，失败返回RS_FS_INVALID
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
函数说明：关闭文件
参数说明：fh  文件打开后的句柄
返回值：成功返回0，失败为-1
*/
rs_s32 rs_fs_close(RS_FILE fh)
{
	FS_Close((FS_HANDLE)fh);
	return 0;
}

/*
函数说明：把数据读取到缓冲区内
参数说明：fh  文件打开后的句柄
		  buffer  需要读入的数据的缓冲区
		  size  需要写入的数据的长度
返回值：成功读取文件数据的长度，失败返回-1
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
函数说明：把数据写入到文件中
参数说明：fh  文件打开后的句柄
		  buffer  需要写入的数据
		  size  需要写入的数据的长度
返回值：成功为写入文件的长度，失败返回-1
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
函数说明：设置文件指针的位置
参数说明：fh  文件打开后的句柄
		  offset  相对于origin的偏移量
		  origin  文件头 尾 当前位置
返回值：返回0，失败返回-1
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
函数说明：获取文件的大小
参数说明：fh  文件打开后的句柄
返回值：文件的大小
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
函数说明：判断文件系统中的文件是否存在
参数说明：fileName  文件的路径
返回值：存在为1，其他为0
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
函数说明：删除文件系统中的文件
参数说明：fileName  文件的路径
返回值：成功为0，其他为失败
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
