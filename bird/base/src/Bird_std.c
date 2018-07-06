/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      Bird_std.c
  Author:       Version:        Date: 2013-05-29
  Description: The bird standard library function, bird module reference
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifdef RJ_GPS_APP


#include    "eint.h"
#include    "Rj_DW02.h"
#include "Bird_app.h"

kal_int8 rj_gps_error_code = 0;
U8 yd_sms_code_type = 0;

extern void TRACE_UART(kal_uint8 * fmt,...);

/*-------------------------------------------------------
                      log 操作
-------------------------------------------------------*/
void RJ_GPS_log(kal_char *fmt,...)
{
#ifdef RJ_GPS_LOG_ON
    kal_prompt_trace(MOD_SOC,  fmt);
#endif
}


/*-------------------------------------------------------
                      timer 操作
-------------------------------------------------------*/
void RJ_GPS_StartTimer(U16 timerid, U32 delay, FuncPtr funcPtr)
{
    StartTimer(timerid, delay, funcPtr);
}

void RJ_GPS_StopTimer(U16 timerid)
{
    StopTimer(timerid);
}
/*-------------------------------------------------------*/


/*-------------------------------------------------------
                      字串处理
-------------------------------------------------------*/
void RJ_GPS_sprintf(S8* str, const S8* fmt_str,...)
{
    va_list arg_list;
    va_start(arg_list,fmt_str);
    vsprintf(str,fmt_str,arg_list);
    va_end(arg_list);
}

/*-------------------------------------------------------
                      文件操作
-------------------------------------------------------*/
int RJ_GPS_FS_Read(FS_HANDLE FileHandle, void * DataPtr, kal_uint32 Length, kal_uint32 * Read)
{
    FS_Read(FileHandle, DataPtr, Length, Read);
}

int RJ_GPS_FS_Close(FS_HANDLE FileHandle)
{
    FS_Close(FileHandle);
}


void yd_tk001_set_utility_time(U8 * data)
{
    MYTIME currTime;
    U8 time[6];
    TRACE_UART("%s %d %s",__FILE__,__LINE__, data);
    memcpy(time,( const void *)data, 6);
    currTime.nYear = 10*(time[0]>>4)+(time[0]&0X0F) + 2000;
    currTime.nMonth= 10*(time[1]>>4)+(time[1]&0X0F) ;
    currTime.nDay = 10*(time[2]>>4)+(time[2]&0X0F) ;
    currTime.nHour = 10*(time[3]>>4)+(time[3]&0X0F) ;
    currTime.nMin = 10*(time[4]>>4)+(time[4]&0X0F) ;
    currTime.nSec = 10*(time[5]>>4)+(time[5]&0X0F) ;
    currTime.DayIndex= applib_dt_dow(currTime.nYear, currTime.nMonth, currTime.nDay);
    mmi_dt_set_rtc_dt((MYTIME * )&currTime);
}

