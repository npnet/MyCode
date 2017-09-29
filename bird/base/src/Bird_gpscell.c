/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD GPS station data processing function
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifdef RJ_GPS_APP

#include "Bird_app.h"
#include "Bird_gpscell.h"
#include <stdlib.h>
#include  "Rj_DW02.h" 
#include  "yd_main.h" 
#include  "Bird_task.h" 
#ifdef FOR_MTK_GPS_TEST /* for MTK GPS restart test .add by huangbx */
#include "mdi_datatype.h"
#endif

kal_uint16 rj_gps_for_at_test = 0;
RJ_gps_context_struct RJ_Gps_Info;
/* GPS 模块工作状态 */
volatile U8 rj_gps_gps_module;                                                                                   
static S16 em_gps_port = 0;
static U8 em_gps_cmd_type = 0;

U8 nsnryxnum1 =0;
U8 nsnryxnum2 =0;
U8 nmaxsnr = 0;
S8 g_n_saltllite_snr_isvalid = 0;
float g_n_saltllite_snr_curnow = 0.0;
float g_n_saltllite_snr_cur = 0;
S8 g_n_heart_saltlite_num=0;
U32 gps_remove_timer=0;
U16 gps_restart_timer=0;

S8 gpsinfo[100];
U8 bird_gps_close_open_flag = 0;

peph_op_struct agps_peph_op;

RJ_Gps_Position gps_position = {0};
RJ_Gps_Position heart_position = {0};
RJ_Gps_Position alarm_position = {0};
RJ_Gps_Position rj_position;
RJ_gps_engineer_satinfo engineer_satinfo[MDI_GPS_NMEA_MAX_SVVIEW];              /* 工程模式数据数组 */
#ifdef MTK_SLEEP_ENABLE
kal_uint8 gps_signal_handle;
#endif
#ifdef HD8020_GPS_SUPPORT
extern kal_int8 gps_close_location_flag;
#endif

/*函数声明*/
void RJ_GPS_SetGpsModule(U8 in);
void RJ_GPS_Enter(void);
void RJ_GPS_Stop();
//extern void RJ_GPS_distance_handle(mdi_gps_nmea_rmc_struct rmc_data);
extern void RJ_GPS_distance_handle(mdi_gps_nmea_gga_struct rmc_data);
extern void BIRD_GPS_Cold_Start();

extern YD_TK001_COMMAND_Info yd_tk001_info;
extern U8 bird_sim_signal_strength_in_percentage;
extern U8* bird_get_agps_address(void);

/*-------------------------------------------------------
                     判断卫星是否可用
-------------------------------------------------------*/
U8 RJ_GPS_is_working(void)
{
    U8 ret = 0;
    kal_prompt_trace( MOD_SOC,"___RJ_GPS_is_working num ,valid =%d,%d",rj_position.satellite_num,g_n_saltllite_snr_isvalid); 
    if((g_n_saltllite_snr_isvalid==1)&&(rj_position.satellite_num!=0)){
		ret = 1;
    }

    return ret;
}

S8 RJ_GPS_get_satellite_num()
{
    return rj_position.satellite_num;
}

/*仅仅为了兼容*/
kal_uint8  Rj_GPS_Valid_Satelite_num(void)
{
    return RJ_GPS_get_satellite_num();
}

/*-------------------------------------------------------
                     GPS 参数初始化
-------------------------------------------------------*/
void RJ_GPS_Init(void)
{
        kal_prompt_trace( MOD_SOC,"[RJ_GPS_Gps_Init]");
        memset(&RJ_Gps_Info, 0, sizeof(RJ_Gps_User_Info));
        //memset(&rj_position, 0, sizeof(RJ_Gps_Position));
        //RJ_GPS_SetGpsModule(RJ_GPS_STATUS_ON);
}

/*-------------------------------------------------------
                     设置GPS 模块状态标识
                     1，运行RJ_GPS_STATUS_ON
                     0，休眠RJ_GPS_STATUS_OFF
-------------------------------------------------------*/
void RJ_GPS_SetGpsModule(U8 in)
{
        rj_gps_gps_module = in;
        //rj_position.satellite_num = 0;
}

/*-------------------------------------------------------
                      获取GPS 模块状态标识
                     1，运行RJ_GPS_STATUS_ON
                     0，休眠RJ_GPS_STATUS_OFF
-------------------------------------------------------*/
U8 RJ_GPS_GetGpsModule(void)
{
        return rj_gps_gps_module;
}

float bird_get_saltllite_cur(){
	return g_n_saltllite_snr_cur;
}
void bird_set_saltllite_cur(float n){
	g_n_saltllite_snr_cur=n;
}
void bird_set_saltllite_curnow(float n){
	g_n_saltllite_snr_curnow=n;
}
float bird_get_saltllite_curnow(){
	return g_n_saltllite_snr_curnow;
}

void bird_set_gps_snr_info(U8 * address )
{
    S8  num = nmaxsnr;    
     memcpy(address, (U8 *)&num, 1);
     num = nsnryxnum1;  	 
     memcpy(address+1, (U8 *)&num, 1);
     num = nsnryxnum2;  	 
     memcpy(address+2, (U8 *)&num, 1);
    *(address-2) = 0x17;
    *(address-1) = 0x03;
}

