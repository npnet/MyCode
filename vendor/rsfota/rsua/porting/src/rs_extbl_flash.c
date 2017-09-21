/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2007-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

/**
 * Implementation of the flash wrappers for use in the ua.
 */

/* Standard Redstone UA-SDK headers. */
#include "rsplatform.h"
#include "ftl.h"
#include <string.h>
//#include <bl_types.h>
//#include <bl_init.h>
//#include <bl_common.h>
#include "rs_extbl_oem.h"

extern FTL_FuncTbl *g_ftlFuncTbl;//FTL global variable

void rs_ua_set_array(rs_u8 *buffer, rs_u32 size, rs_u32 value)
{
	rs_u8 *p;
	rs_u32 i;

	p = buffer;
	for (i = 0; i < size; i++)
		*p++ = value;
}

void rs_test_data_array(rs_u8 *data, rs_u32 size)
{
	rs_u8 *p;
	rs_u32 i;

	p = data;
	for (i = 0; i < size; i++) {
		rs_trace("%02x ", *p++);
	}
}

/**
 * init NOR Flash
 *
 * @param
 *
 * @remarks
 *
 * @return
 * rs_true	: init successfully
 * rs_false	: init failed
 */
rs_bool rs_ua_flash_init(void)
{
	FTL_STATUS_CODE status;
	kal_uint32 block_size = 0, block_idx = 1;
	status = g_ftlFuncTbl->FTL_Init(NULL);
	if(status != FTL_SUCCESS) {
		rs_trace("FTL Init failed, quit update\n\r");
		return (rs_false);
	}
	#if 1
	rs_trace("jintao test block_size[%d], page size[%d]\n\r", g_ftlFuncTbl->FTL_GetBlockSize(1, NULL), g_ftlFuncTbl->FTL_GetPageSize());
	#endif
	rs_trace("FTL Init ok, quit update\n\r");
	return(rs_true);
}

rs_u32 rs_ua_getBlockSize()
{
	return g_ftlFuncTbl->FTL_GetBlockSize(1, NULL);
}

rs_u32 rs_ua_getPageSize()
{
#if 0
	return g_ftlFuncTbl->FTL_GetPageSize();
#endif
	return PAGE_SIZE;
}

/**
 * transfer physical address to block/page
 *
 * @param
 * @param[addr]		: physical address to transfer
 * @param[vBlock]	: data ptr of transferred block index
 * @param[vPage]	: data ptr of transferred page index
 *
 * @remarks
 *
 * @return
 * rs_true	: transfer successfully
 * rs_false	: transfer failed
 */
rs_bool rs_ua_addrToBlockPage(rs_u32 addr, rs_u32 *vBlock, rs_u32 *vPage)
{
	rs_u32 status;

	status = g_ftlFuncTbl->FTL_AddrToBlockPage(addr, vBlock, vPage, NULL);
	if (status != FTL_SUCCESS) {
		rs_trace("%s, transfer address[%x] to block/page failed, quit\n\r", __func__, addr);
		return (rs_false);
	}

	return (rs_true);
}

/**
 * Erase physical address on NOR Flash Raw disk
 *
 * @param
 * @param[address]	: physical address of special Block on Erasing
 * @param[size]			: erase block size
 *
 * @remarks
 *
 * @return
 * rs_true	: erase successfully
 * rs_false	: erase failed
 */

rs_bool rs_ua_flash_erase(volatile rs_u8 *address,
                        rs_u32 size)
{
	rs_u32 vBlock;
	rs_u32 vPage;
	rs_u32 vAddress = ((rs_u32)address);//note, maybe error

	//rs_trace("%s, address is %p, size is %x\n\r", __func__, address, size);
	// check block aligned or not
	if (vAddress & (RS_FOTA_BLOCK_BUFFER_SIZE - 1)) 
	{
		rs_trace("%s, failed causion:address is not aligned by Block[0x20000], quit\n\r", __func__);
		return (rs_false);
	}
	
	// address to Block/Page
	if (!rs_ua_addrToBlockPage(vAddress, &vBlock, &vPage))
	{
		rs_trace("%s, failed causion:transfer address to block/page failed, quit\n\r", __func__);
		return (rs_false);
	}
	//rs_trace("%s, vBlock is %d, vPage is %d\n\r", __func__, vBlock, vPage);
	// check erase block - page
	if (vPage != 0)
	{
		rs_trace("%s, failed causion:we must erase an entire block!!!\n\r", __func__);
		return (rs_false);
	}

	// erase
	if (g_ftlFuncTbl->FTL_EraseBlock(vBlock, NULL) != FTL_SUCCESS){
		rs_trace("%s, failed causion:erase error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
		return (rs_false);
	}

	return(rs_true);
}

rs_bool rs_ua_flash_read_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data)
{
	if (g_ftlFuncTbl->FTL_ReadPage(blockindex, page_index, (rs_u32 *)data, NULL) != FTL_SUCCESS)
	{
		rs_trace("%s, failed causion:read error at block[%d] page[%d], quit\n\r", __func__, blockindex, page_index);
		return (rs_false);
	}
	return (rs_true);
}

