# 1 "temp/res/ucm.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/ucm.c"

# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 3 "temp/res/ucm.c" 2


# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 6 "temp/res/ucm.c" 2
# 1 "../../mmi/inc/CustomCfg.h" 1
# 7 "temp/res/ucm.c" 2


<?xml version="1.0" encoding="UTF-8"?>


<APP id="APP_UCM"
# 21 "temp/res/ucm.c"
>

    <!--
    ***************************************************************
    Include Area
    ***************************************************************
    -->
    <INCLUDE file="GlobalResDef.h"/>
    <INCLUDE file="mmi_rp_app_ucm_def.h"/>
    <INCLUDE file="ConnectivityResDef.h"/>
# 41 "temp/res/ucm.c"
    <INCLUDE file="mmi_rp_srv_editor_def.h"/>

    <INCLUDE file="mmi_rp_app_mainmenu_def.h"/>

    <!--
    ***************************************************************
    Callback mgnt Area
    ***************************************************************
    -->
  <RECEIVER id="EVT_ID_SRV_UCM_ACTION_NOTIFY" proc="vm_ivr_handler"/>
        <RECEIVER id="EVT_ID_SRV_UCM_INDICATION" proc="vm_ivr_handler"/>
        <RECEIVER id="EVT_ID_SRV_UCM_STATUS_CHANGE" proc="vm_ivr_handler"/>

        <RECEIVER id="EVT_ID_SRV_MODE_SWITCH_FINISH_NOTIFY" proc="mmi_ucm_flight_mode_processing_hdlr"/>
        <RECEIVER id="EVT_ID_SRV_MODE_SWITCH_PRE_NOTIFY" proc="mmi_ucm_flight_mode_processing_hdlr"/>
        <RECEIVER id="EVT_ID_SRV_UCM_ACTION_NOTIFY" proc="mmi_ucm_act_notify_hdlr"/>
        <RECEIVER id="EVT_ID_SRV_UCM_INDICATION" proc="mmi_ucm_ind_hdlr"/>



    <RECEIVER id="EVT_ID_SRV_UCM_CNF_INDICATION" proc="mmi_ucm_conf_ind_hdlr"/>

    <RECEIVER id="EVT_ID_MDI_AUDIO_SPEECH_STATUS_IND" proc="mmi_ucm_speech_status_ind_hdlr"/>
    <RECEIVER id="EVT_ID_GPIO_EXT_DEV_KEY_1" proc="mmi_ucm_headset_key_hdlr"/>
# 81 "temp/res/ucm.c"
    <RECEIVER id="EVT_ID_SRV_SIM_CTRL_AVAILABILITY_CHANGED" proc="mmi_ucm_sim_availability_changed_hdlr"/>
# 93 "temp/res/ucm.c"
    <RECEIVER id="EVT_ID_GPIO_BEFORE_LCD_SLEEP_IN" proc="mmi_ucm_backlight_turn_off_query_hdlr"/>
# 102 "temp/res/ucm.c"
    <!--
    ***************************************************************
    NVRAM Area
    ***************************************************************
    -->
# 152 "temp/res/ucm.c"
    <!--
    ***************************************************************
    TIMER Area
    ***************************************************************
    -->

    <TIMER id="UCM_NOTIFY_DURATION_TIMER"/>
    <TIMER id="UCM_FTE_AUTO_LOCK_TIMER_ID"/>
    <TIMER id="UCM_VT_FALL_TO_VOICE_NOTIFY_TIMER_ID"/>
    <TIMER id="VT_START_CAMERA_QUERY_TIMER"/>
    <TIMER id="CM_UPDATE_NCENTER_TIMER"/>
    <TIMER id="TIMER_ID_UCM_DELAY_RING"/>


    <!--
    ***************************************************************
    STRING Area
    ***************************************************************
    -->
# 245 "temp/res/ucm.c"
    <STRING id="STR_ID_UCM_DIALER"/>






    <STRING id="STR_ID_UCM_CALLING"/>
# 291 "temp/res/ucm.c"
    <STRING id="STR_ID_UCM_VOICE_ACTIVE"/>
    <STRING id="STR_ID_UCM_VOICE_HOLD"/>





    <STRING id="STR_ID_UCM_REJECT"/>




    <STRING id="STR_ID_UCM_ANSWER"/>
