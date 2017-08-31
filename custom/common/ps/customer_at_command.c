/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 * Filename:
 * ---------
 * Custom_at_command.c
 *
 * Description:
 * ------------
 *   This file is intends for Customer to implement and parse its own defined AT commands
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
*****************************************************************************/
//#include "kal_release.h"
//#include "kal_non_specific_general_types.h"
#include "stdio.h"
#include "string.h"
//#include "stack_config.h"	

#include "kal_general_types.h"
#include "kal_internal_api.h"
#include "ata_external.h"
#if (defined(__COMPOSITE_WEBCAM__))
#include "cal_api.h"
#include "cal_comm_def.h"
#endif
#ifdef __TWOMICNR_SUPPORT__
#include "two_mic_NR_custom_if.h" 
#endif

#if defined(__PLMN_LIST_WITH_CELL_INFO__)
#include "ps_public_struct.h"
#endif
#include "bird_app.h"
extern void kal_monitor_buffer(kal_uint32 index);
extern kal_bool twomicnr_command_hdlr(char *full_cmd_string);

/*****************************************************************************
* CUSTOM_SYMBOL:
*	customer can modify this symbol.
*     Every command begin with this symbol will be recognized as a customer-defined command
*	and passed to custom_command_hdlr().
*     For example: if CUSTOM_SYMBOL is set to '^', 
*				then AT^AAAA, AT^BBBB will be customer-defined command.
*   	NOTE:  '+'  and '/' and ' \ 'is not allowed since they might be standard command format
*****************************************************************************/
#define CUSTOM_SYMBOL  '^'	 // '+'  and '/' and ' \ 'is NOT allowed   

/*****************************************************************************
* MAX_UART_LEN = 128 bytes.
*	when you write a lot of data to UART at the same time, 
*	UART's buffer might be full and cannot handle.
*     so the safer way is to seperate the long data, each time only write data length <= 128
*****************************************************************************/

kal_uint8 custom_get_atcmd_symbol(void);
kal_bool custom_command_hdlr(char *full_cmd_string);

/*****************************************************************************
* FUNCTION
*  	rmmi_write_to_uart()
* DESCRIPTION
*   	This function extern L4C RMMI function,
*     is to write data to UART
* PARAMETERS
*   	kal_uint8 * buffer 	- the data to be written
*	kal_uint16 length 	- the length of data to be written
*     kal_bool stuff 		- KAL_TRUE : <CR><LF> will be added at the beginning and end. (eg.<CR><LF>OK<CR><LF>)
*					   KAL_FALSE: no <CR><LF> is added
* RETURNS
*	none
*****************************************************************************/
extern void rmmi_write_to_uart(kal_uint8 * buffer, kal_uint16 length, kal_bool stuff); 

#ifdef __GATI_ENABLE__
extern kal_bool gati_command_hdlr(kal_uint8* cmd_string, kal_uint8* full_string);

module_type gati_mod_table[] = 
{
    MOD_PHB,
    MOD_L4C,
    MOD_L4C,
    MOD_L4C, 
    MOD_NIL         /* Need to be the last one */
};

kal_char *gati_cmd_table[] = 
{
    "CPBBA",
    "CPBBR",
    "CPBBS",
    "CPBB"
};

#endif

#ifdef __GAS_SUPPORT__
extern kal_bool gas_command_hdlr(kal_uint8* cmd_string, kal_uint8* full_string);

module_type gas_mod_table[] = 
{
#ifdef __TCPIP__
    MOD_ABM,
#endif
    MOD_L4C,
    MOD_CSM,
#if defined(__BTMTK__)
    MOD_BT,
#endif
    MOD_NIL         /* Need to be the last one */
};

kal_char *gas_cmd_table[] = 
{
#ifdef __TCPIP__
    "EWIFI",
#endif
    "CL4C",
    "CCSM"
#if defined(__BTMTK__)
    ,"EBTAT"
#endif
};


#endif


typedef struct
{
	char *commandString;
	custom_rsp_type_enum (*commandFunc)(custom_cmdLine *commandBuffer_p);
} custom_atcmd;

custom_cmd_mode_enum custom_find_cmd_mode(custom_cmdLine *cmd_line);

custom_rsp_type_enum custom_test_func(custom_cmdLine *commandBuffer_p)
{
    custom_cmd_mode_enum result;
    custom_rsp_type_enum ret_value  = CUSTOM_RSP_ERROR;

    result = custom_find_cmd_mode(commandBuffer_p);		
    switch (result)
    {
        case CUSTOM_READ_MODE:
            ret_value = CUSTOM_RSP_OK;
            break;
        case CUSTOM_ACTIVE_MODE:
            ret_value = CUSTOM_RSP_OK;
            break;
        case CUSTOM_SET_OR_EXECUTE_MODE:
            ret_value = CUSTOM_RSP_OK;
            break;
        case CUSTOM_TEST_MODE:
            ret_value = CUSTOM_RSP_OK;
            break;    
        default:
            ret_value = CUSTOM_RSP_ERROR;
            break;
	}
    return ret_value;
}


