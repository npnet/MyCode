# 1 "temp/res/UnifiedMessage.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/UnifiedMessage.c"

# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 3 "temp/res/UnifiedMessage.c" 2

# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 5 "temp/res/UnifiedMessage.c" 2
# 1 "../../mmi/inc/CustomCfg.h" 1
# 6 "temp/res/UnifiedMessage.c" 2


<?xml version="1.0" encoding="UTF-8"?>


<APP id="APP_UNIFIEDMESSAGE"

    package_name="native.mtk.messaging"
    name="STR_ID_UM_MAIN_MENU_MESSAGES_TEXT"

        img="IMG_UM_LAUNCHER_ICON"

    launch="mmi_um_entry_main_message_menu"
    package_proc="mmi_um_package_proc"



    >




 <!--
    ***************************************************************
    Include Area
    ***************************************************************
    -->
    <INCLUDE file="GlobalResDef.h"/>

    <INCLUDE file="mmi_rp_app_sms_def.h"/>
# 49 "temp/res/UnifiedMessage.c"
    <INCLUDE file="MainMenuDef.h"/>
    <INCLUDE file="mmi_rp_app_mainmenu_def.h"/>





    <INCLUDE file="WapResDef.h"/>

    <!--
    ***************************************************************
    STRING Area
    ***************************************************************
    -->
    <STRING id="STR_ID_UM_MAIN_MENU_MESSAGES_TEXT"/>
    <STRING id="STR_UM_WRITE_MSG_ID"/>
    <STRING id="STR_UM_SENT_ID"/>




    <STRING id="STR_UM_SETTING_ID"/>
    <STRING id="STR_UM_MSG_ID"/>
    <STRING id="STR_UM_MSGS_ID"/>







    <STRING id="STR_UM_UNREAD_MSG"/>
    <STRING id="STR_UM_UNREAD_MSG_WITH_NUMBER"/>
    <STRING id="STR_UM_UNREAD_MSGS_WITH_NUMBER"/>

    <STRING id="STR_UM_SEND_FAILED_MSG"/>
    <STRING id="STR_UM_SEND_FAILED_MSG_NUMBER"/>
    <STRING id="STR_UM_SEND_FAILED_MSGS_NUMBER"/>




    <STRING id="STR_UM_SEND_FAILED_SAVE_SUCCESS_ID"/>

    <STRING id="STR_UM_EMPTY_SUBJECT_ID"/>
    <STRING id="STR_UM_EMPTY_CONTENT_ID"/>

    <STRING id="STR_UM_INCOMPLETE_ID"/>
    <STRING id="STR_UM_NOT_SUPPORT_ID"/>
# 145 "temp/res/UnifiedMessage.c"
    <STRING id="STR_UM_EMPTY_MSG_ID"/>
# 184 "temp/res/UnifiedMessage.c"
    <STRING id="STR_UM_CB"/>





    <STRING id="STR_UM_DELETE_FOLDER_ID"/>
# 214 "temp/res/UnifiedMessage.c"
    <!--
    ***************************************************************
    IMAGE Area
    ***************************************************************
    -->
    <IMAGE id="IMG_UM_UNREAD_ICON">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\NCenter\\\\icons\\\\icon_msg_unread.png"</IMAGE>






    <IMAGE id="IMG_UM_LAUNCHER_ICON">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\VUI\\\\Launcher\\\\Mainmenu\\\\Icons\\\\Message.png"</IMAGE>



    <IMAGE id="IMG_UM_MAIN_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_MG.bmp"</IMAGE>

    <IMAGE id="IMG_UM_WRITE_MSG_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_WRITE.bmp"</IMAGE>
    <IMAGE id="IMG_UM_INBOX_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_INBOX.bmp"</IMAGE>
    <IMAGE id="IMG_UM_UNSENT_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_Unsent.bmp"</IMAGE>
    <IMAGE id="IMG_UM_SENT_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_Sent.bmp"</IMAGE>
    <IMAGE id="IMG_UM_DRAFT_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_DraftsBox.bmp"</IMAGE>
# 246 "temp/res/UnifiedMessage.c"
    <IMAGE id="IMG_UM_DELETE_FOLDER_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_Delete.bmp"</IMAGE>



    <IMAGE id="IMG_UM_SETTING_ID">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Submenu\\\\UniMessage\\\\Uni_Set.bmp"</IMAGE>






                <IMAGE id="IMG_UM_ENTRY_SCRN_CAPTION">"..\\\\..\\\\Customer\\\\Images""\\\\EmptyImage.bmp"</IMAGE>
