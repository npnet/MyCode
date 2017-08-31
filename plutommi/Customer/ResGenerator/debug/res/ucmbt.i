# 1 "temp/res/ucmbt.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/ucmbt.c"

# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 3 "temp/res/ucmbt.c" 2
# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 4 "temp/res/ucmbt.c" 2
# 1 "../../mmi/inc/CustomCfg.h" 1
# 5 "temp/res/ucmbt.c" 2


<?xml version="1.0" encoding="UTF-8"?>


<APP id="APP_UCMBT">

    <!--
    ***************************************************************
    Include Area
    ***************************************************************
    -->
    <INCLUDE file="GlobalResDef.h"/>

    <INCLUDE file="mmi_rp_app_ucmbt_def.h"/>

    <INCLUDE file="ConnectivityResDef.h"/>

    <INCLUDE file="mmi_rp_srv_editor_def.h"/>

    <INCLUDE file="mmi_rp_app_mainmenu_def.h"/>

    <!--
    ***************************************************************
    Callback mgnt Area
    ***************************************************************
    -->

    <!--
    ***************************************************************
    NVRAM Area
    ***************************************************************
    -->

    <!--
    ***************************************************************
    TIMER Area
    ***************************************************************
    -->

    <TIMER id="UCM_NOTIFY_DURATION_TIMER_BT"/>
    <TIMER id="UCM_TONE_AFTER_VIBRATION_BT"/>
    <TIMER id="UCM_MO_PROCESSING_TIMER_BT"/>




          <!--
          ***************************************************************
          Callback mgnt Area
          ***************************************************************
          -->







    <!--
    ***************************************************************
    STRING Area
    ***************************************************************
    -->

    <STRING id="STR_ID_UCM_BT_CALLING"/>
    <STRING id="STR_ID_UCM_BT_REJECT"/>
    <STRING id="STR_ID_UCM_VOICE_INCOMING"/>
    <STRING id="STR_ID_UCM_BT_ANSWER"/>
    <STRING id="STR_ID_UCM_BT_NOT_ALLOW_TO_HOLD"/>
    <STRING id="STR_ID_UCM_BT_NOT_ALLOW_TO_SWAP"/>
    <STRING id="STR_ID_UCM_BT_HOLD"/>
    <STRING id="STR_ID_UCM_BT_RETRIEVE"/>
    <STRING id="STR_ID_UCM_BT_SWAP"/>
    <STRING id="STR_ID_UCM_BT_END_SINGLE"/>
    <STRING id="STR_ID_UCM_BT_END_ALL"/>
    <STRING id="STR_ID_UCM_BT_END_CALL"/>
    <STRING id="STR_ID_UCM_BT_PHONEBOOK"/>
    <STRING id="STR_ID_UCM_BT_MUTE"/>
    <STRING id="STR_ID_UCM_BT_UNMUTE"/>
    <STRING id="STR_ID_UCM_BT_HAND_HELD"/>
    <STRING id="STR_ID_UCM_BT_HAND_FREE"/>
    <STRING id="STR_ID_UCM_BT_ACTIVE"/>
    <STRING id="STR_ID_UCM_BT_PAIRED_HOLD"/>
    <STRING id="STR_ID_UCM_BT_END_ALL_ACTIVE"/>
    <STRING id="STR_ID_UCM_BT_END_ALL_HELD"/>
    <STRING id="STR_ID_UCM_BT_CALL_ENDED"/>
    <STRING id="STR_ID_UCM_BT_PROCESSING"/>
    <STRING id="STR_ID_UCM_EMERGENCY_CALLS_ONLY"/>
    <STRING id="STR_ID_UCM_BT_CALL_WILL_BE_ENDED"/>
    <STRING id="STR_ID_UCM_BT_DTMF"/>
    <STRING id="STR_ID_UCM_BT_SWITCH_AUDIO_PATH"/>
# 108 "temp/res/ucmbt.c"
    <!--
    ***************************************************************
    IMAGE Area
    ***************************************************************
    -->
# 148 "temp/res/ucmbt.c"
    <!--
    ***************************************************************
    SCREEN Area
    ***************************************************************
    -->
    <SCREEN id="GRP_ID_UCM_BT_PRE_MO"/>
    <SCREEN id="GRP_ID_UCM_BT_MO"/>
    <SCREEN id="GRP_ID_UCM_BT_MT"/>
    <SCREEN id="GRP_ID_UCM_BT_INCALL"/>
    <SCREEN id="GRP_ID_UCM_BT_CALL_END"/>




    <SCREEN id="SCR_ID_UCM_BT_OUTGOING"/>
    <SCREEN id="SCR_ID_UCM_BT_INCOMING"/>
    <SCREEN id="SCR_ID_UCM_BT_IN_CALL"/>
    <SCREEN id="SCR_ID_UCM_BT_CALL_END"/>
    <SCREEN id="SCR_ID_UCM_BT_PROCESSING"/>
    <SCREEN id="SCR_ID_UCM_BT_DUMMY"/>
    <SCREEN id="SCR_ID_UCM_BT_DIALLING"/>
    <SCREEN id="SCR_ID_UCM_END_AND_ACCEPT_CONFIRM"/>
    <SCREEN id="SCR_ID_UCM_BT_DTMF_EDITOR"/>



    <!--
    ***************************************************************
    EVENT Area
    ***************************************************************
     -->




    <!--
    ***************************************************************
    MENU Area
    ***************************************************************
    -->

   <MENU id="MENU_ID_UCM_BT_INCALL_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_HOLD</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_RETRIEVE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_SWAP</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_DTMF</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_MUTE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_UNMUTE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_END_SINGLE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_END_ALL</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_SWITCH_AUDIO_PATH</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_BQB_TEST_HOLD</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_BT_INCALL_OPTION_BQB_TEST_END</MENUITEM_ID>




   </MENU>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_HOLD" str="STR_ID_UCM_BT_HOLD"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_RETRIEVE" str="STR_ID_UCM_BT_RETRIEVE"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_SWAP" str="STR_ID_UCM_BT_SWAP"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_DTMF" str="STR_ID_UCM_BT_DTMF"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_MUTE" str="STR_GLOBAL_MUTE"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_UNMUTE" str="STR_ID_UCM_BT_UNMUTE"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_END_SINGLE" str="STR_ID_UCM_BT_END_SINGLE"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_END_ALL" str="STR_ID_UCM_BT_END_ALL"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_SWITCH_AUDIO_PATH" str="STR_ID_UCM_BT_SWITCH_AUDIO_PATH"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_BQB_TEST_HOLD" str="STR_GLOBAL_MODE"/>
   <MENUITEM id="MENU_ID_UCM_BT_INCALL_OPTION_BQB_TEST_END" str="STR_GLOBAL_NORMAL"/>
# 235 "temp/res/ucmbt.c"
 </APP>
