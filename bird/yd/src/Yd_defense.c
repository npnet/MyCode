#include "Yd_defense.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

// the flag to notify gps callback func to post msg to the main circle
kal_bool volatile b_gps_msg_post = KAL_FALSE;/*设防位置信息刷新控制标志位*/
RJ_Gps_Position yd_set_defense_position = {0};/*设防位置信息存储结构体*/

/*=============================   使用的外部变量与函数  ====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;

extern void yd_send_save_nv_msg();
extern void bird_soc_sendpos();
extern kal_bool is_ac_charger_in(void);
extern void bird_soc_set_sf_res(U8 rtn);
extern void bird_soc_set_cf_res(U8 rtn);
extern void bird_soc_set_defense_res(U8 rtn);
extern void yd_send_reboot_control_msg();


void yd_init_defense_param()
{
    yd_tk001_info.defense_flag = 0;
    yd_tk001_info.server_defense_flag=0;

    //默认为2ACC设防
    yd_tk001_info.auto_defense_flag = 2;
    yd_tk001_info.defense_delay = 120;	 //120s
}

U8 yd_tk001_get_defense()
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_get_defense flag: %d",yd_tk001_info.defense_flag);
    return yd_tk001_info.defense_flag;
}

U8 yd_tk001_get_server_defense()
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_get_server_defense flag: %d",yd_tk001_info.server_defense_flag);
    return yd_tk001_info.server_defense_flag;
}

void bird_set_defense_delay_value(U32 time)
{
    yd_tk001_info.defense_delay = time;
}

U32 bird_get_defense_delay_value()
{
    return yd_tk001_info.defense_delay;
}

void yd_tk001_defense_cancel(void)
{
    b_gps_msg_post = MMI_FALSE;
    memset(&yd_set_defense_position,0,sizeof(yd_set_defense_position));
}

void yd_tk001_defense_init(void)
{
    b_gps_msg_post = MMI_TRUE;
    memset(&yd_set_defense_position,0,sizeof(yd_set_defense_position));
}

void yd_tk001_defense_ini()
{
    if(yd_tk001_info.defense_flag != 1)
    {
        yd_tk001_defense_cancel();
    }
    else
    {
        yd_tk001_defense_init();
    }
}

void yd_tk001_set_defense(MMI_BOOL flag)
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_set_defense flag = %d",flag);
    if(flag)
    {
        if(yd_tk001_info.defense_flag != 1)
        {
            yd_tk001_info.defense_flag = 1;
            yd_tk001_defense_init();
        }
    }
    else
    {
        if(yd_tk001_info.defense_flag != 0)
        {
            yd_tk001_info.defense_flag = 0;
            yd_tk001_defense_cancel();
        }
    }
}

void yd_tk001_set_server_defense(U8 flag)
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_set_server_defense flag = %d",flag);
    if(flag==0)
    {
        yd_tk001_info.server_defense_flag = 0;
    }
    else if(flag==1)
    {
        yd_tk001_info.server_defense_flag = 1;
    }
    else if(flag==2)
    {
        yd_tk001_info.server_defense_flag = 2;
    }
    else if(flag==3)
    {
        yd_tk001_info.server_defense_flag = 3;
    }
    else if(flag==4)
    {
        yd_tk001_info.server_defense_flag = 4;
    }
}

MMI_BOOL yd_tk001_defense_is_set_by_server()
{
    if((yd_tk001_info.server_defense_flag==2)||(yd_tk001_info.server_defense_flag==3))
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

#ifdef DW23_PCB
extern void dw23_speaker_control(kal_uint8 soundnum);
#endif

void yd_tk001_post_soft_cf()
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_post_soft_cf defense = %d",yd_tk001_get_defense());

    if(yd_tk001_get_defense()==1)
    {
        yd_tk001_set_defense(MMI_FALSE);
        yd_send_save_nv_msg();
    }
    yd_tk001_set_server_defense(0);
    yd_send_save_nv_msg();
    bird_soc_sendpos();
}


void yd_tk001_post_soft_sf()
{
    kal_prompt_trace(MOD_SOC,"yd_tk001_post_soft_sf defense = %d",yd_tk001_get_defense());

    if(yd_tk001_get_defense()==0)
    {
        yd_tk001_set_defense(MMI_TRUE);
        yd_send_save_nv_msg();
        bird_soc_sendpos();
    }
}

/*============================= 平台消息 =============================*/
void bird_set_cf_res()
{
    U8 nrtn = 1;
    kal_prompt_trace(MOD_SOC,"bird_set_cf_res params");
    yd_tk001_post_soft_cf();
    if(yd_tk001_get_defense()==0)
    {
        nrtn = 1;
        if(!is_ac_charger_in())
        {
            nrtn = 2;
        }
    }
    else
    {
        nrtn = 0;
        if(!is_ac_charger_in())
        {
            nrtn = 3;
        }
    }
    bird_soc_set_cf_res(nrtn);
}

