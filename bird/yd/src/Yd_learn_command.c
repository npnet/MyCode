#include "Yd_learn_command.h"
#include "Bird_app.h"
#include "Yd_main.h"

U32 g_learn_id=0;

/*===============================使用的外部变量与函数====================================*/
extern void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id);

void Yd_learn_deal()
{
	kal_prompt_trace(MOD_SOC," Yd_learn_deal %d ",g_learn_id);

	StopTimer(BIRD_LEARN_TIMER);
	bird_soc_set_param_res("LEARN=1",0,g_learn_id);		
}

void Yd_learn_res(U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_learn_res %d",id);
	
       g_learn_id=id;
	StopTimer(BIRD_LEARN_TIMER);
	StartTimer(BIRD_LEARN_TIMER, 30*1000, Yd_learn_deal);
}

void yd_learn_response()
{
	kal_prompt_trace(MOD_SOC," yd_learn_response %d ",g_learn_id);

	StopTimer(BIRD_LEARN_TIMER);
	bird_soc_set_param_res("LEARN=1",1,g_learn_id);		
}

void yd_set_learn_ok_msg()
{
	SetProtocolEventHandler(yd_learn_response,MSG_ID_REMOTER_LEARN_SUCCESS);
}