# 313 "temp/res/ucm.c"
    <STRING id="STR_ID_UCM_HAND_HELD"/>
    <STRING id="STR_ID_UCM_HAND_FREE"/>





    <STRING id="STR_ID_UCM_PROCESSING"/>
    <STRING id="STR_ID_UCM_NOT_ALLOW_TO_HOLD"/>
    <STRING id="STR_ID_UCM_NOT_ALLOW_TO_SWAP"/>
    <STRING id="STR_ID_UCM_HOLD"/>
    <STRING id="STR_ID_UCM_RETRIEVE"/>
    <STRING id="STR_ID_UCM_SWAP"/>
    <STRING id="STR_ID_UCM_CONFERENCE"/>
    <STRING id="STR_ID_UCM_SPLIT"/>
    <STRING id="STR_ID_UCM_END_SINGLE"/>
    <STRING id="STR_ID_UCM_END_CONFERENCE"/>
    <STRING id="STR_ID_UCM_END_ALL"/>
    <STRING id="STR_ID_UCM_END_CALL"/>




    <STRING id="STR_ID_UCM_NEW_CALL"/>

    <STRING id="STR_ID_UCM_PHONEBOOK"/>




    <STRING id="STR_ID_UCM_SOUND_RECORDER"/>
# 359 "temp/res/ucm.c"
    <STRING id="STR_ID_UCM_DTMF"/>



    <STRING id="STR_ID_UCM_VOICE_CALL"/>




    <STRING id="STR_ID_UCM_ACTIVE"/>
# 381 "temp/res/ucm.c"
    <STRING id="STR_ID_UCM_END_ALL_ACTIVE"/>
    <STRING id="STR_ID_UCM_END_ALL_HELD"/>
# 451 "temp/res/ucm.c"
    <!--
    ***************************************************************
    IMAGE Area
    ***************************************************************
    -->
# 741 "temp/res/ucm.c"
    <IMAGE id="IMG_ID_UCM_CALL_STATE_ACTIVE">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\Outgoing\\\\CL_ACT.bmp"</IMAGE>
    <IMAGE id="IMG_ID_UCM_CALL_STATE_HOLD">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\Outgoing\\\\CL_HOLD.bmp"</IMAGE>




    <IMAGE id="IMG_ID_UCM_ACTIVE_CALL">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\CallCenter\\\\CC_activecall.bmp"</IMAGE>
# 774 "temp/res/ucm.c"
    <!--
    ***************************************************************
    AUDIO Area
    ***************************************************************
    -->




    <!--
    ***************************************************************
    SCREEN Area
    ***************************************************************
    -->




    <SCREEN id="GRP_ID_UCM_PRE_MO"/>
    <SCREEN id="GRP_ID_UCM_MO"/>
    <SCREEN id="GRP_ID_UCM_MT"/>
    <SCREEN id="GRP_ID_UCM_INCALL"/>
    <SCREEN id="GRP_ID_UCM_VT"/>
    <SCREEN id="GRP_ID_UCM_INT_MARKER"/>
    <SCREEN id="GRP_ID_UCM_MARKER"/>
    <SCREEN id="GRP_ID_UCM_CALL_END"/>
    <SCREEN id="GRP_ID_UCM_AUTO_REDIAL"/>
    <SCREEN id="GRP_ID_UCM_CONFIRM"/>
    <SCREEN id="GRP_ID_UCM_MO_FAIL_POST_ACTION"/>
    <SCREEN id="GRP_ID_UCM_UHA"/>

    <SCREEN id="SCR_ID_UCM_OUTGOING"/>
    <SCREEN id="SCR_ID_UCM_INCOMING"/>
    <SCREEN id="SCR_ID_UCM_IN_CALL"/>
    <SCREEN id="SCR_ID_UCM_DTMF_EDITOR"/>
    <SCREEN id="SCR_ID_UCM_MULTI_HELD_CALL"/>
    <SCREEN id="SCR_ID_UCM_CALL_END"/>
    <SCREEN id="SCR_ID_UCM_VOIP_DIALER"/>
    <SCREEN id="SCR_ID_UCM_VOICE_DIALER"/>
    <SCREEN id="SCR_ID_UCM_TRANSFER"/>
    <SCREEN id="SCR_ID_UCM_DEFLECT"/>
    <SCREEN id="SCR_ID_UCM_CONFIRM"/>
    <SCREEN id="SCR_ID_UCM_PROCESSING"/>
    <SCREEN id="SCR_ID_UCM_AUTO_REDIAL"/>
    <SCREEN id="SCR_ID_UCM_DIAL_CALL_TYPE_MENU"/>



    <SCREEN id="SCR_ID_UCM_BGS_EFFECT_SELECT"/>
    <SCREEN id="SCR_ID_UCM_URIAGENT_CONFIRM"/>
    <SCREEN id="SCR_ID_UCM_SEND_DTMF"/>
    <SCREEN id="SCR_ID_UCM_DUMMY"/>
    <SCREEN id="SCR_ID_UCM_ECT_CONFIRM"/>
    <SCREEN id="SCR_ID_UCM_REJ_BY_SMS_CONFIRM"/>
    <SCREEN id="SCR_ID_UCM_DISCONNECT_SCREEN"/>
    <SCREEN id="SCR_ID_UCM_GRAY_SCREEN"/>
    <SCREEN id="SCR_ID_UCM_MO_FAIL_POST_ACTION"/>
    <SCREEN id="SCR_ID_UCM_VT_INCALL"/>
    <SCREEN id="SCR_ID_UCM_VT_DIALER"/>
    <SCREEN id="SCR_ID_UCM_VT_MO_FAIL_POST_ACTION"/>
    <SCREEN id="SCR_ID_UCM_VT_FALLTO_VOICE_NOTIFY"/>
    <SCREEN id="SCR_ID_UCM_VT_VIDEO_QUALITY"/>
    <SCREEN id="SCR_ID_UCM_VT_PIC_VIEWER"/>


    <!--
    ***************************************************************
    EVENT Area
    ***************************************************************
    <EVENT id="EVT_ID_UCM_DISCONNECTING"/>
    -->



    <!--
    ***************************************************************
    MENU Area
    ***************************************************************
    -->
