#include "Bird_oilcut.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 oc_count=0;/*运动过渡到心跳时延时断油的控制位*/
/*=============================   使用的外部变量与函数====================================*/
extern U8 g_acc_sleep_status;

#ifdef BIRD_OC_LOGIC
extern void OCIN_INI();
extern void OCIN_change(U8 ocin);
#endif
extern void yd_send_save_nv_msg(void);
extern void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id);
extern void bird_soc_sendpos();
extern MMI_BOOL get_Acc_onoff();


void bird_init_oilcut_param(void)
{
    yd_tk001_info.oc_info.oc_state= 0;
    yd_tk001_info.oc_info.instru_id= 0;
    yd_tk001_info.oc_info.exec_state= OC_SET_SUCCESS;
    yd_tk001_info.oc_main_heart_time=60;
}

void bird_set_ocin_state(U8 flag)
{
    yd_tk001_info.oc_info.oc_state=flag;
}

U8 bird_get_ocin_state(void)
{
    return yd_tk001_info.oc_info.oc_state;
}

void bird_set_oc_instruct(U32 instruct)
{
    yd_tk001_info.oc_info.instru_id=instruct;
}

U32 bird_get_oc_instruct(void)
{
    return yd_tk001_info.oc_info.instru_id;
}

void bird_set_oc_exec_state(U8 flag)
{
    yd_tk001_info.oc_info.exec_state=flag;
}

U8 bird_get_oc_exec_state(void)
{
    return yd_tk001_info.oc_info.exec_state;
}

void bird_set_oc_main_heart_time(U8 flag)
{
    yd_tk001_info.oc_main_heart_time=flag;
}

U8 bird_get_oc_main_heart_time(void)
{
    return yd_tk001_info.oc_main_heart_time;
}

void bird_ocin_init(void)
{
#ifdef BIRD_OC_LOGIC
    OCIN_INI();
    if(bird_get_ocin_state()==1)
    {
        kal_prompt_trace(MOD_SOC,"restart OC_ABNORMAL ");
        bird_set_oc_exec_state(OC_ABNORMAL);
        yd_send_save_nv_msg();
    }
#else
    bird_set_dy_state(bird_get_ocin_state());//add by wtl 20160918
#endif
    kal_prompt_trace(MOD_SOC,"bird_ocin_init");
}

void bird_set_dy_state(U8 flag)
{
#ifdef BIRD_OC_LOGIC
    if(flag==1)
        OCIN_change(1);
    else
        OCIN_change(0);
#endif
}

void Yd_oc_start_handle(void)
{
    kal_prompt_trace(MOD_SOC,"Yd_oc_start_handle entry");

    if(bird_get_ocin_state()==1)
    {
        if((bird_get_oc_exec_state()==OC_SET_SUCCESS)||(bird_get_oc_exec_state()==OC_EXECUTED))
        {
            kal_prompt_trace(MOD_SOC,"Yd_oc_start_handle OC executed");
            //bird_set_dy_state(1);
        }
        else
        {
            kal_prompt_trace(MOD_SOC,"Yd_oc_start_handle OC to execute");
            bird_set_dy_state(1);
            bird_set_oc_exec_state(OC_EXECUTED);
            yd_send_save_nv_msg();
            bird_soc_set_param_res("OC_ENABLE=1",OC_EXECUTED,bird_get_oc_instruct());
            bird_soc_sendpos();
        }
    }
}

void Yd_oc_heart_handle(void)
{
    kal_prompt_trace(MOD_SOC,"Yd_oc_heart_handle %d",oc_count);

    oc_count++;

    if(oc_count>10)
    {
        Rj_stop_timer(BIRD_TASK_OC_MAIN_HEART_TIMER);
        oc_count=0;
        Yd_oc_start_handle();

        Rj_stop_timer(BIRD_TASK_OC_RUN_TIMER);
        Rj_start_timer(BIRD_TASK_OC_RUN_TIMER,1000,Yd_oc_heart_handle,NULL);
    }
    else
    {
        Rj_start_timer(BIRD_TASK_OC_RUN_TIMER,1000,Yd_oc_heart_handle,NULL);
    }
}

void bird_oilcut_accsleep_handler(void)
{
    kal_prompt_trace(MOD_SOC,"bird_oilcut_accsleep_handler entry");

    if((g_acc_sleep_status==1)&&(!get_Acc_onoff())&&(bird_get_ocin_state()==1))
    {
        if((bird_get_oc_exec_state()==OC_SET_SUCCESS)||(bird_get_oc_exec_state()==OC_EXECUTED))
        {
            kal_prompt_trace(MOD_SOC,"bird_oilcut_accsleep_handler OC executed");
        }
        else
        {
            kal_prompt_trace(MOD_SOC,"bird_oilcut_accsleep_handler OC to execute");
            bird_set_dy_state(1);
            bird_set_oc_exec_state(OC_EXECUTED);
            yd_send_save_nv_msg();
            bird_soc_set_param_res("OC_ENABLE=1",OC_EXECUTED,bird_get_oc_instruct());
            bird_soc_sendpos();
        }
    }
}

void bird_oc_logic_res(void)
{
    if(bird_get_oc_exec_state()==OC_EXECUTED)
    {
        kal_prompt_trace(MOD_SOC,"OC_EXECUTED");
    }
    else
    {
        if((g_acc_sleep_status==1)&&(!get_Acc_onoff()))
        {
            kal_prompt_trace(MOD_SOC,"OC_SET_SUCCESS");
            bird_set_dy_state(1);
            bird_set_oc_exec_state(OC_SET_SUCCESS);
        }
        else if((bird_get_oc_exec_state()==OC_TO_BE_EXEC)||(bird_get_oc_exec_state()==OC_ABNORMAL)||(bird_get_ocin_state()==0))
        {
            kal_prompt_trace(MOD_SOC,"OC_TO_BE_EXEC");
            bird_set_oc_exec_state(OC_TO_BE_EXEC);
        }
    }
}

void bird_ocin_state_res(U8 flag)
{
    if(bird_get_ocin_state() != flag)
    {
        bird_set_ocin_state(flag);
        bird_soc_sendpos();
        if(1 == flag)
            kal_prompt_trace(MOD_SOC,"OC_ENABLE sendpos");
        else
            kal_prompt_trace(MOD_SOC,"OC_DISABLE sendpos");
    }
}

