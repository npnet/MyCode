#include "Yd_az_alarm.h"
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
extern void Yd_stop_sleepTimer();
extern U8 bird_get_normal_sleep_state();
extern void yd_tk001_wakeup_function();
extern void yd_tk001_set_record_from_NV();




void yd_init_az_alarm_param(kal_uint8 interval_time)
{
    alarm_msg[TK001_ALARM_ENUM_az].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_az].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_az].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_az].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_az].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_az].compare_time));
}

U8 yd_tk001_get_az_flag()
{
    return yd_tk001_info.az_flag;
}

void yd_tk001_set_az_flag(U8 flag)
{
    yd_tk001_info.az_flag = flag;
}

void yd_az_alarm_handler()
{
    if(bird_get_equmode()!=1)
    {
        alarm_msg[TK001_ALARM_ENUM_az].upload_onoff = 1;
        alarm_msg[TK001_ALARM_ENUM_az].interval_time = 5;/*重启后*/
        alarm_msg[TK001_ALARM_ENUM_az].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_az]));
    }
    yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_az]));
    kal_prompt_trace(MOD_SOC,"yd_az_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_az].upload_onoff);

    if(alarm_msg[TK001_ALARM_ENUM_az].upload_onoff)
    {
        bird_soc_sendalarm(TK001_ALARM_ENUM_az);
    }
    else
    {
        bird_soc_sendpos();
    }
}

void yd_az_alarm_off()
{
    kal_prompt_trace(MOD_SOC,"yd_az_alarm_off");
    Yd_stop_sleepTimer();
    if(bird_get_normal_sleep_state())
    {
        yd_tk001_wakeup_function();/*唤醒*/
    }
    yd_tk001_set_az_flag(0);
    yd_tk001_set_record_from_NV();
    bird_soc_sendpos();
}

void yd_az_alarm_on()
{
    Yd_stop_sleepTimer();
    if(bird_get_normal_sleep_state())
    {
        yd_tk001_wakeup_function();/*唤醒*/
    }

    yd_tk001_set_az_flag(1);
    yd_tk001_set_record_from_NV();

    kal_prompt_trace(MOD_SOC,"yd_az_alarm_on defend flag = %d",yd_tk001_get_defense());
    if(!yd_tk001_get_defense())
        bird_soc_sendpos();
    else
        yd_az_alarm_handler();
}

void yd_uart_az_on()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_az_on az_flag = %d",yd_tk001_get_az_flag());
    if(yd_tk001_get_az_flag()==0)
    {
        yd_az_alarm_on();
    }
}

void yd_uart_az_off()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_az_off az_flag = %d",yd_tk001_get_az_flag());
    if(yd_tk001_get_az_flag()==1)
    {
        yd_az_alarm_off();
    }
}


void yd_set_az_alarm_msg()
{
#ifdef BIRD_ECU_MMI_SUPPORT
    SetProtocolEventHandler(yd_uart_az_on,MSG_ID_YD_TK001_MSG_AZ_OPEN);
    SetProtocolEventHandler(yd_uart_az_off,MSG_ID_YD_TK001_MSG_AZ_CLOSE);
#else
    SetProtocolEventHandler(yd_az_alarm_on,MSG_ID_YD_TK001_MSG_AZ_OPEN);
    SetProtocolEventHandler(yd_az_alarm_off,MSG_ID_YD_TK001_MSG_AZ_CLOSE);
#endif
}

/*=================================  未被使用==============================*/

void yd_defense_az_alarm()
{
    kal_prompt_trace(MOD_SOC," yd_defense_az_alarm az_flag = %d",yd_tk001_get_az_flag());
    if(yd_tk001_get_az_flag())
    {
        yd_az_alarm_handler();
    }
}

