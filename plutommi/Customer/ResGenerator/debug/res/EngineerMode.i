# 1 "temp/res/EngineerMode.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/EngineerMode.c"
# 956 "temp/res/EngineerMode.c"
# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 957 "temp/res/EngineerMode.c" 2
# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 958 "temp/res/EngineerMode.c" 2
# 1 "../../../interface/hal/drv_sw_def/drv_sw_features_color.h" 1
# 74 "../../../interface/hal/drv_sw_def/drv_sw_features_color.h"
# 1 "../../../hal/drv_def/drv_features_color.h" 1
# 74 "../../../hal/drv_def/drv_features_color.h"
# 1 "../../../hal/drv_def/drv_features_chip_select.h" 1
# 168 "../../../hal/drv_def/drv_features_chip_select.h"
# 1 "../../../hal/drv_def/drv_features_6261.h" 1
# 169 "../../../hal/drv_def/drv_features_chip_select.h" 2
# 75 "../../../hal/drv_def/drv_features_color.h" 2
# 75 "../../../interface/hal/drv_sw_def/drv_sw_features_color.h" 2
# 959 "temp/res/EngineerMode.c" 2

<?xml version = "1.0" encoding="UTF-8"?>

<APP id = "APP_ENGINEERMODE1">

<!----------------- Include Area ---------------------------------------------->

    <INCLUDE file = "MMI_features.h"/>
    <INCLUDE file = "MMIDataType.h"/>
    <INCLUDE file = "GlobalResDef.h"/>




<!----------------- Common Area ----------------------------------------------->

    <!----------------- Timer Id ------------------------------------------>
    <TIMER id = "EM_NOTIFYDURATION_TIMER"/>
    <TIMER id = "EM_RINGTONE_HIGHLIGHT_TIMER"/>
    <TIMER id = "EM_GPRS_PING_TIMER"/>
    <TIMER id = "EM_GPRS_SOC_DEMO_APP_TIMER"/>

    <!----------------- Event Id ------------------------------------------>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_GLOBAL_PROCESSING">Processing...</STRING>
    <STRING id = "STR_ID_EM_SIM_CARD_IS_NOT_AVAILABLE">This SIM card is not available</STRING>
    <STRING id = "STR_ID_EM_WARNING">Not allow or conflict!</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_SET_LSK">Set</STRING>
    <STRING id = "STR_ID_EM_NOTICE_SUCCESS_REBOO">Done.\nPower off in 3 sec</STRING>
    <STRING id = "STR_ID_EM_GLOBAL_AUTO">Auto</STRING>
    <STRING id = "STR_ID_EM_TRUE">True</STRING>
    <STRING id = "STR_ID_EM_FALSE">False</STRING>





    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "GRP_ID_EM_ROOT"/>
    <SCREEN id = "SCR_ID_EM_CMN_SCRN"/>
    <SCREEN id = "SCR_ID_EM_DUALMODE_SWITCH_PROGRESS"/>

<!----------------- Menu Id ------------------------------------------->
    <MENUSET id = "MENUSET_ON_OFF">
        <MENUITEM id = "MENU_ID_EM_MISC_ON" str = "STR_GLOBAL_ON"/>
        <MENUITEM id = "MENU_ID_EM_MISC_OFF" str = "STR_GLOBAL_OFF"/>
    </MENUSET>

    <MENUSET id = "MENUSET_TRUE_FALSE">
        <MENUITEM id = "MENU_ID_EM_TRUE" str = "STR_ID_EM_TRUE"/>
        <MENUITEM id = "MENU_ID_EM_FALSE" str = "STR_ID_EM_FALSE"/>
    </MENUSET>

<!----------------- Level1 Menu : Mainmenu ------------------------------------>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_APP_NAME">Engineering mode</STRING>

    <!----------------- Image Id ------------------------------------------>
    <IMAGE id = "IMG_ID_EM_MAIN_ICON">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\TitleBar\\\\TB_EM.png"</IMAGE>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_MAIN_MENU" type = "APP_MAIN" str = "STR_ID_EM_APP_NAME">


        <MENUITEM_ID>MENU_ID_EM_NW_SETTING</MENUITEM_ID>


        <MENUITEM_ID>MENU_ID_EM_DEVICE</MENUITEM_ID>

        <MENUITEM_ID>MENU_ID_EM_AUDIO</MENUITEM_ID>


        <MENUITEM_ID>MENU_ID_EM_GPRS_ACT</MENUITEM_ID>

        <MENUITEM_ID>MENU_ID_EM_MISC</MENUITEM_ID>
# 1054 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_RF_TEST_TOOL</MENUITEM_ID>
# 1065 "temp/res/EngineerMode.c"
   </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- Level2 Menu | Network Setting ----------------------------->


    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_NW_SETTING">Network setting</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_NW_SETTING" type = "APP_SUB" str = "STR_ID_EM_NW_SETTING">


        <MENUITEM_ID>MENU_ID_EM_NW_NETWORK_INFO</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_EM_NW_BAND_SELECTION</MENUITEM_ID>
# 1127 "temp/res/EngineerMode.c"
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Network Setting | Network Info ---------------------------->

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_NW_INFO_LOG"/>

    <!----------------- String Id ----------------------------------------->

    <STRING id = "STR_ID_EM_NW_NETWORK_INFO">Network info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_CELL_SEL">RR_CELL_SEL</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_CH_DSCR">RR Ch Dscr</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_CTRL_CHAN">RR ctrl chan</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_RACH_CTRL">RR rach ctrl</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_LAI_INFO">RR lai info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_RADIO_LINK">RR radio link</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_RR_MEAS_REP">RR meas rep</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_CC_CHAN_INFO">CC chan info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_CC_CALL_INFO">CC call info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_CA_LIST_INFO">CA list info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_PLMN_INFO">PLMN info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_GPRS_INFO">GPRS info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_SI2Q_MI_INFO">Si2Q/Mi info</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_TBF_STATUS">TBF status</STRING>
    <STRING id = "STR_ID_EM_NW_INFO_BLOCK_INFO">Block info</STRING>

    <!----------------- Menu Id --------external----------------------------------->
    <MENU id = "MENU_ID_EM_NW_NETWORK_INFO" str = "STR_ID_EM_NW_NETWORK_INFO">
         <MENUITEM id = "MENU_ID_EM_NW_SETTING_SIM_1" str = "STR_GLOBAL_SIM_1"/>
