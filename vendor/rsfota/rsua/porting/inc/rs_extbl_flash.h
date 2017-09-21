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