# 279 "temp/res/UnifiedMessage.c"
    <!--
    ***************************************************************
    SCREEN Area
    ***************************************************************
    -->
    <SCREEN id="GRP_ID_UM_ROOT"/>
    <SCREEN id="SCR_ID_UM_START"/>
    <SCREEN id="SCR_ID_UM_MAIN"/>
    <SCREEN id="SCR_ID_UM_MAIN_MESSAGE"/>
    <SCREEN id="SCR_ID_UM_WRITE_MSG"/>
    <SCREEN id="SCR_ID_UM_DELETE_FOLDER"/>



    <SCREEN id="SCR_ID_UM_SETTING"/>
    <SCREEN id="SCR_ID_UM_PROCESSING"/>
    <SCREEN id="SCR_ID_UM_FOLDER"/>
    <SCREEN id="SCR_ID_UM_MULTI_OP"/>
    <SCREEN id="SCR_ID_UM_CONVERSATION_BOX"/>
    <SCREEN id="SCR_ID_UM_MS_CONVERSATION_BOX"/>
    <SCREEN id="SCR_ID_UM_THREAD_MSG"/>
    <SCREEN id="SCR_ID_UM_MS_THREAD_MSG"/>
    <SCREEN id="SCR_ID_UM_CONVERSATION_BOX_SETTING"/>
    <SCREEN id="SCR_ID_UM_PREFER_SIM_SETTING"/>

    <!--
    ***************************************************************
    MENU Area
    ***************************************************************
    -->

    <MENU id="MAIN_MENU_MESSAGES_MENUID" type="APP_MAIN" str="MAIN_MENU_MESSAGES_TEXT" img="MAIN_MENU_MESSAGES_ICON" highlight="highlight_mainmenu_messages"
        shortcut="ON" shortcut_img="MAIN_MENU_MESSAGES_ICON" launch="mmi_um_entry_main_message_menu_with_check">




                                    <MENUITEM_ID>MENU_ID_UM_WRITE_MSG</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_CS_BOX</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_INBOX</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_DRAFT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_UNSENT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_SENT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_ARCHIVE</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_SIM</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_REPORT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_DELETE_FOLDER</MENUITEM_ID>



                                    <MENUITEM_ID>MENU_ID_UM_MMS_V01_MAIN</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_PBOX_MAIN</MENUITEM_ID>

                                    <MENUITEM_ID>MENU_ID_UM_EMAIL_MAIN</MENUITEM_ID>

                                    <MENUITEM_ID>MENU_ID_UM_IMPS_MAIN</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_CB_MAIN</MENUITEM_ID>






                                    <MENUITEM_ID>MENU_ID_UM_TEMPLATE_SMS_ONLY</MENUITEM_ID>





                                    <MENUITEM_ID>MENU_ID_UM_SETTING_SMS_ONLY</MENUITEM_ID>


    </MENU>


    <MENU id="MENU_ID_UM_WRITE_MSG" type="APP_SUB" str="STR_UM_WRITE_MSG_ID" img="IMG_UM_WRITE_MSG_ID" highlight="mmi_um_highlight_write_msg"
     shortcut="ON" shortcut_img="IMG_UM_WRITE_MSG_ID" launch="mmi_um_entry_write_msg_ext">





                                    <MENUITEM_ID>MENU_ID_UM_WRITE_MSG_SMS</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_WRITE_MSG_MMS</MENUITEM_ID></MENU>
# 429 "temp/res/UnifiedMessage.c"
    <MENUITEM id="MENU_ID_UM_INBOX" str="STR_GLOBAL_INBOX" img="IMG_UM_INBOX_ID" highlight="mmi_um_highlight_inbox" shortcut="ON" shortcut_img="IMG_UM_INBOX_ID" launch="mmi_um_entry_inbox"/>




    <MENUITEM id="MENU_ID_UM_DRAFT" str="STR_GLOBAL_DRAFTS" img="IMG_UM_DRAFT_ID" highlight="mmi_um_highlight_draft"/>
    <MENUITEM id="MENU_ID_UM_UNSENT" str="STR_GLOBAL_OUTBOX" img="IMG_UM_UNSENT_ID" highlight="mmi_um_highlight_unsent"/>


    <MENUITEM id="MENU_ID_UM_SENT" str="STR_UM_SENT_ID" img="IMG_UM_SENT_ID" highlight="mmi_um_highlight_sent" shortcut="ON" shortcut_img="IMG_UM_SENT_ID" launch="mmi_um_entry_sent"/>
