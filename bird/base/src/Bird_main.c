/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD the main program module, service module of different business into different
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/

#include "Bird_app.h"
#include "Bird_main.h"
#include "bird_task.h"
#include "drv_comm.h" //add by lqy test
#ifdef BIRD_ECU_MMI_SUPPORT
#include "Yd_uart_main.h"
#endif
#ifdef SMART_SEAL_SUPPORT
#include "smartseal_drv.h"
#endif
#include "can.h"
#include "mcp2515.h"

//用户信息变量
RJ_Gps_User_Info rj_user_info;

U16 RJ_Gps_wrong_info;

U16 RJ_Gps_wrong_info__________;


/* 查询类型标记 */
S8 rj_gps_search_mark[4];

//信号灯
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
kal_bool b_LED_IS_ENABLE = KAL_TRUE;

//sim count
U16 g_n_sim_count = 0;
extern U8 g_sim_count_signal;

extern kal_bool b_charge_usb_in;
/* yanchunhai 20120806 定义判断SIM插入的变量:  插入   KAL_TRUE, 未插入KAL_FALSE  */
extern kal_bool b_SIM_IS_OK;

extern volatile kal_int8 count_judge_gps_app_timer ;
extern void Yd_24reset();
extern void RJ_GPS_Gsmled_LightWink();
extern void bird_watchdog_control();
extern void yd_set_power_off_msg();
extern void can_rx_data_check();
extern void Bird_Tbox_Init(void);
extern void Bird_acc();

/*-------------------------------------------------------
                      获取传感器状态
                      return 1 运动
                      return 0 静止
-------------------------------------------------------*/
#ifdef BIRD_ECU_MMI_SUPPORT
extern U8 get_uart_connect_status();
extern U8 get_uart_run_status();
kal_bool get_uart_motion_flag()
{
    kal_bool flag=KAL_FALSE;

    if(get_uart_connect_status()==0)
    {
        flag=get_b_motion();
    }
    else
    {
        flag=((kal_bool)get_uart_run_status());
    }

    kal_prompt_trace(MOD_SOC,"get_uart_motion_flag=%d", flag);
    return flag;

}
#endif


U8 RJ_GPS_GetSensorStatus()
{
    kal_prompt_trace( MOD_SOC,"RJ_GPS_GetSensorStatus b_is_no_motion   = %d", get_b_motion());

#if defined(BIRD_ECU_MMI_SUPPORT)
    if(get_uart_motion_flag() == KAL_FALSE)
#else
    if(get_b_motion() == KAL_FALSE)
#endif
    {
        /* GPS 静止 */
        return 0;
    }
    else
    {
        /* GPS 运动 */
        return 1;
    }
}

/*---------------------------------------------
                  LED灯控制
---------------------------------------------*/
/*-----------------led control start-------------*/
void RJ_GPS_LEDENABLE_Delay()
{
    StopTimer(BIRD_LED_ENABLE_DELAY);
    b_LED_IS_ENABLE = FALSE;
}


void RJ_GPS_LED1Light(S8 onoff)
{
    cus_leds_led1_light(onoff);
}

void RJ_GPS_LED2Light(S8 onoff)
{
    kal_prompt_trace( MOD_SOC,"RJ_GPS_LED2Light %d",onoff);
    cus_leds_led2_light(onoff);
}

void RJ_GPS_LED3Light(S8 onoff)
{
    kal_prompt_trace( MOD_SOC,"RJ_GPS_LED3Light",onoff);
    cus_leds_led3_light(onoff);
}


