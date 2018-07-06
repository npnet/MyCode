#include "Yd_fly_mode_control.h"
#include "Bird_app.h"
#include "Yd_main.h"

/*=============================   使用的外部变量与函数====================================*/
extern void DW02_SWITCH_TO_FLIGHT(MMI_BOOL  is_switch);


void Yd_switch_fly(bool bfly)
{
    DW02_SWITCH_TO_FLIGHT(bfly);
}

void Yd_change_to_normal()
{
    Yd_switch_fly(KAL_FALSE);
    kal_prompt_trace(MOD_SOC,"[Yd_change_to_normal] to_normal");
}

void Yd_fly_mode_change()
{
    kal_prompt_trace(MOD_SOC,"[Yd_fly_mode_change] to_fly");
    Yd_switch_fly(KAL_TRUE);
    StartTimer(BIRD_FLY_MODE_CHANGE, 1000*8, Yd_change_to_normal);
}

void yd_send_fly_mode_change_msg()
{
    ilm_struct *bird_ilm = NULL;

    DRV_BuildPrimitive(bird_ilm, MOD_MMI, MOD_MMI, MSG_ID_YD_TK001_MSG_FLY, NULL);
    msg_send_ext_queue(bird_ilm);
}

void yd_set_fly_mode_change_msg()
{
    SetProtocolEventHandler(Yd_fly_mode_change,MSG_ID_YD_TK001_MSG_FLY);
}


