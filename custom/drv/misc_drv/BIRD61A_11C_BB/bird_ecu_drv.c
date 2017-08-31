#ifdef BIRD_UART2_SUPPORT
#ifdef BIRD_ECU_SUPPORT
#include 	"stack_common.h"  
#include 	"stack_msgs.h"
#include 	"app_ltlcom.h"       /* Task message communiction */
#include 	"syscomp_config.h"
#include 	"task_config.h"
#include 	"drv_comm.h"
//#include    "adc.h"
#include    "drvsignals.h"
#include    "eint.h"
#include     "l1audio.h"
#include     "intrctrl.h"

#include "kal_general_types.h"
#include "stack_config.h"
#include "task_main_func.h"
#include "kal_public_api.h"
#include "stack_ltlcom.h"
#include "hisr_config.h"

#include    "dcl.h"

#include "mmi_features.h"
#include "TimerEvents.h"
#include "device.h"
#include "app_datetime.h"
#include "rj_dw02.h"


#include "bird_app.h"
#include    "dcl_adc.h"

extern U8 g_n_ydislogin; 
kal_uint8 g_m_readuartdata = 0;
kal_uint8 ECU2GPS_buf[60] = {0};
kal_uint8 ECU2GPS_tmp_buf[60] = {0};
kal_uint8 ECU2GPS_C0_CMD1[10] = {0};
kal_uint8 ECU2GPS_C0_CMD2[30] = {0};
kal_uint8 ECU2GPS_C0_CMD3[30] = {0};
kal_uint8 ECU2GPS_C0_CMD4[60] = {0};
kal_uint8 ECU2GPS_C0_CMD5[60] = {0};
kal_uint8 ECU2GPS_C0_CMD6[60] = {0};
kal_uint8 ECU2GPS_C0_CMD7[60] = {0};
kal_uint8 ECU2GPS_C0_CMD8[60] = {0};
kal_uint8 ECU2GPS_C0_CMD9[60] = {0};
kal_uint8 ECU2GPS_C0_CMD10[60] = {0};
kal_uint8 ECU2GPS_C0_CMD11[60] = {0};
kal_uint8 ECU2GPS_C0_CMD12[60] = {0};
kal_uint8 ECU2GPS_C0_CMD13[60] = {0};
kal_uint8 ECU2GPS_A0_CMD[60] = {0};
kal_uint8 ECU2GPS_C9_CMD[60] = {0};

static kal_uint8 cnt=0;
static kal_uint8 ECU_status=0;     //ECU_status=1 -- A1    ECU_status=2 -- A2
static kal_uint8 C0_index = 0;
static kal_uint8 Command0_type = 0;
static kal_uint8 C9_index = 0;
static kal_uint8 Command1_type = 0;
volatile kal_uint8 ECU_COM_connect_flag = 0;

extern void ACC_ECU_check_timer();
extern kal_bool defend_switch_status; 

#ifdef __BIRD_DW22__
	kal_uint8	power_voltage_percent;
#endif	


//防盗拆数剧处理开始0xc0 0x01
void GPS_Anti_theft_cmd_reply() //GPS防盗拆应答数据
{
	kal_uint8 GPS_reply_wirte_buf[100] = {0};
	kal_uint16 tmp_buf=0;
	kal_uint16 i;	
	GPS_reply_wirte_buf[0] = 0xA9;
	GPS_reply_wirte_buf[1] = 0x05;	
	GPS_reply_wirte_buf[2] = 0x01;
	GPS_reply_wirte_buf[3] = 0x42;
	GPS_reply_wirte_buf[4] = 0x54;
	GPS_reply_wirte_buf[5] = 0x92;	
	GPS_reply_wirte_buf[6] = 0x44;
	tmp_buf=GPS_reply_wirte_buf[0];
	for(i=1;i<7;i++)
	{
	tmp_buf ^=GPS_reply_wirte_buf[i];
	}
	GPS_reply_wirte_buf[7] = tmp_buf;
	GPS_reply_wirte_buf[8] = 0x0d;	
	ECU_UART_Write(GPS_reply_wirte_buf,9);
	kal_prompt_trace(MOD_SOC, "GPS_Anti_theft_cmd_reply");			

}
void ECU_Anti_theft_cmd_check() //第三方设备防盗拆数据解析
{
	if(ECU2GPS_C0_CMD1!=0)
	{
		if((ECU2GPS_C0_CMD1[3]==0x42)&&(ECU2GPS_C0_CMD1[4]==0x54)&&(ECU2GPS_C0_CMD1[5]==0x92)&&(ECU2GPS_C0_CMD1[6]==0x44))
		{
			kal_prompt_trace(MOD_SOC, "ECU_Anti_theft_cmd_check");			
			GPS_Anti_theft_cmd_reply();
		}
	}
}
//防盗拆数据处理结束

