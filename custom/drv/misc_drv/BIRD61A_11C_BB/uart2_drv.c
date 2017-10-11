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


#ifdef RJ_GPS_APP
#include "bird_app.h"
#include "bird_task.h"
#endif
#include    "dcl_adc.h"

static DCL_HANDLE con_handleecu;
static UART_CTRL_OPEN_T con_openecu; 
kal_uint8 ecu_uart_port = 1;  //add by lqy2014-11-20     // 2  means  UART3 

/********************************************************/

static DCL_HANDLE con_handle;
static DCL_HANDLE con_handle1;
static DCL_HANDLE con_handlebd;  //add by lqy2014-11-20
static UART_CTRL_OPEN_T con_data1;
static UART_CTRL_OPEN_T con_openbd; //add by lqy2014-11-20
static UART_CTRL_POWERON_T con_data2;
static UART_CTRL_POWERON_T con_data3;

kal_uint8 bd_uart_port = 2;  //add by lqy2014-11-20     // 2  means  UART3 
/**********************************************************/
#if 1//def BIRD_UART2_SUPPORT
void ECU_UART_INIT(void)
{
     UART_CTRL_DCB_T bddata;
     DCL_STATUS status;	 
     int i;
     kal_uint16 	sendlen; 
    kal_prompt_trace(MOD_SOC," ECU_UART_INIT ");	
    GPIO_ModeSetup(12, 2);		/*UART*/	
    GPIO_ModeSetup(17, 2);		/*UART*/	
    Vcamd_Supply();
    con_data2.bFlag_Poweron = DCL_TRUE;
    con_handleecu = DclSerialPort_Open(ecu_uart_port, 0);
    status = DclSerialPort_Control(con_handleecu, UART_CMD_POWER_ON, (DCL_CTRL_DATA_T*)&con_data2);
    if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," UART_CMD_POWER_ON status = %d",status);
   }	
		   
   con_openecu.u4OwenrId = MOD_BIRD2;  
   con_handleecu = DclSerialPort_Open(ecu_uart_port, 0);
   status = DclSerialPort_Control(con_handleecu, SIO_CMD_OPEN, (DCL_CTRL_DATA_T*)&con_openecu); 
   if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," SIO_CMD_OPEN status = %d",status);
   }	
   bddata.u4OwenrId = MOD_BIRD2;  
   bddata.rUARTConfig.u4Baud = UART_BAUD_115200;
   bddata.rUARTConfig.u1DataBits = LEN_8;
   bddata.rUARTConfig.u1StopBits = SB_1;
   bddata.rUARTConfig.u1Parity = PA_NONE;
   bddata.rUARTConfig.u1FlowControl = FC_NONE;
   bddata.rUARTConfig.ucXonChar = 0x11;
   bddata.rUARTConfig.ucXoffChar = 0x13;
   bddata.rUARTConfig.fgDSRCheck = KAL_FALSE;
   
   con_handleecu = DclSerialPort_Open(ecu_uart_port, 0);	
   status = DclSerialPort_Control(con_handleecu, SIO_CMD_SET_DCB_CONFIG, (DCL_CTRL_DATA_T*)&bddata);
   if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," SIO_CMD_SET_DCB_CONFIG status = %d",status);
   

}
}

 void ECU_UART_CLOSE(void)
 {
    kal_prompt_trace(MOD_SOC," ECU_UART_CLOSE ");
    GPIO_ModeSetup(12, 0);		/*UART*/	
    GPIO_InitIO(0, 12);  
    GPIO_ModeSetup(17, 0);		/*UART*/	
    GPIO_InitIO(0, 17);	
    DclSerPort_Close(con_handleecu); 	   
    Vcamd_ShutOff();
 }

kal_uint16 ECU_UART_Write( kal_uint8 *buffer, kal_uint16 length)
{    

         kal_uint16 writeLen = 0;
    
	  UART_CTRL_PUT_BYTES_T data;
	  DCL_HANDLE con_handle;

	  kal_prompt_trace(MOD_SOC," ECU_UART_Write buffer");	
         data.u4OwenrId = MOD_BIRD2;
	  data.u2Length = length;
	  data.puBuffaddr = buffer;
	  con_handle = DclSerialPort_Open(ecu_uart_port, 0);
		 
	  DclSerialPort_Control(con_handle,SIO_CMD_PUT_BYTES, (DCL_CTRL_DATA_T*)&data);
	  writeLen =  data.u2RetSize;
          DclSerialPort_Close(con_handle); 
		
    return writeLen;
    
}


extern void get_time_check_can_data(void);
extern kal_uint8 can_rx_buf[][12];
void ECU_UART_READ_HANDLE(kal_uint8 *Buffaddr, kal_uint16 Length)
{
	static index = 0;
       kal_prompt_trace(MOD_SOC," ECU_UART_READ_HANDLE Length = %d",Length);	
	//kal_prompt_trace(MOD_SOC," ECU_UART_READ_HANDLE Buffaddr = %s",Buffaddr);	
	/*
	#ifdef BIRD_ECU_SUPPORT
	BD_ECU_INIT(Buffaddr,Length);
	#endif
*/
if(Length == 100)
{
	
	memcpy(can_rx_buf+100*index,Buffaddr, Length);
	index ++;
}
else
{
	memcpy(can_rx_buf+100*index,Buffaddr, Length);
	get_time_check_can_data();	
	index = 0;
}


}
#endif
#ifdef BIRD_UART3_SUPPORT
static kal_uint8 bd_sate_id[32]={0};              /*BDsatellite id*/
static kal_uint8 bd_snr[32]={0};                  /*BD SNR in dB*/