# 1170 "temp/res/EngineerMode.c"
    </MENU>

    <MENU id = "MENU_ID_EM_NW_NETWORK_INFO_LIST" type = "APP_SUB" str = "STR_ID_EM_NW_NETWORK_INFO" flag = "CHECKBOX">
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_CELL_SEL" str = "STR_ID_EM_NW_INFO_RR_CELL_SEL"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_CH_DSCR" str = "STR_ID_EM_NW_INFO_RR_CH_DSCR"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_CTRL_CHAN" str = "STR_ID_EM_NW_INFO_RR_CTRL_CHAN"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_RACH_CTRL" str = "STR_ID_EM_NW_INFO_RR_RACH_CTRL"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_LAI_INFO" str = "STR_ID_EM_NW_INFO_RR_LAI_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_RADIO_LINK" str = "STR_ID_EM_NW_INFO_RR_RADIO_LINK"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_RR_MEAS_REP" str = "STR_ID_EM_NW_INFO_RR_MEAS_REP"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_CC_CHAN_INFO" str = "STR_ID_EM_NW_INFO_CC_CHAN_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_CC_CALL_INFO" str = "STR_ID_EM_NW_INFO_CC_CALL_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_CA_LIST_INFO" str = "STR_ID_EM_NW_INFO_CA_LIST_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_PLMN_INFO" str = "STR_ID_EM_NW_INFO_PLMN_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_GPRS_INFO" str = "STR_ID_EM_NW_INFO_GPRS_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_SI2Q_MI_INFO" str = "STR_ID_EM_NW_INFO_SI2Q_MI_INFO"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_TBF_STATUS" str = "STR_ID_EM_NW_INFO_TBF_STATUS"/>
        <MENUITEM id = "MENU_ID_EM_NW_INFO_BLOCK_INFO" str = "STR_ID_EM_NW_INFO_BLOCK_INFO"/>
   </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Network Setting | Band Selection--------------------------->

    <!----------------- Screen Id ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_NW_BAND_SELECTION">Band selection</STRING>

    <STRING id = "STR_ID_EM_BAND_SEL_ALL_3G_OFF">Not allow all 3G band off</STRING>
    <STRING id = "STR_ID_EM_NW_BAND_SEL_2G">2G band setting</STRING>

    <STRING id = "STR_ID_EM_NW_BAND_SEL_GSM850">850 band</STRING>


    <STRING id = "STR_ID_EM_NW_BAND_SEL_EGSM900">900 band</STRING>


    <STRING id = "STR_ID_EM_NW_BAND_SEL_DCS1800">1800 band</STRING>


    <STRING id = "STR_ID_EM_NW_BAND_SEL_PCS1900">1900 band</STRING>


    <STRING id = "STR_ID_EM_NW_BAND_SEL_850_1800">850/1800</STRING>


    <STRING id = "STR_ID_EM_NW_BAND_SEL_900_1800">900/1800</STRING>



    <STRING id = "STR_ID_EM_NW_BAND_SEL_850_1900">850/1900</STRING>
# 1241 "temp/res/EngineerMode.c"
    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_NW_BAND_SELECTION" type = "APP_SUB" str = "STR_ID_EM_NW_BAND_SELECTION">
        <MENU id = "MENU_ID_EM_NW_BD_SL_SIM_1" str = "STR_GLOBAL_SIM_1">
        </MENU>
# 1257 "temp/res/EngineerMode.c"
    </MENU>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Network Setting | Prefer Rat ------------------------------>
# 1279 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Network Setting | Cell Lock ------------------------------->
# 1295 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Network Setting | Network WCDMA PREFERRED --------------------------->
# 1312 "temp/res/EngineerMode.c"
<!----------------- EM Item End ------------------------------------------->

<!----------------- Network Setting | IVSR -------------------------------->
# 1336 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Network Setting | Network Event --------------------------->
# 1371 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Network Setting | PLMN List ------------------------------->
# 1389 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Network Setting | Service selection ------------------------------->
# 1401 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Network Setting | Gprs Connection ------------------------------->







<!----------------- EM Item End ----------------------------------------------->


<!----------------- Network Setting | 3G_info --------------------------------->
# 1496 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Network Setting | TDD --------------------------------->
# 1577 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->




<!----------------- Level2 Menu | Device -------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEVICE">Device</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEVICE" type = "APP_SUB" str = "STR_ID_EM_DEVICE">

        <MENUITEM_ID>MENU_ID_EM_DEV_SET_UART</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_EM_DEV_GPIO</MENUITEM_ID>


        <MENUITEM_ID>MENU_ID_EM_MM_COLOR</MENUITEM_ID>
# 1616 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_DEV_HW_LEVEL</MENUITEM_ID>







        <MENUITEM_ID>MENU_ID_EM_DEV_DCM_MODE</MENUITEM_ID>



                <MENUITEM_ID>MENU_ID_EM_DEV_BQB_MODE</MENUITEM_ID>