void yd_set_utility_time(U8 * data)
{
    MYTIME currTime;
    U8 time[24];
    S8 yearchar[8];
    S8 monthchar[8];
    S8 daychar[8];
    S8 hourchar[8];
    S8 minchar[8];
    S8 secchar[8];
    kal_int16 nyear;
    kal_int8 nmonth;
    kal_int8 nday;
    kal_int8 nhour;
    kal_int8 nmin;
    kal_int8 nsec;
    S8* hpos1;
    S8* hpos2;
    S8* kpos;
    S8* mpos1;
    S8* mpos2;
    S8 yearlen,monthlen,daylen,hourlen,mulen,seclen;
    S8 npos;

    memset(time,0,sizeof(time));
    memcpy(time,(S8 *)data, strlen((S8 *)data));
    //year
    yearlen = 4;
    memset(yearchar,0,sizeof(yearchar));
    memcpy(yearchar,(S8 *)data,yearlen);
    nyear = strtol(yearchar,NULL,10);
    //month
    hpos1 = strchr(data, '-');
    hpos2 = strchr(hpos1+1, '-');
    monthlen = hpos2-hpos1-1;
    npos = hpos1 - data + 1;
    memset(monthchar,0,sizeof(monthchar));
    memcpy(monthchar,(S8 *)(data+npos),monthlen);

    nmonth = atoi(monthchar);

    //day
    kpos = strchr(data, ' ');
    daylen = kpos-hpos2-1;
    npos = hpos2 - data + 1;
    memset(daychar,0,sizeof(daychar));
    memcpy(daychar,(S8 *)(data+npos),daylen);
    nday = atoi(daychar);

    //hour
    mpos1 = strchr(data, ':');
    hourlen = mpos1 - kpos - 1;
    npos = kpos - data + 1;
    memset(hourchar,0,sizeof(hourchar));
    memcpy(hourchar,(S8 *)(data+npos),hourlen);
    nhour = atoi(hourchar);

    //nmin
    mpos2 = strchr(mpos1+1, ':');
    mulen = mpos2 - mpos1 - 1;
    npos = mpos1 - data + 1;
    memset(minchar,0,sizeof(minchar));
    memcpy(minchar,(S8 *)(data+npos),mulen);
    nmin = atoi(minchar);

    //sec
    seclen = strlen(data)-(mpos2-data) - 1;
    npos = mpos2 - data + 1;
    memset(secchar,0,sizeof(secchar));
    memcpy(secchar,(S8 *)(data+npos),seclen);
    nsec = atoi(secchar);

    kal_prompt_trace(MOD_SOC,"yd_set_utility_time year = %d,month=%d,day=%d,hour=%d,min=%d,sec=%d",
                     nyear,nmonth,nday,nhour,nmin,nsec);

    currTime.nYear = nyear;
    currTime.nMonth= nmonth ;
    currTime.nDay = nday;
    currTime.nHour = nhour ;
    currTime.nMin = nmin;
    currTime.nSec = nsec ;
    currTime.DayIndex= applib_dt_dow(currTime.nYear, currTime.nMonth, currTime.nDay);
    mmi_dt_set_rtc_dt((MYTIME * )&currTime);

}

void bird_set_test_time(U8 * address )
{
    applib_time_struct current_time = {0} ;
    S8 rj_time[6];
    S8 yearchar[4];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];
    S8 jgchar[1];
    S8 jgchar2[1];
    S8 blank[1];
    applib_dt_get_rtc_time(&current_time);
    //kal_prompt_trace(MOD_SOC,"bird_set_test_time year= %d , month= %d,day=%d,hour=%d,min=%d,sec=%d",
    //	current_time.nYear,current_time.nMonth,current_time.nDay,current_time.nHour,current_time.nMin,current_time.nSec);
    //memcpy(yearchar,&current_time.nYear,2);
    sprintf(yearchar,"%d",current_time.nYear);
//lrf test
    if(current_time.nMonth<10)
    {
        monthchar[0]='0';
        sprintf(monthchar+1,"%d",current_time.nMonth);
    }
    else
    {

        sprintf(monthchar,"%d",current_time.nMonth);
    }

    if(current_time.nDay<10)
    {
        daychar[0]='0';
        sprintf(daychar+1,"%d",current_time.nDay);
    }
    else
    {
        sprintf(daychar,"%d",current_time.nDay);
    }

    if(current_time.nHour<10)
    {
        hourchar[0]='0';
        sprintf(hourchar+1,"%d",current_time.nHour);
    }
    else
    {
        sprintf(hourchar,"%d",current_time.nHour);
    }

    if(current_time.nMin<10)
    {
        minchar[0]='0';
        sprintf(minchar+1,"%d",current_time.nMin);
    }
    else
    {
        sprintf(minchar,"%d",current_time.nMin);
    }

    if(current_time.nSec<10)
    {
        secchar[0]='0';
        sprintf(secchar+1,"%d",current_time.nSec);
    }
    else
    {
        sprintf(secchar,"%d",current_time.nSec);
    }

    jgchar[0] = '-';
    jgchar2[0]=':';
    blank[0]=' ';
    memcpy(address, (S8 *)&yearchar, 4);
    memcpy(address+4,(S8 *)jgchar, 1);
    memcpy(address+5, (S8 *)&monthchar, 2);
    memcpy(address+7,(S8 *)jgchar, 1);
    memcpy(address+8, (S8 *)&daychar, 2);
    memcpy(address+10,(S8 *)blank, 1);
    memcpy(address+11, (S8 *)&hourchar, 2);
    memcpy(address+13,(S8 *)jgchar2, 1);
    memcpy(address+14, (S8 *)&minchar, 2);
    memcpy(address+16,(S8 *)jgchar2, 1);
    memcpy(address+17, (S8 *)&secchar, 2);

}

