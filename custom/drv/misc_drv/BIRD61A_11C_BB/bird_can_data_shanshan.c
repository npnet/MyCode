#ifdef CAN_SHANSHAN_SUPPORT
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
#include "can.h"
#include "bird_can_data_shanshan.h"


#define VALTAGE_SPACE  3
#define TEMPERATURE_SPACE  6

#define CAN_ID_NUM_SS   	65
#define CAN_UNIT_NUM_SS   	12
#define CAN_DATA_LENGTH_SS 8
extern kal_uint8 can_rx_temp_buf[CAN_ID_NUM_SS][CAN_UNIT_NUM_SS];
extern kal_uint8 alarm_flag;
extern can_data_save_struct car_can_data;;

extern void can_id_fill(kal_uint8 *unit, kal_uint32 id);

kal_uint8 temperature_diff = 0;
kal_uint8 battery_high_temp = 0;
kal_uint8 over_voltage = 0;
kal_uint8 low_voltage = 0;
kal_uint8 low_soc = 0;
kal_uint8 over_charge = 0;
kal_uint8 over_charge_diff = 0;
kal_uint8 need_voltage = 0;
kal_uint8 motor_control_unit_temp = 0;
kal_uint8 motor_temp = 0;

extern kal_uint8 car_data_index;
extern kal_uint8 car_data[car_data_num]; //整车数据
extern kal_uint8 motor_data[motor_data_num]; //驱动电机
extern kal_uint8 extreme_value_data[extreme_value_data_nun]; //极值数据
extern kal_uint8 alarm_data[alarm_data_nun]; //报警数据
extern kal_uint8 charging_device_voltage_data[charging_device_data_nun];/*可充电储能装置电压数据*/
extern kal_uint8 charging_device_temperature_data[charging_device_data_nun];/*可充电储能装置温度数据*/


extern kal_uint8 can_rx_buf[][12];
extern kal_uint8 bird_set_byte_bit(kal_uint8 bytenum,kal_uint8 bitnum, kal_uint8 pos);

void can1_rx_id18F1FB27_data_check()
{
	kal_uint8 value = 0;

	/*整车数据 车辆状态*/
	value = (bird_set_byte_bit(can_rx_buf[0][0+4],2,6));
	if(value == 0x03)
	{
			car_data[0] = 0x01;
	}
	else
	{
			car_data[0] = 0x02;
	}
	/*整车数据 运行模式 定值0x01*/
	car_data[2] = 0x01;

	/*整车数据 DC/DC状态*/
	value = (bird_set_byte_bit(can_rx_buf[0][0+4],1,3));
	if (value == 0x00)
		car_data[14] = 0x02;
	else if (value == 0x01)
		car_data[14] = 0x01;

	/*整车数据 加速踏板行程值*/
	car_data[18] = can_rx_buf[0][1+4];
	/*整车数据 制动踏板状态*/
	value = (bird_set_byte_bit(can_rx_buf[0][2+4],2,4));
	if (value == 0)
		car_data[19] = 0x00;
	else if (value == 1)
		car_data[19] = 0x65;
}

void can1_rx_id1818D0F3_data_check()
{
	kal_uint8 alarm = 0;
	/*整车数据 总电压*/
	car_data[9]=can_rx_buf[1][1+4];
	car_data[10]=can_rx_buf[1][0+4];
	kal_prompt_trace(MOD_SOC, "car_data[9] = %x, car_data[10] = %x", car_data[9], car_data[10]);	
	/*整车数据 总电流*/
	car_data[11]=can_rx_buf[1][3+4];
	car_data[12]=can_rx_buf[1][2+4];
	kal_prompt_trace(MOD_SOC, "car_data[11] = %x, car_data[12] = %x", car_data[11], car_data[12]);	
	/*整车数据 SOC*/
	car_data[13]=(can_rx_buf[1][4+4])*4/10;
	kal_prompt_trace(MOD_SOC, "car_data[13] = %x", car_data[13]);	

	/*通用报警标志 SOC过高报警*/
	alarm_data[1] |= (bird_set_byte_bit(can_rx_buf[1][5+4],1,2))<<7;
	/*通用报警标志 绝缘报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[1][6+4],2,1));
	if (alarm != 0)
		alarm_data[2] |= 0x08;
	else
		alarm_data[2] &= 0xf7;
	/*可充电储能装置电压数据 电压*/
	charging_device_voltage_data[2]=can_rx_buf[1][0+4];
	charging_device_voltage_data[3]=can_rx_buf[1][1+4];
	charging_device_voltage_data[4]=can_rx_buf[1][2+4];
	charging_device_voltage_data[5]=can_rx_buf[1][3+4];
}


