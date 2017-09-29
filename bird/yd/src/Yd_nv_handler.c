#include "Yd_nv_handler.h"
#include "Bird_gpscell.h"
#include "Bird_app.h"
#include "Yd_main.h"


/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern YD_TK002_COMMAND_Info yd_tk002_info;
#ifdef BIRD_BT_SUPPORT    
extern YD_TK003_COMMAND_Info yd_tk003_info;
#endif

extern S32 ReadRecord(nvram_lid_enum nLID, U16 nRecordId, void *pBuffer, U16 nBufferSize, S16 *pError);
extern S32 WriteRecord(nvram_lid_enum nLID, U16 nRecordId, void *pBuffer, U16 nBufferSize, S16 *pError);
extern void yd_init_reboot_control_param(void);
extern void yd_init_acc_control_param(void);
extern void yd_init_defense_param(void);
extern void bird_init_oilcut_param(void);
extern void yd_init_sleep_param(void);
extern void yd_init_wakeup_param(void);


U8 * bird_get_nv_batch(void)
{
     kal_prompt_trace(MOD_SOC,"bird_get_nv_batch"); 
     return yd_tk001_info.nvbatch;
}

U8 * bird_get_nv2_batch(void)
{
     kal_prompt_trace(MOD_SOC,"bird_get_nv2_batch"); 
     return yd_tk002_info.nvbatch;
}

#ifdef BIRD_BT_SUPPORT    
U8 * bird_get_nv3_batch(void)
{
     kal_prompt_trace(MOD_SOC,"bird_get_nv3_batch"); 
     return yd_tk003_info.nvbatch;
}
#endif

U8 * bird_get_nv5_batch(void)
{
     kal_prompt_trace(MOD_SOC,"bird_get_nv5_batch"); 
     return yd_tk005_info.nvbatch;
}

void yd_tk001_set_record_from_NV()
{
	S16 error;     

	WriteRecord(NVRAM_EF_TD_TK001_SETTING,
				1,
				&yd_tk001_info,
				NVRAM_YD_TK001_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"yd_tk001_set_record_from_NV success..");
	}
}

void yd_tk002_set_record_from_NV()
{
	S16 error;     

	WriteRecord(NVRAM_EF_TD_TK002_SETTING,
				1,
				&yd_tk002_info,
				NVRAM_YD_TK002_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"yd_tk002_set_record_from_NV success..");
	}
}

#ifdef BIRD_BT_SUPPORT    
void yd_tk003_set_record_from_NV()
{
	S16 error;     

	WriteRecord(NVRAM_EF_TD_TK003_SETTING,
				1,
				&yd_tk003_info,
				NVRAM_YD_TK003_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"yd_tk003_set_record_from_NV success..");
	}
}
#endif

void yd_tk005_set_record_from_NV()
{
	S16 error;     

	WriteRecord(NVRAM_EF_TD_TK005_SETTING,
				1,
				&yd_tk005_info,
				NVRAM_YD_TK005_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"yd_tk005_set_record_from_NV success..");
	}
}

void yd_tk001_clean_record_from_NV()
{
	memset(&yd_tk001_info, 0x00, sizeof(yd_tk001_info));
	yd_tk001_set_record_from_NV();
	kal_prompt_trace(MOD_SOC,"yd_tk001_clean_record_from_NV");    
}

void yd_tk002_clean_record_from_NV()
{
	memset(&yd_tk002_info, 0x00, sizeof(yd_tk002_info));
	yd_tk002_set_record_from_NV();
	kal_prompt_trace(MOD_SOC,"yd_tk002_clean_record_from_NV");    
}

#ifdef BIRD_BT_SUPPORT    
void yd_tk003_clean_record_from_NV()
{
	memset(&yd_tk003_info, 0x00, sizeof(yd_tk003_info));
	yd_tk003_set_record_from_NV();
	kal_prompt_trace(MOD_SOC,"yd_tk003_clean_record_from_NV");    
}
#endif