void bird_set_gps_vol_info(U8 * address )
{
    //U32 u = bird_get_curvoltage();
    //bird_set_U32_to_data(address,u);
    double dvol; 
    U32 nvol; 
	dvol = bird_get_adc_channel_voltage();
	nvol = (U32)(dvol*100);
	bird_set_U32_to_data(address,nvol);
    *(address-2) = 0x18;
    *(address-1) = 0x04;
}

void bird_set_heart_vol_info(U8 * address )
{
    double dvol; 
    U32 nvol; 
	dvol = bird_get_adc_channel_voltage();
	nvol = (U32)(dvol*100);
	bird_set_U32_to_data(address,nvol);
}
S8 bird_get_saltllite_snr_isvalid(){
	return g_n_saltllite_snr_isvalid;
}
S8 bird_get_saltllite_num(){
	S8  num = RJ_GPS_get_satellite_num();    
	if(num>9){
		num = 9;
	}
	return num;
}
S8 bird_heart_get_saltllite_num(){
	if(g_n_heart_saltlite_num>9)
	{
		g_n_heart_saltlite_num = 9;
	}
	return g_n_heart_saltlite_num;
}
void bird_heart_set_saltllite_num(S8 nnum){
	g_n_heart_saltlite_num = nnum;
}
S8 bird_get_gsm_strength(){
	
	S8 num =  (int)(bird_sim_signal_strength_in_percentage / 10.0+0.5);
	kal_prompt_trace(MOD_SOC,"bird_get_gsm_strength %d,%d",bird_sim_signal_strength_in_percentage,num); 
	if(num>9){
		num = 9;
	}
	return num;
}
kal_uint8 bird_get_gsm_strength_forat()
{
	return bird_get_gsm_strength();
}
S8 bird_get_gsm_strength100(){
	
	S8 num =  bird_sim_signal_strength_in_percentage;
	return num;
}
S8 bird_get_bat_num(){
	S8  num = srv_charbat_get_battery_level();   
	kal_prompt_trace(MOD_SOC,"bird_get_bat_num %d",num);
	if(num>9){
		num = 9;
	}
	return num;
}
S16 bird_get_vol()
{
    double dvol; 
    S16 nvol; 
	dvol = bird_get_adc_channel_voltage();
	nvol = (S16)(dvol*1000*1000);
	return nvol;
}


void bird_set_gps_able_satellite_num(U8 * address )
{
	S8  num = RJ_GPS_get_satellite_num();    
	if(num>9){
		num = 9;
	}		
     memcpy(address, (U8 *)&num, 1);
    *(address-2) = 0x16;
    *(address-1) = 0x01;
}


void bird_set_gsm_detail_data(U8 * address )
{
    memcpy(address, (U8 *)&bird_sim_signal_strength_in_percentage, 1);  
    //memcpy(address, (U8 *)&num, 1);	
    *(address-2) = 0x15;
    *(address-1) = 0x01;
}

void bird_set_battery_detail_data(U8 * address )
{
    S8  num = srv_charbat_get_battery_level();   
    memcpy(address, (U8 *)&num, 1);
    *(address-3) = 0x14;
    *(address-2) = 0x02;
}

kal_char* bird_get_snrinfo(){
	return gpsinfo;
}

void bird_set_gpspositon(S8 updatekind)
{
    if(RJ_GPS_is_working())
    {
		if(updatekind==1)
		{
			if( gps_position.ishavedate==0){
				if(rj_position.ishavedate==1){
					gps_position.la = rj_position.la;
					gps_position.lg = rj_position.lg;
					gps_position.al = rj_position.al;
					gps_position.di = rj_position.di;
					gps_position.east_west=rj_position.east_west;
					gps_position.north_south=rj_position.north_south;
					gps_position.satellite_num=rj_position.satellite_num;
					gps_position.satellite_num2=rj_position.satellite_num2;
					gps_position.sd=rj_position.sd;	
					gps_position.ishavedate = 1;
				}

			}
		}
		else if(updatekind==2){/*
			if(rj_position.ishavedate==1){			
				gps_position.la = rj_position.la;
				gps_position.lg = rj_position.lg;
				gps_position.al = rj_position.al;
				gps_position.di = rj_position.di;
				gps_position.east_west=rj_position.east_west;
				gps_position.north_south=rj_position.north_south;
				gps_position.satellite_num=rj_position.satellite_num;
				gps_position.satellite_num2=rj_position.satellite_num2;
				gps_position.sd=rj_position.sd;	
				gps_position.ishavedate = 2;
			}
			*/
			if(gps_position.ishavedate != 0)
			    gps_position.ishavedate = 2;
		}
		else{
			if(rj_position.ishavedate==1){			
				gps_position.la = rj_position.la;
				gps_position.lg = rj_position.lg;
				gps_position.al = rj_position.al;
				gps_position.di = rj_position.di;
				gps_position.east_west=rj_position.east_west;
				gps_position.north_south=rj_position.north_south;
				gps_position.satellite_num=rj_position.satellite_num;
				gps_position.satellite_num2=rj_position.satellite_num2;
				gps_position.sd=rj_position.sd;	
				gps_position.ishavedate = 1;
			}
		}
    }
	kal_prompt_trace(MOD_SOC,"gps_position.ishavedate =%d",gps_position.ishavedate);
}

