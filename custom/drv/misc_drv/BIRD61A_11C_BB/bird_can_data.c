
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
#include "bird_can_data_shanshan.h"




kal_uint8 insulation_alarm_flag = 0;
kal_uint8 dc2dc_alarm_flag = 0;
kal_uint8 alarm_flag = 0;

kal_uint8 cell_nun=0;
kal_uint16 charging_data_num=0;

kal_uint8 car_data_index = 0;
kal_uint8 car_data[car_data_num]={0}; //整车数据
kal_uint8 motor_data[motor_data_num]={0}; //驱动电机
kal_uint8 extreme_value_data[extreme_value_data_nun]={0}; //极值数据
kal_uint8 alarm_data[alarm_data_nun]={0}; //报警数据
kal_uint8 charging_device_voltage_data[charging_device_data_nun]={0};/*可充电储能装置电压数据*/
kal_uint8 charging_device_temperature_data[charging_device_data_nun]={0};/*可充电储能装置温度数据*/


/*
kal_uint8 car_data_30S[car_data_num*30]={0}; //整车数据
kal_uint8 car_data_temp_30S[car_data_num*30]={0}; //整车数据
kal_uint8 motor_data_30S[motor_data_num*30]={0}; 
kal_uint8 motor_data_temp_30S[motor_data_num*30]={0}; 
kal_uint8 extreme_value_data_30S[extreme_value_data_nun*30]={0}; 
kal_uint8 extreme_value_data_temp_30S[extreme_value_data_nun*30]={0}; 
kal_uint8 alarm_data_30S[alarm_data_nun*30]={0}; 
kal_uint8 alarm_data_temp_30S[alarm_data_nun*30]={0}; 
kal_uint8 charging_device_data_30S[charging_device_data_nun*30]={0}; 
kal_uint8 charging_device_data_temp_30S[charging_device_data_nun*30]={0}; 
*/


can_data_save_struct car_can_data;
can_data_save_struct car_can_data_30s[30];
can_data_save_struct car_can_data_temp_30s[30];

