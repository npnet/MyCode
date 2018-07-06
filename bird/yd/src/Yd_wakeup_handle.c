#include "Yd_wakeup_handle.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"


/*=============================   ʹ�õ��ⲿ�����뺯��====================================*/
extern U8 rj_gps_stage;/* ���߼�����״̬��ʶ */
extern U8 g_n_sleeptimerbegin;/*���߶�ʱ��������־λ*/
extern U8 g_n_sleepcount;
extern U8 g_n_equstatue_count;/*��̴���ʱ,������ͳ�ƴ���*/
extern U8 g_n_ydislogin;/*��������½�ɹ���־*/
extern U16 g_n_readunsend_count;/*������δ�������*/
extern U8 g_n_readunsend_end;/*������δ���������־λ*/
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;/*�źŵ�*/
extern U8 g_n_is_gps_close;
extern U8 g_acc_close_gps;
extern U8 g_acc_sleep_status;

extern Yd_main(void);
extern void bird_set_normal_sleep_state(U8 flag);
extern U8 yd_tk001_get_reboot_flag(void);
extern void yd_send_save_nv_msg(void);
extern BOOL Lima_get_soc_conn_flag(void);
extern BOOL Lima_get_soc_init_flag(void);
extern void Bird_soc_conn(void);
extern void yd_send_gps_open_msg(void);
extern kal_bool is_ac_charger_in(void);
extern void Yd_set_all_msg_handler();


void yd_init_wakeup_param()
{
    yd_tk001_info.WAKEUP= 1;
    yd_tk001_info.WAKEUPT= 60;
}

void bird_set_wakeup_value(U8 value)
{
    yd_tk001_info.WAKEUP= value;
}

U8 bird_get_wakeup_value()
{
    return yd_tk001_info.WAKEUP;
}

void bird_set_wakeupt_value(U32 value)
{
    yd_tk001_info.WAKEUPT= value;
}

U32 bird_get_wakeupt_value()
{
    return yd_tk001_info.WAKEUPT;
}


void yd_tk001_wakeup_function()
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_wakeup_function rj_gps_stage = %d",rj_gps_stage);
    Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
    Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER);
    Rj_stop_timer(BIRD_TASK_OC_MAIN_HEART_TIMER);
    Rj_stop_timer(BIRD_TASK_OC_RUN_TIMER);

    if(g_n_ydislogin != 1)
        return;

    if(bird_get_normal_sleep_state())
    {
        g_n_readunsend_end=0; //read unsend pos data
        g_n_readunsend_count = 0;
        g_n_sleepcount=0;
        Yd_set_all_msg_handler();
        bird_set_normal_sleep_state(MMI_FALSE);
        rj_led_status_info.b_GPS_IS_CLOSE = KAL_FALSE;

        if (Lima_get_soc_init_flag() == FALSE)
        {
            Bird_soc_conn();
        }
        rj_led_status_info.b_GPS_IS_CLOSE = KAL_FALSE;
        rj_led_status_info.b_GSM_IS_CLOSE = KAL_FALSE;
        rj_led_status_info.b_SIM_IS_CLOSE = KAL_FALSE;

        rj_gps_stage = BIRD_STATUS_RUN;
        Rj_stop_timer(Bird_task_main_Timer);
        Rj_start_timer(Bird_task_main_Timer, RJ_GPS_APP_15S, Yd_main,NULL);
    }
    else if(rj_gps_stage != BIRD_STATUS_RUN)
    {
        rj_gps_stage = BIRD_STATUS_RUN;
        g_n_sleeptimerbegin=0;
        Rj_stop_timer(Bird_task_main_Timer);
        Rj_start_timer(Bird_task_main_Timer, RJ_GPS_APP_15S, Yd_main,NULL);
    }
    yd_tk001_set_reboot_flag(0);
    //yd_tk001_set_record_from_NV();
    //try open gps
//	if(rj_led_status_info.b_GPS_IS_CLOSE == KAL_TRUE )
//	{
    g_n_is_gps_close=0;
    g_acc_close_gps=0;
    g_acc_sleep_status=0;
    yd_send_gps_open_msg();
//	}
}

void Yd_run_for_wakeup_device()
{
    Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER); /*�ص����߶�ʱ��*/
    Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);/*�ص��Ͽ����Ӷ�ʱ��*/
    Rj_stop_timer(RJ_TASK_GPS_RESET_TIMER);/*GPS������ʱ��*/
    g_n_sleeptimerbegin = 0;
    g_n_equstatue_count = 1;
    kal_prompt_trace(MOD_SOC,"Yd_run_for_wakeup_device rj_gps_stage = %d",rj_gps_stage);
    if(rj_gps_stage != BIRD_STATUS_RUN)/*��ǰ״̬�������У���ת��Ϊ����״̬*/
    {
        yd_tk001_wakeup_function();/*����*/
    }
}

