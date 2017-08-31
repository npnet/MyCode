# 1 "temp/res/DialerCui.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/DialerCui.c"
# 88 "temp/res/DialerCui.c"
# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 89 "temp/res/DialerCui.c" 2
# 1 "../customerinc/CustResDef.h" 1
# 195 "../customerinc/CustResDef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/CustResDef.h" 2
# 90 "temp/res/DialerCui.c" 2


<?xml version="1.0" encoding="UTF-8"?>
<APP id="CUI_DIALER">

    <!----- Include Area ------------------------------------------------------>
    <INCLUDE file="GlobalResDef.h,mmi_rp_srv_editor_def.h"/>


    <!----- String Resource Area ---------------------------------------------->
    <STRING id="STR_ID_CUI_DIALER_NO_SPEED_DIAL_NUM"/>
# 133 "temp/res/DialerCui.c"
    <!----- Image Resource Area ----------------------------------------------->


    <!----- Screen Resource Area ---------------------------------------------->
    <SCREEN id="GRP_ID_CUI_DIALER_SEARCH_SETTING"/>
    <SCREEN id="SCR_ID_CUI_DIALER_MAIN"/>
# 151 "temp/res/DialerCui.c"
    <!----- Menu Resource Area ------------------------------------------------>





    <MENU id="MENU_ID_CUI_DIALER_MENU" type="OPTION" str="STR_GLOBAL_OPTIONS">

        <MENUITEM id="MENU_ID_CUI_DIALER_DIAL" str="STR_GLOBAL_DIAL"/>
        <MENUITEM id="MENU_ID_CUI_DIALER_SAVE_CONTACT" str="STR_GLOBAL_SAVE_TO_PHONEBOOK"/>
# 171 "temp/res/DialerCui.c"
    </MENU>
# 200 "temp/res/DialerCui.c"
    <!----- Nvram Data Area ------------------------------------------------>

    <CACHEDATA type="byte" id="NVRAM_IDLE_DIALER_TTS_SETTING" restore_flag="TRUE">
        <DEFAULT_VALUE> [0x01] </DEFAULT_VALUE>
        <DESCRIPTION> TTS setting on / off </DESCRIPTION>
    </CACHEDATA>

    <!----- Timer ID Area -------------------------------------------------->

    <TIMER id="EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMER"/>







</APP>