# 452 "temp/res/UnifiedMessage.c"
    <MENU id="MENU_ID_UM_DEL_ALL_OPT" type="OPTION" str="STR_GLOBAL_DELETE_ALL"></MENU>
# 542 "temp/res/UnifiedMessage.c"
    <MENUITEM id="MENU_ID_UM_CB_MAIN" str="STR_UM_CB" img="IMG_SMS_CB_MENUENTRY"/>
# 555 "temp/res/UnifiedMessage.c"
    <MENU id="MENU_ID_UM_SETTING" type="APP_SUB" str="STR_UM_SETTING_ID" img="IMG_UM_SETTING_ID" highlight="mmi_um_highlight_setting">
                                    <MENUITEM_ID>MENU_ID_UM_SETTING_SMS</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_SETTING_MMS</MENUITEM_ID>



                                    <MENUITEM_ID>MENU_ID_UM_SETTING_CS</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_SETTING_PREFER_SIM</MENUITEM_ID></MENU>



    <MENUITEM id="MENU_ID_UM_SETTING_SMS_ONLY" str="STR_ID_SMS_MESSAGESETUP_MENUENTRY" img="IMG_ID_SMS_MSG_SETTINGS_MENUENTRY"/>
# 577 "temp/res/UnifiedMessage.c"
    <MENUITEM id="MENU_ID_UM_WRITE_MSG_SMS" str="STR_GLOBAL_TEXT_MESSAGE" img="IMG_MESSAGE_UNREAD"/>
# 625 "temp/res/UnifiedMessage.c"
    <MENUITEM id="MENU_ID_UM_SETTING_SMS" str="STR_GLOBAL_TEXT_MESSAGE" img="IMG_MESSAGE_SMS_MENUENTRY"/>
# 661 "temp/res/UnifiedMessage.c"
    <MENU id="MENU_ID_UM_OPT" type="OPTION" str="STR_GLOBAL_OPTIONS">
                                    <MENUITEM_ID>MENU_ID_UM_DEL_ALL_OPT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_MS_OPT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_MS_DELETE_OPT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_FILTER_BY_OPT</MENUITEM_ID>
                                    <MENUITEM_ID>MENU_ID_UM_SORT_BY_OPT</MENUITEM_ID></MENU>



    <EVENT id="EVT_ID_MMI_UM_REFRESH" type="SENDER"/>
    <EVENT id="EVT_ID_MMI_UM_ENTRY_MAIN_MSG" type="SENDER"/>
    <EVENT id="EVT_ID_MMI_UM_ENTRY_FOLDER" type="SENDER"/>

    <RECEIVER id="EVT_ID_SRV_UM_NOTIFY_READY" proc="mmi_um_ui_adp_receive_event"/>
    <RECEIVER id="EVT_ID_SRV_UM_NOTIFY_HIGHLIGHT_MSG" proc="mmi_um_ui_adp_receive_event"/>
    <RECEIVER id="EVT_ID_SRV_UM_NOTIFY_REFRESH" proc="mmi_um_ui_adp_receive_event"/>
    <RECEIVER id="EVT_ID_SRV_SIM_CTRL_UNAVAILABLE" proc="mmi_um_ui_cc_sim_ctrl_unavailable_hdlr"/>
    <RECEIVER id="EVT_ID_SRV_SIM_CTRL_AVAILABLE" proc="mmi_um_ui_cc_sim_ctrl_available_hdlr"/>
    <RECEIVER id="EVT_ID_SRV_SIM_CTRL_AVAILABILITY_CHANGED" proc="mmi_um_ui_cc_sim_status_hdlr"/>
    <RECEIVER id="EVT_ID_SRV_MODE_SWITCH_FINISH_NOTIFY" proc="mmi_um_ui_cc_mode_switch_status_hdlr"/>
    <RECEIVER id="EVT_ID_SRV_SAT_SIM_REFRESH" proc="mmi_um_sim_refresh_event_hdlr"/>




    <TIMER id="MESSAGES_TTS_COUNT_DOWN_TIMER_ID"/>
    <TIMER id="UM_INPROGRESS_TIMER_ID"/>
# 718 "temp/res/UnifiedMessage.c"
</APP>
