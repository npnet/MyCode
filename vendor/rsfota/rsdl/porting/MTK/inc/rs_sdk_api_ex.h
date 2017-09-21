#ifndef _RS_SDK_API_EX_H_
#define _RS_SDK_API_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rs_datatype.h"

/**
����˵��������sdk���ṩ�ļ��ӿڵ�һ����չ
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_check_ex(rs_s32 type);


/**
����˵��������sdk���ṩ�����ؽӿڵ�һ����չ�������˿ռ���ж�
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_download_ex(rs_s32 type);

/**
����˵������װ�Ѿ����ص�������
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
��¶���û�����
*/
rs_s32 rs_sdk_install_ex();

/*
	��ȡ��ǰ�汾�ĳ���
*/
rs_u32 rs_sdk_new_version_size(void);

/*
 * ��ȡ�°汾�İ汾��
*/
rs_s8* rs_sdk_new_version_name(void);

/*
 * ��ȡ�°汾�İ汾����
*/
rs_s8* rs_sdk_new_version_description(void);

#ifdef __cplusplus
}
#endif

#endif