void yd_tk005_clean_record_from_NV()
{
	memset(&yd_tk005_info, 0x00, sizeof(yd_tk005_info));
	yd_tk005_set_record_from_NV();
	kal_prompt_trace(MOD_SOC,"yd_tk005_clean_record_from_NV");    
}

void yd_tk001_get_record_from_NV()
{
	S16 error;
	U8  nv_buffer[NVRAM_YD_TK001_RECORD_SIZE] = {0};

	ReadRecord(NVRAM_EF_TD_TK001_SETTING, 1, nv_buffer, NVRAM_YD_TK001_RECORD_SIZE, &error);
	if(NVRAM_READ_SUCCESS == error)
	{
		U16 i;
		for( i =0 ; i<10 ; i++)
		{
			kal_prompt_trace(MOD_SOC,"nv %x ..",nv_buffer[i]);
		}

		if(nv_buffer[0] == 0)
		{
			yd_tk001_set_default_record_to_NV();            
		}
		else
		{
			memcpy(&yd_tk001_info, (const void *)nv_buffer, sizeof(yd_tk001_info)); 	 
			//nv版本为空则设置默认值
			if(bird_get_nv_batch()==NULL)
			{
				kal_prompt_trace(MOD_SOC,"NV BATCH NULL");      
				yd_tk001_set_default_record_to_NV();    
				return;
			}
			if(strcmp((const char *)bird_get_nv_batch(), YD_NV_BATCH) != 0)  
			{
				kal_prompt_trace(MOD_SOC,"NV BATCH SOFT BATCH %s,%s",bird_get_nv_batch(),YD_NV_BATCH);     
				yd_tk001_clean_record_from_NV();
				yd_tk001_set_default_record_to_NV();    
				return;
			}
		}
	}
}

void yd_tk004_get_record_from_NV()
{
	S16 error;
	U8  barcode_buffer[NVRAM_EF_BARCODE_NUM_SIZE] = {0};

	ReadRecord(NVRAM_EF_BARCODE_NUM_LID, 1, barcode_buffer, NVRAM_EF_BARCODE_NUM_SIZE, &error);
	if(NVRAM_READ_SUCCESS == error)
	{
		U16 i;
		for( i =0 ; i<10 ; i++)
		{
			kal_prompt_trace(MOD_SOC,"nv %x ..",barcode_buffer[i]);
		}
		
		if(barcode_buffer[5]==0x34)
		{
			factory_calibration_flag = 1;
		}
		else
		{
			factory_calibration_flag =0;
		}
	}
}

void yd_tk002_set_default_record_to_NV()
{
	S16 error;    

	yd_tk002_info.format_flag = 1;

	memset(yd_tk002_info.server_address, 0x00, sizeof(yd_tk002_info.server_address));
	memcpy((void *)yd_tk002_info.server_address, (const void *)YD_TK001_HTTP, strlen((const char *)YD_TK001_HTTP));
	/*NV默认值*/
	memcpy((void *)yd_tk002_info.nvbatch, (const void *)YD_NV_BATCH, strlen((const char *)YD_NV_BATCH));
	WriteRecord(NVRAM_EF_TD_TK002_SETTING,
				1,
				&yd_tk002_info,
				NVRAM_YD_TK002_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error) 
	{
		kal_prompt_trace(MOD_SOC,"SUCCESS %s %d",__FILE__,__LINE__); 
	}
	else
	{
		kal_prompt_trace(MOD_SOC,"ERR %s %d",__FILE__,__LINE__); 
	}
}

