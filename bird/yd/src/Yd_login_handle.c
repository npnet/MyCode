#include "Yd_login_handle.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_n_ydislogin = 0;/*服务器登陆成功标志*/
U8 g_n_conn_times = 0;/*登入失败次数*/
U8 g_n_calib_times = 0;/*校准时间次数*/
U8 g_n_ydischangesim= 0;/*SIM卡更换标志，初始化为0，未修改为1，修改为2，注册为3*/
U8 g_n_calib_delay=0;
U8 g_calibstart_flag=0;
U8 g_gettime_flag=0;

void Yd_calib();

/*=============================   使用的外部变量与函数====================================*/
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
extern volatile kal_int8 count_judge_gps_app_timer;
extern U8 g_n_ydischangesim;
extern U8 rj_gps_stage;
extern U16 g_login_count;
extern kal_bool  b_acc_switch_status;

extern void RJ_GPS_iccid(void);/*获取ICCID*/
extern void RJ_SYS_ReStart(void);
extern void bird_soc_send_tboxlogin(void);
extern void bird_soc_sendlogin(void);
extern void bird_soc_send_tboxtiming();
extern double bird_get_adc_channel_voltage(void);
extern void yd_set_utility_time(U8 *date);
extern void yd_tk001_defense_ini();
extern void Yd_set_all_msg_handler();
extern void Yd_main_factory();
extern void Yd_main();
extern void Yd_savedata();
extern void yd_send_gps_open_msg();
extern void Yd_readposfile();
extern void Yd_heart();
extern void Yd_tbox_heart();
extern void Yd_lc();
extern void bird_set_acc_state(U8 flag);
extern kal_bool get_Acc_onoff2();
extern void yd_send_save_nv_msg();
#ifdef BIRD_ECU_MMI_SUPPORT
extern volatile kal_uint8 ECU_COM_connect_flag;
extern void set_uart_connect_status(U8 flag);
#endif
extern U8 bird_get_oc_exec_state(void);
extern U8 bird_get_ocin_state(void);
extern U32 bird_get_oc_instruct(void);
extern void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id);
extern void bird_soc_sendcalib();
extern void txboxdecimal(U8*dec,U8* hex);
extern void can_data_reset();
extern void bird_soc_send_tboxlogout();
extern void bird_soc_send_tboxselfdefine();
extern void Bird_Tbox_delete();

U8 bird_get_islogin()
{
    return g_n_ydislogin;
}

void Yd_login()
{
    count_judge_gps_app_timer=0;
    g_n_ydislogin = 0;

    rj_led_status_info.b_SERVER_IS_LOGGING = KAL_TRUE;//LED闪烁

    kal_prompt_trace(MOD_SOC,"Yd_login");

    Rj_stop_timer(Bird_task_login_Timer);
    Rj_start_timer(Bird_task_login_Timer, RJ_GPS_APP_1M, Yd_login,NULL);
    
    if(Lima_get_soc_conn_flag())
    {
        bird_soc_sendlogin();
    }

}
// socket 从disconnect到connet 后被调用
void Yd_conn_logintxbox()
{
    kal_prompt_trace(MOD_SOC,"Yd_conn_logintxbox %d",b_acc_switch_status);
    if(!b_acc_switch_status)
        return;

    //g_login_count++; add by zyz
    g_n_ydislogin=2;
    bird_soc_send_tboxlogin();
}

void Yd_logintxbox()
{
    U8 nrestarttimes = 0;
    U8 nrestarttimes_rec = 0;
    U8 nrestresion = 0;

    count_judge_gps_app_timer=0;
    rj_led_status_info.b_SERVER_IS_LOGGING = KAL_TRUE;//LED闪烁

    kal_prompt_trace(MOD_SOC,"Yd_logintxbox %d %d",g_n_conn_times,b_acc_switch_status);
    if(!b_acc_switch_status)
        return;
//不符合规范要求，连续登录3次未成功，应该在30分钟后，重新连接并登录，登录成功后补发数据。

    if(g_n_conn_times>=3)
    {
        g_n_conn_times=0;
        Lima_set_soc_init_flag(FALSE);
        Lima_Soc_Dinit();
        Bird_clear_soc_conn();
        Bird_soc_conn();

        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_start_timer(Bird_task_logintxbox_Timer, bird_get_ser_res_time()*1000, Yd_logintxbox,NULL);
	
    }
	//add by zyz
	else if(g_n_conn_times == 2)
	{
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_start_timer(Bird_task_logintxbox_Timer, bird_get_conn_interval()*RJ_GPS_APP_1M, Yd_logintxbox,NULL);
	}
    else
    {
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_start_timer(Bird_task_logintxbox_Timer, bird_get_ser_res_time()*1000, Yd_logintxbox,NULL);
    }
    if(Lima_get_soc_conn_flag()||g_n_conn_times == 0)
    {
        //Rj_stop_timer(Bird_task_connlogin_Timer);
        g_n_conn_times++;
        bird_soc_send_tboxlogin();
    }
}

