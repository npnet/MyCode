#include "Yd_readposfile.h"
#include "Bird_socket.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"


U16 g_n_readdeal_count = 0;/*离线文件发送次数*/
U16 g_n_readunsend_count = 0;/*休眠状态离线文件已处理统计次数*/
U8 g_n_readunsend_end=0;/*休眠状态离线文件已处理统计结束标志*/
U8 g_n_readdeal_end=0;/*离线文件发送完成标志*/

BIRD_READPOS_STRUCT g_readpos={0};

/*=============================   使用的外部变量与函数====================================*/
extern U8 bird_get_normal_sleep_state();
extern BOOL Lima_get_soc_conn_flag();
extern U8 Yd_get_equstatus();
extern S8 Bird_soc_is_exist_unread(U16 noff);
extern void Bird_soc_conn();
extern S8 Bird_soc_getfrom_file(U16 noff);
extern void Bird_soc_setflag_posfile(applib_time_struct dt);
extern void Yd_DinitSocket();
extern void Bird_soc_sendbufAdd2(Send_Info *_sendinfo);
extern U16 bird_get_real_send_buf_length();

S8 Bird_comp_filename(kal_wchar *name1,kal_wchar *name2)
{	
    U8 i=0;
    char name1_char[30];
    char name2_char[30];

    mmi_wcs_to_asc(name1_char,name1);
    mmi_wcs_to_asc(name2_char,name2);	

    for(i=0;i<14;i++)
    {
       if(name1_char[i]>name2_char[i])
	   	return 1;
       else if(name1_char[i]<name2_char[i])
	   	return -1;

    }
    return 0;
}