void can1_rx_id1819D0F3_data_check()
{
	/*极值数据 电池单体电压最高值*/
	extreme_value_data[2] = can_rx_buf[2][3+4];
	extreme_value_data[3] = can_rx_buf[2][2+4];
	/*极值数据 电池单体电压最低值*/
	extreme_value_data[6] = can_rx_buf[2][1+4];
	extreme_value_data[7] = can_rx_buf[2][0+4];
	/*极值数据 最高温度值*/
	extreme_value_data[10] = can_rx_buf[2][4+4];
	/*极值数据 最低温度值*/
	extreme_value_data[13] = can_rx_buf[2][5+4];
}

void can1_rx_id181AD0F3_data_check()
{
	/*极值数据 最高电压电池子系统号*/
	extreme_value_data[0] = can_rx_buf[3][0+4];
	/*极值数据 最高电压电池单体代号*/
	extreme_value_data[1] = can_rx_buf[3][1+4];
	/*极值数据 最低电压电池子系统号*/
	extreme_value_data[4] = can_rx_buf[3][2+4];
	/*极值数据 最低电压电池单体代号*/
	extreme_value_data[5] = can_rx_buf[3][3+4];
	/*极值数据 最高温度子系统号*/
	extreme_value_data[8] = can_rx_buf[3][4+4];
	/*极值数据 最高温度探针单体代号*/
	extreme_value_data[9] = can_rx_buf[3][5+4];
	/*极值数据 最低温度子系统号*/
	extreme_value_data[11] = can_rx_buf[3][6+4];
	/*极值数据 最低温度探针子系统代号*/
	extreme_value_data[12] = can_rx_buf[3][7+4];
}