void bird_set_real_time(U8 * address,applib_time_struct *current_time)
{
    S8 rj_time[6];
    S8 yearchar[4];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];
    S8 jgchar[1];
    S8 jgchar2[1];
    S8 blank[1];
    applib_dt_get_rtc_time(current_time);
    kal_prompt_trace(MOD_SOC,"bird_set_real_time %d %d %d %d %d %d",current_time->nYear,current_time->nMonth,current_time->nDay,
                     current_time->nHour,current_time->nMin,current_time->nSec);
    sprintf(yearchar,"%d",current_time->nYear);
//lrf test
    if(current_time->nMonth<10)
    {
        monthchar[0]='0';
        sprintf(monthchar+1,"%d",current_time->nMonth);
    }
    else
    {

        sprintf(monthchar,"%d",current_time->nMonth);
    }

    if(current_time->nDay<10)
    {
        daychar[0]='0';
        sprintf(daychar+1,"%d",current_time->nDay);
    }
    else
    {
        sprintf(daychar,"%d",current_time->nDay);
    }

    if(current_time->nHour<10)
    {
        hourchar[0]='0';
        sprintf(hourchar+1,"%d",current_time->nHour);
    }
    else
    {
        sprintf(hourchar,"%d",current_time->nHour);
    }

    if(current_time->nMin<10)
    {
        minchar[0]='0';
        sprintf(minchar+1,"%d",current_time->nMin);
    }
    else
    {
        sprintf(minchar,"%d",current_time->nMin);
    }

    if(current_time->nSec<10)
    {
        secchar[0]='0';
        sprintf(secchar+1,"%d",current_time->nSec);
    }
    else
    {
        sprintf(secchar,"%d",current_time->nSec);
    }

    jgchar[0] = '-';
    jgchar2[0]=':';
    blank[0]=' ';
    memcpy(address, (S8 *)&yearchar, 4);
    memcpy(address+4,(S8 *)jgchar, 1);
    memcpy(address+5, (S8 *)&monthchar, 2);
    memcpy(address+7,(S8 *)jgchar, 1);
    memcpy(address+8, (S8 *)&daychar, 2);
    memcpy(address+10,(S8 *)blank, 1);
    memcpy(address+11, (S8 *)&hourchar, 2);
    memcpy(address+13,(S8 *)jgchar2, 1);
    memcpy(address+14, (S8 *)&minchar, 2);
    memcpy(address+16,(S8 *)jgchar2, 1);
    memcpy(address+17, (S8 *)&secchar, 2);

}
void bird_set_test_time_afert(U8 * address )
{
    applib_time_struct current_time = {0} ;
    applib_time_struct addsec_time = {0} ;
    applib_time_struct tmp_time = {0};
    S8 rj_time[6];
    S8 yearchar[4];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];
    S8 jgchar[1];
    S8 jgchar2[1];
    S8 blank[1];
    tmp_time.nSec=25;
    applib_dt_get_rtc_time(&current_time);
    applib_dt_increase_time(&current_time,&tmp_time,&addsec_time);

    sprintf(yearchar,"%d",addsec_time.nYear);
//lrf test
    if(addsec_time.nMonth<10)
    {
        monthchar[0]='0';
        sprintf(monthchar+1,"%d",addsec_time.nMonth);
    }
    else
    {

        sprintf(monthchar,"%d",addsec_time.nMonth);
    }

    if(addsec_time.nDay<10)
    {
        daychar[0]='0';
        sprintf(daychar+1,"%d",addsec_time.nDay);
    }
    else
    {
        sprintf(daychar,"%d",addsec_time.nDay);
    }

    if(addsec_time.nHour<10)
    {
        hourchar[0]='0';
        sprintf(hourchar+1,"%d",addsec_time.nHour);
    }
    else
    {
        sprintf(hourchar,"%d",addsec_time.nHour);
    }

    if(addsec_time.nMin<10)
    {
        minchar[0]='0';
        sprintf(minchar+1,"%d",addsec_time.nMin);
    }
    else
    {
        sprintf(minchar,"%d",addsec_time.nMin);
    }

    if(addsec_time.nSec<10)
    {
        secchar[0]='0';
        sprintf(secchar+1,"%d",addsec_time.nSec);
    }
    else
    {
        sprintf(secchar,"%d",addsec_time.nSec);
    }

    jgchar[0] = '-';
    jgchar2[0]=':';
    blank[0]=' ';
    memcpy(address, (S8 *)&yearchar, 4);
    memcpy(address+4,(S8 *)jgchar, 1);
    memcpy(address+5, (S8 *)&monthchar, 2);
    memcpy(address+7,(S8 *)jgchar, 1);
    memcpy(address+8, (S8 *)&daychar, 2);
    memcpy(address+10,(S8 *)blank, 1);
    memcpy(address+11, (S8 *)&hourchar, 2);
    memcpy(address+13,(S8 *)jgchar2, 1);
    memcpy(address+14, (S8 *)&minchar, 2);
    memcpy(address+16,(S8 *)jgchar2, 1);
    memcpy(address+17, (S8 *)&secchar, 2);

}