void yd_tk002_get_record_from_NV()
{
	S16 error;
	U8  nv_buffer[NVRAM_YD_TK002_RECORD_SIZE] = {0};
	ReadRecord(NVRAM_EF_TD_TK002_SETTING, 1, nv_buffer, NVRAM_YD_TK002_RECORD_SIZE, &error);
	if(NVRAM_READ_SUCCESS == error)
	{
		U16 i;
		for( i =0 ; i<10 ; i++)
		{
			kal_prompt_trace(MOD_SOC,"nv2 %x ..",nv_buffer[i]);
		}

		if(nv_buffer[0] == 0)
		{
			yd_tk002_set_default_record_to_NV();            
		}
		else
		{
			memcpy(&yd_tk002_info, (const void *)nv_buffer, sizeof(yd_tk002_info)); 	 
			//nv版本为空则设置默认值 
			if(bird_get_nv2_batch()==NULL)
			{
				kal_prompt_trace(MOD_SOC,"NV2 BATCH NULL");      
				yd_tk002_set_default_record_to_NV();    
				return;
			}
			if(strcmp((const char *)bird_get_nv2_batch(), YD_NV_BATCH) != 0)  
			{
				kal_prompt_trace(MOD_SOC,"NV2 BATCH SOFT BATCH %s,%s",bird_get_nv2_batch(),YD_NV_BATCH);     
				yd_tk002_clean_record_from_NV();
				yd_tk002_set_default_record_to_NV();    
				return;
			}
		}
	}
	kal_prompt_trace(MOD_SOC,"yd_tk002_get_record_from_NV %d,%d",yd_tk002_info.format_flag,sizeof(yd_tk002_info));   
}

#ifdef BIRD_BT_SUPPORT    
void yd_tk003_set_default_record_to_NV()
{
	S16 error;    

	yd_tk003_info.format_flag = 1;

	memset(yd_tk003_info.bt_mac, 0x00, sizeof(yd_tk003_info.bt_mac));
	/*NV默认值*/
	memcpy((void *)yd_tk003_info.nvbatch, (const void *)YD_NV_BATCH, strlen((const char *)YD_NV_BATCH));
	WriteRecord(NVRAM_EF_TD_TK003_SETTING,
				1,
				&yd_tk003_info,
				NVRAM_YD_TK003_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"SUCCESS %s %d",__FILE__,__LINE__); 
	}
	else
	{
		kal_prompt_trace(MOD_SOC,"ERR %s %d",__FILE__,__LINE__); 
	}
}

void yd_tk003_get_record_from_NV()
{
	S16 error;
	U8  nv_buffer[NVRAM_YD_TK003_RECORD_SIZE] = {0};

	ReadRecord(NVRAM_EF_TD_TK003_SETTING, 1, nv_buffer, NVRAM_YD_TK003_RECORD_SIZE, &error);
	if(NVRAM_READ_SUCCESS == error)
	{
		U16 i;
		for( i =0 ; i<10 ; i++)
		{
			kal_prompt_trace(MOD_SOC,"nv3 %x ..",nv_buffer[i]);
		}

		if(nv_buffer[0] == 0)
		{
			yd_tk003_set_default_record_to_NV();
		}
		else
		{
			memcpy(&yd_tk003_info, (const void *)nv_buffer, sizeof(yd_tk003_info)); 	 
			//nv版本为空则设置默认值
			if(bird_get_nv3_batch()==NULL)
			{
				kal_prompt_trace(MOD_SOC,"NV3 BATCH NULL");      
				yd_tk003_set_default_record_to_NV();    
				return;
			}
			if(strcmp((const char *)bird_get_nv3_batch(), YD_NV_BATCH) != 0)  
			{
				kal_prompt_trace(MOD_SOC,"NV3 BATCH SOFT BATCH %s,%s",bird_get_nv3_batch(),YD_NV_BATCH);     
				yd_tk003_clean_record_from_NV();
				yd_tk003_set_default_record_to_NV();    
				return;
			}
		}
	}
	kal_prompt_trace(MOD_SOC,"yd_tk003_get_record_from_NV %d,%d",yd_tk003_info.format_flag,sizeof(yd_tk003_info));   
}
#endif

