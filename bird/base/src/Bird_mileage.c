#include "Bird_mileage.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"



U8 g_n_lc_hour = 99;
S32 g_n_get_lc=0;
U8 g_n_equstatue_count = 1;/*里程处理时,非运行统计次数*/
U8 g_n_station_count = 0;/*里程处理时,非休眠状态，卫星持续不可用次数*/
RJ_Gps_Position yd_lc_tj_position = {0};

/*=============================   使用的外部变量与函数====================================*/

extern S32 RJ_GPS_GetDistance(double lat1, double lng1, double lat2, double lng2);
extern S8 bird_get_saltllite_num();
extern void bird_soc_sendlc(U8 type);
extern void applib_dt_get_rtc_time(applib_time_struct *t);
extern int RandomEvent_Random(int _min, int _max);
extern U8 Yd_get_equstatus();




U8 Yd_get_lc_static_count()
{
    return g_n_equstatue_count;
}

U8 Yd_get_lc_station_count()
{
    return g_n_station_count;
}

U8 Yd_get_lc_static_time()
{
    kal_prompt_trace(MOD_SOC," Yd_get_lc_static_time %d ",g_n_equstatue_count);
    if(g_n_equstatue_count>12|| g_n_equstatue_count==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//里程统计
void bird_set_lc_tj()
{
    S32 dis = 0;
    S8 nsalitenum=0;
    nsalitenum = bird_get_saltllite_num();
    kal_prompt_trace(MOD_SOC," bird_set_lc_tj saltllite_num = %d ",nsalitenum);
    if(nsalitenum>=3)
    {
        if(yd_lc_tj_position.la!=0 ||  yd_lc_tj_position.lg!=0)
        {
            dis = RJ_GPS_GetDistance( yd_lc_tj_position.la, yd_lc_tj_position.lg,rj_position.la, rj_position.lg);
            g_n_get_lc = g_n_get_lc + dis;
            yd_lc_tj_position.la = rj_position.la;
            yd_lc_tj_position.lg = rj_position.lg;
        }
        else
        {
            yd_lc_tj_position.la = rj_position.la;
            yd_lc_tj_position.lg = rj_position.lg;
        }
    }
}

S32 bird_get_lc_tj()
{
    return g_n_get_lc;
}

void bird_lc_tj_clear()
{
    g_n_get_lc = 0;
}

void bird_ini_lc_tj()
{
    memset(&yd_lc_tj_position, 0, sizeof(yd_lc_tj_position));
}

void Yd_delay_sendlc()
{
    applib_time_struct current_time = {0} ;

    applib_dt_get_rtc_time(&current_time);
    kal_prompt_trace(MOD_SOC,"  Yd_delay_sendlc");
    bird_soc_sendlc(1);
    g_n_lc_hour = current_time.nHour;
    bird_lc_tj_clear();
}

void Yd_lc()
{
    int iRandomTime = 0;
    kal_uint32 ntimer = 20*1000;
    applib_time_struct current_time = {0} ;
    applib_dt_get_rtc_time(&current_time);
    if(current_time.nHour!=g_n_lc_hour)
    {
        if(current_time.nMin==0)
        {
            iRandomTime = RandomEvent_Random(0,300);
            kal_prompt_trace(MOD_SOC,"Yd_lc iRandomTime = %d ",iRandomTime);
            Rj_stop_timer(BIRD_TASK_DELAY_SENDLC_TIMER);
            Rj_start_timer(BIRD_TASK_DELAY_SENDLC_TIMER, iRandomTime*1000, Yd_delay_sendlc,NULL);
        }
    }
    //run count
    if(Yd_get_equstatus()==0)
    {
        g_n_equstatue_count = g_n_equstatue_count + 1;
        if(g_n_equstatue_count>250)
        {
            g_n_equstatue_count = 250;
        }
        kal_prompt_trace(MOD_SOC," Yd_lc g_n_equstatue_count%d ",g_n_equstatue_count);
    }
    else
    {
        g_n_equstatue_count = 1;
        g_n_station_count = 0;
    }
    //station count
    if(RJ_GPS_is_working()==1)
    {
        g_n_station_count = 0;
    }
    else
    {
        if(bird_get_normal_sleep_state())
        {
            /*当前休眠*/
            g_n_station_count=0;
        }
        else
        {
            g_n_station_count = g_n_station_count + 1;
            if(g_n_station_count>250)
            {
                g_n_station_count = 250;
            }
        }
    }
    kal_prompt_trace(MOD_SOC," Yd_lc g_n_station_count%d ",g_n_station_count);
    Rj_stop_timer(BIRD_TASK_LC_TJ_TIMER);
    Rj_start_timer(BIRD_TASK_LC_TJ_TIMER, ntimer, Yd_lc,NULL);
}