void bird_set_sf_res()
{
    U8 nrtn = 1;
    kal_prompt_trace(MOD_SOC,"bird_set_sf_res");
    yd_tk001_post_soft_sf();
    if(yd_tk001_get_defense()==1)
    {
        nrtn = 1;
        if(!is_ac_charger_in())
        {
            nrtn = 2;
        }
    }
    else
    {
        nrtn = 0;
        if(!is_ac_charger_in())
        {
            nrtn = 3;
        }
    }
    bird_soc_set_sf_res(nrtn);
}

//永久设防
void bird_set_defense_res(U8 rtn)
{
    U8 nrtn = 0;
    kal_prompt_trace(MOD_SOC,"bird_set_defense_res %d",rtn);

    if(rtn==2)
    {
        yd_tk001_set_defense(MMI_TRUE);
        yd_tk001_set_server_defense(2);
        yd_send_save_nv_msg();
        bird_soc_set_defense_res(1);
        bird_soc_sendpos();
    }
    else
    {
        bird_soc_set_defense_res(0);
    }
}

/*=============================  DW23 驱动消息  =============================*/
void yd_tk001_defend_on()
{
    Rj_stop_timer(BIRD_TASK_SET_DEFENSE_DELAY_TIMER);
    kal_prompt_trace(MOD_SOC,"yd_tk001_defend_on");

    if((yd_tk001_get_defense()==0)&&!yd_tk001_defense_is_set_by_server())
    {
        yd_tk001_set_defense(MMI_TRUE);
        yd_send_save_nv_msg();
        bird_soc_sendpos();
    }
}

void yd_tk001_defend_off()
{
    U8 acclock;
    kal_prompt_trace(MOD_SOC,"yd_tk001_defend_off");

    if((yd_tk001_get_defense()==1)&&!yd_tk001_defense_is_set_by_server())
    {
        yd_tk001_set_defense(MMI_FALSE);
        yd_send_save_nv_msg();
        bird_soc_sendpos();
    }
}

void yd_set_defend_on_msg()
{
    SetProtocolEventHandler(yd_tk001_defend_on,MSG_ID_AUX_DEFEND_OPEN);
}

void yd_set_defend_off_msg()
{
    SetProtocolEventHandler(yd_tk001_defend_off,MSG_ID_AUX_DEFEND_CLOSED);
}

#ifdef BIRD_ECU_MMI_SUPPORT
#include "Yd_uart_main.h"

//服务器下发设防/撤防时，根据波导uart 1.1处理函数
U8 g_defend_count=0;/*uart 设防消息处理次数*/
U8 g_defend_flag=0xff;/*uart 相应设防消息时的设防标志位*/
extern void GPS2ECU_UART_wirte_C9_CMD1(BIRD_UART_C9_INFO *c9_info);

void yd_ecu_defend_to_uart()
{
    BIRD_UART_C9_INFO c9_info;

    kal_prompt_trace(MOD_SOC," yd_ecu_defend_to_uart %d",g_defend_flag);

    memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));
    c9_info.defend_flag=1;
    c9_info.defend=g_defend_flag;
    GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void yd_ecu_defend_to_server(U8 flag)
{
    kal_prompt_trace(MOD_SOC," yd_ecu_defend_to_server %d",flag);
    if(flag==TO_SERVER_NORES)
    {
        if(g_defend_flag==1)
        {
            if(!is_ac_charger_in())
                bird_soc_set_sf_res(5);
            else
                bird_soc_set_sf_res(4);
        }
        else
        {
            if(!is_ac_charger_in())
                bird_soc_set_cf_res(5);
            else
                bird_soc_set_cf_res(4);
        }
    }
    else if(flag==TO_SERVER_NOK)
    {
        if(g_defend_flag==1)
        {
            if(!is_ac_charger_in())
                bird_soc_set_sf_res(3);
            else
                bird_soc_set_sf_res(0);
        }
        else
        {
            if(!is_ac_charger_in())
                bird_soc_set_cf_res(3);
            else
                bird_soc_set_cf_res(0);
        }
    }
    else if(flag==TO_SERVER_OK)
    {
        if(g_defend_flag==1)
        {
            if(!is_ac_charger_in())
                bird_soc_set_sf_res(2);
            else
                bird_soc_set_sf_res(1);
        }
        else
        {
            if(!is_ac_charger_in())
                bird_soc_set_cf_res(2);
            else
                bird_soc_set_cf_res(1);
        }
    }
}
void yd_ecu_defend_deal()
{
    ilm_struct       *rj_ilm = NULL;

    kal_prompt_trace(MOD_SOC,"yd_ecu_defend_deal %d",g_defend_count);

    if(g_defend_count<NORES_RESENDTIMES)
    {
        yd_ecu_defend_to_uart();
        g_defend_count++;
        StartTimer(BIRD_ECU_DEFEND_RESP, NORES_TIME*1000, yd_ecu_defend_deal);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"yd_ecu_defend_deal no response");
        yd_ecu_defend_to_server(TO_SERVER_NORES);
        g_defend_count=0;
        g_defend_flag=0xff;
        StopTimer(BIRD_ECU_DEFEND_RESP);
    }
}