S32 Bird_soc_getlast_unsend(kal_wchar *name, U8* unsendtime)
{
    FS_HANDLE  handle=-1;
    FS_HANDLE  findfist_hdl=-1;	
    FS_DOSDirEntry file_info;
    kal_wchar  file[BIRD_FILE_PATH_LEN];
    kal_wchar  file_patten[BIRD_FILE_PATH_LEN];
    char file_name[BIRD_FILE_PATH_LEN];
    kal_wchar first_name[BIRD_FILE_PATH_LEN];
    U32 file_path_len = (BIRD_FILE_PATH_LEN + 1) * 2;
    U32  len;
    U32 lenper;
    U32  buflen;
    S8 buf[MAX_BIRD_SENDBUF_SIZE];
    S32 nresult=-1;
    Send_Info _send;	
    U32 off=0;
    S8 yearchar[6];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];	
    applib_time_struct curtime={0};

    lenper = strlen(BIRD_TBOXINFO);

    memset(file_patten, 0, sizeof(file_patten));
    kal_wsprintf(file_patten,BIRD_TBOXINFO_FILE,(S16)MMI_CARD_DRV);

    findfist_hdl=FS_FindFirst(file_patten, 0, 0, &file_info, name, file_path_len);
	
    memset(file_name, 0, sizeof(file_name));
    mmi_wcs_to_asc(file_name,name);
    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend1 %d %s",findfist_hdl,file_name);

    if (findfist_hdl >= 0)
    {		
        memset(first_name, 0, sizeof(first_name));
        while (FS_FindNext(findfist_hdl, &file_info, first_name, file_path_len) == FS_NO_ERROR)
        {
            memset(file_name, 0, sizeof(file_name));
            mmi_wcs_to_asc(file_name,first_name);
  
            kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend3 %s",file_name);
            if(Bird_comp_filename(first_name,name)<0)
                wcscpy(name,first_name);		
        }; 
        memset(file, 0, sizeof(file));
        kal_wsprintf(file,BIRD_TBOXINFO_DIR,(S16)MMI_CARD_DRV);
        mmi_wcscat(file, L"\\");
        mmi_wcscat(file, name);

        memset(file_name, 0, sizeof(file_name));
        mmi_wcs_to_asc(file_name,file);
        kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend file_name=%s",file_name);
        FS_FindClose(findfist_hdl);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend nofile");
        FS_FindClose(findfist_hdl);
        return nresult;
    }
	
    handle = FS_Open(file, FS_CREATE | FS_READ_WRITE);
    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend2 %d",handle);

    if (handle >= FS_NO_ERROR)
    {
	FS_GetFileSize(handle,&buflen);
	if(buflen>0)
	{
	       for(off=0;off<buflen;off=off+lenper)
		{
			FS_Seek(handle, off, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend %d,%d,%d,%d",buflen,len,off,buf[lenper-1]);
			//未处理9
			memcpy(yearchar, (S8 *)&buf, 4);
			memcpy(monthchar, (S8 *)&buf+4, 2);
			memcpy(daychar, (S8 *)&buf+6, 2);
			memcpy(hourchar, (S8 *)&buf+8, 2);
			memcpy(minchar, (S8 *)&buf+10, 2);
			memcpy(secchar, (S8 *)&buf+12, 2);
			applib_dt_get_rtc_time(&curtime);   

			if((atol(yearchar)<curtime.nYear)||(atol(monthchar)<curtime.nMonth)||(atol(daychar)<curtime.nDay)
				||(atol(hourchar)<curtime.nHour)||(atol(minchar)+bird_get_ser_res_time()/60+1<curtime.nMin))
			{
			}
			else
			{
			    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend time limit");
			    FS_Close(handle);
			    return nresult;
			}
			if(buf[lenper-1]=='9')
			{
				if((g_readpos.offset!=off/lenper)||(g_readpos.data_time.nYear!=atol(yearchar))||(g_readpos.data_time.nMonth!=atol(monthchar))
					||(g_readpos.data_time.nDay!=atol(daychar))||(g_readpos.data_time.nHour!=atol(hourchar))
					||(applib_dt_compare_time(&curtime,&g_readpos.cur_time,NULL)!= DT_TIME_LESS))
				{
				    g_readpos.offset=off/lenper;
				    g_readpos.data_time.nYear=atol(yearchar);
				    g_readpos.data_time.nMonth=atol(monthchar);
				    g_readpos.data_time.nDay=atol(daychar);
				    g_readpos.data_time.nHour=atol(hourchar);
				    g_readpos.cur_time.nYear=curtime.nYear;
				    g_readpos.cur_time.nMonth=curtime.nMonth;
				    g_readpos.cur_time.nDay=curtime.nDay;
				    g_readpos.cur_time.nHour=curtime.nHour;
				    g_readpos.cur_time.nMin=curtime.nMin+bird_get_ser_res_time()/60+1;
				    memcpy(unsendtime, (S8 *)&buf, 14);
				    return off/lenper;
				}
				else
				{
				    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend time limit2");
				}
				FS_Close(handle);
				return nresult;
			}
			else if((off+lenper==buflen)&&(buf[lenper-1]=='1'))
			{
				kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend delete file");
				if((atol(yearchar)<curtime.nYear)||(atol(monthchar)<curtime.nMonth)||(atol(daychar)<curtime.nDay)
				||(atol(hourchar)<curtime.nHour))
				{ 
				    FS_Close(handle);
				    FS_Delete(file);
				    return nresult;
				}
				else
				{
				    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend delete time limit");
				    FS_Close(handle);
				    return nresult;
				}
			}
		}
	}
	else{
		kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend buflen<=0");
		FS_Close(handle);
		FS_Delete(file);
		return nresult;
	}

    }
    FS_Close(handle);
    return nresult;
}

