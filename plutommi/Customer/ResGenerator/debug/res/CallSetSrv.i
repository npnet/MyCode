# 1 "temp/res/CallSetSrv.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/CallSetSrv.c"

# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 3 "temp/res/CallSetSrv.c" 2

<?xml version="1.0" encoding="UTF-8"?>

<APP id="SRV_CALLSET">



    <!--Include Area-->

    <!-----------------------------------------------------Event Resource Area----------------------------------------------------->
    <EVENT id="EVT_ID_SRV_CALLSET_ACT_RESPONSE"/>
    <EVENT id="EVT_ID_SRV_CALLSET_LINE_SWITCHED" type="SENDER"/>
    <EVENT id="EVT_ID_SRV_CALLSET_CFU_STATUS_IND" type="SENDER"/>
    <EVENT id="EVT_ID_SRV_CALLSET_CTM_SETTING_CHANGED" type="SENDER"/>
    <EVENT id="EVT_ID_SRV_CALLSET_INT_ACT"/>






    <RECEIVER id="EVT_ID_SRV_SIM_CTRL_AVAILABILITY_CHANGED" proc="srv_callset_hdlr_sim_availability_changed"/>
    <RECEIVER id="EVT_ID_SRV_MODE_SWITCH_FINISH_NOTIFY" proc="srv_callset_hdlr_mode_switched"/>

    <!-----------------------------------------------------String Resource Area----------------------------------------------------->


    <STRING id="STR_ID_SRV_CALLSET_RESULT_FULL"/>
    <STRING id="STR_ID_SRV_CALLSET_RESULT_TOO_LONG"/>
    <STRING id="STR_ID_SRV_CALLSET_RESULT_DUPLICATED"/>
    <STRING id="STR_ID_SRV_CALLSET_RESULT_EMPTY"/>

    <!-----------------------------------------------------Image Resource Area------------------------------------------------------>





    <!-----------------------------------------------------MMI cache Resource Area------------------------------------------------------>
# 92 "temp/res/CallSetSrv.c"
    <CACHEDATA type="byte" id="NVRAM_CALLSET_CTR_MODE" restore_flag="TRUE">
        <DEFAULT_VALUE> [0X00] </DEFAULT_VALUE>
        <DESCRIPTION> Call time reminder mode </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="byte" id="NVRAM_CALLSET_ANSWER_MODE" restore_flag="TRUE">
        <DEFAULT_VALUE> [0X00] </DEFAULT_VALUE>
        <DESCRIPTION> Answer mode </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="byte" id="NVRAM_CALLSET_CALL_TIME_DISPLAY" restore_flag="TRUE">
        <DEFAULT_VALUE> [0X01] </DEFAULT_VALUE>
        <DESCRIPTION> Call time display </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="byte" id="NVRAM_CALLSET_REDIAL" restore_flag="TRUE">
        <DEFAULT_VALUE> [0X00] </DEFAULT_VALUE>
        <DESCRIPTION> Auto redial mode </DESCRIPTION>
    </CACHEDATA>
# 192 "temp/res/CallSetSrv.c"
    <CACHEDATA type="byte" id="NVRAM_CALLSET_QUICK_END_FLAG" restore_flag="TRUE">
        <DEFAULT_VALUE> [0X00] </DEFAULT_VALUE>
        <DESCRIPTION> Auto quick end </DESCRIPTION>
    </CACHEDATA>
# 283 "temp/res/CallSetSrv.c"
    <CACHEDATA type="short" id="NVRAM_CALLSET_CTR_TIME">
        <DEFAULT_VALUE> [0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Call time reminder time </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="short" id="NVRAM_CALLSET_QUICK_END_TIME" restore_flag="TRUE">
        <DEFAULT_VALUE> [0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Auto quick end duration </DESCRIPTION>
    </CACHEDATA>
# 306 "temp/res/CallSetSrv.c"
</APP>