void bird_ini_gpsposition()
{
	//S8 doublechar[40];
	
	//if(bird_get_pos_ishavedate()==0)
	{
	    //kal_prompt_trace(MOD_SOC,"bird_ini_gpsposition no position");
	    memset(&gps_position, 0, sizeof(gps_position));
	    gps_position.ishavedate=0;
	}
	/*
	else
	{
	    kal_prompt_trace(MOD_SOC,"bird_ini_gpsposition have position");
	    memset(&gps_position, 0, sizeof(gps_position));
	    gps_position.la=yd_tk001_info.pos_la;
	    gps_position.lg=yd_tk001_info.pos_lg;
	    gps_position.al=yd_tk001_info.pos_al;
	    gps_position.sd=yd_tk001_info.pos_sd;
	    gps_position.di=yd_tk001_info.pos_di;
	    gps_position.north_south=yd_tk001_info.pos_north_south;
	    gps_position.east_west=yd_tk001_info.pos_east_west;
	    gps_position.ishavedate=yd_tk001_info.pos_ishavedate;
		
	    memset(doublechar,0,sizeof(doublechar));
	    sprintf(doublechar,"%f", gps_position.la);
	    kal_prompt_trace(MOD_SOC, "bird_ini_gpsposition la=%s", doublechar);
	    memset(doublechar,0,sizeof(doublechar));
	    sprintf(doublechar,"%f", gps_position.lg);
	    kal_prompt_trace(MOD_SOC, "bird_ini_gpsposition lg=%s", doublechar);

	}
	*/
}

void bird_set_heartpositon()
{
	if(RJ_GPS_is_working()==1){
			heart_position.la = rj_position.la;
			heart_position.lg = rj_position.lg;
			heart_position.al = rj_position.al;
			heart_position.di = rj_position.di;
			heart_position.east_west=rj_position.east_west;
			heart_position.north_south=rj_position.north_south;
			heart_position.satellite_num=rj_position.satellite_num;
			heart_position.satellite_num2=rj_position.satellite_num2;
			heart_position.sd=rj_position.sd;	
			heart_position.ishavedate = 1;
	}		
}
void bird_ini_heartposition()
{
	memset(&heart_position, 0, sizeof(heart_position));
	heart_position.ishavedate=0;
	
}

/*-------------------------------------------------------
                     gga 数据回调
                     数据内容
                     fix_mode        定位维数
-------------------------------------------------------*/
void BIRD_Gps_restart(void)
{
	StopTimer(BIRD_GPS_RESTART_TIMER);
	kal_prompt_trace( MOD_SOC,"BIRD_Gps_restart");
	{
	    RJ_GET_AGPS(&agps_peph_op);
	    gps_restart_timer=0;
	}
}

static void RJ_GPS_nmea_txt_callback(mdi_gps_nmea_txt_struct *param) 
{
	mdi_gps_nmea_txt_struct *txt_data = param;
	kal_int8  i = 0;
	kal_int8 hed[] = {'H', 'E', 'D', '*', '1', '9', 0x0D, 0x0A, 0x00};
	kal_int8 ant[] = {'A', 'N', 'T', '_', 'O', 'K',  '*', '5', '3', 0x0D, 0x0A, 0x00};

	if ((txt_data->hed_0 == 0x02) && (txt_data->hed_1 == 0x01) && (txt_data->hed_2 == 0x02))
	{
		for (i = 0; i<sizeof(hed); i++) {
			if (hed[i] != txt_data->hed_string[i]) {
				kal_prompt_trace(MOD_SOC, "RJ_GPS_nmea_txt_callback no hed match");
				return;
			}
		}
		kal_prompt_trace(MOD_SOC, "RJ_GPS_nmea_txt_callback hed match");
		agps_peph_op.cold_start_ack_num++;
	}
	else if ((txt_data->hed_0 == 0x02) && (txt_data->hed_1 == 0x02) && (txt_data->hed_2 == 0x02))
	{
		for (i = 0; i<sizeof(ant); i++) {
			if (ant[i] != txt_data->hed_string[i]) {
				kal_prompt_trace(MOD_SOC, "RJ_GPS_nmea_txt_callback no ant match");
				return;
			}
		}
		kal_prompt_trace(MOD_SOC, "RJ_GPS_nmea_txt_callback ant match");
		agps_peph_op.cold_start_ack_num++;
	}
	else
	{
		kal_prompt_trace(MOD_SOC, "RJ_GPS_nmea_txt_callback no match");
	}
}

U8 g_pos_data_num=0;
U8 g_pos_data[9]={0};
U8 g_pos_data_30S[9*30]={0};
U8 g_gpscheck=0;
void RJ_reset_GPS_flag(void)
{
    g_n_saltllite_snr_isvalid = 0;
    rj_position.satellite_num = 0;
    gps_remove_timer=0;
    memset(&g_pos_data,0,sizeof(g_pos_data));
}

