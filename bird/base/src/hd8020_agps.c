#ifdef HD8020_GPS_SUPPORT

#include "Bird_app.h"
#include "Bird_gpscell.h"
#include <stdlib.h>
#include  "Rj_DW02.h"
#include  "yd_main.h"
#include  "Bird_task.h"
#include "Bird_nbrcell.h"


kal_int8 gps_close_location_flag = 0;
extern peph_op_struct agps_peph_op;
extern nbr_Cell_Info_struct g_mmi_nbr_service_cell_info[2];
extern nbr_Cell_Info_struct g_mmi_nbr_neighbor_cell_info[2][6];
extern nbr_Cell_Info_struct rj_agps_data;
extern nbr_Cell_Info_struct rj_agps_data2;
extern nbr_Cell_Info_struct rj_agps_data3;

extern nbr_Cell_Info_struct rj_agps_data_temp;
extern nbr_Cell_Info_struct rj_agps_data1_temp;
extern nbr_Cell_Info_struct rj_agps_data2_temp;
extern nbr_Cell_Info_struct rj_agps_data3_temp;

extern nbr_Cell_Info_struct rj_agps_send_data;
extern nbr_Cell_Info_struct rj_agps_send_data2;
extern nbr_Cell_Info_struct rj_agps_send_data3;
kal_uint16 cynomsg_checksum(const unsigned char *buf, unsigned short len)
{
    kal_uint16 x;
    S8 checksum[] = { 0x00, 0x00 };
    for (x = 0; x < len; x++)
    {
        *checksum += buf[x];
        *(checksum + 1) += *checksum;
    }
    return *(kal_uint16*) checksum;
}



void checksum_test()
{
    kal_uint16 temp,temp1,temp2,temp3;
    S8 abc[4] = {0x0A,0x04,0x00,0x00};//{0x06,0x91,0x01,0x00,0x02};//{0x01,0x05,0x01,0x00,0x01};//{0xF1,0xD9,0x01,0x05,0x01,0x00,0x01};
    S8 abc1[5] = {0x06,0x70,0x01,0x00,0x00};
    S8 abc2[5] = {0x06,0x70,0x01,0x00,0x01};
    S8 abc3[4] = {0x0B,0x40,0x00,0x00};
    S8 a[26] = {/*0xF1, 0xD9, */0x0B, 0x02, 0x16, 0x00, 0xFA, 0x47, 0x2D, 0x00, 0xB3, 0x48, 0xB9, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x06, 0x14, 0x08, 0x1A, 0x06, 0x00, 0x00, 0x00, 0xE0, 0x07};
    S8 b[] = {0x06, 0x14, 0x08, 0x1A, 0x06, 0x00, 0x00, 0x00, 0xE0, 0x07};
    char abcd[3]="12";

    temp = atoi(abcd);
    temp = cynomsg_checksum(abc,4);
    temp1 = cynomsg_checksum(abc1,5);
    temp2 = cynomsg_checksum(abc2,5);
    temp3 = cynomsg_checksum(a,26);

    kal_prompt_trace(MOD_GPS, "the checksum result is temp3 = %x, ", temp3);
}