void RJ_GPS_Gpsled_LightWink()
{
    //extern  unsigned char cus_leds_set_red_light_status(unsigned char status);

    StopTimer(RJ_GPS_TIMER_REDLIGHT);
    /*
    b_LED_IS_ENABLE = TRUE;
       if(!b_LED_IS_ENABLE) //lrf add
       {

       	cus_leds_red_light(0);
    cus_leds_set_red_light_status(0);
    return;
       } //lrf add
       */
    if(rj_led_status_info.b_GPS_IS_CLOSE)
    {
        cus_leds_led1_light(0);
        cus_leds_set_led1_light_status(0);
    }
    else if(rj_led_status_info.b_GPS_IS_SERACHING)
    {
        if(cus_leds_get_led1_light_status()== 0)
        {
            cus_leds_led1_light(1);
            // Delayms(5);  //25 10
            kal_sleep_task(25);
            cus_leds_led1_light(0);
            cus_leds_set_led1_light_status(1);
        }
        else
        {
            cus_leds_led1_light(0);
            cus_leds_set_led1_light_status(0);
        }
    }
    else
    {
        cus_leds_led1_light(1);
        cus_leds_set_led1_light_status(1);
    }



    if(rj_led_status_info.b_GPS_IS_SERACHING)
    {

        StartTimer(RJ_GPS_TIMER_REDLIGHT, RJ_GPS_LED_UNLIGHT_TIME, RJ_GPS_Gpsled_LightWink);
    }
    else
    {

        StartTimer(RJ_GPS_TIMER_REDLIGHT, RJ_GPS_LED_SLOWLIGHT_TIME, RJ_GPS_Gpsled_LightWink);
    }
}

U8 led_status1_flag=0;
U8 led_status2_flag=0;
U8 led_status3_flag=0;

#ifdef __BIRD_LED_IDENTIFY_WINK__
U8 led_status4_flag=0;
U8 led_status5_flag=0;
U8 led_status6_flag=0;
static void Bird_LED_Wink(kal_uint8 ntimes, kal_uint16 light, kal_uint16 jump);
void Bird_LED_Identify_Wink(void);

/*---------------------------------------/
  *MACRO contrl : BIRD_LED_IDENTIFY_WINK_SUPPORT
  *function name :Bird_LED_Wink
  *parameters:
  	*ntimes : set wink times.if jump equal to 0,led only wink once,will ignore ntimes's value
  	*light :     set led hold light time
  	*jump :    return Bird_LED_Identify_Wink time,if jump equal to 0,will set default 10 msec
  *create data : 2017-04-28 14:38
/---------------------------------------*/
static void Bird_LED_Wink(kal_uint8 ntimes,kal_uint16 light,kal_uint16 jump)
{
    static kal_uint16 led_light = 0;
    static kal_uint16 fun_jump = 0;
    static kal_uint8 led_wink_times = 0;
    static kal_uint8 led_times_limit = 0;
    static kal_uint8 led_times_set_flag = 0;
    if(!led_times_set_flag)
    {
        led_times_set_flag = 1;
        led_light = light;
        fun_jump = jump;
        led_times_limit = ntimes * 2;
        if(fun_jump == 0)
        {
            fun_jump = 10;
            led_times_limit = 2;
        }
    }
    StopTimer(RJ_GPS_TIMER_YELLOLIGHT);
    if(led_wink_times < led_times_limit)
    {
        if(cus_leds_get_led2_light_status() == 0)
        {
            led_wink_times ++;
            cus_leds_led2_light(1);
            cus_leds_set_led2_light_status(1);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, led_light, Bird_LED_Wink);
        }
        else
        {
            led_wink_times ++;
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, led_light, Bird_LED_Wink);
        }
    }
    else
    {
        led_wink_times = 0;
        led_times_limit = 0;
        led_times_set_flag = 0;
        led_light = 0;
        if(cus_leds_get_led2_light_status() != 0)
        {
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
        }

        StartTimer(RJ_GPS_TIMER_YELLOLIGHT, fun_jump, Bird_LED_Identify_Wink);
        fun_jump = 0;
    }
}