U8 bird_busend_handle(U8* unsendtime, kal_wchar* file_name)
{
    FS_HANDLE  handle=-1;
    U32  len;
    U32 lenper;
    U32  buflen;
    kal_wchar  name[BIRD_FILE_PATH_LEN];
    char pr_name[BIRD_FILE_PATH_LEN];
    S8 buf[MAX_BIRD_SENDBUF_SIZE];
    U8 nresult=0;
    Send_Info _send;	
    U32 noff=0;
    U16 i=0; 
    U8 check_code=0; 
    S8 comp_buf[15]={0};
    applib_time_struct nfind={0};

    lenper = bird_get_real_send_buf_length();
    memset(name, 0, sizeof(name));
    kal_wsprintf(name,BIRD_TBOXDATA_DIR,(S16)MMI_CARD_DRV);
    mmi_wcscat(name, L"\\");
    mmi_wcscat(name, file_name);

    memset(pr_name, 0, sizeof(pr_name));
    mmi_wcs_to_asc(pr_name,name);

    handle = FS_Open(name, FS_CREATE | FS_READ_WRITE);
    kal_prompt_trace(MOD_SOC,"bird_busend_handle %d %s",handle,pr_name);

    if (handle >= FS_NO_ERROR)
    {
	FS_GetFileSize(handle,&buflen);
	if(buflen>=0)
	{
	       for(noff=0;noff<buflen;noff=noff+lenper)
		{
			FS_Seek(handle, noff, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			kal_prompt_trace(MOD_SOC,"bird_busend_handle %d,%d,%d",noff,len,lenper);
			comp_buf[0]='2';
			comp_buf[1]='0';

			for(i=0;i<14;i++)
			{
                         comp_buf[i+2]=buf[24+i]+0x30;
			}
			kal_prompt_trace(MOD_SOC,"bird_busend_handle %s %s",comp_buf,unsendtime);
	              if(strncmp(comp_buf, unsendtime,14) == 0) 
			{
				_send.buf_len=0;
				_send.ini_flag=0;
				_send.send_flow=0;
				_send.send_type=BIRD_SOC_SEND_GPSPOS;
				memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
				memcpy(_send.send_buf,buf,lenper);
				_send.buf_len = lenper;
				
				_send.send_buf[2]=0x03;
				for(i=2;i<_send.buf_len-1;i++)
				    check_code ^=_send.send_buf[i];
				_send.send_buf[lenper-1]=check_code;

				Bird_soc_sendbufAdd2(&_send);
				FS_Close(handle);
				nresult=1;
			}
		}
		if(nresult!=1)
		{
		    S8 yearchar[6];
		    S8 monthchar[4];
		    S8 daychar[4];
		    S8 hourchar[4];
		    S8 minchar[4];
		    S8 secchar[4];	
		    memcpy(yearchar, (S8 *)&unsendtime, 4);
		    memcpy(monthchar, (S8 *)&unsendtime+4, 2);
		    memcpy(daychar, (S8 *)&unsendtime+6, 2);
		    memcpy(hourchar, (S8 *)&unsendtime+8, 2);
		    memcpy(minchar, (S8 *)&unsendtime+10, 2);
		    memcpy(secchar, (S8 *)&unsendtime+12, 2);
		
		    nfind.nYear=atoi(yearchar);
		    nfind.nMonth=atoi(monthchar);
		    nfind.nDay=atoi(daychar);
		    nfind.nHour=atoi(hourchar);
		    nfind.nMin=atoi(minchar);
		    nfind.nSec=atoi(secchar);
		    kal_prompt_trace(MOD_SOC,"bird_busend_handle not find");

		    Bird_soc_setflag_posfile(nfind);
		}
	}
	else{
		kal_prompt_trace(MOD_SOC,"bird_busend_handle buflen<0");
	}

    }
    FS_Close(handle);
    return nresult;
}
void Yd_readposfile()
{
       S32 nresult=-1;
	U8* unsend_time = 0;
	U8 busend = 0;	
	kal_wchar file_name[BIRD_FILE_PATH_LEN];

	if(!Lima_get_soc_conn_flag())
	{
	    return;
	}		

	{
	    nresult = Bird_soc_getlast_unsend(file_name,unsend_time);
	    kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle nresult = %d",nresult);
	    if(nresult>=0)
	    {
	        busend=bird_busend_handle(unsend_time,file_name);
	        kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle busend %d",busend);
	        if(busend==1)
	        {
	            StopTimer(BIRD_READ_FILE);
	            StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
	        }
	        else
	        {
	            kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle no unsend");
	            //StopTimer(BIRD_READ_FILE);
	            //StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);      
	        }
	    }
           else
           {
	        kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle nread<0");
	        StopTimer(BIRD_READ_FILE);
	        StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
           }
	}
}
