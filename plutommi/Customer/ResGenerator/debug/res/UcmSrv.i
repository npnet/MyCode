# 1 "temp/res/UcmSrv.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/UcmSrv.c"
<?xml version="1.0" encoding="UTF-8"?>

# 1 "../../mmi/inc/MMI_features.h" 1
# 69 "../../mmi/inc/MMI_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/MMI_features.h" 2
# 4 "temp/res/UcmSrv.c" 2

<APP id="SRV_UCM">
    <INCLUDE file="GlobalResDef.h"/>

<!--
**************************************** String section ***************************************
-->
# 30 "temp/res/UcmSrv.c"
<!--
**************************************** Other Resource ***************************************
-->
    <TIMER id="UCM_CONFIRM_TIMER_ID"/>
    <TIMER id="UCM_AUTO_REDIAL_TIMER"/>
    <TIMER id="UCM_SEND_DTMF_TIMER"/>
    <TIMER id="UCM_AUTO_ANSWER_MACHINE_TIMER"/>
    <TIMER id="UCM_AUTO_ANSWER_MACHINE_END_TIMER"/>




    <RECEIVER id="EVT_ID_SRV_SHUTDOWN_HANDLER_RESET" proc="srv_ucm_shutdown_reset_hdlr"/>

</APP>
