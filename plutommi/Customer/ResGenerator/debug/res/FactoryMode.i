# 1 "temp/res/FactoryMode.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/FactoryMode.c"
# 240 "temp/res/FactoryMode.c"
# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 241 "temp/res/FactoryMode.c" 2
# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 242 "temp/res/FactoryMode.c" 2





<?xml version="1.0" encoding="UTF-8"?>

<APP id="APP_FACTORYMODE">
    <INCLUDE file = "MMI_features.h"/>
    <INCLUDE file = "MMIDataType.h"/>
    <INCLUDE file = "GlobalResDef.h"/>
    <INCLUDE file = "SettingDefs.h"/>
    <INCLUDE file = "custresdef.h"/>

    <!----------------- String Id ----------------------------------------->
    <STRING id = "STR_ID_FM_ROOT"/>
    <STRING id = "STR_ID_FM_VERSION"/>
    <STRING id = "STR_ID_FM_BBCHIP_VER"/>
    <STRING id = "STR_ID_FM_DSPCODE_VER"/>
    <STRING id = "STR_ID_FM_DSPATCH_VER"/>


    <STRING id = "STR_ID_FM_SW_SUMMARY"/>
    <STRING id = "STR_ID_FM_SW_VER"/>


    <STRING id = "STR_ID_FM_RESOURCE_BIN"/>
    <STRING id = "STR_ID_FM_RESOURCE_LANGPACK"/>
    <STRING id = "STR_ID_FM_RESOURCE_CUSTPACK"/>



    <STRING id = "STR_ID_FM_HW_VER"/>





    <STRING id = "STR_ID_FM_ISN"/>


    <STRING id = "STR_ID_FM_BUILD_TIME"/>


    <STRING id = "STR_ID_FM_LED"/>
    <STRING id = "STR_ID_FM_LED_MAIN"/>



    <STRING id = "STR_ID_FM_LED_KEYPAD"/>

        <STRING id = "STR_ID_FM_LED_STATUS_R"/>


        <STRING id = "STR_ID_FM_LED_STATUS_G"/>


        <STRING id = "STR_ID_FM_LED_STATUS_B"/>

    <STRING id = "STR_ID_FM_LCD"/>


    <STRING id = "STR_ID_FM_KEYPAD"/>



    <STRING id = "STR_ID_FM_LOUDSPK"/>
    <STRING id = "STR_ID_FM_LOUDSPK_AMP_OVER_CURRENT_DETECTION"/>
    <STRING id = "STR_ID_FM_EACHLOOP"/>







    <STRING id = "STR_ID_FM_VIBRATOR"/>
    <STRING id = "STR_ID_FM_VIBRATOR_3_1_FIRST"/>
    <STRING id = "STR_ID_FM_VIBRATOR_3_1_SECOND"/>
    <STRING id = "STR_ID_FM_ADC"/>
    <STRING id = "STR_ID_FM_RECEIVER"/>
    <STRING id = "STR_ID_FM_HEADSET"/>



    <STRING id = "STR_ID_FM_RTC"/>






    <STRING id = "STR_ID_FM_AUTO_TEST_ROOT"/>
    <STRING id = "STR_ID_FM_AUTO_TEST_REPORT"/>
    <STRING id = "STR_ID_FM_AUTO_TEST_RESULT"/>
    <STRING id = "STR_ID_FM_AUTO_TEST_PASS"/>
    <STRING id = "STR_ID_FM_AUTO_TEST_FAIL"/>


    <STRING id = "STR_ID_FM_AUTO_TEST_RETEST"/>

    <STRING id = "STR_ID_FM_AUTO_TEST_ALLDONE"/>
    <STRING id = "STR_ID_FM_AUTO_TEST_ALLPASSED"/>

    <STRING id = "STR_ID_FM_AUTO_TEST_UNFINISHED"/>
    <STRING id = "STR_ID_FM_AUTOTEST_LED_R"/>
    <STRING id = "STR_ID_FM_AUTOTEST_LED_G"/>
    <STRING id = "STR_ID_FM_AUTOTEST_LED_B"/>
    <STRING id = "STR_ID_FM_AUTOTEST_CLAM"/>
    <STRING id = "STR_ID_FM_AUTOTEST_BACKLIGHT"/>
    <STRING id = "STR_ID_FM_AUTOTEST_RECEIVER"/>
    <STRING id = "STR_ID_FM_AUTOTEST_MIC"/>
    <STRING id = "STR_ID_FM_AUTOTEST_SPEAKER"/>
    <STRING id = "STR_ID_FM_AUTOTEST_HEADSET"/>
    <STRING id = "STR_ID_FM_AUTOTEST_MELODY"/>
    <STRING id = "STR_ID_FM_AUTOTEST_VIB"/>
    <STRING id = "STR_ID_FM_AUTOTEST_NAND"/>





    <STRING id = "STR_ID_FM_AUTOTEST_LEFT_SPEAKER"/>
    <STRING id = "STR_ID_FM_AUTOTEST_RIGHT_SPEAKER"/>