/********************************
* The following table is used for the customer
* They can defined their own command and 
* corresponding handler function here
* Note that the last line {NULL, NULL} should not be removed
* since it is the terminator indicator
*********************************/
const custom_atcmd custom_cmd_table[ ] =
{    
    {"AT%CUSTOM",custom_test_func},

#if defined(__ATA_SUPPORT__) && defined(__MTK_TARGET__)
	#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__)|| defined(__MSDC_MSPRO__)) || defined(__MSDC_SD_SDIO)
						{"AT^MSDC",ata_t_card_hdlr_v1}, 		  //T Card
	#endif
				
						{"AT+EMVI",ata_vibrator_hdlr_v1},			  //Vibrator
				
	#if (defined(FM_RADIO_ENABLE))	
						{"AT+EMFM",ata_fm_hdlr_v1}, 			  //FM
	#endif
				
	#if defined(TOUCH_PANEL_SUPPORT) && defined(TOUCH_PANEL_INTERNAL)
						{"AT+ETPID",ata_tp_hdlr_v1},			  //TouchPanel
	#endif
				
	#if defined(ISP_SUPPORT)
						{"AT+ECMFP",ata_camera_hdlr_v1},		  //camera
	#endif
				
	#if defined(__ATV_SUPPORT__)
						{"AT+EMATV",ata_matv_hdlr_v1},				//Matv
						{"AT+EMATVP",ata_matv_video_hdlr_v1},				//Matv video	
	#endif
				
	#if defined(__CUST_NEW__)
						{"AT+EKPT",ata_kpt_hdlr_v1},			  //Keypad Test
	#endif
				
						{"AT+EMAUDIO",ata_audio_hdlr_v1},			  //Audio
						{"AT+ELED",ata_led_hdlr},			   //LED
						{"AT+ELEDT",ata_led_tsem_hdlr},		   //LED take sem
						{"AT+ELEDG",ata_led_gsem_hdlr},		   //LED give sem
						{"AT+ELCM",ata_lcm_hdlr},			   //LCM
						{"AT+EMBKL",ata_bkl_hdlr},			   //Backlight
#endif/*__ATA_SUPPORT__ && __MTK_TARGET__*/

    {NULL, NULL}  // this lind should not be removed, it will be treat as 
};

/*****************************************************************************
* FUNCTION
*     custom_find_cmd_mode()
* DESCRIPTION
*     This function is used to parsing the command mode for the customer
*     The input position of the cmd_line should stay at the character right after the command name
* PARAMETERS
*     cmd_line      the custom_cmdLine which contains the command string and its current parsing position
* RETURNS
*     custom_cmd_mode_enum
*****************************************************************************/
custom_cmd_mode_enum custom_find_cmd_mode(custom_cmdLine *cmd_line)
{
    custom_cmd_mode_enum result;

    if (cmd_line->position < cmd_line->length - 1)
    {
        switch (cmd_line->character[cmd_line->position])
        {
            case '?':  /* AT+...? */
            {
                cmd_line->position++;
                result = CUSTOM_READ_MODE;
                break;
            }
            case '=':  /* AT+...= */
            {
                cmd_line->position++;
                if ((cmd_line->position < cmd_line->length - 1 ) &&
                    (cmd_line->character[cmd_line->position] == '?'))
                {
                    cmd_line->position++;
                    result = CUSTOM_TEST_MODE;
                }
                else
                {
                    result = CUSTOM_SET_OR_EXECUTE_MODE;
                }
                break;
            }
            default:  /* AT+... */
            {
                result = CUSTOM_ACTIVE_MODE;
                break;
            }
        }
    }
    else
    {
        result = CUSTOM_ACTIVE_MODE;
    }
    return (result);
}

#ifdef RJ_GPS_APP
#include "common_nvram_editor_data_item.h"
extern kal_uint16 rj_gps_for_at_test;
extern void RJ_GPS_Exit_GPSTest();

#endif
/*add by jiazhaoran for gps at command*/
#ifndef BIRD_EXTERNEL_POWER_UNSUPPORT	
extern double bird_get_adc_channel_voltage();
#endif
#ifdef BIRD_UART2_SUPPORT
extern kal_uint8 g_m_readuartdata;	 
#endif
extern kal_int8 acc_sensor_get_ini();
extern kal_uint8 bird_get_gsm_strength_forat();

#ifdef BIRD_BT_SUPPORT
extern void send_device_num_to_BT(void);
extern kal_uint8 AT_test_flag;
extern kal_uint8 mac_data[];
#endif
extern kal_uint8 RJ_GPS_GetSimCardStatus();
volatile kal_uint8 watchdog_at_flag =1;
kal_bool watchdog_state_test = 0 ; //0: LEVEL_LOW; 1: LEVEL_HIGH
kal_uint8 reset_flag = 0;
kal_uint8 watchdog_testcount =0;
void bird_watchdog_test()
{
	//static kal_uint8 count = 0;
	kal_uint8 gpio51_data;	
	char buffer[MAX_UART_LEN+1];	
	RJ_GPS_StopTimer(BIRD_WDG_timer);
	watchdog_testcount ++;
	watchdog_state_test =  !watchdog_state_test;
#ifdef DW06A_PCB
	GPIO_ModeSetup(3, 0);		//eint4	
	GPIO_InitIO(1, 3);
	GPIO_WriteIO(watchdog_state_test,3);	
#else
	GPIO_ModeSetup(30, 0);		//eint4	
	GPIO_InitIO(1, 30);
	GPIO_WriteIO(watchdog_state_test,30);
#endif
	gpio51_data =  GPIO_ReadIO(51);		
	if( gpio51_data ==0)
	{
	RJ_GPS_StopTimer(BIRD_WDG_timer_TEST);
	watchdog_at_flag=1;
	sprintf(buffer, "RESET = %d", watchdog_at_flag);
 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);			
	watchdog_testcount =0;
	}
	else if(watchdog_testcount>100)
	{
	RJ_GPS_StopTimer(BIRD_WDG_timer_TEST);
	watchdog_at_flag=0;
	sprintf(buffer, "RESET = %d", watchdog_at_flag);
 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);			
	}
	else
	{
	RJ_GPS_StartTimer(BIRD_WDG_timer_TEST, 50, bird_watchdog_test); 	
	//watchdog_at_flag=1;	
	}
}
extern kal_uint16 gp_gsv_num;
extern kal_uint16 gp_gsv_sat;