void RJ_GPS_nmea_txt_callback(mdi_gps_nmea_txt_struct *param)
{
    mdi_gps_nmea_txt_struct *txt_data = param;
    kal_int8  i = 0;
    kal_int8 hed[] = {'H', 'E', 'D', '*', '1', '9', 0x0D, 0x0A, 0x00};
    kal_int8 ant[] = {'A', 'N', 'T', '_', 'O', 'K',  '*', '5', '3', 0x0D, 0x0A, 0x00};

    if ((txt_data->hed_0 == 0x02) && (txt_data->hed_1 == 0x01) && (txt_data->hed_2 == 0x02))
    {
        for (i = 0; i<sizeof(hed); i++)
        {
            if (hed[i] != txt_data->hed_string[i])
            {
                kal_prompt_trace(MOD_GPS, "RJ_GPS_nmea_txt_callback no hed match");
                return;
            }
        }
        kal_prompt_trace(MOD_GPS, "RJ_GPS_nmea_txt_callback hed match");
        agps_peph_op.cold_start_ack_num++;
    }
    else if ((txt_data->hed_0 == 0x02) && (txt_data->hed_1 == 0x02) && (txt_data->hed_2 == 0x02))
    {
        for (i = 0; i<sizeof(ant); i++)
        {
            if (ant[i] != txt_data->hed_string[i])
            {
                kal_prompt_trace(MOD_GPS, "RJ_GPS_nmea_txt_callback no ant match");
                return;
            }
        }
        kal_prompt_trace(MOD_GPS, "RJ_GPS_nmea_txt_callback ant match");
        agps_peph_op.cold_start_ack_num++;
    }
    else
    {
        kal_prompt_trace(MOD_GPS, "RJ_GPS_nmea_txt_callback no match");
    }
}

void BIRD_GPS_Write_Pos_Data(void)
{
    S8 agps_aid_pos_buf[30] = {0};
    S8 agps_aid_pos_head_id[6] = {0xF1, 0xD9, 0x0B, 0x10, 0x11, 0x00};
    kal_uint8 agps_aid_pos_pos = 0;
    kal_uint32 i=0, k = 0;
    kal_uint16 checksum = 0;
    kal_uint32 llapos_lati = 0, llapos_logi = 0, llapos_alti = 0;

    memset(agps_aid_pos_buf, 0, sizeof(agps_aid_pos_buf));
    memcpy(agps_aid_pos_buf, agps_aid_pos_head_id, sizeof(agps_aid_pos_head_id));
    agps_aid_pos_pos = 6;

    agps_aid_pos_buf[agps_aid_pos_pos] = 1;  /*1 is lla; 0 is ecef*/
    agps_aid_pos_pos++;
    bird_get_agps_llapos(&llapos_lati, &llapos_logi, &llapos_alti);
    memcpy(agps_aid_pos_buf + agps_aid_pos_pos, (S8*)&(llapos_lati), 4);
    agps_aid_pos_pos += 4;
    memcpy(agps_aid_pos_buf + agps_aid_pos_pos, (S8*)&(llapos_logi), 4);
    agps_aid_pos_pos += 4;
    memcpy(agps_aid_pos_buf + agps_aid_pos_pos, (S8*)&(llapos_alti), 4);
    agps_aid_pos_pos += 4;
    agps_aid_pos_buf[agps_aid_pos_pos++] = 0;
    agps_aid_pos_buf[agps_aid_pos_pos++] = 0;
    agps_aid_pos_buf[agps_aid_pos_pos++] = 0;
    agps_aid_pos_buf[agps_aid_pos_pos++] = 0;
    checksum = cynomsg_checksum(agps_aid_pos_buf+2, agps_aid_pos_pos-2);
#if BIRD_AGPS_DEBUG
    kal_prompt_trace(MOD_GPS, "BIRD_GPS_Write_Pos_Data checksum = %x", checksum);
#endif
    memcpy(agps_aid_pos_buf + agps_aid_pos_pos, (S8*)&(checksum), 2);
    agps_aid_pos_pos += 2;
    BD_UART_Write(agps_aid_pos_buf, agps_aid_pos_pos);
#if BIRD_AGPS_DEBUG
    for (i = 0; i<agps_aid_pos_pos; i++)
    {
        kal_prompt_trace(MOD_GPS, "agps_aid_pos_buf[%d] = %x", i, agps_aid_pos_buf[i]);
    }
#endif
}