void txboxdecimal(U8*dec,U8* hex)
{
    dec[0]=hex[0]/16*10+hex[0]%16;
    dec[1]=hex[1]/16*10+hex[1]%16;
    dec[2]=hex[2]/16*10+hex[2]%16;
    dec[3]=hex[3]/16*10+hex[3]%16;
    dec[4]=hex[4]/16*10+hex[4]%16;
    dec[5]=hex[5]/16*10+hex[5]%16;

}

U8* txboxhex(U8* hex)
{
    U8 time[6]=0;

    memcpy(time, hex,6);

    time[0]=hex[0]/10*16+hex[0]%10;
    time[1]=hex[1]/10*16+hex[1]%10;
    time[2]=hex[2]/10*16+hex[2]%10;
    time[3]=hex[3]/10*16+hex[3]%10;
    time[4]=hex[4]/10*16+hex[4]%10;
    time[5]=hex[5]/10*16+hex[5]%10;

    return time;
}

/*---------------------------------------------
                         错误码写入文件
                         重开机后上传
---------------------------------------------*/
void RJ_GPS_WriteErrorCode()
{
    FS_HANDLE data_file = -1;
    U8 reset_error_log[200] = {0};
    U32 written,bytes_read;
    U32 file_size = 0;
    kal_uint8 *data_buff = NULL;
    applib_time_struct current_time;

    kal_prompt_trace(MOD_SOC,"RJ_GPS_WriteErrorCode");
    applib_dt_get_rtc_time(&current_time);

    kal_prompt_trace( MOD_SOC, "applib_dt_get_date_time %d:%d:%d:%d:%d:%d",
                      current_time.nYear,  current_time.nMonth, current_time.nDay, current_time.nHour, current_time.nMin, current_time.nSec);

    sprintf(reset_error_log, "reset occur %d:%d:%d:%d:%d:%d . the error code:%d\n",
            current_time.nYear,  current_time.nMonth, current_time.nDay, current_time.nHour, current_time.nMin, current_time.nSec,rj_gps_error_code);

    data_file = FS_Open(L"C:\\errorcode.dat",FS_CREATE|FS_READ_WRITE);
    //FS_GetFileSize(data_file, &file_size);
    if (data_file < FS_NO_ERROR)
    {
        FS_Delete(L"C:\\errorcode.dat");
        data_file = FS_Open(L"C:\\openerrorcode.dat",FS_CREATE|FS_READ_WRITE);
        FS_Seek(data_file,0,FS_FILE_END);
        FS_Write(data_file, &reset_error_log, strlen(reset_error_log), (U32*) & written);
    }
    else
    {
        FS_Seek(data_file,0,FS_FILE_END);
        FS_Write(data_file, &reset_error_log, strlen(reset_error_log), (U32*) & written);
    }
    FS_Close(data_file);   //gouyafeng

    /*
            data_file = FS_Open(L"C:\\errorcode.dat", FS_READ_ONLY);
            FS_Read(data_file, data_buff, file_size, &bytes_read);
            kal_prompt_trace(MOD_SOC,"resoult %s count %d",data_buff,bytes_read);
            FS_Close(data_file);
        */
}

/*---------------------------------------------
                   读取文件存储的错误码
                         重开机后上传
---------------------------------------------*/
void RJ_GPS_ReadErrorCode()
{
    FS_HANDLE data_file;

    kal_prompt_trace( MOD_SOC,"RJ_GPS_ReadErrorCode");

    data_file = FS_Open(L"C:\\errorcode.dat", FS_READ_ONLY);

    if(data_file >0)
    {
        FS_Read(data_file, &rj_gps_error_code, sizeof(rj_gps_error_code), NULL);

    }
    else
    {

    }

    FS_Close(data_file);

    kal_prompt_trace( MOD_SOC,"RJ_GPS_ReadErrorCode = %d", rj_gps_error_code);
}

