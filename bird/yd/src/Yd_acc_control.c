#include "Yd_acc_control.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_acc_close_gps=0;/*acc关闭GPS的处理标志位*/
U8 g_acc_sleep_status=0;/*acc休眠状态标志位 (心跳过度休眠和心跳关闭GPS时，终端非运行ACC断开则置1，初始化与唤醒置0)*/

/*====================================使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
extern U8 rj_gps_stage; /* 主逻辑运行状态标识 */
extern U8 g_n_is_gps_close;

extern U32 bird_get_defense_delay_value();
extern MMI_BOOL yd_tk001_defense_is_set_by_server();
extern void yd_tk001_set_defense(MMI_BOOL flag);
extern void yd_send_save_nv_msg();
extern void bird_soc_sendpos();
extern void bird_soc_sendlc(U8 type);
extern void bird_set_car_state(U8 flag);
#ifdef DW23_PCB
extern void dw23_speaker_control(kal_uint8 soundnum);
#endif
extern void bird_oilcut_accsleep_handler();
#ifdef __BIRD_LED_IDENTIFY_WINK__
extern void Bird_LED_Identify_Wink(void);
#endif
extern void RJ_GPS_Gsmled_LightWink();
extern void Yd_logouttxbox();
extern void Yd_tbox_heart();
extern void Yd_main();

void yd_init_acc_control_param()
{
	yd_tk001_info.acc_state=0;
	yd_tk001_info.ACCLOCK = 1;
}

void bird_set_acc_state(U8 flag)
{
	yd_tk001_info.acc_state = flag;
}

U8 bird_get_acc_state()
{
	return yd_tk001_info.acc_state;
}

U8 bird_get_acclock_value()
{
	return yd_tk001_info.ACCLOCK;
}

void bird_set_acclock_value(U8 mvalue)
{
	yd_tk001_info.ACCLOCK= mvalue;
}

MMI_BOOL get_Acc_onoff()
{
	if((bird_get_acc_state()==1))
		return MMI_TRUE;
	else
		return MMI_FALSE;
}

#ifdef BIRD_ECU_MMI_SUPPORT

extern kal_uint8 ECU_defend_status();
extern kal_uint8 ECU_ACC_status();

void yd_uart_acc_off()
{
	kal_prompt_trace(MOD_SOC,"yd_uart_acc_off %d",ECU_ACC_status());

	if(ECU_ACC_status()==1)
	{	
		if(bird_get_acc_state()==1)
		{
			bird_set_acc_state(0);
			if((ECU_defend_status()==0)&&!yd_tk001_defense_is_set_by_server())
				yd_tk001_set_defense(MMI_TRUE);	
			yd_send_save_nv_msg();
			bird_soc_sendpos();
			bird_soc_sendlc(2);
		}
	}
}

void yd_uart_acc_on()
{
	kal_prompt_trace(MOD_SOC,"yd_uart_acc_on %d",ECU_ACC_status());

	if(ECU_ACC_status()==1)
	{	
		if(bird_get_acc_state()==0)
		{
			bird_set_acc_state(1);
			if((ECU_defend_status()==0)&&!yd_tk001_defense_is_set_by_server())
				yd_tk001_set_defense(MMI_FALSE);	
			yd_send_save_nv_msg();		
			bird_soc_sendpos();
			bird_soc_sendlc(2);
		}
	}
}

void yd_drv_acc_off()
{
	kal_prompt_trace(MOD_SOC,"yd_drv_acc_off %d",ECU_ACC_status());

	if(ECU_ACC_status()==0)
	{	
		if(bird_get_acc_state()==1)
		{
			bird_set_acc_state(0);
			if((ECU_defend_status()==0)&&!yd_tk001_defense_is_set_by_server())
				yd_tk001_set_defense(MMI_TRUE);	
			yd_send_save_nv_msg();		
			bird_soc_sendpos();
		}
	}
}

void yd_drv_acc_on()
{
	kal_prompt_trace(MOD_SOC,"yd_drv_acc_on %d",ECU_ACC_status());

	if(ECU_ACC_status()==0)
	{	
		if(bird_get_acc_state()==0)
		{
			bird_set_acc_state(1);
			if((ECU_defend_status()==0)&&!yd_tk001_defense_is_set_by_server())
				yd_tk001_set_defense(MMI_FALSE);	
			yd_send_save_nv_msg();		
			bird_soc_sendpos();
		}
	}
}

