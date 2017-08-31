# 1 "temp/res/GCallSrv.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "temp/res/GCallSrv.c"

# 1 "../../mmi/inc/mmi_features.h" 1
# 69 "../../mmi/inc/mmi_features.h"
# 1 "../../mmi/inc/MMI_features_switch.h" 1
# 67 "../../mmi/inc/MMI_features_switch.h"
# 1 "../../mmi/inc/MMI_features_type.h" 1
# 68 "../../mmi/inc/MMI_features_switch.h" 2
# 70 "../../mmi/inc/mmi_features.h" 2
# 3 "temp/res/GCallSrv.c" 2
# 1 "../customerinc/custresdef.h" 1
# 195 "../customerinc/custresdef.h"
# 1 "../../mmi/inc/MMI_features.h" 1
# 196 "../customerinc/custresdef.h" 2
# 4 "temp/res/GCallSrv.c" 2

<?xml version="1.0" encoding="UTF-8"?>

<APP id="SRV_GCALL">



    <!--Include Area-->

    <!-----------------------------------------------------Event Resource Area----------------------------------------------------->
    <EVENT id="EVT_ID_SRV_GCALL_ACT" type="SENDER"/>
    <EVENT id="EVT_ID_SRV_GCALL_CNF" type="SENDER"/>

    <RECEIVER id="EVT_ID_SRV_GCALL_ACT" proc="srv_gcall_post_act_evt_hdlr"/>
    <RECEIVER id="EVT_ID_SRV_GCALL_CNF" proc="srv_gcall_post_cnf_evt_hdlr"/>


    <!-----------------------------------------------------String Resource Area----------------------------------------------------->


    <STRING id="STR_ID_GCALL_UNKNOWN_CALLER"/>
    <STRING id="STR_ID_GCALL_RESTRICTED"/>
    <STRING id="STR_ID_GCALL_UNAVAILABLE"/>






    <STRING id="STR_ID_GCALL_UNABLE_TO_CONNECT"/>
    <STRING id="STR_ID_GCALL_OP_FAIL"/>
# 57 "temp/res/GCallSrv.c"
    <STRING id="STR_ID_GCALL_FDN_FAIL"/>
    <STRING id="STR_ID_GCALL_CALL_ENDED"/>



    <STRING id="STR_ID_GCALL_CALL_ABORTED"/>
    <STRING id="STR_ID_GCALL_REDIAL"/>
    <STRING id="STR_ID_GCALL_REDIAL_TIMES"/>
    <STRING id="STR_ID_GCALL_REDIAL_NEXT_ATTEMPT"/>
    <STRING id="STR_ID_GCALL_REDIAL_SEC"/>
    <STRING id="STR_ID_GCALL_REDIAL_MIN"/>



    <STRING id="STR_ID_GCALL_DTMF_CONFIRMATION"/>
# 82 "temp/res/GCallSrv.c"
    <STRING id="STR_ID_GCALL_SS_NFY_CF_ON"/>

    <STRING id="STR_ID_GCALL_SS_NFY_ALL_BARRED"/>
    <STRING id="STR_ID_GCALL_SS_NFY_ALL_OUTGOING_BARRED"/>
    <STRING id="STR_ID_GCALL_SS_NFY_ALL_INCOMING_BARRED"/>
    <STRING id="STR_ID_GCALL_SS_NFY_WTG"/>
    <STRING id="STR_ID_GCALL_SS_NFY_HLD"/>
    <STRING id="STR_ID_GCALL_SS_NFY_RTVD"/>



    <STRING id="STR_ID_GCALL_SS_NFY_CONF"/>



    <STRING id="STR_ID_GCALL_SS_NFY_CUG"/>
    <STRING id="STR_ID_GCALL_SS_NFY_CLIR_REJECTED"/>
    <STRING id="STR_ID_GCALL_SS_NFY_CTFAC"/>
# 112 "temp/res/GCallSrv.c"
    <!-----------------------------------------------------Image Resource Area------------------------------------------------------>





    <IMAGE id="IMG_ID_GCALL_LINE1">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\Outgoing\\\\CL_line1.bmp"</IMAGE>
    <IMAGE id="IMG_ID_GCALL_LINE2">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\Outgoing\\\\CL_line2.bmp"</IMAGE>
# 153 "temp/res/GCallSrv.c"
    <IMAGE id="IMG_ID_GCALL_OUTGOING">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\Connect\\\\CL_NET.PNG"</IMAGE>
# 162 "temp/res/GCallSrv.c"
    <IMAGE id="IMG_ID_GCALL_CALL_ENDED">"..\\\\..\\\\Customer\\\\Images\\\\PLUTO240X320""\\\\MainLCD\\\\Call\\\\ended\\\\CL_END.gif"</IMAGE>





    <!------------------------------------------------------Menu Resource Area------------------------------------------------------>


    <!--Menu Tree Area-->





    <!------------------------------------------------------Other Resource---------------------------------------------------------->
    <TIMER id="SS_NOTIFY_TIMER"/>

</APP>