#if BIRD_AGPS_WRITE_POSTIME
void BIRD_GPS_Write_Postime_Data(void)
{
    S8 agps_aid_postime_buf[30] = {0};
    S8 agps_aid_postime_head_id[6] = {0xF1, 0xD9, 0x0B, 0x02, 0x16, 0x00};
    kal_uint8 agps_aid_postime_pos = 0;
    applib_time_struct dt, utc_time;
    kal_uint32 i=0, k = 0;
    kal_uint16 checksum = 0;
    kal_uint32 pos_lati = 0, pos_logi = 0, pos_alti = 0;

    memset(agps_aid_postime_buf, 0, sizeof(agps_aid_postime_buf));
    memcpy(agps_aid_postime_buf, agps_aid_postime_head_id, sizeof(agps_aid_postime_head_id));
    agps_aid_postime_pos = 6;

    bird_get_agps_pos(&pos_lati, &pos_logi, &pos_alti);
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, (S8*)&(pos_lati), 4);
    agps_aid_postime_pos += 4;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, (S8*)&(pos_logi), 4);
    agps_aid_postime_pos += 4;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, (S8*)&(pos_alti), 4);
    agps_aid_postime_pos += 4;
    applib_dt_get_rtc_time(&dt);
    applib_dt_utc_to_rtc(-8, &dt, &utc_time);
#if BIRD_AGPS_DEBUG
    kal_prompt_trace(MOD_GPS, "dt is[%02d-%02d %02d:%02d] however utc_time is [%02d-%02d %02d:%02d]", dt.nMonth,dt.nDay,dt.nHour, dt.nMin, utc_time.nMonth,utc_time.nDay,utc_time.nHour, utc_time.nMin);
#endif
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nMonth, 1);
    agps_aid_postime_pos++;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nDay, 1);
    agps_aid_postime_pos++;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nHour, 1);
    agps_aid_postime_pos++;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nMin, 1);
    agps_aid_postime_pos++;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nSec, 1);
    agps_aid_postime_pos += 4;
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, &utc_time.nYear, 2);
    agps_aid_postime_pos += 2;

    checksum = cynomsg_checksum(agps_aid_postime_buf+2, agps_aid_postime_pos-2);
#if BIRD_AGPS_DEBUG
    kal_prompt_trace(MOD_GPS, "BIRD_GPS_Write_Postime_Data checksum = %x", checksum);
#endif
    memcpy(agps_aid_postime_buf + agps_aid_postime_pos, (S8*)&(checksum), 2);
    agps_aid_postime_pos += 2;
    BD_UART_Write(agps_aid_postime_buf, agps_aid_postime_pos);
#if BIRD_AGPS_DEBUG
    for (i = 0; i<agps_aid_postime_pos; i++)
    {
        kal_prompt_trace(MOD_GPS, "agps_aid_postime_buf[%d] = %x", i, agps_aid_postime_buf[i]);
    }
#endif
}
#endif

void BIRD_GPS_Cold_Start(peph_op_struct *peph_op)
{
    S8 agps_cold_start_buf[9] = {0xF1, 0xD9, 0x06, 0x40, 0x01, 0x00, 0x01, 0x48, 0x22};

    Bird_Log_print(MOD_SOC, "BIRD_GPS_Cold_Start");
    StopTimer(BIRD_GPS_COLD_RESTART_TIMER);
#ifdef HD8020_ACK_CHECK
    peph_op->cold_start_ack_num = 0;
    peph_op->cold_start_times++;
    if (peph_op->cold_start_times >BIRD_COLD_START_COUNT)/*if cold start continuely 3 times, stop cold start*/
        return;
    BD_UART_Write(agps_cold_start_buf, sizeof(agps_cold_start_buf));
    StartTimerEx(BIRD_GPS_COLD_RESTART_CHECK_TIMER, 2000, bird_cold_start_check, peph_op);
#else
    BD_UART_Write(agps_cold_start_buf, sizeof(agps_cold_start_buf));
    kal_sleep_task(BIRD_WRITE_AGPS_DATA_DELAY*12);
    BIRD_Write_Agps_Data(peph_op);
#endif
}

