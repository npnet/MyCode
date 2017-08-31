#ifdef BIRD_CAN_SUPPORT
#include "can.h"
#include    "eint.h"
#include 	"stack_common.h"  
#include 	"stack_msgs.h"
#include 	"app_ltlcom.h"       /* Task message communiction */
#include 	"syscomp_config.h"
#include 	"task_config.h"
#include 	"drv_comm.h"
#include    "drvsignals.h"
#include "mcp2515.h"
#include "bird_app.h"
#include "app_datetime.h"
#include "rj_dw02.h"

extern const unsigned char CAN2_EINT_NO;
extern const unsigned char CAN1_EINT_NO;
extern can_data_save_struct car_can_data;

static kal_bool can1_eint_state = (kal_bool)KAL_FALSE;
static kal_bool can2_eint_state = (kal_bool)KAL_FALSE;


volatile applib_time_struct cur_time;
 

#define CANINTF_MERRF 0x80
#define CANINTF_WAKIF 0x40
#define CANINTF_ERRIF 0x20
#define CANINTF_TX2IF 0x10
#define CANINTF_TX1IF 0x08
#define CANINTF_TX0IF 0x04
#define CANINTF_RX1IF 0x02
#define CANINTF_RX0IF 0x01

#define RXBCTRL_OFF 0
#define RXBSIDH_OFF 1
#define RXBSIDL_OFF 2
#define RXBEID8_OFF 3
#define RXBEID0_OFF 4
#define RXBDLC_OFF  5
#define RXBDAT_OFF  6

#define CANINTF_RX (CANINTF_RX0IF | CANINTF_RX1IF)
#define CANINTF_TX (CANINTF_TX2IF | CANINTF_TX1IF | CANINTF_TX0IF)
#define CANINTF_ERR (CANINTF_ERRIF)

#define RXBCTRL(n)  (((n) * 0x10) + 0x60 + RXBCTRL_OFF)
#define CAN_FRAME_MAX_DATA_LEN	8
#define SPI_TRANSFER_BUF_LEN	(6 + CAN_FRAME_MAX_DATA_LEN)
#define CAN_FRAME_MAX_BITS	128

#define RXBDLC_LEN_MASK  0x0f

#define CAN_ID_NUM   	65
#define CAN_UNIT_NUM   	12
#define CAN_DATA_LENGTH 8
kal_uint8 can_rx_buf[CAN_ID_NUM][CAN_UNIT_NUM] = {0};
kal_uint8 can_rx_temp_buf[CAN_ID_NUM][CAN_UNIT_NUM] = {0};

extern void can_rx_data_check();

void can_id_fill(kal_uint8 *unit, kal_uint32 id)
{
	kal_uint8 i = 0;
	
	unit[0] = (id & 0xff000000) >>24;
	unit[1] = (id & 0xff0000) >>16;
	unit[2] = (id & 0xff00)>>8;
	unit[3] = (id & 0xff);
}

