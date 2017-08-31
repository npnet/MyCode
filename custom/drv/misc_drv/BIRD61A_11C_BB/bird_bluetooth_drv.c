#ifdef BIRD_BT_SUPPORT
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

#ifdef __BIRD_GPS_APP__
#include "bird_gps_framework.h"   
#include "bird_gps_app.h"   
#endif

#include "bird_app.h"
#include "bird_task.h"
#include    "dcl_adc.h"

#include "nmeap.h"    // add by lqy
#include "bird_gpscell.h" // add by lqy

#include "spi_hal.h"
#include "spi.h"



//用户信息变量
extern RJ_Gps_User_Info rj_user_info;


//add by lqy for BD
kal_int32 EINT_SW_Debounce_Modify(kal_uint8 eintno, kal_uint8 debounce_time);



extern void TRACE_UART(kal_uint8 * fmt,...);
extern void TRACE_P_3D(kal_uint8 * fmt,...);
extern void TRACE_P_GPS(kal_uint8 * fmt,...);

#define BLUETOOTH_EINT_PIN  52       //   9
#define BLUETOOTH_EINT_NO    23  //     8

#define EDGE_SENSITIVE           KAL_TRUE
static kal_bool bluetooth_state = (kal_bool)KAL_FALSE   ; //0: LEVEL_LOW; 1: LEVEL_HIGH

#define SPI_BUFFER_SIZE (3*16)    // 128*16
kal_uint8 bird_send_buf[15][SPI_BUFFER_SIZE]; 
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 
volatile kal_uint8 bird_tx_buf[SPI_BUFFER_SIZE]; 

__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 
volatile kal_uint8 bird_rx_buf[SPI_BUFFER_SIZE]; 

/*
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 
kal_uint32 bird_spi_data[SPI_BUFFER_SIZE]; 
*/

//#define TEST_STRING "Nordic_20161209_v00"

//kal_int32  spi_id = 0; 
SPI_HANDLE bird_SPI_handle;
//int SPI_return = 10;
SPI_RESULT bird_spi_result;
volatile kal_uint8 rx_on_flag = 0;

//static kal_uint8       tx_buf[] = TEST_STRING;           /**< TX buffer. */
//static kal_uint8       rx_buf[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
//static const kal_uint8 spi_length = sizeof(bird_tx_buf);        /**< Transfer length. */
SPI_CONFIG_PARAM_T spi_conf_para;

extern void TRACE_P_3D(kal_uint8 * fmt,...);
extern void rmmi_write_to_uart(kal_uint8 *buffer, kal_uint16 length, kal_bool stuff);
extern kal_uint16 ECU_UART_Write( kal_uint8 *buffer, kal_uint16 length);
extern void yd_uart_search_c9_04();
void get_bluetooth_data(void);
void unwap_bluetooth_data(kal_uint8 * buffer,kal_uint8 length);
void bird_spi_test(void);
void send_device_num_to_BT(void);
void ECU_data_to_tempbuf(kal_uint8 * bufer,kal_uint8 length);


kal_uint8 BT2GPS_tmp_buf[60] = {0};
kal_uint8 BT2GPS_C0_CMD1[10] = {0};
kal_uint8 BT2GPS_C0_CMD2[30] = {0};
kal_uint8 BT2GPS_C0_CMD3[30] = {0};
kal_uint8 BT2GPS_C0_CMD4[60] = {0};
kal_uint8 BT2GPS_C0_CMD5[60] = {0};
kal_uint8 BT2GPS_C0_CMD6[60] = {0};
kal_uint8 BT2GPS_C0_CMD7[60] = {0};
kal_uint8 BT2GPS_C0_CMD8[60] = {0};
kal_uint8 BT2GPS_C0_CMD9[60] = {0};
kal_uint8 BT2GPS_C0_CMD10[60] = {0};
kal_uint8 BT2GPS_C0_CMD11[60] = {0};
kal_uint8 BT2GPS_A0_CMD[60] = {0};
kal_uint8 BT2GPS_C9_CMD[60] = {0};
kal_uint8 BT2GPS_A9_CMD[60] = {0};
kal_uint8 BT2GPS_D0_CMD[60] = {0};
kal_uint8 BT2GPS_D0_CMD2[10] = {0};
kal_uint8 BT2GPS_B0_CMD2[60] = {0};
kal_uint8 BT2GPS_B0_CMD1[60] = {0};
kal_uint8 BT2GPS_B9_CMD[60] = {0};
kal_uint8 A9_NO_PRAPARE_CMD[10] = {0xA9,0x05,0x02,0x00,0xff,0xff,0xff,0x51,0x0d};
kal_uint8 A9_PRAPARE_CMD[10] = {0xA9,0x05,0x02,0x01,0xff,0xff,0xff,0x50,0x0d};

kal_uint8 bt_sent_enable_flag = 0;//0;
kal_uint8 bt_connect_status = 0;
kal_uint8 bt_induction_status = 0;
kal_uint8 AT_test_flag = 0;

void bird_spi_handler(void)
{
kal_prompt_trace(MOD_BIRD,"bird_spi_handler  enter");
//kal_prompt_trace(MOD_BIRD,"bird_spi_handler   rx_buf[0],[1],[2] = %d %d %d %d %d %d %d %d %d %d %d %d %d %d  ",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8],rx_buf[9],rx_buf[10],rx_buf[11],rx_buf[12],rx_buf[13]);
}