void BIRD_GPS_Cold_Start_First(void)
{
    S8 agps_cold_start_buf[9] = {0xF1, 0xD9, 0x06, 0x40, 0x01, 0x00, 0x01, 0x48, 0x22};
    kal_prompt_trace(MOD_GPS, "BIRD_GPS_Cold_Start_First");

    StopTimer(BIRD_GPS_COLD_RESTART_TIMER);
    BD_UART_Write(agps_cold_start_buf, 9);
    kal_sleep_task(BIRD_WRITE_AGPS_DATA_DELAY);
}

void BIRD_GPS_Write_Peph_Data(peph_op_struct *peph_op)
{
    Bird_Log_print(MOD_SOC, "BIRD_GPS_Write_Peph_Data");
    peph_op->index = 0;
    peph_op->peph_ack_ok_num = 0;
    peph_op->peph_ack_err_num = 0;
    SetProtocolEventHandler(BIRD_GPS_Write_Peph_Data_Proc, MSG_ID_YD_TK001_MSG_WRITE_PEPH);
    kal_sleep_task(20);
    bird_deal_send_msg_handler	(MSG_ID_YD_TK001_MSG_WRITE_PEPH, 0, 0);
}

void BIRD_Write_Agps_Data_Check(peph_op_struct *peph_op)
{
#if BIRD_GPS_DEBUG
    Bird_Log_print(MOD_SOC, "BIRD_Write_Agps_Data_Check ok_count = %d, item = %d", peph_op->peph_ack_ok_num, peph_op->total_item);
#endif
    StopTimer(BIRD_AGPS_WRITE_CHECK_TIMER);
    peph_op->write_peph_times++;
    if (peph_op->write_peph_times > BIRD_AGPS_WRITE_COUNT)
    {
        Bird_Log_print(MOD_SOC, "Error: write_peph_times > 2");
        return;
    }
    if ((peph_op->peph_ack_ok_num < peph_op->total_item) ||(peph_op->peph_ack_err_num > 1))
    {
        Bird_Log_print(MOD_SOC, "Error: agps ack count is %d, ack err count is %d", peph_op->peph_ack_ok_num, peph_op->peph_ack_err_num);
        StartTimerEx(BIRD_AGPS_WRITE_TIMER, RJ_GPS_APP_5S, BIRD_Write_Agps_Data, peph_op);
    }
}

void bird_gps_write_peph_data_timeout_ex(peph_op_struct *peph_op)
{
    kal_prompt_trace(MOD_GPS, "bird_gps_write_peph_data_timeout_ex");
    BD_UART_Write(&peph_op->agps_buf[peph_op->peph_pos ], peph_op->peph_len);
}

void bird_gps_write_peph_data_timeout(void)
{
    bird_gps_write_peph_data_timeout_ex(&agps_peph_op);
    StopTimer(BIRD_AGPS_WRITE_TIMEOUT_TIMER);
}