void can_table_create(void)
{
	kal_uint8 i = 0, j=0;
	
	kal_prompt_trace(MOD_SOC, "can_table_create");
	for (i = 0; i<CAN_ID_NUM;i++)
	{
		for(j = 0;j < CAN_UNIT_NUM; j++)
		{
			can_rx_temp_buf[i][j] = 0;
		}
	}
	can_id_fill(can_rx_temp_buf[0],	0x180128D0);  
	can_id_fill(can_rx_temp_buf[1],	0x180228D0);  
	can_id_fill(can_rx_temp_buf[2],	0x180328D0);  
	can_id_fill(can_rx_temp_buf[3],	0x180428D0);  
	can_id_fill(can_rx_temp_buf[4],	0x180528D0);  
	can_id_fill(can_rx_temp_buf[5],	0x181C28F3);  
	can_id_fill(can_rx_temp_buf[6],	0x1801D0F3);  
	can_id_fill(can_rx_temp_buf[7],	0x1803D0F3);  
	can_id_fill(can_rx_temp_buf[8],	0x1800D028);  
	can_id_fill(can_rx_temp_buf[9],	0x1801D08F);  
	can_id_fill(can_rx_temp_buf[10],	0x180228F3);
	can_id_fill(can_rx_temp_buf[11],	0x181B28F3);
	can_id_fill(can_rx_temp_buf[12],	0x181D28F3);
	can_id_fill(can_rx_temp_buf[13],	0xC00D0EF );
	can_id_fill(can_rx_temp_buf[14],	0x180028E5);
	can_id_fill(can_rx_temp_buf[15],	0x18E628F3);    // charg
	can_id_fill(can_rx_temp_buf[16],	0x18E528F3);
	can_id_fill(can_rx_temp_buf[17],	0x18E428F3);
	can_id_fill(can_rx_temp_buf[18],	0x18E328F3);
	can_id_fill(can_rx_temp_buf[19],	0x18E228F3);
	can_id_fill(can_rx_temp_buf[20],	0x18E128F3);
	can_id_fill(can_rx_temp_buf[21],	0x18E028F3);
	can_id_fill(can_rx_temp_buf[22],	0x18DF28F3);
	can_id_fill(can_rx_temp_buf[23],	0x18DE28F3);
	can_id_fill(can_rx_temp_buf[24],	0x18DD28F3);
	can_id_fill(can_rx_temp_buf[25],	0x18DC28F3);
	can_id_fill(can_rx_temp_buf[26],	0x18DB28F3);
	can_id_fill(can_rx_temp_buf[27],	0x18DA28F3);
	can_id_fill(can_rx_temp_buf[28],	0x18D928F3);
	can_id_fill(can_rx_temp_buf[29],	0x18D828F3);
	can_id_fill(can_rx_temp_buf[30],	0x18D728F3);
	can_id_fill(can_rx_temp_buf[31],	0x18D628F3);
	can_id_fill(can_rx_temp_buf[32],	0x18D528F3);
	can_id_fill(can_rx_temp_buf[33],	0x18D428F3);
	can_id_fill(can_rx_temp_buf[34],	0x18D328F3);
	can_id_fill(can_rx_temp_buf[35],	0x18D228F3);
	can_id_fill(can_rx_temp_buf[36],	0x18D128F3);
	can_id_fill(can_rx_temp_buf[37],	0x18D028F3);
	can_id_fill(can_rx_temp_buf[38],	0x18CF28F3);
	can_id_fill(can_rx_temp_buf[39],	0x18CE28F3);
	can_id_fill(can_rx_temp_buf[40],	0x18CD28F3);
	can_id_fill(can_rx_temp_buf[41],	0x18CC28F3);
	can_id_fill(can_rx_temp_buf[42],	0x18CB28F3);
	can_id_fill(can_rx_temp_buf[43],	0x18CA28F3);
	can_id_fill(can_rx_temp_buf[44],	0x18C928F3);
	can_id_fill(can_rx_temp_buf[45],	0x18C828F3);
	can_id_fill(can_rx_temp_buf[46],	0x18C728F3);
	can_id_fill(can_rx_temp_buf[47],	0x18C628F3);
	can_id_fill(can_rx_temp_buf[48],	0x18C528F3);
	can_id_fill(can_rx_temp_buf[49],	0x18C428F3);
	can_id_fill(can_rx_temp_buf[50],	0x18C328F3);
	can_id_fill(can_rx_temp_buf[51],	0x18C228F3);
	can_id_fill(can_rx_temp_buf[52],	0x18C128F3);
	can_id_fill(can_rx_temp_buf[53],	0x18C028F3);
	can_id_fill(can_rx_temp_buf[54],	0x18BF28F3);
	can_id_fill(can_rx_temp_buf[55],	0x18BE28F3);
	can_id_fill(can_rx_temp_buf[56],	0x18BD28F3);
	can_id_fill(can_rx_temp_buf[57],	0x18BC28F3);
	can_id_fill(can_rx_temp_buf[58],	0x18BB28F3);
	can_id_fill(can_rx_temp_buf[59],	0x18BA28F3);
	can_id_fill(can_rx_temp_buf[60],	0x18B928F3);
	can_id_fill(can_rx_temp_buf[61],	0x18B828F3);
	can_id_fill(can_rx_temp_buf[62],	0x18B728F3);
	can_id_fill(can_rx_temp_buf[63],	0x18B628F3);
	can_id_fill(can_rx_temp_buf[64],	0x18B528F3);
	
/*
	for (i = 0; i<CAN_ID_NUM;i++)
	{
		for(j = 0;j < CAN_UNIT_NUM; j++)
		{
			can_rx_buf[i][j] = 0;
		}
	}
	can_id_fill(can_rx_buf[0],	0x180128D0);  
	can_id_fill(can_rx_buf[1],	0x180228D0);  
	can_id_fill(can_rx_buf[2],	0x180328D0);  
	can_id_fill(can_rx_buf[3],	0x180428D0);  
	can_id_fill(can_rx_buf[4],	0x180528D0);  
	can_id_fill(can_rx_buf[5],	0x181C28F3);  
	can_id_fill(can_rx_buf[6],	0x1801D0F3);  
	can_id_fill(can_rx_buf[7],	0x1803D0F3);  
	can_id_fill(can_rx_buf[8],	0x1800D028);  
	can_id_fill(can_rx_buf[9],	0x1801D08F);  
	can_id_fill(can_rx_buf[10],	0x180228F3);
	can_id_fill(can_rx_buf[11],	0x181B28F3);
	can_id_fill(can_rx_buf[12],	0x181D28F3);
	can_id_fill(can_rx_buf[13],	0xC00D0EF );
	can_id_fill(can_rx_buf[14],	0x180028E5);
	can_id_fill(can_rx_buf[15],	0x18E628F3);
	can_id_fill(can_rx_buf[16],	0x18E528F3);
	can_id_fill(can_rx_buf[17],	0x18E428F3);
	can_id_fill(can_rx_buf[18],	0x18E328F3);
	can_id_fill(can_rx_buf[19],	0x18E228F3);
	can_id_fill(can_rx_buf[20],	0x18E128F3);
	can_id_fill(can_rx_buf[21],	0x18E028F3);
	can_id_fill(can_rx_buf[22],	0x18DF28F3);
	can_id_fill(can_rx_buf[23],	0x18DE28F3);
	can_id_fill(can_rx_buf[24],	0x18DD28F3);
	can_id_fill(can_rx_buf[25],	0x18DC28F3);
	can_id_fill(can_rx_buf[26],	0x18DB28F3);
	can_id_fill(can_rx_buf[27],	0x18DA28F3);
	can_id_fill(can_rx_buf[28],	0x18D928F3);
	can_id_fill(can_rx_buf[29],	0x18D828F3);
	can_id_fill(can_rx_buf[30],	0x18D728F3);
	can_id_fill(can_rx_buf[31],	0x18D628F3);
	can_id_fill(can_rx_buf[32],	0x18D528F3);
	can_id_fill(can_rx_buf[33],	0x18D428F3);
	can_id_fill(can_rx_buf[34],	0x18D328F3);
	can_id_fill(can_rx_buf[35],	0x18D228F3);
	can_id_fill(can_rx_buf[36],	0x18D128F3);
	can_id_fill(can_rx_buf[37],	0x18D028F3);
	can_id_fill(can_rx_buf[38],	0x18CF28F3);
	can_id_fill(can_rx_buf[39],	0x18CE28F3);
	can_id_fill(can_rx_buf[40],	0x18CD28F3);
	can_id_fill(can_rx_buf[41],	0x18CC28F3);
	can_id_fill(can_rx_buf[42],	0x18CB28F3);
	can_id_fill(can_rx_buf[43],	0x18CA28F3);
	can_id_fill(can_rx_buf[44],	0x18C928F3);
	can_id_fill(can_rx_buf[45],	0x18C828F3);
	can_id_fill(can_rx_buf[46],	0x18C728F3);
	can_id_fill(can_rx_buf[47],	0x18C628F3);
	can_id_fill(can_rx_buf[48],	0x18C528F3);
	can_id_fill(can_rx_buf[49],	0x18C428F3);
	can_id_fill(can_rx_buf[50],	0x18C328F3);
	can_id_fill(can_rx_buf[51],	0x18C228F3);
	can_id_fill(can_rx_buf[52],	0x18C128F3);
	can_id_fill(can_rx_buf[53],	0x18C028F3);
	can_id_fill(can_rx_buf[54],	0x18BF28F3);
	can_id_fill(can_rx_buf[55],	0x18BE28F3);
	can_id_fill(can_rx_buf[56],	0x18BD28F3);
	can_id_fill(can_rx_buf[57],	0x18BC28F3);
	can_id_fill(can_rx_buf[58],	0x18BB28F3);
	can_id_fill(can_rx_buf[59],	0x18BA28F3);
	can_id_fill(can_rx_buf[60],	0x18B928F3);
	can_id_fill(can_rx_buf[61],	0x18B828F3);
	can_id_fill(can_rx_buf[62],	0x18B728F3);
	can_id_fill(can_rx_buf[63],	0x18B628F3);
	can_id_fill(can_rx_buf[64],	0x18B528F3);
*/
	//for (i = 0; i<CAN_ID_NUM;i++)
	{
		//for(j = 0;j < CAN_UNIT_NUM; j++)
		{
			//kal_prompt_trace(MOD_SOC, "can_id_fill unit[%d][%d] = %x", i, j, can_rx_buf[i][j]);
		}
	}	
}