static void RJ_GPS_nmea_gga_callback(mdi_gps_nmea_gga_struct *param) 
{
#if BIRD_GPS_DEBUG
	S8 doublechar[40];
#endif
S8 fjchar1[16];
//S8 fjchar2[16];
       mdi_gps_nmea_gga_struct *gga_data = param;
	static BOOL b_GPS_led=KAL_FALSE;
       extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;

	kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback enter");	
	kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback gga_data->satellites =%d",gga_data->sat_in_view);	
       kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback gga_data->quality  =%d",gga_data->quality);
       kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback gga_data =%d %d",gga_data->east_west,gga_data->north_south);
       kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback gga_data->latitude  =%d %d",(U16)gga_data->latitude,gps_remove_timer);
	  
	  if((gga_data->quality==1|| gga_data->quality==2)&&(gga_data->sat_in_view!=0)&&((U16)(gga_data->latitude)!=0))
	  {
	  	gps_remove_timer++;
	  	if(gps_remove_timer>=2)
	  	{
	  	g_n_saltllite_snr_isvalid = 1;
	  	rj_position.satellite_num = gga_data->sat_in_view;
		rj_led_status_info.b_GPS_IS_SERACHING = KAL_FALSE ;
	  	}

#if BIRD_GPS_CLOSE_OPEN_TEST	
		if (bird_gps_close_open_flag == 0)
		{
			bird_gps_close_open_flag = 1;
			StartTimer(BIRD_GPS_CLOSE_TIMER, RJ_GPS_APP_15S, BIRD_Gps_Stop);
		}
#endif
	  }
	  else{
	  	g_n_saltllite_snr_isvalid = 0;
	  	rj_position.satellite_num = 0;
	  	gps_remove_timer=0;
		rj_led_status_info.b_GPS_IS_SERACHING = KAL_TRUE ;
	  }
	  if(RJ_GPS_is_working())
        {
             rj_position.la = gga_data->latitude;
             rj_position.lg = gga_data->longitude;
             rj_position.al = gga_data->altitude;
	      rj_position.east_west = gga_data->east_west;
	      rj_position.north_south= gga_data->north_south;
	      rj_position.ishavedate = 1;
#if BIRD_GPS_DEBUG
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f", gga_data->longitude);
		Bird_Log_print(MOD_SOC, "lg = %s,sa=%d", doublechar, gga_data->sat_in_view);
		memset(doublechar,0,sizeof(doublechar));
		sprintf(doublechar,"%f", gga_data->latitude);
		Bird_Log_print(MOD_SOC, "la = %s,ql=%d", doublechar, gga_data->quality);
#endif
            RJ_GPS_distance_handle(*gga_data);
            bird_set_agps_pos(gga_data->latitude, gga_data->longitude, gga_data->altitude);
			
		gps_restart_timer=0;
        }
	  else{
		gps_restart_timer++;
	  	kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback gps_restart_timer=%d",gps_restart_timer);	
		
		if(gps_restart_timer==120)
			StartTimer(BIRD_GPS_RESTART_TIMER, 100, BIRD_Gps_restart);

	  }
	 kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback g_n_saltllite_snr_isvalid=%d,rj_position.satellite_num= %d",g_n_saltllite_snr_isvalid,rj_position.satellite_num);	
	 g_gpscheck=1;
	 g_pos_data[0]=0x00;	 
	 g_pos_data[0]=(0x01&!RJ_GPS_is_working());	 
	 if(RJ_GPS_is_working())
	 {
	     g_pos_data[0]=(0x01&(rj_position.north_south!=78))<<1;	 
	     g_pos_data[0]=(0x01&(rj_position.east_west!=69))<<2;
		 kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback g_pos_data %d,%d,%d",rj_position.north_south,rj_position.east_west);
	 }
	 g_pos_data[1]=(U8)((U32)(rj_position.lg*1000000)/0x1000000);	 
	 g_pos_data[2]=(U8)(((U32)(rj_position.lg*1000000)/0x10000)%0x100);	 
	 g_pos_data[3]=(U8)(((U32)(rj_position.lg*1000000)/0x100)%0x100);	 
	 g_pos_data[4]=(U8)((U32)(rj_position.lg*1000000)%0x100);	 
	 g_pos_data[5]=(U8)((U32)(rj_position.la*1000000)/0x1000000);	 
	 g_pos_data[6]=(U8)(((U32)(rj_position.la*1000000)/0x10000)%0x100);	 
	 g_pos_data[7]=(U8)(((U32)(rj_position.la*1000000)/0x100)%0x100);	 
	 g_pos_data[8]=(U8)((U32)(rj_position.la*1000000)%0x100);	 
	 kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback g_pos_data %d,%d,%d,%d,%d",(U32)rj_position.lg,g_pos_data[1],
	 	g_pos_data[2],g_pos_data[3],g_pos_data[4]);	

	 memcpy(&g_pos_data_30S[g_pos_data_num],&g_pos_data,9);
	 g_pos_data_num++;
	 if(g_pos_data_num >= 30)
	     g_pos_data_num = 0;

	  bird_set_saltllite_curnow(gga_data->h_precision);
	   
	   //memset(fjchar1,0,sizeof(fjchar1));
	   //sprintf(fjchar1,"%f",gga_data->h_precision);
	   //kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback quality,cur %d,%s",gga_data->quality,fjchar1);

	   /*
		memset(fjchar2,0,sizeof(fjchar2));
	   sprintf(fjchar2,"%f",gga_data->longitude);
	   
	   kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gga_callback %s,%s",fjchar1,fjchar2);
	   */
        /*
        if(!b_GPS_led)
               RJ_GPS_Gpsled_LightWink();

	    b_GPS_led = KAL_TRUE;
	 */
}

static void RJ_GPS_nmea_gll_callback(mdi_gps_nmea_gll_struct *param)
{
       mdi_gps_nmea_gll_struct *gll_data = param;
        kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_gll_callback %d",gll_data->status);
        
}

/*----------------------------------------------------------------------------------------------------
		计算两坐标点间球面距离，lat1,lat2 为两点纬度，lng1,lng2 为两点经度
		返回两点球面距离(单位: km)
-----------------------------------------------------------------------------------------------------*/
#define RJ_GPS_EARTH_RADIUS	6378.137;
#define RJ_GPS_PI	3.141593

