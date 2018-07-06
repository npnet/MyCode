#include "MMIDataType.h"
#include "Syscomp_config.h"
#include "Smslib_def.h"
#include "Bird_std.h"


extern void Bird_start_();
extern void RJ_SYS_ReStart();
extern kal_uint32 bird_get_bat_channel_voltage(void);	
extern void rj_low_volt_alarm(void);
extern U32 bird_get_bvl();
extern void Bird_set_reseterr(U8 n_err_type);
extern void Bird_set_resttime();
extern kal_bool bird_second_task_create(comptask_handler_struct **handle);
extern kal_bool bird_task_create(comptask_handler_struct **handle);
extern void bird_set_SMS_codetype(U8 type);
extern U8 yd_tk001_all_Msg(smslib_general_struct* data);
extern void Bird_Get_OTA_Version(kal_uint8 *ver);
extern  void BD_UART_READ_HANDLE(kal_uint8 *Buffaddr, kal_uint16 Length);
extern  void ECU_UART_READ_HANDLE(kal_uint8 *Buffaddr, kal_uint16 Length);
extern void rj_outside_volt_connect(void);
extern void rj_outside_volt_disconnect(void);
extern kal_bool __custom_command_hdlr(char *full_cmd_string);


#pragma arm section rwdata = "TBOX_START_EXTSRAM_RW", zidata = "TBOX_START_EXTSRAM_RW"

kal_bool b_charge_usb_in;
U8 g_signal_flag=0;
U8 g_sim_count_signal=0;
kal_bool b_SIM_IS_OK = KAL_FALSE;
RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
kal_bool b_sim_is_available = KAL_FALSE;
kal_bool b_service_is_available = KAL_FALSE;
    		

void Bird_start()
{
	Bird_start_();
}

void RJ_SYS_ReStart__()
{
	RJ_SYS_ReStart();
}

kal_uint32 bird_get_bat_channel_voltage__(void)
{
	return(bird_get_bat_channel_voltage());
}
void rj_low_volt_alarm__(void)
{
	rj_low_volt_alarm();
}
U32 bird_get_bvl__()
{
	return(bird_get_bvl());
}

void Bird_set_reseterr__(U8 n_err_type)
{
	Bird_set_reseterr(n_err_type);
	
}    
	
void Bird_set_resttime__()
{
	Bird_set_resttime();
}

kal_bool bird_second_task_create__(comptask_handler_struct **handle)
{

	return(bird_second_task_create(handle));
}

kal_bool bird_task_create__(comptask_handler_struct **handle)
{
	return(bird_task_create(handle));
}
void bird_set_SMS_codetype__(U8 type)
{
	bird_set_SMS_codetype(type);
}

U8 yd_tk001_all_Msg__(smslib_general_struct* data)
{
	return(yd_tk001_all_Msg(data));
}
void Bird_Get_OTA_Version__(kal_uint8 *ver)
{
	Bird_Get_OTA_Version(ver);
}
void BD_UART_READ_HANDLE__(kal_uint8 *Buffaddr, kal_uint16 Length)
{
	BD_UART_READ_HANDLE(Buffaddr, Length);
}
void ECU_UART_READ_HANDLE__(kal_uint8 *Buffaddr, kal_uint16 Length)
{
	ECU_UART_READ_HANDLE(Buffaddr, Length);
}

kal_bool custom_command_hdlr(char *full_cmd_string)
{
	return(__custom_command_hdlr(full_cmd_string));
}

void rj_outside_volt_connect__(void)
{
	rj_outside_volt_connect();
}


void rj_outside_volt_disconnect__(void)
{
	rj_outside_volt_disconnect();
}


//Please do NOT modify any content above!

///



