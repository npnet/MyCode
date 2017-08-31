#include "Yd_run_static_event.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_uart_run_status=0;/*uart�˶�״̬��־λ*/
U8 g_lundong_state = 0;/*�ֶ�״̬*/

/*=============================ʹ�õ��ⲿ�����뺯��=============================*/
extern U8 rj_gps_stage;/* ���߼�����״̬��ʶ */
extern U8 oc_count;/*�˶����ɵ�����ʱ��ʱ���͵Ŀ��Ʊ�־*/
extern U8 g_n_sleeptimerbegin;/*���߶�ʱ��������־λ*/
extern U8 g_n_equstatue_count;/*���ͳ��ʱ��ֹ�Ĵ���*/

extern U16 bird_get_vibration_running_alarm_level();
extern void yd_tk001_alarm_vibration_count();
extern U16 bird_get_vibration_running_alarm_times_value();
extern void yd_tk001_wakeup_function();
extern void yd_tk001_post_alarm_vibration_ext();
extern U8 RJ_GPS_GetSensorStatus();
extern U8 bird_get_equmode();
extern U32 bird_get_alarm_delay_value();
#ifdef BIRD_ECU_MMI_SUPPORT
extern void yd_uart_run_status(U8 run_state);
#endif

void Yd_static_event()
{
       kal_prompt_trace(MOD_SOC,"Yd_static_event %d",rj_gps_stage); 

	#ifdef BIRD_ECU_MMI_SUPPORT
	if(rj_gps_stage == BIRD_STATUS_RUN)
	{
		yd_uart_run_status(0);
	}
	#endif
}

void Yd_run_event()
{
	U32 ntimer = 0;
       kal_prompt_trace(MOD_SOC,"Yd_run_event "); 
#ifdef __BIRD_YD_CTA__
	return;
#endif
	oc_count=0;

    	if(!g_vibration_is_timer_count_start)
    	{
       	g_vibration_count = 0;
        	g_vibration_is_timer_count_start = 1;
		ntimer = 1000*bird_get_vibration_running_alarm_level();
		Rj_stop_timer(BIRD_TASK_VIBRATION_TIMER_COUNT);
		Rj_start_timer(BIRD_TASK_VIBRATION_TIMER_COUNT, ntimer, yd_tk001_alarm_vibration_count,NULL);        	
    	}
    
    	g_vibration_count++;

    	if(g_vibration_count >= bird_get_vibration_running_alarm_times_value())
    	{
        	Rj_stop_timer(BIRD_TASK_YD_NORMAL_SLEEP_TIMER); /*�ص����߶�ʱ��*/
        	Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);/*�ص��Ͽ����Ӷ�ʱ��*/
        	Rj_stop_timer(RJ_TASK_GPS_RESET_TIMER);/*�ص�GPS������ʱ��*/
        	yd_tk001_alarm_vibration_count();
		g_n_sleeptimerbegin=0;
		g_n_equstatue_count = 1;
	
	#ifdef BIRD_ECU_MMI_SUPPORT
		if((rj_gps_stage != BIRD_STATUS_RUN)&&(get_uart_connect_status()==1))
		{
			yd_uart_run_status(1);
		}
		if(get_uart_connect_status()==1)
			return;
	#endif			
		if(rj_gps_stage != BIRD_STATUS_RUN)/*��ǰ״̬�������У���ת��Ϊ����״̬*/
		{
			yd_tk001_wakeup_function();/*����*/  
		}
        	/*��ʱ30�뱨������ֹ�û�30���ڳ���(�����ڳ��ϣ������𶯣����ܿ�򿪳���Կ�ף���ɳ���)*/
		if(g_vibration_is_timer_start == 0)
		{
			g_vibration_is_timer_start = 1;
			if(bird_get_equmode()==1)
			{
				yd_tk001_post_alarm_vibration_ext();
			}
			else
			{
				ntimer = 1000*bird_get_alarm_delay_value();
				StopTimer(BIRD_VIBRATION_TIMER);
				StartTimer(BIRD_VIBRATION_TIMER, ntimer, yd_tk001_post_alarm_vibration_ext);			
			}
		}		
    	}
}


void yd_set_static_event_msg()
{
	SetProtocolEventHandler(Yd_static_event,MSG_ID_YD_TK001_MSG_NORMAL_STATIC);  
}

void yd_set_run_event_msg()
{
	SetProtocolEventHandler(Yd_run_event,MSG_ID_YD_TK001_MSG_NORMAL_RUNNING);  
}

/*=============================   uart  ����  ==================================*/

void yd_uart_run()
{
	kal_prompt_trace(MOD_SOC,"yd_uart_run"); 

	Yd_run_for_wakeup_device();
	g_uart_run_status=1;	
}

void yd_uart_static()
{
	kal_prompt_trace(MOD_SOC,"yd_uart_static"); 

	g_uart_run_status=0;	
}

U8 get_uart_run_status()
{
	kal_prompt_trace(MOD_SOC,"get_uart_run_status %d",g_uart_run_status); 

	return g_uart_run_status;
}

void yd_set_uart_static_event_msg()
{
	SetProtocolEventHandler(yd_uart_static,MSG_ID_YD_TK001_MSG_ECU_STATIC);
}

void yd_set_uart_run_event_msg()
{
	SetProtocolEventHandler(yd_uart_run,MSG_ID_YD_TK001_MSG_ECU_RUNNING);
}