volatile kal_uint8 ACC_flag=0,b_motion_flag=0,vibrate_flag=0,az_flag=0,lock_electric_flag=0,electric_motor_flag=0,Rollover_flag=0, defend_flag=0xff;
volatile kal_uint8 speed_limit_flag=0;
kal_uint8 ECU_SUMbuf[300] = {0};
kal_uint8 lock_flag=0xFF;
volatile kal_uint8 touchuan_count = 0;
kal_bool ecu_motion_flag = KAL_FALSE ;
kal_uint8 ECU_connect = 0;
extern kal_uint32 ecu_count;
kal_uint8 ECU_connect_cnt = 2;
kal_uint8 ECU_ACC_status_flag=0;
kal_uint8 ECU_charger_status_flag=0;
kal_uint8 ECU_defend_avlid_flag=0;
kal_uint8 ECU_acc_avlid_flag=0;
kal_uint32 ECU_speed_data = 0;
kal_uint32 ECU_mileage_data =0;
kal_uint32 ECU_sum_mileage_data =0;
kal_uint8 ECU_buffer_flag = 0;
kal_uint16 ECU_battery_status = 0;
kal_uint8 ECU_start_status_flag=0;
kal_uint8 ECU_start_mode_flag=0;
//实时状态数据处理开始0xc0 0x02
void ECU_real_time_status_cmd_check() //第三方设备实时状态数据解析
{
	kal_uint8 cvalue,cvalue1,cvalue2,cvalue3,cvalue4,cvalue5,cvalue6,cvalue7,cvalue8,cvalue9;	
	static kal_uint8 count_vib = 0;
	static kal_uint8 count_rollover_run = 0;	
	static kal_uint8 count_rollover_static = 0;		
	static kal_uint8 wrap_cnt = 0;
	int i;
	static kal_uint8 count_sport = 0;

	static kal_uint16 wrap_index2_cnt =0;
	static kal_uint16 byte_cnt =0;

	kal_prompt_trace(MOD_SOC, "ECU_UART_READ_HANDLE");	
	ECU_buffer_flag = 1;
	//wrap_index2_cnt++;

	//if(wrap_index2_cnt == 15)
	//	{
		memset(ECU_SUMbuf,0,300);	
		memcpy(ECU_SUMbuf,ECU2GPS_C0_CMD2,24);
	//	wrap_index2_cnt = 0;
		touchuan_count = 24;
	//	Bird_ecu_data_alarm();
	//	}
		//设防标志位

#ifdef __BIRD_DW22__
		power_voltage_percent = ECU2GPS_C0_CMD2[11];
#endif	
		cvalue9 = ECU2GPS_C0_CMD2[21] & 0x80;
		if(cvalue9 == 0x80)
		{
			ECU_defend_avlid_flag=1;
		}
		else
		{
			ECU_defend_avlid_flag=0;
		}

		cvalue = ECU2GPS_C0_CMD2[7] & 0x02;
		kal_prompt_trace(MOD_SOC," ECU_UART_READ_HANDLE DEFEND:%x %x ",ECU2GPS_C0_CMD2[7]  ,cvalue);
/*
			if((ECU_connect_cnt == 1)||(ECU_connect_cnt == 2))
			{
				if(cvalue == 0x02)
				{
					defend_switch_status = KAL_TRUE;
					defend_switch_on();
					//bird_set_soc_log_info(ECU_status_log,"def_on_cnt");
				}
				else
				{
					defend_switch_status = KAL_FALSE;
					defend_switch_off();
					//bird_set_soc_log_info(ECU_status_log,"def_off_cnt");
				}

			}
			else
			*/
			{
				if(cvalue == 0x02)
				{
					if(defend_flag != 1)
					{
						defend_switch_status = KAL_TRUE;
						defend_switch_on();
					//bird_set_soc_log_info(ECU_status_log,"def_on");

					}
					defend_flag=1;
				}
				else
				{
					if(defend_flag != 0)
					{
						defend_switch_status = KAL_FALSE;
						defend_switch_off();
					//bird_set_soc_log_info(ECU_status_log,"def_off");

					}
					defend_flag = 0;
				}
			}

	       
		//if sport
		cvalue1 = ECU2GPS_C0_CMD2[7] & 0x80;
		if(cvalue1 == 0x80)
		{
			if(b_motion_flag==0 || b_motion_flag==2)
				{
				 ecu_motion_flag = KAL_TRUE;
				rj_ecu_running_alarm();
				}
			b_motion_flag=1;
			if(count_sport == 0 || count_sport == 15)
			{
			Bird_vibrate_is_on_alarm();
			count_sport = 0;
			}
			count_sport++;

		}
		else
		{
			if(b_motion_flag==0 || b_motion_flag==1)
				{
				 ecu_motion_flag = KAL_FALSE;
				rj_ecu_static_alarm();
				}
			b_motion_flag=2;
			count_sport = 0;			
		}
		
		//if vibrate
		cvalue2 = ECU2GPS_C0_CMD2[7] & 0x40;
		if(cvalue2 == 0x40)
		{
		
			if(count_vib==0 || count_vib==15) 
				{
				Bird_vibrate_is_on_alarm();	
				count_vib = 0;
				}
			count_vib++ ;
			
		}
		else
		{
		count_vib = 0;
		}
		
		//if AZ
		cvalue3 = ECU2GPS_C0_CMD2[7] & 0x20;
		if(cvalue3 == 0x20)
		{
			if(az_flag==0 || az_flag==2)
				{
				Bird_az_is_open_alarm();
				}
			az_flag=1; 	
		}
		else
		{
			if(az_flag==0 || az_flag==1)
				{
				Bird_az_is_close_alarm();
				}
			az_flag=2; 	
		}

		// lock_car status
		cvalue4 = ECU2GPS_C0_CMD2[5] & 0x04;
		if(cvalue4 == 0x04)
		{
			if((lock_flag == 0xFF)||(lock_flag == 0))
			{
			lock_flag=1;
			Bird_electric_motor_disconnect_status();
			}

		}
		else
		{
			if((lock_flag == 0xFF)||(lock_flag == 1))
			{
			lock_flag=0;				
			Bird_electric_motor_normal_status();
			}
		}
		//限速	
		cvalue5 = ECU2GPS_C0_CMD2[5] & 0x02;

		if(cvalue5 == 0x02)
		{
			if((speed_limit_flag == 2)||(speed_limit_flag == 0))
			{
			speed_limit_flag=1;
			Bird_speed_limit_on();
			}

		}
		else
		{
			if((speed_limit_flag == 0)||(speed_limit_flag == 1))
			{
			speed_limit_flag=2;				
			Bird_speed_limit_off();
			}
		}
// ACC 标志位
		cvalue8 = ECU2GPS_C0_CMD2[21] & 0x40;
		if(cvalue8== 0x40)
			{
			ECU_acc_avlid_flag=1;
			}
		else
		{
			ECU_acc_avlid_flag=0;
		}

//  ACC ON/OFF
		cvalue6 = ECU2GPS_C0_CMD2[6] & 0x80;

		if(cvalue6 == 0x80)
		{
			if((ECU_ACC_status_flag == 2)||(ECU_ACC_status_flag == 0))
			{
			ECU_ACC_status_flag=1;
			Bird_ECU_ACC_on();
			}

		}
		else
		{
			if((ECU_ACC_status_flag == 0)||(ECU_ACC_status_flag == 1))
			{
			ECU_ACC_status_flag=2;				
			Bird_ECU_ACC_off();}
		}

//充电/未充电		
		cvalue7 = ECU2GPS_C0_CMD2[7] & 0x01;

		if(cvalue7 == 0x01)
		{
			if((ECU_charger_status_flag == 2)||(ECU_charger_status_flag == 0))
			{
			ECU_charger_status_flag=1;
			Bird_ecu_charging_on();
			}

		}
		else
		{
			if((ECU_charger_status_flag == 0)||(ECU_charger_status_flag == 1))
			{
			ECU_charger_status_flag=2;				
			Bird_ecu_uncharging();
			}
		}
		ECU_battery_status = ECU2GPS_C0_CMD2[11];
		ECU_speed_data = ECU2GPS_C0_CMD2[13] *256 + ECU2GPS_C0_CMD2[14] ;
		kal_prompt_trace(MOD_SOC," ECU_speed =%d",ECU_speed_data );		
		ECU_mileage_data = ECU2GPS_C0_CMD2[15] *256 + ECU2GPS_C0_CMD2[16] ;
		kal_prompt_trace(MOD_SOC," ECU_mileage =%d",ECU_mileage_data );		
		ECU_sum_mileage_data = (ECU2GPS_C0_CMD2[17] <<16)+ (ECU2GPS_C0_CMD2[18] <<8)+(ECU2GPS_C0_CMD2[19] );	
		kal_prompt_trace(MOD_SOC," ECU_sum_mileage =%d",ECU_sum_mileage_data );		

//  整车启动/关闭
		cvalue8 = ECU2GPS_C0_CMD2[5] & 0x01;

		if(cvalue8 == 0x01)
		{
			if((ECU_start_status_flag == 2)||(ECU_start_status_flag == 0))
			{
			ECU_start_status_flag=1;
			Bird_ECU_start();
			}

		}
		else
		{
			if((ECU_start_status_flag == 0)||(ECU_start_status_flag == 1))
			{
			ECU_start_status_flag=2;				
			Bird_ECU_close();}
		}

//启动模式
		cvalue9 = ECU2GPS_C0_CMD2[6] & 0x30;

		if(cvalue9 == 0x00)
		{
			if((ECU_start_mode_flag == 2)||(ECU_start_mode_flag == 0)||(ECU_start_mode_flag == 3)||(ECU_start_mode_flag == 4))
			{
			ECU_start_mode_flag=1;
			Bird_soft_start();
			}

		}
		else if(cvalue9 == 0x10)
		{
			if((ECU_start_mode_flag == 1)||(ECU_start_mode_flag == 0)||(ECU_start_mode_flag == 3)||(ECU_start_mode_flag == 4))
			{
			ECU_start_mode_flag=2;				
			Bird_hard_start();}
		}	
		else if(cvalue9 == 0x20)
		{
			if((ECU_start_mode_flag == 2)||(ECU_start_mode_flag == 0)||(ECU_start_mode_flag == 1)||(ECU_start_mode_flag == 4))
			{
			ECU_start_mode_flag=3;				
			Bird_speed_start();}
		}	
		else
		{
			if((ECU_start_mode_flag == 2)||(ECU_start_mode_flag == 0)||(ECU_start_mode_flag == 3)||(ECU_start_mode_flag == 1))
			{
			ECU_start_mode_flag=4;				
			Bird_selfdefine_start();
			}			
		}
		
}
kal_uint16 tone_number = 0;
kal_uint16 tone_volume =0;
kal_uint8 ECU_C0_CMD2_length()
{
	return touchuan_count;
}
kal_uint8 ECU_C0_CMD2_flag()
{
	return ECU_buffer_flag;
}
kal_uint32 ECU_speed()
{
	return ECU_speed_data;
}
kal_uint32 ECU_single_mileage()
{
	return ECU_mileage_data;
}
kal_uint32 ECU_sum_mileage()
{
	return ECU_sum_mileage_data;
}
kal_uint8 ECU_defend_status()
{
	return ECU_defend_avlid_flag;
}
kal_uint8 ECU_ACC_status()
{
	return ECU_acc_avlid_flag;
}
kal_uint16 ECU_BATTERY_percent()
{
	return ECU_battery_status;
}
kal_uint16 play_tone_number()
{
	return tone_number;
}
kal_uint16 get_tone_volume()
{
	return tone_volume;
}
kal_uint8 ECU_restart_flag = 0;
kal_uint8 ECU_close_flag = 0;