# 993 "temp/res/ucm.c"
    <MENU id="MENU_ID_UCM_OUTGOING_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                                <MENUITEM_ID>MENU_ID_UCM_OUTGOING_OPTION_DIALER</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_OUTGOING_OPTION_HAND_HELD</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_OUTGOING_OPTION_HAND_FREE</MENUITEM_ID>



    </MENU>

    <MENU id="MENU_ID_UCM_INCOMING_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_ANSWER</MENUITEM_ID>
                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_ANSWER_BY_VOICE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_DIALER</MENUITEM_ID>



                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_REPLACE</MENUITEM_ID>
                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_REDIAL_FROM_OTHER_SIM</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_END_ALL_ACTIVE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_END_ALL_HELD</MENUITEM_ID>
                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_BACKGROUND_SOUND</MENUITEM_ID>



                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_SWITCH_AUDIO_PATH</MENUITEM_ID>
                                        <MENUITEM_ID>MENU_ID_UCM_INCOMING_OPTION_REJ_BY_SMS</MENUITEM_ID>
    </MENU>


    <MENU id="MENU_ID_UCM_INCALL_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_HOLD</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_RETRIEVE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SWAP</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_CONF</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SPLIT</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_END_SINGLE</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_END_CONF</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_END_ALL</MENUITEM_ID>



                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_NEW_CALL</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_MAIN_MENU</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_PHONEBOOK</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_CALLLOG</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SMS</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_BROWSER</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_HOMESCREEN</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_BACKGROUND</MENUITEM_ID>



                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SWITCH_TO_VIDEO_CALL</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SOUND_RECORDER</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_BGSND_CM_OPT_EFFECT</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_MUTE</MENUITEM_ID>



                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_DTMF</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SWITCH_AUDIO_PATH</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_CTM_CONNECT</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_CTM_SETTING</MENUITEM_ID>




                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_SPEAKER_VOLUME</MENUITEM_ID>
   </MENU>


    <MENUITEM id="MENU_ID_UCM_OUTGOING_OPTION_DIALER" str="STR_ID_UCM_DIALER"/>
    <MENUITEM id="MENU_ID_UCM_OUTGOING_OPTION_HAND_HELD" str="STR_ID_UCM_HAND_HELD"/>
    <MENUITEM id="MENU_ID_UCM_OUTGOING_OPTION_HAND_FREE" str="STR_ID_UCM_HAND_FREE"/>
    <MENUITEM id="MENU_ID_UCM_INCOMING_OPTION_ANSWER" str="STR_ID_UCM_ANSWER"/>



    <MENUITEM id="MENU_ID_UCM_INCOMING_OPTION_DIALER" str="STR_ID_UCM_DIALER"/>



    <MENUITEM id="MENU_ID_UCM_INCOMING_OPTION_REPLACE" str="STR_GLOBAL_REPLACE"/>



    <MENUITEM id="MENU_ID_UCM_INCOMING_OPTION_END_ALL_ACTIVE" str="STR_ID_UCM_END_ALL_ACTIVE"/>
    <MENUITEM id="MENU_ID_UCM_INCOMING_OPTION_END_ALL_HELD" str="STR_ID_UCM_END_ALL_HELD"/>
