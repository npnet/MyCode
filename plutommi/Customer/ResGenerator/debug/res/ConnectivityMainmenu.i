# 1 "temp/res/ConnectivityMainmenu.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/ConnectivityMainmenu.c"
# 194 "temp/res/ConnectivityMainmenu.c"
# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 195 "temp/res/ConnectivityMainmenu.c" 2
# 1 "../customerinc/CustResDef.h" 1
# 195 "../customerinc/CustResDef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/CustResDef.h" 2
# 196 "temp/res/ConnectivityMainmenu.c" 2



<?xml version = "1.0" encoding = "UTF-8"?>

<APP id = "APP_CONNECTIVITY_MAINMENU"

     package_name = "native.mtk.connectivity"
     name = "STR_ID_CONNECTIVITY_TEXT"

     img = "IMG_ID_CONNECTIVITY_ICON"

     launch = "mmi_connectivity_app_launch_func"

>
    <INCLUDE file = "GlobalMenuItems.h"/>
    <INCLUDE file = "GlobalResDef.h"/>
    <INCLUDE file = "mmi_rp_app_bluetooth_def.h"/>






    <INCLUDE file = "mmi_rp_app_mainmenu_def.h"/>


    <INCLUDE file = "mmi_rp_app_dataaccount_def.h"/>
# 241 "temp/res/ConnectivityMainmenu.c"
    <!-- string ----------------------------------------------------------------------------------->

    <STRING id = "STR_MENU_ID_CONNECTIVITY_TEXT"/>
    <STRING id = "STR_ID_CONNECTIVITY_TEXT"/>
# 255 "temp/res/ConnectivityMainmenu.c"
    <!-- image ------------------------------------------------------------------------------------>

    <IMAGE id = "IMG_MENU_ID_CONNECTIVITY_ICON">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\SubMenu\\\\Settings\\\\ST_connect.bmp"</IMAGE>

    <IMAGE id = "IMG_ID_CONNECTIVITY_ICON">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\VUI\\\\launcher\\\\Mainmenu\\\\Icons\\\\Connect.png"</IMAGE>
# 268 "temp/res/ConnectivityMainmenu.c"
    <!-- screen ----------------------------------------------------------------------------------->


    <!-- menu ------------------------------------------------------------------------------------->


    <MENU id = "MENU_ID_CONNECTIVITY" type = "APP_MAIN" str = "STR_MENU_ID_CONNECTIVITY_TEXT" parent = "MAIN_MENU_SETTINGS_MENUID" img = "IMG_MENU_ID_CONNECTIVITY_ICON" highlight = "HighlightConnectivity">
# 298 "temp/res/ConnectivityMainmenu.c"
        <MENUITEM_ID>MENU_ID_DATA_CONNECTION_SIM</MENUITEM_ID>


        <MENUITEM_ID>SERVICES_DATA_CONNECT_MAIN_MENU_ID</MENUITEM_ID>
# 331 "temp/res/ConnectivityMainmenu.c"
</MENU>
# 342 "temp/res/ConnectivityMainmenu.c"
</APP>