extern void Bird_acc();

void Yd_login_res(U8 *datetime,U8 *flag)
{
    double voltage;
    voltage=bird_get_adc_channel_voltage();

    kal_prompt_trace(MOD_SOC," Yd_login_res %d ",*flag);
    //set time
    yd_set_utility_time(datetime);
    can_data_reset();
    //login res
    //login sucess
    {
        Rj_stop_timer(Bird_task_login_Timer);
        g_gettime_flag=1;
        Lima_set_soc_init_flag(FALSE);
        Lima_Soc_Dinit();
        Bird_clear_soc_conn();
        Bird_soc_conn();
        g_n_ydislogin = 3;
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_start_timer(Bird_task_logintxbox_Timer, 1*1000, Yd_logintxbox,NULL);
        Rj_stop_timer(Bird_task_savedata_Timer);
        Rj_start_timer(Bird_task_savedata_Timer, 5*1000, Yd_savedata,NULL);/*启动存储数据*/
    }
}

applib_time_struct g_login_time= {0};
void Yd_tboxlogin_res(U8 *buf,U8 length)
{
	MYTIME currTime;
    U8 time_rec[6]= {0};
    U8 time[6]= {0};

    kal_prompt_trace(MOD_SOC," Yd_tboxlogin_res %d",g_n_ydislogin);

#if 1//add by zyz
	g_n_conn_times=0;

	g_login_count++;

    memcpy(time_rec, buf,6);
    txboxdecimal(time,time_rec);

    currTime.nYear = (U16)(time[0]+2000);
    currTime.nMonth= time[1] ;
    currTime.nDay = time[2];
    currTime.nHour = time[3] ;
    currTime.nMin = time[4];
    currTime.nSec = time[5] ;
    currTime.DayIndex= applib_dt_dow(currTime.nYear, currTime.nMonth, currTime.nDay);

	kal_prompt_trace(MOD_SOC," Yd_tboxlogin_res %d %d %d %d %d %d",currTime.nYear,currTime.nMonth,currTime.nDay,currTime.nHour,currTime.nMin,currTime.nSec);
	
	    /* Do error handing */
    if (currTime.nYear < 2018 || currTime.nMonth > 12 || currTime.nDay > 31 || currTime.nHour > 24 ||
        currTime.nMin > 60 || currTime.nSec > 60)
    {
		kal_prompt_trace(MOD_SOC,"Yd_tboxlogin_res error format time");
	   	applib_dt_get_rtc_time(&g_login_time);
    }
	else
	{	
	    mmi_dt_set_rtc_dt((MYTIME * )&currTime);
		memcpy(&g_login_time , &currTime , sizeof(applib_time_struct));
	}
	
#endif
	
    //applib_dt_get_rtc_time(&g_login_time);
    //g_login_time.nMin++;

    if((g_n_ydislogin==0)||(g_n_ydislogin==3))
    {
        g_n_ydislogin = 1;
        rj_led_status_info.b_SERVER_IS_LOGGING = KAL_FALSE;
        g_n_conn_times=0;
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_stop_timer(Bird_task_connlogin_Timer);
        //设置消息句柄
        Yd_set_all_msg_handler();
        yd_send_save_nv_msg();

        bird_soc_send_tboxselfdefine();
        Rj_stop_timer(Bird_task_heart_Timer);
        Rj_start_timer(Bird_task_heart_Timer, 2*60*1000, Yd_tbox_heart,NULL);/*启动发心跳过程*/

        Rj_stop_timer(Bird_task_main_Timer);
        Rj_start_timer(Bird_task_main_Timer, 2*1000, Yd_main,NULL);/*启动主定时器，开始处理位置信息*/
        StartTimer(BIRD_DELETE_FILE_TIMER, 1*60*1000, Bird_Tbox_delete);


    }
    else if(g_n_ydislogin==2)
    {
        g_n_ydislogin = 1;
        rj_led_status_info.b_SERVER_IS_LOGGING = KAL_FALSE;
        g_n_conn_times=0;
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_stop_timer(Bird_task_connlogin_Timer);

        bird_soc_send_tboxselfdefine();
    }
    else
    {
        g_n_ydislogin = 1;
        rj_led_status_info.b_SERVER_IS_LOGGING = KAL_FALSE;
        g_n_conn_times=0;
        Rj_stop_timer(Bird_task_logintxbox_Timer);
        Rj_stop_timer(Bird_task_connlogin_Timer);
    }

}

