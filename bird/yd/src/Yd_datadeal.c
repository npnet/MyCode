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

#include "Yd_datadeal.h"
#include "Bird_socket.h"
#include "Yd_main.h"
#include "Yd_base.h"
#include "Bird_nbrcell.h"
#ifdef BIRD_ECU_MMI_SUPPORT
#include "Yd_uart_main.h"
#endif

//位置数据类型
U8 g_n_pos_kind;
//pos send 
Send_Info possend;	

extern S8 gpsinfo[100];
extern U8 g_acc_sleep_status;
extern kal_uint32 bird_get_bat_channel_voltage(void);
extern double bird_get_adc_channel_voltage();
extern kal_uint32 bird_get_charge_channel_voltage(void);
extern kal_uint32 bird_get_charge_channel_curr(void);
extern void bird_set_charge_channel_voltage(void);
extern void bird_set_charge_channel_curr(void);
#ifdef __MMI_BT_SUPPORT__
extern void srv_bt_cm_switch_off(void);
extern void srv_bt_cm_switch_on(void);
extern U8 bird_get_bt_state();
#endif
extern MMI_BOOL yd_tk001_defense_is_set_by_server();
extern float yd_get_speed();
extern void bird_set_agps_address(U8 *src);
extern U8* bird_get_agps_address(void);

#ifdef BIRD_ECU_MMI_SUPPORT
extern kal_uint8 ECU_SUMbuf[300];
extern kal_uint8 ECU_C0_CMD2_length();
extern kal_uint32 ECU_single_mileage();
extern kal_uint32 ECU_sum_mileage();
extern kal_uint16 ECU_UART_Write( kal_uint8 *buffer, kal_uint16 length);
#endif

#ifdef __MMI_BT_SUPPORT__
void bird_set_bluetooth_state(U8 nstate)
{
	if(nstate==0){
		srv_bt_cm_switch_off();
		}
	else{
		srv_bt_cm_switch_on();
	}
}

U8 bird_get_bluetooth_state()
{
	return bird_get_bt_state();
}
#endif

U32 YD_get_state()
{
	U32 nreturn = 0;
	if(yd_tk001_get_defense()==1){
		nreturn = nreturn | 0x02;
	}
	if(bird_get_normal_sleep_state()){
		nreturn = nreturn | 0x01;
	}
	
	if(Yd_get_equstatus()){
		nreturn = nreturn | 0x04;
	}
	if(!is_ac_charger_in()){
		nreturn = nreturn | 0x08;
	}
	if(yd_tk001_get_az_flag()){
		nreturn = nreturn | 0x10;
	}
	if(yd_tk001_defense_is_set_by_server()){
		nreturn = nreturn | 0x20;
	}
	if(get_Acc_onoff()){
		nreturn = nreturn | 0x40;
	}
	if(bird_get_ocin_state()){
		nreturn = nreturn | 0x80;
	}
	if(bird_get_lock_state()){
		nreturn = nreturn | 0x100;
	}
	if(bird_get_vibration_running_alarm_flag()){
		nreturn = nreturn | 0x200;
	}
	#ifdef BIRD_OC_LOGIC
	if(((bird_get_oc_exec_state()==OC_SET_SUCCESS)||(bird_get_oc_exec_state()==OC_EXECUTED))&&(bird_get_ocin_state())){
		nreturn = nreturn | 0x400;
	}
	#else
	if(bird_get_ocin_state()){
		nreturn = nreturn | 0x400;
	}
	#endif
	if(bird_get_motor_state()){
		nreturn = nreturn | 0x800;
	}
	#ifdef BIRD_ECU_MMI_SUPPORT
	if(get_uart_charging_status()){
		nreturn = nreturn | 0x1000;
	}
	if(get_uart_connect_status()){
		nreturn = nreturn | 0x2000;
	}
	if(yd_get_third_party_support()){
		nreturn = nreturn | 0x4000;
	}
	if(get_car_status()){
		nreturn = nreturn | 0x8000;
	}
	#endif
	kal_prompt_trace(MOD_SOC," YD_get_state %d",nreturn);
	return nreturn;
}

U32 YD_get_state_support()
{
	U32 nreturn = 0;

	if(1){ //设防状态
		nreturn = nreturn | 0x02;
	}
	if(1){ //休眠状态
		nreturn = nreturn | 0x01;
	}
	
	if(1){ //运动静止状态
		nreturn = nreturn | 0x04;
	}
	if(1){ //充电状态
		nreturn = nreturn | 0x08;
	}
	if(1){ //鞍座状态
		nreturn = nreturn | 0x10;
	}
	if(1){ //永久设防状态
		nreturn = nreturn | 0x20;
	}
	if(1){ //acc状态
		nreturn = nreturn | 0x40;
	}
	if(1){ //断油状态
		nreturn = nreturn | 0x80;
	}
	if(1){ //锁车状态
		nreturn = nreturn | 0x100;
	}
	if(1){ //震动报警开关状态
		nreturn = nreturn | 0x200;
	}
	if(1){ //断油执行状态
		nreturn = nreturn | 0x400;
	}
	if(1){ //马达状态
		nreturn = nreturn | 0x800;
	}
	#ifdef BIRD_ECU_MMI_SUPPORT
	if(1){ //uart 充电状态
		nreturn = nreturn | 0x1000;
	}
	if(1){ //uart 连接状态
		nreturn = nreturn | 0x2000;
	}
	if(1){ //uart 是否支持
		nreturn = nreturn | 0x4000;
	}
	if(1){ //uart 整车启动状态
		nreturn = nreturn | 0x8000;
	}
	#endif
	kal_prompt_trace(MOD_SOC," YD_get_state_support %d",nreturn);
	return nreturn;
}

U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind)
{
	U8 *alarm_times = NULL;/*  报警次数*/
	applib_time_struct *compare_time = NULL;
	U8 ret_val = 0;

	alarm_times = &(alarm_kind->alarm_times);
	compare_time = &(alarm_kind->compare_time);

	if((alarm_times != NULL)&&(compare_time != NULL))
	{
		if(alarm_kind->upload_onoff)
		{
			applib_time_struct currtent_time;
			applib_dt_get_rtc_time(&currtent_time); 
			if((applib_dt_compare_time(&currtent_time,compare_time,NULL)!= DT_TIME_LESS)||(bird_get_equmode()==1))
			{
				ret_val = 1;
			}            
		}
	}
	return ret_val;
}

void yd_set_compare_time_function(ALARM_msg_param *alarm_kind)
{
	U8 *alarm_times = NULL;
	applib_time_struct tmp_time = {0};    /*报警短信时间间隔*/
	applib_time_struct *compare_time = NULL;
	
	if((alarm_kind->call_onoff == 0) && (alarm_kind->upload_onoff == 0))
	{
		return;
	}

	alarm_times = &(alarm_kind->alarm_times);
	tmp_time.nMin = alarm_kind->interval_time;
	compare_time = &(alarm_kind->compare_time);

	if((alarm_times != NULL) && (compare_time != NULL))
	{
		applib_time_struct currtent_time;
		applib_dt_get_rtc_time(&currtent_time); 
		applib_dt_increase_time(&currtent_time,&tmp_time,compare_time);

		if(alarm_kind->upload_onoff)
		{
			(*alarm_times)++;
		}
	}
}

U32 YD_get_func_support()
{
	U32 nreturn = 0;
	
#ifdef BIRD_BT_SUPPORT
	if(1){ //ACC
		nreturn = nreturn | 0x01;
	}
	if(0){ //SOS
		nreturn = nreturn | 0x02;
	}
	
	if(0){ //鞍座
		nreturn = nreturn | 0x04;
	}
	if(0){ //OC 控制
		nreturn = nreturn | 0x08;
	}
	if(0){ //碰撞功能
		nreturn = nreturn | 0x10;
	}
	if(1){ //动静3D 传感器
		nreturn = nreturn | 0x20;
	}
	if(1){ //GPS
		nreturn = nreturn | 0x40;
	}
	if(1){ //BT
		nreturn = nreturn | 0x80;
	}
	if(1){ //ECU
		nreturn = nreturn | 0x100;
	}
	if(0){ //OBD
		nreturn = nreturn | 0x200;
	}
	if(1){ //看门狗
		nreturn = nreturn | 0x400;
	}
	if(0){ //印章
		nreturn = nreturn | 0x800;
	}
	if(0){ //车锁
		nreturn = nreturn | 0x1000;
	}
	if(1){ //震动报警
		nreturn = nreturn | 0x2000;
	}
	if(1){ //启动/关闭（电门控制）
		nreturn = nreturn | 0x4000;
	}
	if(1){ //解锁控制
		nreturn = nreturn | 0x8000;
	}
	if(1){ //锁车控制
		nreturn = nreturn | 0x10000;
	}
#elif defined(BIRD_ECU_MMI_SUPPORT)
	if(1){ //ACC
		nreturn = nreturn | 0x01;
	}
	if(0){ //SOS
		nreturn = nreturn | 0x02;
	}
	
	if(0){ //鞍座
		nreturn = nreturn | 0x04;
	}
	if(0){ //OC 控制
		nreturn = nreturn | 0x08;
	}
	if(0){ //碰撞功能
		nreturn = nreturn | 0x10;
	}
	if(1){ //动静3D 传感器
		nreturn = nreturn | 0x20;
	}
	if(1){ //GPS
		nreturn = nreturn | 0x40;
	}
	if(0){ //BT
		nreturn = nreturn | 0x80;
	}
	if(1){ //ECU
		nreturn = nreturn | 0x100;
	}
	if(0){ //OBD
		nreturn = nreturn | 0x200;
	}
	if(1){ //看门狗
		nreturn = nreturn | 0x400;
	}
	if(0){ //印章
		nreturn = nreturn | 0x800;
	}
	if(0){ //车锁
		nreturn = nreturn | 0x1000;
	}
	if(1){ //震动报警
		nreturn = nreturn | 0x2000;
	}
	if(1){ //启动/关闭（电门控制）
		nreturn = nreturn | 0x4000;
	}
	if(1){ //解锁控制
		nreturn = nreturn | 0x8000;
	}
	if(1){ //锁车控制
		nreturn = nreturn | 0x10000;
	}
#endif	
	kal_prompt_trace(MOD_SOC," YD_get_func_support %d",nreturn);
	return nreturn;
}

