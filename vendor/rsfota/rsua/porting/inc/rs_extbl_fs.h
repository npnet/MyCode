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

#include "rsplatform.h"

rs_s32 rs_fs_open_only(const char* fileName);

rs_s32 rs_fs_open_create(const char* fileName);

rs_s32 rs_fs_open_rw(const char* fileName);

void rs_fs_close(rs_s32 handle);

rs_u32 rs_fs_read(rs_s32 handle,void *buffer, rs_u32 len, rs_u32 *read);

rs_s32 rs_fs_write(rs_s32 handle, const void* buffer, int size, rs_u32 *written);

rs_s32 rs_fs_seek(rs_s32 handle,rs_u32 offset, rs_s32 origin);

rs_s32 rs_fs_getFileSize(rs_s32 handle,rs_s32* size);

rs_u32 rs_fs_exists(const char* fileName);

rs_u32 rs_fs_remove(const char* fileName);

#endif
