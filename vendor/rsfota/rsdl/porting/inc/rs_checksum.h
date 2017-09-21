/*
 *
 * 版权(c) 红石阳光（北京）科技有限公司
 * 版权(c) 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
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

