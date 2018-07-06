#include "Yd_rollover_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"


/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);


void yd_init_rollover_alarm_param(kal_uint8 interval_time)
{
    alarm_msg[TK001_ALARM_ENUM_static_rollover].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_static_rollover].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_static_rollover].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_static_rollover].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_static_rollover].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_static_rollover].compare_time));

    alarm_msg[TK001_ALARM_ENUM_run_rollover].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_run_rollover].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_run_rollover].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_run_rollover].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_run_rollover].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_run_rollover].compare_time));
}

U8 yd_tk001_get_rollover_alarm()
{
    return yd_tk001_info.rollover_alarm;
}

void yd_tk001_set_rollover_alarm(U8 flag)
{
    yd_tk001_info.rollover_alarm = flag;
}

void yd_static_rollover_alarm_handler()
{
    if(bird_get_equmode()!=1)
    {
        alarm_msg[TK001_ALARM_ENUM_static_rollover].upload_onoff = yd_tk001_get_rollover_alarm();
        alarm_msg[TK001_ALARM_ENUM_static_rollover].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_static_rollover]));
    }
    yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_static_rollover]));
    kal_prompt_trace(MOD_SOC,"yd_static_rollover_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_static_rollover].upload_onoff);

    if(alarm_msg[TK001_ALARM_ENUM_static_rollover].upload_onoff)
    {
        bird_soc_sendalarm(TK001_ALARM_ENUM_static_rollover);
    }
}


void yd_run_rollover_alarm_handler()
{
    if(bird_get_equmode()!=1)
    {
        alarm_msg[TK001_ALARM_ENUM_run_rollover].upload_onoff = yd_tk001_get_rollover_alarm();
        alarm_msg[TK001_ALARM_ENUM_run_rollover].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_run_rollover]));
    }
    yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_run_rollover]));
    kal_prompt_trace(MOD_SOC,"yd_run_rollover_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_run_rollover].upload_onoff);

    if(alarm_msg[TK001_ALARM_ENUM_run_rollover].upload_onoff)
    {
        bird_soc_sendalarm(TK001_ALARM_ENUM_run_rollover);
    }
}

void yd_tk001_post_alarm_static_rollover()
{
    yd_run_rollover_alarm_handler();
}

void yd_tk001_post_alarm_run_rollover()
{
    yd_run_rollover_alarm_handler();
}