void bluetooth_interrput_handler(void)
{
	 ilm_struct       *aux_ilm = NULL;
	kal_prompt_trace(MOD_SOC,"bluetooth_interrput_handler  enter");

	if (bluetooth_state == KAL_FALSE)
	{	
             kal_prompt_trace(MOD_SOC," bluetooth_state KAL_FALSE"); 
	//	EINT_SW_Debounce_Modify(BLUETOOTH_EINT_NO,10);
		bluetooth_state = (kal_bool)!bluetooth_state;
		EINT_Set_Polarity(BLUETOOTH_EINT_NO,bluetooth_state);
//	 bird_spi_test();
	}
	else
	{
		
              kal_prompt_trace(MOD_SOC," bluetooth_state KAL_TURE"); 
	//	EINT_SW_Debounce_Modify(BLUETOOTH_EINT_NO,10);                              
		bluetooth_state = (kal_bool)!bluetooth_state;
		EINT_Set_Polarity(BLUETOOTH_EINT_NO,bluetooth_state); 
	//bird_spi_test();
	}
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_GET_BT_DATA, NULL);       
	msg_send_ext_queue(aux_ilm); 
//RJ_GPS_StartTimer(BIRD_SPI_TEST, 2, get_bluetooth_data);	
	
}

void get_bluetooth_data(void)
{
	kal_uint32 result;
	kal_uint8 i;
	kal_uint8 blue_tooth_wirte_buf[30] = {0};
	memset(bird_rx_buf,0,30);
	rx_on_flag = 1;
	result = spi_read(bird_SPI_handle, bird_rx_buf, 21, 1, NULL);    //ok
	rx_on_flag = 0;
	 if(result !=  SPI_RESULT_OK)
	 {
	      kal_prompt_trace(MOD_SOC, "bird_spi_test spi_write fail");
	  }
	  else 
	  {
	  for(i=0;i<20;i++)
	  	{
		blue_tooth_wirte_buf[i] = bird_rx_buf[i+1];
	  	}
	unwap_bluetooth_data(blue_tooth_wirte_buf,20);
//	 ECU_UART_Write(blue_tooth_wirte_buf,20);     //
	  
	  kal_prompt_trace(MOD_SOC, "get_bluetooth_data OK"); 
	   }
	kal_prompt_trace(MOD_BIRD,"unwap_from_bluetooth   rx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",bird_rx_buf[0],bird_rx_buf[1],bird_rx_buf[2],bird_rx_buf[3],bird_rx_buf[4],bird_rx_buf[5],bird_rx_buf[6],bird_rx_buf[7],bird_rx_buf[8],bird_rx_buf[9],bird_rx_buf[10],bird_rx_buf[11]);
}



kal_uint8 bird_get_bt_connect_status()
{
	return bt_connect_status;
}
kal_uint8 bird_get_bt_induction_status()
{
	return bt_induction_status;
}
void bt_distance_check()
{
	if(BT2GPS_D0_CMD2[3]==0)
	{
	bt_induction_status =0;
	kal_prompt_trace(MOD_BIRD,"Bird_BT_leave ");
	}
		
	if(BT2GPS_D0_CMD2[3]==1)
	{
	bt_induction_status =1;
	kal_prompt_trace(MOD_BIRD,"Bird_BT_near ");
	}
	yd_uart_search_c9_04();	
}