/*---------------------------------------------
                          获取错误码
                          0，无错
---------------------------------------------*/
int RJ_GPS_GetErrorCode()
{
    return rj_gps_error_code;
}

/*---------------------------------------------
                          获取错误码
                          0，无错
---------------------------------------------*/
int RJ_GPS_SetErrorCode(int in)
{
    rj_gps_error_code = in;
}

U8 bird_set_U32_to_data(U8 * addr,U32 para)
{
    *(addr+3) = para%256;
    *(addr+2) = (para >>8)%256;
    *(addr+1) = (para >>16)%256;
    *(addr) =  para >>24;
}

void Lima_Soc_Itoa(S8* out, S32 num)
{
    S32 rem     = 0;//余数
    S32 integer = 0;//取整
    U8  i       = 0;
    U8  len     = 0;
    S8  ch;


    if (0 == num)
    {
        out[0] = '0';
    }
    else
    {
        integer = num;
        while (integer != 0)
        {
            rem     = integer%10;
            integer = integer/10;
            out[i]  = rem + '0';
            i++;
        }
        //字符串长度
        len = i;
        //逆置
        for(i = 0; i < len/2; i++)
        {
            ch              = out[i];
            out[i]          = out[len - i -1];
            out[len - i -1] = ch;
        }

    }

}

/***************************低电报警begin************************************************/


void bird_set_SMS_codetype(U8 type)
{
    yd_sms_code_type = type;
}

U8 bird_get_SMS_codetype()
{
    return yd_sms_code_type;
}

void RJ_GPS_sms_send_cbk_internal(srv_sms_callback_struct *pData)
{
    //暂时不需要处理，如果有资源要释放的话走这边
}

S8 * RJ_GPS_chset_text_to_ucs2_str(U8*dest, U32  dest_size, U8 *src)
{
    mmi_chset_convert(MMI_CHSET_UTF8, MMI_CHSET_UCS2, (char *)src, (char *)dest, (strlen((char *)src) + 1)*ENCODING_LENGTH);
}


U16 RJ_GPS_send_sms_internal(S8* pNumber, S8* pContent)
{
    U8 len, i;
    SMS_HANDLE send_handle;
    S8 ucs2_addr[42];
    kal_prompt_trace(MOD_SOC, "RJ_GPS_send_sms_internal");
    send_handle = srv_sms_get_send_handle();

    if (send_handle == NULL)
    {
        /* can not get handle */
        return 0;
    }

    if ((U16)srv_sms_is_bg_send_action_busy() == MMI_TRUE)
    {
        /* background sending is busy */
        return 0;
    }
    TRACE_UART("%s %d",__FILE__,__LINE__);
    srv_sms_set_no_sending_icon(send_handle);

    /* set address number */
    memset(ucs2_addr, 0, sizeof(ucs2_addr));
    mmi_asc_to_ucs2(ucs2_addr, pNumber);
    srv_sms_set_address(send_handle, ucs2_addr);

    /* set content of DCS encoding */
    srv_sms_set_content_dcs(send_handle, 0x08);

    /* set content */
    srv_sms_set_content(send_handle, pContent, (U16)((mmi_ucs2strlen(pContent) + 1) * 2));
    srv_sms_set_sim_id(send_handle, SRV_SMS_SIM_1);

    /* send request */
    srv_sms_send_msg(send_handle, RJ_GPS_sms_send_cbk_internal, NULL);
    kal_prompt_trace(MOD_SOC, "RJ_GPS_send_sms_internal");
    return 1;

}


void RJ_GPS_Send_Msg_to_user(S8* msg_data,S8* user_number )
{
    S8 temp[RJ_GPS_MSG_MAX_LENGTH+1];
    S8 number[RJ_GPS_PNONE_NUM] = {0};

    memset(temp, 0, sizeof(temp));
    strcpy(number, (const char *)user_number);
    TRACE_UART( "RJ_GPS_Send_Msg data = %s",msg_data);

    RJ_GPS_chset_text_to_ucs2_str(temp,RJ_GPS_MSG_MAX_LENGTH,(U8 *)msg_data);
    //把服务器返回的信息发送给用户

    RJ_GPS_send_sms_internal((S8*)number, temp);
}