# 399 "temp/res/FactoryMode.c"
    <STRING id = "STR_ID_FM_LCD_AUTO"/>
    <STRING id = "STR_ID_FM_LCD_R"/>
    <STRING id = "STR_ID_FM_LCD_G"/>
    <STRING id = "STR_ID_FM_LCD_B"/>
    <STRING id = "STR_ID_FM_LCD_W"/>
    <STRING id = "STR_ID_FM_LCD_BLACK"/>

    <STRING id = "STR_ID_FM_RTC_NOTIFY"/>


        <STRING id = "STR_ID_FM_MTBF"/>
# 483 "temp/res/FactoryMode.c"
        <STRING id = "STR_ID_FM_VBAT">ADC_VBAT</STRING>
        <STRING id = "STR_ID_FM_BTEMP">ADC_VBATTMP</STRING>
        <STRING id = "STR_ID_FM_VAUX">ADC_VAUX</STRING>
        <STRING id = "STR_ID_FM_CURRENT">ADC_VISENSE</STRING>
        <STRING id = "STR_ID_FM_VCHGR">ADC_VCHGR</STRING>



    <!----------------- Menu Id ------------------------------------------->
    <MENU id="MENU_ID_FM_ROOT" type = "APP_SUB" str="STR_ID_FM_ROOT">
# 524 "temp/res/FactoryMode.c"
        <MENUITEM id="MENU_ID_FM_ADC" str="STR_ID_FM_ADC"/>
# 534 "temp/res/FactoryMode.c"
        <MENUITEM id="MENU_ID_FM_RTC" str="STR_ID_FM_RTC"/>
# 593 "temp/res/FactoryMode.c"
        <MENUITEM_ID>MENU_ID_FM_MISC_AUTO_TEST_SETTING</MENUITEM_ID>