double RJ_GPS_rad(double d)
{
	double in;

	in = (d * RJ_GPS_PI ) / 180.0;


	return in;
}

S32 RJ_GPS_GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double s ; 
	S32 out;
	//S8 str[20]; 

	double radLat1;
	double radLat2;
	double a;
	double b;

       if((lat1==lat2) && (lng1==lng2)) return 0;
       
	radLat1 = RJ_GPS_rad(lat1);
	radLat2 = RJ_GPS_rad(lat2);
	a = radLat1 - radLat2;
	b = RJ_GPS_rad(lng1) - RJ_GPS_rad(lng2);

	s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
		
	s = s *1000 * RJ_GPS_EARTH_RADIUS ;
		
	//out = round(s * 10000);
	out = (S32)floor(s);
	
	return out;
}

double BIRD_GPS_GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double s ; 
	S32 out;
	//S8 str[20]; 

	double radLat1;
	double radLat2;
	double a;
	double b;

       if((lat1==lat2) && (lng1==lng2)) return 0;
       
	radLat1 = RJ_GPS_rad(lat1);
	radLat2 = RJ_GPS_rad(lat2);
	a = radLat1 - radLat2;
	b = RJ_GPS_rad(lng1) - RJ_GPS_rad(lng2);

	s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
		
	s = s *1000 * RJ_GPS_EARTH_RADIUS ;
		
	//out = round(s * 10000);
	//out = (S32)floor(s);
	
	return s;
}
/*-------------------------------------------------------
                     rmc 数据回调
                     数据内容
                     satellite_num            卫星数
                     latitude                     纬度
                     longitude                   经度
                     utc_time utc_date      格林威治时间
-------------------------------------------------------*/
static void RJ_GPS_nmea_rmc_callback(mdi_gps_nmea_rmc_struct *param)
{

        mdi_gps_nmea_rmc_struct *rmc_data = param;
        U32 dis =0;
		 kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_rmc_callback status,navstatus: %d,%d",rmc_data->status,rmc_data->nav_status);
    /*    
	  if(rmc_data->status==65)
	  {
	  	g_n_saltllite_snr_isvalid = 1;
	  }
	  else{
	  	g_n_saltllite_snr_isvalid = 0;
	  }
	  
        if(RJ_GPS_is_working())
        {
            rj_position.la = rmc_data->latitude;
            rj_position.lg = rmc_data->longitude;
	      rj_position.east_west = rmc_data->east_west;
	     rj_position.north_south= rmc_data->north_south;
            RJ_GPS_distance_handle(*rmc_data);
        }

	 
*/	  
}

kal_int16 getMinAngle(float a0,float a1){
       kal_int16 angle = 0;
	kal_int16 na0,na1;
	na0 = floor(a0);
	na1 = floor(a1);
       if(na1 >= 270 && na0 < 90)
	{
        	angle = (na1 - (na0 + 360)) % 180;
        }
	 else if (na1 <= 90 && na0 >= 270) 
        {
                angle = (na1 + 360 - na0) % 180;
        }
	 else {
              angle = (na1 - na0);
              if (abs(angle) > 180)
		{
                   angle -= 360;    
               }
        }
        return abs(angle);
}

/*-------------------------------------------------------
                     gsv 数据回调
                     数据内容
                     ground_speed        速度 km/h
                     drection                 方向 -真北参照系
-------------------------------------------------------*/
static void RJ_GPS_nmea_vtg_callback(mdi_gps_nmea_vtg_struct *param)
{
	U8 nrunstatus = 0;
	kal_int16 disdi=0.0;
	U8 ncorner;
        mdi_gps_nmea_vtg_struct *vtg_data = param;

        if(vtg_data->hspeed_km < 300&&RJ_GPS_is_working())
        {
            rj_position.sd = vtg_data->hspeed_km; 
            rj_position.di   = vtg_data->true_heading;   
	     ncorner = bird_get_corner_value();
	     kal_prompt_trace(MOD_SOC," RJ_GPS_nmea_vtg_callback %d",bird_get_corner_value());
	     #if 1
	     if(ncorner>0){
		     nrunstatus = Yd_get_equstatus();
		     if(nrunstatus==1)
		     	{
		     		if(rj_position.sd >8){
					if(gps_position.ishavedate==1)
					{
						disdi = getMinAngle(rj_position.di,gps_position.di);
						if(disdi>ncorner)
						{
					//		if(Yd_get_restartflag()==0){
								bird_set_gpspositon(0);
					/*		}
							else{
								bird_set_gpspositon(2);
							}*/
							//发位置T3
	                     			bird_soc_sendpos();
						}
					}
					
		     		}
		     	}
	     }	    
	     #endif
            #ifndef BIRD_ECU_MMI_SUPPORT	
            RJ_GPS_speed_handle(vtg_data->hspeed_km);
            #endif
        }
        else
        {
            /* 卫星数为0，啥也不干 */
            //alarm_speed_times = 0;
            kal_prompt_trace( MOD_WPS,"satellite_num is 0");
        }
}