void Bird_LED_Identify_Wink(void)
{
    StopTimer(RJ_GPS_TIMER_YELLOLIGHT);
    if(rj_led_status_info.b_SIM_IS_SERACHING == KAL_TRUE)
    {
        kal_prompt_trace(MOD_USB,"[LED] SIM Searching");
        Bird_LED_Wink(1, 200, 1500);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status1_flag==0)
        {
            led_status1_flag=1;
            led_status2_flag=0;
            led_status3_flag=0;
            led_status4_flag=0;
            led_status5_flag=0;
            led_status6_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_SOC_IS_CONNECTING == KAL_TRUE&&rj_led_status_info.b_SOC_IS_CONNECTED == KAL_FALSE\
            &&rj_led_status_info.b_GSM_IS_SERACHING == KAL_TRUE)
    {
        kal_prompt_trace(MOD_USB,"[LED] GSM Searching");
        Bird_LED_Wink(5, 200, 2000);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status2_flag==0)
        {
            led_status1_flag=0;
            led_status2_flag=1;
            led_status3_flag=0;
            led_status4_flag=0;
            led_status5_flag=0;
            led_status6_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_SOC_IS_CONNECTING == KAL_TRUE && rj_led_status_info.b_SOC_IS_CONNECTED == KAL_FALSE)
    {
        kal_prompt_trace(MOD_USB,"[LED] SOC Connecting");
        Bird_LED_Wink(2, 200, 1500);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status3_flag==0)
        {
            led_status1_flag=0;
            led_status2_flag=0;
            led_status3_flag=1;
            led_status4_flag=0;
            led_status5_flag=0;
            led_status6_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_SERVER_IS_LOGGING == KAL_TRUE)
    {
        kal_prompt_trace(MOD_USB,"[LED] Login");
        Bird_LED_Wink(3, 200, 1500);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status4_flag==0)
        {
            led_status1_flag=0;
            led_status2_flag=0;
            led_status3_flag=0;
            led_status4_flag=1;
            led_status5_flag=0;
            led_status6_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_GPS_IS_SERACHING == KAL_TRUE&&rj_led_status_info.b_GPS_IS_CLOSE == KAL_FALSE)
    {
        kal_prompt_trace(MOD_USB,"[LED] GPS Searching");
        Bird_LED_Wink(4, 200, 2000);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status5_flag==0)
        {
            led_status1_flag=0;
            led_status2_flag=0;
            led_status3_flag=0;
            led_status4_flag=0;
            led_status5_flag=1;
            led_status6_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_GPS_IS_SERACHING == KAL_FALSE&&rj_led_status_info.b_GPS_IS_CLOSE == KAL_FALSE)
    {
        kal_prompt_trace(MOD_USB,"[LED] GPS Search Done");
        cus_leds_led2_light(1);
        cus_leds_set_led2_light_status(1);
        StartTimer(RJ_GPS_TIMER_YELLOLIGHT, 5*1000, Bird_LED_Identify_Wink);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status6_flag==0)
        {
            led_status1_flag=0;
            led_status2_flag=0;
            led_status3_flag=0;
            led_status4_flag=0;
            led_status5_flag=0;
            led_status6_flag=1;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if(rj_led_status_info.b_GPS_IS_CLOSE == KAL_TRUE&&rj_led_status_info.b_GPS_IS_SERACHING == KAL_FALSE)
    {
        kal_prompt_trace(MOD_USB,"[LED] GPS Close");
        cus_leds_led2_light(0);
        cus_leds_set_led2_light_status(0);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"[LED] Error");
        Bird_LED_Wink(1,200, 0);
    }
}
#endif