void bird_write_peph_data_ex(peph_op_struct *peph_op)
{
    kal_uint32  k = 0, pos1 = 0, pos2 = 0, count = 0;
    kal_uint16  write_len = 0;

    for (k = peph_op->index; k < peph_op->agps_len; k++)
    {
        if ((peph_op->agps_buf[k] == 0xF1) && (peph_op->agps_buf[k+1] == 0xD9))
        {
            count++;
            if (count == 1)
            {
                pos1 = k;
                kal_prompt_trace(MOD_GPS, "bird_write_peph_data_ex pos1 = %d", k);
            }
            else
            {
                pos2 = k;
                kal_prompt_trace(MOD_GPS, "bird_write_peph_data_ex pos2 = %d", k);
                break;
            }
        }
    }
    peph_op->index = k;
    if ((k == peph_op->agps_len) && (pos1 != 0))
    {
        pos2 = peph_op->agps_len;
#if BIRD_AGPS_DEBUG
        applib_dt_get_rtc_time(&peph_op->peph_write_over_time);
        Bird_Log_print(MOD_SOC, "agps_over_time is [%04d-%02d-%02d %02d:%02d:%02d]",  peph_op->peph_write_over_time.nYear,
                       peph_op->peph_write_over_time.nMonth,peph_op->peph_write_over_time.nDay,peph_op->peph_write_over_time.nHour,
                       peph_op->peph_write_over_time.nMin, peph_op->peph_write_over_time.nSec);
#endif
    }

    if (pos2 != 0)
    {
        agps_peph_op.peph_len = pos2 - pos1;
        agps_peph_op.peph_pos = pos1;
        StartTimer(BIRD_AGPS_WRITE_TIMEOUT_TIMER, 2000, bird_gps_write_peph_data_timeout);
        Bird_Log_print(MOD_SOC, "bird_write_peph_data_ex, pos1 = %d, length = %d", pos1, pos2 - pos1);
        peph_op->write_peph_start = 1;
        write_len = BD_UART_Write((kal_uint8 *)peph_op->agps_buf+pos1, pos2 - pos1);
    }
}

void BIRD_GPS_Write_Peph_Data_Proc(void)
{
    bird_write_peph_data_ex(&agps_peph_op);
}

void BIRD_Write_Agps_Data(peph_op_struct *peph_op)
{
    Bird_Log_print(MOD_SOC, "BIRD_Write_Agps_Data");
    StopTimer(BIRD_AGPS_WRITE_TIMER);
#if BIRD_AGPS_WRITE_POSTIME
    BIRD_GPS_Write_Postime_Data();
    kal_sleep_task(BIRD_WRITE_AGPS_DATA_DELAY);
#endif
    BIRD_GPS_Write_Peph_Data(peph_op);
    StartTimerEx(BIRD_AGPS_WRITE_CHECK_TIMER, 5000, BIRD_Write_Agps_Data_Check, peph_op);
}

void bird_cold_start_check(peph_op_struct *peph_op)
{
    StopTimer(BIRD_GPS_COLD_RESTART_CHECK_TIMER);
    Bird_Log_print(MOD_SOC, "cold_start_ack_num = %d", peph_op->cold_start_ack_num);

    if (peph_op->cold_start_ack_num <BIRD_COLD_START_ACK_COUNT)    /*if cold start ack is fail, cold start again*/
    {
        BIRD_GPS_Cold_Start(peph_op);
        Bird_Log_print(MOD_SOC, "ERROR:cold_start_ack_num < 2");
    }
    else
    {
        BIRD_Write_Agps_Data(peph_op);
    }
}

void bird_socket_write_agps_data(void)
{
    kal_int16 i = 0, k = 0;

    BIRD_Pre_Write_Agps_Data(&agps_peph_op);
    agps_peph_op.total_item = 0;
    for (i = 0; i < agps_peph_op.agps_len; i++)
    {
        if ((agps_peph_op.agps_buf[i] == 0xF1) && (agps_peph_op.agps_buf[i+1] == 0xD9))
            agps_peph_op.total_item++;
    }
    kal_prompt_trace(MOD_GPS, "bird_socket_write_agps_data total_item = %d", agps_peph_op.total_item);
}

void yd_set_write_agps_msg()
{
    SetProtocolEventHandler(bird_socket_write_agps_data,MSG_ID_YD_TK001_MSG_WRITE_AGPS);
}

void BIRD_Pre_Write_Agps_Data(peph_op_struct *peph_op)
{
    Bird_Log_print(MOD_SOC, "BIRD_Pre_Write_Agps_Data");
    peph_op->cold_start_times = 0;
    peph_op->cold_start_ack_num = 0;
    peph_op->write_peph_times = 0;
    peph_op->write_peph_start = 0;
    BIRD_GPS_Cold_Start(peph_op);
}