void bird_ecu_defend_res(U8 defend)
{
    kal_prompt_trace(MOD_SOC,"bird_ecu_defend_res %d",defend);

    g_defend_count = 0;
    g_defend_flag = defend;
    StopTimer(BIRD_ECU_DEFEND_RESP);
    yd_ecu_defend_deal();
}

/*=============================  ECU uart 设防  =============================*/
extern kal_uint8 ECU_defend_status();

void yd_uart_defend_on()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_defend_on %d",ECU_defend_status());
    bird_set_soc_log_info(ECU_status_log,"def_on %x %x %x",ECU_defend_status(),yd_tk001_get_defense(),yd_tk001_defense_is_set_by_server());
    if(ECU_defend_status()==1)
    {
        if((yd_tk001_get_defense()==0)&&!yd_tk001_defense_is_set_by_server())
        {
            yd_tk001_set_defense(MMI_TRUE);
            yd_send_save_nv_msg();
            bird_soc_sendpos();
        }
    }
}

void yd_uart_defend_off()
{
    kal_prompt_trace(MOD_SOC,"yd_uart_defend_off %d",ECU_defend_status());
    bird_set_soc_log_info(ECU_status_log,"def_off %x %x",yd_tk001_get_defense(),yd_tk001_defense_is_set_by_server());
    if(ECU_defend_status()==1)
    {
        if((yd_tk001_get_defense()==1)&&!yd_tk001_defense_is_set_by_server())
        {
            yd_tk001_set_defense(MMI_FALSE);
            yd_send_save_nv_msg();
            bird_soc_sendpos();
        }
    }
}

void yd_set_uart_defend_on_msg()
{
    SetProtocolEventHandler(yd_uart_defend_on,MSG_ID_AUX_DEFEND_OPEN);
}

void yd_set_uart_defend_off_msg()
{
    SetProtocolEventHandler(yd_uart_defend_off,MSG_ID_AUX_DEFEND_CLOSED);
}

#endif
/*=============================  静音设防  =============================*/

void yd_mute_sf()
{
    kal_prompt_trace(MOD_SOC,"yd_mute_sf %d",yd_tk001_get_defense());

    if(yd_tk001_get_defense()==0)
    {
        yd_tk001_set_defense(MMI_TRUE);
        yd_send_save_nv_msg();
        bird_soc_sendpos();
    }
}

void yd_tk001_mute_defend_on()
{
    Rj_stop_timer(BIRD_TASK_SET_DEFENSE_DELAY_TIMER);
    kal_prompt_trace(MOD_SOC,"yd_tk001_mute_defend_on");

    if((yd_tk001_get_defense()==0)&&!yd_tk001_defense_is_set_by_server())
    {
        yd_tk001_set_defense(MMI_TRUE);
        yd_send_save_nv_msg();
        bird_soc_sendpos();
    }
}

void yd_set_mute_defend_on_msg()
{
    SetProtocolEventHandler(yd_tk001_mute_defend_on,MSG_ID_AUX_MUTE_DEFEND_OPEN);
}


/*=============================== 暂未使用 ===============================*/
//定义默认的设防模式0为软件 1为钥匙 2 ACC
#define YD_KEY_FLAG 2

U8 bird_is_auto_defense()
{
    return yd_tk001_info.auto_defense_flag;
}

U8 Yd_get_key_flag()
{
    return YD_KEY_FLAG;
}

U8 bird_auto_defense_alarm_flag(U8 flag)
{
    U8 oldflag = yd_tk001_info.auto_defense_flag;
    if(flag>2 || flag<0)
    {
        return 0;
    }
    if(flag !=0 && oldflag!=flag)
    {
        if(Yd_get_key_flag()==2) //Acc设防
        {
            if(flag==1)
            {
                return 2;
            }
            yd_tk001_info.auto_defense_flag = flag;
            return 0;
        }
        else
        {
            yd_tk001_info.auto_defense_flag = flag;
            if(oldflag==9)
            {
                return 0;
            }
            yd_send_reboot_control_msg();
            return 1;
        }
    }
    else if(flag ==0 )
    {
        yd_tk001_info.auto_defense_flag = flag;
    }
    return 0;
}

