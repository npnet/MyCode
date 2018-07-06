#include "Yd_sleep_state.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"


U8 g_n_sleepcount = 0;
U8 g_n_sleepposcount = 0;
U8 g_n_sleeptimerbegin=0;/*休眠定时器开启标志位*/
U8 g_n_deepsleeptimerbegin=0;
U16 g_n_deepsleeptimercount=0;
U8  yk001_sleep_state = 0;
U8  yk001_deepsleep_state = 0;

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;


extern kal_bool is_ac_charger_in(void);
extern U8 yd_tk001_get_defense(void);
extern void Yd_DinitSocket(void);
extern void Lima_set_soc_init_flag(BOOL flag);
extern void Lima_Soc_Dinit(void);
extern void Bird_soc_conn(void);
extern void bird_soc_sendheart(void);
extern void Yd_main();
extern void RJ_GPS_LED1Light(S8 onoff);
extern void RJ_GPS_LED2Light(S8 onoff);
extern void RJ_GPS_LED3Light(S8 onoff);
extern void yd_send_gps_close_msg();
extern void Yd_switch_fly(bool bfly);



void yd_init_sleep_param()
{
    yd_tk001_info.normal_sleep.sleep_flag = 0;
    yd_tk001_info.normal_sleep.sleep_time = 5;

    yd_tk001_info.abnormal_sleep.sleep_flag = 1;
    yd_tk001_info.abnormal_sleep.sleep_time = 5;
}

U8 bird_get_normal_sleep_state(void)
{
    //return  yd_tk001_info.normal_sleep.sleep_state;
    return yk001_sleep_state;/*正常和非正常休眠时都为TRUE*/
}

void bird_set_normal_sleep_state(U8 flag)
{
    //yd_tk001_info.normal_sleep.sleep_state = flag;
    yk001_sleep_state = flag;
}

U8 bird_get_deep_sleep_state()
{
    return yk001_deepsleep_state;
}

void bird_set_deep_sleep_state(U8 flag)
{
    yk001_deepsleep_state = flag;
}

U8 bird_is_normal_sleep_on(void)
{
    return yd_tk001_info.normal_sleep.sleep_flag;
}

void bird_set_normal_sleep_flag(U8 flag)
{
    yd_tk001_info.normal_sleep.sleep_flag = flag;
}

U32 bird_get_normal_sleep_value()
{
    return yd_tk001_info.normal_sleep.sleep_time;
}

void bird_set_normal_sleep_value(U32 time)
{
    yd_tk001_info.normal_sleep.sleep_time = time;
}

U8 bird_is_abnormal_sleep_on(void)
{
    return yd_tk001_info.abnormal_sleep.sleep_flag;
}

void bird_set_abnormal_sleep_flag(U8 flag)
{
    yd_tk001_info.abnormal_sleep.sleep_flag = flag;
}

void bird_set_abnormal_sleep_value(U32 time)
{
    yd_tk001_info.abnormal_sleep.sleep_time = time;
}

U32 bird_get_abnormal_sleep_value()
{
    return yd_tk001_info.abnormal_sleep.sleep_time;
}

U8 Yd_is_sleep_on(void)
{
    kal_prompt_trace(MOD_SOC," Yd_is_sleep_on is_ac_charger_in = %d",is_ac_charger_in());
    if(is_ac_charger_in())
    {
        kal_prompt_trace(MOD_SOC," Yd_is_sleep_on normal_sleep=%d",bird_is_normal_sleep_on());
        return bird_is_normal_sleep_on();
    }
    else
    {
        kal_prompt_trace(MOD_SOC," Yd_is_sleep_on abnormal_sleep=%d",bird_is_abnormal_sleep_on());
        return bird_is_abnormal_sleep_on();
    }
}

//平台下发 s3
void Yd_sleeppos_rs()
{
    kal_prompt_trace(MOD_SOC," Yd_sleeppos_rs entry");
    if(bird_get_normal_sleep_state()==1)
    {
        kal_prompt_trace(MOD_SOC," Yd_sleeppos_rs run %d",rj_gps_stage);
        if(rj_gps_stage == BIRD_STATUS_SLEEPBEFORE)
        {
            kal_prompt_trace(MOD_SOC," Yd_sleeppos_rs bgYd_DinitSocket");
            rj_gps_stage = BIRD_STATUS_SLEEP;
            Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
            Rj_start_timer(Bird_task_sleep_dinisocket_Timer, RJ_GPS_APP_1M*2, Yd_DinitSocket,NULL); /*2分钟之后,反初始化socket*/
        }
    }
}