/*****************************************************************************
 * FUNCTION  
 *  BD_URAT_INIT
 * DESCRIPTION
 * UART init 
 * PARAMETERS 
 * // by lqy2014-11-20
 * RETURNS
 *  void
 *****************************************************************************/
void BD_UART_INIT(void)
{
     UART_CTRL_DCB_T bddata;
     DCL_STATUS status;	 
     int i;
     kal_uint16 	sendlen; 
 
    kal_prompt_trace(MOD_SOC," BD_UART_INIT ");
/*Added by Huangwenjian for optimize pm 20160630 begin*/	
    GPIO_ModeSetup(0, 3);		/*UART*/	
    GPIO_ModeSetup(1, 3);		/*UART*/	
/*Added by Huangwenjian for optimize pm 20160630 end*/		
   // Vcamd_Supply();
  //  Vcama_Supply();
    Vvibr_Supply();	

    // We must wait for 200 ms to let DSP hardware ready
    kal_sleep_task(44);  

    con_data3.bFlag_Poweron = DCL_TRUE;
    con_handlebd = DclSerialPort_Open(bd_uart_port, 0);
    status = DclSerialPort_Control(con_handlebd, UART_CMD_POWER_ON, (DCL_CTRL_DATA_T*)&con_data3);
    if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," UART_CMD_POWER_ON status = %d",status);
   }	
		   
   con_openbd.u4OwenrId = MOD_BIRD;  //here must MOD_GPS lqy
   con_handlebd = DclSerialPort_Open(bd_uart_port, 0);
   status = DclSerialPort_Control(con_handlebd, SIO_CMD_OPEN, (DCL_CTRL_DATA_T*)&con_openbd); 
   if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," SIO_CMD_OPEN status = %d",status);
   }	
   bddata.u4OwenrId = MOD_BIRD;  //here must MOD_GPS lqy
   #ifdef BD_HDDZ
      bddata.rUARTConfig.u4Baud = UART_BAUD_115200;
   #endif 
   bddata.rUARTConfig.u1DataBits = LEN_8;
   bddata.rUARTConfig.u1StopBits = SB_1;
   bddata.rUARTConfig.u1Parity = PA_NONE;
   bddata.rUARTConfig.u1FlowControl = FC_NONE;
   bddata.rUARTConfig.ucXonChar = 0x11;
   bddata.rUARTConfig.ucXoffChar = 0x13;
   bddata.rUARTConfig.fgDSRCheck = KAL_FALSE;
   
   con_handlebd = DclSerialPort_Open(bd_uart_port, 0);	
   status = DclSerialPort_Control(con_handlebd, SIO_CMD_SET_DCB_CONFIG, (DCL_CTRL_DATA_T*)&bddata);
   if (status !=STATUS_OK){
        kal_prompt_trace(MOD_SOC," SIO_CMD_SET_DCB_CONFIG status = %d",status);
   }	
   
   memset(bd_sate_id, 0, sizeof(bd_sate_id));
   memset(bd_snr, 0, sizeof(bd_snr));


}

/*****************************************************************************
 * FUNCTION  
 *  BD_URAT_INIT
 * DESCRIPTION
 * UART init 
 * PARAMETERS 
 * // by lqy2014-11-20
 * RETURNS
 *  void
 *****************************************************************************/
 void BD_UART_CLOSE(U8 * url)
 {
    kal_prompt_trace(MOD_SOC," BD_UART_CLOSE ");
/*Added by Huangwenjian for optimize pm 20160630 begin*/	
    GPIO_ModeSetup(0, 0);		/*UART*/	
    GPIO_InitIO(0, 0);  
    GPIO_PullenSetup(0, 0);
    GPIO_ModeSetup(1, 0);		/*UART*/	
    GPIO_InitIO(0, 1);	
    GPIO_PullenSetup(1, 0);
/*Added by Huangwenjian for optimize pm 20160630 end*/	
    DclSerialPort_Close(con_handlebd); 	   
    //Vcamd_ShutOff();
//    Vcama_ShutOff();  
    Vvibr_ShutOff();	  
 }

/*****************************************************************************
 * FUNCTION
 *  BD_UART_Write
 * DESCRIPTION
 *  BD uart write
 * PARAMETERS
 *  buffer      [IN]        buffer pointer 
 *  length     [IN]         length 
 * RETURNS
 *  void
 *****************************************************************************/
kal_uint16 BD_UART_Write( kal_uint8 *buffer, kal_uint16 length)
{    

         kal_uint16 writeLen = 0;
    
	  UART_CTRL_PUT_BYTES_T data;
	  DCL_HANDLE con_handle;

	  kal_prompt_trace(MOD_SOC," BD_UART_Write buffer");	
         data.u4OwenrId = MOD_BIRD;
	  data.u2Length = length;
	  data.puBuffaddr = buffer;
	  con_handle = DclSerialPort_Open(bd_uart_port, 0);
		 
	  DclSerialPort_Control(con_handle,SIO_CMD_PUT_BYTES, (DCL_CTRL_DATA_T*)&data);
	  writeLen =  data.u2RetSize;
          DclSerialPort_Close(con_handle); 
		
    return writeLen;
    

}
void BD_UART_READ_HANDLE(kal_uint8 *Buffaddr, kal_uint16 Length)
{

       kal_prompt_trace(MOD_SOC," BD_UART_READ_HANDLE Length = %d",Length);	
	kal_prompt_trace(MOD_SOC," BD_UART_READ_HANDLE Buffaddr = %s",Buffaddr);	
	BD_NMEAP_INIT(Buffaddr);
	
}
#endif