extern kal_uint8 can_rx_buf[][12];
kal_uint8 bird_set_byte_bit(kal_uint8 bytenum,kal_uint8 bitnum, kal_uint8 pos)
{
	kal_uint16 i=0;
	kal_uint8 bitparam;
	bitparam = (bytenum>>pos)&((1<<bitnum)-1);
	return bitparam;
}
#ifndef  CAN_SHANSHAN_SUPPORT
void can1_rx_id180128D0_data_check()
{
	car_data[18]=can_rx_buf[0][4+2];
	car_data[19]=can_rx_buf[0][4+3];
}
void can1_rx_id180228D0_data_check()
{
	car_data[15]=bird_set_byte_bit(can_rx_buf[1][6],2,0);
	car_data[1]=can_rx_buf[1][7];
	
}
void can1_rx_id180328D0_data_check()
{
	kal_uint32 speed=0;
	kal_uint8 alarm = 0;
	speed = can_rx_buf[2][4] *10;
	car_data[3] =speed%256;
	car_data[4] =speed/256;
	alarm_data[3] |= (bird_set_byte_bit(can_rx_buf[2][9],1,6))<<4;
	alarm_data[1] |= (bird_set_byte_bit(can_rx_buf[2][10],1,0))<<2;
	alarm_data[1] |= (bird_set_byte_bit(can_rx_buf[2][10],1,1))<<3;
	alarm_data[3] |= (bird_set_byte_bit(can_rx_buf[2][9],1,6))<<1;
	alarm_data[3] |= (bird_set_byte_bit(can_rx_buf[2][10],1,0))<<2;



	alarm_data[2] |= (bird_set_byte_bit(can_rx_buf[2][9],3,3))<<5;
	 alarm = bird_set_byte_bit(can_rx_buf[2][9],3,3);
	if(alarm> 0)
	{
		alarm_data[2] |= 1<<5;     //  驱动电机控制器温度报警

	}
	
}
void can1_rx_id180428D0_data_check()
{
	motor_data[9] =can_rx_buf[3][5];
	motor_data[10] =can_rx_buf[3][4];
	motor_data[11] =can_rx_buf[3][11];
	motor_data[12] =can_rx_buf[3][10];
}
void can1_rx_id180528D0_data_check()
{
	car_data[0]=can_rx_buf[4][6];
	car_data[2]=can_rx_buf[4][7];
	car_data[14] =can_rx_buf[4][8];
	motor_data[0]=can_rx_buf[4][9];
	motor_data[1]=can_rx_buf[4][9];              ///// ??????
	motor_data[2]=can_rx_buf[4][10];
	

}
void can1_rx_id1801D0F3_data_check()
{
	kal_uint32 batterr_vol=0;
	kal_uint32 sum_current =0;
	kal_uint8 alarm = 0;
	batterr_vol = (can_rx_buf[6][8]+can_rx_buf[6][7]*256)*10;
	car_data[9] =batterr_vol%256;
	car_data[10] =batterr_vol/256;
	sum_current = (can_rx_buf[6][6]+can_rx_buf[6][5]*256)*10;
	car_data[11] =sum_current%256;
	car_data[12] =sum_current/256;	
	car_data[13] =can_rx_buf[6][10]*0.4;	
//	alarm_data[2] |= bird_set_byte_bit(can_rx_buf[6][4],1,0)<<2;	
	alarm = bird_set_byte_bit(can_rx_buf[6][9],2,5);
	if(alarm> 0)
	{
		alarm_data[2] |= 1<<2;     //  绝缘报警
		if(alarm == 1)
		{
			if(insulation_alarm_flag == 0)
			{
				insulation_alarm_flag = 1;
				alarm_flag = 1;
				bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
			}

		}
		else
			insulation_alarm_flag = 0;
	}
	else
		insulation_alarm_flag = 0;
}
void can1_rx_id1803D0F3_data_check()
{
	car_data[16]=can_rx_buf[7][11];
	car_data[17]=0;
	extreme_value_data[2]=can_rx_buf[7][5];
	extreme_value_data[3]=can_rx_buf[7][4];
	extreme_value_data[6]=can_rx_buf[7][7];
	extreme_value_data[7]=can_rx_buf[7][6];	

}
void can1_rx_id1800D028_data_check()
{
	kal_uint32 sum_mileage_data=0;
	sum_mileage_data = (can_rx_buf[8][7]*256+can_rx_buf[8][8])*10;
	car_data[5] =sum_mileage_data%256;
	car_data[6] =sum_mileage_data/256%256;
	car_data[7] =sum_mileage_data/256/256;
	car_data[8] =0;	
}
void can1_rx_id1801D08F_data_check()
{
	kal_uint8 alarm = 0;

	alarm_data[2] |= bird_set_byte_bit(can_rx_buf[9][7],1,0)<<4;	
	alarm_data[3] |= bird_set_byte_bit(can_rx_buf[9][7],1,2);	

	alarm = bird_set_byte_bit(can_rx_buf[9][5],2,4);
	if(alarm> 0)
	{
		alarm_data[2] |= 1<<4;         // dc/dc状态报警
		if(alarm == 1)
		{
			if(dc2dc_alarm_flag == 0)
			{
				dc2dc_alarm_flag = 1;	
				alarm_flag = 1;
				bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
			}
			                      
		}
		else
			dc2dc_alarm_flag = 0;		
	}
	dc2dc_alarm_flag = 0;
}
void can1_rx_id180228F3_data_check(kal_uint8* data)
{
	charging_device_voltage_data[2]=*data;
	charging_device_voltage_data[3]=*(data+1);
	charging_device_voltage_data[4]=*(data+2);
	charging_device_voltage_data[5]=*(data+3);
}
void can1_rx_id181B28F3_data_check()
{
	alarm_data[1] =0x00;
	alarm_data[1] |= bird_set_byte_bit(can_rx_buf[11][4],1,1);
	alarm_data[1] |= bird_set_byte_bit(can_rx_buf[11][5],1,0)<<4;
	alarm_data[1] |= bird_set_byte_bit(can_rx_buf[11][4],1,6)<<5;
	alarm_data[1] |= bird_set_byte_bit(can_rx_buf[11][4],1,7)<<6;
	alarm_data[1] |= bird_set_byte_bit(can_rx_buf[11][5],1,1)<<7;
	alarm_data[2] |= bird_set_byte_bit(can_rx_buf[11][5],1,2);
	alarm_data[2] |= bird_set_byte_bit(can_rx_buf[11][4],1,0)<<2;	
	
}
void can1_rx_id181D28F3_data_check()
{
	extreme_value_data[0]=can_rx_buf[12][4];
	extreme_value_data[1]=can_rx_buf[11][5];
	extreme_value_data[4]=can_rx_buf[11][6];	
	extreme_value_data[5]=can_rx_buf[11][7];
	extreme_value_data[8]=can_rx_buf[11][8];
	extreme_value_data[9]=can_rx_buf[11][9];
	extreme_value_data[11]=can_rx_buf[11][10];
	extreme_value_data[12]=can_rx_buf[11][11];		
	
}
void can1_rx_idC00D0EF_data_check()
{
	motor_data[3] =can_rx_buf[13][9];
	motor_data[4] =can_rx_buf[13][7];
	motor_data[5] =can_rx_buf[13][6];
	motor_data[6] =can_rx_buf[13][11];
	motor_data[7] =can_rx_buf[13][10];
	motor_data[8] =can_rx_buf[13][8];

}
void can1_rx_id180028E5_data_check()
{

}
void can1_rx_id181C28F3_data_check()
{
	extreme_value_data[10]=can_rx_buf[5][8];
	extreme_value_data[13]=can_rx_buf[5][9];
}


