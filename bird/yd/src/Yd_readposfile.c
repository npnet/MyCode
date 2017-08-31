#include "Yd_readposfile.h"
#include "Bird_socket.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"


U16 g_n_readdeal_count = 0;/*离线文件发送次数*/
U16 g_n_readunsend_count = 0;/*休眠状态离线文件已处理统计次数*/
U8 g_n_readunsend_end=0;/*休眠状态离线文件已处理统计结束标志*/
U8 g_n_readdeal_end=0;/*离线文件发送完成标志*/
U16 g_n_send_buf=0;

/*=============================   使用的外部变量与函数====================================*/
extern U8 bird_get_normal_sleep_state();
extern BOOL Lima_get_soc_conn_flag();
extern U8 Yd_get_equstatus();
extern S8 Bird_soc_is_exist_unread(U16 noff);
extern void Bird_soc_conn();
extern S8 Bird_soc_getfrom_file(U16 noff);
extern void Bird_soc_setflag_posfile(U16 noff);
extern void Yd_DinitSocket();
void Bird_soc_sendbufAdd2(Send_Info *_sendinfo);
	
S32 Bird_soc_getlast_unsend(kal_wchar *name)
{
    S32  handle;
    FS_DOSDirEntry file_info;
    kal_wchar  file[BIRD_FILE_PATH_LEN];
    kal_wchar  file_patten[BIRD_FILE_PATH_LEN];
    char file_name[BIRD_FILE_PATH_LEN];
    U32 file_path_len = (BIRD_FILE_PATH_LEN + 1) * 2;
    U32  len;
    U32 lenper;
    U32  buflen;
    static S8 buf[MAX_BIRD_SENDBUF_SIZE];
    S32 nresult=-1;
    Send_Info _send;	
    U32 off=0;
    S8 yearchar[4];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];	
    applib_time_struct curtime={0};

    lenper = strlen(BIRD_TBOXINFO);

    memset(file_patten, 0, sizeof(file_patten));
    kal_wsprintf(file_patten,BIRD_TBOXINFO_FILE,(S16)MMI_CARD_DRV);

    handle=FS_FindFirst(file_patten, 0, 0, &file_info, name, file_path_len);
    kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend1 %d",handle);

    if (handle >= 0)
    {
        memset(file, 0, sizeof(file));
        kal_wsprintf(file,BIRD_TBOXINFO_DIR,(S16)MMI_CARD_DRV);
        mmi_wcscat(file, L"\\");
        mmi_wcscat(file, name);
		
        memset(file_name, 0, sizeof(file_name));
        mmi_wcs_to_asc(file_name,file);
        kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend file_name=%s",file_name);
        FS_FindClose(handle);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"Bird_soc_getlast_unsend nofile");
        FS_FindClose(handle);
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
				||(atol(hourchar)<curtime.nHour)||(atol(minchar)+5<curtime.nMin))
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
				nresult = off/lenper;
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

U8 bird_busend_handle(U32 off, kal_wchar* file_name)
{
    S32  handle;
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
	
    lenper = g_n_send_buf;
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
	       noff=off*lenper;
		{
			FS_Seek(handle, noff, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			kal_prompt_trace(MOD_SOC,"bird_busend_handle %d,%d,%d",noff,len,lenper);
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
				return nresult;
			}
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
	S32 nread = 0;
	U8 busend = 0;	
	kal_wchar file_name[BIRD_FILE_PATH_LEN];

	{
	    nread = Bird_soc_getlast_unsend(file_name);
	    kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle nread = %d",nread);
	    if(nread>=0)
	    {
	        busend=bird_busend_handle(nread,file_name);
	        kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle busend %d",busend);
	        if(busend==1)
	        {
	            //StopTimer(BIRD_READ_FILE);
	            //StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
	        }
	        else
	        {
	            kal_prompt_trace(MOD_SOC,"Yd_readposfile_handle no unsend");
	            StopTimer(BIRD_READ_FILE);
	            StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);      
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