FS_HANDLE file_handle = -1;
unsigned short                 file[BIRD_FILE_PATH_LEN];
char buf[BIRD_SAVE_LOG_LENGTH];
char buf_log[BIRD_LOG_LENGTH];
char buf_dt[BIRD_DT_LENGTH];
kal_uint32 buf_len = 0;
kal_uint8  write_flag = 0;
kal_uint8  add_flag = 0;
kal_uint8  Tflash_exist_flag = 0;
va_list ap;
applib_time_struct dt;
kal_uint32 vsprintf_ret = 0;
kal_uint32 time_stamp_count = 0;

void Bird_Log_Init(void)
{
    applib_time_struct dt;
    FS_HANDLE 		dir_handle = -1;
    FS_DOSDirEntry file_info;
    WCHAR *file_name;
    char *file_name_str;
    WCHAR *file_path;
    WCHAR *rtlog_dir;
    U32 file_path_len = (BIRD_FILE_PATH_LEN + 1) * 2;
    S32 result = FS_NO_ERROR;

    file_name = (WCHAR*)OslMalloc(file_path_len);
    file_path = (WCHAR*)OslMalloc(file_path_len);
    rtlog_dir = (WCHAR*)OslMalloc(file_path_len);
    file_name_str = (CHAR*)OslMalloc(file_path_len);
    kal_prompt_trace(MOD_SOC,"Bird_Log_Init enter!");
    memset(rtlog_dir, 0, sizeof(rtlog_dir));
    kal_wsprintf((kal_wchar*) rtlog_dir,BIRD_RTLOG_DIR,(S16)MMI_CARD_DRV);
    dir_handle = FS_CreateDir(rtlog_dir);
    if ((dir_handle < 0) && (dir_handle != FS_FILE_EXISTS))
    {
        kal_prompt_trace(MOD_SOC,"Bird_Log_Init FS_CreateDir Failed! dir_handle = %d", dir_handle);
        Tflash_exist_flag = 0;
        OslMfree(file_name);
        OslMfree(file_path);
        OslMfree(file_name_str);
        OslMfree(rtlog_dir);
        return;
    }

    applib_dt_get_rtc_time(&dt);
#if 	BIRD_RTLOG_CLEAN_LOG
    memset(rtlog_dir, 0, sizeof(rtlog_dir));
    kal_wsprintf((kal_wchar*) rtlog_dir, BIRD_RTLOG_FILE, (S16)MMI_CARD_DRV);
    dir_handle = FS_FindFirst(rtlog_dir, 0, 0, &file_info, file_name, file_path_len);
    kal_prompt_trace(MOD_SOC,"Bird_Log_Init FS_FindFirst dir_handle = %d, dt.nHour  = %d", dir_handle, dt.nHour);
    while (dir_handle >= 0 && result == FS_NO_ERROR)
    {
        if (dt.nHour != 2)/*delete log file when am 2h*/
            break;

        memset(file_name_str, 0,sizeof(file_name_str));
        mmi_wcs_to_asc(file_name_str, file_name);
        kal_prompt_trace(MOD_SOC,"Bird_Log_Init file_name_str=%s", file_name_str);
        memset(rtlog_dir, 0, sizeof(rtlog_dir));
        kal_wsprintf((kal_wchar*) rtlog_dir,BIRD_RTLOG_DIR,(S16)MMI_CARD_DRV);
        mmi_wcscat(rtlog_dir, L"\\");
        mmi_wcscat(rtlog_dir, file_name);
        FS_Delete(rtlog_dir);

        memset(file_name_str, 0,sizeof(file_name_str));
        mmi_wcs_to_asc(file_name_str, rtlog_dir);
        kal_prompt_trace(MOD_SOC,"Bird_Log_Init file_name_str dir=%s", file_name_str);
        result = FS_FindNext(dir_handle, &file_info, file_name, file_path_len);
    }
#endif

    memset(file, 0, sizeof(file));
    kal_wsprintf((kal_wchar*) file, BIRD_RTLOG_FILE, (S16)MMI_CARD_DRV);
    dir_handle = FS_FindFirst(file, 0, 0, &file_info, file_name, file_path_len);
    kal_prompt_trace(MOD_SOC,"Bird_Log_Init FS_FindFirst dir_handle = %d", dir_handle);
    if (dir_handle >= 0)
    {
        memset(file_name_str, 0,sizeof(file_name_str));
        mmi_wcs_to_asc(file_name_str, file_name);
        kal_prompt_trace(MOD_SOC,"Bird_Log_Init file_name_str=%s", file_name_str);
        kal_wsprintf((kal_wchar*) file,BIRD_RTLOG_DIR,(S16)MMI_CARD_DRV);
        mmi_wcscat(file, L"\\");
        mmi_wcscat(file, file_name);
    }
    else
    {
        kal_wsprintf((kal_wchar*) file,BIRD_RTLOG_FILE_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour, dt.nMin, dt.nSec);
        memset(file_name_str, 0,sizeof(file_name_str));
        mmi_wcs_to_asc(file_name_str, file);
        kal_prompt_trace(MOD_SOC,"Bird_Log_Init file_name_str else=%s", file_name_str);
    }
    memset(buf, 0, sizeof(buf));
    memset(buf_log, 0, sizeof(buf_log));
    buf_len = 0;
    write_flag = 0;
    Tflash_exist_flag = 1;
    file_handle = FS_Open((U16 *)file, FS_CREATE| FS_READ_WRITE);
    if (file_handle >= FS_NO_ERROR)
    {
        kal_prompt_trace(MOD_SOC,"FS_Open file success!");
        FS_Close(file_handle);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"FS_Open file fail! file_handle = %d", file_handle);
    }
    OslMfree(file_name);
    OslMfree(file_path);
    OslMfree(file_name_str);
    OslMfree(rtlog_dir);
}