double bird_get_bat_channel_voltage_double()
{
	kal_uint32 ntemp = 0;
	double voltage;
	ntemp = bird_get_bat_channel_voltage();
	kal_prompt_trace(MOD_SOC," bird_get_bat_channel_voltage_double %d",ntemp);
	voltage = ntemp / 1000000.0;
	return voltage;
}
void YD_soc_set_common_head(U8* sendBuffer,U16* len){
	S8 dt[1];
	U8 send_len = 0;
	
	dt[0] = ',';
	*sendBuffer = '[';
	
	//datetime
	bird_set_test_time(sendBuffer+1); 
	send_len = 20;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len +1;
	//kind
	memcpy((S8 *)(sendBuffer+send_len), (S8 *)YD_TK001_SW_SOFTKIND, strlen((S8 *)YD_TK001_SW_SOFTKIND));
	send_len=send_len+strlen((S8 *)YD_TK001_SW_SOFTKIND);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len +1;
	//version
	memcpy((S8 *)(sendBuffer+send_len), (const void *)YD_TK001_SW_VERSION, strlen((S8 *)YD_TK001_SW_VERSION));
	send_len =send_len+ strlen((S8 *)YD_TK001_SW_VERSION);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	//id
	memcpy(sendBuffer+send_len, (S8 *)rj_user_info.u_id, 9);
	send_len = send_len + 9;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	*len = send_len;
}

void YD_Soc_Send_login_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T1,13912345678,13987654321,123456,460023615223538,355889008722099,NBBD,C1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 temp8;	
	S8 fjchar[40];		
	
	cmd[0]='T';
	cmd[1]='1';
	dt[0] = ',';
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 2);
	send_len = send_len + 2;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//SIM 
	if(bird_get_authorize_number()!=NULL)
	{
		memcpy(sendBuffer+send_len,(S8 *)bird_get_authorize_number(), strlen((S8 *)bird_get_authorize_number()));
		send_len = send_len + strlen((S8 *)bird_get_authorize_number());
	}
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//OWNER
	if(bird_get_utility_number()!=NULL)
	{
		memcpy(sendBuffer+send_len,(S8 *)bird_get_utility_number(), strlen((S8 *)bird_get_utility_number()));
		send_len = send_len + strlen((S8 *)bird_get_utility_number());
	}	
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//PASS
	memcpy(sendBuffer+send_len, (S8 *)bird_get_utility_pw(), strlen((S8 *)bird_get_utility_pw()));
	send_len = send_len + strlen((S8 *)bird_get_utility_pw());		
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//imsi
	memcpy(sendBuffer+send_len, rj_user_info.u_imsi, strlen((S8 *)rj_user_info.u_imsi));
	send_len = send_len + strlen((S8 *)rj_user_info.u_imsi);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//IMEI 
	memcpy(sendBuffer+send_len, (S8 *)rj_user_info.u_imei, 15);
	send_len = send_len + 15;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	memcpy(sendBuffer+send_len, (S8 *)YD_TK001_SW_CJ, strlen((S8 *)YD_TK001_SW_CJ));
	send_len = send_len + strlen(YD_TK001_SW_CJ);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	memcpy(sendBuffer+send_len, (S8 *)yd_get_model(), strlen((S8 *)yd_get_model()));
	send_len = send_len + strlen((S8 *)yd_get_model());
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	#ifdef BIRD_BT_SUPPORT
	if(bird_get_bt_mac()!=NULL)
	{
	    memcpy(sendBuffer+send_len, (S8 *)bird_get_bt_mac(), strlen((S8 *)bird_get_bt_mac()));
	    send_len = send_len + strlen((S8 *)bird_get_bt_mac());
	    memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	    send_len=send_len+1;	
	}
	#else	//没有mac留空
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	#endif
	
	if(YD_get_func_support()!=0)
	{
	    memset(fjchar,0,sizeof(fjchar));
	    sprintf(fjchar,"%d",YD_get_func_support());
	    memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	    send_len=send_len+strlen((S8 *)fjchar);
	}
	
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_heart_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T0,669]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[2];
	S8 dh[2];	
	S8 fjchar[4];
	U16 nfj;
	S8 temp8;
	
	cmd[0]='T';
	cmd[1]='0';
	dt[0] = ',';
	dh[0] = '.';
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 2);
	send_len = send_len + 2;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//gpsnum  gsm  batnum
	temp8 = bird_get_saltllite_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",temp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	temp8 = bird_get_gsm_strength();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",temp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	temp8 = bird_get_bat_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",temp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_lc_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,U8 type)
{
	//[2011-12-15 10:00:00,1,BD01,030600001,T16,2000,1,10,2000]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 doublechar[40];
	S32 nlc=0;
	
	cmd[0]='T';
	cmd[1]='1';
	cmd[2]='6';
	dt[0] = ',';

	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//lc
	nlc = bird_get_lc_tj();
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",nlc);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//type
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",type);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);

#ifdef BIRD_ECU_MMI_SUPPORT
	if(type==2)
	{
    	    memcpy(sendBuffer+send_len, (S8 *)dt, 1);
    	    send_len=send_len+1;	
    	    memset(doublechar,0,sizeof(doublechar));
    	    sprintf(doublechar,"%d",ECU_single_mileage());
    	    memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
    	    send_len=send_len+strlen((S8 *)doublechar);    
			
    	    memcpy(sendBuffer+send_len, (S8 *)dt, 1);
    	    send_len=send_len+1;	
    	    memset(doublechar,0,sizeof(doublechar));
    	    sprintf(doublechar,"%d",ECU_sum_mileage());
    	    memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
    	    send_len=send_len+strlen((S8 *)doublechar);    
	}
#endif

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_pos_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T3,1,E,113.252432,N,22.564152, 50.6,270.5:99.2,1,460:00:10101:03633,669,48.02,1483073074186,40]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[2];
	S8 mh[2];
	S8 dh[2];
	S8 hg[2];
	S8 fjchar[8];
	U16 nfj;
	S8 ntemp8;
	S8 doublechar[40];
	
	cmd[0]='T';
	cmd[1]='3';
	dt[0] = ',';
	mh[0] = ':';
	dh[0] = '.';
	hg[0] = '-';
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 2);
	send_len = send_len + 2;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数1：0/1/2； 0表示定位失败，1表示定位成功，2表示经纬度信息非实时，而是最近保存的经纬度信息
	if(RJ_GPS_is_working()==1){
		if(gps_position.ishavedate == 1){
				ntemp8=1;
		}
		else if (gps_position.ishavedate == 2){
			ntemp8=2;
		}
		else{
			ntemp8=0;
		}
	}
	else if(gps_position.ishavedate == 1 ){
		ntemp8=2;
	}
	else if(gps_position.ishavedate == 2 ){
		ntemp8=2;
	}
	else{
		ntemp8=0;	
	}
	g_n_pos_kind = ntemp8;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
#if BIRD_GPS_DEBUG	
	Bird_Log_print(MOD_SOC, "sl_num=%d,snr=%d,ntemp8=%d", rj_position.satellite_num, bird_get_saltllite_snr_isvalid(),ntemp8);