void can1_rx_id181BD0F3_data_check()
{
	kal_uint8 alarm = 0, value = 0;
	/*实时数据，充电状态*/
	value = (bird_set_byte_bit(can_rx_buf[4][1+4],2,0));
	if (value ==0)
		car_data[1] = 0x03;
	else if (value ==0x01)
		car_data[1] = 0x01;
	else if (value ==0x02)
		car_data[1] = 0xFE;		
	else if (value == 0x03)
		car_data[1] = 0x04;
	
	/*实时数据，绝缘电阻*/
	car_data[16] = can_rx_buf[4][7+4];
	car_data[17] = can_rx_buf[4][6+4];
	
	/*通用报警标志 温度差异报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][1+4],2,2));
	if (alarm == 0x00)
	{
		alarm_data[1] &= 0xFE;
		temperature_diff = 0;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x01;
		temperature_diff = 0;
	}
	else if (alarm == 0x03)
	{
		if (temperature_diff == 0)
		{
			temperature_diff = 1;
			alarm_data[1] |= 0x01;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}
	/*通用报警标志 电池高温报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][0+4],2,6));
	if (alarm == 0x00)
	{
		alarm_data[1] &= 0xFD;
		battery_high_temp = 0;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x02;
		battery_high_temp = 0;
	}
	else if (alarm == 0x03)
	{
		if (battery_high_temp == 0)
		{
			battery_high_temp = 1;
			alarm_data[1] |= 0x02;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}
	/*通用报警标志 车载储能装置类型过压报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][2+4],2,4));
	if (alarm == 0)
	{
		alarm_data[1] &= 0xFB;
		over_voltage = 0;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x04;
		over_voltage = 0;
	}
	else if (alarm == 0x03)
	{
		if (over_voltage == 0)
		{
			over_voltage = 1;
			alarm_data[1] |= 0x04;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}	

	/*通用报警标志 车载储能装置类型欠压报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][2+4],2,2));
	if (alarm == 0)
	{
		low_voltage = 0;
		alarm_data[1] &= 0xF7;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x08;
		low_voltage = 0;
	}
	else if (alarm == 0x03)
	{
		if (low_voltage == 0)
		{
			low_voltage = 1; 
			alarm_data[1] |= 0x08;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}		
		/*通用报警标志 SOC低报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][3+4],2,4));
	if (alarm == 0)
	{
		alarm_data[1] &= 0xEF;
		low_soc = 0;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x10;
		low_soc = 0;
	}
	else if (alarm == 0x03)
	{
		if (low_soc == 0)
		{
			low_soc = 1;
			alarm_data[1] |= 0x10;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}	

	/*通用报警标志 单体电池过压报警 车载储能类型过充*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][0+4],2,2));
	if (alarm == 0)
	{
		over_charge = 0;
		alarm_data[1] &= 0xDF;
		alarm_data[3] &= 0xFB;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x20;
		alarm_data[3] &= 0x04;
	}
	else if (alarm == 0x03)
	{
		if (over_charge == 0)
		{
			over_charge = 1;
			alarm_data[1] |= 0x20;
			alarm_data[3] &= 0x04;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
			}
	}	

	/*通用报警标志 单体电池欠压报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][0+4],2,0));
	if (alarm == 0)
	{
		need_voltage = 0;
		alarm_data[1] &= 0xBF;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[1] |= 0x40;
		need_voltage = 0;
	}
	else if (alarm == 0x03)
	{
		if (need_voltage == 0)
		{
			need_voltage = 1;
			alarm_data[1] |= 0x40;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}	

	/*通用报警标志 SOC跳变报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][2+4],1,1));
	if (alarm == 0)
		alarm_data[2] &= 0xFE;
	else 
		alarm_data[2] |= 0x01;
	/*通用报警标志 可充电储能系统不匹配报警*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][2+4],1,0));
	if (alarm == 0)
		alarm_data[2] &= 0xFD;
	else 
		alarm_data[2] |= 0x02;	

	/*通用报警标志 单体一致性差报警  车载储能类型过充*/
	alarm = (bird_set_byte_bit(can_rx_buf[4][1+4],2,4));
	if (alarm == 0)
	{
		alarm_data[2] &= 0xFB;
		alarm_data[3] &= 0xFB;
		over_charge_diff = 0;
	}
	else if ((alarm == 0x01) || (alarm == 0x02))
	{
		alarm_data[2] |= 0x04;
		alarm_data[3] &= 0x04;
		over_charge_diff = 0;
	}
	else if (alarm == 0x03)
	{
		if (over_charge_diff == 0)
		{
			over_charge_diff = 1;
			alarm_data[2] |= 0x04;
			alarm_data[3] &= 0x04;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}
}


void can1_rx_id1806E5F4_data_check()
{
}

void can1_rx_id0C1127EF_data_check()
{
	/*驱动电机数据 驱动电机转速*/
	motor_data[4] = can_rx_buf[5][3+4];
	motor_data[5] = can_rx_buf[5][2+4];      
	/*驱动电机数据 驱动电机转矩*/
	motor_data[6] = can_rx_buf[5][1+4];
	motor_data[7] = can_rx_buf[5][0+4];      	
	/*驱动电机数据 电机控制器输入电压*/
	motor_data[9] = can_rx_buf[5][5+4];
	motor_data[10] = can_rx_buf[5][4+4]; 	
	/*驱动电机数据 电机控制器直流母线电流*/
	motor_data[11] = can_rx_buf[5][7+4];
	motor_data[12] = can_rx_buf[5][6+4]; 		
}


void can1_rx_id0C1227EF_data_check()
{
	/*驱动电机数据 驱动电机控制器温度*/
	motor_data[3] = can_rx_buf[6][4+4];
	/*驱动电机数据 驱动电机温度*/
	motor_data[8] = can_rx_buf[6][5+4];
}


void can1_rx_id0C1327EF_data_check()
{
	kal_uint8 value = 0;
	/*驱动电机数据 驱动电机个数*/
	motor_data[0] = 0x01;
	/*驱动电机数据 驱动电机序号*/
	motor_data[1] = 0x01;
	/*实时数据 驱动电机状态*/
	value = (bird_set_byte_bit(can_rx_buf[7][0+4],2,6));
	if (value == 0)
	{
		motor_data[2] = 0x04;
	}
	else if (value == 0x01)
	{
		motor_data[2] = 0x01;
	}
	else if (value == 0x02)
	{
		motor_data[2] = 0x03;
	}		
	else if (value == 0x03)
	{
		motor_data[2] |= 0xFE;
	}	
	/*通用报警标志 驱动电机控制器温度报警*/
	value = (bird_set_byte_bit(can_rx_buf[7][1+4],2,4));
	if (value == 0)
	{
		motor_control_unit_temp = 0;
		alarm_data[2] &= 0x7F;
	}
	else if (value == 0x01)
	{
		motor_control_unit_temp = 0;
		alarm_data[2] |= 0x80;
	}
	else if (value == 0x02)
	{
		if (motor_control_unit_temp == 0)
		{
			motor_control_unit_temp = 1;
			alarm_data[2] |= 0x80;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}
	/*通用报警标志 驱动电机温度报警*/
	value = (bird_set_byte_bit(can_rx_buf[7][1+4],2,6));
	if (value == 0)
	{
		alarm_data[3] &= 0xFD;
		motor_temp = 0;
	}
	else if (value == 0x01)
	{
		alarm_data[3] |= 0x02;
		motor_temp = 0;
	}
	else if (value == 0x02)
	{
	
		if(motor_temp == 0)
		{
			motor_temp = 1;
			alarm_data[3] |= 0x02;
			alarm_flag = 1;
			bird_send_message(MSG_ID_CAN_RX_DATA_ALARM);
		}
	}	
	
}

void can1_rx_id18FA01FB_data_check()
{
}


void can1_rx_id0801EF27_data_check()
{
}


void can1_rx_id10088A9E_data_check()
{
	kal_uint8 value = 0;

	/*通用报警标志 DC-DC状态报警*/
	value = (bird_set_byte_bit(can_rx_buf[10][5+4],1,3));
	if (value == 0)
		alarm_data[2] &= 0xBF;
	else if (value == 0)
		alarm_data[2] |= 0x40;
}

void can1_rx_id10098A9E_data_check()
{
	/*整车数据 车速*/
	car_data[4] = can_rx_buf[11][0+4];
	car_data[3] = can_rx_buf[11][1+4];
}

void can1_rx_id10098B9E_data_check()
{
	kal_uint8 value = 0;
	/*整车数据 档位信息*/
	value = (bird_set_byte_bit(can_rx_buf[12][0+4],4,0));
	switch(value)
	{
		case 0x00:
			car_data[15] = 0x3F;
			break;
		case 0x01:
			car_data[15] = 0x3d;
			break;
		case 0x02:
			car_data[15] = 0x30;
			break;
		case 0x03:
			car_data[15] = 0x3e;
			break;	
		case 0x04:
			car_data[15] = 0x31;
			break;
		case 0x05:
			car_data[15] = 0x32;
			break;
		case 0x06:
			car_data[15] = 0x33;
			break;
		case 0x07:
			car_data[15] = 0x34;
			break;
		case 0x08:
			car_data[15] = 0x35;
			break;
		default:
			car_data[15] = 0x34;
			break;				
	}

}


void can1_rx_id18FA01FC_data_check()
{
	/*整车数据 累计里程*/
	car_data[5] = can_rx_buf[13][3+4];
	car_data[6] = can_rx_buf[13][2+4];
	car_data[7] = can_rx_buf[13][1+4];
	car_data[8] = can_rx_buf[13][0+4];
}

void can1_rx_id18FA01FA_data_check()
{
}


void can1_rx_id18FF50E5_data_check()
{
}


void can1_rx_id1801A7E5_data_check()
{
}

void can1_rx_id18FDFF2A_data_check()
{
}


void can1_rx_id18FDFF1E_data_check()
{
}


void can1_rx_id18FDFF13_data_check()
{
}

void can1_rx_id18FDFF1A_data_check()
{
}


void can1_rx_id18FF97D2_data_check()
{
	kal_uint8 value = 0, i = 0;

	for (i = CAN_VOLTAGE_NUM;  i<CAN_VOLTAGE_NUM+CAN_VOLTAGE_UNIT_LENGTH; i++)
	{
		value = can_rx_buf[i][0+4];
#if SHANSHAN_CAN_DEBUG  	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list i = %d value = %d", i, value);	
#endif
		if ((value >0x25) || (((value + VALTAGE_SPACE- 1) % VALTAGE_SPACE) != 0))
		{
			kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list value error! value = %d", value);	
			return;
		}

		charging_device_voltage_data[11+(value-1)*2] = can_rx_buf[i][2+4];
		charging_device_voltage_data[12+(value-1)*2] = can_rx_buf[i][1+4];
		charging_device_voltage_data[13+(value-1)*2] = can_rx_buf[i][4+4];
		charging_device_voltage_data[14+(value-1)*2] = can_rx_buf[i][3+4];
		charging_device_voltage_data[15+(value-1)*2] = can_rx_buf[i][6+4];
		charging_device_voltage_data[16+(value-1)*2] = can_rx_buf[i][5+4];	
#if SHANSHAN_CAN_DEBUG 		
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[11+(value-1)*2]);	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[12+(value-1)*2]);	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[13+(value-1)*2]);	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[14+(value-1)*2]);	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[15+(value-1)*2]);	
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF97D2_data_check voltage list  value = %x", charging_device_voltage_data[16+(value-1)*2]);	
#endif		
	}

}


