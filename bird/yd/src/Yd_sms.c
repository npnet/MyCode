/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:
  Author:       Version:        Date: 2013-05-29
  Description:   The YD service message processing module
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/
#include "Yd_sms.h"
#include "Yd_main.h"
#include "Bird_app.h"
#include "Bird_task.h"

#ifdef __MMI_UCM__
#include "UCMGProt.h"
#include "UcmCoreProt.h"
#endif

S8 yd_tk001_temp_num[YD_TK001_PNONE_NUM];
S8 yd_tk001_msg[RJ_GPS_SND_BUFFER_SIZE];

U8 g_n_sms_schye = 0;
U8 g_n_sms_schye_getsms = 0;
U8 yd_tk001_user_number[YD_TK001_PHONE_LEN];
U8 yd_tk001_new_psw[YD_TK001_PHONE_LEN];

extern volatile kal_int8 count_judge_gps_app_timer ;
extern volatile kal_int8 count_judge_gps_send_timer ;
extern U8 g_n_ydischangesim;
extern U16 RJ_GPS_send_sms_internal(S8* pNumber, S8* pContent);

kal_bool yd_tk001_Check_Condition(smslib_general_struct* data)
{
    kal_bool ret = KAL_TRUE;

    if (data == NULL || data ->forMMI_UserData_length == 0 )
    {
        ret = KAL_FALSE;
    }
    else  if(strcmp(data->forMMI_TPAddr.number, "10086") == 0 || data->forMMI_TPAddr.number == 0 ||strcmp(data->forMMI_TPAddr.number, "10010") == 0 )
    {
        kal_prompt_trace(  MOD_SOC,"RJ_GPS_DealWith_Msg 10086 or 10010 ");
        ret = KAL_FALSE ;
    }

    return ret;
}

