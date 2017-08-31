/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   YD business data processing module
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#include "Yd_tboxpro.h"
#include "Yd_main.h"
#include "Yd_base.h"
#include "Bird_socket.h"
#include "Rj_dw02.h"

//登入流水号
U16 g_login_count=0;

extern can_data_save_struct car_can_data;
extern can_data_save_struct car_can_data_30s[30];
extern U8 g_pos_data[9];
extern U8 g_pos_data_30S[9*30];

extern RJ_Gps_User_Info rj_user_info;
extern U16 g_n_send_buf;

//数据单元加密方式:
U8 bird_get_encryption()
{
    return ENCRYPT_NULL;
}

//可充电储能子系统数
U8 bird_get_subsystem_number()
{
    return 0;

}
//可充电储能系统编码长度
U8 bird_get_system_codelength()
{
    return 0;
}
//可充电储能系统编码
U8* bird_get_system_code()
{
    return NULL;
}

//时间采集数据
U8* bird_get_can_time()
{
    if((car_can_data.date[0]==0)||(car_can_data.date[1]==0)||(car_can_data.date[2]==0))
        return NULL;
    else
        return car_can_data.date;
}

U8* bird_get_can_time_30S(U8 index)
{
    if((car_can_data_30s[index].date[0]==0)||(car_can_data_30s[index].date[1]==0)||(car_can_data_30s[index].date[2]==0))
        return NULL;
    else
        return car_can_data_30s[index].date;
}

//整车数据
U8* bird_get_vehicle()
{
    return car_can_data.car;
}
U8 bird_get_vehicle_length()
{
    return car_data_num;
}
U8* bird_get_vehicle_30S(U8 index)
{
    return car_can_data_30s[index].car;
}

//驱动电机数据
U8* bird_get_drivemotor()
{
    return car_can_data.motor;
}
U8 bird_get_drivemotor_length()
{
    return motor_data_num;
}
U8* bird_get_drivemotor_30S(U8 index)
{
    return car_can_data_30s[index].motor;
}

//燃料电池数据
U8* bird_get_fuel()
{
    return NULL;
}
U8 bird_get_fuel_length()
{
    return 0;
}
U8* bird_get_fuel_30S(U8 index)
{
    return NULL;
}

//发动机数据
U8* bird_get_engine()
{
    return NULL;
}
U8 bird_get_engine_length()
{
    return 0;
}
U8* bird_get_engine_30S(U8 index)
{
    return NULL;
}

//车辆位置数据
U8* bird_get_vehicle_pos()
{
    return g_pos_data;
}
U8 bird_get_vehicle_pos_length()
{
    return 9;
}
U8* bird_get_vehicle_pos_30S(U8 index)
{
    return g_pos_data_30S+index*9;
}

//极值数据
U8* bird_get_extremum()
{
    return car_can_data.extreme_value;
}
U8 bird_get_extremum_length()
{
    return extreme_value_data_nun;
}
U8* bird_get_extremum_30S(U8 index)
{
    return car_can_data_30s[index].extreme_value;
}

//报警数据
U8* bird_get_alarm()
{
    return car_can_data.alarm;
}
U8 bird_get_alarm_length()
{
    return alarm_data_nun;
}
U8* bird_get_alarm_30S(U8 index)
{
    return car_can_data_30s[index].alarm;
}

//可充电储能装置电压数据
U8* bird_get_rech_vol()
{
    return car_can_data.charging_device;
}
U16 bird_get_rech_vol_length()
{
    return charging_device_data_nun;
}
U8* bird_get_rech_vol_30S(U8 index)
{
    return car_can_data_30s[index].charging_device;
}

//可充电储能装置温度数据
U8* bird_get_temp()
{
    return NULL;
}
U8 bird_get_temp_length()
{
    return 0;
}
U8* bird_get_temp_30S(U8 index)
{
    return NULL;
}