# 1652 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_DEV_MEMORY_TEST</MENUITEM_ID>
# 1684 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_DEV_PMU_REG_SET</MENUITEM_ID>
# 1698 "temp/res/EngineerMode.c"
    </MENU>
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | GPIO ----------------------------------------->

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_DEV_GPIO_LIST"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_GPIO">GPIO</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_LIST_GPO_GPIO">List GPO/GPIO</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_EDIT_GPIO">GPIO editor</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_EDIT_GPO">GPO editor</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_DIRECTION">Direction</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_LEVEL">Current level</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_DIRECTION_OUT">Out</STRING>
    <STRING id = "STR_ID_EM_DEV_GPIO_DIRECTION_IN">In</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_GPIO" type = "APP_SUB" str = "STR_ID_EM_DEV_GPIO">
        <MENUITEM id = "MENU_ID_EM_DEV_GPIO_LIST_GPO_GPIO" str = "STR_ID_EM_DEV_GPIO_LIST_GPO_GPIO"/>
        <MENUITEM id = "MENU_ID_EM_DEV_GPIO_EDIT_GPIO" str = "STR_ID_EM_DEV_GPIO_EDIT_GPIO"/>
        <MENUITEM id = "MENU_ID_EM_DEV_GPIO_EDIT_GPO" str = "STR_ID_EM_DEV_GPIO_EDIT_GPO"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | MM Color ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_MM_COLOR">MM Color Adjust</STRING>
    <STRING id = "STR_ID_EM_DEV_SHARP">Sharpness</STRING>
    <STRING id = "STR_ID_EM_DEV_CONTRAST">Contrast</STRING>
    <STRING id = "STR_ID_EM_DEV_SATURATION">Saturation</STRING>
    <!----------------- Menu Id ------------------------------------------->
    <MENUITEM id = "MENU_ID_EM_MM_COLOR" str = "STR_ID_EM_DEV_MM_COLOR"/>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | Backlight ------------------------------------------>
# 1760 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Device | EINT ----------------------------------------->
# 1775 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | ADC ------------------------------------------>
# 1790 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | CLAM ----------------------------------------->
# 1804 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | HW Level ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_HW_LEVEL">Set default level</STRING> <!-- Engineer Mode Menu Caption String -->

    <STRING id = "STR_ID_EM_DEV_HW_LEVEL_BATTERY">Battery</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_PWM_SET_1">Pwm1</STRING>
    <STRING id = "STR_ID_EM_DEV_PWM_SET_2">Pwm2</STRING>
    <STRING id = "STR_ID_EM_DEV_PWM_SET_3">Pwm3</STRING>

    <STRING id = "STR_ID_EM_DEV_LCD_SET_PARAM_1">Set value</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_HW_LEVEL" type = "APP_SUB" str = "STR_ID_EM_DEV_HW_LEVEL">
        <MENUITEM id = "MENU_ID_EM_DEV_HW_LEVEL_BATTERY" str = "STR_ID_EM_DEV_HW_LEVEL_BATTERY"/>



        <MENUITEM id = "MENU_ID_EM_DEV_HW_LEVEL_PWM1" str = "STR_ID_EM_DEV_PWM_SET_1"/>
        <MENUITEM id = "MENU_ID_EM_DEV_HW_LEVEL_PWM2" str = "STR_ID_EM_DEV_PWM_SET_2"/>
        <MENUITEM id = "MENU_ID_EM_DEV_HW_LEVEL_PWM3" str = "STR_ID_EM_DEV_PWM_SET_3"/>

    </MENU>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | Set UART ----------------------------------------->

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_DEV_UART_POWER_ONOFF_MENU"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_SET_UART">Set UART</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_SET_UART_SETTING">UART setting</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_SET_UART_TST_PS">TST-PS config</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_SET_UART_TST_L1">TST-L1 config</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_SET_UART_PS">PS config</STRING> <!-- Engineer Mode Menu Caption String -->


    <STRING id = "STR_ID_EM_DEV_USB_PORT">USB port</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_USB_PORT2">USB port 2</STRING> <!-- Engineer Mode Menu Caption String -->






    <STRING id = "STR_ID_EM_DEV_UART_1">UART 1</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_UART_2">UART 2</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_UART_3">UART 3</STRING> <!-- Engineer Mode Menu Caption String -->
# 1873 "temp/res/EngineerMode.c"
    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_SET_UART" type = "APP_SUB" str = "STR_ID_EM_DEV_SET_UART">
        <MENUITEM id = "MENU_ID_EM_DEV_UART_SETTING" str = "STR_ID_EM_DEV_SET_UART_SETTING"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | Sleep Mode --------------------------------------->
# 1894 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | DCM Mode ----------------------------------------->


    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_DCM_MODE">DCM mode</STRING> <!-- Engineer Mode Menu Caption String -->

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_DCM_MODE" type = "APP_SUB" str = "STR_ID_EM_DEV_DCM_MODE" flag = "RADIO">
        <MENUITEM id = "MENU_ID_EM_DEV_DCM_MODE_ON" str = "STR_GLOBAL_ON"/>
        <MENUITEM id = "MENU_ID_EM_DEV_DCM_MODE_OFF" str = "STR_GLOBAL_OFF"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | BQB Mode ----------------------------------------->


    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_BQB_MODE">BQB mode</STRING> <!-- Engineer Mode Menu Caption String -->

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_BQB_MODE" type = "APP_SUB" str = "STR_ID_EM_DEV_BQB_MODE" flag = "RADIO">
        <MENUITEM id = "MENU_ID_EM_DEV_BQB_MODE_ON" str = "STR_GLOBAL_ON"/>
        <MENUITEM id = "MENU_ID_EM_DEV_BQB_MODE_OFF" str = "STR_GLOBAL_OFF"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | NAND Format -------------------------------------->
# 1942 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | MTV Setting --------------------------------------->
# 1973 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Device | Video Setting --------------------------------------->
# 2024 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Device | Video Streaming setting --------------------------------------->
# 2046 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Device | USB ---------------------------------------------->
# 2083 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | Memory TEST ---------------------------------------------->


    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_MEMORY_TEST">Memory info</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_MEMORY_TEST_EMI_SETTING">EMI register settings</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_MEMORY_TEST_CUSTOM_SETTING">Custom settings</STRING> <!-- Engineer Mode Menu Caption String -->
    <STRING id = "STR_ID_EM_DEV_MEMORY_TEST_FLASH_INFO">Flash physical info</STRING> <!-- Engineer Mode Menu Caption String -->

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_DEV_MEMORY_TEST" type = "APP_SUB" str = "STR_ID_EM_DEV_MEMORY_TEST">
        <MENUITEM id = "MENU_ID_EM_DEV_MEMORY_TEST_EMI_SETTING" str = "STR_ID_EM_DEV_MEMORY_TEST_EMI_SETTING"/>
        <MENUITEM id = "MENU_ID_EM_DEV_MEMORY_TEST_CUSTOM_SETTING" str = "STR_ID_EM_DEV_MEMORY_TEST_CUSTOM_SETTING"/>
        <MENUITEM id = "MENU_ID_EM_DEV_MEMORY_TEST_FLASH_INFO" str = "STR_ID_EM_DEV_MEMORY_TEST_FLASH_INFO"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | FM Radio ----------------------------------------->