//ECU控制GPS指令处理 0xc0 0x09
void ECU_contronl_cmd_check()
{
	kal_uint16 cvalue1,cvalue2,cvalue3,cvalue4;
	kal_uint16 cvalue5,cvalue6,cvalue7,cvalue8;
	kal_prompt_trace(MOD_SOC," ECU_contronl_cmd_check");
	
	cvalue1 = ECU2GPS_C0_CMD9[3] & 0x02;
	cvalue3 = ECU2GPS_C0_CMD9[6] & 0x02;	
	if(cvalue3 == 0x02)
		{
			if(cvalue1 == 0x02)
			{			
				Bird_ECU_sent_restart();
			}
		}
	
	cvalue4 = ECU2GPS_C0_CMD9[6] & 0x01;	
	cvalue2 = ECU2GPS_C0_CMD9[3] & 0x01;
	if(cvalue4 == 0x01)
		{
			if(cvalue2 == 0x01)
			{
				Bird_ECU_sent_close();
			}	
		}
	tone_number = ((ECU2GPS_C0_CMD9[4] & 0x38)>>3)+1;
	tone_volume = ECU2GPS_C0_CMD9[4] & 0x07;
	
	cvalue5 = ECU2GPS_C0_CMD9[3] & 0x04;	
	cvalue6 = ECU2GPS_C0_CMD9[6] & 0x04;
	if(cvalue6 == 0x04)
		{
			if(cvalue5 == 0x04)
			{
				Bird_ECU_sent_playtone();
			}	
		}

#ifdef BIRD_BT_SUPPORT
	cvalue7 = ECU2GPS_C0_CMD9[3] & 0x08;	
	cvalue8 = ECU2GPS_C0_CMD9[6] & 0x08;
	if(cvalue8 == 0x08)
		{
			if(cvalue7 == 0x08)
			{
				Bird_ECU_bt_accredit();
			}	
		}
#endif	
}
BIRD_UART_A0_INFO A0_info;
//应答远程指令数据处理开始
kal_uint8 ECU_remote_cmd_reply() //第三方设备对远程指令的应答数据解析
{
	kal_uint16 i;
	kal_uint8 cvalue,cvalue1,cvalue2,cvalue3,cvalue4,cvalue5,cvalue6,cvalue7;	
	kal_uint8 cvalue_flag,cvalue1_flag,cvalue2_flag,cvalue3_flag,cvalue4_flag,cvalue5_flag,cvalue6_flag,cvalue7_flag;		
	if(ECU2GPS_A0_CMD!=0)
	{
		Bird_ECU_A0_cmd_alarm();
		memset(&A0_info,0,sizeof(BIRD_UART_A0_INFO));   
		cvalue = ECU2GPS_A0_CMD[3]&0x01;
		if(cvalue == 0x01)
		{
			A0_info.defend=1;
		}
		 
		cvalue1 = ECU2GPS_A0_CMD[3]&0x02;
		if(cvalue1 == 0x02)
		{
			A0_info.limitspeed=1;
		}

		cvalue2 = ECU2GPS_A0_CMD[3]&0x04;
		if(cvalue2 == 0x04)
		{
			A0_info.findcar=1;
		}

		cvalue3 = ECU2GPS_A0_CMD[3]&0x08;
		if(cvalue3 == 0x08)
		{
			A0_info.lockcar=1;
		}

		cvalue4 = ECU2GPS_A0_CMD[3]&0x10;
		if(cvalue4 == 0x10)
		{
			A0_info.anzuo=1;
		}

		cvalue5 = ECU2GPS_A0_CMD[3]&0x20;
		if(cvalue5 == 0x20)
		{
			A0_info.startup=1;
		}

		cvalue6 = ECU2GPS_A0_CMD[3]&0x40;
		if(cvalue6 == 0x40)
		{
			A0_info.reserved=1;
		}		

		cvalue7 = ECU2GPS_A0_CMD[3]&0x80;
		if(cvalue7 == 0x80)
		{
			A0_info.selfcheck=1;
		}	

		cvalue_flag = ECU2GPS_A0_CMD[6]&0x01;
		if(cvalue_flag == 0x01)
		{
			A0_info.defend_flag=1;
		}
		
		cvalue1_flag = ECU2GPS_A0_CMD[6]&0x02;
		if(cvalue1_flag == 0x02)
		{
			A0_info.limitspeed_flag=1;
		}

		cvalue2_flag = ECU2GPS_A0_CMD[6]&0x04;
		if(cvalue2_flag == 0x04)
		{
			A0_info.findcar_flag=1;
		}

		cvalue3_flag = ECU2GPS_A0_CMD[6]&0x08;
		if(cvalue3_flag == 0x08)
		{
			A0_info.lockcar_flag=1;
		}

		cvalue4_flag = ECU2GPS_A0_CMD[6]&0x10;
		if(cvalue4_flag == 0x10)
		{
			A0_info.anzuo_flag=1;
		}

		cvalue5_flag = ECU2GPS_A0_CMD[6]&0x20;
		if(cvalue5_flag == 0x20)
		{
			A0_info.startup_flag=1;
		}

		cvalue6_flag = ECU2GPS_A0_CMD[6]&0x40;
		if(cvalue6_flag == 0x40)
		{
			A0_info.reserved_flag=1;
		}		

		cvalue7_flag = ECU2GPS_A0_CMD[6]&0x80;
		if(cvalue7_flag == 0x80)
		{
			A0_info.selfcheck_flag=1;
		}			
	}
}
void BD_ecu_inquire_main(kal_uint16 length)
{
	kal_uint16 i;
	kal_uint8 C9_init_flag=0;	
	if(ECU2GPS_C9_CMD!=0)
	{	
		for(i=3;i<(length-2);i++)
			{
				if(ECU2GPS_C9_CMD[i] ==0xff)
				{
				C9_init_flag ++;
				}
				else
				{
				C9_init_flag = 0;
				}
			}
			if(C9_init_flag == (length-5))
			{
				if(ECU2GPS_C9_CMD[2]==0x01)
				{
					Bird_C9_CMD1_INQUIRE();
				}
				if(ECU2GPS_C9_CMD[2]==0x02)
				{
					Bird_C9_CMD2_INQUIRE();			
				}
				if(ECU2GPS_C9_CMD[2]==0x03)
				{
					Bird_C9_CMD3_INQUIRE();
				}
				if(ECU2GPS_C9_CMD[2]==0x04)
				{
					Bird_C9_CMD4_INQUIRE();
				}
				}
	}
		
}