void Yd_sleep_sendheart()
{
    //bird_soc_sendstate(Yd_get_sleep_state());
    Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/
    Lima_Soc_Dinit();
    Bird_soc_conn();
    bird_soc_sendheart();
}

void Yd_stop_sleepTimer()
{
    //lrf add sleep timer ini
    Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER);
    g_n_sleeptimerbegin=0;
    g_n_sleepcount=0;
}

void Yd_start_normal_sleep()
{
    U8 issleepon=0;

    issleepon = bird_is_normal_sleep_on();//||(!get_Acc_onoff());//add byt wtl 20160621 acc off允许休眠
    kal_prompt_trace(MOD_SOC,"[Yd_start_normal_sleep]sleep_state=%d,get_Acc_onoff=%d,issleepon=%d",bird_get_normal_sleep_state(),get_Acc_onoff(),issleepon);

    if(issleepon) /*如果当前允许休眠*/
    {
        RJ_GPS_LED1Light(0);
        rj_led_status_info.b_GPS_IS_CLOSE = KAL_TRUE;
        RJ_GPS_LED2Light(0);
        rj_led_status_info.b_GSM_IS_CLOSE = KAL_TRUE;
        RJ_GPS_LED3Light(0);
        rj_led_status_info.b_SIM_IS_CLOSE = KAL_TRUE;

        //kal_prompt_trace(MOD_SOC,"%s %d",__FILE__,__LINE__);
        bird_set_normal_sleep_state(MMI_TRUE);  //休眠状态为是
        rj_gps_stage = BIRD_STATUS_SLEEPBEFORE; //设置当前运行状态为休眠前
        Rj_stop_timer(Bird_task_main_Timer);
        Rj_start_timer(Bird_task_main_Timer, 1*1000, Yd_main,NULL);	  //RJ_GPS_GpsSleep();      // gps SLEEP
        yd_send_gps_close_msg();
        //StartTimer(Bird_sleep_dinisocket_Timer, RJ_GPS_APP_1M, Yd_DinitSocket); /*5分钟之后,反初始化socket*/
    }
    g_n_sleeptimerbegin=0;
}

void Yd_start_abnormal_sleep()
{
    U8 issleepon=0;
    kal_prompt_trace(MOD_SOC,"[Yd_start_abnormal_sleep]sleep_state=%d",bird_get_normal_sleep_state());
    issleepon = bird_is_abnormal_sleep_on();
    if(issleepon && is_ac_charger_in()==KAL_FALSE) //如果当前允许休眠	未插电
    {
        RJ_GPS_LED1Light(0);
        rj_led_status_info.b_GPS_IS_CLOSE = KAL_TRUE;
        RJ_GPS_LED2Light(0);
        rj_led_status_info.b_GSM_IS_CLOSE = KAL_TRUE;
        RJ_GPS_LED3Light(0);
        rj_led_status_info.b_SIM_IS_CLOSE = KAL_TRUE;

        bird_set_normal_sleep_state(MMI_TRUE);  //休眠状态为是
        rj_gps_stage = BIRD_STATUS_SLEEPBEFORE; //设置当前运行状态为休眠前
        Rj_stop_timer(Bird_task_main_Timer);

        Rj_start_timer(Bird_task_main_Timer, 200, Yd_main,NULL);
        // gps SLEEP
        yd_send_gps_close_msg();
        //StartTimer(Bird_sleep_dinisocket_Timer, RJ_GPS_APP_1M, Yd_DinitSocket); /*5分钟之后,反初始化socket*/
    }
    g_n_sleeptimerbegin=0;

}