# 2146 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | TST Output Mode----------------------------------->
# 2165 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->




<!----------------- Device | USB Logging -------------------------------------->
# 2190 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | CAM AF Constshot --------------------------------->
# 2294 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->




<!----------------- Device | Mini GPS ----------------------------------------->
# 2475 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | Motion ------------------------------------------->
# 2583 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Device | Speed lcd ------------------------------------>
# 2598 "temp/res/EngineerMode.c"
<!----------------- Speech lcd End ---------------------------------------->

<!----------------- Device | PMU register setting ------------------------->

    <!----------------- Screen Id ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_DEV_PMU_REG_SET">PMU register setting</STRING>
    <STRING id = "STR_ID_EM_DEV_PMU_REG_SET_WRITE">Write</STRING>
    <STRING id = "STR_ID_EM_DEV_PMU_REG_SET_Addr">Address(Hex)</STRING>
    <STRING id = "STR_ID_EM_DEV_PMU_REG_SET_VALUE">Value(Hex)</STRING>
    <STRING id = "STR_ID_EM_DEV_PMU_REG_SET_HIGH_LOW">High\low bit(in order)</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENUITEM id = "MENU_ID_EM_DEV_PMU_REG_SET" str = "STR_ID_EM_DEV_PMU_REG_SET"/>

<!----------------- Speech lcd End ---------------------------------------->

<!----------------- Device | PXS sensor ------------------------------------------->
# 2655 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Device | WLAN EAP Mode ------------------------------------>
# 2686 "temp/res/EngineerMode.c"
<!------------------------- Switch End ---------------------------------------->


<!----------------- Device | NFC ----------------------------------------->
# 2914 "temp/res/EngineerMode.c"
<!----------------- Device | NFC end ------------------------------------->
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Level2 Menu | Audio --------------------------------------->
    <!----------------- Screen Id ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUDIO">Audio</STRING>

    <STRING id = "STR_ID_EM_AUD_16_LELVEL_MAX_GAIN">Max analog gain</STRING>
    <STRING id = "STR_ID_EM_AUD_16_LELVEL_STEP">Step</STRING>


    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_AUDIO" type = "APP_SUB" str = "STR_ID_EM_AUDIO">

        <MENUITEM_ID>MENU_ID_EM_AUD_SET_MODE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_NORMAL_MODE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_LOUDSP_MODE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_HEADSET_MODE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_RING_TONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_SPEECH_ENHANCEMENT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_SET_MAX_SWING</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_EM_AUD_DEBUG_INFO</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_EM_AUD_AUTO_VM_SETTING</MENUITEM_ID>

    </MENU>


<!----------------- EM Item End ----------------------------------------------->