can1_rx_charging_device_data_check(kal_uint8* data,kal_uint16 index)
{
	if(index ==0)
	{
		if(cell_nun != 0)
		{
			charging_device_voltage_data[6]=0x00;
			charging_device_voltage_data[7]=cell_nun;
			charging_device_voltage_data[10]=cell_nun;
			cell_nun = 0;
		}
		charging_device_voltage_data[8]=0x00;
		charging_device_voltage_data[9]=*(data)>>1;
		charging_device_voltage_data[11+8*index]=*(data)&0x01;
		charging_device_voltage_data[12+8*index]=*(data+1);
		cell_nun++;
	}
	else if(*data != 0)
	{
		charging_device_voltage_data[11+8*index]=*(data)&0x01;
		charging_device_voltage_data[12+8*index]=*(data+1);
		cell_nun++;
	}
	if(*(data+2)!=0)
	{
		charging_device_voltage_data[13+8*index]=*(data+2)&0x01;
		charging_device_voltage_data[14+8*index]=*(data+3);
		cell_nun++;
	}
	if(*(data+4)!=0)
	{
		charging_device_voltage_data[15+8*index]=*(data+4)&0x01;
		charging_device_voltage_data[16+8*index]=*(data+5);	
		cell_nun++;
	}
	if(*(data+6)!=0)
	{
		charging_device_voltage_data[17+8*index]=*(data+6)&0x01;
		charging_device_voltage_data[18+8*index]=*(data+7);
		cell_nun++;
	}
}
#endif

