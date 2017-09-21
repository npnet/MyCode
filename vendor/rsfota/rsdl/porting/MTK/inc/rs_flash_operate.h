#ifndef __RSFLASHOPERATE_H__
#define __RSFLASHOPERATE_H__


#ifdef __cplusplus
  extern "C"
  {
#endif

#include "rs_porting_cfg.h"

#include "rs_datatype.h"


/**
����˵����flash��ʼ�� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_init();

/**
����˵������ȡ��ǰflash��һ��page�Ĵ�С 
����˵������
����ֵ���ɹ�����page��size��ʧ�ܷ���-1 
*/
rs_s32 rs_flash_getPageSize();

/**
����˵����flash��ʼ�� 
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_getBlockSize();

/**
����˵��������block 
����˵����addr��Ҫ������block�ĵ�ַ
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_eraseBlock(rs_u32 addr);

/**
����˵��������page��ȡ���� 
����˵����addr ��Ҫ��ȡ�ĵ�ַ
		 buffer ���Ҫ��ȡ���ݵĻ�����
		 len ��Ҫд��buffer���ݵĳ��ȣ�����Ϊһ��page�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_readPage(rs_u32 addr, rs_u8* buffer, rs_s32 len);


/**
����˵��������pageд������ 
����˵����addr ��Ҫд��ĵ�ַ
		 Buffer ��Ҫд���buffer����
		 Len ��Ҫд������ݵĳ��ȣ�����Ϊһ��page�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_writePage(rs_u32 addr, const rs_u8* buffer, rs_s32 len);

/**
����˵��������block��ȡ���� 
����˵����addr ��Ҫ��ȡ�ĵ�ַ,block����
		 Buffer ��Ҫ��ȡ��buffer����
		 Len ��Ҫ��ȡ�����ݵĳ��ȣ�����ΪС��block�ĳ���
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_readLessBlock(rs_u32 addr, rs_u8* buffer, rs_s32 len);

/**
����˵���������ݺ�������ݶ�Ӧ��checkSumд�뵽ĳ��block��
����˵����addr ��Ҫд��ĵ�ַ,block����
		buffer ��Ҫд������ݵĻ�����
		len buffer���ݵĻ�����
		checkSum ������ݵ�У��ֵ
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_flash_writeDataWithCSToBlock(rs_u32 addr, const rs_u8* buffer, rs_s32 len, rs_u32 checkSum);

/**
����˵������ָ����ַ��ȡ����unsigned long�͵�����ֵ
����˵����addr ��Ҫ��ȡ�ĵ�ַ
����ֵ�����ض���������ֵ
*/
rs_u32 rs_flash_readU32(rs_u32 addr);

/*
����˵�������ݵ�ַ�����ȣ�����ö����ݵ�checksumֵ
����˵����addr ��ʼ��ַ��page����
			len ���ݳ���
����ֵ��ʧ��Ϊ-1���ɹ�ΪУ��ֵ
*/
rs_u32 rs_flash_calculatCSByAddr(rs_u32 addr, rs_s32 len);

#ifdef __cplusplus
  }
#endif

#endif