<!----------------- Audio | Set Mode ------------------------------------------>


    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUD_SET_MODE">Set mode</STRING>
    <STRING id = "STR_ID_EM_AUD_SET_VOLUME">Set volume</STRING>
    <STRING id = "STR_ID_EM_AUD_NORMAL_MODE">Normal mode</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR">Fir</STRING>
    <STRING id = "STR_ID_EM_AUD_SPEECH">Speech</STRING>
    <STRING id = "STR_ID_EM_AUD_KEY_TONE">Key tone</STRING>
    <STRING id = "STR_ID_EM_AUD_MELODY">Melody</STRING>
    <STRING id = "STR_ID_EM_AUD_SOUND">FM radio</STRING>
    <STRING id = "STR_ID_EM_AUD_MICROPHONE">Microphone</STRING>
    <STRING id = "STR_ID_EM_AUD_SIDE_TONE">Side tone</STRING>
    <STRING id = "STR_ID_EM_AUD_LOUDSP_MODE">LoudSp mode</STRING>
    <STRING id = "STR_ID_EM_AUD_HEADSET_MODE">Headset mode</STRING>

    <STRING id = "STR_ID_EM_AUD_FIR_0">FIR 0</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR_1">FIR 1</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR_2">FIR 2</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR_3">FIR 3</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR_4">FIR 4</STRING>
    <STRING id = "STR_ID_EM_AUD_FIR_5">FIR 5</STRING>

    <STRING id = "STR_ID_EM_AUD_TV_OUT">TV out</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_0">Volume 0</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_1">Volume 1</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_2">Volume 2</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_3">Volume 3</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_4">Volume 4</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_5">Volume 5</STRING>
    <STRING id = "STR_ID_EM_AUD_VOLUME_6">Volume 6</STRING>
    <STRING id = "STR_ID_EM_AUD_16_LELVEL_VOLUME">16 level setting</STRING>
    <STRING id = "STR_ID_EM_AUD_DIRECT_APPLY">Direct Apply</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENUSET id = "AUDIO_VOL_LIST">
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_0" str = "STR_ID_EM_AUD_VOLUME_0"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_1" str = "STR_ID_EM_AUD_VOLUME_1"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_2" str = "STR_ID_EM_AUD_VOLUME_2"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_3" str = "STR_ID_EM_AUD_VOLUME_3"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_4" str = "STR_ID_EM_AUD_VOLUME_4"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_5" str = "STR_ID_EM_AUD_VOLUME_5"/>
        <MENUITEM id = "MENU_ID_EM_AUD_VOL_6" str = "STR_ID_EM_AUD_VOLUME_6"/>
        <MENUITEM id = "MENU_ID_EM_AUD_16_LELVEL_VOLUME" str = "STR_ID_EM_AUD_16_LELVEL_VOLUME"/>
    </MENUSET>

    <MENU id = "MENU_ID_EM_AUD_SPEECH" type = "APP_SUB" str = "STR_ID_EM_AUD_SPEECH">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_KEY_TONE" type = "APP_SUB" str = "STR_ID_EM_AUD_KEY_TONE">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_MELODY" type = "APP_SUB" str = "STR_ID_EM_AUD_MELODY">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_SOUND" type = "APP_SUB" str = "STR_ID_EM_AUD_SOUND">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_MICROPHONE" type = "APP_SUB" str = "STR_ID_EM_AUD_MICROPHONE">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_SIDE_TONE" type = "APP_SUB" str = "STR_ID_EM_AUD_SIDE_TONE">
        <MENUSET_ID>AUDIO_VOL_LIST</MENUSET_ID>
    </MENU>

    <MENUSET id = "AUDIO_MODE">
        <MENUITEM_ID>MENU_ID_EM_AUD_SPEECH</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_KEY_TONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_MELODY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_SOUND</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_MICROPHONE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_SIDE_TONE</MENUITEM_ID>
    </MENUSET>

    <MENU id = "MENU_ID_EM_AUD_SET_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SET_MODE" flag = "RADIO">
        <MENUITEM id = "MENU_ID_EM_AUD_SET_NORMAL_MODE" str = "STR_ID_EM_AUD_NORMAL_MODE"/>
        <MENUITEM id = "MENU_ID_EM_AUD_SET_LOUDSP_MODE" str = "STR_ID_EM_AUD_LOUDSP_MODE"/>
        <MENUITEM id = "MENU_ID_EM_AUD_SET_HEADSET_MODE" str = "STR_ID_EM_AUD_HEADSET_MODE"/>
    </MENU>

    <MENU id = "MENU_ID_EM_AUD_FIR" type = "APP_SUB" str = "STR_ID_EM_AUD_FIR" flag = "RADIO">
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_0" str = "STR_ID_EM_AUD_FIR_0"/>
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_1" str = "STR_ID_EM_AUD_FIR_1"/>
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_2" str = "STR_ID_EM_AUD_FIR_2"/>
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_3" str = "STR_ID_EM_AUD_FIR_3"/>
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_4" str = "STR_ID_EM_AUD_FIR_4"/>
        <MENUITEM id = "MENU_ID_EM_AUD_FIR_5" str = "STR_ID_EM_AUD_FIR_5"/>
    </MENU>

    <MENU id = "MENU_ID_EM_AUD_NORMAL_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_NORMAL_MODE">
        <MENUITEM_ID>MENU_ID_EM_AUD_FIR</MENUITEM_ID>
        <MENUSET_ID>AUDIO_MODE</MENUSET_ID>
    </MENU>

    <MENU id = "MENU_ID_EM_AUD_LOUDSP_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_LOUDSP_MODE">
        <MENUSET_ID>AUDIO_MODE</MENUSET_ID>
    </MENU>

    <MENU id = "MENU_ID_EM_AUD_HEADSET_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_HEADSET_MODE">
        <MENUSET_ID>AUDIO_MODE</MENUSET_ID>
    </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- Audio | Speech Enhancement -------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUD_SPEECH_ENHANCEMENT">Speech enhancement</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_COMMON_PARA">Common parameters</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_BT_EARPHONE_MODE">BT earphone mode</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_BT_CORDLESS_MODE">BT cordless mode</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_AUX1_MODE">AUX1 mode</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_AUX2_MODE">AUX2 mode</STRING>
    <STRING id = "STR_ID_EM_AUD_SPH_ENH_AUX3_MODE">AUX3 mode</STRING>



    <STRING id = "STR_ID_EM_AUD_PARAMETER">Parameter</STRING>

    <!----------------- Menu Id ----------------------------------------->
    <MENUSET id = "SPH_PARAM_LIST_9">
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_0" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_1" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_2" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_3" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_4" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_5" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_6" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_7" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_8" str = "STR_ID_EM_AUD_PARAMETER"/>
    </MENUSET>

    <MENUSET id = "SPH_PARAM_LIST_16">
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_0</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_1</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_2</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_3</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_4</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_5</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_6</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_7</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_AUD_PARAM_8</MENUITEM_ID>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_9" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_10" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_11" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_12" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_13" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_14" str = "STR_ID_EM_AUD_PARAMETER"/>
        <MENUITEM id = "MENU_ID_EM_AUD_PARAM_15" str = "STR_ID_EM_AUD_PARAMETER"/>
    </MENUSET>



    <MENU id = "MENU_ID_EM_AUD_SPEECH_ENHANCEMENT" type = "APP_SUB" str = "STR_ID_EM_AUD_SPEECH_ENHANCEMENT">
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_COMMON_PARA" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_COMMON_PARA">
            <MENUSET_ID>SPH_PARAM_LIST_9</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_NORMAL_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_NORMAL_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_HEADSET_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_HEADSET_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_LOUDSPK_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_LOUDSP_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_BT_EARPHONE_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_BT_EARPHONE_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_BT_CORDLESS_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_BT_CORDLESS_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_AUX1_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_AUX1_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_AUX2_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_AUX2_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
        <MENU id = "MENU_ID_EM_AUD_SPH_ENH_AUX3_MODE" type = "APP_SUB" str = "STR_ID_EM_AUD_SPH_ENH_AUX3_MODE">
            <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
        </MENU>
    </MENU>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Audio | Ring Tone ----------------------------------------->

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_AUD_RING_TONE"/>
    <SCREEN id = "SCR_ID_EM_AUD_RING_TONE_MENU"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUD_RING_TONE">Ring tone</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_AUD_RING_TONE" type = "APP_SUB" str = "STR_ID_EM_AUD_RING_TONE">
    </MENU>