/*-------------------------------------------------------
                     gsa 数据回调
                     数据内容
                     fix_mode        定位维数
-------------------------------------------------------*/
static void RJ_GPS_nmea_gsa_callback(mdi_gps_nmea_gsa_struct *param)
{
        mdi_gps_nmea_gsa_struct *gsa_data = param;

        #ifdef FOR_MTK_GPS_TEST /* for MTK GPS restart test .add by huangbx */
                //kal_prompt_trace( MOD_WPS,"RJ_GPS_nmea_gsa_callback fix_mode = %d", gsa_data->fix_mode);

                /* 定位维数, 3 维定位才准确 */
                if(gsa_data->fix_mode == '3')                           /*asc 49 = 1*/
                {
                    //RJ_GPS_close_gps();
                }

        #endif
}

/*-------------------------------------------------------
                     gsv 数据回调
                     数据内容
                     sate_id        卫星ID
                     snr              信噪比
-------------------------------------------------------*/
static void RJ_GPS_nmea_gsv_callback(mdi_gps_nmea_gsv_struct *param)
{
        mdi_gps_nmea_gsv_struct * gsv_data = param;
        U8 m;
	 U8	n;
	 U8 i,j,snrindex;
	 U8 n2;
	 S8 fjchar[8];
	 S8 dt[1];
	 kal_uint8 sal[12];
	 kal_uint8 snr[12];
	 kal_uint8 snrtemp,saltemp;
	 U8 nindex;
        dt[0] = ':';
        memset(engineer_satinfo, 0, sizeof(engineer_satinfo));
	  memset(gpsinfo,0,sizeof(gpsinfo));
	  memset(sal,0,sizeof(sal));
	  memset(snr,0,sizeof(snr));
	  n2 = 0;
        nindex = 0;
		/*
       if(gsv_data->max_snr>=30)
	{
		g_n_saltllite_snr_isvalid = 1;
	}
	else{
		g_n_saltllite_snr_isvalid = 0;
	}
		*/
	//kal_prompt_trace(MOD_USB,"RJ_GPS_nmea_gga_callback maxsnr = %d ",gsv_data->max_snr);
        for(m=0;m<12;m++)
        {
        	snr[m]= gsv_data->rsv[m].snr;
		sal[m]= gsv_data->rsv[m].sate_id;
		/*
        	if(m < MDI_GPS_NMEA_MAX_SVVIEW)
        	{
        		memset(fjchar,0,sizeof(fjchar));
			if(gsv_data->rsv[m].sate_id<10){
				fjchar[0] = '0';
				sprintf(fjchar+1,"%d",gsv_data->rsv[m].sate_id);
			}
			else{
				sprintf(fjchar,"%d",gsv_data->rsv[m].sate_id);
			}
			memcpy(gpsinfo+n2, fjchar, 2);
			n2 = n2 + 2;
			
			memset(fjchar,0,sizeof(fjchar));
			if(gsv_data->rsv[m].snr<10){
				fjchar[0] = '0';
				sprintf(fjchar+1,"%d",gsv_data->rsv[m].snr);
			}
			else{
				sprintf(fjchar,"%d",gsv_data->rsv[m].snr);
			}
			memcpy(gpsinfo+n2, fjchar, 2);
			n2 = n2 + 2;
			if(m<11)
			{			
				memcpy(gpsinfo+n2, (S8 *)dt, 1);
				n2 = n2 + 1;
			}
        	}
		else{
			memset(fjchar,0,sizeof(fjchar));
			fjchar[0] = '0';
			fjchar[1] = '0';
			memcpy(gpsinfo+n2, fjchar, 2);
			n2 = n2 + 2;
			memcpy(gpsinfo+n2, fjchar, 2);
			n2 = n2 + 2;
			if(m<11)
			{			
				memcpy(gpsinfo+n2, (S8 *)dt, 1);
				n2 = n2 + 1;
			}			
		}
		n = n + 1;
		*/
       }
		/*
	for(i=0;i<12-1;i++)
       {
       	kal_prompt_trace( MOD_USB, "RJ_GPS_nmea_gsv_callback sal,snr %d,%d",sal[i],snr[i]);
	}
	*/
       for(i=0;i<12-1;i++)
       {
       	snrtemp = snr[i];
		snrindex = i;
       	for(j=i+1;j<12;j++)
       	{
       		if(snrtemp<snr[j])
       		{
       			snrtemp = snr[j];
				snrindex = j;
       		}
       	}
		if(snrindex!=i)
		{
			snrtemp = snr[snrindex];
			snr[snrindex] = snr[i];
			snr[i] = snrtemp;

			saltemp = sal[snrindex];
			sal[snrindex] = sal[i];
			sal[i] = saltemp;
		}
       }
       for(i=0;i<12;i++)
       {
        	memset(fjchar,0,sizeof(fjchar));
		if(sal[i]<10){
			fjchar[0] = '0';
			sprintf(fjchar+1,"%d",sal[i]);
		}
		else{
			sprintf(fjchar,"%d",sal[i]);
		}
		memcpy(gpsinfo+n2, fjchar, 2);
		n2 = n2 + 2;
			
		memset(fjchar,0,sizeof(fjchar));
		if(snr[i]<10){
			fjchar[0] = '0';
			sprintf(fjchar+1,"%d",snr[i]);
		}
		else{
			sprintf(fjchar,"%d",snr[i]);
		}
		memcpy(gpsinfo+n2, fjchar, 2);
		n2 = n2 + 2;
		if(i<11)
		{			
			memcpy(gpsinfo+n2, (S8 *)dt, 1);
			n2 = n2 + 1;
		}       	
       }
	/*   
	memcpy(gpsinfo+n2, (S8 *)dt, 1);
	n2 = n2 + 1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%f",bird_get_saltllite_curnow());
	memcpy(gpsinfo+n2, fjchar, sizeof(fjchar));
	n2 = n2 + sizeof(fjchar);
	
	memcpy(gpsinfo+n2, (S8 *)dt, 1);
	n2 = n2 + 1;
	
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",bird_get_saltllite_snr_isvalid());
	memcpy(gpsinfo+n2, fjchar, sizeof(fjchar));
	*/
	  // rj_position.satellite_num = gsv_data->num_sv_trk;
	kal_prompt_trace( MOD_SOC, "RJ_GPS_nmea_gsv_callback SNR=%s,x=%d,snrsalnum=%d,num_sv_trk=%d", gpsinfo,RJ_GPS_get_satellite_num(),gsv_data->sates_in_view,gsv_data->num_sv_trk);	
	//nsnryxnum1 = n;
	//nsnryxnum2 = n2;

}