#endif
	if(ntemp8==0){
	 	//参数2：E/W；E表示东经，W表示西经
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
		if(Yd_get_equstatus()==0){
				sprintf(doublechar,"%d",0);
		}
		else{
				sprintf(doublechar,"%f",yd_get_speed());
		}
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	else{
	 	//参数2：E/W；E表示东经，W表示西经
	 	memset(fjchar,0,sizeof(fjchar));

		 	if(gps_position.east_west==69)
		 	{
		 		memcpy(fjchar, "E", 1);
		 	}
		 	else
		 	{
		 		memcpy(fjchar, "W", 1);
		 	}	
		 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
		 	send_len=send_len+1;
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;	

			//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
			memset(doublechar,0,sizeof(doublechar));
			sprintf(doublechar,"%f",gps_position.lg);
#if BIRD_GPS_DEBUG			
			Bird_Log_print(MOD_SOC, "lg=%s", doublechar);
#endif
			memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
			send_len=send_len+strlen((S8 *)doublechar);
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;	

			//参数4：N/S；N表示北纬，S表示南纬
			memset(fjchar,0,sizeof(fjchar));
		 	if(gps_position.north_south==78)
		 	{
		 		memcpy(fjchar, "N", 1);
		 	}
		 	else
		 	{
		 		memcpy(fjchar, "S", 1);
		 	}		
		 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
		 	send_len=send_len+1;
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;	

			//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
			memset(doublechar,0,sizeof(doublechar));
			sprintf(doublechar,"%f",gps_position.la);
#if BIRD_GPS_DEBUG			
			Bird_Log_print(MOD_SOC, "la=%s", doublechar);
#endif
			memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
			send_len=send_len+strlen((S8 *)doublechar);
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;	

			//参数6：速度，单位公里/小时，如50.6
			memset(doublechar,0,sizeof(doublechar));
			if(Yd_get_equstatus()==0){
				sprintf(doublechar,"%d",0);
			}
			else{
				sprintf(doublechar,"%f",yd_get_speed());
			}		
			memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
			send_len=send_len+strlen((S8 *)doublechar);
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;	

			//参数7：方位角，取值范围：0-359.9，单位度，如270.5
			memset(doublechar,0,sizeof(doublechar));
			sprintf(doublechar,"%f",gps_position.di);
			memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
			send_len=send_len+strlen((S8 *)doublechar);
			memcpy(sendBuffer+send_len, (S8 *)mh, 1);
			send_len=send_len+1;				
			memset(doublechar,0,sizeof(doublechar));
			sprintf(doublechar,"%f",gps_position.al);
			memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
			send_len=send_len+strlen((S8 *)doublechar);
			memcpy(sendBuffer+send_len, (S8 *)dt, 1);
			send_len=send_len+1;					
	}

	//参数8：状态
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)hg, 1);
	send_len=send_len+1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state_support());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	
	//参数9：终端cell-id信息，格式为"MCC：MNC：LAC：CellID"即"区码：中国移动标识：位置区域：小区编号"，均用10进制数表示
	//mcc:
	nfj = rj_agps_send_data.mcc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//mnc:
	nfj = rj_agps_send_data.mnc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//lac:
	nfj = rj_agps_send_data.lac;
	memset(fjchar,0,sizeof(fjchar));	
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//cell:
	nfj = rj_agps_send_data.cell_id;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);	
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		

	//参数10：三位数字，
	//gpsnum  gsm  batnum
	ntemp8 = bird_get_saltllite_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_gsm_strength();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_bat_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数11：外部电压
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%f",bird_get_adc_channel_voltage());
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数12：位置数据唯一序号采用当前时间戳（没有对应多媒体数据时，此参数为空）
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",0);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数13:外接设备电量百分比例:40 如无效或没有此位请写0
	#ifdef BIRD_ECU_MMI_SUPPORT
	ntemp8 = get_uart_battery_percent_ext();
	#else
	ntemp8 = 0;
	#endif	 
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_state_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,U8 nstate)
{
	//[2011-12-15 10:00:00, 3,BD01,030600001,T18,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	
	memset(cmd,0,sizeof(cmd));
	cmd[0]='T';
	cmd[1]='1';
	cmd[2]='8';
	dt[0] = ',';
	
	YD_soc_set_common_head(sendBuffer,&send_len);
    
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//state
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nstate);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_alarm_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,TK001_ALARM_ENUM alarm_category)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T3,1,E,113.252432,N,22.564152, 50.6,270.5:99.2,1,460:00:10101:03633,669,48.02,1483073074186,40]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[2];
	S8 mh[2];
	S8 dh[2];	
	S8 hg[2];
	S8 fjchar[10];
	U16 nfj;
	S8 ntemp8;
	S8 nstate;
	S8 doublechar[40];
	S32 vol;
	
	dt[0] = ',';
	mh[0] = ':';
	dh[0] = '.';
	hg[0] = '-';
	
	memset(cmd,0,sizeof(cmd));
	if(alarm_category==TK001_ALARM_ENUM_vibration)
	{
		memcpy(cmd,"T21",3);
	}
	else if(alarm_category==TK001_ALARM_ENUM_power_off)
	{
		memcpy(cmd,"T4",2);
	}
	else if(alarm_category==TK001_ALARM_ENUM_low_power)
	{
		memcpy(cmd,"T7",2);
       	YD_soc_set_common_head(sendBuffer,&send_len);
		//cmd
		memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
		send_len = send_len + strlen((S8 *)cmd);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//bat vol
		memset(fjchar,0,sizeof(fjchar));
		vol = bird_get_bat_channel_voltage();
		sprintf(fjchar,"%d",vol);
		memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
		send_len = send_len + strlen((S8 *)fjchar);		
		*(sendBuffer+send_len)=']';	
    		send_len = send_len+1;
    		*sendBuffer_len = send_len;
		return;		
	}
	else if(alarm_category==TK001_ALARM_ENUM_moved)
	{
		memcpy(cmd,"T8",2);
	}
	else if(alarm_category==TK001_ALARM_ENUM_speed)
	{
		memcpy(cmd,"T17",3);
	}	
	else if(alarm_category==TK001_ALARM_ENUM_sos)
	{
		memcpy(cmd,"T32",3);
	}
	else if(alarm_category==TK001_ALARM_ENUM_az)
	{
		memcpy(cmd,"T33",3);
	}
	else if(alarm_category==TK001_ALARM_ENUM_collision)
	{
		memcpy(cmd,"T35",3);
	}
	else if(alarm_category==TK001_ALARM_ENUM_static_rollover)
	{
		memcpy(cmd,"T51",3);
	}	
	else if(alarm_category==TK001_ALARM_ENUM_run_rollover)
	{
		memcpy(cmd,"T50",3);
	}	

 	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数1：0/1/2； 0表示定位失败，1表示定位成功，2表示经纬度信息非实时，而是最近保存的经纬度信息
	if(alarm_category==TK001_ALARM_ENUM_speed)
	{
	nfj=alarm_position.ishavedate;
	}
	else
	{
	if(RJ_GPS_is_working()==1){
		if(gps_position.ishavedate == 1){
				nfj=1;
		}
		else if (gps_position.ishavedate == 2){
			nfj=2;
		}
		else{
			nfj=0;
		}
	}
	else if(gps_position.ishavedate == 1 ){
		nfj=2;
	}
	else if(gps_position.ishavedate == 2 ){
		nfj=2;
	}	
	else{
		nfj=0;
	}
	}
	g_n_pos_kind = nfj;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	if(nfj==0){
	 	//参数2：E/W；E表示东经，W表示西经
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
		if(Yd_get_equstatus()==0){
				sprintf(doublechar,"%d",0);
		}
		else{
				sprintf(doublechar,"%f",yd_get_speed());
		}
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	else{
	if(alarm_category==TK001_ALARM_ENUM_speed)
	{
	 	//参数2：E/W；E表示东经，W表示西经
	 	memset(fjchar,0,sizeof(fjchar));
	 	if(alarm_position.east_west==69)
	 	{
	 		memcpy(fjchar, "E", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "W", 1);
	 	}			
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",alarm_position.lg);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memset(fjchar,0,sizeof(fjchar));
	 	if(alarm_position.north_south==78)
	 	{
	 		memcpy(fjchar, "N", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "S", 1);
	 	}
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",alarm_position.la);
#if BIRD_GPS_DEBUG		
		//Bird_Log_print(MOD_SOC, "la=%s", doublechar);
#endif
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
			sprintf(doublechar,"%f",alarm_position.sd);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",alarm_position.di);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)mh, 1);
		send_len=send_len+1;				
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",alarm_position.al);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	else
	{
	 	//参数2：E/W；E表示东经，W表示西经
	 	memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.east_west==69)
	 	{
	 		memcpy(fjchar, "E", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "W", 1);
	 	}			
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.lg);
#if BIRD_GPS_DEBUG		
		//Bird_Log_print(MOD_SOC, "lg=%s", doublechar);
#endif
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.north_south==78)
	 	{
	 		memcpy(fjchar, "N", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "S", 1);
	 	}
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.la);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
		if(Yd_get_equstatus()==0){
			sprintf(doublechar,"%d",0);
		}
		else{
			sprintf(doublechar,"%f",yd_get_speed());
		}	
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.di);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)mh, 1);
		send_len=send_len+1;				
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.al);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	}

	//参数8：状态
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)hg, 1);
	send_len=send_len+1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state_support());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	 
	//参数9：终端cell-id信息，格式为"MCC：MNC：LAC：CellID"即"区码：中国移动标识：位置区域：小区编号"，均用10进制数表示
	//mcc:
	nfj = rj_agps_send_data.mcc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//mnc:
	nfj = rj_agps_send_data.mnc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//lac:
	nfj = rj_agps_send_data.lac;
	memset(fjchar,0,sizeof(fjchar));	
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//cell:
	nfj = rj_agps_send_data.cell_id;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		

	//参数10：三位数字，
	//gpsnum  gsm  batnum
	ntemp8 = bird_get_saltllite_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_gsm_strength();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_bat_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	
	//参数11：外部电压
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%f",bird_get_adc_channel_voltage());
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数12：位置数据唯一序号采用当前时间戳（没有对应多媒体数据时，此参数为空）
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",0);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	
	//参数13:外接设备电量百分比例:40 如无效或没有此位请写0
	#ifdef BIRD_ECU_MMI_SUPPORT
	ntemp8 = get_uart_battery_percent_ext();
	#else
	ntemp8 = 0;
	#endif	 
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_dw_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T3,1,E,113.252432,N,22.564152, 50.6,270.5:99.2,1,460:00:10101:03633,669,48.02,1483073074186,40]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[2];
	S8 mh[2];
	S8 dh[2];
	S8 hg[2];
	S8 fjchar[8];
	U16 nfj;
	S8 ntemp8;
	S8 nstate;
	S8 doublechar[40];
	
	dt[0] = ',';
	mh[0] = ':';
	dh[0] = '.';	
	hg[0] = '-';	
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T10",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数1：0/1/2； 0表示定位失败，1表示定位成功，2表示经纬度信息非实时，而是最近保存的经纬度信息
	if(RJ_GPS_is_working()==1){
		if(gps_position.ishavedate == 1){
				nfj=1;
		}
		else if (gps_position.ishavedate == 2){
			nfj=2;
		}
		else{
			nfj=0;
		}
	}
	else if(gps_position.ishavedate == 1 ){
		nfj=2;
	}
	else if(gps_position.ishavedate == 2 ){
		nfj=2;
	}	
	else{
		nfj=0;
	}
	g_n_pos_kind = nfj;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	if(nfj==0){
	 	//参数2：E/W；E表示东经，W表示西经
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	else{
	 	//参数2：E/W；E表示东经，W表示西经
	 	memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.east_west==69)
	 	{
	 		memcpy(fjchar, "E", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "W", 1);
	 	}			
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.lg);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.north_south==78)
	 	{
	 		memcpy(fjchar, "N", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "S", 1);
	 	}
	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.la);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
		if(Yd_get_equstatus()==0){
			sprintf(doublechar,"%d",0);
		}
		else{
			sprintf(doublechar,"%f",gps_position.sd);
		}		
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.di);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)mh, 1);
		send_len=send_len+1;				
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.al);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}

	//参数8：状态
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)hg, 1);
	send_len=send_len+1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",YD_get_state_support());
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	 
	//参数9：终端cell-id信息，格式为"MCC：MNC：LAC：CellID"即"区码：中国移动标识：位置区域：小区编号"，均用10进制数表示
	//mcc:
	nfj = rj_agps_send_data.mcc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//mnc:
	nfj = rj_agps_send_data.mnc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//lac:
	nfj = rj_agps_send_data.lac;
	memset(fjchar,0,sizeof(fjchar));	
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//cell:
	nfj = rj_agps_send_data.cell_id;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		

	//参数10：三位数字，
	//gpsnum  gsm  batnum
	ntemp8 = bird_get_saltllite_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_gsm_strength();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	ntemp8 = bird_get_bat_num();
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数11：外部电压
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%f",bird_get_adc_channel_voltage());
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数12：位置数据唯一序号采用当前时间戳（没有对应多媒体数据时，此参数为空）
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",0);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数13:外接设备电量百分比例:40 如无效或没有此位请写0
	#ifdef BIRD_ECU_MMI_SUPPORT
	ntemp8 = get_uart_battery_percent_ext();
	#else
	ntemp8 = 0;
	#endif	 
	memset(doublechar,0,sizeof(doublechar));
	sprintf(doublechar,"%d",ntemp8);
	memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
	send_len=send_len+strlen((S8 *)doublechar);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_jy_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T99,1483073074186,1483073074186]
	U16 send_len;
	S8 cmd[4];
	S8 cn1[4];
	S8 cn2[4];
	S8 cn3[4];
	S8 cn4[4];
	S8 cn5[4];
	S8 cn6[4];
	S8 cn7[4];
	S8 cn8[4];
	S8 cn9[4];
	S8 n1,n2,n3,n4,n5,n6,n7,n8,n9; 
	S8 fjchar[8];
	S8 dt[1];
	U16 njy1,njy2;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T99",3);
	
  	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数1：
	memset(cn1,0,sizeof(cn1));
	memset(cn2,0,sizeof(cn2));
	memset(cn3,0,sizeof(cn3));
	memset(cn4,0,sizeof(cn4));
	memset(cn5,0,sizeof(cn5));
	memset(cn6,0,sizeof(cn6));
	memset(cn7,0,sizeof(cn7));
	memset(cn8,0,sizeof(cn8));
	memset(cn9,0,sizeof(cn9));
	memcpy(cn1, (S8 *)rj_user_info.u_id, 1);
	memcpy(cn2, (S8 *)(rj_user_info.u_id+1), 1);
	memcpy(cn3, (S8 *)(rj_user_info.u_id+2), 1);
	memcpy(cn4, (S8 *)(rj_user_info.u_id+3), 1);
	memcpy(cn5, (S8 *)(rj_user_info.u_id+4), 1);
	memcpy(cn6, (S8 *)(rj_user_info.u_id+5), 1);
	memcpy(cn7, (S8 *)(rj_user_info.u_id+6), 1);
	memcpy(cn8, (S8 *)(rj_user_info.u_id+7), 1);
	memcpy(cn9, (S8 *)(rj_user_info.u_id+8), 1);
	n1 = atoi(cn1);
	n2 = atoi(cn2);
	n3 = atoi(cn3);
	n4 = atoi(cn4);
	n5 = atoi(cn5);
	n6 = atoi(cn6);
	n7 = atoi(cn7);
	n8 = atoi(cn8);
	n9 = atoi(cn9);
	njy1 = (n1+n2+n3+n4+n5+n6+n7+n8+n9)*(n7+n8+n9)+(n8*n9);
	njy2 = (n1+n2+n3+n4+n5+n6+n7+n8+n9)*(n7+n8+n9)+(n2+n4+n6+n8)*(n1+n3+n8+n9);
	kal_prompt_trace(MOD_SOC," YD_Soc_Send_jy_ReqBuffer %d,%d ",njy1,njy2);
	
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",njy1);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",njy2);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	 
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_zc_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T30,13912345678,13987654321,123456,460023615223538,355889008722099,NBBD]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];	
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T30",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);	
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//SIM 
	if(bird_get_authorize_number()!=NULL)
	{
		memcpy(sendBuffer+send_len,(S8 *)bird_get_authorize_number(), strlen((S8 *)bird_get_authorize_number()));
		send_len = send_len + strlen((S8 *)bird_get_authorize_number());
	}
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//OWNER
	if(bird_get_utility_number()!=NULL)
	{
		memcpy(sendBuffer+send_len,(S8 *)bird_get_utility_number(), strlen((S8 *)bird_get_utility_number()));
		send_len = send_len + strlen((S8 *)bird_get_utility_number());
	}	
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//PASS
	memcpy(sendBuffer+send_len, (S8 *)bird_get_utility_pw(), strlen((S8 *)bird_get_utility_pw()));
	send_len = send_len + strlen((S8 *)bird_get_utility_pw());	
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len = send_len + 1;
	//imsi
	memcpy(sendBuffer+send_len, rj_user_info.u_imsi, strlen((S8 *)rj_user_info.u_imsi));
	send_len = send_len + strlen((S8 *)rj_user_info.u_imsi);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//IMEI 
	memcpy(sendBuffer+send_len, (S8 *)rj_user_info.u_imei, 15);
	send_len = send_len + 15;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//CJ
	memcpy(sendBuffer+send_len, (S8 *)YD_TK001_SW_CJ, strlen((S8 *)YD_TK001_SW_CJ));
	send_len = send_len + strlen(YD_TK001_SW_CJ);
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_getdw_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-1510:00:00,1,BD01,030600001,T31,1,E,113.252432,N,22.564152, 50.6,270.5,460:00:10101:03633]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[2];
	S8 dh[2];	
	S8 mh[2];
	S8 fjchar[8];
	U16 nfj;
	S8 ntemp8;
	S8 nstate;
	S8 doublechar[40];
	
	dt[0] = ',';
	mh[0] = ':';
	dh[0] = '.';	
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T31",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	

	//参数1：0/1/2； 0表示定位失败，1表示定位成功，2表示经纬度信息非实时，而是最近保存的经纬度信息
	if(RJ_GPS_is_working()==1){
		if(gps_position.ishavedate == 1){
				nfj=1;
		}
		else if (gps_position.ishavedate == 2){
			nfj=2;
		}
		else{
			nfj=0;
		}
	}
	else if(gps_position.ishavedate == 1 ){
		nfj=2;
	}
	else if(gps_position.ishavedate == 2 ){
		nfj=2;
	}		
	else{
		nfj=0;
	}
	g_n_pos_kind = nfj;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	if(nfj==0){
	 	//参数2：E/W；E表示东经，W表示西经
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	else{
	 	//参数2：E/W；E表示东经，W表示西经
	 	memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.east_west==69)
	 	{
	 		memcpy(fjchar, "E", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "W", 1);
	 	}			

	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数3：经度值，取值范围：0 -180.000000，单位度，如113.252432
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.lg);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数4：N/S；N表示北纬，S表示南纬
		memset(fjchar,0,sizeof(fjchar));
	 	if(gps_position.north_south==78)
	 	{
	 		memcpy(fjchar, "N", 1);
	 	}
	 	else
	 	{
	 		memcpy(fjchar, "S", 1);
	 	}

	 	memcpy(sendBuffer+send_len, (S8 *)fjchar, 1);
	 	send_len=send_len+1;
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数5：纬度值，取值范围：0 -180.000000，单位度，如22.564152
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.la);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数6：速度，单位公里/小时，如50.6
		memset(doublechar,0,sizeof(doublechar));
		if(Yd_get_equstatus()==0){
			sprintf(doublechar,"%d",0);
		}
		else{
			sprintf(doublechar,"%f",gps_position.sd);
		}			
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;	

		//参数7：方位角，取值范围：0-359.9，单位度，如270.5
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.di);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		send_len=send_len+strlen((S8 *)doublechar);
		memcpy(sendBuffer+send_len, (S8 *)mh, 1);
		send_len=send_len+1;				
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f",gps_position.al);
		memcpy(sendBuffer+send_len, (S8 *)doublechar, strlen((S8 *)doublechar));
		memcpy(sendBuffer+send_len, (S8 *)dt, 1);
		send_len=send_len+1;		
	}
	 
	//参数8：终端cell-id信息，格式为"MCC：MNC：LAC：CellID"即"区码：中国移动标识：位置区域：小区编号"，均用10进制数表示
	//mcc:
	nfj = rj_agps_send_data.mcc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//mnc:
	nfj = rj_agps_send_data.mnc;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//lac:
	nfj = rj_agps_send_data.lac;
	memset(fjchar,0,sizeof(fjchar));	
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dh, 1);
	send_len=send_len+1;
	//cell:
	nfj = rj_agps_send_data.cell_id;
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nfj);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
 }

