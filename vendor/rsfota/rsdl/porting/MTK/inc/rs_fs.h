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
#include "rs_datatype.h"

#define RS_FS_MAX_PATH          256

typedef int RS_FILE;

#define RS_FS_INVALID           ((RS_FILE)-1)

typedef enum
{
	RS_FS_OPEN_CREATE,      // �����ļ�
	RS_FS_OPEN_READ,        // �Զ��ķ�ʽ��
	RS_FS_OPEN_WRITE,		// ��д�ķ�ʽ��
	                        
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
����˵�������ļ�
����˵����fileName ��Ҫ�򿪵��ļ�
		 OpenMode ���ļ���ģʽ
����ֵ��RS_FS_INVALID��ʾʧ�ܣ�����Ϊ���ļ��ɹ���ľ��
*/
RS_FILE rs_fs_open(const rs_s8 * fileName, rs_fs_openMode openMode);

/**
����˵�����ر��ļ�
����˵����fh�ļ���� 
����ֵ��0��ʾ�ɹ���������ʾʧ��
*/
rs_s32 rs_fs_close(RS_FILE fh);

/**
����˵������ȡ�ļ�
����˵����fh�ļ���� 
		 Buffer��Ŷ�ȡ���ݵĻ�����
		 Size�������ĳ���
����ֵ��>0��ʾ�ɹ���ȡ�ļ����ݵĳ��ȣ�<0��ʾʧ�� 
*/
rs_s32 rs_fs_read(RS_FILE fh, void* buffer, rs_s32 size);

/**
����˵����д���ļ�
����˵����fh�ļ���� 
		 Buffer���Ҫд�����ݵĻ�����
		 Size�������ĳ���
����ֵ��>0��ʾ�ɹ�д���ļ����ݵĳ��ȣ�<0��ʾʧ�� 
*/
rs_s32 rs_fs_write(RS_FILE fh, const void* buffer, rs_s32 size);

/**
����˵����д���ļ�
����˵����fh�ļ���� 
		 Offsetƫ�ƻ�׼��ľ��� 
		 Origin��׼�������
����ֵ��0��ʾ�ɹ���-1��ʾʧ�� 
*/
rs_s32 rs_fs_seek(RS_FILE fh, rs_s32 offset, rs_s32 origin);

/**
����˵������ȡ�ļ���С
����˵����fh�ļ���� 
����ֵ��>=0��ʾ�ɹ���-1��ʾʧ�� 
*/
rs_s32 rs_fs_size(RS_FILE fh);

/**
����˵�����ļ��Ƿ����
����˵����fileName�ļ�·�� 
����ֵ��>=0��ʾ�ɹ���-1��ʾʧ�� 
*/
rs_s32 rs_fs_exists(const rs_s8* fileName);

/**
����˵����ɾ���ļ�
����˵����fileName�ļ�·�� 
����ֵ��0��ʾ�ɹ���-1��ʾʧ�� 
*/
rs_s32 rs_fs_remove(const rs_s8* fileName);

#ifdef __cplusplus
}
#endif

#endif