void can1_rx_id18FF98D2_data_check()
{
	kal_uint8 value = 0, i = 0;

	value = can_rx_buf[34][0+4];
	
	charging_device_temperature_data[4+(value-1)] = can_rx_buf[34][1+4];
	charging_device_temperature_data[5+(value-1)] = can_rx_buf[34][2+4];
	charging_device_temperature_data[6+(value-1)] = can_rx_buf[34][3+4];
	charging_device_temperature_data[7+(value-1)] = can_rx_buf[34][4+4];
	charging_device_temperature_data[8+(value-1)] = can_rx_buf[34][5+4];
	charging_device_temperature_data[9+(value-1)] = can_rx_buf[34][6+4];	

	value = can_rx_buf[35][0+4];
	
	charging_device_temperature_data[4+(value-1)] = can_rx_buf[35][1+4];
	charging_device_temperature_data[5+(value-1)] = can_rx_buf[35][2+4];
	charging_device_temperature_data[6+(value-1)] = can_rx_buf[35][3+4];
	charging_device_temperature_data[7+(value-1)] = can_rx_buf[35][4+4];
	charging_device_temperature_data[8+(value-1)] = can_rx_buf[35][5+4];
	charging_device_temperature_data[9+(value-1)] = can_rx_buf[35][6+4];	
#if SHANSHAN_CAN_DEBUG 
	for(i = 4; i<14; i++)
	{
		kal_prompt_trace(MOD_SOC, "can1_rx_id18FF98D2_data_check temperature list value = %x", charging_device_temperature_data[i]);	
	}
#endif	
}