void Yd_normal_sleep()
{
    U32 ntimer = 0;
    U32 nvalue = 0;
    kal_prompt_trace(MOD_SOC,"[Yd_normal_sleep]sleep_state=%d",bird_get_normal_sleep_state());
    kal_prompt_trace(MOD_SOC,"bird_is_normal_sleep_on %d",bird_is_normal_sleep_on());
    kal_prompt_trace(MOD_SOC,"bird_is_abnormal_sleep_on %d",bird_is_abnormal_sleep_on());
    kal_prompt_trace(MOD_SOC,"bird_get_normal_sleep_value %d",bird_get_normal_sleep_value());
    kal_prompt_trace(MOD_SOC,"g_n_sleeptimerbegin %d",g_n_sleeptimerbegin);
    /*如果未启动休眠定时器*/
    if(g_n_sleeptimerbegin==0)
    {
        g_n_sleeptimerbegin=1;
        /*判断是正常休眠还是断电休眠*/
        if(is_ac_charger_in())
        {
            kal_prompt_trace(MOD_SOC,"BIRD_YD_NORMAL_SLEEP TIMER START");
            nvalue = bird_get_normal_sleep_value();
            //StartTimer(BIRD_YD_NORMAL_SLEEP_TIMER, 1000*bird_get_normal_sleep_value() , Yd_start_normal_sleep);
            ntimer = 1000*60*nvalue;
            Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER);
            Rj_start_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER, ntimer, Yd_start_normal_sleep,NULL);
        }
        else
        {
            kal_prompt_trace(MOD_SOC,"BIRD_YD_ABNORMAL_SLEEP TIMER START");
            nvalue = bird_get_abnormal_sleep_value();
            ntimer = 1000*60*nvalue;
            //StartTimer(BIRD_YD_NORMAL_SLEEP_TIMER, 1000*bird_get_abnormal_sleep_value() , Yd_start_abnormal_sleep);
            Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER);
            Rj_start_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER, ntimer, Yd_start_abnormal_sleep,NULL);
        }
    }
}

/****************************************未被使用*******************************************/
U8 Yd_get_sleep_state();

void Yd_deep_sleep_sendheart()
{
    Yd_switch_fly(FALSE);
    Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/
    Lima_Soc_Dinit();
    bird_set_deep_sleep_state(FALSE);
    Bird_soc_conn();
    //Yd_SendHeartInfo();
    Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
    Rj_start_timer(Bird_task_sleep_dinisocket_Timer, RJ_GPS_APP_1M*2, Yd_DinitSocket,NULL); /*2分钟之后,反初始化socket*/
}

void Yd_start_deep_sleep()
{
    kal_prompt_trace(MOD_SOC,"[Yd_start_deep_sleep] entry");
    Rj_stop_timer(BIRD_TASK_YD_DEEP_SLEEP_TIMER);
    rj_gps_stage = BIRD_STATUS_DEEPSLEEP;
    bird_set_deep_sleep_state(TRUE);
    Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/
    Rj_stop_timer(BIRD_TASK_SOCKET_SEND); //停止发送定时器
    Lima_Soc_Dinit();
    Yd_switch_fly(TRUE);
    Rj_stop_timer(Bird_task_main_Timer);
    Rj_start_timer(Bird_task_main_Timer, RJ_GPS_APP_10S, Yd_main,NULL);
}

extern volatile kal_uint16 count_system_time;
void Yd_start_deep_sleep_count()
{
    Rj_stop_timer(BIRD_TASK_YD_DEEP_SLEEP_TIMER);
    Rj_start_timer(BIRD_TASK_YD_DEEP_SLEEP_TIMER, 1*60*1000, Yd_start_deep_sleep_count,NULL);
    kal_prompt_trace(MOD_SOC,"[Yd_start_deep_sleep_count]deep_sleep_count=%d",g_n_deepsleeptimercount);
    g_n_deepsleeptimercount++;
    if(g_n_deepsleeptimercount>24*60*3)
    {
        count_system_time = 0; /*深度休眠重置7天重启变量*/
    }
    if(g_n_deepsleeptimercount>24*60*5) //5天进入深度休眠
    {
        Yd_start_deep_sleep();
    }
}

void Yd_deep_sleep()
{
    kal_prompt_trace(MOD_SOC,"[Yd_deep_sleep]deep_sleep_begin=%d",g_n_deepsleeptimerbegin);
    /*如果未启动休眠定时器*/
    if(g_n_deepsleeptimerbegin==0)
    {
        kal_prompt_trace(MOD_SOC,"[Yd_deep_sleep]deep_sleep_begin=%d",g_n_deepsleeptimerbegin);
        g_n_deepsleeptimerbegin=1;
        Rj_stop_timer(BIRD_TASK_YD_DEEP_SLEEP_TIMER);
        Rj_start_timer(BIRD_TASK_YD_DEEP_SLEEP_TIMER, 1*60*1000, Yd_start_deep_sleep_count,NULL);
    }
}


U8 Yd_get_sleep_state()
{
    if(!is_ac_charger_in())
    {
        return 7;
    }
    else
    {
        if(yd_tk001_get_defense()==1)
        {
            return 5;
        }
        else
        {
            return 6;
        }
    }
	
}