void bt_imei_check()
{
	kal_uint8 i = 0,flag=1;
	kal_uint8 j = 0;	
	kal_uint8 bt_sent_imei[8] ={0};
	kal_uint8 bt_receive_imei[15] = {0};
	kal_prompt_trace(MOD_BIRD,"bt_imei_check entry ");

	if(BT2GPS_D0_CMD[0]!=0)
	{
	for(i=0;i<8;i++)
	{
		bt_sent_imei[i] = BT2GPS_D0_CMD[3+i];
		kal_prompt_trace(MOD_BIRD,"bt_imei_check bt_sent_imei[%d]=%x",i,bt_sent_imei[i]);

	}
	
	bt_receive_imei[0] = (bt_sent_imei[0]&0x0f) + 0x30;
	bt_receive_imei[1] = (bt_sent_imei[1]>>4) + 0x30;
	bt_receive_imei[2] = (bt_sent_imei[1]&0x0f) + 0x30;
	bt_receive_imei[3] = (bt_sent_imei[2]>>4) + 0x30;
	bt_receive_imei[4] = (bt_sent_imei[2]&0x0f) + 0x30;
	bt_receive_imei[5] = (bt_sent_imei[3]>>4 )+ 0x30;
	bt_receive_imei[6] = (bt_sent_imei[3]&0x0f) + 0x30;
	bt_receive_imei[7] = (bt_sent_imei[4]>>4) + 0x30;
	bt_receive_imei[8] = (bt_sent_imei[4]&0x0f) + 0x30;		
	bt_receive_imei[9] = (bt_sent_imei[5]>>4) + 0x30;
	bt_receive_imei[10] = (bt_sent_imei[5]&0x0f)+ 0x30;
	bt_receive_imei[11] = (bt_sent_imei[6]>>4) + 0x30;
	bt_receive_imei[12] = (bt_sent_imei[6]&0x0f)  + 0x30;
	bt_receive_imei[13] = (bt_sent_imei[7]>>4) + 0x30;
	bt_receive_imei[14] = (bt_sent_imei[7]&0x0f) + 0x30;
	for(j=0;j<15;j++)
	{
		kal_prompt_trace(MOD_BIRD,"bt_imei_check bt_receive_imei[%d]=%x, rj_user_info.u_imsi[%d]=%x",j,bt_receive_imei[j],j,rj_user_info.u_imei[j]);

		if((bt_receive_imei[j]) !=(rj_user_info.u_imei[j]) )
			flag=0;

	}
	if(flag==1)
	{
	
		kal_prompt_trace(MOD_BIRD,"bt_imei_check 1");
		
		bt_sent_enable_flag = 1;
		bt_connect_status =1;
	//	bt_induction_status =1;
	ECU_data_to_tempbuf(A9_PRAPARE_CMD,9);
		yd_uart_search_c9_04();
	}
	else
		kal_prompt_trace(MOD_BIRD,"bt_imei_check 000");

	}
}
void bt_connect_check()
{
	kal_uint8 cvalue ;
	if( BT2GPS_B0_CMD2 [0]!=0)
	{
		cvalue = BT2GPS_B0_CMD2[3] &0x01;
		if(cvalue == 0x00)
		{
			if(bt_sent_enable_flag ==1)
			{			
				bt_sent_enable_flag = 0;
				bt_connect_status =0;
				bt_induction_status =0;    // 远离
				yd_uart_search_c9_04();			
			}
		}
	}
}
kal_uint8 mac_data[6]={0};
void bt_mac_check()
{
	kal_uint8 i;
	if( BT2GPS_B0_CMD1 [0]!=0)
	{
		for(i=0;i<6;i++)
		{
		mac_data[i] = BT2GPS_B0_CMD1[3+i];
		}
	}
	kal_prompt_trace(MOD_BIRD,"bt_mac_check BT2GPS_B0_CMD1 = %x %x %x %x %x %x %x %x %x ,",BT2GPS_B0_CMD1[0],BT2GPS_B0_CMD1[1],BT2GPS_B0_CMD1[2],BT2GPS_B0_CMD1[3],BT2GPS_B0_CMD1[4],BT2GPS_B0_CMD1[5],BT2GPS_B0_CMD1[6],BT2GPS_B0_CMD1[7],BT2GPS_B0_CMD1[8]);
	Bird_BT_MAC_data_alarm();	
}
kal_uint8 send_count = 0;
kal_uint8 write_count = 0;
void ECU_to_bluetooth(kal_uint8 * bufer,kal_uint8 length)
{
	kal_uint8 * buf = NULL;
	kal_uint8 lenth = 0;
	kal_uint32 result;
	kal_uint8 wap_num;
	kal_uint8 i,j;
	kal_uint8 temp_buf[32] = {0};
	buf = bufer;
	lenth = length;
	wap_num =(lenth+19)/20;
	for(j=0;j<wap_num;j++)
		{
		if(j != (wap_num - 1))
			{
			memset(temp_buf,0,21);
			temp_buf[0]  = 0xff;
			for(i =0;i<20;i++)
				{
				temp_buf[i+1] = *(buf+j*20+i);
				}

			memcpy((kal_uint8 *)bird_send_buf[send_count],temp_buf,21);
			send_count++;
			
			//result = spi_write(SPI_handle, tx_buf, 21, 1, NULL);    // bird_spi_handler 
			kal_prompt_trace(MOD_BIRD,"ECU_to_bluetooth lenth = %d, wap_num0 =%d ,",lenth,wap_num);
			lenth = lenth - 20;
			//kal_sleep_task(1);
			}
		else
			{
			memset(temp_buf,0,21);
			temp_buf[0]  = 0xff;
			for(i =0;i<lenth;i++)
				{
				temp_buf[i+1] = *(buf+j*20+i);
				}
			memcpy((kal_uint8 *)bird_send_buf[send_count],temp_buf,21);
			send_count++;
			//result = spi_write(SPI_handle, tx_buf, lenth+1, 1, NULL);    //  bird_spi_handler
			kal_prompt_trace(MOD_BIRD,"ECU_to_bluetooth lenth = %d, wap_num1 =%d ,",lenth,wap_num);
			//kal_sleep_task(1);
			}
			if(send_count == 15)
			{
			send_count = 0;
			}		
		}
}