# 606 "temp/res/FactoryMode.c"
    </MENU>



    <!----------------- Misc. | Auto Test Setting --------------------------------->

        <!----------------- Screen Id ----------------------------------------->
        <SCREEN id = "SCR_ID_FM_MISC_AUTO_TEST_SETTING"/>
        <SCREEN id = "SCR_ID_FM_MISC_AUTO_TEST_ADD"/>
        <SCREEN id = "SCR_ID_FM_AUTO_TEST_ENTER_PRIORITY"/>
        <SCREEN id = "SCR_ID_FM_MISC_AUTO_TEST_CHANGE_PRIORITY"/>

        <!----------------- String Id ----------------------------------------->
        <STRING id = "STR_ID_FM_MISC_AUTO_TEST_SETTING">Quick test list</STRING>
        <STRING id = "STR_ID_FM_AUTO_TEST_ADD">Add from list</STRING>
        <STRING id = "STR_ID_FM_AUTO_TEST_CHANGE_PRIORITY">Change priority</STRING>

        <STRING id = "STR_ID_FM_AUTO_TEST_ENTER_PRIORITY">Enter priority</STRING>

        <!----------------- Menu Id ------------------------------------------->
        <MENU id = "MENU_ID_FM_MISC_AUTO_TEST_SETTING" type = "APP_SUB" str = "STR_ID_FM_MISC_AUTO_TEST_SETTING">
        </MENU>

        <MENU id = "MENU_ID_FM_AUTO_TEST_OPTIONS" type = "OPTION" str = "STR_GLOBAL_OPTIONS">
            <MENUITEM id = "MENU_ID_FM_AUTO_TEST_ADD" str = "STR_ID_FM_AUTO_TEST_ADD"/>
            <MENUITEM id = "MENU_ID_FM_AUTO_TEST_CHANGE_PRIORITY" str = "STR_ID_FM_AUTO_TEST_CHANGE_PRIORITY"/>
            <MENUITEM id = "MENU_ID_FM_AUTO_TEST_REMOVE" str = "STR_GLOBAL_REMOVE"/>
        </MENU>

    <!----------------- EM Item End ----------------------------------------------->

    <MENU id="MENU_ID_FM_VERSION" type = "APP_SUB" str="STR_ID_FM_VERSION">
        <MENUITEM id="MENU_ID_FM_SW_SUMMARY" str="STR_ID_FM_SW_SUMMARY"/>
        <MENUITEM id="MENU_ID_FM_SW_VER" str="STR_ID_FM_SW_VER"/>



        <MENUITEM id="MENU_ID_FM_ISN" str="STR_ID_FM_ISN"/>
        <MENUITEM id="MENU_ID_FM_BBCHIP_VER" str="STR_ID_FM_BBCHIP_VER"/>
        <MENUITEM id="MENU_ID_FM_DSPCODE_VER" str="STR_ID_FM_DSPCODE_VER"/>
        <MENUITEM id="MENU_ID_FM_DSPATCH_VER" str="STR_ID_FM_DSPATCH_VER"/>
        <MENUITEM id="MENU_ID_FM_HW_VER" str="STR_ID_FM_HW_VER"/>


        <MENUITEM id="MENU_ID_FM_BUILD_TIME" str="STR_ID_FM_BUILD_TIME"/>


    </MENU>


    <MENU id="MENU_ID_FM_RESOURCE_BIN" type = "APP_SUB" str="STR_ID_FM_RESOURCE_BIN">
        <MENUITEM id="MENU_ID_FM_RESOURCE_LANGPACK" str="STR_ID_FM_RESOURCE_LANGPACK"/>
        <MENUITEM id="MENU_ID_FM_RESOURCE_CUSTPACK" str="STR_ID_FM_RESOURCE_CUSTPACK"/>
    </MENU>
# 680 "temp/res/FactoryMode.c"
    <MENU id="MENU_ID_FM_LCD" type = "APP_SUB" str="STR_ID_FM_LCD">
        <MENUITEM id="MENU_ID_FM_LCD_AUTO" str="STR_ID_FM_LCD_AUTO"/>
        <MENUITEM id="MENU_ID_FM_LCD_R" str="STR_ID_FM_LCD_R"/>
        <MENUITEM id="MENU_ID_FM_LCD_G" str="STR_ID_FM_LCD_G"/>
        <MENUITEM id="MENU_ID_FM_LCD_B" str="STR_ID_FM_LCD_B"/>
        <MENUITEM id="MENU_ID_FM_LCD_W" str="STR_ID_FM_LCD_W"/>
        <MENUITEM id="MENU_ID_FM_LCD_BLACK" str="STR_ID_FM_LCD_BLACK"/>
    </MENU>