void can_rx_data_check_shanshan()
{
	kal_int16 i,j;
	kal_int16 cell_device_index;
	kal_uint32 temp_id;
	kal_uint32 id;	

	kal_prompt_trace(MOD_SOC, "can_rx_data_check_shanshan");	

	i=0;
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(id== 0x18F1FB27)
	{
		can1_rx_id18F1FB27_data_check();
	}
	i++;     // 1
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x1818D0F3)
	{
		can1_rx_id1818D0F3_data_check();
	}
	i++;    // 2
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x1819D0F3)
	{
		can1_rx_id1819D0F3_data_check();
	}
	i++;  // 3
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x181AD0F3)
	{
		can1_rx_id181AD0F3_data_check();
	}
	i++;  // 4
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(id == 0x181BD0F3)
	{
		can1_rx_id181BD0F3_data_check();
	}
	i++; //  5
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(id == 0x0C1127EF)
	{
		can1_rx_id0C1127EF_data_check();
	}	
	i++; // 6
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x0C1227EF)
	{
		can1_rx_id0C1227EF_data_check();
	}
	i++; // 7
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(id == 0x0C1327EF)
	{
		can1_rx_id0C1327EF_data_check();
	}
	i++; // 8
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FA01FB)
	{
		can1_rx_id18FA01FB_data_check();
	}
	i++; // 9
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x0801EF27)
	{
		can1_rx_id0801EF27_data_check();
	}
	i++; // 10
	
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x10088A9E)
	{
		can1_rx_id10088A9E_data_check(&can_rx_buf[i][4]);
	}
	i++;  // 11
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];
	if(id == 0x10098A9E)
	{
		can1_rx_id10098A9E_data_check();
	}
	i++;  // 12
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x10098B9E)
	{
		can1_rx_id10098B9E_data_check();
	}
	i++; // 13
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FA01FC)
	{
		can1_rx_id18FA01FC_data_check();
	}
	i++; // 14
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FA01FA)
	{
		can1_rx_id18FA01FA_data_check();
	}
	i++; // 15
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FF50E5)
	{
		can1_rx_id18FF50E5_data_check();
	}
	i++; // 16
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x1801A7E5)
	{
		can1_rx_id1801A7E5_data_check();
	}
	i++; // 17
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FDFF2A)
	{
		can1_rx_id18FDFF2A_data_check();
	}	

	i++; // 18
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FDFF1E)
	{
		can1_rx_id18FDFF1E_data_check();
	}
	i++; // 19
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FDFF13)
	{
		can1_rx_id18FDFF13_data_check();
	}	
	i++; // 20
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FDFF1A)
	{
		can1_rx_id18FDFF1A_data_check();
	}
	
	i++; // 21
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FF97D2)
	{
		can1_rx_id18FF97D2_data_check();
	}

	i = 34; // 34
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x18FF98D2)
	{
		can1_rx_id18FF98D2_data_check();
	}

	i = 36; // 36
	id=(can_rx_buf[i][0]<<24)|(can_rx_buf[i][1]<<16)|(can_rx_buf[i][2]<<8)|can_rx_buf[i][3];

	if(id == 0x1806E5F4)
	{
		can1_rx_id1806E5F4_data_check();
	}	
	
	cell_device_index = 0;
	memcpy(car_can_data.car,car_data,car_data_num);
	memcpy(car_can_data.motor,motor_data,motor_data_num);
	memcpy(car_can_data.extreme_value,extreme_value_data,extreme_value_data_nun);
	memcpy(car_can_data.alarm,alarm_data,alarm_data_nun);
	memcpy(car_can_data.charging_device_voltage,charging_device_voltage_data,charging_device_data_nun);		
	memcpy(car_can_data.charging_device_temperature, charging_device_temperature_data,charging_device_temperature_length);		
}