void yd_tk001_set_default_record_to_NV()
{
    S16 error;    
    yd_tk001_info.ntrace= 1;
    yd_tk001_info.format_flag = 1;
    
    yd_tk001_info.illegal_stream_alarm = 1;      

    yd_tk001_info.continue_search.search_times = 0;    
    yd_tk001_info.continue_search.search_interval = 0; 

    yd_tk001_info.main_interval = 15;

    yd_tk001_info.alarm_delay = 30;		//30;

/*lrf add*/
   yd_tk001_info.heart_interval = 120;

   yd_tk001_info.nrestarttimes=1;
   yd_tk001_info.CALLLOCK=1; //sf
   yd_tk001_info.CALLDISP=1;//YZ
   yd_tk001_info.SMS=0;
   yd_tk001_info.VIBCALL=0;
   yd_tk001_info.agps_pos_lati =0x2D47F9;
   yd_tk001_info.agps_pos_long=0xB948AA;
   yd_tk001_info.agps_pos_alti=0x1D;
   yd_tk001_info.agps_llapos_lati =0x11B01D30;
   yd_tk001_info.agps_llapos_long=0x48606164;
   yd_tk001_info.agps_llapos_alti=0x0C62;   
   yd_tk001_info.Corner=0;
   yd_tk001_info.hitlvalue = 7; 
   yd_tk001_info.hitldelaytime = 4; 
   yd_tk001_info.hitllatchtime = 5; 
   yd_tk001_info.motor_state= 0;
   yd_tk001_info.equmode=0;
   yd_tk001_info.lock_state= 0;
   yd_tk001_info.limit_speed=0;
   yd_tk001_info.uart_bat_interval=10;
   yd_tk001_info.car_status=0;
   
	bird_init_oilcut_param();

	yd_init_sleep_param();
	yd_init_wakeup_param();
	yd_init_reboot_control_param();
	yd_init_acc_control_param();
	yd_init_defense_param();
   
	yd_tk001_info.rollover_alarm = 1;
	yd_tk001_info.az_flag = 0;
	yd_tk001_info.move_position_alarm = 1;
	yd_tk001_info.move_position_distance = 300;
	yd_tk001_info.sensitivity_level = 5;
	yd_tk001_info.vibration_value.vibration_level = 10;


	#ifdef __BIRD_RUN_EINT__
		yd_tk001_info.vibration_value.vibration_times = 1;       
	#else
		yd_tk001_info.vibration_value.vibration_times = 5;
	#endif

#ifdef BIRD_SWZ	
	yd_tk001_info.vibration_running_alarm = 0;  
#else
	yd_tk001_info.vibration_running_alarm = 1;  
#endif

	yd_tk001_info.speed_alarm = 0;
	yd_tk001_info.speed_int = 0;
	yd_tk001_info.external_power_alarm = 1;
	yd_tk001_info.POFT = 5;
	yd_tk001_info.negative_voltage_alarm = 1;
	yd_tk001_info.nlbv = 3650000;

   memset(yd_tk001_info.CID, 0x00, sizeof(yd_tk001_info.CID));
   
   memset(yd_tk001_info.sys_password, 0x00, sizeof(yd_tk001_info.sys_password));
   //memset(yd_tk001_info.server_address, 0x00, sizeof(yd_tk001_info.server_address));
   memset(yd_tk001_info.abd_address, 0x00, sizeof(yd_tk001_info.abd_address));
   memset(yd_tk001_info.agps_address, 0x00, sizeof(yd_tk001_info.agps_address));
   memset(yd_tk001_info.admin_pw, 0x00, sizeof(yd_tk001_info.admin_pw));
   
    memcpy((void *)yd_tk001_info.sys_password, (const void*)YD_TK001_PW, strlen((const void *)YD_TK001_PW));
    //memcpy((void *)yd_tk001_info.server_address, (const void *)YD_TK001_HTTP, strlen((const char *)YD_TK001_HTTP));
    memcpy((void *)yd_tk001_info.abd_address, (const void *)BD_AGNSS_ABDS_1, strlen((const char *)BD_AGNSS_ABDS_1));
    memcpy((void *)yd_tk001_info.agps_address, (const void *)BD_AGNSS_AGPS_1, strlen((const char *)BD_AGNSS_AGPS_1));
    memcpy((void *)yd_tk001_info.ipadd, (const void *)YD_TK001_IP, strlen((const char *)YD_TK001_IP));
    memcpy((void *)yd_tk001_info.admin_pw, (const void *)YD_TK001_YD_ADMIN_PSW, strlen((const char *)YD_TK001_YD_ADMIN_PSW));
	/*NV???*/
    memcpy((void *)yd_tk001_info.nvbatch, (const void *)YD_NV_BATCH, strlen((const char *)YD_NV_BATCH));
    WriteRecord(NVRAM_EF_TD_TK001_SETTING,
                        1,
                        &yd_tk001_info,
                        NVRAM_YD_TK001_RECORD_SIZE,
                        &error);
    if(NVRAM_WRITE_SUCCESS == error)
    {
        kal_prompt_trace(MOD_SOC,"SUCCESS %s %d",__FILE__,__LINE__); 
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"ERR %s %d",__FILE__,__LINE__); 
    }
}