U8* TB_Soc_set_time()
{   
    U8 time[6]=0;
    applib_time_struct current_time = {0} ;
	
    applib_dt_get_rtc_time(&current_time);   
	
    time[0]=(U8)(current_time.nYear%100);
    time[1]=current_time.nMonth;
    time[2]=current_time.nDay;
    time[3]=current_time.nHour;
    time[4]=current_time.nMin;
    time[5]=current_time.nSec;
	
    return time;
}

void TB_Soc_get_cantime(applib_time_struct *dt)
{
    U8 time[6]={0};

    memcpy(time, bird_get_can_time(),6);
	
    dt->nYear=(U16)(time[0]+2000);
    dt->nMonth=time[1];
    dt->nDay=time[2];
    dt->nHour=time[3];
    dt->nMin=time[4];
    dt->nSec=time[5];

}

void TB_Soc_get_cantime_30S(applib_time_struct *dt,U8 index)
{
    U8 time[6]={0};

    memcpy(time, bird_get_can_time_30S(index),6);
	
    dt->nYear=(U16)(time[0]+2000);
    dt->nMonth=time[1];
    dt->nDay=time[2];
    dt->nHour=time[3];
    dt->nMin=time[4];
    dt->nSec=time[5];

}

//终端上报的登入请求
void TB_Soc_Send_login_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
    //23 23 01 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x01,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=30+bird_get_subsystem_number()*bird_get_system_codelength();
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	//数据采集时间
	memcpy(sendBuffer+send_len,TB_Soc_set_time(),6);	
	send_len = send_len + 6;
	//登入流水号
	sendBuffer[send_len]=(U8)(g_login_count/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(g_login_count%256);
	send_len = send_len + 1;
	//ICCID
	memcpy(sendBuffer+send_len, (U8*)rj_user_info.u_iccid,strlen(rj_user_info.u_iccid));
	send_len = send_len + strlen(rj_user_info.u_iccid);
	
	//可充电储能子系统数
	sendBuffer[send_len]=bird_get_subsystem_number();
	send_len = send_len + 1;
	
	//可充电储能系统编码长度
	sendBuffer[send_len]=bird_get_system_codelength();
	send_len = send_len + 1;
	
	//可充电储能系统编码
	if((bird_get_subsystem_number()!=0)&&(bird_get_system_codelength()!=0))
	{
	    memcpy(sendBuffer+send_len, (U8*)bird_get_system_code(),bird_get_subsystem_number()*bird_get_system_codelength());
	    send_len = send_len + bird_get_subsystem_number()*bird_get_system_codelength();
	}
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_login_ReqBuffer %d",send_len);
}

//终端登出请求
void TB_Soc_Send_logout_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
    //23 23 04 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x04,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=8;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	//数据采集时间
	memcpy(sendBuffer+send_len,TB_Soc_set_time(),6);	
	send_len = send_len + 6;
	//登入流水号
	sendBuffer[send_len]=(U8)(g_login_count/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(g_login_count%256);
	send_len = send_len + 1;
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_logout_ReqBuffer %d",send_len);
}

//终端上报校时请求
void TB_Soc_Send_timing_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
    //23 23 08 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x08,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=0;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_timing_ReqBuffer %d",send_len);
}

//终端上报心跳
void TB_Soc_Send_heart_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
    //23 23 07 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x07,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=0;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;

	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_heart_ReqBuffer%d",send_len);
}