void Bird_Log_Write(void)
{
    kal_uint32 ret_len = 0;

    if (add_flag == 1)
    {
        return;
    }
    StopTimer(Bird_LOG_WRITE);
    time_stamp_count++;
    if (time_stamp_count > BIRD_TIME_STAMP_MAX)
    {
        if (bird_applib_dt_get_rtc_time(&dt) == 0)
        {
            sprintf((char*)buf_dt, "[%04d-%02d-%02d %02d:%02d:%02d]", dt.nYear, dt.nMonth,dt.nDay,dt.nHour, dt.nMin, dt.nSec);
            strcat((char*)buf, buf_dt);
            buf_len += strlen(buf_dt);
            strcat((char*)buf, "\n");
            buf_len ++;
        }
        time_stamp_count = 0;
        kal_prompt_trace(MOD_SOC, "Bird_Log_Write time_stamp_count > BIRD_TIME_STAMP_MAX 1");
    }
    if (buf_len >= BIRD_SAVE_LOG_LENGTH )
    {
        memset(buf, 0, sizeof(buf));
        buf_len = 0;
        write_flag = 0;
        kal_prompt_trace(MOD_SOC, "Bird_Log_Write buf_len >= BIRD_SAVE_LOG_LENGTH");
        return;
    }
    file_handle = FS_Open((U16 *)file,  FS_READ_WRITE);
    if (file_handle >= FS_NO_ERROR)
    {
        FS_Seek( file_handle, 0, FS_FILE_END);
        FS_Write(file_handle, buf, buf_len, &ret_len);
        FS_Close(file_handle);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"Bird_Log_Write error! file_handle = %d", file_handle);
    }
    memset(buf, 0, sizeof(buf));
    buf_len = 0;
    write_flag = 0;
}

void Bird_Log_print(module_type mod_id, const kal_char *fmt,...)
{
    add_flag = 1;
    memset(buf_log, 0, sizeof(buf_log));
    va_start(ap,fmt);
    vsprintf_ret = vsprintf(buf_log, fmt, ap);
    va_end(ap);
    if (vsprintf_ret < 0)
    {
        kal_prompt_trace(MOD_SOC, "Bird_Log_print get buf_log error!");
        add_flag = 0;
        return;
    }
    //kal_prompt_trace(mod_id, "Bird_Log_print %s",  buf_log);
#if 0
    if (Tflash_exist_flag == 0)
        return;
    strcat((char*)buf, buf_log);
    buf_len += strlen(buf_log);
    strcat((char*)buf, "\n");
    buf_len ++;
    add_flag = 0;

    if ((buf_len > BIRD_START_WRITE_LOG_LENGTH) && (write_flag == 0))
    {
        StartTimer(Bird_LOG_WRITE, 1, Bird_Log_Write);
        write_flag = 1;
    }
#endif
}
#endif



