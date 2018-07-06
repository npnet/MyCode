#include "Yd_low_power_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"


/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern U8 bird_get_normal_sleep_state();
extern void Lima_set_soc_init_flag(BOOL flag);
extern void Lima_Soc_Dinit();
extern void Bird_soc_conn();


void yd_init_low_power_alarm_param(kal_uint8 interval_time)
{
    alarm_msg[TK001_ALARM_ENUM_low_power].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_low_power].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_low_power].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_low_power].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_low_power].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_low_power].compare_time));

}

U8 bird_get_low_power_alarm_flag()
{
    return yd_tk001_info.negative_voltage_alarm;
}

void bird_set_low_power_alarm_flag(U8 flag)
{
    yd_tk001_info.negative_voltage_alarm = flag;
}

void bird_set_bvl(U32 flag)
{
    yd_tk001_info.nlbv= flag;
}

U32 bird_get_bvl()
{
    return yd_tk001_info.nlbv;
}

void yd_low_power_alarm_handler()
{
    if(bird_get_equmode()!=1)
    {
        alarm_msg[TK001_ALARM_ENUM_low_power].upload_onoff = bird_get_low_power_alarm_flag();
        alarm_msg[TK001_ALARM_ENUM_low_power].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_low_power]));
    }
    yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_low_power]));
    kal_prompt_trace(MOD_SOC,"yd_low_power_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_low_power].upload_onoff);

    if(alarm_msg[TK001_ALARM_ENUM_low_power].upload_onoff)
    {
        if(bird_get_normal_sleep_state()==1)
        {
            Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/
            Lima_Soc_Dinit();/*释放资源，取消Socket请求*/
            Bird_soc_conn();
        }
        bird_soc_sendalarm(TK001_ALARM_ENUM_low_power);
    }
}

void yd_low_power_msg_handler()
{
    yd_low_power_alarm_handler();
}

void yd_set_low_power_alarm_msg()
{
    SetProtocolEventHandler(yd_low_power_msg_handler,MSG_ID_RJ_LOW_VOLT);
}

