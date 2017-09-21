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
#ifndef _RS_SDK_API_H_
#define _RS_SDK_API_H_

#include "rs_datatype.h"
#include "rs_state.h"


#ifdef __cplusplus
extern "C" {
#endif

// ��ȡ�豸��ϢKEY
#define PROPERTY_KEY_DEVICE_DEVID		"product.devid"			//�豸ΨһID
#define PROPERTY_KEY_DEVICE_MAN			"product.manu"			//�豸���� 
#define PROPERTY_KEY_DEVICE_MODEL		"product.model"			//�豸�ͺ�
#define PROPERTY_KEY_DEVICE_FWV			"product.fwv"			//�豸��ǰ�̼��汾��
#define PROPERTY_KEY_DEVICE_LANG		"product.lang"			//�豸ʹ������
#define PROPERTY_KEY_DEVICE_OPERATORS	"product.operators"		//�豸������Ӫ�̣���46000
#define PROPERTY_KEY_DEVICE_OSV			"product.osv"			//��ȡϵͳ�İ汾��
#define PROPERTY_KEY_DEVICE_LOC			"product.loc"			//��ȡλ����Ϣ
#define PROPERTY_KEY_DEVICE_UTDID		"product.utdid"			//�豸Ψһ��ʶ��
#define PROPERTY_KEY_DEVICE_APP_KEY		"product.appkey"		//app key
#define PROPERTY_KEY_DEVICE_CHANNEL		"product.channel"		//channel
#define PROPERTY_KEY_DEVICE_DEVMAC		"product.devmac"		//MAC

// ����������������Ϣ
typedef struct _RS_FW_DD_ 
{
	rs_s8    key[64];				    //keyֵ����Ϊ32���ֽڣ���ְ�id
	rs_s8    correlator[64];			//����Ϊ32���ֽڣ��������˶�Ӧ��Ψһ��ʶ
	rs_s8    objectUri[516];			//url����Ϊ512���ֽڣ����������ص�ַ
	rs_u32   objectSize;				//��������С
	rs_s8    toVersion[132];			//����Ϊ128���ֽڣ�������Ŀ��汾
	rs_s8    hashValue[34];             //32λ�ַ���
	rs_s8    dateLine[20];              //16Ϊ�ַ�����yyyyMMddTHHmmssZ
	rs_s8    brief[516];			    //����Ϊ512���ֽڣ���������������Ϣ
	rs_u32   autoDownload;				//�Ƿ��Զ����أ�0���Զ����أ�1 WIFI���Զ����أ�2���������Զ�����
	rs_u32	 forceUpdate;				//�Ƿ�ǿ��������0��ʾ��ǿ��������1��ʾǿ������
} RS_FWDL_DD;

/**
����˵��������TOTA����
����˵������
����ֵ���ɹ�����RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵����
*/
void rs_sdk_start();

/**
����˵�������̼�����
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
*/
rs_s32 rs_sdk_check(rs_s32 type);

/**
����˵�����Զ����̼�����
����˵����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
��¶���û�����
*/
rs_s32 rs_sdk_autoCheck();


/**
����˵�������������� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
*/
rs_s32 rs_sdk_download(rs_s32 type);

/**
����˵����ֹͣ�������� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
*/
rs_s32 rs_sdk_stopDownload();


/**
����˵������װ�Ѿ����ص�������
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
*/
rs_s32 rs_sdk_install();


/**
����˵������ȡ����ǰ��������������Ϣ 
����˵������
����ֵ���ɹ�����DD��Ϣ��ָ�룬���򷵻�ΪNULL���ṹ�����RS_FW_DD
�ر�˵���� 
*/
const RS_FWDL_DD* rs_sdk_getPkgDD();

/**
����˵������ȡ�Ѿ������������Ĵ�С
����˵������
����ֵ�������Ѿ������������Ĵ�С
�ر�˵���� 
��¶���û�����
*/
rs_s32 rs_sdk_getAlreadyDownloadSize();

/**
����˵������ȡ���汾�ķ������ͣ��������Զ�����  �ֶ����� 
����˵������
����ֵ������rs_sdk_check���������
�ر�˵���� 
*/
rs_s32 rs_sdk_getCheckInitType();

/**
����˵����������ǰ�汾
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵���� �����⵽�汾�������Ѿ�������ɲ��֣������Ѿ�������ɣ������Է����˰汾������������أ�Ӧ������ֹͣ������
*/
rs_s32 rs_sdk_discardCurentPkg();

/**
����˵������ȡ��ǰ����������
����ֵ�����������룬��rs_error.h����
*/
rs_u32 rs_sdk_getLastError();

/**
����˵������ȡ��װ���֮��Ĵ�����
����˵����
����ֵ�����ش�����
*/
rs_s32 rs_sdk_getInstallErrCode();

/**
����˵������ȡ��ǰ��״̬ 
����˵������
����ֵ�����ص�ǰ��״̬
��¶���û�����
*/
RS_FWDL_STATE rs_sdk_getDLCurrentState();


/**
����˵�����ж��Ƿ���Ҫ���Զ����Ĺ���
����˵����
����ֵ��RS_TRUE��ʾ��Ҫ RS_FALSE��ʾ����Ҫ
*/
rs_bool rs_sdk_needAutoCheck();

#ifdef __cplusplus
}
#endif

#endif