/*-------------------------------------------------------
                     GPS 数据回调
-------------------------------------------------------*/
static void RJ_GPS_nmea_string_callback(const U8 *buffer, U32 length)
{
       
}


/*-------------------------------------------------------
                     GPS 数据回调
-------------------------------------------------------*/
void RJ_GPS_Callback(mdi_gps_parser_info_enum type, void *buffer, U32 length)
{
    /*
        applib_time_struct current_time;
        applib_dt_get_date_time(&current_time);
        kal_prompt_trace( MOD_WPS, "RJ_GPS_Callback %d:%d:%d:%d:%d:%d" ,
        current_time.nYear,  current_time.nMonth, current_time.nDay, current_time.nHour, current_time.nMin, current_time.nSec); 
        */
        //RJ_GPS_log( "RJ_GPS_Callback");
        
	switch(type)
	{
		case MDI_GPS_PARSER_NMEA_GGA:
			RJ_GPS_nmea_gga_callback(buffer);
			break;
		case MDI_GPS_PARSER_NMEA_RMC:
			RJ_GPS_nmea_rmc_callback(buffer);
			break;
		case MDI_GPS_PARSER_NMEA_VTG:
			RJ_GPS_nmea_vtg_callback(buffer);
			break;
		
		case MDI_GPS_PARSER_NMEA_GSA:
			RJ_GPS_nmea_gsa_callback(buffer);
			break;
                #if defined(__AGPS_USER_PLANE__)
            case MDI_GPS_PARSER_MA_STATUS:
                    // if (RJ_Gps_Info)
                    //TRACE_P_GPS("MDI_GPS_PARSER_MA_STATUS");
                    kal_prompt_trace( MOD_USB,"MDI_GPS_PARSER_MA_STATUS");
                        memcpy(&RJ_Gps_Info.minigps_ma_position, buffer, sizeof(supl_mmi_position_struct));
                    break;
            #endif

		case MDI_GPS_PARSER_NMEA_GSV:
			RJ_GPS_nmea_gsv_callback(buffer);
			break;
		case MDI_GPS_PARSER_RAW_DATA:
			RJ_GPS_nmea_string_callback(buffer, length);
			break;      
		case MDI_GPS_PARSER_NMEA_GLL:
			RJ_GPS_nmea_gll_callback(buffer);
			break;    	
		case MDI_GPS_PARSER_NMEA_TXT:
			RJ_GPS_nmea_txt_callback(buffer);
			break;    				
	}
}

/*-------------------------------------------------------
                      启动GPS功能，冷启动
-------------------------------------------------------*/
extern void bird_gps_exit_main(void);
extern void bird_gps_enter_main(void);
extern void mmi_em_dev_gps_minigps_minigps_exit(void);
extern void mmi_em_dev_minigps_minigps_init();

void Gps_get_handle()
{
	gps_signal_handle= L1SM_GetHandle();
}
//#define GPS_TEST
#ifdef BIRD_ECU_SUPPORT
U8 gps_bird_disable_sleep_status = 0;
U8 m_gps_status = 0; //0 close 1 open
extern kal_bool  b_acc_gpio_status;

void Bird_SleepDisable()
{
      kal_prompt_trace( MOD_SOC,"Bird_SleepDisable gps,acc,flag = %d,%d,%d",m_gps_status,b_acc_gpio_status,gps_bird_disable_sleep_status);
      if(gps_bird_disable_sleep_status==0)
	{
		gps_bird_disable_sleep_status = 1;
		L1SM_SleepDisable(gps_signal_handle);  
      	}
}

void Bird_SleepEnable()
{
       kal_prompt_trace( MOD_SOC,"Bird_SleepEnable gps,acc,flag = %d,%d,%d",m_gps_status,b_acc_gpio_status,gps_bird_disable_sleep_status);
	if(m_gps_status==0 && b_acc_gpio_status ==KAL_FALSE)
	{
	      if(gps_bird_disable_sleep_status==1)
		{
			gps_bird_disable_sleep_status = 0;
			L1SM_SleepEnable(gps_signal_handle);  
	      	}
	}
}
#endif