void YD_Soc_Send_search_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 * params,U32 sevice_order_id)
{
	//[2011-12-1510:00:00, 1,BD01,030600001,T14,FREQ=15,213331]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T14",3);
	
	kal_prompt_trace(MOD_SOC,"YD_Soc_Send_search_ReqBuffer param %s",params);

	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//params
	memcpy((S8 *)(sendBuffer+send_len), (const void *)params, strlen((S8 *)params));
	send_len =send_len+ strlen((S8 *)params);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	//add sevice_order_id
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",sevice_order_id);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_set_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 * params,S8 rtn,U32 sevice_order_id)
{
	//[2011-12-15 10:00:00,1,BD01,030600001,T2,DOMAIN,1,32123]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T2",2);
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//paramcmd
	memcpy((S8 *)(sendBuffer+send_len), (const void *)params, strlen((S8 *)params));
	send_len =send_len+ strlen((S8 *)params);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//return value
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",rtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//add sevice_order_id
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",sevice_order_id);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_cq_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T11]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';	
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T11",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_sf_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 rtn)
{
	//[2011-12-1510:00:00, 1,BD01,030600001,T12,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T12",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//return value
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",rtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_cf_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 rtn)
{
	//[2011-12-1510:00:00, 1,BD01,030600001,T13,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T13",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		
	//return value
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",rtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_T37_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T37,BIRD,...]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 ntemp8;
	S8 fjchar[300];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T37",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//CJ
	memcpy(sendBuffer+send_len, (S8 *)yd_get_T37_model(), strlen((S8 *)yd_get_T37_model()));
	send_len = send_len + strlen((S8 *)yd_get_T37_model());
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	//return value
	memset(fjchar,0,sizeof(fjchar));
	#ifdef BIRD_ECU_MMI_SUPPORT
	memcpy(sendBuffer+send_len, ECU_SUMbuf, ECU_C0_CMD2_length());
	send_len=send_len+ECU_C0_CMD2_length();
	#endif
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_uart_T37_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,BIRD_URAT_DATA_ENUM uart_data_type)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T37,BIRD,...]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 hg[1];
	S8 ntemp8;
	S8 fjchar[300];
	U16 nfj;
	
	dt[0] = ',';
	hg[0] = '-';	
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T37",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//CJ
	#ifdef BIRD_ECU_MMI_SUPPORT
	memcpy(sendBuffer+send_len, (S8 *)yd_get_uart_model(uart_data_type), strlen((S8 *)yd_get_uart_model(uart_data_type)));
	send_len = send_len + strlen((S8 *)yd_get_uart_model(uart_data_type));
	memcpy(sendBuffer+send_len, (S8 *)hg, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len, (S8 *)yd_get_T37_vendor(), strlen((S8 *)yd_get_T37_vendor()));
	send_len = send_len + strlen((S8 *)yd_get_T37_vendor());
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;

	//return value
	memset(fjchar,0,sizeof(fjchar));
	memcpy(sendBuffer+send_len, yd_get_uart_buf(uart_data_type), yd_get_uart_buf_length(uart_data_type));
	send_len=send_len+yd_get_uart_buf_length(uart_data_type);
	#endif
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void YD_Soc_Send_T15_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 rtn)
{
	//[2011-12-1510:00:00, 1,BD01,030600001,T15,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T15",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		
	//return value
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",rtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

/*Send Server log information by socket*/
kal_uint8 LOG_INFO_TRANS_BUF[200]={0};	/*log information buffer*///wyz add	

void YD_Soc_Send_log_info_trans_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T37,BIRD,LOG]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 ntemp8;
	S8 fjchar[300];
	U16 nfj;
	
	dt[0] = ',';	
	YD_soc_set_common_head(sendBuffer,&send_len);
	
	//cmd
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T37",3);
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//CJ
	memcpy(sendBuffer+send_len, (S8 *)YD_TK001_BDLOG, strlen((S8 *)YD_TK001_BDLOG));
	send_len = send_len + strlen(YD_TK001_BDLOG);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len,LOG_INFO_TRANS_BUF,strlen(LOG_INFO_TRANS_BUF));
	send_len=send_len+strlen(LOG_INFO_TRANS_BUF);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}//WYZ add

void YD_Soc_Send_log_ecu_receive_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,U8* log_buf,U16 log_length)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T37,BIRD,LOG]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 ntemp8;
	S8 fjchar[300];
	U16 nfj;
	
	dt[0] = ',';
	YD_soc_set_common_head(sendBuffer,&send_len);
	
	//cmd
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T37",3);
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	//CJ
	memcpy(sendBuffer+send_len, (S8 *)YD_TK001_BDLOG, strlen((S8 *)YD_TK001_BDLOG));
	send_len = send_len + strlen(YD_TK001_BDLOG);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;
	memcpy(sendBuffer+send_len,log_buf,log_length);
	send_len=send_len+log_length;

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}
void YD_Soc_Send_df_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,S8 rtn)
{
	//[2011-12-15 10:00:00, 1,BD01,030600001,T34,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T34",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, 3);
	send_len = send_len + 3;
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;		
	//return value
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",rtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);
	
	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}