<!----------------- EM Item End ----------------------------------------------->

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Audio | Debug Info ---------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUD_DEBUG_INFO">Debug info</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_AUD_DEBUG_INFO" type = "APP_SUB" str = "STR_ID_EM_AUD_DEBUG_INFO">
        <MENUSET_ID>SPH_PARAM_LIST_16</MENUSET_ID>
    </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- Audio | Auto VM Setting ----------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_AUD_AUTO_VM_SETTING">Auto record setting</STRING>
    <STRING id = "STR_ID_EM_AUD_VM_SUPPORT">VM Support</STRING>
    <STRING id = "STR_ID_EM_AUD_AUTO_VM_SPH">Auto Speech Record</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_AUD_VM_SUPPORT" type = "APP_SUB" str = "STR_ID_EM_AUD_VM_SUPPORT" flag = "RADIO">
        <MENUSET_ID>MENUSET_ON_OFF</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_AUTO_VM_SPH" type = "APP_SUB" str = "STR_ID_EM_AUD_AUTO_VM_SPH" flag = "RADIO">
        <MENUSET_ID>MENUSET_ON_OFF</MENUSET_ID>
    </MENU>
    <MENU id = "MENU_ID_EM_AUD_AUTO_VM_SETTING" type = "APP_SUB" str = "STR_ID_EM_AUD_AUTO_VM_SETTING">


        <MENUSET_ID>MENUSET_ON_OFF</MENUSET_ID>
    </MENU>



<!----------------- EM Item End ----------------------------------------------->


<!----------------- Level2 Menu | GPRS Act ------------------------------------>

    <!----------------- String Id ----------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_GPRS_ACT">GPRS act</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_GPRS_ACT" type = "APP_SUB" str = "STR_ID_EM_GPRS_ACT">

        <MENUITEM_ID>MENU_ID_EM_GPRS_AUTO_ATTACH</MENUITEM_ID>
# 3229 "temp/res/EngineerMode.c"
    </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- GPRS Act | GPRS Auto Attach ------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_GPRS_AUTO_ATTACH">Attach</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_GPRS_AUTO_ATTACH" type = "APP_SUB" str = "STR_ID_EM_GPRS_AUTO_ATTACH">
    </MENU>


<!----------------- EM Item End ----------------------------------------------->


<!----------------- GPRS Act | GPRS 1st PDP ----------------------------------->
# 3283 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- GPRS Act | GPRS 2nd PDP ----------------------------------->
# 3305 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- GPRS Act | Connection management -------------------------------->
# 3341 "temp/res/EngineerMode.c"
<!----------------- GPRS Act | Fast dormancy ------------------------------->
# 3368 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- EM Item End ----------------------------------------------->


<!----------------- Level2 Menu | Misc.---------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_MISC">Misc.</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_MISC" type = "APP_SUB" str = "STR_ID_EM_MISC">
# 3399 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_MISC_RAM_TEST</MENUITEM_ID>



        <MENUITEM_ID>MENU_ID_EM_MISC_MEMORY_DUMP</MENUITEM_ID>







        <MENUITEM_ID>MENU_ID_EM_MISC_INTERNET_APPLICATION</MENUITEM_ID>
# 3491 "temp/res/EngineerMode.c"
        <MENUITEM_ID>MENU_ID_EM_MRE</MENUITEM_ID>





    </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Auto Answer --------------------------------------->
# 3512 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | High Speed SIM ------------------------------------>
# 3526 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Misc. | Backlight Mode ------------------------------------>
# 3539 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Auto Reset -------------------------------->
# 3552 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Misc. | RAM Test ------------------------------------------>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_MISC_RAM_TEST">RAM test</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENUITEM id = "MENU_ID_EM_MISC_RAM_TEST" str = "STR_ID_EM_MISC_RAM_TEST"/>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Misc. | Memory Dump --------------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_MISC_MEMORY_DUMP">Memory dump</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_MISC_MEMORY_DUMP" type = "APP_SUB" str = "STR_ID_EM_MISC_MEMORY_DUMP" flag = "RADIO">
        <MENUSET_ID>MENUSET_ON_OFF</MENUSET_ID>
    </MENU>