//实时信息
U8 TB_Soc_Send_realinfo_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len)
{
    //23 23 02 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 01 02 03 04 05 06 07 08 09 00
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x02,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=6+bird_get_vehicle_length()+bird_get_drivemotor_length()+bird_get_fuel_length()+bird_get_engine_length()+
	    bird_get_vehicle_pos_length()+bird_get_extremum_length()+bird_get_alarm_length()+
	    bird_get_rech_vol_length()+bird_get_temp_length()+6;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	//数据采集时间
	if(bird_get_can_time()!=NULL)
	{
	    memcpy(sendBuffer+send_len,bird_get_can_time(),6);	
	    send_len = send_len + 6;
	}
       else
       {
           return 0;
       }
	//信息类型标志
	//信息体
	//整车数据
	if(bird_get_vehicle_length()!=0)
	{
	sendBuffer[send_len]=0x01;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_vehicle(),bird_get_vehicle_length());
	send_len = send_len + bird_get_vehicle_length();
	}

	//驱动电机数据
	if(bird_get_drivemotor_length()!=0)
	{	
	sendBuffer[send_len]=0x02;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_drivemotor(),bird_get_drivemotor_length());
	send_len = send_len + bird_get_drivemotor_length();
	}

	//燃料电池数据
	if(bird_get_fuel_length()!=0)
	{	
	sendBuffer[send_len]=0x03;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_fuel(),bird_get_fuel_length());
	send_len = send_len + bird_get_fuel_length();
	}

	//发动机数据
	if(bird_get_engine_length()!=0)
	{	
	sendBuffer[send_len]=0x04;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_engine(),bird_get_engine_length());
	send_len = send_len + bird_get_engine_length();
	}
	
	//车辆位置数据
	if(bird_get_vehicle_pos_length()!=0)
	{	
	sendBuffer[send_len]=0x05;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_vehicle_pos(),bird_get_vehicle_pos_length());
	send_len = send_len + bird_get_vehicle_pos_length();
	}
	
	//极值数据
	if(bird_get_extremum_length()!=0)
	{	
	sendBuffer[send_len]=0x06;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_extremum(),bird_get_extremum_length());
	send_len = send_len + bird_get_extremum_length();
	}
	
	//报警数据
	if(bird_get_alarm_length()!=0)
	{	
	sendBuffer[send_len]=0x07;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_alarm(),bird_get_alarm_length());
	send_len = send_len + bird_get_alarm_length();
	}
	
	//可充电储能装置电压数据
	if(bird_get_rech_vol_length()!=0)
	{	
	sendBuffer[send_len]=0x08;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_rech_vol(),bird_get_rech_vol_length());
	send_len = send_len + bird_get_rech_vol_length();
	}
	
	//可充电储能装置温度数据
	if(bird_get_temp_length()!=0)
	{	
	sendBuffer[send_len]=0x09;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_temp(),bird_get_temp_length());
	send_len = send_len + bird_get_temp_length();
	}
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_realinfo_ReqBuffer %d",send_len);
	return 1;
}

//补发信息
U8 TB_Soc_Send_resendinfo_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8 index)
{
    //23 23 02 fe 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 01 02 03 04 05 06 07 08 09 00
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    U8 commad[2]={0x03,0xfe};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	memcpy(sendBuffer+send_len,commad,2);	
	send_len = send_len + 2;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=6+bird_get_vehicle_length()+bird_get_drivemotor_length()+bird_get_fuel_length()+bird_get_engine_length()+
	    bird_get_vehicle_pos_length()+bird_get_extremum_length()+bird_get_alarm_length()+
	    bird_get_rech_vol_length()+bird_get_temp_length()+6;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	//数据采集时间
	if(bird_get_can_time_30S(index)!=NULL)
	{
	    memcpy(sendBuffer+send_len,bird_get_can_time_30S(index),6);	
	    send_len = send_len + 6;
	}
       else
       {
           return 0;
       }

	//信息类型标志
	//信息体
	//整车数据
	if(bird_get_vehicle_length()!=0)
	{
	sendBuffer[send_len]=0x01;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_vehicle_30S(index),bird_get_vehicle_length());
	send_len = send_len + bird_get_vehicle_length();
	}

	//驱动电机数据
	if(bird_get_drivemotor_length()!=0)
	{	
	sendBuffer[send_len]=0x02;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_drivemotor_30S(index),bird_get_drivemotor_length());
	send_len = send_len + bird_get_drivemotor_length();
	}

	//燃料电池数据
	if(bird_get_fuel_length()!=0)
	{	
	sendBuffer[send_len]=0x03;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_fuel_30S(index),bird_get_fuel_length());
	send_len = send_len + bird_get_fuel_length();
	}

	//发动机数据
	if(bird_get_engine_length()!=0)
	{	
	sendBuffer[send_len]=0x04;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_engine_30S(index),bird_get_engine_length());
	send_len = send_len + bird_get_engine_length();
	}
	
	//车辆位置数据
	if(bird_get_vehicle_pos_length()!=0)
	{	
	sendBuffer[send_len]=0x05;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_vehicle_pos_30S(index),bird_get_vehicle_pos_length());
	send_len = send_len + bird_get_vehicle_pos_length();
	}
	
	//极值数据
	if(bird_get_extremum_length()!=0)
	{	
	sendBuffer[send_len]=0x06;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_extremum_30S(index),bird_get_extremum_length());
	send_len = send_len + bird_get_extremum_length();
	}
	
	//报警数据
	if(bird_get_alarm_length()!=0)
	{	
	sendBuffer[send_len]=0x07;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_alarm_30S(index),bird_get_alarm_length());
	send_len = send_len + bird_get_alarm_length();
	}
	
	//可充电储能装置电压数据
	if(bird_get_rech_vol_length()!=0)
	{	
	sendBuffer[send_len]=0x08;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_rech_vol_30S(index),bird_get_rech_vol_length());
	send_len = send_len + bird_get_rech_vol_length();
	}
	
	//可充电储能装置温度数据
	if(bird_get_temp_length()!=0)
	{	
	sendBuffer[send_len]=0x09;
	send_len = send_len + 1;
	memcpy(sendBuffer+send_len, (U8 *)bird_get_temp_30S(index),bird_get_temp_length());
	send_len = send_len + bird_get_temp_length();
	}

	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_resendinfo_ReqBuffer %d",send_len);
	return 1;						  
}