void bird_soc_sendlogin(void){
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
	_send.send_type=BIRD_SOC_SEND_OTHER;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_login_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_sendheart(void){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_HEART;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_heart_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_sendpos(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_GPSPOS;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	if(bird_get_trace()==0)
	{
		return;
	}
		
	RJ_GPS_GetAGPSData();
	YD_Soc_Send_pos_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);	
	possend = _send;	
	
	if(!Lima_get_soc_conn_flag())
	{
		if(Bird_get_bufunsend()<3)
		{
			Bird_soc_sendbufAdd2(&_send);
		}
		else{
			Bird_soc_save_file(&possend);
		}
		
	}
	else{
		Bird_soc_sendbufAdd2(&_send);
	}
	
}

void bird_soc_sendsleepstatue(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_SLEEPPOS;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	if(bird_get_trace()==0)
	{
		return;
	}
	RJ_GPS_GetAGPSData();
	YD_Soc_Send_pos_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);	
	Bird_soc_sendbufAdd2(&_send);
	
}
void bird_soc_sendstate(S8 nstate){
	Send_Info _send;	
	if(bird_get_trace()==0)
	{
		return;
	}
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_OTHER;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	RJ_GPS_GetAGPSData();
	YD_Soc_Send_state_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,nstate);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	if(alarm_category==TK001_ALARM_ENUM_low_power)
	{
		_send.send_type=BIRD_SOC_SEND_ALARM_LOW;
	}
	else if(alarm_category==TK001_ALARM_ENUM_power_off)
	{
		_send.send_type=BIRD_SOC_SEND_ALARM_POWEROFF;
	}
	else if(alarm_category==TK001_ALARM_ENUM_moved)
	{
		_send.send_type=BIRD_SOC_SEND_ALARM_MOVE;
	}
	else if(alarm_category==TK001_ALARM_ENUM_vibration)
	{
		_send.send_type=BIRD_SOC_SEND_ALARM_ZD;
	}
	else
	{
		_send.send_type=BIRD_SOC_SEND_OTHER;
	}
			
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	if(alarm_category!=TK001_ALARM_ENUM_low_power)
	{
		if(bird_get_trace()==0)
		{
			return;
		}
	}

	RJ_GPS_GetAGPSData();
	YD_Soc_Send_alarm_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,alarm_category);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_sendlc(U8 type){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_HEART;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_lc_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,type);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_search_param_res(U8 *params,U32 sevice_order_id)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_search_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,params,sevice_order_id);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_to_upper(U8 *str)
{
	S8 i=0;
	for(i=0;i<strlen(str);i++)
	{
	 	if(str[i]>='a'&&str[i]<='z')
		  	str[i]-=32;
 	}
}
void bird_search_param_res(U8 *params,U8 *params2)
{
	S8 paramsvalue[100];
	S8 fjchar[16];
	S8 fjchar32[32];
	S8 fjchar32temp[32];
	kal_int32 vol; 
	S8 i=0;
	S8 nlen;
	U8 paramupper[20];
	U32 sevice_order_id = 1;

	memset(paramupper,0,sizeof(paramupper));
	memcpy(paramupper, (U8 *)params, strlen((U8 *)params));	
	bird_to_upper(paramupper);
	
	memset(paramsvalue,0,sizeof(paramsvalue));
	kal_prompt_trace(MOD_SOC,"bird_search_param_res params:%s",params);
	kal_prompt_trace(MOD_SOC,"bird_search_param_res params2:%s",params2);

	 if(strcmp(params2, "")==0)
	 {
	 	sevice_order_id = 1;
	 }
	 else{
	 	sevice_order_id = atol(params2);
	 }
	 kal_prompt_trace(MOD_SOC," bird_search_param_res, sevice_order_id:%d",sevice_order_id);

	if(strcmp(paramupper, "SOFTVERSION") == 0)
	{
		memcpy(paramsvalue, "SOFTVERSION", strlen("SOFTVERSION"));	
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("SOFTVERSION"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SOFTVERSION")+strlen("="), (S8 *)RJ_GPS_VERSION, strlen((S8 *)RJ_GPS_VERSION));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "GSM") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_gsm_strength100());
		memcpy(paramsvalue, "GSM", strlen("GSM"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("GSM"), "=", strlen("="));
		memcpy(paramsvalue+strlen("GSM")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "GPS") == 0)
	{
		//memset(fjchar,0,sizeof(fjchar));
		//sprintf(fjchar,"%d",bird_get_saltllite_num());
		memcpy(paramsvalue, "GPS", strlen("GPS"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("GPS"), "=", strlen("="));
		bird_to_upper(paramsvalue);	 
		memcpy(paramsvalue+strlen("GPS")+strlen("="), (S8 *)gpsinfo, strlen((S8 *)gpsinfo));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "VBAT") == 0)
	{
			
		
		memset(fjchar32temp,0,sizeof(fjchar32temp));
		nlen = 0;
		//bat
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_bat_channel_voltage());
		memcpy(fjchar32temp, (S8 *)fjchar32, strlen((S8 *)fjchar32));
		nlen = strlen((S8 *)fjchar32);
		memcpy(fjchar32temp+nlen, (S8 *)":", strlen((S8 *)":"));
		nlen = nlen + strlen((S8 *)":");
		
		//bat vol
		memset(fjchar32,0,sizeof(fjchar32));
		vol = bird_get_charge_channel_voltage();
		sprintf(fjchar32,"%d",vol);
		memcpy(fjchar32temp+nlen, (S8 *)fjchar32, strlen((S8 *)fjchar32));
		nlen = nlen + strlen((S8 *)fjchar32);
		memcpy(fjchar32temp+nlen, (S8 *)":", strlen((S8 *)":"));
		nlen = nlen + strlen((S8 *)":");
		
		//cur 
		memset(fjchar32,0,sizeof(fjchar32));
		vol = bird_get_charge_channel_curr();
		sprintf(fjchar32,"%d",vol);
		memcpy(fjchar32temp+nlen, (S8 *)fjchar32, strlen((S8 *)fjchar32));
		
		memcpy(paramsvalue, "VBAT", strlen("VBAT"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VBAT"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VBAT")+strlen("="), (S8 *)fjchar32temp, strlen((S8 *)fjchar32temp));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
		bird_set_charge_channel_voltage();
		bird_set_charge_channel_curr();
	}
	else if(strcmp(paramupper, "VIBCHK") == 0)
	{
		memset(fjchar32temp,0,sizeof(fjchar32temp));
		nlen = 0;
		//X
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_vibration_running_alarm_level());
		memcpy(fjchar32temp, (S8 *)fjchar32, strlen((S8 *)fjchar32));
		nlen = strlen((S8 *)fjchar32);
		memcpy(fjchar32temp+nlen, (S8 *)":", strlen((S8 *)":"));
		nlen = nlen + strlen((S8 *)":");
		
		//Y
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_vibration_running_alarm_times_value());
		memcpy(fjchar32temp+nlen, (S8 *)fjchar32, strlen((S8 *)fjchar32));
		nlen = nlen + strlen((S8 *)fjchar32);

		memcpy(paramsvalue, "VIBCHK", strlen("VIBCHK"));
		bird_to_upper(paramsvalue);
              memcpy(paramsvalue+strlen("VIBCHK"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIBCHK")+strlen("="), (S8 *)fjchar32temp, strlen((S8 *)fjchar32temp));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);		
		
	}
	else if(strcmp(paramupper, "VIN") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%f",bird_get_adc_channel_voltage());
		memcpy(paramsvalue, "VIN", strlen("VIN"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VIN"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIN")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "LOGIN") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_islogin());
		memcpy(paramsvalue, "LOGIN", strlen("LOGIN"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("LOGIN"), "=", strlen("="));
		memcpy(paramsvalue+strlen("LOGIN")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "PLCID") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",1);
		memcpy(paramsvalue, "PLCID", strlen("PLCID"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("PLCID"), "=", strlen("="));
		memcpy(paramsvalue+strlen("PLCID")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "IMSI") == 0)
	{
		memcpy(paramsvalue, "IMSI", strlen("IMSI"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("IMSI"), "=", strlen("="));
		memcpy(paramsvalue+strlen("IMSI")+strlen("="), (S8 *)rj_user_info.u_imsi, strlen((S8 *)rj_user_info.u_imsi));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
      else if(strcmp(paramupper, "IMEI") == 0)
	{
		memcpy(paramsvalue, "IMEI", strlen("IMEI"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("IMEI"), "=", strlen("="));
		memcpy(paramsvalue+strlen("IMEI")+strlen("="), (S8 *)rj_user_info.u_imei, strlen((S8 *)rj_user_info.u_imei));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "PSW") == 0)
	{
		memcpy(paramsvalue, "PSW", strlen("PSW"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("PSW"), "=", strlen("="));
		memcpy(paramsvalue+strlen("PSW")+strlen("="), (S8 *)bird_get_utility_pw(), strlen((S8 *)bird_get_utility_pw()));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "ACC") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",get_Acc_onoff());
		memcpy(paramsvalue, "ACC", strlen("ACC"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("ACC"), "=", strlen("="));
		memcpy(paramsvalue+strlen("ACC")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "DOMAIN") == 0)
	{
		memcpy(paramsvalue, "DOMAIN", strlen("DOMAIN"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("DOMAIN"), "=", strlen("="));
		memcpy(paramsvalue+strlen("DOMAIN")+strlen("="), (S8 *)bird_get_server_address(), strlen((S8 *)bird_get_server_address()));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "FREQ") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_main_interval_value());
		memcpy(paramsvalue, "FREQ", strlen("FREQ"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("FREQ"), "=", strlen("="));
		memcpy(paramsvalue+strlen("FREQ")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "ALARMDELAY") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_alarm_delay_value());
		memcpy(paramsvalue, "ALARMDELAY", strlen("ALARMDELAY"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("ALARMDELAY"), "=", strlen("="));
		memcpy(paramsvalue+strlen("ALARMDELAY")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "TRACE") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_trace());
		memcpy(paramsvalue, "TRACE", strlen("TRACE"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("TRACE"), "=", strlen("="));
		memcpy(paramsvalue+strlen("TRACE")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "PULSE") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_heart_interval_value());
		memcpy(paramsvalue, "PULSE", strlen("PULSE"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("PULSE"), "=", strlen("="));
		memcpy(paramsvalue+strlen("PULSE")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "PHONE") == 0)
	{
		memcpy(paramsvalue, "PHONE", strlen("PHONE"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("PHONE"), "=", strlen("="));
		if(bird_get_authorize_number()==NULL)
		{
			memcpy(paramsvalue+strlen("PHONE")+strlen("="), (S8 *)" ", strlen((S8 *)" "));
		}
		else
		{
			memcpy(paramsvalue+strlen("PHONE")+strlen("="), (S8 *)bird_get_authorize_number(), strlen((S8 *)bird_get_authorize_number()));
		}
		
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "USER") == 0)
	{
		memcpy(paramsvalue, "USER", strlen("USER"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("USER"), "=", strlen("="));
		if(bird_get_utility_number()==NULL)
		{
			memcpy(paramsvalue+strlen("PHONE")+strlen("="), (S8 *)" ", strlen((S8 *)" "));
		}
		else
		{			 
			memcpy(paramsvalue+strlen("USER")+strlen("="), (S8 *)bird_get_utility_number(), strlen((S8 *)bird_get_utility_number()));
		}
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "RADIUS") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_move_position_alarm_value());
		memcpy(paramsvalue, "RADIUS", strlen("RADIUS"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("RADIUS"), "=", strlen("="));
		memcpy(paramsvalue+strlen("RADIUS")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "VIB") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_vibration_running_alarm_flag());
		memcpy(paramsvalue, "VIB", strlen("VIB"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VIB"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIB")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "VIBL") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_vibration_sensitivity_value());
		memcpy(paramsvalue, "VIBL", strlen("VIBL"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VIBL"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIBL")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "POF") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_low_power_alarm_flag());
		memcpy(paramsvalue, "POF", strlen("POF"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("POF"), "=", strlen("="));
		memcpy(paramsvalue+strlen("POF")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "LBV") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_bvl());
		memcpy(paramsvalue, "LBV", strlen("LBV"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("LBV"), "=", strlen("="));
		memcpy(paramsvalue+strlen("LBV")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "SPEED") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_speed_alarm_value());
		memcpy(paramsvalue, "SPEED", strlen("SPEED"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("SPEED"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SPEED")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "SPEEDF") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_speed_alarm_flag());
		memcpy(paramsvalue, "SPEEDF", strlen("SPEEDF"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("SPEEDF"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SPEEDF")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "CORNER") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_corner_value());
		memcpy(paramsvalue, "CORNER", strlen("CORNER"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("CORNER"), "=", strlen("="));
		memcpy(paramsvalue+strlen("CORNER")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);

	}
	else if(strcmp(paramupper, "SLEEP") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_is_abnormal_sleep_on());
		memcpy(paramsvalue, "SLEEP", strlen("SLEEP"));
		bird_to_upper(paramsvalue);
		kal_prompt_trace(MOD_SOC,"bird_search_param_res params1:%s",paramsvalue);
             memcpy(paramsvalue+strlen("SLEEP"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SLEEP")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "NSLEEP") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_is_normal_sleep_on());
		memcpy(paramsvalue, "NSLEEP", strlen("NSLEEP"));
		bird_to_upper(paramsvalue);
		kal_prompt_trace(MOD_SOC,"bird_search_param_res params1:%s",paramsvalue);
             memcpy(paramsvalue+strlen("NSLEEP"), "=", strlen("="));
		memcpy(paramsvalue+strlen("NSLEEP")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "SLEEPT") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_abnormal_sleep_value());
		memcpy(paramsvalue, "SLEEPT", strlen("SLEEPT"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("SLEEPT"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SLEEPT")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "CALLLOCK") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_CALLLOCK());
		memcpy(paramsvalue, "CALLLOCK", strlen("CALLLOCK"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("CALLLOCK"), "=", strlen("="));
		memcpy(paramsvalue+strlen("CALLLOCK")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "CALLDISP") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_CALLDISP());
		memcpy(paramsvalue, "CALLDISP", strlen("CALLDISP"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("CALLDISP"), "=", strlen("="));
		memcpy(paramsvalue+strlen("CALLDISP")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "SMS") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_SMS());
		memcpy(paramsvalue, "SMS", strlen("SMS"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("SMS"), "=", strlen("="));
		memcpy(paramsvalue+strlen("SMS")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "VIBCALL") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_VIBCALL());
		memcpy(paramsvalue, "VIBCALL", strlen("VIBCALL"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VIBCALL"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIBCALL")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "CID") == 0)
	{
		memcpy(paramsvalue, "CID", strlen("CID"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("CID"), "=", strlen("="));
		memcpy(paramsvalue+strlen("CID")+strlen("="), (S8 *)bird_get_CID(), strlen((S8 *)bird_get_CID()));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "POFT") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_poft_value());
		memcpy(paramsvalue, "POFT", strlen("POFT"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("POFT"), "=", strlen("="));
		memcpy(paramsvalue+strlen("POFT")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "WAKEUP") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_wakeup_value());
		memcpy(paramsvalue, "WAKEUP", strlen("WAKEUP"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("WAKEUP"), "=", strlen("="));
		memcpy(paramsvalue+strlen("WAKEUP")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "WAKEUPT") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_wakeupt_value());
		memcpy(paramsvalue, "WAKEUPT", strlen("WAKEUPT"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("WAKEUPT"), "=", strlen("="));
		memcpy(paramsvalue+strlen("WAKEUPT")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "ACCLT") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_defense_delay_value());
		memcpy(paramsvalue, "ACCLT", strlen("ACCLT"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("ACCLT"), "=", strlen("="));
		memcpy(paramsvalue+strlen("ACCLT")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}		
	else if(strcmp(paramupper, "ACCLOCK") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_acclock_value());
		memcpy(paramsvalue, "ACCLOCK", strlen("ACCLOCK"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("ACCLOCK"), "=", strlen("="));
		memcpy(paramsvalue+strlen("ACCLOCK")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "VIBGPS") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",1);
		memcpy(paramsvalue, "VIBGPS", strlen("VIBGPS"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("VIBGPS"), "=", strlen("="));
		memcpy(paramsvalue+strlen("VIBGPS")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	else if(strcmp(paramupper, "AGPSADDR") == 0)
	{
		memcpy(paramsvalue, "AGPSADDR", strlen("AGPSADDR"));
		bird_to_upper(paramsvalue);
		memcpy(paramsvalue+strlen("AGPSADDR"), "=", strlen("="));
		memcpy(paramsvalue+strlen("AGPSADDR")+strlen("="), (S8 *)bird_get_agps_address(), strlen((S8 *)bird_get_agps_address()));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	#ifdef __MMI_BT_SUPPORT__
	else if(strcmp(paramupper, "BLUETOOTH") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_bluetooth_state());
		memcpy(paramsvalue, "BLUETOOTH", strlen("BLUETOOTH"));
		bird_to_upper(paramsvalue);
             memcpy(paramsvalue+strlen("BLUETOOTH"), "=", strlen("="));
		memcpy(paramsvalue+strlen("BLUETOOTH")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}	
	#endif
	#if defined(BIRD_SWZ)||defined(__BIRD_RUN_EINT__)	
	else if(strcmp(paramupper, "HITL") == 0)
	{
		memset(fjchar,0,sizeof(fjchar));
		sprintf(fjchar,"%d",bird_get_hitlvalue());
		memcpy(paramsvalue, "HITL", strlen("HITL"));
		bird_to_upper(paramsvalue);
              memcpy(paramsvalue+strlen("HITL"), "=", strlen("="));
		memcpy(paramsvalue+strlen("HITL")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
       #endif
	else if(strcmp(paramupper, "MODEL") == 0)
	{
		memcpy(paramsvalue, "MODEL", strlen("MODEL"));
		bird_to_upper(paramsvalue);
		memcpy(paramsvalue+strlen("MODEL"), "=", strlen("="));
		memcpy(paramsvalue+strlen("MODEL")+strlen("="), (S8 *)yd_get_model(), strlen((S8 *)yd_get_model()));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "EQUMODE") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_equmode());
		memcpy(paramsvalue, "EQUMODE", strlen("EQUMODE"));
		bird_to_upper(paramsvalue);
		memcpy(paramsvalue+strlen("EQUMODE"), "=", strlen("="));
		memcpy(paramsvalue+strlen("EQUMODE")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
	else if(strcmp(paramupper, "OCHEARTTIME") == 0)
	{
		memset(fjchar32,0,sizeof(fjchar32));
		sprintf(fjchar32,"%d",bird_get_oc_main_heart_time());
		memcpy(paramsvalue, "OCHEARTTIME", strlen("OCHEARTTIME"));
		bird_to_upper(paramsvalue);
		memcpy(paramsvalue+strlen("OCHEARTTIME"), "=", strlen("="));
		memcpy(paramsvalue+strlen("OCHEARTTIME")+strlen("="), (S8 *)fjchar32, strlen((S8 *)fjchar32));
		bird_soc_search_param_res(paramsvalue,sevice_order_id);
	}
}
void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_set_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,paramcmd,rtn,sevice_order_id);
	Bird_soc_sendbufAdd2(&_send);
}

#define bird_is_digit(c)        (((c >= '0') && (c <= '9')))
MMI_BOOL bird_params_is_digit(S8 *url)
{
    MMI_BOOL ret=MMI_FALSE;
    S8 *temp_url=url;


    while(*temp_url)
    {
          if(bird_is_digit(*temp_url))
	  {
                temp_url++;
	  }
	  else
	  {  
                ret=MMI_FALSE;
                return MMI_FALSE;
	  }
    }
    return MMI_TRUE;

}
void bird_set_param_res(U8 *params,U8 *params2)
{
	S8 paramsvalue[100];
	S8 paramcmd[10];
	S8 *p;
	U16 ntemp = 0;
	U32 ntemp1 = 0;
	U8 nrtn = 1;
	U16 nv1,nv2;
	U8 paramupper[20];
	U32 sevice_order_id = 1;

	memset(paramupper,0,sizeof(paramupper));
	memset(paramsvalue,0,sizeof(paramsvalue));
	memset(paramcmd,0,sizeof(paramcmd));
	kal_prompt_trace(MOD_SOC,"bird_set_param_res params:%s",params);
	 p = strchr(params, '=');
	 if(p==NULL || strlen(p)<2)
	 	return;
	 kal_prompt_trace(MOD_SOC,"bird_set_param_res p,len1,len2:%s %d %d",p,strlen(params) , strlen(p));
	 memcpy(paramcmd, params,strlen(params) - strlen(p));
	 memcpy(paramupper, (U8 *)paramcmd, strlen((U8 *)paramcmd));	
	bird_to_upper(paramupper);
	
	 memcpy(paramsvalue, p+1, strlen(p)-1);
	 kal_prompt_trace(MOD_SOC,"bird_set_param_res paramcmd:%s,paramvalue:%s",paramcmd,paramsvalue);

	 if(strcmp(params2, "")==0)
	 {
	 	sevice_order_id = 1;
	 }
	 else{
	 	sevice_order_id = atol(params2);
	 }
	 kal_prompt_trace(MOD_SOC," bird_set_param_res, sevice_order_id:%d",sevice_order_id);
	 

        //数值设定，如果参数非数值，返回失败
        if((strcmp(paramupper, "PSW") != 0)&&(strcmp(paramupper, "DOMAIN") != 0)&&(strcmp(paramupper, "VIBCHK") != 0)
			&&(strcmp(paramupper, "CID") != 0)&&(strcmp(paramupper, "AGPSADDR") != 0))
        {
            if(bird_params_is_digit(paramsvalue)==MMI_FALSE)
            {
			kal_prompt_trace(MOD_SOC,"bird_set_param_res error");
			bird_soc_set_param_res(params,0,sevice_order_id);     
			return;
            }
        }
	if(strcmp(paramupper, "PSW") == 0)
	{
	       if(strlen(paramsvalue)>=YD_TK001_PHONE_LEN)
	       {
			nrtn = 0;
	       }
	       else{
		    bird_set_utility_pw(paramsvalue);	
	       }
	       bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "DOMAIN") == 0)
	{
	       if(strlen(paramsvalue)>=YD_TK001_SERVER_ADDRESS_LEN)
	       {
			nrtn = 0;
	       }
	       else{
		    bird_set_server_address(paramsvalue);
		    yd_tk002_set_record_from_NV();
	       }
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
			//重新连接
		Lima_set_soc_init_flag(FALSE);
             Lima_Soc_Dinit();
		Bird_soc_conn();
		bird_soc_sendlogin();		
	}
	else if(strcmp(paramupper, "FREQ") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1<2)
		{
			nrtn = 0;
		}
		else{
			bird_set_main_interval_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "ALARMDELAY") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1<=0)
		{
			nrtn = 0;
		}
		else	{
			bird_set_alarm_delay_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "TRACE") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_trace_value(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "PULSE") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1<10)
		{
			nrtn = 0;
		}
		else{
			bird_set_heart_interval_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "PHONE") == 0)
	{
	       if(strlen(paramsvalue)>=YD_TK001_PHONE_LEN)
	       {
			nrtn = 0;
	       }
	       else{
		    bird_set_authorize_number(paramsvalue);
	       }
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "USER") == 0)
	{
	       if(strlen(paramsvalue)>=YD_TK001_PHONE_LEN)
	       {
			nrtn = 0;
	       }
	       else{
		    bird_set_utility_number(paramsvalue);
	       }
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "RADIUS") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1==0)
		{
			bird_set_move_position_alarm_flag(0);
			bird_set_move_position_alarm_value(0);
		}
		else{
			bird_set_move_position_alarm_flag(1);
			bird_set_move_position_alarm_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "VIB") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_soc_set_param_res(params,nrtn,sevice_order_id);
			if(bird_get_vibration_running_alarm_flag()!=ntemp)
			{
				bird_set_vibration_running_alarm_flag(ntemp);
				bird_soc_sendpos();
				kal_prompt_trace(MOD_SOC,"VIB sendpos");
			}
		}
		else{
			nrtn = 0;
			bird_soc_set_param_res(params,nrtn,sevice_order_id);
		}
	}
	else if(strcmp(paramupper, "VIBL") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp<=0 || ntemp>15)
		{
			nrtn = 0;
		}
		else{
			nrtn = 1;		
			bird_set_vibration_sensitivity_value(ntemp);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "VIBCHK") == 0)
	{
		S8 vibv1[8];
		S8 vibv2[8];
		 p = strchr(paramsvalue, ':');
	 	if(p==NULL || strlen(p)<1)
	 		return;
	 	memcpy(vibv1, paramsvalue,strlen(paramsvalue) - strlen(p));
	 	memcpy(vibv2, p+1, strlen(p)-1);
		if((atol(vibv1)>=0xffff)||(atol(vibv2)>=0xffff))
		{
			nrtn = 0;
		}
		else{
		nv1 = atoi(vibv1);
		nv2 = atoi(vibv2);
		kal_prompt_trace(MOD_SOC,"bird_set_param_res VIBCHK 1:%d,2:%d",nv1,nv2);
		nrtn = 1;		
		bird_vibration_running_alarm_value(nv1,nv2);	
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "POF") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_external_power_alarm_flag(ntemp);
			bird_set_low_power_alarm_flag(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "LBV") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if((ntemp1>4000000)||(ntemp1<3500000))
		{
			nrtn = 0;
		}
		else{
			nrtn = 1;
			bird_set_bvl(ntemp1);
		}		
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "SPEED") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1>0)
		{
			bird_speed_alarm_flag(1);
			bird_speed_alarm_value(ntemp1);
		}
		else{
			bird_speed_alarm_flag(0);
			bird_speed_alarm_value(0);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "SPEEDF") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_speed_alarm_flag(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "CORNER") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1>180)
		{
			nrtn = 0;
		}
		else{
			nrtn = 1;
			bird_set_corner_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}		
	else if(strcmp(paramupper, "SLEEP") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_abnormal_sleep_flag(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "NSLEEP") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_normal_sleep_flag(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "SLEEPT") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1<2)
		{
			nrtn = 0;
		}
		else
		{
			nrtn = 1;
			bird_set_abnormal_sleep_value(ntemp1);
			bird_set_normal_sleep_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "CALLLOCK") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_CALLLOCK(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "CALLDISP") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_CALLDISP(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "SMS") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_SMS(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}		
	else if(strcmp(paramupper, "VIBCALL") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_VIBCALL(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "CID") == 0)
	{
	       if(strlen(paramsvalue)>=10)
	       {
			nrtn = 0;
	       }
	       else{
			nrtn = 1;
			bird_set_CID(paramsvalue);
	       }
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "POFT") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if((ntemp1<=0)||(ntemp1>0xff))
		{
			nrtn = 0;
		}
		else	{
			nrtn = 1;
			bird_set_poft_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "WAKEUP") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_wakeup_value(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else if(strcmp(paramupper, "WAKEUPT") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if((ntemp1<=0)||(ntemp1>0xff))
		{
			nrtn = 0;
		}
		else	{
			nrtn = 1;
			bird_set_wakeupt_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "ACCLT") == 0)
	{
		ntemp1 = atol(paramsvalue);
		if(ntemp1<=0)
		{
			nrtn = 0;
		}
		else	{
			nrtn = 1;
			bird_set_defense_delay_value(ntemp1);
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "ACCLOCK") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_acclock_value(ntemp);
		}
		else{
			nrtn = 0;
		}
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "VIBGPS") == 0)
	{
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
#ifdef __MMI_BT_SUPPORT__
	else if(strcmp(paramupper, "BLUETOOTH") == 0)
	{
		ntemp = atoi(paramsvalue);
		if(ntemp>=0 && ntemp<=1)
		{
			nrtn = 1;
			bird_set_bluetooth_state(ntemp);
		}
		else{
			nrtn = 0;
		}	
		bird_to_upper(paramcmd);
     		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
#endif
	else if(strcmp(paramupper, "AGPSADDR") == 0)
	{
	       if(strlen(paramsvalue)>=YD_TK001_SERVER_ADDRESS_LEN)
	       {
			nrtn = 0;
	       }
	       else{
		    bird_set_agps_address(paramsvalue);
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "OC_ENABLE") == 0)/*断开油路*/
	{
	#ifdef BIRD_OC_LOGIC
		bird_oc_logic_res();
		bird_soc_set_param_res(params,bird_get_oc_exec_state(),sevice_order_id);
		bird_ocin_state_res(1);
		bird_set_oc_instruct(sevice_order_id);
	#else
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
		bird_ocin_state_res(1);
		bird_set_dy_state(1);
	#endif
	}
	else if(strcmp(paramupper, "OC_DISABLE") == 0)/*恢复油路*/
	{
	#ifdef BIRD_OC_LOGIC
		bird_set_oc_instruct(sevice_order_id);
		bird_set_oc_exec_state(OC_SET_SUCCESS);
		bird_soc_set_param_res(params,OC_SET_SUCCESS,sevice_order_id);
		bird_ocin_state_res(0);
		bird_set_dy_state(0);
	#else
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
		bird_ocin_state_res(0);
		bird_set_dy_state(0);
	#endif
	}
	#if defined(BIRD_SWZ)||defined(__BIRD_RUN_EINT__)		
	else if(strcmp(paramupper, "HITL") == 0)
	{
		ntemp = atoi(paramsvalue);
		if((ntemp<=0)||(ntemp>15))
		{
			nrtn = 0;
		}
		else
		{
			nrtn = 1;
			bird_set_hitl_value(ntemp);
		}	
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	#endif
	else if(strcmp(paramupper, "EQUMODE") == 0)
	{
	       ntemp = atoi(paramsvalue);
	       if(ntemp>0xff)
		{
			nrtn = 0;
		}	
		else
		{
			nrtn = 1;
			bird_set_equmode(ntemp);
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "LIMITSPEED") == 0)
	{

		kal_prompt_trace(MOD_SOC,"bird_set_param_res LIMITSPEED");
		ntemp = atoi(paramsvalue);
		if(((ntemp>=10)&&(ntemp<=100))||(ntemp==0))
		{
		    #ifdef BIRD_ECU_MMI_SUPPORT
		    Yd_ecu_limit_speed_res(ntemp,sevice_order_id);
		    #endif
		}
		else
		{
			nrtn = 0;
			bird_to_upper(paramcmd);
			bird_soc_set_param_res(params,nrtn,sevice_order_id);
		}
	}	
	else if(strcmp(paramupper, "FINDCAR") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_findcar_res(sevice_order_id);
		#endif
	}
	else if(strcmp(paramupper, "OPENTRUNK") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_openaz_res(sevice_order_id);
		#endif
	}
	else if(strcmp(paramupper, "CARCHECK") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_carcheck_res(sevice_order_id);
		#endif
	}
	else if(strcmp(paramupper, "CARSTART") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_carstart_res(sevice_order_id,1);
		#endif
	}	
	else if(strcmp(paramupper, "CARSTOP") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_carstart_res(sevice_order_id,0);
		#endif
	}	
	else if(strcmp(paramupper, "GETBAT") == 0)
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_getbat_res(sevice_order_id);
		#endif
	}	
	else if(strcmp(paramupper, "OCHEARTTIME") == 0)
	{
	       ntemp = atoi(paramsvalue);
	       if(ntemp>0xff)
		{
			nrtn = 0;
		}	
		else
		{
			nrtn = 1;
			bird_set_oc_main_heart_time(ntemp);
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "POWEROFF") == 0)
	{
	       if(is_ac_charger_in())
		{
			nrtn = 0;
		}	
		else
		{
			nrtn = 1;
			yd_tk001_power_off_handler();
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "UARTBAT") == 0)
	{
	       ntemp = atoi(paramsvalue);
	       if(ntemp>0xff)
		{
			nrtn = 0;
		}	
		else
		{
			nrtn = 1;
			bird_set_uart_bat_interval(ntemp);
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "RESETNVRAM") == 0)
	{
	       ntemp = atoi(paramsvalue);
	       if(ntemp!=16)
		{
			nrtn = 0;
		}	
		else
		{
			nrtn = 1;
			yd_tk001_set_default_record_to_NV();
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "WAKEUPGPS") == 0)
	{
	       ntemp = atoi(paramsvalue);
	       if(ntemp!=1)
		{
			nrtn = 0;
		}
		else
		{
			nrtn = 1;
			bird_ini_gpsposition();
			yd_tk001_wakeup_function();
		}
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}
	else if(strcmp(paramupper, "IMEI") == 0)
	{
	       if(strlen(paramsvalue)!=15)
	       {
			nrtn = 0;
	       }
	       else{
		    nrtn=yd_set_imei_by_server(paramsvalue);	
		    if(nrtn==1)
		        yd_tk001_power_off_handler();
	       }
		bird_to_upper(paramcmd);
		bird_soc_set_param_res(params,nrtn,sevice_order_id);
	}	
	else{
		return;
	}
	//save to nv
	yd_send_save_nv_msg();
}

void bird_soc_cq_res()
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_cq_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_set_sf_res(U8 rtn)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_sf_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,rtn);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_set_cf_res(U8 rtn)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_cf_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,rtn);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_set_defense_res(U8 rtn)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_df_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,rtn);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_T37_send()
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_T37_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_uart_T37_send(BIRD_URAT_DATA_ENUM uart_data_type)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_uart_T37_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,uart_data_type);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_T15_res(U8 rtn)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_T15_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,rtn);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_set_log_info_trans_res()
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_log_info_trans_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}
void bird_soc_set_log_ecu_receive_res(U8* log_buf,U16 log_length)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_log_ecu_receive_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,log_buf,log_length);
	Bird_soc_sendbufAdd2(&_send);
}

