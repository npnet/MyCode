#include "Yd_power_off.h"
#include "Bird_app.h"
#include "Yd_main.h"

/*=============================   使用的外部变量与函数====================================*/
#ifdef DW02_PROJECT
extern void rj_volt_poweroff();
#endif
extern void bird_set_normal_sleep_state(U8 flag);
extern BOOL Lima_get_soc_init_flag();
extern void Bird_soc_conn();
extern void bird_soc_sendpos();
extern kal_uint32 bird_get_bat_channel_voltage(void);
extern kal_bool is_ac_charger_in(void);

void yd_tk001_power_off_handler()
{
	kal_prompt_trace(MOD_SOC,"yd_tk001_power_off_handler");  
#if defined(BIRD_EXTERNEL_POWER_UNSUPPORT)
	return;
#else
	StartTimer(BIRD_POWEROFF_TIMER, RJ_GPS_APP_10S, rj_volt_poweroff);
	return;
#endif
}


void yd_uart_shutdown()
{
	kal_prompt_trace(MOD_SOC,"yd_uart_shutdown %d",is_ac_charger_in());
	if(!is_ac_charger_in())
	{
	#ifndef BIRD_EXTERNEL_POWER_UNSUPPORT      	 	
		rj_volt_poweroff();
 	#endif
	}
}

void yd_set_power_off_msg()
{
}

void yd_set_uart_power_off_msg()
{
	SetProtocolEventHandler(yd_uart_shutdown,MSG_ID_YD_TK001_MSG_CLOSE_ALARM);
}