//终端应答查询参数命令
void TB_Soc_Send_search_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8 count, U8* param, U8 rtn)
{
    //23 23 80 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    //U8 commad[2]={0x80,0x01};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	sendBuffer[send_len]=0x80;
	send_len = send_len + 1;
	sendBuffer[send_len]=rtn;
	send_len = send_len + 1;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=7+count;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;
	
	//数据采集时间
	memcpy(sendBuffer+send_len,TB_Soc_set_time(),6);	
	send_len = send_len + 6;	
	//参数总数
	sendBuffer[send_len]=count;
	send_len = send_len + 1;
	//参数 项列表 
	memcpy(sendBuffer+send_len, (U8*)param,count);
	send_len = send_len +count;
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_search_ReqBuffer %d",send_len);
}

//终端应答设置参数命令
void TB_Soc_Send_set_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8 count, U8* param, U8 rtn)
{
    //23 23 81 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    //U8 commad[2]={0x81,0x01};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	sendBuffer[send_len]=0x81;
	send_len = send_len + 1;
	sendBuffer[send_len]=rtn;
	send_len = send_len + 1;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=7+count;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;

	//数据采集时间
	memcpy(sendBuffer+send_len,TB_Soc_set_time(),6);	
	send_len = send_len + 6;	
	//参数总数
	sendBuffer[send_len]=count;
	send_len = send_len + 1;
	//参数 项列表 
	memcpy(sendBuffer+send_len, (U8*)param,count);
	send_len = send_len +count;
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	kal_prompt_trace(MOD_SOC,"TB_Soc_Send_set_ReqBuffer:length=%d",send_len);
}