void Yd_logouttxbox()
{

    kal_prompt_trace(MOD_SOC,"Yd_logouttxbox %d",g_login_count);

    bird_soc_send_tboxlogout();
    //g_login_count++;
}
void Yd_24calib()
{
    int iRandomTime = 0;
    kal_uint32 ntimer = 20*1000;
    applib_time_struct current_time = {0};

    applib_dt_get_rtc_time(&current_time);
    kal_prompt_trace(MOD_SOC," Yd_24calib nHour %d",current_time.nHour);

    if(current_time.nHour==2)
    {
        if(current_time.nMin==0)
        {
            if(g_n_calib_delay==0)
            {
                iRandomTime = RandomEvent_Random(0,7200);
                kal_prompt_trace(MOD_SOC," Yd_24calib  bg rest  iRandomTime = %d",iRandomTime);
                g_n_calib_delay = 1;
                g_login_count=0;
                Rj_stop_timer(Bird_task_calibtime_Timer);
                Rj_start_timer(Bird_task_calibtime_Timer, iRandomTime*1000, Yd_calib,NULL);//10s
            }
        }
    }
    Rj_stop_timer(Bird_task_24calib_Timer);
    Rj_start_timer(Bird_task_24calib_Timer, ntimer, Yd_24calib,NULL);

}

void Yd_calib_res(U8 *buf,U8 length)
{
    MYTIME currTime;
    U8 time_rec[6]= {0};
    U8 time[6]= {0};

    double voltage;
    voltage=bird_get_adc_channel_voltage();

    kal_prompt_trace(MOD_SOC,"Yd_calib_res %d",length);

    if(length<=8)
        return;

    memcpy(time_rec, buf,6);
    txboxdecimal(time,time_rec);

    currTime.nYear = (U16)(time[0]+2000);
    currTime.nMonth= time[1] ;
    currTime.nDay = time[2];
    currTime.nHour = time[3] ;
    currTime.nMin = time[4];
    currTime.nSec = time[5] ;
    currTime.DayIndex= applib_dt_dow(currTime.nYear, currTime.nMonth, currTime.nDay);
    mmi_dt_set_rtc_dt((MYTIME * )&currTime);

    {
        g_n_calib_times=0;
        Rj_stop_timer(Bird_task_calibtime_Timer);
        Rj_stop_timer(Bird_task_24calib_Timer);
        Rj_start_timer(Bird_task_24calib_Timer, 60*1000, Yd_24calib,NULL);
    }
    if(g_calibstart_flag==0)
    {
        g_calibstart_flag=1;
        //acc设防模式
        yd_tk001_defense_ini();
        //同步acc状态，motor状态，lock状态

        //设置消息句柄
        Yd_set_all_msg_handler();
        yd_send_save_nv_msg();

#ifdef BIRD_ECU_MMI_SUPPORT
        set_uart_connect_status(ECU_COM_connect_flag);
#endif
        rj_gps_stage = BIRD_STATUS_HEARTBEFORE; /* 当前为静止*/

        /*工厂测试*/
        /*
        if((voltage>=21)&&(voltage<=23))
        {
        	rj_gps_stage = BIRD_STATUS_RUN;
        	Rj_stop_timer(Bird_task_main_Timer);
        	Rj_start_timer(Bird_task_main_Timer, 30*1000, Yd_main_factory,NULL);
        }
        else
        */
        {
            Rj_stop_timer(Bird_task_main_Timer);
            Rj_start_timer(Bird_task_main_Timer, 30*1000, Yd_main,NULL);/*启动主定时器，开始处理位置信息*/
        }

        //Rj_stop_timer(BIRD_TASK_LC_TJ_TIMER);
        //Rj_start_timer(BIRD_TASK_LC_TJ_TIMER, 30*1000, Yd_lc,NULL);

        //StartTimer(BIRD_READ_FILE, 120*1000, Yd_readposfile);	//读离线
        //yd_send_gps_open_msg();
    }

}

void Yd_calib()
{
    kal_prompt_trace(MOD_SOC," Yd_calib %d",g_n_calib_times);
    g_n_calib_delay=0;
    Rj_stop_timer(Bird_task_24calib_Timer);

    if(g_n_calib_times>=3)
    {
        g_n_calib_times=0;
        Lima_set_soc_init_flag(FALSE);
        Lima_Soc_Dinit();
        Bird_clear_soc_conn();
        Bird_soc_conn();

        Rj_stop_timer(Bird_task_calibtime_Timer);
        Rj_start_timer(Bird_task_calibtime_Timer, RJ_GPS_APP_1M, Yd_calib,NULL);
    }
    else
    {
        Rj_stop_timer(Bird_task_calibtime_Timer);
        Rj_start_timer(Bird_task_calibtime_Timer, RJ_GPS_APP_1M, Yd_calib,NULL);
    }
    g_n_calib_times++;
    bird_soc_send_tboxtiming();
}