//C0 07命令解析
kal_uint8 ECU_bt_sensor_flag=0;

void bird_ecu_cmd7_check()
{
	kal_uint16 cvalue1,cvalue2,cvalue3,cvalue4;
	kal_prompt_trace(MOD_SOC," bird_ecu_cmd7_check");
#ifdef BIRD_BT_SUPPORT
	cvalue1 = ECU2GPS_C0_CMD7[5] & 0x20;
	if(cvalue1 == 0x01)
	{
		if((ECU_bt_sensor_flag == 2)||(ECU_bt_sensor_flag == 0))
		{
		ECU_bt_sensor_flag=1;
		Bird_ECU_bt_sensor_open();
		}

	}
	else
	{
		if((ECU_bt_sensor_flag == 0)||(ECU_bt_sensor_flag == 1))
		{
		ECU_bt_sensor_flag=2;				
		Bird_ECU_bt_sensor_close();}
	}
#endif	
}
/*
*lock_car :1 锁车，0解锁search_car: 1寻车，0 不寻车 
*defence :1设防 0撤防  limit_speed_value :限速值(0 -40)
*/
void GPS2ECU_UART_wirte_C9_CMD1(BIRD_UART_C9_INFO *c9_info)
{
	kal_uint8 GPS2ECU_wirte_buf[9] = {0};
	kal_uint16 tmp_buf=0;
	kal_uint16 i;
	
	GPS2ECU_wirte_buf[0] = 0xC9;
	GPS2ECU_wirte_buf[1] = 0x05;	
	GPS2ECU_wirte_buf[2] = 0x01;
	GPS2ECU_wirte_buf[3] = 0x00; 
	if(c9_info->selfcheck)
		GPS2ECU_wirte_buf[3] |= 1<<7;
	if(c9_info->reserved)
		GPS2ECU_wirte_buf[3] |= 1<<6;
	if(c9_info->startup)
		GPS2ECU_wirte_buf[3] |= 1<<5;
	if(c9_info->anzuo)
		GPS2ECU_wirte_buf[3] |= 1<<4;
	if(c9_info->lockcar)
		GPS2ECU_wirte_buf[3] |= 1<<3;
	if(c9_info->findcar)
		GPS2ECU_wirte_buf[3] |= 1<<2;
	if(c9_info->limitspeed)
		GPS2ECU_wirte_buf[3] |= 1<<1;
	if(c9_info->defend)
		GPS2ECU_wirte_buf[3] |= 1;

	GPS2ECU_wirte_buf[4] = c9_info->limitspeed_value;
	GPS2ECU_wirte_buf[5] = 0x00;
	GPS2ECU_wirte_buf[6] = 0x00;
	if(c9_info->selfcheck_flag)
		GPS2ECU_wirte_buf[6] |= 1<<7;
	if(c9_info->reserved_flag)
		GPS2ECU_wirte_buf[6] |= 1<<6;
	if(c9_info->startup_flag)
		GPS2ECU_wirte_buf[6] |= 1<<5;
	if(c9_info->anzuo_flag)
		GPS2ECU_wirte_buf[6] |= 1<<4;;	
	if(c9_info->lockcar_flag)
		GPS2ECU_wirte_buf[6] |= 1<<3;
	if(c9_info->findcar_flag)
		GPS2ECU_wirte_buf[6] |= 1<<2;
	if(c9_info->limitspeed_flag)
		GPS2ECU_wirte_buf[6] |= 1<<1;
	if(c9_info->defend_flag)
		GPS2ECU_wirte_buf[6] |= 1;

	tmp_buf=GPS2ECU_wirte_buf[0];
	for(i=1;i<7;i++)
	{
	tmp_buf ^=GPS2ECU_wirte_buf[i];
	}
	GPS2ECU_wirte_buf[7] = tmp_buf;	
	GPS2ECU_wirte_buf[8] = 0x0d;
	ECU_UART_Write(GPS2ECU_wirte_buf,9);			
}