void can_rx_data_check()
{
	kal_int16 i,j;
	kal_int16 cell_device_index;
	kal_uint32 temp_id;
	kal_uint32 temp_id0,temp_id1,temp_id2,temp_id3,temp_id4,temp_id5,temp_id6,temp_id7,temp_id8,temp_id9,temp_id10,temp_id11,temp_id12,temp_id13,temp_id14,temp_id15;	

kal_prompt_trace(MOD_SOC, "can_rx_data_check,");	
#ifdef CAN_SHANSHAN_SUPPORT
	can_rx_data_check_shanshan();
#else
	i=0;
	temp_id0=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(temp_id0== 0x180128D0)
	{
		can1_rx_id180128D0_data_check();
	}
	i++;     // 1
	temp_id1=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id1 == 0x180228D0)
	{
		can1_rx_id180228D0_data_check();
	}
	i++;    // 2
	temp_id2=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id2 == 0x180328D0)
	{
		can1_rx_id180328D0_data_check();
	}
	i++;  // 3
	temp_id3=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id3 == 0x180428D0)
	{
		can1_rx_id180428D0_data_check();
	}
	i++;  // 4
	temp_id4=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(temp_id4 == 0x180528D0)
	{
		can1_rx_id180528D0_data_check();
	}
	i++; //  5
	temp_id5=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(temp_id15 == 0x181C28F3)
	{
		can1_rx_id181C28F3_data_check();
	}	
	i++; // 6
	temp_id6=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id6 == 0x1801D0F3)
	{
		can1_rx_id1801D0F3_data_check();
	}
	i++; // 7
	temp_id7=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(temp_id7 == 0x1803D0F3)
	{
		can1_rx_id1803D0F3_data_check();
	}
	i++; // 8
	temp_id8=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id8 == 0x1800D028)
	{
		can1_rx_id1800D028_data_check();
	}
	i++; // 9
	temp_id9=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id9 == 0x1801D08F)
	{
		can1_rx_id1801D08F_data_check();
	}
	i++; // 10
	
	temp_id10=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id10 == 0x180228F3)
	{
		can1_rx_id180228F3_data_check(&can_rx_buf[i][4]);
	}
	i++;  // 11
	temp_id11=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(temp_id11 == 0x181B28F3)
	{
		can1_rx_id181B28F3_data_check();
	}
	i++;  // 12
	temp_id12=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id12 == 0x181D28F3)
	{
		can1_rx_id181D28F3_data_check();
	}
	i++; // 13
	temp_id13=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id13 == 0xC00D0EF)
	{
		can1_rx_idC00D0EF_data_check();
	}
	i++; // 14
	temp_id14=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(temp_id14 == 0x180028E5)
	{
		can1_rx_id180028E5_data_check();
	}
	cell_device_index = 0;
	for(j=0;j<50;j++)
	{
		i++; // 15
		temp_id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
		if(temp_id == 0x18E628F3 - 0x00010000*cell_device_index)
		{
			can1_rx_charging_device_data_check(&can_rx_buf[i][4],cell_device_index);
		}
		cell_device_index++;
	}

	charging_data_num=11+2*cell_nun;
	charging_device_voltage_data[0]=0x01;
	charging_device_voltage_data[1]=0x01;
	memcpy(car_can_data.car,car_data,car_data_num);
	memcpy(car_can_data.motor,motor_data,motor_data_num);
	memcpy(car_can_data.extreme_value,extreme_value_data,extreme_value_data_nun);
	memcpy(car_can_data.alarm,alarm_data,alarm_data_nun);
	memcpy(car_can_data.charging_device_voltage,charging_device_voltage_data,charging_device_data_nun);	
#endif

	memcpy((kal_uint8*)&car_can_data_temp_30s[car_data_index],(kal_uint8*)&car_can_data,can_save_data_len);
	if(alarm_flag == 1)
	{
		if(car_data_index == 29)
			memcpy((kal_uint8*)&car_can_data_30s,(kal_uint8*)&car_can_data_temp_30s,can_save_data_len*30);   //(kal_uint8*)
		else
		{
		memcpy((kal_uint8*)&car_can_data_30s,(kal_uint8*)&car_can_data_temp_30s[car_data_index+1],can_save_data_len*(29-car_data_index));
		memcpy((kal_uint8*)&car_can_data_30s[29-car_data_index],(kal_uint8*)&car_can_data_temp_30s,can_save_data_len*(car_data_index+1));
		}	
		alarm_flag = 0;
	}

	car_data_index++;
	if(car_data_index >= 30)
		car_data_index = 0;
		


	
/*
	else
	{
		return;
	}
*/
}
kal_uint16 bird_charging_data_num()
{
#ifdef CAN_SHANSHAN_SUPPORT
	return charging_device_data_nun;
#else
	return charging_data_num;
#endif
}
void can_data_reset()
{
	kal_prompt_trace(MOD_SOC, "can_data_reset");	
	memset(&car_can_data.date,0x00,sizeof(car_can_data.date));
	can_data_shanshan_init();
}

void can_data_30S_reset()
{
	U8 i=0;
	
	kal_prompt_trace(MOD_SOC, "can_data_30S_reset");	
	//car_data_index=0;
	for(i=0;i<30;i++)
	{
	    memset(&car_can_data_temp_30s[i].date,0x00,sizeof(car_can_data.date));
	    memset(&car_can_data_30s[i].date,0x00,sizeof(car_can_data.date));		
	}
}