<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | AMR ----------------------------------------------->
# 3592 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Misc. | Internet Application ------------------------------>

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "GRP_ID_EM_MISC_INET_APP_USER_AGENT"/>
    <SCREEN id = "GRP_ID_EM_MISC_INET_APP_ACCEPT_HEADER"/>
    <SCREEN id = "GRP_ID_EM_MISC_INET_APP_MMS_VERSION"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_MISC_INTERNET_APPLICATION">Internet application</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_USER_AGENT">User agent</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_ACCEPT_HEADER">Accept header</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_MMS_VERSION">MMS version</STRING>

    <STRING id = "STR_ID_EM_MISC_INET_APP_AH_WAP_1_2_1">WAP 1.2.1</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_AH_WAP_2_0">WAP 2.0</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_AH_WML">WML</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_AH_XHTML">XHTML</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_AH_HTML">HTML</STRING>

    <STRING id = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_0">MMS 1.0</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_1">MMS 1.1</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_2">MMS 1.2</STRING>

    <STRING id = "STR_ID_EM_MISC_INET_APP_USER_AGENT_STRING">User agent string</STRING>
    <STRING id = "STR_ID_EM_MISC_INET_APP_USER_AGENT_PROFILE">User agent profile</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_MISC_INTERNET_APPLICATION" type = "APP_SUB" str = "STR_ID_EM_MISC_INTERNET_APPLICATION" flag = "CUI_MENU_NORMAL_LIST_WITH_NUMBERED_ICONS">
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_USER_AGENT" str = "STR_ID_EM_MISC_INET_APP_USER_AGENT"/>
        <MENUITEM_ID>MENU_ID_EM_MISC_INET_APP_ACCEPT_HEADER</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_EM_MISC_INET_APP_MMS_VERSION</MENUITEM_ID>
    </MENU>

    <MENU id = "MENU_ID_EM_MISC_INET_APP_ACCEPT_HEADER" type = "APP_SUB" str = "STR_ID_EM_MISC_INET_APP_ACCEPT_HEADER" flag = "CUI_MENU_NORMAL_RADIO_BUTTON_LIST">
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_AH_WAP_1_2_1" str = "STR_ID_EM_MISC_INET_APP_AH_WAP_1_2_1"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_AH_WAP_2_0" str = "STR_ID_EM_MISC_INET_APP_AH_WAP_2_0"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_AH_WML" str = "STR_ID_EM_MISC_INET_APP_AH_WML"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_AH_XHTML" str = "STR_ID_EM_MISC_INET_APP_AH_XHTML"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_AH_HTML" str = "STR_ID_EM_MISC_INET_APP_AH_HTML"/>
    </MENU>

    <MENU id = "MENU_ID_EM_MISC_INET_APP_MMS_VERSION" type = "APP_SUB" str = "STR_ID_EM_MISC_INET_APP_MMS_VERSION" flag = "CUI_MENU_NORMAL_RADIO_BUTTON_LIST">
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_MV_MMS_1_0" str = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_0"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_MV_MMS_1_1" str = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_1"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_MV_MMS_1_2" str = "STR_ID_EM_MISC_INET_APP_MV_MMS_1_2"/>
    </MENU>

    <MENU id = "MENU_ID_EM_MISC_INET_APP_UA_OPTIONS" type = "APP_SUB" str = "STR_GLOBAL_OPTIONS" flag = "CUI_MENU_NORMAL_LIST_WITH_NUMBERED_ICONS">
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_UA_ACTIVATE" str = "STR_GLOBAL_ACTIVATE"/>
        <MENUITEM id = "MENU_ID_EM_MISC_INET_APP_UA_EDIT" str = "STR_GLOBAL_EDIT"/>
    </MENU>

    <!----------------- MMI Caches ------------------------------------------->
    <CACHEDATA type="byte" id="NVRAM_EM_INET_ACTIVE_USER_AGENT">
        <DEFAULT_VALUE> [0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Byte Cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="byte" id="NVRAM_EM_INET_ACTIVE_ACCEPT_HEADER">
        <DEFAULT_VALUE> [0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Byte Cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="byte" id="NVRAM_EM_INET_ACTIVE_MMS_VERSION">
        <DEFAULT_VALUE> [0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Byte Cache </DESCRIPTION>
    </CACHEDATA>

<!----------------- EM Item End ----------------------------------------------->



<!----------------- Misc. | Touch Screen -------------------------------------->
# 3688 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Video High Bit Rate ------------------------------->
# 3702 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Misc. | Cell reselection ---------------------------------->
# 3722 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Cascade Menu ---------------------------------------->
# 3735 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Check Drive --------------------------------------->
# 3752 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Software Tracer ----------------------------------->
# 3766 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | DM Self Register --------------------------------------->
# 3806 "temp/res/EngineerMode.c"
<!----------------- DM Self Register Item End ----------------------------->

<!----------------- Misc. | DM Self Register --------------------------------------->
# 3835 "temp/res/EngineerMode.c"
<!----------------- DM Self Register Item End ----------------------------->


<!----------------- Misc. | DCD Network Paramter ------------------------------>
# 3867 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | FOTA Debug ---------------------------------------->
# 3894 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | DM Add APN ---------------------------------------->
# 3915 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Video Parameter ----------------------------------->
# 3940 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->
# 3975 "temp/res/EngineerMode.c"
<!----------------- Misc. | Software Patch ------------------------------------>
# 3986 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Level3 Menu | JAVA ---------------------------------------->
# 4012 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->
# 4167 "temp/res/EngineerMode.c"
<!----------------- JAVA | Heap Size Setting ---------------------------------->
# 4201 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- JAVA | Performance ---------------------------------------->
# 4229 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- JAVA | JUI Debug ------------------------------------------>
# 4253 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- MED Memory Setting ------------------------------------------>
# 4271 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Misc | Rf desense ----------------------------------------->
# 4288 "temp/res/EngineerMode.c"
<!----------------- Misc | Rf desense end ------------------------------------->


<!----------------- X download ----------------------------------------------->
# 4303 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Level3 Menu | MRE ---------------------------------------->
    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_MRE">MRE</STRING>

    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_MISC_MRE_INFO"></SCREEN>

    <!----------------- Menu Id ------------------------------------------->

    <MENUITEM id = "MENU_ID_EM_MRE" str = "STR_ID_EM_MRE"/>


<!----------------- EM Item End ----------------------------------------------->


<!----------------- Misc. | Power on CPU query ------------------------------->
# 4337 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Level2 Menu | Debug Info ---------------------------------->
# 4362 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Debug Info | Last Exception ------------------------------->
# 4374 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Debug Info | System Stats --------------------------------->
# 4391 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Debug Info | Drive Letter --------------------------------->
# 4408 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Iperf ----------------------------------------------------->
# 4494 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Level2 Menu | Bluetooth ----------------------------------->
# 4527 "temp/res/EngineerMode.c"
<!----------------- Bluetooth Item End ----------------------------------------------->



<!----------------- Bluetooth | General Test --------------------------------->

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_BT_BD_ADDR">BD address</STRING>
# 4637 "temp/res/EngineerMode.c"
<!----------------- Bluetooth | Avrcp connect tg actively -------------------->







<!----------------- Avrcp connect tg actively Item End ------------------------>



<!----------------- Bluetooth | RF Test --------------------------------------->
# 4708 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Bluetooth | Get Chip Version ------------------------------>
# 4721 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Bluetooth | UPF IOT Test ---------------------------------->
# 4735 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->





<!----------------- Level2 Menu | Profiling ----------------------------------->
    <SCREEN id = "GRP_ID_EM_MISC_MEDIA_MEMORY_SETTING"></SCREEN>
# 4794 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Profiling | SWLA ----------------------------->
# 4817 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->






<!----------------- Profiling | GDI Profiling -------------------------->
# 4887 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Profiling | MultiMedia Profiling -------------------------->
# 4955 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Profiling | Framework Profiling -------------------------->
# 4978 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Profiling | UI Benchmark ---------------------------------->
# 4997 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Profiling | Memory Monitor -------------------------------->
# 5030 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Profiling | Memory Stat. ---------------------------------->
# 5054 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Profiling | Venus Debug Panel ----------------------------->
# 5080 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Profiling | FS Test -------------------------------------->
# 5094 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Profiling | CSB Browser ----------------------------------->
# 5110 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Profiling | Multimedia | xxx --------------------------------->
    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "GRP_ID_EM_PROFILING_AUTO_PLAY"></SCREEN>
    <SCREEN id = "GRP_ID_EM_PROFILING_VDOPLY"></SCREEN>
# 5124 "temp/res/EngineerMode.c"
    <SCREEN id = "GRP_ID_EM_PROFILING_CAMERA"></SCREEN>
    <SCREEN id = "GRP_ID_EM_PROFILING_CAMERA_APP_CAPTURE"></SCREEN>
    <SCREEN id = "GRP_ID_EM_PROFILING_IMAGE_DECODE"></SCREEN>
    <SCREEN id = "GRP_ID_EM_PROFILING_VDOREC"></SCREEN>

    <!----------------- String Id ------------------------------------------->
# 5191 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- Profiling | Gesture Test --------------------------------->
# 5204 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- Level2 Menu | RF Test Tool -------------------------------->
    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_DUALMODE_SWITCH_PROGRESS"/>

    <!----------------- String Id ----------------------------------------->

    <STRING id = "STR_ID_EM_RF_TEST_TOOL">RF test tool</STRING>

    <STRING id = "STR_ID_EM_RF_TEST_GSM_CONFIRM_NOTIFY_TEST">Wifi must be off. Will power off after test.</STRING> <!-- Engineer Mode RF Test GSM Menu Caption String-->

    <!----------------- Menu Id ------------------------------------------->

    <MENU id = "MENU_ID_EM_RF_TEST_TOOL" type = "APP_SUB" str = "STR_ID_EM_RF_TEST_TOOL">


        <MENUITEM_ID>MENU_ID_EM_RF_TEST_TOOL_GSM</MENUITEM_ID>





    </MENU>
# 5336 "temp/res/EngineerMode.c"
<!----------------- RF Test Tool | GSM RF Test -------------------------------->

    <!----------------- String Id ----------------------------------------->
    <SCREEN id = "SCR_ID_EM_RF_TEST_GSM_POWER_SCAN_RESULT"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_EM_RF_TEST_TOOL_GSM">GSM</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_TEST">TX test</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_RX_TEST">RX test</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_POWER_SCAN">Power scan</STRING>
    <STRING id = "STR_ID_EM_AUD_SET_SUCCES">Successfully Set!!</STRING>
        <STRING id = "STR_ID_EM_AUD_SET_ERROR">Set Error!!</STRING>


    <STRING id = "STR_ID_EM_RF_GSM_TX_BAND">Band</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BAND_GSM850">GSM850</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BAND_GSM900">GSM900</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BAND_DCS1800">DCS1800</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BAND_PCS1900">PCS1900</STRING>

    <STRING id = "STR_ID_EM_RF_GSM_TX_ARFCN">ARFCN</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_TSC">TSC</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_PCL">PCL</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_AFC">AFC</STRING>

    <STRING id = "STR_ID_EM_RF_GSM_TX_BURST_TYPE">Burst type</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BURST_NB">NB</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_TX_BURST_AB">AB</STRING>

    <STRING id = "STR_ID_EM_RF_GSM_TX_GAIN">Gain</STRING>
    <STRING id = "STR_ID_EM_RF_GSM_RX_BURST_BURST_RX">Burst RX</STRING>

    <!----------------- Menu Id ------------------------------------------->
    <MENU id = "MENU_ID_EM_RF_TEST_TOOL_GSM" str = "STR_ID_EM_RF_TEST_TOOL_GSM"/>

    <MENU id = "MENU_ID_EM_RF_TEST_TOOL_GSM_LIST" type = "APP_SUB" str = "STR_ID_EM_RF_TEST_TOOL_GSM">
        <MENUITEM id = "MENU_ID_EM_RF_GSM_TX_TEST" str = "STR_ID_EM_RF_GSM_TX_TEST"/>
        <MENUITEM id = "MENU_ID_EM_RF_GSM_RX_TEST" str = "STR_ID_EM_RF_GSM_RX_TEST"/>
        <MENUITEM id = "MENU_ID_EM_RF_GSM_POWER_SCAN" str = "STR_ID_EM_RF_GSM_POWER_SCAN"/>
    </MENU>


<!----------------- EM Item End ----------------------------------------------->



<!----------------- RF Test Tool | WIFI RF Test ------------------------------->
# 5509 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- Level2 Menu | VoIP ---------------------------------------->
# 5553 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->



<!----------------- VoIP | VoIP Settings -------------------------------------->






<!----------------- EM Item End ----------------------------------------------->



<!----------------- VoIP | SIP Support ---------------------------------------->
# 5579 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- VoIP | SIP Require ---------------------------------------->
# 5593 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->


<!----------------- VoIP | VoIP Log ------------------------------------------->
# 5608 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->




<!----------------- Audio | CacheData ----------------------------------------->
    <CACHEDATA type="double" id="NVRAM_AUDIO_DEBUG_INFO1" restore_flag="FALSE" restore_id="@NVRAM_CACHE_ID_RESERVED">
        <DEFAULT_VALUE> [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Double cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="double" id="NVRAM_AUDIO_DEBUG_INFO2" restore_flag="FALSE" restore_id="@NVRAM_CACHE_ID_RESERVED">
        <DEFAULT_VALUE> [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Double cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="double" id="NVRAM_AUDIO_DEBUG_INFO3" restore_flag="FALSE" restore_id="@NVRAM_CACHE_ID_RESERVED">
        <DEFAULT_VALUE> [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Double cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="double" id="NVRAM_AUDIO_DEBUG_INFO4" restore_flag="FALSE" restore_id="@NVRAM_CACHE_ID_RESERVED">
        <DEFAULT_VALUE> [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF] </DEFAULT_VALUE>
        <DESCRIPTION> Double cache </DESCRIPTION>
    </CACHEDATA>



<!----------------- EM Item End ----------------------------------------------->

<!----------------- GDI image decode ------------------------------------------>
# 5656 "temp/res/EngineerMode.c"
<!----------------- EM Item End ----------------------------------------------->

<!----------------- XML END --------------------------------------------------->

</APP>