void RJ_GPS_Gsmled_LightWink()
{
#if 1   /*Modified by Huangwenjian 20160523 begin*/
    kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink enter");
    StopTimer(RJ_GPS_TIMER_YELLOLIGHT);

    if ((rj_led_status_info.b_GSM_IS_SERACHING == KAL_TRUE) || (rj_led_status_info.b_SIM_IS_SERACHING == KAL_TRUE) || (rj_led_status_info.b_SERVER_IS_LOGGING == KAL_TRUE))
    {
        if(cus_leds_get_led2_light_status()== 0)
        {
            kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink boot led status == 0");
            cus_leds_led2_light(1);
            cus_leds_set_led2_light_status(1);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_FAST_ON_TIME, RJ_GPS_Gsmled_LightWink);
        }
        else
        {
            kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink boot led status == 1");
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_FAST_OFF_TIME, RJ_GPS_Gsmled_LightWink);
        }
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status1_flag==0)
        {
            led_status1_flag=1;
            led_status2_flag=0;
            led_status3_flag=0;
            yd_uart_search_c9_04();
        }
#endif

    }
    else if ((rj_led_status_info.b_GPS_IS_CLOSE == KAL_FALSE) && (rj_led_status_info.b_GPS_IS_SERACHING == KAL_TRUE))
    {
        if(cus_leds_get_led2_light_status()== 0)
        {
            kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink gsm searched led status == 0");
            cus_leds_led2_light(1);
            cus_leds_set_led2_light_status(1);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_SLOW_ON_TIME, RJ_GPS_Gsmled_LightWink);
        }
        else
        {
            kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink gsm searchedled status == 1");
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_SLOW_ON_TIME, RJ_GPS_Gsmled_LightWink);
        }
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status2_flag==0)
        {
            led_status2_flag=1;
            led_status1_flag=0;
            led_status3_flag=0;
            yd_uart_search_c9_04();
        }
#endif
    }
    else if ((rj_led_status_info.b_GPS_IS_CLOSE == KAL_FALSE) && (rj_led_status_info.b_GPS_IS_SERACHING == KAL_FALSE))
    {
        kal_prompt_trace( MOD_SOC,"GR_GPS_LED_LightWink gps searched");
        cus_leds_led2_light(1);
        StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_ALWAYS_TIME, RJ_GPS_Gsmled_LightWink);
#ifdef BIRD_ECU_MMI_SUPPORT
        if(led_status3_flag==0)
        {
            led_status3_flag=1;
            led_status1_flag=0;
            led_status2_flag=0;
            yd_uart_search_c9_04();
        }
#endif

    }
    else if (rj_led_status_info.b_GPS_IS_CLOSE == KAL_TRUE)
    {
        cus_leds_led2_light(0);
    }
#else
    static kal_bool is_sevice_available = KAL_FALSE;

    StopTimer(RJ_GPS_TIMER_YELLOLIGHT);
    if(rj_led_status_info.b_GSM_IS_CLOSE)
    {
        cus_leds_led2_light(0);
        cus_leds_set_led2_light_status(0);
    }
    else if(rj_led_status_info.b_GSM_IS_SERACHING)
    {
        if(cus_leds_get_led2_light_status()== 0)
        {
            cus_leds_led2_light(1);

            kal_sleep_task(25);
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(1);

            // StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_LIGHT_TIME, RJ_GPS_YelloLightWink);

        }
        else
        {
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
        }
    }
    else
    {
        if(rj_led_status_info.b_SIM_IS_SERACHING)
        {
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
        }
        else
        {
            cus_leds_led2_light(1);
            cus_leds_set_led2_light_status(1);
        }

    }

    if((rj_led_status_info.b_SIM_IS_SERACHING)&&(rj_led_status_info.b_GSM_IS_SERACHING))
    {
        is_sevice_available = KAL_FALSE;

        StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_UNLIGHT_TIME, RJ_GPS_Gsmled_LightWink);
    }
    else
    {
        is_sevice_available = KAL_TRUE;

        StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_SLOWLIGHT_TIME, RJ_GPS_Gsmled_LightWink);
    }

#endif /*Modified by Huangwenjian 20160523 end*/
}