void can_data_shanshan_init(void)
{
	/*可充电储能子系统个数 可充电储能子系统号*/
	charging_device_voltage_data[0]=0x01;
	charging_device_voltage_data[1]=0x01;

	/*单体电池总数*/	
	charging_device_voltage_data[6]= 0x00;
	charging_device_voltage_data[7]= 39;
	/*本帧起始电池序号*/
	charging_device_voltage_data[8]= 0x00;
	charging_device_voltage_data[9]= 01;	
	/*本帧单体电池总数*/
	charging_device_voltage_data[10]= 39;	

	/*可充电储能子系统个数 可充电储能子系统号*/
	charging_device_temperature_data[0]=0x01;
	charging_device_temperature_data[1]=0x01;

	/*可充电储能温度探针个数*/
	charging_device_temperature_data[2]=0x00;
	charging_device_temperature_data[3]=0x0A;
}
void can_table_create_shanshan(void)
{
	kal_uint8 i = 0, j=0;
	
	kal_prompt_trace(MOD_SOC, "can_table_create_shanshan");
	for (i = 0; i<CAN_ID_NUM_SS;i++)
	{
		for(j = 0;j < CAN_UNIT_NUM_SS; j++)
		{
			can_rx_temp_buf[i][j] = 0;
		}
	}
	can_id_fill(can_rx_temp_buf[0],	0x18F1FB27);  
	can_id_fill(can_rx_temp_buf[1],	0x1818D0F3);  
	can_id_fill(can_rx_temp_buf[2],	0x1819D0F3);  
	can_id_fill(can_rx_temp_buf[3],	0x181AD0F3);  
	can_id_fill(can_rx_temp_buf[4],	0x181BD0F3);  
	can_id_fill(can_rx_temp_buf[5],	0x0C1127EF);  
	can_id_fill(can_rx_temp_buf[6],	0x0C1227EF);  
	can_id_fill(can_rx_temp_buf[7],	0x0C1327EF);  
	can_id_fill(can_rx_temp_buf[8],	0x18FA01FB);  
	can_id_fill(can_rx_temp_buf[9],	0x0801EF27);  
	can_id_fill(can_rx_temp_buf[10],	0x10088A9E);
	can_id_fill(can_rx_temp_buf[11],	0x10098A9E);
	can_id_fill(can_rx_temp_buf[12],	0x10098B9E);
	can_id_fill(can_rx_temp_buf[13],	0x18FA01FC);
	can_id_fill(can_rx_temp_buf[14],	0x18FA01FA);
	can_id_fill(can_rx_temp_buf[15],	0x18FF50E5);
	can_id_fill(can_rx_temp_buf[16],	0x1801A7E5);
	can_id_fill(can_rx_temp_buf[17],	0x18FDFF2A);
	can_id_fill(can_rx_temp_buf[18],	0x18FDFF1E);
	can_id_fill(can_rx_temp_buf[19],	0x18FDFF13);
	can_id_fill(can_rx_temp_buf[20],	0x18FDFF1A);
	
	can_id_fill(can_rx_temp_buf[21],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[22],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[23],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[24],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[25],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[26],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[27],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[28],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[29],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[30],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[31],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[32],	0x18FF97D2);
	can_id_fill(can_rx_temp_buf[33],	0x18FF97D2);
	
	can_id_fill(can_rx_temp_buf[34],	0x18FF98D2);
	can_id_fill(can_rx_temp_buf[35],	0x18FF98D2);
	can_id_fill(can_rx_temp_buf[36],	0x1806E5F4);
}

#endif