void BIRD_GPS_Hot_Start(void)
{
}

void RJ_GET_AGPS(peph_op_struct *peph_op)
{
    applib_time_struct currtent_time= {0};
    peph_op_struct *local_peph_op = peph_op;

    Bird_Log_print(MOD_SOC, "RJ_GET_AGPS n");
    applib_dt_get_rtc_time(&currtent_time);
    //超过1小时同步AGPS数据
    if (gps_close_location_flag == KAL_FALSE)
    {
        HD8020_AGPS(&agps_peph_op);
        kal_prompt_trace(MOD_GPS, "gps_close_location_flag false");
    }
    else
    {
        kal_prompt_trace(MOD_GPS, "gps_close_location_flag true");
        if ((currtent_time.nYear>peph_op->gps_close_time.nYear)||
                (currtent_time.nMonth>peph_op->gps_close_time.nMonth)||
                (currtent_time.nDay>peph_op->gps_close_time.nDay)||
                ((currtent_time.nHour*60+currtent_time.nMin)-(peph_op->gps_close_time.nHour*60+peph_op->gps_close_time.nMin)>60)||
                (peph_op->gps_close_time.nYear==0))
        {
            kal_prompt_trace(MOD_GPS, "gps_close_time > 60m");
            HD8020_AGPS(&agps_peph_op);
        }
    }
}
void HD8020_AGPS(peph_op_struct *peph_op)
{
    applib_time_struct currtent_time= {0};
    peph_op_struct *local_peph_op = peph_op;

    Bird_Log_print(MOD_SOC, "HD8020_AGPS");
    applib_dt_get_rtc_time(&currtent_time);
    //超过1小时同步AGPS数据
    if((currtent_time.nYear>peph_op->get_peph_last_time.nYear)||
            (currtent_time.nMonth>peph_op->get_peph_last_time.nMonth)||
            (currtent_time.nDay>peph_op->get_peph_last_time.nDay)||
            ((currtent_time.nHour*60+currtent_time.nMin)-(peph_op->get_peph_last_time.nHour*60+peph_op->get_peph_last_time.nMin)>60)||
            (peph_op->get_peph_last_time.nYear==0))
    {
        if(!Lima_get_soc_conn_flag())
        {
            Rj_stop_timer(BIRD_TASK_GPRS_TIMER_START);
            Rj_start_timer(BIRD_TASK_GPRS_TIMER_START, 5*1000, RJ_GET_AGPS,local_peph_op);
            return;
        }
        memset(local_peph_op->agps_buf, 0x00, sizeof(local_peph_op->agps_buf));
        local_peph_op->agps_len = 0;

        if(bird_get_agps_address() == NULL)/**********!!!!!!!!!!!!!!!!!!!!!!!*********/
        {
            BD_socket_create(BD_AGNSS_AGPS_1);  // agps
#if BIRD_AGPS_DEBUG
            kal_prompt_trace(MOD_GPS, "agps addr=%s", BD_AGNSS_AGPS_1);
#endif
        }
        else
        {
            BD_socket_create(bird_get_agps_address());
#if BIRD_AGPS_DEBUG
            kal_prompt_trace(MOD_GPS, "agps addr=%s", bird_get_agps_address());
#endif
        }
    }
    else
    {
        StartTimerEx(BIRD_GPS_COLD_RESTART_TIMER,2000, BIRD_Pre_Write_Agps_Data, local_peph_op);
    }
}
kal_uint8 arrayfind(kal_uint8 *str, kal_uint8 *sub_str, kal_int8 length, kal_int8 length_sub)
{
    kal_int8 i;
    for(i = 0; i < length; i++)
    {
        kal_int8 tem = i;
        kal_int8 j = 0;
        while(str[i++] == sub_str[j++])
        {
            if( j>= length_sub)
            {
                return 1;
            }
        }
        i = tem;
    }
    return 0;
}

