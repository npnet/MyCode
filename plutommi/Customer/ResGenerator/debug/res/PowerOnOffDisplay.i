# 1 "temp/res/PowerOnOffDisplay.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/PowerOnOffDisplay.c"
# 98 "temp/res/PowerOnOffDisplay.c"
# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 99 "temp/res/PowerOnOffDisplay.c" 2
# 1 "../customerinc/CustResDef.h" 1
# 195 "../customerinc/CustResDef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/CustResDef.h" 2
# 100 "temp/res/PowerOnOffDisplay.c" 2


<?xml version = "1.0" encoding = "UTF-8"?>

<APP id = "SRV_PHNSET_ONOFF">

    <!-- string ----------------------------------------------------------------------------------->
# 118 "temp/res/PowerOnOffDisplay.c"
    <!-- image ----------------------------------------------------------------------------------->



    <!-- screen ----------------------------------------------------------------------------------->
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON_SYSTEM_LIST"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW_2"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON_USER_VIEW"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_OFF"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_OFF_SYSTEM_LIST"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW_2"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_OFF_USER_VIEW"/>
    <SCREEN id = "SCR_ID_PHNSET_POWER_ON_OFF_LAST"/>

    <!-- screen group id -------------------------------------------------------------------------->
    <SCREEN id = "GRP_ID_POWER_ON_OFF_MAIN"/>
    <SCREEN id = "GRP_ID_POWER_ON_OFF_SYSTEM"/>
    <SCREEN id = "GRP_ID_POWER_ON_OFF_PREVIEW"/>

    <!-- menu ------------------------------------------------------------------------------------->
# 188 "temp/res/PowerOnOffDisplay.c"
    <CACHEDATA type="short" id="NVRAM_RESTORE_POWER_ON_DISPLAY">
        <DEFAULT_VALUE> [0x01, 0x00] </DEFAULT_VALUE>
        <DESCRIPTION> Short Cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="short" id="NVRAM_RESTORE_POWER_OFF_DISPLAY">
        <DEFAULT_VALUE> [0x01, 0x00] </DEFAULT_VALUE>
        <DESCRIPTION> Short Cache </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="short" id="NVRAM_CURRENT_POWER_ON_DISPLAY" restore_flag="TRUE" restore_id="@NVRAM_RESTORE_POWER_ON_DISPLAY">
        <DEFAULT_VALUE> [0x01, 0x00] </DEFAULT_VALUE>
        <DESCRIPTION> current power on display </DESCRIPTION>
    </CACHEDATA>

    <CACHEDATA type="short" id="NVRAM_CURRENT_POWER_OFF_DISPLAY" restore_flag="TRUE" restore_id="@NVRAM_RESTORE_POWER_OFF_DISPLAY">
        <DEFAULT_VALUE> [0x01, 0x00] </DEFAULT_VALUE>
        <DESCRIPTION> current power off display </DESCRIPTION>
    </CACHEDATA>
</APP>
