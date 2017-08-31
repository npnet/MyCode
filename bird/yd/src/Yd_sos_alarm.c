#include "Yd_sos_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"

/*====================================ʹ�õ��ⲿ�����뺯��====================================*/
extern U8 rj_gps_stage;/* ���߼�����״̬��ʶ */

extern void yd_tk001_wakeup_function();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern void yd_tk001_post_soft_sf();
extern void bird_stop_sound_recording();
extern kal_uint8 bird_start_sound_recording();


void yd_sos_handle()
{
	kal_prompt_trace(MOD_SOC,"yd_sos_handle %d",rj_gps_stage);
	if(rj_gps_stage != BIRD_STATUS_RUN)/*��ǰ״̬�������У���ת��Ϊ����״̬*/
	{
		yd_tk001_wakeup_function();/*����*/  
	}
	bird_soc_sendalarm(TK001_ALARM_ENUM_sos);
}

void yd_set_sos_alarm_msg()
{
	SetProtocolEventHandler(yd_sos_handle,MSG_ID_YD_TK001_MSG_SOS_IS_ON);
}