/*把log信息写入LOG_INFO_TRANS_BUF*/
void bird_set_soc_log_info(SOC_LOG_INFO categroy,char *str,...)
{
	if((bird_get_equmode()==2)||(bird_get_equmode()==(U8)categroy))
	{	
		va_list ap;
		kal_uint8 log_info_len=0;

		log_info_len=strlen(LOG_INFO_TRANS_BUF);
		va_end(ap);
		va_start(ap,str);
		vsprintf(LOG_INFO_TRANS_BUF+log_info_len,str,ap);
		va_end(ap);
		memcpy(LOG_INFO_TRANS_BUF+strlen(LOG_INFO_TRANS_BUF),";",1);
		kal_prompt_trace(MOD_SOC,"LOG_BUF:%s",LOG_INFO_TRANS_BUF);
		if(strlen(LOG_INFO_TRANS_BUF)>=169)
		{
			bird_soc_set_log_info_trans_res();
			memset(LOG_INFO_TRANS_BUF,0,sizeof(LOG_INFO_TRANS_BUF));
		}
	}
}//WYZ add

void bird_soc_senddw(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	if(bird_get_trace()==0)
	{
		return;
	}	
	RJ_GPS_GetAGPSData();
	YD_Soc_Send_dw_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);	

}


void bird_soc_dealdw(U8 *params1,U8 *params2){
	U8 paramchar[8];
	U16 param1value,param2value;
	bird_soc_senddw();
	/*
	if(strlen((U8 *)params1)==0 || strlen((U8 *)params2)==0 )
	{
		bird_soc_senddw();
	}
	else{
		memset(paramchar,0,sizeof(paramchar));
		memcpy(paramchar,(U8 *)params1,strlen((U8 *)params1));
    		param1value = strtol(paramchar,NULL,10);
		memset(paramchar,0,sizeof(paramchar));
		memcpy(paramchar,(U8 *)params2,strlen((U8 *)params2));
    		param2value = strtol(paramchar,NULL,10);	
		kal_prompt_trace(  MOD_SOC,"bird_soc_senddw p1,p2 %d,%d", param1value,param2value);
		bird_set_continue_search_gprs(param2value,param1value);
		yd_tk001_wakeup_function();
	}
	*/

}