void GPS2ECU_UART_wirte_C9_CMD4(BIRD_UART_C9_04_INFO *c4_info)
{
	kal_uint8 GPS2ECU_wirte_buf[20] = {0};
	kal_uint16 tmp_buf=0;
	kal_uint16 i;
	
	GPS2ECU_wirte_buf[0] = 0xC9;
	GPS2ECU_wirte_buf[1] = 0x10;	
	GPS2ECU_wirte_buf[2] = 0x04;
	GPS2ECU_wirte_buf[3] = 0x00|(rj_user_info.u_imsi[0]-0x30); 
	GPS2ECU_wirte_buf[4] = ((rj_user_info.u_imsi[1]-0x30)) <<4|((rj_user_info.u_imsi[2]-0x30)) ; 
	GPS2ECU_wirte_buf[5] = ((rj_user_info.u_imsi[3]-0x30)) <<4|((rj_user_info.u_imsi[4]-0x30)) ; 
	GPS2ECU_wirte_buf[6] = ((rj_user_info.u_imsi[5]-0x30)) <<4|((rj_user_info.u_imsi[6]-0x30)) ; 
	GPS2ECU_wirte_buf[7] = ((rj_user_info.u_imsi[7]-0x30)) <<4|((rj_user_info.u_imsi[8]-0x30)) ; 
	GPS2ECU_wirte_buf[8] = ((rj_user_info.u_imsi[9]-0x30)) <<4|((rj_user_info.u_imsi[10]-0x30)) ; 
	GPS2ECU_wirte_buf[9] = ((rj_user_info.u_imsi[11]-0x30)) <<4|((rj_user_info.u_imsi[12]-0x30)) ; 
	GPS2ECU_wirte_buf[10] = ((rj_user_info.u_imsi[13]-0x30)) <<4|((rj_user_info.u_imsi[14]-0x30)) ; 	
	GPS2ECU_wirte_buf[11] = 0x00; 	
	GPS2ECU_wirte_buf[11] |=(0x01&(c4_info->vibrate_status))<<7;
#ifdef BIRD_BT_SUPPORT	
	GPS2ECU_wirte_buf[11] |=(0x01&(c4_info->btconnect_status))<<6;	
	GPS2ECU_wirte_buf[11] |=(0x01&(c4_info->btKey_induction_status))<<5;	
#endif
	GPS2ECU_wirte_buf[11] |=(0x03&(c4_info->gps_status))<<3;
	GPS2ECU_wirte_buf[11] |=(0x03&(c4_info->gsm_status))<<1;	
	if(c4_info->sim_status)
	GPS2ECU_wirte_buf[11] |= 1;	
	GPS2ECU_wirte_buf[12] = 0x00; 	
	GPS2ECU_wirte_buf[13] = 0x00; 	
	GPS2ECU_wirte_buf[14] = 0x00; 	
	GPS2ECU_wirte_buf[15] = 0x00; 	
	GPS2ECU_wirte_buf[16] = 0x00; 	
	GPS2ECU_wirte_buf[17] = 0x00; 	

	tmp_buf=GPS2ECU_wirte_buf[0];
	for(i=1;i<18;i++)
	{
	tmp_buf ^=GPS2ECU_wirte_buf[i];
	}
	GPS2ECU_wirte_buf[18] = tmp_buf;	
	GPS2ECU_wirte_buf[19] = 0x0d;
	ECU_UART_Write(GPS2ECU_wirte_buf,20);		
}