rs_bool rs_ua_flash_write_page(rs_u32 blockindex , rs_u32 page_index , rs_u8* data)
{
	if (g_ftlFuncTbl->FTL_WritePage(blockindex, page_index, (rs_u32 *)data, NULL) != FTL_SUCCESS)
	{
		rs_trace("%s, failed causion:read error at block[%d] page[%d], quit\n\r", __func__, blockindex, page_index);
		return (rs_false);
	}
	return (rs_true);
}

/**
 * read physical address on NOR Flash Raw disk
 *
 * @param
 * @param[destination]	: having read data size of data_ptr
 * @param[address]			: physical address of special Block on Reading
 * @param[size]					: reading block size
 *
 * @remarks
 *
 * @return
 * rs_true	: read successfully
 * rs_false	: read failed
 */
rs_bool rs_ua_flash_read(rs_u8 *destination,
                       volatile rs_u8 *address,
                       rs_u32 size)
{
	rs_u32 vBlock, vPage, i, j, vSizeCount, vSizeRemain, vAlignOffset;
	rs_u32 vAddress = ((rs_u32)address);
	rs_u8 *p;
	rs_u8 data[PAGE_SIZE] = {0};
	rs_u32 blocksize ,blk_page_count;

	blocksize = rs_ua_getBlockSize();
	blk_page_count = blocksize/PAGE_SIZE;
	
	//rs_trace("%s, vAddress[%x], size[%d]\n\r", __func__, vAddress, size);
	// check some common error
	if (size < 0) {
		return (rs_false);
	}
	rs_ua_set_array(data, PAGE_SIZE, 0);

	// check page aligned or not
	vAlignOffset = 0;
	if (vAddress & (PAGE_SIZE - 1)) {
		vAlignOffset = vAddress & (PAGE_SIZE - 1);
		vAddress = (vAddress - vAlignOffset);//vAddress & 0xfffffe00
	}

	//rs_trace("%s, handled vAddress[%x], vAlignOffset[%d]\n\r", __func__, vAddress, vAlignOffset);
	// transfer address to Block/Page
	if (!rs_ua_addrToBlockPage(vAddress, &vBlock, &vPage)) {
		rs_trace("%s No-aligned route, failed causion:transfer address to block/page failed, quit\n\r", __func__);
		return (rs_false);
	}

	// caculate size count/remain
	vSizeCount = (size / PAGE_SIZE);
	vSizeRemain = (size % PAGE_SIZE);

	//rs_trace("%s, transfer vAddress[%x] to vBlock[%d] vPage[%d], size[%d], vAlignOffset[%x]\
	//	vSizeCount[%d], vSizeRemain[%d]\n\r", __func__, vAddress, vBlock, vPage, size, vAlignOffset, vSizeCount, vSizeRemain);

	// Section A: size < PAGE_SIZE (include Address aligned or not)
	if (vSizeCount == 0) {
//		rs_trace("%s, run Section A\n\r", __func__);
		p = destination;//pointer to destination ptr
		// read one page data first
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			rs_trace("%s, Section A failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}

		// NO-aligned address
		if (vAlignOffset != 0) {
			// A.1 read size in one page, copy directly
			if ((PAGE_SIZE - vAlignOffset) >= vSizeRemain) {
				for (i = vAlignOffset; i < (vAlignOffset + vSizeRemain); i++) {
					*p = data[i];
					p++;
				}
//				rs_trace("%s, Section A.1 read data[0~3]:%x, data[%d~%d]:%x\n\r", __func__, *(rs_u32 *)destination, (size - 1), (size - 4), *(rs_u32 *)(destination + size - 4));
			//	rs_trace("%s, Section A.1 read data[0~3]:", __func__);
				//rs_print_data_array(destination, 4);
				//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
				//rs_print_data_array((destination + size - 4), 4);
				return (rs_true);
			// A.2 read size between two pages
			} else {
				vSizeRemain = (vSizeRemain - (PAGE_SIZE - vAlignOffset));
				rs_trace("%s, Section A.2 read size overlap two pages, remain size[%d]", __func__, vSizeRemain);
				for (i = vAlignOffset; i < PAGE_SIZE; i++) {
					*p = data[i];
					p++;
				}
				// handle and check page
				vPage++;
				if (vPage > (blk_page_count - 1)) {
					vBlock++;
					vPage = 0;
					// check block count
				}
				// init data array
				rs_ua_set_array(data, PAGE_SIZE, 0);
				
				// read one page
				if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
					rs_trace("%s, Section A.2 failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
					return (rs_false);
				}

				//copy remain data to destination ptr
				for (i = 0; i < vSizeRemain; i++) {
					*p = data[i];
					p++;
				}
			//	rs_trace("%s, Section A.2 read data[0~3]:", __func__);
				//rs_print_data_array(destination, 4);
				//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
				//rs_print_data_array((destination + size - 4), 4);
				return (rs_true);
			}
		}
		// A.3 Aligned adrress, but size < PAGE_SIZE
		//copy data directly to destination ptr
		for (i = 0; i < vSizeRemain; i++) {
			*p = data[i];
			p++;
		}
		//rs_trace("%s, Section A.3 read data[0~3]:", __func__);
		//rs_print_data_array(destination, 4);
		//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
		//rs_print_data_array((destination + size - 4), 4);
		return (rs_true);
	}

	// Section B: size > PAGE_SIZE
//	rs_trace("%s, run Section B\n\r", __func__);
	p = destination;//pointer to destination ptr
	// According to No-aligned Address, read some data align to page
	if (vAlignOffset != 0) {
		// first, read one page first
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			rs_trace("%s, Setion B.1 failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}
		// copy one page to data ptr
		for (i = vAlignOffset; i < PAGE_SIZE; i++) {
			*p = data[i];
			p++;
		}
		vSizeCount = ((size - (PAGE_SIZE - vAlignOffset)) / PAGE_SIZE);
		vSizeRemain = ((size - (PAGE_SIZE - vAlignOffset)) % PAGE_SIZE);
		// next page
		vPage++;
		// check page
		if (vPage > (blk_page_count - 1)) {
			vBlock++;
			vPage = 0;
			// check block count
		}
		// init data array
		rs_ua_set_array(data, PAGE_SIZE, 0);
	}

//	rs_trace("%s, after resize, vBlock[%d] vPage[%d], vSizeCount[%d], vSizeCount[%d]\n\r", __func__, vBlock, vPage, vSizeCount, vSizeRemain);
	// Now the address && data && destination ptr is aligned to PAGE_SIZE
	// Section B.1:  size > PAGE_SIZE, No-aligned Address, between two pages + some remain size(<PAGE_SIZE)
	if (vSizeCount == 0) {
		// read one page first
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			rs_trace("%s, Setion B.1 failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}
		// copy read-data to data ptr
		for ( i = 0; i < vSizeRemain; i++) {
			*p = data[i];
			p++;
		}
		//rs_trace("%s, Section B.1 read data[0~3]:", __func__);
		//rs_print_data_array(destination, 4);
		//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
		//rs_print_data_array((destination + size - 4), 4);	
		return (rs_true);
	}

//	rs_trace("%s, circle before, vSizeCount[%d]\n\r", __func__, vSizeCount);
	// Now cycle read, address && data && destination ptr is aligned to PAGE_SIZE
	// Section B.2 and B.3 and B.4, n * PAGE_SIZE, perhaps remain data size
	for (j = 0 ; j < vSizeCount; j++) {
//		rs_trace("%s, circling, vBlock[%d] vPage[%d], vSizeCount[%d], vSizeRemain[%d]\n\r", __func__, vBlock, vPage, vSizeCount, vSizeRemain);
		// read data
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			rs_trace("%s, Setion B.2 || B.3 failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}
		//rs_trace(", vpage = %d,data[%d~%d]:", vPage,0,3);
		//rs_print_data_array(data, 4);
		// copy one page data to destination ptr
		for (i = 0; i < PAGE_SIZE; i++) {
			*p = data[i];
			p++;
		}
		
		// next page
		vPage++;
//		rs_trace("%s, after resize ???, get vPage[%d]\n\r", __func__, vPage);
		// check page
		if (vPage > (blk_page_count - 1)) {
			vBlock++;
			vPage = 0;
			// check block count
		}
		// init data array
		rs_ua_set_array(data, PAGE_SIZE, 0);
	}

//	rs_trace("%s, after resize circle, vSizeCount[%d], vBlock[%d], vPage[%d], vSizeRemain[%d]\n\r", __func__, vSizeCount, vBlock, vPage, vSizeRemain);
	// Section B.2 and B.3 and B.4
	// Section B.2: NO-aligned address, some pages + some remain size(<PAGE_SIZE)
	// Section B.3: Aligned address, some pages + some remain size(<PAGE_SIZE)
	// Section B.4: Aligned address, some pages + no remain size
	if (vSizeRemain != 0) {
//		rs_trace("%s, after resize 111, block[%d] page[%d], size count[%d], size remain[%d]\n\r", __func__, vBlock, vPage, vSizeCount, vSizeRemain);
		// read data
		if (g_ftlFuncTbl->FTL_ReadPage(vBlock, vPage, (rs_u32 *)data, NULL) != FTL_SUCCESS) {
			rs_trace("%s, Setion B.2 || B.3 failed causion:read error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}
		// copy remain data to destination ptr
		for (i = 0; i < vSizeRemain; i++) {
			*p = data[i];
			p++;
		}
		//rs_trace("%s, Section B.2 || B.3 read data[0~3]:", __func__);
		//rs_print_data_array(destination, 4);
		//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
		//rs_print_data_array((destination + size - 4), 4);
		return (rs_true);
	}
	//rs_trace("%s, Section B.4 read data[0~3]:", __func__);
	//rs_print_data_array(destination, 4);
	//rs_trace(", data[%d~%d]:", (size - 4), (size - 1));
	//rs_print_data_array((destination + size - 4), 4);
	return(rs_true);
}

/**
 * extend to erase block index on NOR Flash Raw disk
 *
 * @param
 * @param[vBlock]	: block index to erase
 *
 * @remarks
 *
 * @return
 * rs_true	: erase successfully
 * rs_false	: erase failed
 */
rs_bool rs_ua_flash_erase_ext(rs_u32 vBlock)
{
	//rs_trace("%s, vBlock is %p, \n\r", __func__, vBlock);
	if (g_ftlFuncTbl->FTL_EraseBlock(vBlock, NULL) != FTL_SUCCESS) {
		rs_trace("%s finished, failed causion:erase error at block[%d], quit\n\r", __func__, vBlock);
		return (rs_false);
	}
	return(rs_true);
}

/**
 * write data to sepcial block on NOR Flash Raw disk
 *
 * @param
 * @param[address]	: physical address of special Block on Writing
 * @param[source]		: having read data size of data_ptr
 * @param[size]			: writing block size
 *
 * @remarks
 *
 * @return
 * rs_true	: write successfully
 * rs_false	: write failed
 */
rs_bool rs_ua_flash_write(volatile rs_u8 *address,
                       rs_u8 *source,
                       rs_u32 size)
{
	rs_u32 vBlock, vPage;
	rs_u32 vAddress = ((rs_u32)address);//note, maybe error
	rs_u8 *p;
	rs_u32 addr;
	rs_u32 blocksize ,blk_page_count;
	rs_u32 needWriteSize = size;

	blocksize = rs_ua_getBlockSize();
	blk_page_count = blocksize/PAGE_SIZE;

	//rs_trace("%s, address[%x], size[%d]\n\r", __func__, vAddress, size);
//	rs_print_data_array(source, 4);
	// check page aligned or not
	if (vAddress & (PAGE_SIZE - 1)) {
		rs_trace("%s, failed causion:address is not aligned by PAGE_SIZE[0x200], quit\n\r", __func__);
		return (rs_false);
	}

	// transfer address to Block/Page
	if (!rs_ua_addrToBlockPage(vAddress, &vBlock, &vPage)) {
		rs_trace("%s, failed causion:transfer address to block/page failed, quit\n\r", __func__);
		return (rs_false);
	}

	//rs_trace("%s, transfer address[%p] to block[%d] page[%d], size is %x\n\r", __func__, address, vBlock, vPage, size);

	// write data to special block/page cycle
	p = source;
	while (needWriteSize > PAGE_SIZE) {
		if (vPage > (blk_page_count - 1)) {
			vBlock++;
			vPage = 0;

			// check block count
//			rs_trace("%s, next block vBlock[%d], needWriteSize[%d]\n\r", __func__, vBlock, needWriteSize);
			if (rs_ua_flash_erase_ext(vBlock) != rs_true){
				rs_trace("%s, erase block[%d] failed\n\r", __func__, vBlock);
				return (rs_false);
			}
		}

		
		if (g_ftlFuncTbl->FTL_WritePage(vBlock, vPage, (rs_u32 *)p, NULL) != FTL_SUCCESS) {
			rs_trace("%s finished, failed causion:erase error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}

		needWriteSize -= PAGE_SIZE;
		p += PAGE_SIZE;
		//handle page
		vPage++;
		
	}

	if (needWriteSize > 0)
	{
		rs_u8 buf[PAGE_SIZE] = {0};

		memcpy(buf,  p,  needWriteSize);
		if (g_ftlFuncTbl->FTL_WritePage(vBlock, vPage, (rs_u32 *)p, NULL) != FTL_SUCCESS) {
			rs_trace("%s finished, failed causion:erase error at block[%d] page[%d], quit\n\r", __func__, vBlock, vPage);
			return (rs_false);
		}		
	}
	
	return(rs_true);
}