kal_uint8 GPS_at_flag = 2;
void bird_gps_num(kal_uint16 param)
{
		static int param_snr;
		static kal_uint8 test_count=0;
		char buffer[MAX_UART_LEN+1];
		if(test_count == 0)
			param_snr = param;
		if((gp_gsv_num>=1)&&(gp_gsv_sat>=param_snr)&&(test_count<=7))   //     ranyingchun 530  
		{
			GPS_at_flag = 1;
			sprintf(buffer, "GPS_at_flag = %d,%d", GPS_at_flag,gp_gsv_sat);
	 		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
			return;
		}
		if(test_count>7)
		{
			GPS_at_flag = 0;
			sprintf(buffer, "GPS_at_flag = %d,%d", GPS_at_flag,gp_gsv_sat);
	 		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);					
			return;
		}

		test_count++;
		RJ_GPS_StartTimer(Bird_read_gpsnum_timer, 1000*2, bird_gps_num);  			
                                    
}
int test_count1=0;
void bird_gps_num_test(kal_uint16 param)
{
		static int param_snr;
		char buffer[MAX_UART_LEN+1];
		if(test_count1 == 0)
			param_snr = param;
		if(test_count1>7)
			{
		sprintf(buffer,"\r\n+EBGPS:FAIL\r\n");
		 rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
				return;
			}
		if((gp_gsv_num>=1)&&(gp_gsv_sat>=param_snr))   //     ranyingchun 530  
		{
		sprintf (buffer, "\r\n+EBGPS:OK\r\n");	 
		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
		//RJ_GPS_StartTimer(BIRD_WDG_timer_TEST, 5000, bird_watchdog_test); 					
		return;
		}else
		{
		sprintf(buffer,"\r\n+EBGPS:testing\r\n");
		 rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
		}													
	test_count1++;
	RJ_GPS_StartTimer(Bird_read_gpsnum_timer, 1000*5, bird_gps_num);                                      
}
kal_uint8 SOS_at_flag = 0;
kal_uint8 AZ_at_flag = 0;
kal_uint8 ACC_at_flag = 0;
kal_uint8 ADC_at_flag = 0;
kal_uint8 GSM_at_flag = 2;
kal_uint8 MS_at_flag = 0;
kal_uint8 UART_at_flag = 0;
kal_uint8 SIM_at_flag = 2;
kal_uint16 sos_count =0;
kal_uint16 az_count =0;
kal_uint16 acc_count =0;	
kal_uint16 count1 = 0;
kal_uint8 sos_init_status = 0;
kal_uint8 sos_status = 0;
kal_uint8 az_init_status = 0;
kal_uint8 az_status = 0;
kal_uint8 acc_init_status = 0;
kal_uint8 acc_status = 0;
kal_bool oc_state =0;
extern char bird_at_test_flag;
void bird_at_main_test()
{
	kal_uint8 gsm_flag = 0;
	kal_uint8 sim_flag = 0;
	double adc_dvol; 
	kal_uint8 ms_flag =0;
	kal_uint8 tmp_buf[]="AT^GPSTST=1";
	char buffer[MAX_UART_LEN+1];
	
	oc_state =  !oc_state;
	GPIO_ModeSetup(12, 0);		//eint4	
	GPIO_InitIO(1, 12);
	GPIO_WriteIO(oc_state,12);

	if(SOS_at_flag == 2)
		{			
		sos_init_status = GPIO_ReadIO(31);					
		if(sos_init_status != sos_status)
		{
			sos_count++;
			sos_status = sos_init_status;
		}
		if(sos_count >3)
		{
			SOS_at_flag =1;
			sprintf(buffer, "SOS_at_flag = %d", SOS_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}
		if((count1 > 10)&&(sos_count <4))
		{
			SOS_at_flag =0;			
		sprintf(buffer, "SOS_at_flag = %d", SOS_at_flag);
	 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}				
	}
		
	if(AZ_at_flag == 2)
	{
		az_init_status = GPIO_ReadIO(32);			
		if(az_init_status != az_status)
		{
			az_count++;
			az_status = az_init_status;
		}
		if(az_count >3)
		{
			AZ_at_flag =1;
			sprintf(buffer, "AZ_at_flag = %d", AZ_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}	
		if((count1 > 10)&&(az_count <4))
		{
			AZ_at_flag =0;					
			sprintf(buffer, "AZ_at_flag = %d", AZ_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}
	}
	
	if(ACC_at_flag == 2)
	{
		acc_init_status = GPIO_ReadIO(14);				
		if(acc_init_status != acc_status)
		{
			acc_count++;
			acc_status = acc_init_status;
		}
		if(acc_count >3)
		{
			ACC_at_flag =1;			
			sprintf(buffer, "ACC_at_flag = %d", ACC_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}
		if((count1 >30)&&(acc_count <4))
		{				
			ACC_at_flag =0;						
			sprintf(buffer, "ACC_at_flag = %d", ACC_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}			
	}
	
	#ifndef BIRD_EXTERNEL_POWER_UNSUPPORT						
	if(ADC_at_flag == 2)
	{
		adc_dvol = bird_get_adc_channel_voltage();
		if((adc_dvol > 11.500000) &&(adc_dvol < 12.500000))
		{
			ADC_at_flag = 1;				
		}
		else
		{ADC_at_flag = 0;	}
		sprintf(buffer, "ADC_at_flag = %d", ADC_at_flag);
	 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
	}
	#endif	
	if(MS_at_flag == 2)
	{
		ms_flag = acc_sensor_get_ini();
		if(ms_flag ==1)
		{
		MS_at_flag = 1;
		}
		else
		{MS_at_flag = 0;	}
		sprintf(buffer, "MS_at_flag = %d", MS_at_flag);
	 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
	}
	
	if(GSM_at_flag == 2)
	{
		gsm_flag = bird_get_gsm_strength_forat();		
		if((gsm_flag>5)&&(gsm_flag<10))
		{
			GSM_at_flag = 1;
			sprintf(buffer, "GSM_at_flag = %d", GSM_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);			
		}
		else if(count1>20)
		{
			GSM_at_flag =0;
			sprintf(buffer, "GSM_at_flag = %d", GSM_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
		}
	}

	if(SIM_at_flag == 2)
	{
		sim_flag = RJ_GPS_GetSimCardStatus();		
		if(sim_flag == 1)
		{
			SIM_at_flag = 1;
			sprintf(buffer, "SIM_at_flag = %d", SIM_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
		}
		else if(count1>50)
		{
			SIM_at_flag = 0;
			sprintf(buffer, "SIM_at_flag = %d", SIM_at_flag);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
		}
	}	
	
	#ifdef BIRD_ECU_SUPPORT
	if(UART_at_flag == 2)
	{
		ECU_UART_Write(tmp_buf,50);
		bird_deal_send_msg_handler(MSG_ID_AUX_ECU_DATA_ALARM, 0, 0);
		if(g_m_readuartdata ==1)
		{
			UART_at_flag = 1;
		}
		else
			{UART_at_flag = 0;}	
		sprintf(buffer, "UART_at_flag = %d", UART_at_flag);
	 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);				
	}
	#endif
	
	if(watchdog_at_flag ==2)
	{
		watchdog_at_flag = 0;
		bird_watchdog_test();	
	}
#ifdef BIRD_BT_SUPPORT	
		if(((mac_data[0]!=0)||(mac_data[1]!=0)||(mac_data[2]!=0)||(mac_data[3]!=0)||(mac_data[4]!=0)||(mac_data[5]!=0))&&(AT_test_flag == 1))
			{
			sprintf(buffer, "MAC_address = %x:%x:%x:%x:%x:%x",mac_data[5],mac_data[4],mac_data[3],mac_data[2],mac_data[1],mac_data[0]);
		 	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);		
		 	AT_test_flag = 2;
			}
#endif
	count1 ++;		
	RJ_GPS_StartTimer(BIRD_ATTEST_timer, 200, bird_at_main_test);					
	if((SOS_at_flag == 1)&&(AZ_at_flag == 1)&&(ACC_at_flag == 1))
	{
		RJ_GPS_StopTimer(BIRD_ATTEST_timer);
	}
	
}
void Bird_at_test(kal_uint16 param1,kal_uint16 param2)
{
	kal_uint16 value,value1,value2,value3,value4,value5,value6,value7;
	static kal_uint16 temp_param1,temp_param2;
	
	char buffer[MAX_UART_LEN+1];
	temp_param2 = param2;
	
	value = temp_param2&0x10;	
	if(value == 0x10)
		{
			GPIO_ModeSetup(31, 0);		//
			GPIO_InitIO(0, 31);	
			SOS_at_flag =2;
		}
	
	value1 = temp_param2 &0x20;
	if(value1 == 0x20)
		{
			GPIO_ModeSetup(32, 0);		//
			GPIO_InitIO(0, 32);			
			AZ_at_flag =2;		
		}
	
	value2 = temp_param2 & 0x40;
	if(value2 == 0x40)
		{
			GPIO_ModeSetup(14, 0);		//
			GPIO_InitIO(0, 14);		
			ACC_at_flag = 2;
		}
	
	value3 = temp_param2 & 0x08;
	if(value3 == 0x08)
		ADC_at_flag = 2;
	
	value4 = temp_param2 & 0x80;
	if(value4 == 0x80)
		MS_at_flag = 2;

	value5 = temp_param2 & 0x04;
	if(value5 == 0x04)
		UART_at_flag = 2;

	value6 = temp_param2 & 0x02;
	if(value6 == 0x02)
		watchdog_at_flag = 2;	
	
#ifdef BIRD_BT_SUPPORT	
	value7 = temp_param2 & 0x01;
	if(value7 == 0x01)
	{
		AT_test_flag = 1;
		send_device_num_to_BT();
	}
#endif
	
	if(GPS_at_flag == 2)
	{
		GPS_at_flag =3;
		bird_gps_num(param1);
	}			
	bird_at_main_test();		
	
}
/*****************************************************************************
* FUNCTION
*  	custom_command_hdlr()
* DESCRIPTION
*   	This function should parse the custom AT command and do correspondent action.
*     Customer should maintain and modify the code.
* PARAMETERS
*   	kal_uint8 * cmd_string
* RETURNS
*    KAL_TRUE : the command is handled by the customer
*    KAL_FALSE: the command is not handled by the customer
*****************************************************************************/
kal_bool custom_command_hdlr(char *full_cmd_string)
{
    char buffer[MAX_UART_LEN+1]; //MAUI_02377056
    char *cmd_name, *cmdString;
    kal_uint8 index = 0; 
    kal_uint16 length;
    kal_uint16 i;
    custom_cmdLine command_line;
    char *gps_param;
    kal_uint8 index_num = 0; 
   kal_uint16 param_num1=0;
   kal_uint16 param_num2=0;   
   kal_uint16 param_num3=0; 
   kal_uint16 param_num=0;    

    cmd_name = buffer;

    length = strlen(full_cmd_string);
    length = length > MAX_UART_LEN ? MAX_UART_LEN : length;    
    while ((full_cmd_string[index] != '=' ) &&  //might be TEST command or EXE command
        (full_cmd_string[index] != '?' ) && // might be READ command
        (full_cmd_string[index] != 13 ) && //carriage return
        index < length)  
    {
        cmd_name[index] = full_cmd_string[index] ;
        index ++;
    }
    cmd_name[index] = '\0' ;    
    
   index_num=index;
    for (i = 0 ; custom_cmd_table[i].commandString != NULL; i++ )
    {
        cmdString = custom_cmd_table[i].commandString;
        if (strcmp(cmd_name, cmdString) == 0 )
        {
            strncpy(command_line.character, full_cmd_string, COMMAND_LINE_SIZE + NULL_TERMINATOR_LENGTH);
            command_line.character[COMMAND_LINE_SIZE] = '\0';
            command_line.length = strlen(command_line.character);
            command_line.position = index;
            if (custom_cmd_table[i].commandFunc(&command_line) == CUSTOM_RSP_OK) 
            {
                sprintf(buffer, "OK");
                rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
            }
            else
            {
                sprintf(buffer, "ERROR");
                rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
            }
            return KAL_TRUE;
        }
    }    

    if (index <= 2)
        return KAL_FALSE;
	
    /* just a very basic example : customer can implement their own */
    cmd_name += 3;
    if (strcmp(cmd_name, "EXAMPLE") == 0)
    {		
        /* BEGIN: do the following parsing and correspondent action */
        /*  												    */
        /*  												    */
        /*  												    */
        /* END: do the following parsing and correspondent action    */

        /* generate final result code: "OK" or "ERROR" */
        if(KAL_TRUE)    // if operation is success
        {
                sprintf(buffer, "OK");
                rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
        }
        else if (KAL_FALSE) // if operation is fail
        {
            sprintf(buffer, "ERROR");
            rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
        }
        return KAL_TRUE;
    }
    #if !defined(__MMI_FMI__) || defined(WISDOM_MMI)
    else if (strcmp(cmd_name, "buffer") == 0)
    {		
        kal_print("Buffer Monitor enabled!");
        kal_monitor_buffer(0xffffffff);		
        sprintf(buffer, "OK");
        rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);

        return KAL_TRUE;
    }	
    #endif	
    #ifdef __TWOMICNR_SUPPORT__ 
    else if (twomicnr_command_hdlr(full_cmd_string) == KAL_TRUE)
    {
	return KAL_TRUE;
    }
    #endif
    #if (defined(__COMPOSITE_WEBCAM__)) 
    else if (strcmp(cmd_name, "SENSOR") == 0)
    {
        CAL_FEATURE_CTRL_STRUCT CalInPara;
        CalInPara.FeatureId = CAL_FEATURE_CAMERA_TUNING_SENSOR;
        CalInPara.FeatureSetValue = (kal_uint32)(full_cmd_string);
        CalCtrl(CAL_FEATURE_CTRL, (void*)&CalInPara, NULL, 0, NULL);
        return KAL_TRUE;
    }
    #endif
//add by yanchunhai
	        #ifdef RJ_GPS_APP
        else if (strcmp(cmd_name, "GPSTST") == 0)
        {

		extern kal_bool b_SIM_IS_OK;
		extern kal_bool get_Acc_onoff2(void);
              //extern void RJ_GPS_AT_OPEN();              
		extern kal_bool get_b_motion();
		kal_uint8 tmp_buf[]="AT^GPSTST=1";
		static kal_uint16 num=0;
		static kal_uint16 acc_num=0;		
		if(num==0)
			{
				BD_UART_INIT();
				acc_sensor_probe();
				num=1;
			}
		if((acc_num ==0)&&(b_SIM_IS_OK ==1))
		{
				#ifdef BIRD_ECU_SUPPORT
				ECU_UART_INIT();
				#endif
				acc_num =1;
		}
		GPIO_ModeSetup(51, 0);		//eint4	
		GPIO_InitIO(0, 51);	
                if((full_cmd_string[strlen("AT^GPSTST=")]!=13)&& (full_cmd_string[strlen("AT^GPSTST=")]!='0'))
                {
			if((full_cmd_string[index_num] == '=') && (index_num < length))   //&& (strcmp(cmd_name, "AT^GPSTST"))
				{      int i=0;
					 index_num ++;
					while ((full_cmd_string[index_num] != ',' ) &&  
		      				  (full_cmd_string[index_num] != '?' ) && 
		     				   (full_cmd_string[index_num] != 13 ) && index_num < length) 
						{
							gps_param[i] = full_cmd_string[index_num] ;
							i++;
							index_num ++;
						}
		   	  	 gps_param[i] = '\0' ;   		
				  param_num1 = atoi(gps_param);
				}
                        rj_gps_for_at_test =1;
                        if((rj_gps_for_at_test == 1)&&(bird_at_test_flag ==1))
                        {

					bird_deal_send_msg_handler(MSG_ID_YD_TK001_MSG_NORMAL_MOTOR_STATUS, 0, 0);
	#ifndef BIRD_EXTERNEL_POWER_UNSUPPORT					
			  		sprintf(buffer, "ADC = %f", bird_get_adc_channel_voltage());
                     		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
	#endif
	#ifndef BIRD_ACC_UNSUPPORT						
             	  		      sprintf(buffer, "ACC = %d", GET_ACC_SWITCH_STATUS());
                     		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
	#endif
					sprintf(buffer, "GSM = %d", bird_get_gsm_strength_forat());
                     		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);			
				      sprintf(buffer, "3D = %d", get_b_motion());
                     		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
				#ifdef BIRD_UART2_SUPPORT	
					ECU_UART_Write(tmp_buf,50);
					sprintf(buffer, "UART = %d", g_m_readuartdata);
                     		rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
				#endif
					watchdog_testcount =0;

					bird_watchdog_test();				
					test_count1=0;
					bird_gps_num_test(param_num1);
					
                        }
                }		   
		return KAL_TRUE;
        	}			
        else if (strcmp(cmd_name, "BIRDTST") == 0)
        {
		extern kal_uint8  Rj_GPS_Valid_Satelite_num(void);
		extern kal_uint8 factory_calibration_flag;

				static kal_uint16 num=0;
		if(num==0)
			{
				BD_UART_INIT();
				#ifdef BIRD_ECU_SUPPORT
				ECU_UART_INIT();
				#endif				
				acc_sensor_probe();
				num=1;
		}
		GPIO_ModeSetup(51, 0);		//eint4	
		GPIO_InitIO(0, 51);			
                if((full_cmd_string[strlen("AT^BIRDTST=")]!=13)&& (full_cmd_string[strlen("AT^BIRDTST=")]!='0'))
                {
			if((full_cmd_string[index_num] == '=') && (index_num < length))   //&& (strcmp(cmd_name, "AT^GPSTST"))
				{      int i=0;
					 index_num ++;
					while ((full_cmd_string[index_num] != ',' ) &&  
		      				  (full_cmd_string[index_num] != '?' ) && 
		     				   (full_cmd_string[index_num] != 13 ) && index_num < length) 
						{
							gps_param[i] = full_cmd_string[index_num] ;
							i++;
							index_num ++;
						}
		   	  	 gps_param[i] = '\0' ;   		
				  param_num1 = atoi(gps_param);
				  
				index_num ++;
				i=0;
				while ((full_cmd_string[index_num] != ',' ) &&  
	      				  (full_cmd_string[index_num] != '?' ) && 
	     				   (full_cmd_string[index_num] != 13 ) && index_num < length) 
					{
						gps_param[i] = full_cmd_string[index_num] ;
						i++;
						index_num ++;
					}
				gps_param[i] = '\0' ;   		
				  param_num2 = atoi(gps_param);
				  
				index_num ++;
				i=0;
				while ((full_cmd_string[index_num] != ',' ) &&  
	      				  (full_cmd_string[index_num] != '?' ) && 
	     				   (full_cmd_string[index_num] != 13 ) && index_num < length) 
					{
						gps_param[i] = full_cmd_string[index_num] ;
						i++;
						index_num ++;
					}
				gps_param[i] = '\0' ;   		
				  param_num3 = atoi(gps_param);				  
				}
                        rj_gps_for_at_test =1;
                        if((rj_gps_for_at_test == 1)&&(bird_at_test_flag ==1))
                        {
                        
				RJ_GPS_StopTimer(BIRD_ATTEST_timer);
				 GPS_at_flag = 2;
				 GSM_at_flag = 2;
				 SIM_at_flag = 2;
				 Bird_at_test(param_num1,param_num2);
				 sprintf(buffer, "FACTORY_CALI_OK = %d", factory_calibration_flag);
                     	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
				 bird_at_test_flag =0;
                        }

                }		   
		return KAL_TRUE;
        }

		
	  else if (strcmp(cmd_name, "GETGPSSN") == 0)
        {
        	extern kal_uint8  Rj_GPS_Valid_Satelite_num(void);	
		extern kal_char* bird_get_snrinfo();
		
		if(full_cmd_string[strlen("AT^GETGPSSN=")]=='1')
             {
             	
                   if(Rj_GPS_Valid_Satelite_num()>=3)
                   	{
                   		sprintf(buffer, "SN1:%s", bird_get_snrinfo());
				rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
                   	}
		 	else{
				sprintf(buffer, "SN0:%s", bird_get_snrinfo());
				rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
		 	}
			
		}
		return KAL_TRUE;
        }

        #endif
//add by yanchunhai
    else
    {
#ifdef __GAS_SUPPORT__
        if( gas_command_hdlr( (kal_uint8*)cmd_name, (kal_uint8*)full_cmd_string)==KAL_TRUE )
        {
            // it is a gati command and we will send the command to the appropriate module
            return KAL_TRUE;
        }
#endif 
    #ifdef __GATI_ENABLE__ 
        /* Judge if the command string is a GATI command*/
        if( gati_command_hdlr( (kal_uint8*)cmd_name, (kal_uint8*)full_cmd_string)==KAL_TRUE )
        {
            // it is a gati command and we will send the command to the appropriate module
            return KAL_TRUE;
        }		 	
    #endif
    }	
		 
    /* unrecognized command */
    /* we should return KAL_FALSE to let ATCI judge if the it can handle the command */
    return KAL_FALSE;
	
}

/*****************************************************************************
* FUNCTION
*  	custom_get_atcmd_symbol()
* DESCRIPTION
*   	This function returns special symbol for customer-defined AT command, 
*   	so AT parser can recognize its a custom command and forward the string to custom_command_handler().
*
*     CUSTOMER DOES NOT need to modify this function.
* PARAMETERS
*   	none
* RETURNS
*     kal_uint8 
*****************************************************************************/
kal_uint8 custom_get_atcmd_symbol(void)
{
   return (CUSTOM_SYMBOL);
}

/*****************************************************************************
* USER DEFINE CONSTANT
*  	RMMI_CUSTOM_DCD_VALUE_IN_DATA_STATE
* DESCRIPTION
* 	1. This value defines the control DCD value for data mode (CSD/GPRS dialup)
*   2. CUSTOMER CAN modify this value accroding to its TE requirement
*****************************************************************************/
#define RMMI_CUSTOM_DCD_VALUE_IN_DATA_STATE (1)

/*****************************************************************************
* USER DEFINE CONSTANT
*  	RMMI_CUSTOM_DCD_VALUE_IN_CMD_STATE
* DESCRIPTION
* 	1. This value defines the control DCD value for AT command mode (Normal/Escaped AT)
*   2. CUSTOMER CAN modify this value accroding to its TE requirement
*****************************************************************************/
#define RMMI_CUSTOM_DCD_VALUE_IN_CMD_STATE (0)

/*****************************************************************************
* FUNCTION
*  	custom_get_dcd_value_for_data_state()
* DESCRIPTION
*   	This function returns the user-defined DCD value for data state
*
*     CUSTOMER DOES NOT need to modify this function.
* PARAMETERS
*   	none
* RETURNS
*     kal_uint8 
*****************************************************************************/
kal_uint8 custom_get_dcd_value_for_data_state(void)
{
    return (kal_uint8) RMMI_CUSTOM_DCD_VALUE_IN_DATA_STATE;
}

/*****************************************************************************
* FUNCTION
*  	custom_get_dcd_value_for_cmd_state()
* DESCRIPTION
*   	This function returns the user-defined DCD value for command state
*       It shall be inverted value of custom_get_dcd_value_for_data_state()
*     CUSTOMER DOES NOT need to modify this function.
* PARAMETERS
*   	none
* RETURNS
*     kal_uint8 
*****************************************************************************/
kal_uint8 custom_get_dcd_value_for_cmd_state(void)
{
    return (kal_uint8)RMMI_CUSTOM_DCD_VALUE_IN_CMD_STATE;
}

#ifdef __MOD_TCM__
/*****************************************************************************
* FUNCTION
*  	custom_get_gprs_dialup_connect_post_string()
* DESCRIPTION
*   This function returns the post string right after CONNECT in gprs dialup.
*
*   The return string length must be less than 64
*
*   If post_string is XXXXXX, 
*   then MS sends "<cr><lf>CONNECT XXXXXX<cr><lf>" when GPRS dialup
*   Ex.1 if post_string=" 230400", 
*        then entire output string is "<cr><lf>CONNECT 230400<cr><lf>"
*   Ex.2 if post_string=" 56000 V42bis", 
*        then entire output string is "<cr><lf>CONNECT 56000 V42bis<cr><lf>"
*    
* PARAMETERS
*   kal_uint8 act, ACcess Technology, 
                   This field value can be referred to TS27.007
                   0,   GSM
                   1,   GSM Compact
                   2,   UTRAN
                   3,   GSM w/EGPRS
                   4,   UTRAN w/HSDPA
                   5,   UTRAN w/HSUPA
                   6,   UTRAN w/HSDPA and HSUPA
* RETURNS
*     kal_uint8* post_string
*****************************************************************************/
const kal_uint8* custom_get_gprs_dialup_connect_post_string(kal_uint8 act)
{
    /* Note: The return string length must be less than 64!!! */
#ifdef __TC01__
    #if defined(__HSDPA_SUPPORT__)
/* under construction !*/
	#elif defined(__EGPRS_MODE__)
/* under construction !*/
	#else
/* under construction !*/
	#endif
#elif defined(__PS_DIALUP_CONNECT_STRING_BY_ACT__)
    static kal_uint8 temp_string[64];
    const kal_uint8* post_string = temp_string;    
    if ((act == 0) || (act == 1) || (act == 3))
    {
        strcpy(temp_string, " 236800");
    }
    else
    {
        strcpy(temp_string, " 7200000");
    }    
#else
  //Note: user MUST prepare a space before the speed number
  #if defined(__HSDPA_SUPPORT__) && defined(__UMTS_FDD_MODE__)
	static const kal_uint8 post_string[] = " 7200000"; //7.2Mbps
  #elif defined(__HSDPA_SUPPORT__) && defined(__UMTS_TDD128_MODE__)
  	static const kal_uint8 post_string[] = " 2624000"; //2.6Mbps
  #elif defined(__UMTS_FDD_MODE__) || defined(__UMTS_TDD128_MODE__)
    static const kal_uint8 post_string[] =  " 384000"; //384kbps
  #elif defined(__EGPRS_MODE__)
    static const kal_uint8 post_string[] =  " 473600"; //473.6kbps
  #else
    static const kal_uint8 post_string[] =   " 80000"; //80kbps  
  #endif
#endif
    return (const kal_uint8*) post_string;
} // MAUI_02386357, mtk02285, MAUI_02825172
#endif /* __MOD_TCM__ */

/*****************************************************************************
* FUNCTION
*  	custom_is_edit_mode_command()
* DESCRIPTION
*   This function is used to let customer judge if the input command is needed to enter 
* editor mode or not. If true, we will output "> " and wait for 
*       ctrl+z character to execute the whole command string
*       ESC character to cancel the command
*    
* PARAMETERS
*   	data    [IN]    input command string header, maximum length : 10
* RETURNS
*     KAL_TRUE or KAL_FALSE
*****************************************************************************/
kal_bool custom_is_edit_mode_command(kal_uint8 *data)
{
    /*if (strncmp(data, "AT%EDIT", 7) == 0)
        return KAL_TRUE;
    else*/
        return KAL_FALSE;
}

#if defined(__PLMN_LIST_WITH_CELL_INFO__)
kal_uint32 custom_at_posi_get_ndssign(kal_char *buffer, kal_uint32 buf_len, kal_uint8 plmn_num, l4c_rat_plmn_info_struct *list)
{
    /* buf_len: MAX size of the 'buffer' please do not cruppt
       string_length: Indicates how many bytes writes into 'buffer'
    */
    
   /* Prototype of l4c_rat_plmn_info_struct
            #ifdef __PLMN_LIST_WITH_CELL_INFO__
                #define MAX_NUM_CELL_PER_PLMN  10
            #endif 
            
            #if defined(__PLMN_LIST_WITH_CELL_INFO__)
            typedef struct {
                kal_uint16       arfcn;
                kal_int16        rxlev;
                kal_uint8        bsic;
                kal_uint8        la_code[2];
                kal_uint16       cell_id;
                }l4c_location_cell_info_struct;
            #endif
            
            typedef struct {
               kal_uint8   plmn_id[7];
               kal_uint8   status;
               kal_uint8 rat;
               #if defined(__PLMN_LIST_WITH_CELL_INFO__)
               l4c_location_cell_info_struct cell_list[MAX_NUM_CELL_PER_PLMN];
               #endif
            } l4c_rat_plmn_info_struct;
      */
     /* sample code, judge how many PLMN and cell info in the parameter 'list'
          kal_uint8 i;
          kal_uint8 j;
          kal_uint8 ended;
          kal_uint16 temp_lac;
          kal_uint16 temp_lac_2;
          
          for (i = 0; i < plmn_num; i++)
            {
                for(j = 0; j < MAX_NUM_CELL_PER_PLMN; j++)
                {
                    temp_lac = list[i].cell_list[j].la_code[0];
                    temp_lac = (temp_lac << 8) | (list[i].cell_list[j].la_code[1]);

                    if((temp_lac == 0) && (list[i].cell_list[j].cell_id == 0))
                    {
                        break;
                    }

                    // Judge if this is the last cell info 
                    if((i == (plmn_num - 1)) && (j == (MAX_NUM_CELL_PER_PLMN - 1)))
                    {
                        ended = 1;
                    }
                    else if(i == (plmn_num - 1))
                    {
                        temp_lac_2 = list[i].cell_list[j+1].la_code[0];
                        temp_lac_2 = (temp_lac_2 << 8) | (list[i].cell_list[j+1].la_code[1]);

                        if((temp_lac_2 == 0) && (list[i].cell_list[j+1].cell_id == 0))
                        {
                            ended = 1;
                        }
                    }
                 }
             }
    */
    kal_uint32 string_length = 0;
    
    /* Replace the line after thie comment, calculate NDSSIGN and write into buffer */
    string_length = kal_sprintf(buffer, "\"NDSSIGN\"");

    
    /* It's recommended that do not remove the codes after this line! */
    if(string_length > buf_len)
    {
        /* Add length check, void memory curruption */
        ASSERT(0);
    }
    return string_length;
}

kal_bool custom_process_at_posi()
{
	  // customer code
    return KAL_FALSE;
}
#endif
