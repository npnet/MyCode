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

void rs_ua_set_array(rs_u8 *buffer, rs_u32 size, rs_u32 value);

void rs_test_data_array(rs_u8 *data, rs_u32 size);

rs_bool rs_ua_flash_init(void);

rs_u32 rs_ua_getBlockSize();

rs_u32 rs_ua_getPageSize();

rs_bool rs_ua_addrToBlockPage(rs_u32 addr, rs_u32 *vBlock, rs_u32 *vPage);

rs_bool rs_ua_flash_erase(volatile rs_u8 *address, rs_u32 size);

rs_bool rs_ua_flash_read_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

rs_bool rs_ua_flash_write_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data);

rs_bool rs_ua_flash_read(rs_u8 *destination, volatile rs_u8 *address, rs_u32 size);

rs_bool rs_ua_flash_erase_ext(rs_u32 vBlock);

rs_bool rs_ua_flash_write(volatile rs_u8 *address, rs_u8 *source, rs_u32 size);

#endif