//终端应答控制命令
void TB_Soc_Send_control_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8 count, U8* param, U8 rtn)
{
    //23 23 82 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 
    U16 send_len=0;
    U8 head[2]={0x23,0x23};
    //U8 commad[2]={0x81,0x01};
    U16 length=0; 
    U8 check_code=0; 
    U16 i=0; 
	
	//起始符
	memcpy(sendBuffer+send_len,head,2);	
	send_len = send_len + 2;

	//命令单元
	sendBuffer[send_len]=0x82;
	send_len = send_len + 1;
	sendBuffer[send_len]=rtn;
	send_len = send_len + 1;
	//唯一识别码VIN
	memcpy(sendBuffer+send_len,rj_user_info.u_id,strlen(rj_user_info.u_id));	
	send_len = send_len + strlen(rj_user_info.u_id);
	//数据单元加密方式
	sendBuffer[send_len]=bird_get_encryption();
	send_len = send_len + 1;
	//数据单元长度
	length=7+count;
	sendBuffer[send_len]=(U8)(length/256);
	send_len = send_len + 1;
	sendBuffer[send_len]=(U8)(length%256);
	send_len = send_len + 1;

	//数据采集时间
	memcpy(sendBuffer+send_len,TB_Soc_set_time(),6);	
	send_len = send_len + 6;
	//参数总数
	sendBuffer[send_len]=count;
	send_len = send_len + 1;
	//参数 项列表 
	memcpy(sendBuffer+send_len, (U8*)param,count);
	send_len = send_len +count;
	
	//校验码
	for(i=2;i<send_len;i++)
	    check_code ^=sendBuffer[i];
	
	sendBuffer[send_len]=check_code;
	send_len = send_len + 1;
	
	*sendBuffer_len = send_len;
	
    kal_prompt_trace(MOD_SOC,"TB_Soc_Send_control_ReqBuffer %d",send_len);
}

//查询--  参数 项列表
void bird_tbox_search_param_res(U8* rest_buf, U32 length)
{ 
	U8 buf[500];
	U8 param[500];
	U8 t=0;
	U8 num;
	U32 buf_pos=0;
	U32 param_pos=0;
	U16 word=0;
	U8 wordchar[3];
	U8 sgchar[2];

	kal_prompt_trace(MOD_SOC,"bird_tbox_search_param_res %d",length);
	if(length<2)
		return;
	num=rest_buf[0];
	buf_pos=1;
	memset(buf,0,sizeof(buf));
	memcpy(buf, rest_buf+buf_pos,length-buf_pos);
	memset(param,0,sizeof(param));

	for(t=0;t<num;t++)
	{
		if(buf[t]==0x01)
		{
			word=1;
			memset(wordchar,0,sizeof(wordchar));
			wordchar[0]=buf[t];
			wordchar[1]=word/256;
			wordchar[2]=word%256;
			memcpy(param+param_pos, wordchar,3);
			param_pos=param_pos+3;
		}
		else if(buf[t]==0x02)
		{
			word=bird_get_nmal_main_ival();
			memset(wordchar,0,sizeof(wordchar));
			wordchar[0]=buf[t];
			wordchar[1]=word/256;
			wordchar[2]=word%256;
			memcpy(param+param_pos, wordchar,3);
			param_pos=param_pos+3;
		}
		else if(buf[t]==0x03)
		{
			word=bird_get_alarm_main_ival();
			memset(wordchar,0,sizeof(wordchar));
			wordchar[0]=buf[t];
			wordchar[1]=word/256;
			wordchar[2]=word%256;
			memcpy(param+param_pos, wordchar,3);
			param_pos=param_pos+3;
		}
		else if(buf[t]==0x04)
		{    
			memset(sgchar,0,sizeof(sgchar));
			sgchar[0]=buf[t];
			sgchar[1]=strlen((S8 *)bird_get_server_address());
			memcpy(param+param_pos, sgchar,2);
			param_pos=param_pos+2;
		}
		else if(buf[t]==0x05)
		{
			memcpy(param+param_pos, buf+t,1);
			param_pos=param_pos+1;
			memcpy(param+param_pos, (S8 *)bird_get_server_address(),strlen((S8 *)bird_get_server_address()));
			param_pos=param_pos+strlen((S8 *)bird_get_server_address());
		}
		/*
		else if(buf[t]==0x06)
		{
		}
		else if(buf[t]==0x07)
		{
		}
		else if(buf[t]==0x08)
		{
		}
		*/
		else if(buf[t]==0x09)
		{
			memset(sgchar,0,sizeof(sgchar));
			sgchar[0]=buf[t];
			sgchar[1]=bird_get_heart_ival();
			memcpy(param+param_pos, sgchar,2);
			param_pos=param_pos+2;
		}
		/*
		else if(buf[t]==0x0A)
		{
		}
		else if(buf[t]==0x0B)
		{
		}
		*/
		else if(buf[t]==0x0C)
		{
			memset(sgchar,0,sizeof(sgchar));
			sgchar[0]=buf[t];
			sgchar[1]=bird_get_conn_interval();
			memcpy(param+param_pos, sgchar,2);
			param_pos=param_pos+2;
		}
		/*
		else if(buf[t]==0x0D)
		{
		}
		else if(buf[t]==0x0E)
		{
		}
		else if(buf[t]==0x0F)
		{

		}
		else if(buf[t]==0x10)
		{
		}
		*/
		else
		{
		    kal_prompt_trace(MOD_SOC,"bird_tbox_search_param_res param error");
		    bird_soc_send_tboxsearch(length, rest_buf, 0);
		    return;
		}

	}
	bird_soc_send_tboxsearch(num, param, 1);
}

