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
