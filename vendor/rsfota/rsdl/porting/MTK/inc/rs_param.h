#ifndef _RS_PARAM_H_
#define _RS_PARAM_H_

#include "rs_datatype.h"
#include "rs_porting_cfg.h"
#include "rs_dev.h"

/**
����˵������ȡ�豸����
����˵������key���豸���Ա�ʶ
		  ��value������ֵ
����ֵ���ɹ�����0��ʧ�ܷ���1
ע�⣺value�ĳ���Ϊ64���ֽڣ���Ҫ��Ҫ����63���ֽ�
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value);

/**
����˵������ѯ�Ƿ����û�����
����˵����
����ֵ������0����ʾû��UI������1����ʾ��UI
����˵������û��UI�����ĳ������ǲ�ͬ�ġ������ϴ��������ع����У�����ĳ���������
�����ֻ���������ô����֮�������UI�����û����������û��UI����������
*/
rs_s32 rs_cb_have_ui();

/**
����˵������ȡ��һ���Զ������뿪����ʱ������
����˵����
����ֵ������ֵΪ���뿪���ĺ���ʱ��
*/
rs_u32 rs_cb_get_first_check_cycle();

/**
����˵������һ���Զ�����������֮��ÿ���Զ����ļ����ʱ��
����˵����
����ֵ������ֵΪÿ���Լ������ڵļ��ʱ��
*/
rs_u32 rs_cb_get_auto_check_cycle();

/**
����˵������ʱ�����粻�õ�������������ʧ�ܣ������´μ��ʱ���ֻ�Ҫ�ܾã�
		  ������Ҫ��ʱ�Ѽ��ʱ������
����˵����
����ֵ������ֵΪ�´μ��ĵļ��ʱ�䣨һ����Ч��
*/
rs_u32 rs_cb_get_once_check_cyle();


/**
����˵�����Զ�������ڵ���
����˵������
����ֵ���ɹ��Ż�RS_ERR_OK, ʧ�ܷ�������ֵ
*/
rs_s32 rs_cb_auto_check();

/**
����˵����������������Դ���
����˵������
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_check_retry_count();

/**
����˵������ȡ����������Դ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_retry_count();

/**
����˵������ȡ�ϱ����������ʧ�ܲ��ԣ�rs_cb_get_report_retry_count�����ϱ����������ʧ���˻��Զ����ԵĴ�����
		����ܴ���������rs_cb_get_report_max_fail_count�����趨�����ޣ���ô�����ϱ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_report_max_fail_count();

/**
����˵������ȡ��������ϱ�������Դ���
����˵����
����ֵ���������Դ���
�ر�˵���������Դ�����Ҫ���ô�һ�㣬�������ϱ��ɹ���������ں����ж����Ӱ��
*/
rs_s32 rs_cb_get_report_retry_count();

/**
����˵������ȡ���ع��������ʧ�ܲ��ԣ�rs_cb_get_download_retry_count�������ع��������ʧ���˻��Զ����ԵĴ���
		����ܴ��������˴˺����趨�����ޣ���ô��������������
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_max_fail_count();

/**
����˵����socket�����й����еĸ��ֳ�ʱ����
����˵����type ����ͬ��ʱ����
			0  gethostbyname�ĳ�ʱ����
			1  connect�ĳ�ʱ����
			2  send�ĳ�ʱ����
			3  recv�ĳ�ʱ����
����ֵ�����س�ʱ��ʱ�����ڣ���λΪ����
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type);

/**
����˵������ȡ��ǰ���ʣ������Ƿ����
����˵����
����ֵ���������ڵ���33%,�򷵻�RS_TRUE,����RS_FALSE
*/
rs_bool rs_sys_bettery_electricity_enough();

/**
����˵������ȡ��ǰ���õĿռ�
����˵����
����ֵ�����ص�ǰ���õ�ʣ��ռ�
*/
rs_u32 rs_sys_available_space();

//������־�ȼ������ļ�·��
#define RS_CONFIG_DEBUGINFO_FILE "Z:\\debug.ini"

// ����������Ϣ�洢���ļ�·������flash����ʼ��ַ
#ifdef RS_SUPPORT_UPDATE_INFO_FS

#define RS_CONFIG_UPINFO_FILE1 "Z:\\updateinfo1"
#define RS_CONFIG_UPINFO_FILE2 "Z:\\updateinfo2"
#define RS_CONFIG_UPINFO_FILE1 "Z:\\@rsfota\\updateinfo1"
#define RS_CONFIG_UPINFO_FILE2 "Z:\\@rsfota\\updateinfo2"

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info1_file();
const rs_s8* rs_device_get_update_info2_file();

#endif

#define BLOCK_SIZE (0x8000) //32768
#define PAGE_SIZE 512


// ���������ļ��洢���ļ�·������flash�е���ʼ��ַ
#ifdef RS_SUPPORT_UPDATE_DATA_FS

#define FLASH_UPDATE_FILE "Z:\\update.bin"
#define FLASH_UPDATE_FILE "Z:\\@rsfota\\update.bin"

/**
����˵������ȡ������������·��
����˵����
����ֵ����������·��
*/
const rs_s8* rs_device_get_update_file();

#else

/*
������������ݴ洢��flash�У���ô������Ϣ����Ҳ�϶���Ҫ��flash�д洢��
��Ϊbootloader��Ҫ��ȡ
*/

// ��ְ��洢��ַ
#define FLASH_UPDATE_ADDR 0X580000

/**
����˵������ȡ�����������ش�ŵ�flash�ĵ�ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_addr();

// ���ֵ������Դ��custom_MemoryDevice.h
// ��һ��ֵΪfs�ĵ�ַ��ȥ2��block���ڶ���Ϊ��ȥ1��block
#define RS_CONFIG_UPINFO_ADDR1  0x6F8000
#define RS_CONFIG_UPINFO_ADDR2  0x6F0000
/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info1_addr();
rs_u32 rs_device_get_update_info2_addr();

#endif


/**
����˵������ȡVIVA����ʼ��ַ
����˵������
����ֵ������VIVA����ʼ��ַ�����û��VIAVA�����򷵻�0 
*/
rs_u32 rs_sys_getVIVAStartAddr();


#endif