void bird_soc_sendjy(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_jy_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_sendzc(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_zc_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}

void bird_soc_sendgetdw(){
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_getdw_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow);
	Bird_soc_sendbufAdd2(&_send);
}
#ifdef BIRD_ECU_MMI_SUPPORT
void YD_Soc_Send_server_to_uart_ReqBuffer(U8* sendBuffer,U16 *sendBuffer_len,U8* nflow,U8 nrtn,U32 sevice_order_id)
{
	//[2011-12-15 10:00:00,T53,1220,1]
	U16 send_len;    
	S8 cmd[4];
	S8 dt[1];
	S8 fjchar[4];
	U16 nfj;
	
	dt[0] = ',';
	memset(cmd,0,sizeof(cmd));
	memcpy(cmd,"T53",3);
	
	YD_soc_set_common_head(sendBuffer,&send_len);
	//cmd
	memcpy(sendBuffer+send_len, (S8 *)cmd, strlen((S8 *)cmd));
	send_len = send_len + strlen((S8 *)cmd);
	memcpy(sendBuffer+send_len, (S8 *)dt, 1);
	send_len=send_len+1;	
	
	//add sevice_order_id
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",sevice_order_id);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);

	//add rtn
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",nrtn);
	memcpy(sendBuffer+send_len, (S8 *)fjchar, strlen((S8 *)fjchar));
	send_len=send_len+strlen((S8 *)fjchar);

	*(sendBuffer+send_len)=']';	
	send_len = send_len+1;
	*sendBuffer_len = send_len;
}