void RJ_GPS_Enter(void)
{
	applib_time_struct currtent_time={0};
#if BIRD_GPS_DEBUG	
	Bird_Log_print(MOD_SOC, "RJ_GPS_Enter");
#endif
	RJ_GPS_SetGpsModule(RJ_GPS_STATUS_ON);
/*yanchunhai  20130108 add for exit sleep mode begin*/
       #ifdef MTK_SLEEP_ENABLE	
#ifdef BIRD_ECU_SUPPORT
	m_gps_status=1;
	Bird_SleepDisable();
#else
	//gps_signal_handle= L1SM_GetHandle();
	L1SM_SleepDisable(gps_signal_handle);  
#endif
  	 #endif
/*yanchunhai  20130108 add for exit sleep mode end*/
	 // open bd or gps by lqy2014-11-20
	BD_UART_INIT();
#ifdef BD_HDDZ
	RJ_GET_AGPS(&agps_peph_op);  
#endif
#if BIRD_GPS_CLOSE_OPEN_TEST		
	StopTimer(BIRD_GPS_OPEN_TIMER);
	bird_gps_close_open_flag = 0;
#endif
	rj_led_status_info.b_GPS_IS_SERACHING = KAL_TRUE;
	rj_led_status_info.b_GPS_IS_CLOSE= KAL_FALSE;
	gps_restart_timer=0;
	gps_remove_timer=0;
}

/*-------------------------------------------------------
                     开启GPS ，热启动          
-------------------------------------------------------*/
void RJ_GPS_Restart(void)
{
        MDI_RESULT ret;

        kal_prompt_trace( MOD_WPS,"RJ_GPS_Restart");
        TRACE_P_GPS( "RJ_GPS_Restart");
        RJ_GPS_SetGpsModule(RJ_GPS_STATUS_ON);
	   #ifdef MTK_SLEEP_ENABLE
	   #ifdef BIRD_ECU_SUPPORT
	   Bird_SleepDisable();
	   #else
	   L1SM_SleepDisable(gps_signal_handle);
	   #endif
	   #endif

         // open bd or gps by lqy2014-11-20
         BD_UART_INIT();


        //RJ_GPS_Gpsled_LightWink();
}  

/*-------------------------------------------------------
           GPS 从休眠中启动
-------------------------------------------------------*/
void RJ_GPS_AT_OPEN()
{
	if(RJ_GPS_GetGpsModule()==0)
      {
		RJ_GPS_Enter();//冷启动GPS  lrf modify
	}
}
void RJ_GPS_GpsRestart()
{
        //RJ_GPS_record_date(L"GPSstatus.dat", "GPS POWER ON" , KAL_FALSE);   // record power on off time
        kal_prompt_trace( MOD_WPS,"RJ_GPS_GpsRestart");
        RJ_GPS_SetGpsModule(RJ_GPS_STATUS_ON);
        RJ_GPS_Restart();
}

void RJ_GPS_Exit_GPSTest()
{
	rj_gps_for_at_test =0;
#if defined(__GPS_SUPPORT__) || defined(__BT_GPS_SUPPORT__)
	mdi_gps_uart_close(0, MDI_GPS_UART_MODE_LOCATION, RJ_GPS_Callback);
#endif
}



/*-------------------------------------------------------
                            关闭GPS       
-------------------------------------------------------*/
void RJ_GPS_Stop(void)
{
        MDI_RESULT ret;

        TRACE_UART("RJ_GPS_Stop");
#if BIRD_GPS_DEBUG		
        Bird_Log_print(MOD_SOC, "RJ_GPS_Stop");
#endif
	  RJ_GPS_SetGpsModule(RJ_GPS_STATUS_OFF);
        kal_prompt_trace( MOD_WPS,"RJ_GPS_Stop");
#ifdef HD8020_GPS_SUPPORT	 
	 gps_close_location_flag = g_n_saltllite_snr_isvalid;
	 applib_dt_get_rtc_time(&agps_peph_op.gps_close_time); 
#endif

	//stop bd or gps	by lqy2014-11-20
       BD_UART_CLOSE();  

#ifdef MTK_SLEEP_ENABLE
#ifdef BIRD_ECU_SUPPORT
	 m_gps_status = 0;	
   	 Bird_SleepEnable();
#else
	 L1SM_SleepEnable(gps_signal_handle);     
#endif
	    //L1SM_SleepEnable(bmt_sm_handle);     
#endif
        RJ_GPS_LED1Light(0);
        memset(&rj_position, 0, sizeof(rj_position));
        g_n_saltllite_snr_isvalid = 0;
        yd_tk001_set_record_from_NV();
	rj_led_status_info.b_GPS_IS_SERACHING = KAL_FALSE;
	rj_led_status_info.b_GPS_IS_CLOSE= KAL_TRUE;	
	g_gpscheck=0;
	StopTimer(BIRD_GPS_COLD_RESTART_TIMER);
}

void BIRD_Gps_Stop(void)
{
	kal_prompt_trace( MOD_SOC,"BIRD_Gps_Stop");
	StopTimer(BIRD_GPS_CLOSE_TIMER);
	RJ_GPS_Stop();
	StartTimer(BIRD_GPS_OPEN_TIMER, RJ_GPS_APP_1M, RJ_GPS_Enter);
}

/*-------------------------------------------------------
           GPS 进入休眠状态
-------------------------------------------------------*/
void RJ_GPS_GpsSleep()
{
        //RJ_GPS_record_date(L"GPSstatus.dat", "GPS POWER OFF" , KAL_FALSE);   
        kal_prompt_trace( MOD_WPS,"RJ_GPS_GpsSleep");
        RJ_GPS_SetGpsModule(RJ_GPS_STATUS_OFF);
        RJ_GPS_Stop();		
        //memset(&rj_position, 0, sizeof(RJ_Gps_Position));
}

#endif
