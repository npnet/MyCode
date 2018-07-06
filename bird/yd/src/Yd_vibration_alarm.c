#include "Yd_vibration_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern U8 yd_tk001_get_defense();
extern void bird_soc_sendpos();
extern U8 bird_get_VIBCALL();
extern void yd_tk001_CallOwer();


void yd_init_vibration_alarm_param(kal_uint8 interval_time)
{

    alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_vibration].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_vibration].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_vibration].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_vibration].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_vibration].compare_time));
}

U16 bird_get_vibration_running_alarm_level()
{
    return yd_tk001_info.vibration_value.vibration_level;
}

U16 bird_get_vibration_running_alarm_times_value()
{
    return yd_tk001_info.vibration_value.vibration_times;
}

void bird_vibration_running_alarm_value(U16 argv1,U16 argv2)
{
    yd_tk001_info.vibration_value.vibration_level = argv1;
    yd_tk001_info.vibration_value.vibration_times = argv2;
}

void bird_set_vibration_sensitivity_value(U8 level)
{
    yd_tk001_info.sensitivity_level = level;

}

U8 bird_get_vibration_sensitivity_value()
{
    return yd_tk001_info.sensitivity_level;
}

void bird_set_vibration_running_alarm_flag(U8 flag)
{
    if(yd_tk001_info.vibration_running_alarm != flag)
    {
        yd_tk001_info.vibration_running_alarm = flag;
        alarm_msg[TK001_ALARM_ENUM_vibration].alarm_times = 0;
    }
}

U8 bird_get_vibration_running_alarm_flag()
{
    return yd_tk001_info.vibration_running_alarm;
}

void yd_vibration_alarm_handler()
{
    alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff = bird_get_vibration_running_alarm_flag();
    alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff = yd_upload_judge_function(&alarm_msg[TK001_ALARM_ENUM_vibration]);
    if(bird_get_equmode()!=1)
    {
        yd_set_compare_time_function(&alarm_msg[TK001_ALARM_ENUM_vibration]);
    }

    alarm_msg[TK001_ALARM_ENUM_ydcall].upload_onoff = bird_get_VIBCALL();
    alarm_msg[TK001_ALARM_ENUM_ydcall].call_onoff = yd_upload_judge_function(&alarm_msg[TK001_ALARM_ENUM_ydcall]);

    kal_prompt_trace(MOD_SOC,"yd_vibration_alarm_handler ydcall_callonoff = %d",alarm_msg[TK001_ALARM_ENUM_ydcall].call_onoff);
    if(alarm_msg[TK001_ALARM_ENUM_ydcall].call_onoff)
    {
        yd_tk001_CallOwer();
        alarm_msg[TK001_ALARM_ENUM_ydcall].upload_onoff = alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff;
        yd_set_compare_time_function(&alarm_msg[TK001_ALARM_ENUM_ydcall]);
    }

    kal_prompt_trace(MOD_SOC,"yd_vibration_alarm_handler vibration_upload = %d",alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff);
    if(alarm_msg[TK001_ALARM_ENUM_vibration].upload_onoff)
    {
        bird_soc_sendalarm(TK001_ALARM_ENUM_vibration);
    }
}

void yd_tk001_post_alarm_vibration_ext()
{
    g_vibration_is_timer_start = 0;

    kal_prompt_trace(MOD_SOC,"yd_tk001_post_alarm_vibration_ext defense = %d",yd_tk001_get_defense());
    if(!yd_tk001_get_defense())
    {
        return;
    }

    yd_vibration_alarm_handler();
}

void yd_tk001_alarm_vibration_count()
{
    g_vibration_count = 0;
    g_vibration_is_timer_count_start = 0;
}

#ifdef BIRD_ECU_MMI_SUPPORT

void yd_uart_post_vibration_alarm()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_post_vibration_alarm defense = %d",yd_tk001_get_defense());

    if(!yd_tk001_get_defense())
    {
        return;
    }

    yd_vibration_alarm_handler();
}

void yd_set_uart_vibration_alarm_msg()
{
    SetProtocolEventHandler(yd_uart_post_vibration_alarm,MSG_ID_YD_TK001_MSG_VIBRATE_IS_ON);
}

#endif