void bird_soc_server_to_uart_res(U8 nrtn,U32 sevice_order_id)
{
	Send_Info _send;	
	_send.buf_len=0;
	_send.ini_flag=0;
	_send.send_flow=0;
	_send.send_type=BIRD_SOC_SEND_NOREPLY;
	memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	YD_Soc_Send_server_to_uart_ReqBuffer(_send.send_buf,&_send.buf_len,&_send.send_flow,nrtn,sevice_order_id);
	Bird_soc_sendbufAdd2(&_send);
}

void yd_server_send_data_to_uart(U8 *instru_ID,U8 *instru_type,U8 *data)
{
	U8 nrtn = 1;
	U8 type=0;
	U32 sevice_order_id = 1;
	U8 i=0,j=0;
	U16 tmp_buf=0;
	U8 GPS2ECU_wirte_buf[50] = {0};

	if(strcmp(instru_ID, "")!=0)
	{
	 	sevice_order_id = atol(instru_ID);
	}
	if(strcmp(instru_type, "")!=0)
	{
	 	type = atol(instru_type);
	}
	kal_prompt_trace(MOD_SOC," yd_server_send_data_to_uart, %d:%d",sevice_order_id,type);

	//参数设置数据
	if(type==1)
	{
	       if(data!=NULL)
	       {
	       	kal_prompt_trace(MOD_SOC," yd_server_send_data_to_uart1");

	       	if((data[0]==0xC9)&&(data[1]==0x10)&&(data[2]==0x03))
	       	{
	       	    for(i=0,j=0;i<40;i++,j++)
	       	    {
				if(data[i]==0x7E)
				{
				    if(data[i+1]==0x01)
				        GPS2ECU_wirte_buf[j]=0x7E;
				    else if(data[i+1]==0x02)
				        GPS2ECU_wirte_buf[j]=0x5B;
				    else
				        GPS2ECU_wirte_buf[j]=0x7E;
				    i++;
				}
				else if(data[i]==0x7C)
				{
				    if(data[i+1]==0x01)
				        GPS2ECU_wirte_buf[j]=0x7C;
				    else if(data[i+1]==0x02)
				        GPS2ECU_wirte_buf[j]=0x5D;
				    else
				        GPS2ECU_wirte_buf[j]=0x7C;
				    i++;
				}
				else
				{
				    GPS2ECU_wirte_buf[j]=data[i];		
				}
	       	    }

	       	    tmp_buf=GPS2ECU_wirte_buf[0];
	       	    for(i=1;i<18;i++)
	       	    {
	       	        tmp_buf ^=GPS2ECU_wirte_buf[i];
	       	    }
	       	    kal_prompt_trace(MOD_SOC," yd_server_send_data_to_uart1 %d %d %d",GPS2ECU_wirte_buf[18],tmp_buf,GPS2ECU_wirte_buf[19]);
	       	    if((GPS2ECU_wirte_buf[18]==tmp_buf)&&(GPS2ECU_wirte_buf[19]==0x0d))
	       	    {
	       	        ECU_UART_Write(GPS2ECU_wirte_buf,20);
	       	        bird_soc_server_to_uart_res(nrtn,sevice_order_id);
	       	    }
	       	}
	       }
	}

	//自定义数据透传
	if(type==2)
	{
	       if(data!=NULL)
	       {
	       	kal_prompt_trace(MOD_SOC," yd_server_send_data_to_uart2");

	       	if((data[0]==0xC9)&&(data[1]==0x14)&&(data[2]==0x02))
	       	{
	       	    for(i=0,j=0;i<40;i++,j++)
	       	    {
				if(data[i]==0x7E)
				{
				    if(data[i+1]==0x01)
				        GPS2ECU_wirte_buf[j]=0x7E;
				    else if(data[i+1]==0x02)
				        GPS2ECU_wirte_buf[j]=0x5B;
				    else
				        GPS2ECU_wirte_buf[j]=0x7E;
				    i++;
				}
				else if(data[i]==0x7C)
				{
				    if(data[i+1]==0x01)
				        GPS2ECU_wirte_buf[j]=0x7C;
				    else if(data[i+1]==0x02)
				        GPS2ECU_wirte_buf[j]=0x5D;
				    else
				        GPS2ECU_wirte_buf[j]=0x7C;
				    i++;
				}
				else
				{
				    GPS2ECU_wirte_buf[j]=data[i];		
				}
	       	    }

	       	    tmp_buf=GPS2ECU_wirte_buf[0];
	       	    for(i=1;i<22;i++)
	       	    {
	       	        tmp_buf ^=GPS2ECU_wirte_buf[i];
	       	    }
	       	    kal_prompt_trace(MOD_SOC," yd_server_send_data_to_uart1 %d %d %d",GPS2ECU_wirte_buf[22],tmp_buf,GPS2ECU_wirte_buf[23]);
	       	    if((GPS2ECU_wirte_buf[22]==tmp_buf)&&(GPS2ECU_wirte_buf[23]==0x0d))
	       	    {
	       	        ECU_UART_Write(GPS2ECU_wirte_buf,24);
	       	        bird_soc_server_to_uart_res(nrtn,sevice_order_id);
	       	    }
	       	}
	       }
	}

}
#endif