void bird_tbox_set_param_res(U8* rest_buf, U32 length)
{
	U8 buf[500];
	U8 param[500];
	U8 t=0;
	U8 num;
	U32 buf_pos=0;
	U32 param_pos=0;
	U8 wordchar[3];
	U8 sgchar[2];
	U8 flag=1;	
	YD_TK005_COMMAND_Info tk005_info={0};
	YD_TK002_COMMAND_Info tk002_info={0};
	U8 seradd_len=0;
	
	kal_prompt_trace(MOD_SOC,"bird_tbox_set_param_res %d",length);
	if(length<2)
		return;
	num=rest_buf[0];
	buf_pos=1;
	memset(buf,0,sizeof(buf));
	memcpy(buf, rest_buf+buf_pos,length-buf_pos);
	memset(param,0,sizeof(param));

	for(t=0;t<num;t++)
	{
		/*if(buf[t]==0x01)
		{
		}
		else */if(buf[t]==0x02)
		{
			param_pos++;
			memset(wordchar,0,sizeof(wordchar));
			memcpy(wordchar, param+param_pos,2);
			tk005_info.nmal_main_ival=wordchar[0]*256+wordchar[1];
			param_pos=param_pos+2;
		}
		else if(buf[t]==0x03)
		{
			param_pos++;
			memset(sgchar,0,sizeof(sgchar));
			memcpy(sgchar, param+param_pos,1);
			tk005_info.alarm_main_ival=sgchar[0];
			param_pos=param_pos+1;
		}
		else if(buf[t]==0x04)
		{    
			param_pos++;
			memset(sgchar,0,sizeof(sgchar));
			memcpy(sgchar, param+param_pos,1);
			seradd_len=sgchar[0];
			param_pos=param_pos+1;
		}
		else if(buf[t]==0x05)
		{
			param_pos=param_pos+1;
			if(seradd_len!=0)
			{
			    memcpy(tk002_info.server_address, param+param_pos,seradd_len);
			    param_pos=param_pos+seradd_len;
			}
		}
		/*
		else if(buf[t]==0x06)
		{
		}
		else if(buf[t]==0x07)
		{
		}
		else if(buf[t]==0x08)
		{
		}
		*/
		else if(buf[t]==0x09)
		{
			param_pos++;
			memset(sgchar,0,sizeof(sgchar));
			memcpy(sgchar, param+param_pos,1);
			tk005_info.heart_ival=sgchar[0];
			param_pos=param_pos+1;
		}
		/*
		else if(buf[t]==0x0A)
		{
		}
		else if(buf[t]==0x0B)
		{
		}
		*/
		else if(buf[t]==0x0C)
		{
			param_pos++;
			memset(sgchar,0,sizeof(sgchar));
			memcpy(sgchar, param+param_pos,1);
			tk005_info.conn_interval=sgchar[0];
			param_pos=param_pos+1;
		}
		/*
		else if(buf[t]==0x0D)
		{
		}
		else if(buf[t]==0x0E)
		{
		}
		else if(buf[t]==0x0F)
		{

		}
		else if(buf[t]==0x10)
		{
		}
		*/
		else
		{
		    kal_prompt_trace(MOD_SOC,"bird_tbox_set_param_res param error");
		    flag=0;
		}

	}
	if(flag==0)
	{
		bird_soc_send_tboxset(length, rest_buf, 0);
	}
	else
	{
		if(tk005_info.nmal_main_ival!=0)
		    bird_set_nmal_main_ival(tk005_info.nmal_main_ival);
		if(tk005_info.alarm_main_ival!=0)
		    bird_set_alarm_main_ival(tk005_info.alarm_main_ival);
		if(tk005_info.heart_ival!=0)
		    bird_set_heart_ival(tk005_info.heart_ival);
		if(tk005_info.heart_ival!=0)
		    bird_set_heart_ival(tk005_info.heart_ival);
		if(tk005_info.conn_interval!=0)
		    bird_set_heart_ival(tk005_info.conn_interval);
		yd_tk005_set_record_from_NV();
		
		if(seradd_len!=0)
		{
		    bird_set_server_address(tk002_info.server_address);
		    yd_tk002_set_record_from_NV();
		}

		bird_soc_send_tboxset(length, rest_buf, 1);
	}
}

