#include "rs_dev.h"
#include "rs_datatype.h"
#include "rs_flash_operate.h"
#include "rs_fs.h"
#include "rs_dev.h"
#include "rs_mem.h"
#include "rs_error.h"
#include "rs_std_fun.h"


// MTK header
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "kal_public_api.h"
#include "Mmiapi_struct.h"
#include "mmi_msg_struct.h"
#include "Mmi_frm_queue_gprot.h"
#include "Mmi_frm_events_gprot.h"
#include "Mmi_cb_mgr_gprot.h"
#include "TimerEvents.h"

#include "kal_release.h"
#include "kal_general_types.h"
#include "kal_public_defs.h"
#include "kal_public_api.h"
#include "kal_internal_api.h"
#include "kal_trace.h"
#include "dcl_uart.h"
#include "uart_internal.h"
#include "syscomp_config.h"
#include "task_config.h"




void rs_uart_baud(kal_int32 baud) //int handle, VMUINT8 *buffer, VMUINT16 length)
{
	UART_CTRL_DCB_T config_data;
	int handle;
	
	dbg_print("\n\rjintao %s run, baud is %d\n\r", __func__, baud);
	handle = DclSerialPort_Open(0,0);	//DBG_PRINT_PORT

	DclSerialPort_Control(handle,SIO_CMD_GET_OWNER_ID, (DCL_CTRL_DATA_T*)&config_data);
	if (baud == 0)
		config_data.rUARTConfig.u4Baud = UART_BAUD_115200; 	//UART_BAUD_115200;
	else if (baud == 1)
		config_data.rUARTConfig.u4Baud = UART_BAUD_921600; 	//UART_BAUD_921600;
	else
		config_data.rUARTConfig.u4Baud = UART_BAUD_115200; 	//default:UART_BAUD_115200
		
	config_data.rUARTConfig.u1DataBits = LEN_8;
	config_data.rUARTConfig.u1StopBits = SB_1;
	config_data.rUARTConfig.u1Parity = PA_NONE;
	config_data.rUARTConfig.u1FlowControl = FC_NONE;
	config_data.rUARTConfig.ucXonChar = 0x11;
	config_data.rUARTConfig.ucXoffChar = 0x13;
	config_data.rUARTConfig.fgDSRCheck = (DCL_BOOLEAN)KAL_FALSE;
	DclSerialPort_Control(handle, SIO_CMD_SET_DCB_CONFIG, (DCL_CTRL_DATA_T*)&config_data);
	UART_TurnOnPower(0,KAL_TRUE);
}

rs_s32 rs_device_init()
{
	rs_s32 errCode = RS_ERR_OK;

	/*
	if((bmt_Get_PowerOn_Type()==6)||(bmt_Get_PowerOn_Type()==7)||(bmt_Get_PowerOn_Type()==8))//POWER_ON_CHARGER_IN,POWER_ON_USB
		return RS_ERR_FAILED;
		
	rs_uart_baud(0);
	*/
	
	errCode = rs_flash_init();

	return errCode;
}