#endif

void yd_tk001_post_alarm_key_on()
{
	Rj_stop_timer(BIRD_TASK_SET_DEFENSE_DELAY_TIMER);
	kal_prompt_trace(MOD_SOC,"yd_tk001_post_alarm_key_on");
 
	if((bird_get_acc_state()==0))
	{
		bird_set_acc_state(1);
		yd_send_save_nv_msg();		
		bird_soc_sendpos();
	}
	if((yd_tk001_get_defense()==1)&&!yd_tk001_defense_is_set_by_server())
	{
		yd_tk001_set_defense(MMI_FALSE);
		yd_send_save_nv_msg();
		bird_soc_sendpos();
	}

}

void yd_tk001_post_alarm_key_off()
{
	U8 acclock;
	acclock = bird_get_acclock_value();
	kal_prompt_trace(MOD_SOC,"yd_tk001_post_alarm_key_off %d",acclock);

	if(acclock==1)
	{	
		if((yd_tk001_get_defense()==0)&&!yd_tk001_defense_is_set_by_server())
		{
			yd_tk001_set_defense(MMI_TRUE);
			yd_send_save_nv_msg();			
			bird_soc_sendpos(); 
		}
	}	
}

void yd_acc_key_open()
{
	kal_prompt_trace(MOD_SOC,"yd_acc_key_open"); 

	Rj_stop_timer(Bird_task_heart_Timer); 
	Rj_start_timer(Bird_task_heart_Timer, 1*1000, Yd_tbox_heart,NULL);
	Rj_stop_timer(Bird_task_main_Timer); 
	Rj_start_timer(Bird_task_main_Timer, 1*1000, Yd_main,NULL);
}

void yd_acc_key_closed()
{
	kal_prompt_trace(MOD_SOC,"yd_acc_key_closed"); 

	Rj_stop_timer(Bird_task_heart_Timer); 
	Rj_stop_timer(Bird_task_main_Timer); 
	Yd_logouttxbox();
}


#ifdef BIRD_ECU_MMI_SUPPORT
void yd_set_ECU_command_acc_on_msg()
{
	SetProtocolEventHandler(yd_uart_acc_on,MSG_ID_YD_TK001_MSG_ECU_ACC_ON);
}

void yd_set_ECU_command_acc_off_msg()
{
	SetProtocolEventHandler(yd_uart_acc_off,MSG_ID_YD_TK001_MSG_ECU_ACC_OFF);
}

void yd_set_ECU_key_on_msg()
{
	SetProtocolEventHandler(yd_drv_acc_on,MSG_ID_AUX_KEY_OPEN);
}

void yd_set_ECU_key_off_msg()
{
	SetProtocolEventHandler(yd_drv_acc_off,MSG_ID_AUX_KEY_CLOSED);
}
#endif

void yd_set_aux_key_on_msg()
{
	SetProtocolEventHandler(yd_tk001_post_alarm_key_on,MSG_ID_AUX_KEY_OPEN);
}

void yd_set_aux_key_off_msg()
{
	SetProtocolEventHandler(yd_tk001_post_alarm_key_off,MSG_ID_AUX_KEY_CLOSED);
}

void yd_set_acc_key_closed_msg()
{
	SetProtocolEventHandler(yd_acc_key_closed,MSG_ID_ACC_KEY_CLOSED);
}

void Yd_acc_close_gps()
{	
	kal_prompt_trace(MOD_SOC," Yd_acc_close_gps %d",rj_gps_stage);

	if((g_n_is_gps_close==0)&&(Yd_get_equstatus()==0)&&(!get_Acc_onoff()))
	{
		g_n_is_gps_close=1;
		g_acc_sleep_status=1;
		
	#ifdef BIRD_OC_LOGIC
		bird_oilcut_accsleep_handler();
	#endif
	
		rj_led_status_info.b_GPS_IS_CLOSE = KAL_TRUE;
		kal_prompt_trace(MOD_SOC," Yd_acc_close_gps close_gps acc = %d",get_Acc_onoff());
		yd_send_gps_close_msg();
		
	#ifdef __BIRD_LED_IDENTIFY_WINK__
		Bird_LED_Identify_Wink();
	#else
		RJ_GPS_Gsmled_LightWink();
	#endif
	
	}
}


