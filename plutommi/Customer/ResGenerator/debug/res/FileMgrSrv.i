# 1 "temp/res/FileMgrSrv.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/FileMgrSrv.c"
<?xml version="1.0" encoding="UTF-8"?>

# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 4 "temp/res/FileMgrSrv.c" 2

<APP id="SRV_FILEMANAGER">
    <INCLUDE file="GlobalResDef.h"/>

<!--
**************************************** String section ***************************************
-->
<!-- FS Error -->
# 28 "temp/res/FileMgrSrv.c"
    <STRING id="FMGR_FS_PARAM_ERROR_TEXT"/>
    <STRING id="FMGR_FS_DRIVE_NOT_FOUND_TEXT"/>
    <STRING id="FMGR_FS_TOO_MANY_FILES_TEXT"/>
    <STRING id="FMGR_FS_NO_MORE_FILES_TEXT"/>
    <STRING id="FMGR_FS_WRONG_MEDIA_TEXT"/>
    <STRING id="FMGR_FS_INVALID_FILE_SYSTEM_TEXT"/>
    <STRING id="FMGR_FS_FILE_NOT_FOUND_TEXT"/>
    <STRING id="FMGR_FS_INVALID_FILE_HANDLE_TEXT"/>
    <STRING id="FMGR_FS_UNSUPPORTED_DEVICE_TEXT"/>
    <STRING id="FMGR_FS_UNSUPPORTED_DRIVER_FUNCTION_TEXT"/>
    <STRING id="FMGR_FS_CORRUPTED_PARTITION_TABLE_TEXT"/>
    <STRING id="FMGR_FS_TOO_MANY_DRIVES_TEXT"/>
    <STRING id="FMGR_FS_INVALID_FILE_POS_TEXT"/>
    <STRING id="FMGR_FS_ACCESS_DENIED_TEXT"/>
    <STRING id="FMGR_FS_STRING_BUFFER_TOO_SMALL_TEXT"/>
    <STRING id="FMGR_FS_GENERAL_FAILURE_TEXT"/>
    <STRING id="FMGR_FS_PATH_NOT_FOUND_TEXT"/>
    <STRING id="FMGR_FS_FAT_ALLOC_ERROR_TEXT"/>
    <STRING id="FMGR_FS_ROOT_DIR_FULL_TEXT"/>
    <STRING id="FMGR_FS_TIMEOUT_TEXT"/>
    <STRING id="FMGR_FS_BAD_SECTOR_TEXT"/>
    <STRING id="FMGR_FS_DATA_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MEDIA_CHANGED_TEXT"/>
    <STRING id="FMGR_FS_SECTOR_NOT_FOUND_TEXT"/>
    <STRING id="FMGR_FS_ADDRESS_MARK_NOT_FOUND_TEXT"/>
    <STRING id="FMGR_FS_DRIVE_NOT_READY_TEXT"/>
    <STRING id="FMGR_FS_WRITE_PROTECTION_TEXT"/>
    <STRING id="FMGR_FS_DMA_OVERRUN_TEXT"/>
    <STRING id="FMGR_FS_CRC_ERROR_TEXT"/>
    <STRING id="FMGR_FS_DEVICE_RESOURCE_ERROR_TEXT"/>
    <STRING id="FMGR_FS_INVALID_SECTOR_SIZE_TEXT"/>
    <STRING id="FMGR_FS_OUT_OF_BUFFERS_TEXT"/>
    <STRING id="FMGR_FS_FILE_EXISTS_TEXT"/>
    <STRING id="FMGR_FS_LONG_FILE_POS_TEXT"/>
    <STRING id="FMGR_FS_FILE_TOO_LARGE_TEXT"/>
    <STRING id="FMGR_FS_BAD_DIR_ENTRY_TEXT"/>
    <STRING id="FMGR_FS_READ_ONLY_ERROR_TEXT"/>
    <STRING id="FMGR_FS_SYSTEM_CRASH_TEXT"/>
    <STRING id="FMGR_FS_DEVICE_BUSY_TEXT"/>
    <STRING id="FMGR_FS_ABORTED_ERROR_TEXT"/>
    <STRING id="FMGR_FS_FLASH_MOUNT_ERROR_TEXT"/>
    <STRING id="FMGR_FS_PATH_OVER_LEN_ERROR_TEXT"/>
    <STRING id="FMGR_FS_APP_QUOTA_FULL_TEXT"/>
    <STRING id="FMGR_FS_DEVICE_EXPORTED_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MSDC_MOUNT_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MSDC_READ_SECTOR_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MSDC_WRITE_SECTOR_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MSDC_DISCARD_SECTOR_ERROR_TEXT"/>
    <STRING id="FMGR_FS_MSDC_PRESENT_NOT_READY_TEXT"/>
    <STRING id="FMGR_FS_MSDC_NOT_PRESENT_TEXT"/>
    <STRING id="FMGR_FS_FLASH_ERASE_BUSY_TEXT"/>
    <STRING id="FMGR_FS_LOCK_MUTEX_FAIL_TEXT"/>

<!-- Storage name -->
# 108 "temp/res/FileMgrSrv.c"
    <STRING id="STR_ID_FMGR_CARD_REMOVE_PROMPT"/>
# 124 "temp/res/FileMgrSrv.c"
    <STRING id="STR_FMGR_CARD_MOUNTING"/>

    <!-- Event registration -->
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_ADV_ACTION" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_IN" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_OUT" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_SOFT_MOUNT" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_READY" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_FORMAT" hfile="FileMgrSrvGProt.h" />
        <SENDER id="EVT_ID_SRV_FMGR_NOTIFICATION_SET_DEF_STORAGE" hfile="FileMgrSrvGProt.h" />

        <SENDER id="EVT_ID_SRV_FMGRI_CARD_PLUG_IN" hfile="FileMgrSrvIProt.h" />
        <SENDER id="EVT_ID_SRV_FMGRI_CARD_PLUG_OUT" hfile="FileMgrSrvIProt.h" />
        <SENDER id="EVT_ID_SRV_FMGRI_CARD_DISPLAY_MOUNTING" hfile="FileMgrSrvIProt.h" />





    <RECEIVER id="EVT_ID_USB_ENTER_MS_MODE" proc="srv_fmgr_ms_handler" />
    <RECEIVER id="EVT_ID_USB_EXIT_MS_MODE" proc="srv_fmgr_ms_handler" />

    <!-- NVRAM related -->
    <CACHEDATA type="byte" id="NVRAM_EF_FMGR_SORT_OPTION_LID" restore_flag="TRUE">
        <DEFAULT_VALUE> [0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Byte Cache </DESCRIPTION>
    </CACHEDATA>

</APP>
