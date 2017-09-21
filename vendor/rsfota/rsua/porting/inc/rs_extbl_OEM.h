#ifndef __RS_EXTBL_OEM_H__
#define __RS_EXTBL_OEM_H__

#include "rsplatform.h"

/* project */
//#define CORE_TEK
//#define JIAWEI
//#define ABADEEN
//#define KCT
//#define WHATER_WORLD
#define NBBIRD

#define RS_NOR_FS_BASE 0x700000//0x03C0000

/* define ota package store on flash or FS */
#if defined(JIAWEI) || defined(NBBIRD)
#define RS_DIFF_PACKAGE_ON_FLASH
#endif

/* flash page size. IMPORTANT!!! */
#define PAGE_SIZE 512

/* flash block size. IMPORTANT!!! */
#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT)
#define RS_FOTA_BLOCK_BUFFER_SIZE 65536 // 64KB
#elif defined(JIAWEI)
#define RS_FOTA_BLOCK_BUFFER_SIZE 131072// 128KB
#elif defined(CORE_TEK)
#define RS_FOTA_BLOCK_BUFFER_SIZE 4096 // 4K
#elif defined(NBBIRD)
#define RS_FOTA_BLOCK_BUFFER_SIZE 32768 // 0x8000
#endif

/* ram size define */
#if defined(TANGMAO) || defined(WHATER_WORLD) || defined(KCT) || defined(CORE_TEK)
#define RS_FOTA_RAM_BUFFER_SIZE 1572864// 3.5M - 64 * 1024
//#define RS_FOTA_RAM_BUFFER_SIZE 3604480// 3.5M - 64 * 1024
#elif defined(JIAWEI) || defined(NBBIRD)
#define RS_FOTA_RAM_BUFFER_SIZE 1572864// 1.5M
#endif

/* update area start address. IMPORTANT!!! */
#if defined(CORE_TEK)
#define FOTA_UPGRADE_START 0x2B000//block 5
#elif defined(JIAWEI)
#define FOTA_UPGRADE_START 0x20000//block 1
#elif defined(WHATER_WORLD)
#define FOTA_UPGRADE_START 0x30000//block 5
#elif defined(KCT)
#define FOTA_UPGRADE_START 0x30000//block 3
#elif defined(NBBIRD)
#define FOTA_UPGRADE_START 0x28000 //rom base addr 
#endif

/* backup block, redstone sdk will using it. IMPORTANT!!! */
#if defined(ABADEEN)
#define PARITY_BLOCK 127 // backup rom&viva block info
#define PARITY_ADDR 0xCF0000 //backup block for the last upgrade block
#elif defined(JIAWEI)
#define PARITY_BLOCK 97 // backup rom&viva block info
#define PARITY_ADDR 0xC20000 //line to block 97
#elif defined(TANGMAO)
#define PARITY_BLOCK 100 // backup rom&viva block info
#define PARITY_ADDR 0x640000 //line to block 100
#elif defined(WHATER_WORLD)
#define PARITY_ADDR 0x1C90000 //line to block 100
#elif defined(CORE_TEK) 
#define PARITY_ADDR 0x3B7000 //line to block 100
#elif defined(NBBIRD)
#define PARITY_ADDR 0x6E8000
#endif

/* 1024 space between VIVA and ROM */
#if defined(ABADEEN) || defined(CORE_TEK) 
#define BLANK_SIZE 1024
#else
#define BLANK_SIZE 0
#endif

/* fota file store dir */
#if defined(RS_DIFF_PACKAGE_ON_FLASH)
#if defined(JIAWEI)
#define FOTA_FILE_BLOCK 84//reserved block 76 ~ 83, we use block 76~83 to store diff-ota file.
#define RS_FOTA_ADDRESS	0xA80000 //line to block 76
#define FOTA_UPDATE_INFO_BLOCK 99 //store update info, main
#define FOTA_UPDATE_INFO_ADDR 0xC60000 //line to block 99
#define FOTA_BACKUP_UPDATE_INFO_BLOCK 98 //store update info, backup
#define FOTA_BACKUP_UPDATE_INFO_ADDR 0xC40000 //line to block 98
#elif defined(ABADEEN1)
#define FOTA_FILE_BLOCK 82//reserved block 82 ~ 97, we use block 82~97 to store diff-ota file.
#define RS_FOTA_ADDRESS	0x520000 //line to block 82
#define FOTA_UPDATE_INFO_BLOCK 99 //store update info, main
#define FOTA_UPDATE_INFO_ADDR 0x630000 //line to block 99
#define FOTA_BACKUP_UPDATE_INFO_BLOCK 98 //store update info, backup
#define FOTA_BACKUP_UPDATE_INFO_ADDR 0x620000 //line to block 98
#elif defined(NBBIRD)
//#define FOTA_FILE_BLOCK 82
#define RS_FOTA_ADDRESS	 0x580000  //store diff delta
//#define FOTA_UPDATE_INFO_BLOCK 99 //store update info, main
#define FOTA_UPDATE_INFO_ADDR 0x6F8000 //store update info, main
//#define FOTA_BACKUP_UPDATE_INFO_BLOCK 98 //store update info, backup
#define FOTA_BACKUP_UPDATE_INFO_ADDR 0x6F0000 //store update info, backup
#endif
#else
#if defined(TANGMAO) || defined(KCT) || defined(CORE_TEK) || defined(WHATER_WORLD)
#define RSDELTA_PATH	L"Z:\\@rsfota\\update.bin"
#define UPINFO_A	L"Z:\\@rsfota\\updateinfo1"
#define UPINFO_B	L"Z:\\@rsfota\\updateinfo2"
#define EXTEND_FILE L"Z:\\@rsfota\\gps_image.bin"
#define MAIN_UPDATE_FILE 1
#define SUB_UPDATE_FILE 2
#define OTA_PACKAGE_FILE 3
#define EXTEND_FILE_INDEX 4
#endif
#endif

/* log related */
#if !defined(RS_DIFF_PACKAGE_ON_FLASH)
#define RSLOG_PATH	L"Z:\\log.log"
static rs_s32 rs_log_handle;
#endif
#endif