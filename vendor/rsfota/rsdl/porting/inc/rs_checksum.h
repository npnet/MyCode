/*
 *
 * ��Ȩ(c) ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */

#ifndef __RS_SI_CHECKSUM_H__
#define __RS_SI_CHECKSUM_H__

#ifdef  __cplusplus
extern "C"
{
#endif
#include "rs_datatype.h"

rs_s32 rs_open_checksum(/*rs_u32 **checksumTable*/);

rs_u32 rs_get_checksum(rs_u32 crc,
                       const rs_u8 *buf,
                       rs_u32 len/*,
                       rs_u32 *checksumTable*/);

void rs_close_checksum(/*rs_u32 *checksumTable*/);

#ifdef  __cplusplus
}
#endif

#endif