void can_unit_fill(kal_uint32 id, kal_uint8 *data)
{
	kal_uint8 i = 0, id_value[4] = {0};
	static kal_uint8 old_sec = 0,cnt = 0;

	id_value[0] = (id & 0xff000000) >>24;
	id_value[1] = (id & 0xff0000) >>16;
	id_value[2] = (id & 0xff00)>>8;
	id_value[3] = (id & 0xff);

	kal_prompt_trace(MOD_SOC, "can_unit_fill,");	
	for(i = 0; i< CAN_ID_NUM; i++)
	{
	
		if((can_rx_temp_buf[i][0] == id_value[0]) && (can_rx_temp_buf[i][1] == id_value[1]) 
			&& (can_rx_temp_buf[i][2] == id_value[2]) && (can_rx_temp_buf[i][3] == id_value[3]))
		{
			memcpy(&can_rx_temp_buf[i][4], data, CAN_DATA_LENGTH);
		}
	}
	
    	applib_dt_get_rtc_time(&cur_time);
	kal_prompt_trace(MOD_SOC, "cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);		
	if(cnt = 0)
	{
	old_sec = cur_time.nSec;
	cnt = 1;
	}
	if(cur_time.nSec != old_sec)
	{
		car_can_data.date[0] = cur_time.nYear %100;    
		car_can_data.date[1] = cur_time.nMonth;
		car_can_data.date[2] = cur_time.nDay;
		car_can_data.date[3] = cur_time.nHour;
		car_can_data.date[4] = cur_time.nMin;
		car_can_data.date[5] = cur_time.nSec;
		memcpy(can_rx_buf,can_rx_temp_buf, CAN_ID_NUM*CAN_UNIT_NUM);
		bird_send_message(MSG_ID_CAN_RX_CHECK);
	//	can_rx_data_check();
		kal_prompt_trace(MOD_SOC, "can_rx_data_check  cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);	
		old_sec = cur_time.nSec;
	}
	
}



void get_time_check_can_data(void)
{
	static kal_uint8 old_sec = 0,cnt = 0;
    	applib_dt_get_rtc_time(&cur_time);
	kal_prompt_trace(MOD_SOC, "cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);		
	if(cnt = 0)
	{
	old_sec = cur_time.nSec;
	cnt = 1;
	}
	if(cur_time.nSec != old_sec)
	{
		car_can_data.date[0] = cur_time.nYear %100;    
		car_can_data.date[1] = cur_time.nMonth;
		car_can_data.date[2] = cur_time.nDay;
		car_can_data.date[3] = cur_time.nHour;                  
		car_can_data.date[4] = cur_time.nMin;
		car_can_data.date[5] = cur_time.nSec;
		
	//	memcpy(can_rx_buf,can_rx_temp_buf, CAN_ID_NUM*CAN_UNIT_NUM);
		bird_send_message(MSG_ID_CAN_RX_CHECK);
	//	can_rx_data_check();
		kal_prompt_trace(MOD_SOC, "can_rx_data_check nYear = %d, nMonth = %d, nDay = %d, nHour = %d, cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nYear,car_can_data.date[1],car_can_data.date[2],car_can_data.date[3],cur_time.nMin,cur_time.nSec);	
		old_sec = cur_time.nSec;
	}
}


void can1_eint_hisr(void)
{	
	ilm_struct       *aux_ilm = NULL;
	
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_CAN1_HW_RX, NULL); 
	msg_send_ext_queue(aux_ilm);  
	kal_prompt_trace(MOD_SOC, "can1_eint_hisr");	
}
kal_uint16 CAN2_SID=0x00;
kal_uint32 CAN2_EID=0x0000;
kal_uint8 CAN2_DATA[8]= {0};
kal_uint8 CAN2_DLC = 0x00;

kal_uint16 CAN1_SID=0x00;
kal_uint32 CAN1_EID=0x0000;
kal_uint8 CAN1_DATA[8]= {0};
kal_uint8 CAN1_DLC = 0x00;

void can2_hw_rx(void)
{
	kal_uint8  data = 0, intf = 0, i = 0, j = 0;
	kal_uint8 buf[SPI_TRANSFER_BUF_LEN];
	S8 doublechar[40];
	kal_uint32 rec_id = 0;

	intf = MCP2515_byte_read2(CANINTF);
	if (intf == 0)
		return;
#if CAN_DEBUG		
	kal_prompt_trace(MOD_SOC, "can2_hw_rx CANINTF = %x", intf);	
#endif
	/* mask out flags we don't care about */
	intf &= CANINTF_RX | CANINTF_TX | CANINTF_ERR;
	/* receive buffer 0 */
	if (intf & CANINTF_RX0IF) {
		kal_prompt_trace(MOD_SOC, "can2_hw_rx receive buffer 0");	
		for (i = 1; i < RXBDAT_OFF; i++){
			buf[i] = MCP2515_byte_read2(RXBCTRL(0) + i);
#if CAN_DEBUG			
			kal_prompt_trace(MOD_SOC, "can2_hw_rx buf[%d] = %x", i, buf[i]);	
#endif
		}

		CAN2_DLC = (kal_uint8)(buf[5] & 0x0F);
		for (j = 0; i < (RXBDAT_OFF + CAN2_DLC); i++, j++){
			CAN2_DATA[j] = MCP2515_byte_read2(RXBCTRL(0) + i);
#if CAN_DEBUG		
			kal_prompt_trace(MOD_SOC, "can2_hw_rx DATA[%d] = %x", j, CAN2_DATA[j]);
#endif
		}
	}

	/* receive buffer 1 */
	if (intf & CANINTF_RX1IF) {
		kal_prompt_trace(MOD_SOC, "can2_hw_rx receive buffer 1");
		for (i = 1; i < RXBDAT_OFF; i++)
			buf[i] = MCP2515_byte_read2(RXBCTRL(1) + i);

		//len = get_can_dlc(buf[RXBDLC_OFF] & RXBDLC_LEN_MASK);
		for (; i < (RXBDAT_OFF + 8); i++)
			buf[i] = MCP2515_byte_read2(RXBCTRL(1) + i);
	}	
       CAN2_SID = (unsigned int)(buf[1]<<3);
	CAN2_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);
	
	CAN2_EID = (kal_uint32)((buf[2] & 0x03)<<16);
	CAN2_EID |= (kal_uint32)(buf[3]<<8);
	CAN2_EID |= (kal_uint32)(buf[4]);
	
	CAN2_DLC = (kal_uint8)(buf[5] & 0x0F);
	
	rec_id = CAN2_EID & 0x3ffff;
	rec_id |= CAN2_SID << 18;
	can_unit_fill(rec_id, CAN2_DATA);
	
	kal_prompt_trace(MOD_SOC, "rec_id = %x, CAN2_DLC = %x, data:%x %x %x %x %x %x %x %x", rec_id, CAN2_DLC, CAN2_DATA[0], CAN2_DATA[1], CAN2_DATA[2], CAN2_DATA[3], CAN2_DATA[4], CAN2_DATA[5], CAN2_DATA[6], CAN2_DATA[7]);
	MCP2515_byte_write2(CANINTF, 0x00);
}

void can1_hw_rx(void)
{
	kal_uint8  data = 0, intf = 0, i = 0, j = 0;
	kal_uint8 buf[SPI_TRANSFER_BUF_LEN];
	kal_uint32 rec_id = 0;

	intf = MCP2515_byte_read1(CANINTF);
	if (intf == 0)
		return;		
#if CAN_DEBUG		
	kal_prompt_trace(MOD_SOC, "can_hw1_rx CANINTF = %x", intf);		
#endif
	/* mask out flags we don't care about */
	intf &= CANINTF_RX | CANINTF_TX | CANINTF_ERR;
	/* receive buffer 0 */
	if (intf & CANINTF_RX0IF) {
#if CAN_DEBUG			
		kal_prompt_trace(MOD_SOC, "can_hw1_rx receive buffer 0");	
#endif
		for (i = 1; i < RXBDAT_OFF; i++){
			buf[i] = MCP2515_byte_read1(RXBCTRL(0) + i);
#if CAN_DEBUG				
			kal_prompt_trace(MOD_SOC, "can_hw1_rx buf[%d] = %x", i, buf[i]);	
#endif
		}

		CAN1_DLC = (kal_uint8)(buf[5] & 0x0F);
		for (j = 0; i < (RXBDAT_OFF + CAN1_DLC); i++, j++){
			CAN1_DATA[j] = MCP2515_byte_read1(RXBCTRL(0) + i);
#if CAN_DEBUG				
			kal_prompt_trace(MOD_SOC, "can_hw1_rx DATA[%d] = %x", j, CAN1_DATA[j]);
#endif
		}
	}

	/* receive buffer 1 */
	if (intf & CANINTF_RX1IF) {
#if CAN_DEBUG			
		kal_prompt_trace(MOD_SOC, "can_hw1_rx receive buffer 1");
#endif
		for (i = 1; i < RXBDAT_OFF; i++)
			buf[i] = MCP2515_byte_read1(RXBCTRL(1) + i);

		for (; i < (RXBDAT_OFF + 8); i++)
			buf[i] = MCP2515_byte_read1(RXBCTRL(1) + i);
	}	

       CAN1_SID = (unsigned int)(buf[1]<<3);
	CAN1_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);

	CAN1_EID = (kal_uint32)((buf[2] & 0x03)<<16);
	CAN1_EID |= (kal_uint32)(buf[3]<<8);
	CAN1_EID |= (kal_uint32)(buf[4]);
	
	CAN1_DLC = (kal_uint8)(buf[5] & 0x0F);
	rec_id = CAN1_EID & 0x3ffff;
	rec_id |= CAN1_SID << 18;
	
	can_unit_fill(rec_id, CAN1_DATA);	
	kal_prompt_trace(MOD_SOC, "rec_id = %x, CAN1_DLC = %x, data:%x %x %x %x %x", rec_id, CAN1_DLC, CAN1_DATA[0], CAN1_DATA[1], CAN1_DATA[2], CAN1_DATA[3], CAN1_DATA[4]);
	MCP2515_byte_write1(CANINTF, 0x00);
}

void can2_eint_hisr(void)
{	
	ilm_struct       *aux_ilm = NULL;
 
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_CAN2_HW_RX, NULL); 
	msg_send_ext_queue(aux_ilm);  
	kal_prompt_trace(MOD_SOC, "can2_eint_hisr");	
}
#include "gpio_var.c"
void can_int_register()
{
	kal_prompt_trace(MOD_SOC, "can_int_register");	
	GPIO_ModeSetup(CAN1_EINT_GPIO_NUM, 0);
	GPIO_ModeSetup(CAN2_EINT_GPIO_NUM, 0);	
	
	EINT_Registration(CAN1_EINT_NO,KAL_FALSE, can1_eint_state,can1_eint_hisr, KAL_TRUE);
	EINT_Set_Sensitivity(CAN1_EINT_NO, EDGE_SENSITIVE);
	EINT_Set_Polarity(CAN1_EINT_NO, can1_eint_state);

	EINT_Registration(CAN2_EINT_NO,KAL_FALSE, can2_eint_state,can2_eint_hisr, KAL_TRUE);
	EINT_Set_Sensitivity(CAN2_EINT_NO, EDGE_SENSITIVE);
	EINT_Set_Polarity(CAN2_EINT_NO, can2_eint_state);
	
	GPIO_ModeSetup(CAN1_EINT_GPIO_NUM, 2);
	GPIO_ModeSetup(CAN2_EINT_GPIO_NUM, 2);	
}

void can_start(void)
{
	/*disable can1 spi cs*/
	GPIO_ModeSetup(30, 0);	
	GPIO_InitIO(1, 30);
	GPIO_WriteIO(1, 30);	
	/*Enable stb2*/
	GPIO_ModeSetup(CAN_STBY_PORT2, 0);	
	GPIO_InitIO(1, CAN_STBY_PORT2);
	GPIO_WriteIO(0, CAN_STBY_PORT2);		
	/*Enable stb1*/
	GPIO_ModeSetup(CAN_STBY_PORT1, 0);	
	GPIO_InitIO(1, CAN_STBY_PORT1);
	GPIO_WriteIO(0, CAN_STBY_PORT1);
	
	can_table_create();
	
	mcp2515_init_gpio_spi();
	can_int_register();
}
#endif