void bird_tbox_control_param_res(U8* rest_buf, U32 length)
{ 
	kal_prompt_trace(MOD_SOC,"bird_tbox_control_param_res %d",length);
	bird_soc_send_tboxset(length,rest_buf,0);
}

void bird_soc_send_tboxlogin(void)
{
    Send_Info _send;	
    U8 nflag = 0;
	
	nflag = Bird_soc_get_loginflag();
       if(nflag==1)
       {
       	return;
       }
	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_LOGIN;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_login_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_send_tboxlogout(void)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_OTHER;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_logout_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_send_tboxheart(void)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_HEART;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_heart_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}
U8 bird_soc_send_tboxrealinfo(Send_Info *sendinfo)
{
	U8 rtn=0;
	
	sendinfo->buf_len=0;
	sendinfo->ini_flag=0;
	sendinfo->send_flow=0;
	sendinfo->send_type=BIRD_SOC_SEND_GPSPOS;
	memset(sendinfo->send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	rtn=TB_Soc_Send_realinfo_ReqBuffer(sendinfo->send_buf,&sendinfo->buf_len);
	if(rtn==0)
	{
	    kal_prompt_trace(MOD_SOC,"bird_soc_send_tboxrealinfo time not get");
		return 0;
	}
	g_n_send_buf=sendinfo->buf_len;
	Bird_soc_sendbufAdd2(sendinfo);
	return rtn;
}
U8 bird_soc_send_resendinfo(Send_Info *sendinfo,U8 index)
{
	U8 rtn=0;

	sendinfo->buf_len=0;
	sendinfo->ini_flag=0;
	sendinfo->send_flow=0;
	sendinfo->send_type=BIRD_SOC_SEND_GPSPOS;
	memset(sendinfo->send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	rtn=TB_Soc_Send_resendinfo_ReqBuffer(sendinfo->send_buf,&sendinfo->buf_len,index);
	if(rtn==0)
	{
	    kal_prompt_trace(MOD_SOC,"bird_soc_send_resendinfo time not get");
		return 0;
	}
	Bird_soc_sendbufAdd2(sendinfo);
	return rtn;
}
void bird_soc_send_tboxtiming(void)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_OTHER;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_timing_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_send_tboxsearch(U8 count, U8* param, U8 rtn)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_search_ReqBuffer(_send.send_buf,&_send.buf_len,count,param,rtn);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_send_tboxset(U8 count, U8* param, U8 rtn)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_set_ReqBuffer(_send.send_buf,&_send.buf_len, count,param,rtn);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_send_tboxcontrol(U8 count, U8* param, U8 rtn)
{
    Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	TB_Soc_Send_control_ReqBuffer(_send.send_buf,&_send.buf_len,count,param,rtn);
	Bird_soc_sendbufAdd2(&_send);
}