void GPS2ECU_inquire_about_battery_message()
{
	kal_uint8 GPS2ECU_wirte_buf[24] = {0};
	kal_uint16 tmp_buf=0;
	kal_uint16 i,j;
	
	GPS2ECU_wirte_buf[0] = 0xC0;
	GPS2ECU_wirte_buf[1] = 0x14;	
	GPS2ECU_wirte_buf[2] = 0x0B;
	for(j=3;i<22;i++)
	{
	GPS2ECU_wirte_buf[j] = 0xff;
	}

	tmp_buf=GPS2ECU_wirte_buf[0];
	for(i=1;i<22;i++)
	{
	tmp_buf ^=GPS2ECU_wirte_buf[i];
	}
	GPS2ECU_wirte_buf[22] = tmp_buf;	
	GPS2ECU_wirte_buf[23] = 0x0d;
	ECU_UART_Write(GPS2ECU_wirte_buf,24);	
}
#ifdef BIRD_BT_SUPPORT
extern void ECU_to_bluetooth(kal_uint8 * bufer,kal_uint8 length);
extern void ECU_data_to_tempbuf(kal_uint8 * bufer,kal_uint8 length);
extern void unwap_bluetooth_data(kal_uint8 * buffer,kal_uint8 length);
extern kal_uint8 bt_sent_enable_flag;
#endif
kal_uint8   bird_uart_buf[100] = {0};
kal_uint16  bird_uartbuf_len=0;
void BD_ecu_uart_data_check()
{
	kal_uint8 i,j;
	static kal_uint8 n = 0;
   	kal_uint16  buf_len=0;
	kal_uint8 * buf = NULL;
	static kal_uint8 check_byte =0;
	static kal_uint8 status=0;  
	static kal_uint8 count = 0;
	static kal_uint8 count1 = 0;

	buf = bird_uart_buf;
	buf_len = bird_uartbuf_len;
	if (buf!=NULL)
		{

		ECU_COM_connect_flag = 1;

		for(i=0;i<buf_len;i++)
		{
		switch(status)
			{
			case 0:
				cnt = 0;
				if(*(buf+i) == 0xC0)
					{
					 ECU2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = ECU2GPS_tmp_buf[cnt];
					 ECU_status=1; 
					 C0_index = 0;
					 status = 1;
					}
				else if(*(buf+i) == 0xA0)
					{
					 ECU2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = ECU2GPS_tmp_buf[cnt];
					 ECU_status=2; 
					 status = 1;	
					}
				else if(*(buf+i) == 0xC9)
					{
					 ECU2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = ECU2GPS_tmp_buf[cnt];
					 ECU_status=3; 
					 status = 1;	
					}
				else
					{
					status = 0;
					}
				break;	
				case 1:
					cnt ++;
					ECU2GPS_tmp_buf[cnt]=*(buf+i);
					check_byte ^= ECU2GPS_tmp_buf[cnt];
					 if(ECU_status == 1)
					 	{
					 	if(ECU2GPS_tmp_buf[cnt] == 0x05)
					 		{
							C0_index = 1;	
							status = 2;
							break;
					 		}
						else if(ECU2GPS_tmp_buf[cnt] == 0x14)
					 		{
							C0_index = 2;	
							status = 2;
							break;
					 		}
						else if(ECU2GPS_tmp_buf[cnt] == 0x0B)
					 		{
							C0_index = 3;	
							status = 2;
							break;
					 		}
						else if(ECU2GPS_tmp_buf[cnt] == 0x29)
					 		{
							C0_index = 4;	
							status = 2;
							break;
					 		}
						else if(ECU2GPS_tmp_buf[cnt] == 0x10)
					 		{
							C0_index = 5;	
							status = 2;
							break;
					 		}								
						else
							{
							status = 0;
							}
					 	}
					 if(ECU_status == 2)
					 	{
					 	if(ECU2GPS_tmp_buf[cnt] == 0x06)
					 		{
							status = 2;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}	
					 if(ECU_status == 3)
					 	{
					 	if(ECU2GPS_tmp_buf[cnt] == 0x05)
					 		{
							C9_index = 1;	
							status = 2;
							break;
					 		}
					 	else if(ECU2GPS_tmp_buf[cnt] == 0x10)
					 		{
							C9_index = 2;	
							status = 2;
							break;
					 		}	
					 	else if(ECU2GPS_tmp_buf[cnt] == 0x14)
					 		{
							C9_index = 3;	
							status = 2;
							break;
					 		}							
						else
							{
							status = 0;
							}
					 	}
				break;	
				case 2:
					cnt ++;
					ECU2GPS_tmp_buf[cnt]=*(buf+i);
					check_byte ^= ECU2GPS_tmp_buf[cnt];
					 if(ECU_status == 1)
					 	{
					 	if((ECU2GPS_tmp_buf[cnt] == 0x01)&&(C0_index == 1))
					 		{
							Command0_type = 1;	
							status = 3;
							break;
					 		}
					 	else if((ECU2GPS_tmp_buf[cnt] == 0x06)&&(C0_index == 1))
					 		{
							Command0_type = 6;	
							status = 3;
							break;
					 		}
					 	else if((ECU2GPS_tmp_buf[cnt] == 0x09)&&(C0_index == 1))
					 		{
							Command0_type = 9;	
							status = 3;
							break;
					 		}						
						else if((ECU2GPS_tmp_buf[cnt] == 0x02)&&(C0_index == 2))
					 		{
							Command0_type = 2;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x08)&&(C0_index == 2))
					 		{
							Command0_type = 8;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x0A)&&(C0_index == 2))
					 		{
							Command0_type = 10;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x0B)&&(C0_index == 2))
					 		{
							Command0_type = 11;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x0C)&&(C0_index == 2))
					 		{
							Command0_type = 12;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x0D)&&(C0_index == 2))
					 		{
							Command0_type = 13;	
							status = 3;
							break;
					 		}						
						else if((ECU2GPS_tmp_buf[cnt] == 0x05)&&(C0_index == 2))
					 		{
							Command0_type = 5;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x03)&&(C0_index == 3))
					 		{
							Command0_type = 3;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x04)&&(C0_index == 4))
					 		{
							Command0_type = 4;	
							status = 3;
							break;
					 		}
						else if((ECU2GPS_tmp_buf[cnt] == 0x07)&&(C0_index == 5))
					 		{
							Command0_type = 7;	
							status = 3;
							break;
					 		}							
						else
							{
							status = 0;
							}
					 	}
					 if(ECU_status == 2)
					 	{
					 	if(ECU2GPS_tmp_buf[cnt] == 0x01)
					 		{
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}	
					 if(ECU_status == 3)
					 	{
					 	if((ECU2GPS_tmp_buf[cnt] == 0x01)&&(C9_index == 1))
					 		{
					 		Command1_type=1;
							status = 3;
							break;
					 		}
					 	else if((ECU2GPS_tmp_buf[cnt] == 0x03)&&(C9_index == 2))
					 		{
					 		Command1_type=3;
							status = 3;
							break;
					 		}	
					 	else if((ECU2GPS_tmp_buf[cnt] == 0x04)&&(C9_index == 2))
					 		{
					 		Command1_type=4;
							status = 3;
							break;
					 		}
					 	else if((ECU2GPS_tmp_buf[cnt] == 0x02)&&(C9_index == 3))
					 		{
					 		Command1_type=2;
							status = 3;
							break;
					 		}						
						else
							{
							status = 0;
							}
					 	}
				break;					
			case 3:
				 cnt ++;
				 ECU2GPS_tmp_buf[cnt]=*(buf+i);
				if(ECU_status == 1)
					{
					if((cnt == 7)&&(C0_index == 1))     // 5+3-1
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 22)&&(C0_index == 2))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 13)&&(C0_index == 3))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 43)&&(C0_index == 4))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 18)&&(C0_index == 5))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}					
					}

				if(ECU_status == 2)
					{
					if(cnt == 8)     // 5+3-1
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							ECU_status = 0;
							}
						}
					}
				if(ECU_status == 3)
					{					
					if((cnt == 7)&&(C9_index == 1))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 18)&&(C9_index == 2))
						{				
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 22)&&(C9_index == 3))
						{
						if(check_byte == ECU2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}					
					}
				 check_byte ^= ECU2GPS_tmp_buf[cnt];
				break;	
			case 4:	
				cnt ++;
				 ECU2GPS_tmp_buf[cnt]=*(buf+i);
				 if(ECU2GPS_tmp_buf[cnt] == 0x0d)
				 	{
					if(g_n_ydislogin == 1)
						{
						if((ECU_connect_cnt == 1) ||(ECU_connect_cnt == 2))
							{
								ECU_connect_cnt = 0;
								ECU_connect_alarm();
							}
						}
				 	if(ECU2GPS_tmp_buf[0] == 0xC0)
						{
							if(Command0_type==1)
							{
							       memcpy(ECU2GPS_C0_CMD1,ECU2GPS_tmp_buf,cnt+1);		
								if(g_n_ydislogin==1)
								ECU_Anti_theft_cmd_check();
					#ifdef BIRD_BT_SUPPORT
							if(bt_sent_enable_flag ==1)
							//	ECU_to_bluetooth(ECU2GPS_C0_CMD1,cnt+1);
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD1,cnt+1);
					#endif		
							}
							else if(Command0_type==2)
							{
							       memcpy(ECU2GPS_C0_CMD2,ECU2GPS_tmp_buf,cnt+1);		
								if(g_n_ydislogin==1)
								ECU_real_time_status_cmd_check();
					#ifdef BIRD_BT_SUPPORT
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD2,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD2,cnt+1);
					#endif		
							}
							else if(Command0_type==3)
							{							
								memset(ECU2GPS_C0_CMD3,0,30);
							       memcpy(ECU2GPS_C0_CMD3,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD3,cnt+1);
							 //	ECU_to_bluetooth(ECU2GPS_C0_CMD3,cnt+1);
					#endif	
								if(g_n_ydislogin==1)
								Bird_ECU_fault_cmd_alarm();
							}						
							else if(Command0_type==4)
							{
								memset(ECU2GPS_C0_CMD4,0,60);
							       memcpy(ECU2GPS_C0_CMD4,ECU2GPS_tmp_buf,cnt+1);		
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)	
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD4,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD4,cnt+1);
					#endif	
							}
							else if(Command0_type==5)
							{
								memset(ECU2GPS_C0_CMD5,0,60);
							       memcpy(ECU2GPS_C0_CMD5,ECU2GPS_tmp_buf,cnt+1);		
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD5,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD5,cnt+1);
					#endif		
							if(g_n_ydislogin==1)					
								Bird_ECU_selfdefine_cmd_alarm();
							}
							else if(Command0_type==6)
							{
								memset(ECU2GPS_C0_CMD6,0,60);
							       memcpy(ECU2GPS_C0_CMD6,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD6,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD6,cnt+1);
					#endif	
								if(g_n_ydislogin==1)
								Bird_ECU_alarm_cmd();
							}
							
							else if(Command0_type==7)
							{
								memset(ECU2GPS_C0_CMD7,0,60);
							       memcpy(ECU2GPS_C0_CMD7,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD7,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD7,cnt+1);
							if(g_n_ydislogin==1)					
								bird_ecu_cmd7_check();
					#endif	
							}
							else if(Command0_type==8)
							{
								memset(ECU2GPS_C0_CMD8,0,60);
							       memcpy(ECU2GPS_C0_CMD8,ECU2GPS_tmp_buf,cnt+1);		
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD8,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD8,cnt+1);
					#endif	
							}						
							else if(Command0_type==9)
							{
							       memcpy(ECU2GPS_C0_CMD9,ECU2GPS_tmp_buf,cnt+1);		
							if(g_n_ydislogin==1)								   
								ECU_contronl_cmd_check();
					#ifdef BIRD_BT_SUPPORT 
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD9,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD9,cnt+1);
					#endif
							}
							else if(Command0_type==10)
							{
								memset(ECU2GPS_C0_CMD10,0,60);
							       memcpy(ECU2GPS_C0_CMD10,ECU2GPS_tmp_buf,cnt+1);		
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)		
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD10,cnt+1);
							//	ECU_to_bluetooth(ECU2GPS_C0_CMD10,cnt+1);
					#endif
								if(g_n_ydislogin==1)
								Bird_ECU_hardware_config_cmd_alarm();
							}
							else if(Command0_type==11)
							{
								memset(ECU2GPS_C0_CMD11,0,60);
							       memcpy(ECU2GPS_C0_CMD11,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)									
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD11,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD11,cnt+1);
					#endif
								if(g_n_ydislogin==1)

								Bird_ECU_battary_cmd_alarm();
							}
							else if(Command0_type==12)
							{
								memset(ECU2GPS_C0_CMD12,0,60);
							       memcpy(ECU2GPS_C0_CMD12,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)									
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD12,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD11,cnt+1);
					#endif
							}
							else if(Command0_type==13)
							{
								memset(ECU2GPS_C0_CMD13,0,60);
							       memcpy(ECU2GPS_C0_CMD13,ECU2GPS_tmp_buf,cnt+1);	
					#ifdef BIRD_BT_SUPPORT   
							if(bt_sent_enable_flag ==1)									
								ECU_data_to_tempbuf(ECU2GPS_C0_CMD13,cnt+1);
								//ECU_to_bluetooth(ECU2GPS_C0_CMD11,cnt+1);
					#endif
							}							
						}
					else if(ECU2GPS_tmp_buf[0] == 0xA0)
						{
						       memcpy(ECU2GPS_A0_CMD,ECU2GPS_tmp_buf,cnt+1);		
							if(g_n_ydislogin==1)
							ECU_remote_cmd_reply();
					#ifdef BIRD_BT_SUPPORT
						if(bt_sent_enable_flag ==1)		
							ECU_data_to_tempbuf(ECU2GPS_A0_CMD,cnt+1);
							//ECU_to_bluetooth(ECU2GPS_A0_CMD,cnt+1);
					#endif	
						}
					else if(ECU2GPS_tmp_buf[0] == 0xC9)
						{
							memset(ECU2GPS_C9_CMD,0,60);
							memcpy(ECU2GPS_C9_CMD,ECU2GPS_tmp_buf,cnt+1);	
							if(g_n_ydislogin==1)
							BD_ecu_inquire_main((cnt+1));
					#ifdef BIRD_BT_SUPPORT
						if(bt_sent_enable_flag ==1)	
							ECU_data_to_tempbuf(ECU2GPS_C9_CMD,cnt+1);
							//ECU_to_bluetooth(ECU2GPS_C9_CMD,cnt+1);
					#endif
						}
					status = 0;
				 	}			 
				else
					{
					status = 0;
					}			
				break;	
			default:
				break;
			}
		}
	}
	
}
void BD_ECU_INIT(kal_uint8 *Buffaddr, kal_uint16 Length)
{
	bird_uartbuf_len = Length;
	memcpy(bird_uart_buf,Buffaddr,bird_uartbuf_len);
	
   	 if(bird_uartbuf_len>10)
   	 {
		g_m_readuartdata = 1;
	}	
	if(g_n_ydislogin==1)
	BIRD_UART_MESSAGE_DATA();
}
#endif
#endif