# 781 "temp/res/FactoryMode.c"
    <!----------------- Screen Id ----------------------------------------->
    <SCREEN id = "SCR_ID_FM_ROOT"/>
    <SCREEN id = "SCR_ID_FM_VERSION"/>
    <SCREEN id = "SCR_ID_FM_VERSION_SUMMARY"/>
    <SCREEN id = "SCR_ID_FM_RESOURCE_BIN"/>
    <SCREEN id = "SCR_ID_FM_RESOURCE_AUDIO"/>
    <SCREEN id = "SCR_ID_FM_RESOURCE_IMAGE"/>
    <SCREEN id = "SCR_ID_FM_RESOURCE_STR"/>
    <SCREEN id = "SCR_ID_FM_RESOURCE_FONT"/>
    <SCREEN id = "SCR_ID_FM_BBCHIP_VER"/>
    <SCREEN id = "SCR_ID_FM_DSPCODE_VER"/>
    <SCREEN id = "SCR_ID_FM_DSPATCH_VER"/>
    <SCREEN id = "SCR_ID_FM_SW_VER"/>
    <SCREEN id = "SCR_ID_FM_HW_VER"/>
    <SCREEN id = "SCR_ID_FM_MELODY_VER"/>
    <SCREEN id = "SCR_ID_FM_ISN"/>
    <SCREEN id = "SCR_ID_FM_BUILD_TIME"/>
    <SCREEN id = "SCR_ID_FM_LED"/>
    <SCREEN id = "SCR_ID_FM_LCD"/>
    <SCREEN id = "SCR_ID_FM_LCD_AUTO"/>
    <SCREEN id = "SCR_ID_FM_LCD_R"/>
    <SCREEN id = "SCR_ID_FM_LCD_G"/>
    <SCREEN id = "SCR_ID_FM_LCD_B"/>
    <SCREEN id = "SCR_ID_FM_LCD_W"/>
    <SCREEN id = "SCR_ID_FM_LCD_CONTRAST"/>
    <SCREEN id = "SCR_ID_FM_LCD_CONTRAST_MAINLCD"/>
    <SCREEN id = "SCR_ID_FM_LCD_CONTRAST_SUBLCD"/>
    <SCREEN id = "SCR_ID_FM_KEYPAD"/>
    <SCREEN id = "SCR_ID_FM_LOUDSPK"/>
    <SCREEN id = "SCR_ID_FM_EACHLOOP"/>
    <SCREEN id = "SCR_ID_FM_VIBRATOR"/>
    <SCREEN id = "SCR_ID_FM_ADC"/>
    <SCREEN id = "SCR_ID_FM_RECEIVER"/>
    <SCREEN id = "SCR_ID_FM_RTC"/>
    <SCREEN id = "SCR_ID_FM_HEADSET"/>
    <SCREEN id = "SCR_ID_FM_CHARGER"/>
    <SCREEN id = "SCR_ID_FM_RINGTONE"/>
    <SCREEN id = "SCR_ID_FM_AUTO_TEST_ROOT"/>
    <SCREEN id = "SCR_ID_FM_AUTO_TEST_REPORT"/>
    <SCREEN id = "SCR_ID_FM_AUTO_TEST_SUMMARY"/>
    <SCREEN id = "SCR_ID_FM_AUTO_TEST_RESULT"/>
    <SCREEN id = "SCR_ID_FM_AUTO_TEST_START"/>
    <SCREEN id = "SCR_ID_FM_MEMORY_CARD"/>
    <SCREEN id = "SCR_ID_FM_NAND_FLASH"/>
    <SCREEN id = "SCR_ID_FM_CAMERA_TEST"/>
    <SCREEN id = "SCR_ID_FM_CAMERA_TEST_DUMMY"/>
    <SCREEN id = "SCR_ID_FM_CAMERA"/>
    <SCREEN id = "SCR_ID_FM_CAMERA_SENSOR_SETTING"/>
    <SCREEN id = "SCR_ID_FM_CAMERA_ISP_SETTING"/>
    <SCREEN id = "SCR_ID_FM_SETCAMERAPARA"/>
    <SCREEN id = "SCR_ID_FM_MTBF"/>

    <SCREEN id = "SCR_ID_FM_MOTION_CALI"/>





    <SCREEN id = "SCR_ID_FM_PARALLEL_PEN"/>
    <SCREEN id = "SCR_ID_FM_N_CROSS_M_PEN"/>

    <SCREEN id = "SCR_ID_FM_FM_RADIO"/>
    <SCREEN id = "SCR_ID_FM_FM_RADIO_CHANNEL_TEST"/>
    <SCREEN id = "SCR_ID_FM_KEYPAD_TEST"/>
    <SCREEN id = "SCR_ID_FM_KEYPAD_TEST_RESULT"/>
    <SCREEN id = "SCR_ID_FM_PXS_RAW_DATA"/>
    <SCREEN id = "SCR_ID_FM_TOUCH_PANEL"/>
    <SCREEN id = "GRP_ID_FM_ROOT"/>
    <SCREEN id = "GRP_ID_FM_TOUCH_PANEL"/>

                <SCREEN id = "SCR_ID_FM_DUAL_TOUCH_CALIBRATION"/>
    <SCREEN id = "GRP_ID_ID_FM_DUAL_TOUCH_CALIBRATION"/>
# 870 "temp/res/FactoryMode.c"
        <TIMER id = "FM_LCD_COLOR_CHANGE_TIMER"/>
        <TIMER id = "FM_AUTO_TEST_COMMNON_TIMER"/>

        <TIMER id = "FM_RTC_START_TIMER"/>
        <TIMER id = "FM_VIB_TIMER"/>
        <TIMER id = "FM_GENERAL_TIMER"/>






</APP>
