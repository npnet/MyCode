#include "Yd_reboot_control.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_n_is_rest = 0;
//U8 g_n_restartflag = 0;

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;

extern void yd_tk001_set_record_from_NV(void);
#ifdef DW02_PROJECT
extern void rj_reset(void);
#endif


void yd_init_reboot_control_param(void)
{
    yd_tk001_info.reboot = 0;
}

U8 yd_tk001_get_reboot_flag(void)
{
    return yd_tk001_info.reboot;
}

void yd_tk001_set_reboot_flag(U8 flag)
{
    yd_tk001_info.reboot = flag;
}

/*U8 Yd_get_restartflag()
{
	return g_n_restartflag;
}

void Yd_restartflagchange()
{
	g_n_restartflag = 0;
}*/

void RJ_SYS_ReStart()
{
    kal_prompt_trace( MOD_SOC,"RJ_SYS_ReStart");
    yd_tk001_set_reboot_flag(1); // moved by zyz for debug
    yd_tk001_set_record_from_NV();
    rj_reset();
}

void yd_tk001_receive_reboot_msg_handler()
{
    StartTimer(RJ_GPS_RESET_TIMER, RJ_GPS_APP_10S, RJ_SYS_ReStart);
}

void yd_send_reboot_control_msg()
{
    ilm_struct *bird_ilm = NULL;

    DRV_BuildPrimitive(bird_ilm, MOD_MMI, MOD_MMI, MSG_ID_YD_TK001_MSG_REBOOT, NULL);
    msg_send_ext_queue(bird_ilm);
}

void yd_set_reboot_control_msg()
{
    SetProtocolEventHandler(yd_tk001_receive_reboot_msg_handler,MSG_ID_YD_TK001_MSG_REBOOT);
}

void Yd_delay_reset()
{
    kal_prompt_trace(MOD_SOC," Yd_delay_reset");

    g_n_is_rest = 0;
    yd_send_reboot_control_msg();
}

void Yd_24reset()
{
    int iRandomTime = 0;
    kal_uint32 ntimer = 20*1000;
    applib_time_struct current_time = {0};
    applib_dt_get_rtc_time(&current_time);
    kal_prompt_trace(MOD_SOC," Yd_24reset nHour %d",current_time.nHour);
    if(current_time.nHour==2)
    {
        if(current_time.nMin==0)
        {
            if(g_n_is_rest==0)
            {
                iRandomTime = RandomEvent_Random(0,7200);
                kal_prompt_trace(MOD_SOC," Yd_24reset  bg rest  iRandomTime = %d",iRandomTime);
                g_n_is_rest = 1;
                Rj_stop_timer(BIRD_TASK_REST_TIMER);
                Rj_start_timer(BIRD_TASK_REST_TIMER, iRandomTime*1000, Yd_delay_reset,NULL);//10s
            }
        }
    }
    Rj_stop_timer(BIRD_TASK_24REST_TIMER);
    Rj_start_timer(BIRD_TASK_24REST_TIMER, ntimer, Yd_24reset,NULL);
}


void yd_uart_restart()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_restart");
    RJ_SYS_ReStart();
}

void yd_set_uart_reboot_msg()
{
    SetProtocolEventHandler(yd_uart_restart,MSG_ID_YD_TK001_MSG_RESTART_ALARM);
}

