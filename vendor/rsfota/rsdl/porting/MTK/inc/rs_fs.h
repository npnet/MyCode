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
#ifndef RS_FS_H
#define RS_FS_H
#include "rs_datatype.h"

#define RS_FS_MAX_PATH          256

typedef int RS_FILE;

#define RS_FS_INVALID           ((RS_FILE)-1)

typedef enum
{
	RS_FS_OPEN_CREATE,      // 创建文件
	RS_FS_OPEN_READ,        // 以读的方式打开
	RS_FS_OPEN_WRITE,		// 以写的方式打开
	                        
} rs_fs_openMode;

typedef enum 
{
	RS_FS_SEEK_SET,   
	RS_FS_SEEK_CUR,    
	RS_FS_SEEK_END,  

} rs_fs_seekMode;


#ifdef __cplusplus
extern "C" {
#endif

/**
函数说明：打开文件
参数说明：fileName 需要打开的文件
		 OpenMode 打开文件的模式
返回值：RS_FS_INVALID表示失败，其他为打开文件成功后的句柄
*/
RS_FILE rs_fs_open(const rs_s8 * fileName, rs_fs_openMode openMode);

/**
函数说明：关闭文件
参数说明：fh文件句柄 
返回值：0表示成功，其他表示失败
*/
rs_s32 rs_fs_close(RS_FILE fh);

/**
函数说明：读取文件
参数说明：fh文件句柄 
		 Buffer存放读取数据的缓冲区
		 Size缓冲区的长度
返回值：>0表示成功读取文件数据的长度，<0表示失败 
*/
rs_s32 rs_fs_read(RS_FILE fh, void* buffer, rs_s32 size);

/**
函数说明：写入文件
参数说明：fh文件句柄 
		 Buffer存放要写入数据的缓冲区
		 Size缓冲区的长度
返回值：>0表示成功写入文件数据的长度，<0表示失败 
*/
rs_s32 rs_fs_write(RS_FILE fh, const void* buffer, rs_s32 size);

/**
函数说明：写入文件
参数说明：fh文件句柄 
		 Offset偏移基准点的距离 
		 Origin基准点的类型
返回值：0表示成功，-1表示失败 
*/
rs_s32 rs_fs_seek(RS_FILE fh, rs_s32 offset, rs_s32 origin);

/**
函数说明：获取文件大小
参数说明：fh文件句柄 
返回值：>=0表示成功，-1表示失败 
*/
rs_s32 rs_fs_size(RS_FILE fh);

/**
函数说明：文件是否存在
参数说明：fileName文件路径 
返回值：>=0表示成功，-1表示失败 
*/
rs_s32 rs_fs_exists(const rs_s8* fileName);

/**
函数说明：删除文件
参数说明：fileName文件路径 
返回值：0表示成功，-1表示失败 
*/
rs_s32 rs_fs_remove(const rs_s8* fileName);

#ifdef __cplusplus
}
#endif

#endif
