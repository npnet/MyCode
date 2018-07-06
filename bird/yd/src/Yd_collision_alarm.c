#include "Yd_collision_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"

/*=============================   使用的外部变量与函数  ====================================*/
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);


void yd_collision_handle()
{
    kal_prompt_trace(MOD_SOC,"yd_collision_handle");

    bird_soc_sendalarm(TK001_ALARM_ENUM_collision);
}

void yd_set_collision_alarm_msg()
{
    SetProtocolEventHandler(yd_collision_handle,MSG_ID_YD_TK001_MSG_NORMAL_COLLISION);
}

