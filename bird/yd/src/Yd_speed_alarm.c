#include "Yd_speed_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"

#ifdef RJ_GPS_APP
U8 g_speed_is_normal = 0;
#ifdef BIRD_ECU_MMI_SUPPORT
float g_speed_value=0;
#endif
#endif

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];
#ifdef RJ_GPS_APP
extern RJ_Gps_Position gps_position;
extern RJ_Gps_Position alarm_position;
extern RJ_Gps_Position rj_position;
#endif

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern void bird_soc_sendpos();
extern U8 Yd_get_equstatus();
#ifdef BIRD_ECU_MMI_SUPPORT
extern kal_uint32 ECU_speed();
extern U8 get_uart_connect_status();
#endif


void yd_init_speed_alarm_param(kal_uint8 interval_time)
{
    alarm_msg[TK001_ALARM_ENUM_speed].upload_onoff = 1;
    alarm_msg[TK001_ALARM_ENUM_speed].call_onoff = 0;

    alarm_msg[TK001_ALARM_ENUM_speed].alarm_times = 0;
    alarm_msg[TK001_ALARM_ENUM_speed].interval_time = interval_time;
    memset(&(alarm_msg[TK001_ALARM_ENUM_speed].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_speed].compare_time));

}

U8 bird_get_speed_alarm_flag()
{
    return yd_tk001_info.speed_alarm;
}

void bird_speed_alarm_flag(U8 flag)
{
    if(yd_tk001_info.speed_alarm != flag)
    {
        yd_tk001_info.speed_alarm = flag;
        alarm_msg[TK001_ALARM_ENUM_speed].alarm_times = 0;
    }
}

void bird_speed_alarm_value(U32 value)
{
    yd_tk001_info.speed_int= value;
}

U32 bird_get_speed_alarm_value()
{
    return yd_tk001_info.speed_int;
}

void yd_speed_alarm_handler()
{
    alarm_msg[TK001_ALARM_ENUM_speed].upload_onoff = bird_get_speed_alarm_flag();
    alarm_msg[TK001_ALARM_ENUM_speed].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_speed]));
    if(bird_get_equmode()!=1)
    {
        yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_speed]));
    }
    kal_prompt_trace(MOD_SOC,"yd_speed_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_speed].upload_onoff);

    if(alarm_msg[TK001_ALARM_ENUM_speed].upload_onoff)
    {
        bird_soc_sendalarm(TK001_ALARM_ENUM_speed);
    }
}

void yd_speed_alarm_msg_handler()
{
    kal_prompt_trace(MOD_SOC,"yd_speed_alarm_msg_handler equstatus = %d",Yd_get_equstatus());
    if(Yd_get_equstatus()==0)
    {
        return;
    }
    yd_speed_alarm_handler();
}

void yd_set_speed_alarm_msg()
{
    SetProtocolEventHandler(yd_speed_alarm_msg_handler,MSG_ID_MMI_YD_TK001_MSG_ALARM_SPEED);
}

#ifdef RJ_GPS_APP

/*速度超过临界值报警，一直处于临界值之上不在报警，直到速度降低到临界值之下再次超过*/
void RJ_GPS_speed_handle(float gps_speed)
{
    kal_prompt_trace(MOD_SOC, "RJ_GPS_speed_handle = %d", g_speed_is_normal);
    if(bird_get_speed_alarm_flag())
    {
        if(gps_speed > bird_get_speed_alarm_value())
        {
            if(g_speed_is_normal == 0)/*报警消息控制位*/
            {
                ilm_struct       *rj_ilm;

                g_speed_is_normal = 1;
                if(rj_position.ishavedate==1)
                {
                    alarm_position.la = rj_position.la;
                    alarm_position.lg = rj_position.lg;
                    alarm_position.al = rj_position.al;
                    alarm_position.di = rj_position.di;
                    alarm_position.east_west=rj_position.east_west;
                    alarm_position.north_south=rj_position.north_south;
                    alarm_position.satellite_num=rj_position.satellite_num;
                    alarm_position.satellite_num2=rj_position.satellite_num2;
                    alarm_position.sd=gps_speed;//rj_position.sd;//wtl 20160817
                    alarm_position.ishavedate = 1;
                    DRV_BuildPrimitive(rj_ilm, MOD_MMI, MOD_MMI, MSG_ID_MMI_YD_TK001_MSG_ALARM_SPEED, NULL);
                    msg_send_ext_queue(rj_ilm);
                }
                else
                {
                    alarm_position.ishavedate = 0;
                }
            }
        }
        else
        {
            g_speed_is_normal = 0;
        }
    }
    else
    {
        g_speed_is_normal = 0;
    }	
}

float yd_get_speed()
{
#ifdef BIRD_ECU_MMI_SUPPORT
    return g_speed_value;
#else
    return gps_position.sd;
#endif
}

#ifdef BIRD_ECU_MMI_SUPPORT

void Bird_ecu_speed_handle()
{
    if((ECU_speed()!=0)&&(get_uart_connect_status()))
        g_speed_value = ECU_speed();
    else
        g_speed_value = rj_position.sd;

    kal_prompt_trace(MOD_SOC, "Bird_ecu_speed_handle = %d %d %d %d", ECU_speed(),(U16)rj_position.sd,(U16)g_speed_value,bird_get_speed_alarm_value());
    kal_prompt_trace(MOD_SOC, "Bird_ecu_speed_handle = %d", g_speed_is_normal);

    if(bird_get_speed_alarm_flag())
    {
        if(g_speed_value > bird_get_speed_alarm_value())
        {
            if((g_speed_is_normal == 0)&&(Yd_get_equstatus()==1))/*超过速度设定值，报警*/
            {
                ilm_struct       *rj_ilm;
                g_speed_is_normal = 1;
                if(rj_position.ishavedate==1)
                {
                    alarm_position.la = rj_position.la;
                    alarm_position.lg = rj_position.lg;
                    alarm_position.al = rj_position.al;
                    alarm_position.di = rj_position.di;
                    alarm_position.east_west=rj_position.east_west;
                    alarm_position.north_south=rj_position.north_south;
                    alarm_position.satellite_num=rj_position.satellite_num;
                    alarm_position.satellite_num2=rj_position.satellite_num2;
                    alarm_position.sd=g_speed_value;//rj_position.sd;//wtl 20160817
                    alarm_position.ishavedate = 1;
                    DRV_BuildPrimitive(rj_ilm, MOD_MMI, MOD_MMI, MSG_ID_MMI_YD_TK001_MSG_ALARM_SPEED, NULL);
                    msg_send_ext_queue(rj_ilm);
                }
                else
                {
                    alarm_position.ishavedate = 0;
                    alarm_position.sd=g_speed_value;//rj_position.sd;//wtl 20160817
                    DRV_BuildPrimitive(rj_ilm, MOD_MMI, MOD_MMI, MSG_ID_MMI_YD_TK001_MSG_ALARM_SPEED, NULL);
                    msg_send_ext_queue(rj_ilm);
                }
            }
        }
        else
        {
            g_speed_is_normal = 0;
        }
    }
    else
    {
        g_speed_is_normal = 0;
    }
    RJ_GPS_StartTimer(BIRD_ECU_SPEED_TIMER, 1000, Bird_ecu_speed_handle);
}
#endif

#endif