void RJ_GPS_Simled_LightWink()
{
    /****关机灭，GPS搜索快闪(亮0.2，灭1.8)**正常慢闪(亮0.2，灭4.8)**/

    StopTimer(RJ_GPS_TIMER_BLUELIGHT);
    if((rj_led_status_info.b_SIM_IS_CLOSE)&&(rj_led_status_info.b_GSM_IS_CLOSE)&&(rj_led_status_info.b_GPS_IS_CLOSE))
    {
        cus_leds_led3_light(0);
        cus_leds_set_led3_light_status(0);
    }
    else if(rj_led_status_info.b_GPS_IS_SERACHING)
    {
        if(cus_leds_get_led3_light_status()== 0)
        {
            cus_leds_led3_light(1);

            kal_sleep_task(25);
            cus_leds_led3_light(0);
            cus_leds_set_led3_light_status(1);
            //StartTimer(RJ_GPS_TIMER_BLUELIGHT, RJ_GPS_LED_LIGHT_TIME, RJ_GPS_Simled_LightWink);

        }
        else
        {
            cus_leds_led3_light(0);

            cus_leds_set_led3_light_status(0);


        }
    }
    else
    {
        cus_leds_led3_light(1);

        cus_leds_set_led3_light_status(1);
    }

    if((rj_led_status_info.b_SIM_IS_SERACHING)&&(rj_led_status_info.b_GSM_IS_SERACHING))
    {
        StartTimer(RJ_GPS_TIMER_BLUELIGHT, RJ_GPS_LED_SLOWLIGHT_TIME, RJ_GPS_Simled_LightWink);
    }
    else
    {
        StartTimer(RJ_GPS_TIMER_BLUELIGHT, RJ_GPS_LED_UNLIGHT_TIME, RJ_GPS_Simled_LightWink);
    }


}

void RJ_login_LightWink()
{
    StopTimer(RJ_GPS_TIMER_YELLOLIGHT);

    if (rj_led_status_info.b_SERVER_IS_LOGGING == KAL_TRUE)
    {
        if(cus_leds_get_led2_light_status()== 0)
        {
            cus_leds_led2_light(1);
            cus_leds_set_led2_light_status(1);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_FAST_ON_TIME, RJ_login_LightWink);
        }
        else
        {
            cus_leds_led2_light(0);
            cus_leds_set_led2_light_status(0);
            StartTimer(RJ_GPS_TIMER_YELLOLIGHT, RJ_GPS_LED_FAST_OFF_TIME, RJ_login_LightWink);
        }

    }
    else if (rj_led_status_info.b_SERVER_IS_LOGGING == KAL_FALSE)
    {
        cus_leds_led2_light(0);
    }
}

void RJ_GPS_LEDENABLE()
{
    b_LED_IS_ENABLE = TRUE;
    //RJ_GPS_RedLightWink();
    //StopTimer(BIRD_LED_ENABLE_DELAY);
    //StartTimer(BIRD_LED_ENABLE_DELAY,300000, RJ_GPS_LEDENABLE_Delay);
//5min
}

void RJ_GPS_LEDDISABLE()
{
    rj_led_status_info.b_GPS_IS_CLOSE = TRUE;
    rj_led_status_info.b_GSM_IS_CLOSE = TRUE;
    rj_led_status_info.b_SIM_IS_CLOSE = TRUE;
    RJ_GPS_Gpsled_LightWink();
#ifdef __BIRD_LED_IDENTIFY_WINK__
    Bird_LED_Identify_Wink();
#else
    RJ_GPS_Gsmled_LightWink();
#endif
    RJ_GPS_Simled_LightWink();
}
/*---------------led control end----------------------*/