# 1094 "temp/res/ucm.c"
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_HOLD" str="STR_ID_UCM_HOLD"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_RETRIEVE" str="STR_ID_UCM_RETRIEVE"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_SWAP" str="STR_ID_UCM_SWAP"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_CONF" str="STR_ID_UCM_CONFERENCE"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_SPLIT" str="STR_ID_UCM_SPLIT"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_END_SINGLE" str="STR_ID_UCM_END_SINGLE"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_END_CONF" str="STR_ID_UCM_END_CONFERENCE"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_END_ALL" str="STR_ID_UCM_END_ALL"/>




    <MENU id="MENU_ID_UCM_INCALL_OPTION_NEW_CALL" type="OPTION" str="STR_ID_UCM_NEW_CALL">
                                        <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_NEW_CALL_VOICE</MENUITEM_ID>
                                        <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_NEW_CALL_VOIP</MENUITEM_ID>
    </MENU>
# 1121 "temp/res/ucm.c"
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_PHONEBOOK" str="STR_ID_UCM_PHONEBOOK"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_CALLLOG" str="MAIN_MENU_CALL_LOG_TEXT"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_SMS" str="MAIN_MENU_MESSAGES_TEXT"/>
# 1135 "temp/res/ucm.c"
<MENUITEM id="MENU_ID_UCM_INCALL_OPTION_SOUND_RECORDER" str="STR_ID_UCM_SOUND_RECORDER"/>






    <MENU id="MENU_ID_UCM_INCALL_OPTION_MUTE" type="OPTION" str="STR_GLOBAL_MUTE">
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_MUTE_ON</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_MUTE_OFF</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_MUTE_ON" str="STR_GLOBAL_ON"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_MUTE_OFF" str="STR_GLOBAL_OFF"/>



    <MENU id="MENU_ID_UCM_INCALL_OPTION_DTMF" type="OPTION" str="STR_ID_UCM_DTMF">
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_DTMF_ON</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_INCALL_OPTION_DTMF_OFF</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_DTMF_ON" str="STR_GLOBAL_ON"/>
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_DTMF_OFF" str="STR_GLOBAL_OFF"/>
# 1172 "temp/res/ucm.c"
    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_SPEAKER_VOLUME" str="STR_GLOBAL_VOLUME"/>



    <MENUITEM id="MENU_ID_UCM_INCALL_OPTION_NEW_CALL_VOICE" str="STR_ID_UCM_VOICE_CALL"/>



    <MENU id="MENU_ID_UCM_NEW_CALL_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                            <MENUITEM_ID>MENU_ID_UCM_NEW_CALL_OPTION_DIAL</MENUITEM_ID>
                            <MENUITEM_ID>MENU_ID_UCM_NEW_CALL_OPTION_DIAL_SIM1</MENUITEM_ID>
                            <MENUITEM_ID>MENU_ID_UCM_NEW_CALL_OPTION_DIAL_SIM2</MENUITEM_ID>
                            <MENUITEM_ID>MENU_ID_UCM_NEW_CALL_OPTION_SAVE</MENUITEM_ID>
    </MENU>
    <MENUITEM id="MENU_ID_UCM_NEW_CALL_OPTION_DIAL" str="STR_GLOBAL_DIAL"/>




    <MENUITEM id="MENU_ID_UCM_NEW_CALL_OPTION_SAVE" str="STR_GLOBAL_SAVE_TO_PHONEBOOK"/>

    <MENU id="MENU_ID_UCM_IN_CALL_DIALER_OPTION" type="OPTION" str="STR_GLOBAL_OPTIONS">
                            <MENUITEM_ID>MENU_ID_UCM_IN_CALL_DIALER_OPTION_DIAL</MENUITEM_ID>
                            <MENUITEM_ID>MENU_ID_UCM_IN_CALL_DIALER_OPTION_SAVE</MENUITEM_ID>
    </MENU>
     <MENUITEM id="MENU_ID_UCM_IN_CALL_DIALER_OPTION_DIAL" str="STR_GLOBAL_DIAL"/>
     <MENUITEM id="MENU_ID_UCM_IN_CALL_DIALER_OPTION_SAVE" str="STR_GLOBAL_SAVE_TO_PHONEBOOK"/>
# 1223 "temp/res/ucm.c"
    <MENU id="MENU_ID_UCM_CALL_TYPE" str="STR_GLOBAL_DIAL">
                                <MENUITEM_ID>MENU_ID_UCM_CALL_TYPE_VOICE</MENUITEM_ID>



                                <MENUITEM_ID>MENU_ID_UCM_CALL_TYPE_VOIP</MENUITEM_ID>
                                <MENUITEM_ID>MENU_ID_UCM_CALL_TYPE_VIDEO</MENUITEM_ID>
    </MENU>





    <MENUITEM id="MENU_ID_UCM_CALL_TYPE_VOICE" str="STR_ID_UCM_VOICE_CALL" highlight="mmi_ucm_highlight_call_type_voice"/>
# 1276 "temp/res/ucm.c"
</APP>