void yd_tk005_set_default_record_to_NV()
{
	S16 error;    

	yd_tk005_info.format_flag = 1;
	
	/*NV默认值*/
	memcpy((void *)yd_tk005_info.nvbatch, (const void *)YD_NV_BATCH, strlen((const char *)YD_NV_BATCH));
	yd_tk005_info.savedata_ival=2000;
	yd_tk005_info.nmal_main_ival=10;
	yd_tk005_info.alarm_main_ival=1000;
	yd_tk005_info.heart_ival=120;
	yd_tk005_info.ter_res_time=60;
	yd_tk005_info.ser_res_time=60;
	yd_tk005_info.conn_interval=1;

	WriteRecord(NVRAM_EF_TD_TK005_SETTING,
				1,
				&yd_tk005_info,
				NVRAM_YD_TK005_RECORD_SIZE,
				&error);
	if(NVRAM_WRITE_SUCCESS == error)
	{
		kal_prompt_trace(MOD_SOC,"SUCCESS %s %d",__FILE__,__LINE__); 
	}
	else
	{
		kal_prompt_trace(MOD_SOC,"ERR %s %d",__FILE__,__LINE__); 
	}
}

void yd_tk005_get_record_from_NV()
{
	S16 error;
	U8  nv_buffer[NVRAM_YD_TK005_RECORD_SIZE] = {0};

	ReadRecord(NVRAM_EF_TD_TK005_SETTING, 1, nv_buffer, NVRAM_YD_TK005_RECORD_SIZE, &error);
	if(NVRAM_READ_SUCCESS == error)
	{
		U16 i;
		for( i =0 ; i<10 ; i++)
		{
			kal_prompt_trace(MOD_SOC,"nv5 %x ..",nv_buffer[i]);
		}

		if(nv_buffer[0] == 0)
		{
			yd_tk005_set_default_record_to_NV();
		}
		else
		{
			memcpy(&yd_tk005_info, (const void *)nv_buffer, sizeof(yd_tk005_info)); 	 
			//nv版本为空则设置默认值
			if(bird_get_nv5_batch()==NULL)
			{
				kal_prompt_trace(MOD_SOC,"NV5 BATCH NULL");      
				yd_tk005_set_default_record_to_NV();    
				return;
			}
			if(strcmp((const char *)bird_get_nv5_batch(), YD_NV_BATCH) != 0)  
			{
				kal_prompt_trace(MOD_SOC,"NV5 BATCH SOFT BATCH %s,%s",bird_get_nv5_batch(),YD_NV_BATCH);     
				yd_tk005_clean_record_from_NV();
				yd_tk005_set_default_record_to_NV();    
				return;
			}
		}
	}
	kal_prompt_trace(MOD_SOC,"yd_tk005_get_record_from_NV %d,%d",yd_tk005_info.format_flag,sizeof(yd_tk005_info));   
}

void yd_send_save_nv_msg() 
{
	ilm_struct       *rj_ilm = NULL; 
	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SAVENV, 0);       
	msg_send_ext_queue(rj_ilm);		
}

void yd_set_save_nv_msg()
{
	SetProtocolEventHandler(yd_tk001_set_record_from_NV,MSG_ID_YD_TK001_MSG_SAVENV);
}