MMI_BOOL yd_tk001_is_admin_number(U8 *number)
{

    if(strcmp((const char *)number, (const char *)yd_tk001_info.admin_number) == 0)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

MMI_BOOL yd_tk001_DealWith_TJGL(U8 *number)
{
    if(strcmp((const char *)number, (const char *)yd_tk001_info.admin_number) == 0)
    {
        return MMI_FALSE;
    }
    else
    {
        memset(yd_tk001_info.admin_number, 0x00, YD_TK001_PHONE_LEN);
        strcpy(yd_tk001_info.admin_number, (const char *)number);
        return MMI_TRUE;
    }
}

//auto call
void auto_RingOff(void)
{
    kal_prompt_trace(MOD_SOC,"auto_RingOff entry");

    if (mmi_ucm_single_call_option(SRV_UCM_END_SINGLE_ACT, SRV_UCM_OUTGOING_STATE, MMI_UCM_EXEC_IF_PERMIT_PASS) == SRV_UCM_RESULT_OK)
    {
        mmi_ucm_outgoing_call_endkey();
    }
    if (mmi_ucm_single_call_option(SRV_UCM_END_SINGLE_ACT, SRV_UCM_CALL_STATE_CONNECTED, MMI_UCM_EXEC_IF_PERMIT_PASS) == SRV_UCM_RESULT_OK)
    {
        mmi_ucm_end_single_action();
    }
}


void yd_tk001_CallOwer()
{
    U16 czphone[YD_TK001_PHONE_LEN] ;
    mmi_ucm_make_call_para_struct make_call_para;

    if(bird_get_utility_number()==NULL)
    {
        return;
    }
    memset(czphone, 0x00, YD_TK001_PHONE_LEN);
    mmi_asc_to_ucs2((char *)czphone,(char *)bird_get_utility_number());
    kal_prompt_trace(MOD_SOC,"yd_tk001_CallOwer czphone = %s ",czphone);
    mmi_ucm_init_call_para(&make_call_para);
    make_call_para.dial_type = MMI_UCM_VOICE_CALL_TYPE;
    make_call_para.ucs2_num_uri =czphone;
    mmi_ucm_call_launch(0, &make_call_para);
    Rj_stop_timer(Bird_task_ringoff_timer);
    Rj_start_timer(Bird_task_ringoff_timer, 30*1000, auto_RingOff,NULL);
}

void yd_tk001_Send_Msg(S8* msg_data, S8*num)
{
    S8 temp[RJ_GPS_MSG_MAX_LENGTH+1];

    memset(temp, 0, sizeof(temp));

    RJ_GPS_chset_text_to_ucs2_str(temp,RJ_GPS_MSG_MAX_LENGTH,(U8 *)msg_data);
    RJ_GPS_send_sms_internal((S8*)num, temp);
    RJ_GPS_log( "yd_tk001_Send_Msg");

}

/*huangbx 2013.04.17 start */
S8 tempyd[RJ_GPS_MSG_MAX_LENGTH+1];

void yd_tk001_Send_Msg_admin(S8* msg_data)
{
    memset(tempyd, 0, sizeof(tempyd));

    kal_prompt_trace(MOD_SOC, "yd_tk001_Send_Msg_admin %s", msg_data);

    //mmi_chset_convert(MMI_CHSET_GB2312, MMI_CHSET_UCS2, (char *)msg_data, (char *)tempyd, (50+ 1)*ENCODING_LENGTH);

    mmi_chset_convert(MMI_CHSET_GB2312, MMI_CHSET_UCS2, (char *)msg_data, (char *)tempyd, (strlen((char *)msg_data) + 1)*ENCODING_LENGTH);
    //mmi_chset_convert(MMI_CHSET_GB2312, MMI_CHSET_UCS2, (char *)msg_data, (char *)tempyd, sizeof(tempyd));
    kal_prompt_trace(MOD_SOC, "yd_tk001_Send_Msg_admin %s",yd_tk001_temp_num);
    RJ_GPS_send_sms_internal((S8*)yd_tk001_temp_num, tempyd);
    //RJ_GPS_Send_Msg_to_user(tempyd, yd_tk001_temp_num);
}
/*huangbx 2013.04.17 end */


/*-------------------------------------------------------
return status
-------------------------------------------------------*/
void yd_tk001_DealWith_returnStatus()
{
    U8 nconn = Lima_get_soc_conn_flag();

    memset(yd_tk001_msg,0,sizeof(yd_tk001_msg));
    RJ_GPS_sprintf(yd_tk001_msg,"\x43\x4F\x4E\x4E%d\x54\x49\x4D\x45\x52%d\x53\x45\x4E\x44%d\x45\x52\x52%d%d\x52\x45\x53\x54\x49\x4D\x45%d\x53\x49\x47%d\x52\x55\x4E%d\x4C\x4F\x47\x49\x4E\x3A%d",nconn,count_judge_gps_app_timer,count_judge_gps_send_timer,Bird_get_lastreseterr(),Bird_get_reseterr(),Yd_get_restarttimes(),
                   bird_sim_signal_strength_in_percentage,rj_gps_stage,bird_get_islogin());
    RJ_GPS_Send_Msg_to_user(yd_tk001_msg, yd_tk001_temp_num);
}

void Yd_dw_delay()
{
    RJ_GPS_Send_Msg_to_user("\xE8\xBF\x94\xE5\x9B\x9E\xE5\xBD\x93\xE5\x89\x8D\xE4\xBD\x8D\xE7\xBD\xAE\xE4\xBF\xA1\xE6\x81\xAF\xE5\xA4\x84\xE7\x90\x86\xE8\xB6\x85\xE6\x97\xB6\x2C\xE8\xAF\xB7\xE7\xA8\x8D\xE5\x80\x99\xE5\x86\x8D\xE8\xAF\x95",
                            yd_tk001_temp_num);
}
MMI_BOOL yd_tk001_Is_authorize_number(U8 * number)
{

    kal_prompt_trace(  MOD_SOC,"[temp_number1]  %s", number);
    kal_prompt_trace(  MOD_SOC,"[authorize_number2]  %s", yd_tk001_info.authorize_number);

    if(strcmp((const char *)bird_get_utility_number(), ( const char *)number) == 0)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


void yd_tk001_active_sms_timeout()
{
    RJ_GPS_Send_Msg_to_user("\xE4\xB8\x9A\xE5\x8A\xA1\xE6\xB3\xA8\xE5\x86\x8C\xE5\xA4\xB1\xE8\xB4\xA5\xEF\xBC\x8C\xE7\xBB\x88\xE7\xAB\xAF\x53\x49\x4D\xE5\x8D\xA1\xE5\x8F\xB7\xE9\xAA\x8C\xE8\xAF\x81\xE8\xB6\x85\xE6\x97\xB6\xE3\x80\x82",
                            yd_tk001_user_number);
    memset(yd_tk001_user_number, 0x00, sizeof(yd_tk001_user_number));
}

U8 yd_check_str_is_number(U8 *ptr)
{
    U8 i;
    U8 *p = ptr;

    for(i = 0 ; i<strlen((const char *) p); i++)
    {
        if(*(p+i)<'0'||*(p+i)>'9')
            return 0;
    }
    return 1;

}

U8 RJ_GPS_is_first_str_in(const char *str1, const char *str2)
{
    char *p = strstr(str1,str2);
    int cnt = 0;

    kal_prompt_trace(  MOD_SOC,"[____str1___%s]",str1);
    kal_prompt_trace(  MOD_SOC,"[____str2___%s]",str2);

    if((cnt=(int)(p-str1)) == 0)
    {
        //kal_prompt_trace(  MOD_SOC,"[____RJ_GPS_is_first_str_in_______0___%d]",cnt);
        return 0;
    }
    else
    {
        //kal_prompt_trace(  MOD_SOC,"[____RJ_GPS_is_first_str_in______1____%d]",cnt);
        return 1;
    }
}
void Yd_sendregres(U8 nflag)
{
    kal_prompt_trace(MOD_SOC," Yd_sendregres %d",nflag);
    if(nflag==1)
    {
        RJ_GPS_Send_Msg_to_user("\xE6\xB3\xA8\xE5\x86\x8C\xE6\x88\x90\xE5\x8A\x9F\xEF\xBC\x8C\xE6\x82\xA8\xE5\x8F\xAF\xE5\x9B\x9E\xE5\xA4\x8D\x44\x57\xE8\x8E\xB7\xE5\x8F\x96\xE5\xAE\x9E\xE6\x97\xB6\xE4\xBD\x8D\xE7\xBD\xAE", bird_get_authorize_number()); /*??????*/
    }
    else
    {
        memset(yd_tk001_msg,0,sizeof(yd_tk001_msg));
        RJ_GPS_sprintf(yd_tk001_msg,"\xE6\xB3\xA8\xE5\x86\x8C\xE5\xA4\xB1\xE8\xB4\xA5\xEF\xBC\x8C\xE9\x94\x99\xE8\xAF\xAF\xE4\xBB\xA3\xE7\xA0\x81\xE4\xB8\xBA%d",nflag);
        RJ_GPS_Send_Msg_to_user(yd_tk001_msg, bird_get_authorize_number());
    }

}

void Yd_sendgetdwres(S8* add)
{
    Rj_stop_timer(BIRD_TASK_DW_TIMER);
    yd_tk001_Send_Msg_admin(add);

}

U8 yd_tk001_all_Msg(smslib_general_struct* data)
{
    U8 temp[170] = {0};
    U8 str1[40] = {0};
    U8 str2[40] = {0};
    U8 str3[40] = {0};
    U8 str4[40] = {0};
    U8 str5[40] = {0};
    U8 str6[40] = {0};
    U8 str7[40] = {0};
    U8 str8[40] = {0};
    U8 str9[40] = {0};
    U16 i = 0;
    U16 sms_len;
    U8 sms_post_dest[300] = {0};
    S8 *begin_offset=NULL;
    S8 *end_offset=NULL;
    kal_uint8 *ye;
    S32 len=0;
    S32 len1=0;
    S8 * psplit=NULL;
    S8 p1[50];
    S8 * p2=NULL;
    U8 ndyflag=0;
    U8 nlan = 0;
    FS_HANDLE data_file;
    BIRD_id_struct    *bird_id_data = NULL;

    bird_id_data = (BIRD_id_struct*)
                   construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
    bird_id_data->Is_init_post = 0;

    kal_prompt_trace(  MOD_SOC,"yd_tk001_all_Msg");

    memset(yd_tk001_temp_num, 0, sizeof(yd_tk001_temp_num));
    if(data->forMMI_TPAddr.number[0] == '8' && data->forMMI_TPAddr.number[1] == '6')
    {
        kal_prompt_trace(  MOD_SOC,"4444444444");

        for(i = 0 ; i < strlen(data->forMMI_TPAddr.number); i++ )
        {
            yd_tk001_temp_num[i] = data->forMMI_TPAddr.number[i+2];
            if(i>=15)
            {
                break;
            }
        }
    }
    else
    {
        if(strlen(data->forMMI_TPAddr.number)>16)
        {
            return;
        }
        strcpy(yd_tk001_temp_num, data->forMMI_TPAddr.number);
        kal_prompt_trace(  MOD_SOC,"33333333");

    }

    if(KAL_FALSE == yd_tk001_Check_Condition(data))
    {
        if(g_n_sms_schye==1)
        {
            kal_prompt_trace(  MOD_SOC,"yd_tk001_all_Msg 10086 ");
            //kal_prompt_trace(  MOD_SOC,"msg %s %d",sms_post_dest, data->forMMI_UserData_length);
            g_n_sms_schye_getsms = 1;
        }

        return;
    }
    yd_tk001_wakeup_function();

    kal_prompt_trace(  MOD_SOC,"forMMI_TPAddr.number[0] %d",data->forMMI_TPAddr.number[0]);
    kal_prompt_trace(  MOD_SOC,"forMMI_TPAddr.number[1] %d",data->forMMI_TPAddr.number[1]);
    kal_prompt_trace(  MOD_SOC,"msg %s %d",data->forMMI_UserData, data->forMMI_UserData_length);
    if(data->forMMI_UserData_length>60)
    {
        return;
    }

    sms_len = data ->forMMI_UserData_length;

    kal_prompt_trace(  MOD_SOC,"UserData %s", data->forMMI_UserData);

    sscanf(data->forMMI_UserData, "%40[^,],%40[^,],%40[^,],%40[^,]",str1,str2,str3,str4);

    kal_prompt_trace(  MOD_SOC,"str1 %s", str1);
    kal_prompt_trace(  MOD_SOC,"str2 %s", str2);
    kal_prompt_trace(  MOD_SOC,"str3 %s", str3);
    kal_prompt_trace(  MOD_SOC,"str4 %s", str4);


    app_strtolower(str1);
    if(strcmp(str1, YD_TK001_SMS_IMEI) == 0)      /* imeiadmin */
    {
        if(str2[0] != 0)
        {
            return;
        }
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_IMEI");
        if(RJ_GPS_get_imei_array() == NULL)
        {
            RJ_GPS_Send_Msg_to_user("no imei", yd_tk001_temp_num); /* response  no imei*/
        }
        else
        {
            U8 msg[25] = {0};
            RJ_GPS_sprintf(msg,"IMEI:%s",(const char *)RJ_GPS_get_imei_array());
            RJ_GPS_Send_Msg_to_user(msg,yd_tk001_temp_num); /* response imei */
        }

        return;
    }
    else if(strcmp(str1, YD_TK001_SMS_SZMM) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_SZMM");
        //compare uid
        if(strcmp( (S8 *)rj_user_info.u_id,str3)!=0)
        {
            kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_SZMM 2222");
            //RJ_GPS_Send_Msg_to_user("\xE5\xAF\x86\xE7\xA0\x81\xE9\x94\x99\xE8\xAF\xAF",yd_tk001_temp_num);
            return;
        }

        if(bird_get_zctimes()>=2)
        {
            return;
        }
        memset(yd_tk001_user_number, 0x00, sizeof(yd_tk001_user_number));
        strcpy(yd_tk001_user_number, (const char *)yd_tk001_temp_num);
        RJ_GPS_send_sms_internal((S8*)str2, "BIRDYD");
        kal_prompt_trace(  MOD_SOC,"[SZMM]  user_number %s", yd_tk001_user_number);
        kal_prompt_trace(  MOD_SOC,"[SZMM]  yd_tk001_temp_num %s", yd_tk001_temp_num);
        //StartTimer(BIRD_ACTIVE_MSG_TIMER, RJ_GPS_APP_1M,     yd_tk001_active_sms_timeout);
        return;

    }
    else if(strcmp(str1, YD_TK001_SMS_YZDX) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_YZDX");
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg] owner_number %s", yd_tk001_user_number);

        if(yd_tk001_user_number[0]!=0)
        {
            bird_set_authorize_number(yd_tk001_temp_num);
            bird_set_utility_number(yd_tk001_user_number);
            bird_set_sim_iccid_to_nvram(bird_get_sim_iccid());
            yd_tk001_set_record_from_NV();
            //StopTimer(BIRD_ACTIVE_MSG_TIMER);
            g_n_ydischangesim = 3; //SIMID
            bird_soc_sendzc();
        }
        else
        {
            kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_YZDX error");
            return;
        }

    }
    else if(strcmp(str1, YD_TK001_SMS_DW) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_DW");
        if(str2[0] != 0)
        {
            return;
        }
        if(bird_get_dwtimes()>=2)
        {
            return;
        }
        if(yd_tk001_Is_authorize_number(yd_tk001_temp_num))
        {
            bird_soc_sendgetdw();
            //Rj_stop_timer(BIRD_TASK_DW_TIMER);
            //Rj_start_timer(BIRD_TASK_DW_TIMER, RJ_GPS_APP_1M, Yd_dw_delay,NULL);
        }
        return;
    }
    else if(strcmp(str1, YD_TK001_SMS_TJGL) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_TJGL");
        if(strcmp((const char *)bird_get_admin_psw(), (const char *)str2) != 0)
        {
            yd_tk001_Send_Msg("\xE5\xAF\x86\xE7\xA0\x81\xE9\x94\x99\xE8\xAF\xAF", yd_tk001_temp_num);
            return;
        }
        else
        {
            if(yd_check_str_is_number(str3)&&(strlen(str3) == 11))
            {
                if(yd_tk001_DealWith_TJGL(str3))
                {
                    yd_tk001_Send_Msg("\xE6\xB7\xBB\xE5\x8A\xA0\xE5\x8F\xB7\xE7\xA0\x81\xE6\x88\x90\xE5\x8A\x9F", yd_tk001_temp_num);
                    bird_id_data->para_setting_id = BIRD_COMMAND_ADMIN_NO;
                }
                else
                {
                    yd_tk001_Send_Msg("\xE5\x8F\xB7\xE7\xA0\x81\xE5\xB7\xB2\xE7\xBB\x8F\xE5\xAD\x98\xE5\x9C\xA8", yd_tk001_temp_num);
                    return;
                }
            }
            else
            {
                yd_tk001_Send_Msg("\xE8\xBE\x93\xE5\x85\xA5\xE5\x8F\xB7\xE7\xA0\x81\xE6\xA0\xBC\xE5\xBC\x8F\xE9\x94\x99\xE8\xAF\xAF\xEF\xBC\x81",
                                  yd_tk001_temp_num);
                return;
            }
        }

    }
    else if(strcmp(str1, YD_TK001_SMS_IPDZ) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_IPDZ");
        if(yd_tk001_is_admin_number(yd_tk001_temp_num))
        {
            bird_set_server_address(str2);
            bird_id_data->para_setting_id = BIRD_COMMAND_SET_SYS_ADDRESS;
            RJ_GPS_Send_Msg_to_user("\xE8\xAE\xBE\xE7\xBD\xAE\x49\x50\xE5\x9C\xB0\xE5\x9D\x80\xE5\x92\x8C\xE7\xAB\xAF\xE5\x8F\xA3\xE6\x88\x90\xE5\x8A\x9F",
                                    bird_get_admin_number()); /* IPdizhi */
            Lima_set_soc_init_flag(FALSE);
            Lima_Soc_Dinit();
            Bird_soc_conn();
            bird_soc_sendlogin();
        }
        else
        {
            return;
        }
        //  yd_tk001_DealWith_IPDZ(data,sms_len);

    }
    else if(strcmp(str1, YD_TK001_SMS_CQ) == 0)
    {
        if(str2[0] != 0)
        {
            return;
        }
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_CQ");
        if(yd_tk001_Is_authorize_number(yd_tk001_temp_num)||yd_tk001_is_admin_number(yd_tk001_temp_num))
        {
            //  yd_tk001_DealWith_CQ(data,sms_len);
            RJ_GPS_Send_Msg_to_user("\xE8\xAE\xBE\xE5\xA4\x87\xE6\xAD\xA3\xE5\x9C\xA8\xE9\x87\x8D\xE5\x90\xAF\xEF\xBC\x8C\xE8\xAF\xB7\xE7\xA8\x8D\xE5\x90\x8E\xE3\x80\x82\xE3\x80\x82\xE3\x80\x82",
                                    yd_tk001_temp_num);
            yd_send_reboot_control_msg();
        }
        return;
    }
    else if(strcmp(str1, YD_TK001_SMS_CQADMIN) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_CQADMIN");
        {
            RJ_SYS_ReStart();
        }
        return;
    }
    else if(strcmp(str1, YD_TK001_SMS_APN) == 0)     /* APN*/
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_APN");
        if(yd_tk001_Is_authorize_number(yd_tk001_temp_num))
        {
            p2 = data->forMMI_UserData;
            psplit = strchr(p2, ',');
            while(psplit!=NULL)
            {
                memset(p1,0,sizeof(p1));
                memcpy(p1, p2,strlen(p2) - strlen(psplit));
                p2 = psplit+1;
                psplit = strchr(p2, ',');
                if(i==0)
                {
                    memset(str1,0,sizeof(str1));
                    memcpy(str1, p1,strlen(p1));
                }
                else if(i==1)
                {
                    memset(str2,0,sizeof(str2));
                    memcpy(str2, p1,strlen(p1));
                }
                else if(i==2)
                {
                    memset(str3,0,sizeof(str3));
                    memcpy(str3, p1,strlen(p1));
                }
                else if(i==3)
                {
                    memset(str4,0,sizeof(str4));
                    memcpy(str4, p1,strlen(p1));
                }
                else if(i==4)
                {
                    memset(str5,0,sizeof(str5));
                    memcpy(str5, p1,strlen(p1));
                }
                else if(i==5)
                {
                    memset(str6,0,sizeof(str6));
                    memcpy(str6, p1,strlen(p1));
                }
                else if(i==6)
                {
                    memset(str7,0,sizeof(str7));
                    memcpy(str7, p1,strlen(p1));
                }
                else if(i==7)
                {
                    memset(str8,0,sizeof(str8));
                    memcpy(str8, p1,strlen(p1));
                }
                else if(i==8)
                {
                    memset(str9,0,sizeof(str9));
                    memcpy(str9,p1,strlen(p1));
                }
                i++;
            }
            if(p2!=NULL)
            {
                if(i==0)
                {
                    memset(str1,0,sizeof(str1));
                    memcpy(str1, p2,strlen(p2));
                }
                else if(i==1)
                {
                    memset(str2,0,sizeof(str2));
                    memcpy(str2, p2,strlen(p2));
                }
                else if(i==2)
                {
                    memset(str3,0,sizeof(str3));
                    memcpy(str3, p2,strlen(p2));
                }
                else if(i==3)
                {
                    memset(str4,0,sizeof(str4));
                    memcpy(str4, p2,strlen(p2));
                }
                else if(i==4)
                {
                    memset(str5,0,sizeof(str5));
                    memcpy(str5, p2,strlen(p2));
                }
                else if(i==5)
                {
                    memset(str6,0,sizeof(str6));
                    memcpy(str6, p2,strlen(p2));
                }
                else if(i==6)
                {
                    memset(str7,0,sizeof(str7));
                    memcpy(str7, p2,strlen(p2));
                }
                else if(i==7)
                {
                    memset(str8,0,sizeof(str8));
                    memcpy(str8, p2,strlen(p2));
                }
                else if(i==8)
                {
                    memset(str9,0,sizeof(str9));
                    memcpy(str9, p2,strlen(p2));
                }
            }
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str1 %s", str1);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str2 %s", str2);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str3 %s", str3);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str4 %s", str4);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str5 %s", str5);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str6 %s", str6);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str7 %s", str7);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str8 %s", str8);
            kal_prompt_trace(  MOD_SOC,"YD_TK001_SMS_APN str8 %s", str9);
            update_acc_prof_id(Get_aact_id(),str2,str3,atoi(str4),str5,str6,str7,str8,str9);
            RJ_GPS_Send_Msg_to_user("\xE8\xAE\xBE\xE7\xBD\xAE\x41\x50\x4E\xE5\x8F\x82\xE6\x95\xB0\xE6\x88\x90\xE5\x8A\x9F",
                                    yd_tk001_temp_num); /* APN*/
            Lima_set_soc_init_flag(FALSE);
            Lima_Soc_Dinit();
            yd_tk001_wakeup_function();
        }
        else
        {
            return;
        }
    }
    else if(strcmp(str1, YD_TK001_SMS_STATUS) == 0)
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  YD_TK001_SMS_STATUS");
        yd_tk001_DealWith_returnStatus();
        return;
    }
    else if(strcmp(str1, YD_TK001_SMS_TRACE) == 0)       /*trace*/
    {
        if(str2[0] == 0)
        {
            return;
        }
        if(yd_tk001_Is_authorize_number(yd_tk001_temp_num)||yd_tk001_is_admin_number(yd_tk001_temp_num))
        {
            if(str2[0] == '1' )
            {
                bird_set_trace_value(1);
            }
            else if(str2[0] == '0' )
            {
                bird_set_trace_value(0);
            }
            RJ_GPS_Send_Msg_to_user("\xE6\x98\xAF\xE5\x90\xA6\xE5\xBC\x80\xE5\x90\xAF\xE8\xB7\x9F\xE8\xB8\xAA\xE5\xB7\xB2\xE7\xBB\x8F\xE8\xAE\xBE\xE7\xBD\xAE",
                                    yd_tk001_temp_num);
        }

    }
    else
    {
        kal_prompt_trace(  MOD_SOC,"[yd_tk001_DealWith_Msg]  error");
        return;
    }

    //        g_yd_tk001_sms_from_user_or_admin = 1;
    memset(yd_tk001_new_psw, 0x00,sizeof(yd_tk001_new_psw));
    memset(yd_tk001_user_number, 0x00,sizeof(yd_tk001_user_number));
    yd_send_save_nv_msg();
}