void bird_agps_ack_handle(kal_uint8 *Buffaddr, kal_uint16 Length)
{
    kal_uint8 ack_ok_buf[] = {0xF1, 0xD9, 0x05, 0x01, 0x02};/*F1 D9 05 01 02 00 0B 32 xx xx*/
    kal_uint8 ack_err_buf[] = {0xF1, 0xD9, 0x05, 0x00, 0x02, 0x00, 0x0B, 0x32};/*F1 D9 05 00 02 00 0B 32 xx xx*/
    kal_uint8 nmea_find = 0;

    if (agps_peph_op.write_peph_start <1)
    {
        return;
    }
    nmea_find = arrayfind(Buffaddr, ack_ok_buf, Length, sizeof(ack_ok_buf));
    if (nmea_find != 0)
    {
        agps_peph_op.peph_ack_ok_num++;
        kal_prompt_trace(MOD_GPS, "agps_peph_op.peph_ack_ok_num = %d", agps_peph_op.peph_ack_ok_num);
        bird_deal_send_msg_handler	(MSG_ID_YD_TK001_MSG_WRITE_PEPH, 0, 0);
        StopTimer(BIRD_AGPS_WRITE_TIMEOUT_TIMER);
        agps_peph_op.write_peph_start = 0;
    }
}

void bird_set_agps_pos(double lati, double logi, double alti)
{
    S8  position_string[10] = 0;
    float pos_f = 0;
    kal_uint32 pos_u32 = 0;

    sprintf(position_string, "%lf", lati);
    pos_f = atof(position_string);
    pos_u32 = (kal_uint32)(pos_f * 100000);
    yd_tk001_info.agps_pos_lati = pos_u32;
    yd_tk001_info.agps_llapos_lati = yd_tk001_info.agps_pos_lati * 100;
    sprintf(position_string, "%lf", logi);
    pos_f = atof(position_string);
    pos_u32 = (kal_uint32)(pos_f * 100000);
    yd_tk001_info.agps_pos_long= pos_u32;
    yd_tk001_info.agps_llapos_long = yd_tk001_info.agps_pos_long * 100;
    sprintf(position_string, "%lf", alti);
    pos_f = atof(position_string);
    pos_u32 = (kal_uint32)pos_f;
    yd_tk001_info.agps_pos_alti = pos_u32;
    yd_tk001_info.agps_llapos_alti = yd_tk001_info.agps_pos_alti * 100;
}
void bird_get_agps_pos(kal_uint32 *lati, kal_uint32 *logi, kal_uint32 *alti)
{
    S8 pos_value[5] = {0}, i = 0;

    *lati = yd_tk001_info.agps_pos_lati;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, lati, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_pos pos_value[%d] = %x", i, pos_value[i]);
#endif
    *logi = yd_tk001_info.agps_pos_long;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, logi, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_pos pos_value[%d] = %x", i, pos_value[i]);
#endif
    *alti = yd_tk001_info.agps_pos_alti;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, alti, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_pos pos_value[%d] = %x", i, pos_value[i]);
#endif
}

void bird_get_agps_llapos(kal_uint32 *lati, kal_uint32 *logi, kal_uint32 *alti)
{
    S8 pos_value[5] = {0}, i = 0;

    *lati = yd_tk001_info.agps_llapos_lati;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, lati, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_llapos pos_value[%d] = %x", i, pos_value[i]);
#endif
    *logi = yd_tk001_info.agps_llapos_long;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, logi, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_llapos pos_value[%d] = %x", i, pos_value[i]);
#endif
    *alti = yd_tk001_info.agps_llapos_alti;
#if BIRD_AGPS_DEBUG
    memcpy(pos_value, alti, 4);
    for(i = 0; i<4; i++)
        kal_prompt_trace(MOD_GPS, "bird_get_agps_llapos pos_value[%d] = %x", i, pos_value[i]);
#endif
}

#endif