/*-------------------------------------------------------
                      获取SIM 卡状态，是否有卡
-------------------------------------------------------*/
U8 RJ_GPS_GetSimCardStatus()
{
    kal_prompt_trace(MOD_SOC,"RJ_GPS_App_Start b_SIM_IS_OK = %d", b_SIM_IS_OK);

    if(KAL_FALSE != b_SIM_IS_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/***************************************************************************
  Function:       RJ_GPS_App_GpsStart()
  Description:    BIRD service program entrance function, to distinguish between the different services according to different switch
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:

  Output:
  Return:
  Others:
  Author:       lrf2013-05-29
****************************************************************************/
U8 g_gps_fwup=0;
void RJ_GPS_App_GpsStart()
{
    kal_prompt_trace(MOD_SOC,"RJ_GPS_App_GpsStart b_SIM_IS_OK = %d,count=%d", b_SIM_IS_OK,g_n_sim_count);
    RJ_GPS_Init();
    RJ_GPS_LEDENABLE();//lrf add
    Yd_switch_fly(false);
    count_judge_gps_app_timer=0;

    if(g_gps_fwup==1)
        return;
    if(KAL_FALSE != b_SIM_IS_OK)
    {
        rj_led_status_info.b_SIM_IS_SERACHING = KAL_FALSE;
        Yd_reg();
    }
    else
    {
        g_n_sim_count = g_n_sim_count + 1;
        if(g_n_sim_count==60)
        {
            g_n_sim_count = 0;
            if(g_sim_count_signal==1)
            {
                kal_prompt_trace(MOD_SOC,"RJ_GPS_App_GpsStart reboot");
                RJ_SYS_ReStart();
            }
            else
            {
                StartTimer(BIRD_GPRS_TIMER_START, 1000*2, RJ_GPS_App_GpsStart);
            }
        }
        else
        {
            StartTimer(BIRD_GPRS_TIMER_START, 1000*2, RJ_GPS_App_GpsStart);
        }
    }
}

extern void Gps_get_handle();
#ifdef BIRD_ECU_SUPPORT
extern void ACC_ECU_check_timer();
extern void BD_ecu_uart_data_check();
#endif
/***************************************************************************
  Function:       Bird_start()
  Description:    BIRD service program entrance function
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:

  Output:
  Return:
  Others:
  Author:       lrf2013-05-29
****************************************************************************/
char bird_at_test_flag = 0;
#ifdef BIRD_BT_SUPPORT
extern void bluetooth_init(void);
extern void get_bluetooth_data(void);
extern U8* yd_save_bt_mac();
extern void spi_sent_bt();

#endif
void bird_At_test_flag()
{
    bird_at_test_flag = 1;
}


#define BIRD_CAN_SIMULATE_SEND_SUPPORT

#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
void can_simulate_timer()
{
    can1_send_data();
    can2_send_data();

    StartTimer(BIRD_CAN_SIMULATE_SEND_TIMER, 10000, can_simulate_timer);
}
#endif
extern void bird_watchdog_control();

extern void rmmi_write_to_uart(kal_uint8 * buffer, kal_uint16 length, kal_bool stuff);
extern void ECU_UART_INIT(void);
extern void Bird_Tbox_delete();

void Bird_start_()
{
#if 1
    char gpio_data = 0;
    char buffer[128]; //MAUI_02377056

    /* 点亮指示灯*/
#ifdef __BIRD_LED_IDENTIFY_WINK__
    rj_led_status_info.b_SIM_IS_SERACHING = KAL_TRUE;
    rj_led_status_info.b_GPS_IS_SERACHING = KAL_FALSE;
    rj_led_status_info.b_GSM_IS_SERACHING = KAL_TRUE;
    rj_led_status_info.b_SERVER_IS_LOGGING = KAL_TRUE;
    rj_led_status_info.b_GPS_IS_CLOSE = KAL_FALSE;
    rj_led_status_info.b_SOC_IS_CONNECTING = KAL_FALSE;
    rj_led_status_info.b_SOC_IS_CONNECTED = KAL_FALSE;
#else
    rj_led_status_info.b_SIM_IS_SERACHING = KAL_TRUE;
    rj_led_status_info.b_GPS_IS_SERACHING = KAL_TRUE;
    rj_led_status_info.b_GSM_IS_SERACHING = KAL_TRUE;
    rj_led_status_info.b_SERVER_IS_LOGGING = KAL_TRUE;
    rj_led_status_info.b_GPS_IS_CLOSE = KAL_FALSE;
#endif
    yd_tk001_get_record_from_NV();//从NV中读取已存数据 Bird_nv_info
    yd_tk002_get_record_from_NV();
#ifdef BIRD_BT_SUPPORT
    yd_tk003_get_record_from_NV();
#endif
    yd_tk004_get_record_from_NV();
    yd_tk005_get_record_from_NV();
#ifdef __BIRD_LED_IDENTIFY_WINK__
    StartTimer(RJ_GPS_TIMER_YELLOLIGHT, 200, Bird_LED_Identify_Wink);
#else
    StartTimer(RJ_GPS_TIMER_YELLOLIGHT, 200, RJ_GPS_Gsmled_LightWink);
#endif

    //Bird_Log_Init();
    Bird_Tbox_Init();
    can_start();
    SetProtocolEventHandler(can1_hw_rx, MSG_ID_CAN1_HW_RX);
    SetProtocolEventHandler(can2_hw_rx, MSG_ID_CAN2_HW_RX);
    SetProtocolEventHandler(can_rx_data_check, MSG_ID_CAN_RX_CHECK);

    yd_set_aux_key_on_msg();
    yd_set_acc_key_closed_msg();
    //RJ_GPS_StartTimer(BIRD_SOS_STATE_timer, 400, Bird_acc);

    //RJ_GPS_StartTimer(BIRD_ACC_ECU_CHECK_TIMER, 15*1000, can2_eint_hisr);
    //RJ_GPS_Simled_LightWink();
    //RJ_GPS_Gsmled_LightWink();
    sprintf(buffer, "DW11_OPEN");
    rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);

    Gps_get_handle();
    //bird_ocin_init();
    RJ_GPS_StartTimer(BIRD_GPRS_TIMER_START, RJ_GPS_APP_10S, RJ_GPS_App_GpsStart);
    //Rj_stop_timer(BIRD_TASK_24REST_TIMER);
    //Rj_start_timer(BIRD_TASK_24REST_TIMER, 120*1000, Yd_24reset,NULL);
    RJ_GPS_StartTimer(BIRD_WDG_timer, 1*1000, bird_watchdog_control);
    StartTimer(BIRD_AT_TEST_TIMER, 2000, bird_At_test_flag);
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
    StartTimer(BIRD_CAN_SIMULATE_SEND_TIMER, 10000, can_simulate_timer);
#endif

    /*Added by Huangwenjian for dw11 board production 20160530 begin*/
    gpio_data = GPIO_ReadIO(18);
    kal_prompt_trace(MOD_SOC,  "RJ_GPS_App_GpsStart gpio_data18 = %d", gpio_data);
    if (gpio_data == 0)
    {
        g_gps_fwup=1;
        BD_GPS_Manufacture_INIT();
    }
    /*Added by Huangwenjian for dw11 board production 20160530 end*/

    Bird_Log_print(MOD_SOC, "Bird_start");
    ECU_UART_INIT();

#ifdef BIRD_BT_SUPPORT
    Vcama_Supply();
    //GPIO_ModeSetup(3, 0);
    //GPIO_InitIO(1, 3);
    //GPIO_WriteIO(1,3);	//BT_LDOEN
    bluetooth_init();
    RJ_GPS_StartTimer(BIRD_SPI_TEST, 50, spi_sent_bt);
#endif


#ifdef BIRD_BT_SUPPORT
    SetProtocolEventHandler(get_bluetooth_data,MSG_ID_YD_TK001_MSG_GET_BT_DATA);  //add by ryc
    SetProtocolEventHandler(yd_save_bt_mac,MSG_ID_BT_MAC_DATA_ALARM);
#endif

#endif

}