void ECU_data_to_tempbuf(kal_uint8 * bufer,kal_uint8 length)
{
	kal_uint8 * buf = NULL;
	kal_uint8 lenth = 0;
	buf = bufer;
	lenth = length;
	memset((kal_uint8 *)bird_send_buf[send_count],0,SPI_BUFFER_SIZE);
	bird_send_buf[send_count][0] = 0xff;	
	bird_send_buf[send_count][1] = lenth;
	memcpy((kal_uint8 *)(&bird_send_buf[send_count][2]),buf,lenth);
	
	kal_prompt_trace(MOD_BIRD,"ECU_data_to_tempbuf  = %x,%x,%x,%x,%x,%x,%x,%x,",bird_send_buf[send_count][0],bird_send_buf[send_count][1],bird_send_buf[send_count][2],bird_send_buf[send_count][3],bird_send_buf[send_count][4],bird_send_buf[send_count][5],bird_send_buf[send_count][6],bird_send_buf[send_count][7]);
	send_count++;
	if(send_count == 15)
	{
	send_count = 0;
	}
	
}
void spi_sent_bt()
{
	kal_uint32 result;
	kal_uint8 wait_cnt = 0;
	if(send_count != write_count)
	{
	  /*
		memcpy(tx_buf,(kal_uint8 *)send_buf[write_count],21);
		result = spi_write(SPI_handle, tx_buf, 21, 1, NULL);    //  bird_spi_handler
         */
		memcpy(bird_tx_buf,(kal_uint8 *)bird_send_buf[write_count],(bird_send_buf[write_count][1]+2));
		while((rx_on_flag == 1)&&(wait_cnt<3))
		{
		kal_sleep_task(1);
		wait_cnt ++;
		}
		result = spi_write(bird_SPI_handle, bird_tx_buf, (bird_send_buf[write_count][1]+2), 1, NULL);    //  bird_spi_handler
		wait_cnt = 0;
		kal_prompt_trace(MOD_BIRD,"spi_sent_bt , send_count = %d, write_count =%d ,",send_count,write_count);	
		write_count ++;
	}
	if(write_count == 15)
	{
	write_count = 0;
	}	
	RJ_GPS_StartTimer(BIRD_SPI_TEST, 50, spi_sent_bt);			
}
void unwap_bluetooth_data(kal_uint8 * buffer,kal_uint8 length)
{
	kal_uint8 i;
   	kal_uint16  buf_len=0;
	kal_uint8 * buf = NULL;
	static kal_uint8 check_byte =0;
	static kal_uint8 status=0;  
	static kal_uint8 cnt=0;  
	static kal_uint8 BT_status = 0;
	static kal_uint8 C0_cnt_index = 0;
	static kal_uint8 C0_cmd_index = 0;
	static kal_uint8 C9_cnt_index = 0;
	static kal_uint8 C9_cmd_index = 0;
	static kal_uint8 B0_cnt_index = 0;	
	static kal_uint8 B0_cmd_index = 0;
	static kal_uint8 D0_cnt_index = 0;
	static kal_uint8 D0_cmd_index = 0;
	
	buf = buffer;
	buf_len=	length;	

 kal_prompt_trace(MOD_SOC," unwap_bluetooth_data"); 


	if (buf!=NULL)
		{
		for(i=0;i<buf_len;i++)
		{
		switch(status)
			{
			case 0:
			//	 kal_prompt_trace(MOD_SOC,"bluetooth case 0"); 
				cnt = 0;
				if(*(buf+i) == 0xC0)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
				//	 C0_cmd_index = 0;
					 BT_status=1; 
					 status = 1;
					 break;
					}
				else if(*(buf+i) == 0xA0)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
					 BT_status=2; 
					 status = 1;	
					 break;
					}
				else if(*(buf+i) == 0xC9)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
				//	 C9_cmd_index = 0;
					 BT_status=3; 
					 status = 1;	
					 break;
					}
				else if(*(buf+i) == 0xA9)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
					 BT_status=4; 
					 status = 1;	
					 break;
					}
				else if(*(buf+i) == 0xD0)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
					 BT_status=5; 
					 status = 1;	
					 break;
					}	
				else if(*(buf+i) == 0xB0)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
					 BT_status=6; 
					 status = 1;	
					 break;
					}	
				else if(*(buf+i) == 0xB9)
					{
					 BT2GPS_tmp_buf[cnt]=*(buf+i);
					 check_byte = BT2GPS_tmp_buf[cnt];
					 BT_status=7; 
					 status = 1;	
					 break;
					}
				else
					{
					status = 0;
					}
				break;	
				case 1:
					 kal_prompt_trace(MOD_SOC,"bluetooth case 1"); 
					cnt ++;
					BT2GPS_tmp_buf[cnt]=*(buf+i);
					check_byte ^= BT2GPS_tmp_buf[cnt];
					 if(BT_status == 1)
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x05)      // 5   blue_tooth no use
					 		{
							C0_cnt_index = 1;	
							status = 2;
							break;
					 		}
						else if(BT2GPS_tmp_buf[cnt] == 0x14)           //20
					 		{
							C0_cnt_index = 2;	
							status = 2;
							break;
					 		}
						else if(BT2GPS_tmp_buf[cnt] == 0x0B)       //11
					 		{
							C0_cnt_index = 3;	
							status = 2;
							break;
					 		}
						else if(BT2GPS_tmp_buf[cnt] == 0x29)     //  41   35
					 		{
							C0_cnt_index = 4;	
							status = 2;
							break;
					 		}
						else if(BT2GPS_tmp_buf[cnt] == 0x10)         // 16
					 		{
							C0_cnt_index = 5;	
							status = 2;
							break;
					 		}								
						else
							{
							status = 0;
							}
					 	}
					 else if(BT_status == 2)                        // 0xA0
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x06)
					 		{
							status = 2;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}	
					 else if(BT_status == 3)         //0xc9
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x05)
					 		{
							C9_cnt_index = 1;	
							status = 2;
							break;
					 		}
					 	else if(BT2GPS_tmp_buf[cnt] == 0x10)
					 		{
							C9_cnt_index = 2;	
							status = 2;
							break;
					 		}	
					 	else if(BT2GPS_tmp_buf[cnt] == 0x14)
					 		{
							C9_cnt_index = 3;	
							status = 2;
							break;
					 		}							
						else
							{
							status = 0;
							}
					 	}
					 else if(BT_status == 4)                        // 0xA9
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x05)
					 		{
							status = 2;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}	
					 else if(BT_status == 5)                        // 0xD0
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x09)
					 		{
					 		D0_cnt_index = 1;
							status = 2;
							break;
					 		}
						else if(BT2GPS_tmp_buf[cnt] == 0x02)
					 		{
					 		D0_cnt_index = 2;
							status = 2;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}
					 else if(BT_status == 6)                        // 0xB0
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x02)
					 		{
					 		B0_cnt_index = 2;
							status = 2;
							break;
					 		}
					 	else if(BT2GPS_tmp_buf[cnt] == 0x07)
					 		{
					 		B0_cnt_index = 1;
							status = 2;
							break;
					 		}						
						else
							{
							status = 0;
							}
					 	}					 
					 else if(BT_status == 7)                        // 0xD0
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x0A)
					 		{
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
					 kal_prompt_trace(MOD_SOC,"bluetooth case 2"); 
					cnt ++;
					BT2GPS_tmp_buf[cnt]=*(buf+i);
					check_byte ^= BT2GPS_tmp_buf[cnt];
					 if(BT_status == 1)
					 	{
						if((BT2GPS_tmp_buf[cnt] == 0x03)&&(C0_cnt_index == 3))
					 		{
							C0_cmd_index = 3;	
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x04)&&(C0_cnt_index == 4))
					 		{
							C0_cmd_index = 4;	
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x07)&&(C0_cnt_index == 5))
					 		{
							C0_cmd_index = 7;	
							status = 3;
							break;
					 		}	
						else if((BT2GPS_tmp_buf[cnt] == 0x0A)&&(C0_cnt_index == 2))
					 		{
							C0_cmd_index = 10;	
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x0B)&&(C0_cnt_index == 2))
					 		{
							C0_cmd_index = 11;	
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x01)&&(C0_cnt_index == 1))
					 		{
							C0_cmd_index = 1;	
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x06)&&(C0_cnt_index == 1))
					 		{
							C0_cmd_index = 6;	
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x09)&&(C0_cnt_index == 1))
					 		{
							C0_cmd_index = 9;	
							status = 3;
							break;
					 		}						
						else if((BT2GPS_tmp_buf[cnt] == 0x02)&&(C0_cnt_index == 2))
					 		{
							C0_cmd_index = 2;	
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x08)&&(C0_cnt_index == 2))
					 		{
							C0_cmd_index = 8;	
							status = 3;
							break;
					 		}

						else if((BT2GPS_tmp_buf[cnt] == 0x05)&&(C0_cnt_index == 2))
					 		{
							C0_cmd_index = 5;	
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}
					 else if(BT_status == 2)
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x01)
					 		{
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
					 	}	
					 else if(BT_status == 3)       // C9
					 	{
					 	if((BT2GPS_tmp_buf[cnt] == 0x01)&&(C9_cnt_index == 1))
					 		{
					 		C9_cmd_index=1;
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x02)&&(C9_cnt_index == 3))
					 		{
					 		C9_cmd_index=2;
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x03)&&(C9_cnt_index == 2))
					 		{
					 		C9_cmd_index=3;
							status = 3;
							break;
					 		}	
					 	else if((BT2GPS_tmp_buf[cnt] == 0x04)&&(C9_cnt_index == 2))
					 		{
					 		C9_cmd_index=4;
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x05)&&(C9_cnt_index == 2))
					 		{
					 		C9_cmd_index=5;
							status = 3;
							break;
					 		}						
	
					 	}
					 else if(BT_status == 4)                   // A9
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x01)
					 		{
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
						 }
					 else if(BT_status == 5)                   // D0
					 	{
					 	if((BT2GPS_tmp_buf[cnt] == 0x01)&&(D0_cnt_index==1))
					 		{
					 		D0_cmd_index=1;
							status = 3;
							break;
					 		}
						else if((BT2GPS_tmp_buf[cnt] == 0x02)&&(D0_cnt_index==2))
					 		{
					 		D0_cmd_index=2;
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
						 }
					 else if(BT_status == 6)                   // B0
					 	{
					 	if((BT2GPS_tmp_buf[cnt] == 0x02)&&(B0_cnt_index==2))
					 		{
					 		B0_cmd_index=2;
							status = 3;
							break;
					 		}
					 	else if((BT2GPS_tmp_buf[cnt] == 0x01)&&(B0_cnt_index==1))
					 		{
					 		B0_cmd_index=1;
							status = 3;
							break;
					 		}						
						else
							{
							status = 0;
							}
						 }						 
					 else if(BT_status == 7)                   // B9
					 	{
					 	if(BT2GPS_tmp_buf[cnt] == 0x01)
					 		{
							status = 3;
							break;
					 		}
						else
							{
							status = 0;
							}
						 }	
					 else
						{
						status = 0;
						}
				break;					
			case 3:
			//	 kal_prompt_trace(MOD_SOC,"bluetooth case 3"); 
				 cnt ++;
				 BT2GPS_tmp_buf[cnt]=*(buf+i);
				if(BT_status == 1)
					{
					if((cnt == 7)&&(C0_cnt_index == 1))     
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 22)&&(C0_cnt_index == 2))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 13)&&(C0_cnt_index == 3))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 43)&&(C0_cnt_index == 4))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 18)&&(C0_cnt_index == 5))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}					
					}

				else if(BT_status == 2)
					{
					if(cnt == 8)                // 
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					}
				else if(BT_status == 3)
					{					
					if((cnt == 7)&&(C9_cnt_index == 1))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 18)&&(C9_cnt_index == 2))
						{				
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 22)&&(C9_cnt_index == 3))
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}					
					}
				else if(BT_status == 4)
					{
					if(cnt == 7)               
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					}
				else if(BT_status == 5)
					{
					if((cnt == 11)&&(D0_cmd_index==1))                    
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 4)&&(D0_cmd_index==2))                    
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					}
				else if(BT_status == 6)
					{
					if((cnt == 4)&&(B0_cmd_index==2))               
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}
					if((cnt == 9)&&(B0_cmd_index==1))               
						{
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							}
						else 
							{
							status = 0;
							}
						}					
					}				
				else if(BT_status == 7)
					{
					if(cnt == 12)               
						{
						
						if(check_byte == BT2GPS_tmp_buf[cnt])
							{
							status = 4;
							kal_prompt_trace(MOD_BIRD,"BT2GPS_B9_CMD 0000000000");
							}
						else 
							{
							status = 0;
							}
						kal_prompt_trace(MOD_BIRD,"BT2GPS_B9_CMD check_byte = %d",check_byte);
						}
					}
				 check_byte ^= BT2GPS_tmp_buf[cnt];
				break;	
			case 4:	
				 kal_prompt_trace(MOD_SOC,"bluetooth case 4"); 
				cnt ++;
				 BT2GPS_tmp_buf[cnt]=*(buf+i);
				 if(BT2GPS_tmp_buf[cnt] == 0x0d)
				 	{
				 	if(BT2GPS_tmp_buf[0] == 0xC0)
						{
							if(C0_cmd_index==1)
							{
							       memcpy(BT2GPS_C0_CMD1,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD1,cnt+1);
							}
							else if(C0_cmd_index==2)
							{
							       memcpy(BT2GPS_C0_CMD2,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD2,cnt+1);
							}
							else if(C0_cmd_index==3)
							{							
							       memcpy(BT2GPS_C0_CMD3,BT2GPS_tmp_buf,cnt+1);	
								ECU_UART_Write(BT2GPS_C0_CMD3, cnt+1);
							}						
							else if(C0_cmd_index==4)
							{
							       memcpy(BT2GPS_C0_CMD4,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD4, cnt+1);
							}
							else if(C0_cmd_index==5)
							{
								memset(BT2GPS_C0_CMD5,0,60);
							       memcpy(BT2GPS_C0_CMD5,BT2GPS_tmp_buf,cnt+1);
								ECU_UART_Write(BT2GPS_C0_CMD5,cnt+1);
							}
							else if(C0_cmd_index==6)
							{
								memset(BT2GPS_C0_CMD6,0,60);
							       memcpy(BT2GPS_C0_CMD6,BT2GPS_tmp_buf,cnt+1);	
								ECU_UART_Write(BT2GPS_C0_CMD6,cnt+1);
							}
							else if(C0_cmd_index==7)
							{
							       memcpy(BT2GPS_C0_CMD7,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD7, cnt+1);
							}
							else if(C0_cmd_index==8)
							{
								memset(BT2GPS_C0_CMD8,0,60);
							       memcpy(BT2GPS_C0_CMD8,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD8,cnt+1);
							}						
							else if(C0_cmd_index==9)
							{
							       memcpy(BT2GPS_C0_CMD9,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD9,cnt+1);
							}
							else if(C0_cmd_index==10)
							{
							       memcpy(BT2GPS_C0_CMD10,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD10, cnt+1);
							}	
							else if(C0_cmd_index==11)
							{
							kal_prompt_trace(MOD_BIRD,"BT2GPS_C0_CMD11  enter");
							       memcpy(BT2GPS_C0_CMD11,BT2GPS_tmp_buf,cnt+1);		
								ECU_UART_Write(BT2GPS_C0_CMD11, cnt+1);
							}
						}
					else if(BT2GPS_tmp_buf[0] == 0xA0)
						{
						       memcpy(BT2GPS_A0_CMD,BT2GPS_tmp_buf,cnt+1);		
							ECU_UART_Write(BT2GPS_A0_CMD, cnt+1);
						}
					else if(BT2GPS_tmp_buf[0] == 0xC9)
						{
							memset(BT2GPS_C9_CMD,0,60);
							memcpy(BT2GPS_C9_CMD,BT2GPS_tmp_buf,cnt+1);	
							ECU_UART_Write(BT2GPS_C9_CMD, cnt+1);
						}
					else if(BT2GPS_tmp_buf[0] == 0xA9)
						{
							memset(BT2GPS_A9_CMD,0,60);
							memcpy(BT2GPS_A9_CMD,BT2GPS_tmp_buf,cnt+1);	
							ECU_UART_Write(BT2GPS_A9_CMD, cnt+1);
						}
					else if(BT2GPS_tmp_buf[0] == 0xD0)
						{
						if(D0_cmd_index == 1)
							{
							memset(BT2GPS_D0_CMD,0,60);
							memcpy(BT2GPS_D0_CMD,BT2GPS_tmp_buf,cnt+1);	
							//ECU_UART_Write(BT2GPS_D0_CMD, cnt+1);
							bt_imei_check();
							}
						else if(D0_cmd_index == 2)
							{
							memset(BT2GPS_D0_CMD2,0,10);
							memcpy(BT2GPS_D0_CMD2,BT2GPS_tmp_buf,cnt+1);	
							//ECU_UART_Write(BT2GPS_D1_CMD, cnt+1);
							kal_prompt_trace(MOD_BIRD,"BT2GPS_D0_CMD2  1111111111");
							
							bt_distance_check();
							}
						}	
					else if(BT2GPS_tmp_buf[0] == 0xB0)
						{
						if(B0_cmd_index==2)
							{
							memset(BT2GPS_B0_CMD2,0,60);
							memcpy(BT2GPS_B0_CMD2,BT2GPS_tmp_buf,cnt+1);	
							//ECU_UART_Write(BT2GPS_D0_CMD, cnt+1);
							bt_connect_check();
							}
						else if(B0_cmd_index==1)
							{
							memset(BT2GPS_B0_CMD1,0,60);
							memcpy(BT2GPS_B0_CMD1,BT2GPS_tmp_buf,cnt+1);	
							//ECU_UART_Write(BT2GPS_D0_CMD, cnt+1);
							bt_mac_check();
						}
						}
					else if(BT2GPS_tmp_buf[0] == 0xB9)
						{
							memset(BT2GPS_B9_CMD,0,60);
							memcpy(BT2GPS_B9_CMD,BT2GPS_tmp_buf,cnt+1);	
							kal_prompt_trace(MOD_BIRD,"BT2GPS_B9_CMD  1111111111");
							send_device_num_to_BT();

						}
					if(bt_sent_enable_flag == 0)
						ECU_data_to_tempbuf(A9_NO_PRAPARE_CMD,9);
						
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



void bird_spi_test(void)
{
/*
	kal_uint32 result;
	kal_bool brtn=0;
static kal_uint8 count = 0;
	    char buffer[128+1]; //MAUI_02377056
	    
	kal_prompt_trace(MOD_BIRD,"bird_spi_test  enter");
	kal_prompt_trace(MOD_SOC,"bird_spi_test  enter");


	rx_buf[0] = 0;
	rx_buf[1] = 0;
	rx_buf[2] = 0;
	rx_buf[3] = 0;
	rx_buf[4] = 0;
	rx_buf[5] = 0;

	tx_buf[0] = 'D';
	tx_buf[1] = 'W';
	tx_buf[2] = '0';
	tx_buf[3] = '9';
	tx_buf[4] = 0x55;
	tx_buf[5] = '1';
	tx_buf[6] = '3';
	if(count <10)
		{
		tx_buf[7] = '0'+count;
		tx_buf[8] = '0'+count;
		}
	else
		{
		count = 0;
		tx_buf[7] = '0'+count;
		tx_buf[8] = '0'+count;	
		}
		
	tx_buf[9] = 0x55;
	count ++;
	
   	//memset(tx_buf, 0x55, SPI_BUFFER_SIZE);
   	
	result = spi_read(SPI_handle, rx_buf, 16, 1, NULL);    //ok
	
	kal_sleep_task(1);
	
//	result = spi_write(SPI_handle, tx_buf, 16, 1, bird_spi_handler);
	

	//result = spi_readwrite(handle, tx_buf, rx_buf, 16, 1, NULL);     //nok

	kal_prompt_trace(MOD_BIRD,"bird_spi_test  spi_readwrite rx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8],rx_buf[9],rx_buf[10],rx_buf[11]);
	

	
	kal_prompt_trace(MOD_BIRD,"bird_spi_test  enter result = %d",result);
	 if(result !=  SPI_RESULT_OK)
	 {
	      kal_prompt_trace(MOD_SOC, "bird_spi_test spi_write fail");
	  }
	  else 
	  {
	  kal_prompt_trace(MOD_SOC, "bird_spi_test spi_write OK"); 
	   }

//	sprintf(buffer, "bird_spi_test   rx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8],rx_buf[9],rx_buf[10],rx_buf[11]);
//	rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);	
	TRACE_P_3D( "bird_spi_test   rx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8],rx_buf[9],rx_buf[10],rx_buf[11]);
			
	TRACE_P_3D("bird_spi_test  spi_read rx_buf = %c %c %c %c %c %c %c %c %c %c %c %c",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8],rx_buf[9],rx_buf[10],rx_buf[11]);

//	kal_prompt_trace(MOD_BIRD,"bird_spi_test   rx_buf[0],[1],[2] = %x %x %x %x %x %x %x %x %x ",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4],rx_buf[5],rx_buf[6],rx_buf[7],rx_buf[8]);



//	brtn = spi_fifo_pop(SPI_handle,spi_data);
//	kal_prompt_trace(MOD_BIRD,"bird_spi_test  spi_fifo_pop  spi_data = %d %d %d %d %d %d %d %d %d ",spi_data[0],spi_data[1],spi_data[2],spi_data[3],spi_data[4],spi_data[5],spi_data[6],spi_data[7],spi_data[8]);

  
//	kal_prompt_trace(MOD_SOC, "tx_buf[0],[1],[2],[3],[4],[5]=%x,%x,%x,%x,%x,%x",tx_buf[0],tx_buf[1],tx_buf[2],tx_buf[3],tx_buf[4],tx_buf[5]);
	
	
//	spi_test();
//	StartTimer(BIRD_SPI_TEST, 500*1, bird_spi_test);	
*/
}


 void bluetooth_interrput_configuration(void)     //static
{

   GPIO_ModeSetup(BLUETOOTH_EINT_PIN, 2);
//   EINT_SW_Debounce_Modify(MOTION_SENSOR_EINT_NO, 2);    // SW_DEBOUNCE_TIME
//   EINT_Mask(MOTION_SENSOR_EINT_NO);
//   EINT_Registration(MOTION_SENSOR_EINT_NO, KAL_TRUE, KAL_FALSE,                 /* ACT_HIGH     KAL_FALSE*/
 //                    bosch_motion_sensor_eint_hisr, KAL_TRUE);
 //  EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, LEVEL_SENSITIVE);

/*
   EINT_Registration(BLUETOOTH_EINT_NO,KAL_FALSE, KAL_FALSE,bluetooth_interrput_handler, KAL_TRUE);
   EINT_Set_Sensitivity(BLUETOOTH_EINT_NO, EDGE_SENSITIVE);
*/

	EINT_Registration(BLUETOOTH_EINT_NO,KAL_TRUE,bluetooth_state,bluetooth_interrput_handler, KAL_TRUE);
	//EINT_Set_Sensitivity(ALS_EINT_NO, LEVEL_SENSITIVE);
	EINT_Set_Polarity(BLUETOOTH_EINT_NO,bluetooth_state);

	// EINT_SW_Debounce_Modify(BLUETOOTH_EINT_NO,10);

   kal_prompt_trace(MOD_SOC,"bluetooth_interrput_configuration ");

	
}
void bluetooth_AT_test(kal_uint8 * data,kal_uint8 lenth)

{
	kal_uint32 result;
	kal_bool brtn=0;
	kal_uint8 i=0;
static kal_uint8 count = 0;
	
	kal_prompt_trace(MOD_BIRD,"bluetooth_AT_test  enter");
//	kal_prompt_trace(MOD_SOC,"bluetooth_AT_test  enter");


	bird_rx_buf[0] = 0;
	bird_rx_buf[1] = 0;
	bird_rx_buf[2] = 0;
	bird_rx_buf[3] = 0;
	
for(i=0;i<lenth;i++)
{
bird_tx_buf[i] = *(data+i);
}
kal_prompt_trace(MOD_BIRD,"bird_spi_test lenth =%d , tx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",lenth,bird_tx_buf[0],bird_tx_buf[1],bird_tx_buf[2],bird_tx_buf[3],bird_tx_buf[4],bird_tx_buf[5],bird_tx_buf[6],bird_tx_buf[7],bird_tx_buf[8],bird_tx_buf[9],bird_tx_buf[10],bird_tx_buf[11]);

result = spi_read(bird_SPI_handle, bird_rx_buf, 16, 1, NULL);    //ok

kal_sleep_task(1);
if(lenth<=10)
	result = spi_write(bird_SPI_handle, bird_tx_buf, lenth, 1, bird_spi_handler);
else
	result = spi_write(bird_SPI_handle, bird_tx_buf, 10, 1, bird_spi_handler);


	kal_prompt_trace(MOD_BIRD,"bird_spi_test  spi_readwrite rx_buf = %x %x %x %x %x %x %x %x %x %x %x %x",bird_rx_buf[0],bird_rx_buf[1],bird_rx_buf[2],bird_rx_buf[3],bird_rx_buf[4],bird_rx_buf[5],bird_rx_buf[6],bird_rx_buf[7],bird_rx_buf[8],bird_rx_buf[9],bird_rx_buf[10],bird_rx_buf[11]);
	

	
	kal_prompt_trace(MOD_BIRD,"bird_spi_test  enter result = %d",result);
	 if(result !=  SPI_RESULT_OK)
	 {
	      kal_prompt_trace(MOD_SOC, "bird_spi_test spi_write fail");
	  }
	  else 
	  {
	  kal_prompt_trace(MOD_SOC, "bird_spi_test spi_write OK"); 
	   }
/*
if((rx_buf[1]=='N')&&(rx_buf[2]=='o')&&(rx_buf[3]=='r'))
{
	return 1;
}
else
	return 0;
*/
}

void bluetooth_init(void)

{
	bird_SPI_handle = spi_open(0);
	kal_prompt_trace(MOD_BIRD,"bluetooth_init  SPI_return = %d", bird_SPI_handle);

	spi_conf_para.cs_setup_time = 10;
	spi_conf_para.cs_hold_time = 10;
	spi_conf_para.cs_idle_time = 10;
	spi_conf_para.clk_low_time = 10;
	spi_conf_para.clk_high_time = 10;
	spi_conf_para.rx_endian = SPI_ENDIAN_LITTLE;
	spi_conf_para.tx_endian = SPI_ENDIAN_LITTLE;

	spi_conf_para.tx_msbf = SPI_LSB;
	spi_conf_para.rx_msbf = SPI_LSB;
	spi_conf_para.clk_polarity = SPI_CPOL_0;
	spi_conf_para.clk_fmt = SPI_CPHA_0;



	bird_spi_result = spi_configure(bird_SPI_handle, &spi_conf_para);
	kal_prompt_trace(MOD_BIRD,"RJ_GPS_App_GpsStart  bird_spi_result = %d", bird_spi_result);
	if(bird_spi_result !=  SPI_RESULT_OK)
	    {
	         kal_prompt_trace(MOD_SOC, "spi config fail");
	    }
	    else 
	   {
	         kal_prompt_trace(MOD_SOC, "spi config ok");
	    }

	bluetooth_interrput_configuration();
}

	kal_uint8 device_num[15] = {0};
void send_device_num_to_BT(void)
{

	kal_uint8 checkbyte;
	kal_uint8 i;
	kal_prompt_trace(MOD_SOC,"send_device_num_to_BT id . %s \n",rj_user_info.u_id);   
	if(rj_user_info.u_id[0]!=0)
	{
		device_num[0] = 0xB9;
		device_num[1] = 0x0A;
		device_num[2] = 0x01;
		if(AT_test_flag == 1)
			{
			device_num[3] = '1';
			device_num[4] = '1';
			device_num[5] = '1';
			device_num[6] = '1';
			device_num[7] = '1';
			device_num[8] = '1';
			device_num[9] = '1';
			device_num[10] = '1';
			device_num[11] = '1';
			}
		else
			{	
			device_num[3] = *(rj_user_info.u_id);
			device_num[4] = *(rj_user_info.u_id + 1);
			device_num[5] = *(rj_user_info.u_id + 2);
			device_num[6] = *(rj_user_info.u_id + 3);
			device_num[7] = *(rj_user_info.u_id + 4);
			device_num[8] = *(rj_user_info.u_id + 5);
			device_num[9] = *(rj_user_info.u_id + 6);
			device_num[10] = *(rj_user_info.u_id + 7);
			device_num[11] = *(rj_user_info.u_id + 8);
			}
		checkbyte = device_num[0];
		for(i=1;i<=11;i++)
			checkbyte ^= device_num[i];
		device_num[12] =checkbyte;
		device_num[13] = 0x0d;
		kal_sleep_task(1);//hejulang 2017
		ECU_data_to_tempbuf(device_num,14);
		//ECU_to_bluetooth(device_num,14);
	}
}

#endif
