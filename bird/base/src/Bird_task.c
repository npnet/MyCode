#ifdef RJ_GPS_APP

/*-------------------------------------------------------------
            固定服务器连接，日期不同即更新
            控制是否提供服务和是否计费。
--------------------------------------------------------------*/
#include "Bird_app.h"
#include "Bird_task.h"

#include "bird_std.h"

#include "dcl.h"
#include "uart_internal.h"
#include "uart_sw.h"
#include "kal_general_types.h"
#include "drv_comm.h"


rj_context_struct rj_context;
rj_context_struct *rj_context_p = &rj_context;
rj_timer_table_struct rj_timer_table[MAX_NUM_OF_RJ_TIMER];
stack_timer_struct birdbasetimer;
	
static void rj_timer_expiry_callback(void *arg);
static void rj_timer_expiry(ilm_struct *ilm_ptr);

//extern  static DCL_UINT16 UART_GetBytesAvail(DCL_DEV port);
//extern  static kal_uint16 UART_GetBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, kal_uint8 *status, module_type ownerid);



static DCL_UINT16 UART_GetBytesAvail(DCL_DEV port)
{
 DCL_HANDLE handle;
 UART_CTRL_RX_AVAIL_T data;
 
  handle = DclSerialPort_Open(port,0);
  DclSerialPort_Control(handle,SIO_CMD_GET_RX_AVAIL, (DCL_CTRL_DATA_T*)&data);
  DclSerialPort_Close(handle);
  return data.u2RetSize;
}

static kal_uint16 UART_PutBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid)
{
 DCL_HANDLE handle;
 UART_CTRL_PUT_BYTES_T data;
 //UART_CTRL_PUT_UART_BYTE_T data;
 
 data.u4OwenrId = ownerid;
 data.u2Length = Length;
 data.puBuffaddr = Buffaddr;
    handle = DclSerialPort_Open(port,0);
    DclSerialPort_Control(handle,SIO_CMD_PUT_BYTES, (DCL_CTRL_DATA_T*)&data); 
    return data.u2RetSize;
}
static kal_uint16 UART_GetBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, kal_uint8 *status, module_type ownerid)
{
    DCL_HANDLE handle;
    UART_CTRL_GET_BYTES_T data;
    data.u4OwenrId = ownerid;
    data.u2Length = Length;
    data.puBuffaddr = Buffaddr;
    data.pustatus = status;
    handle = DclSerialPort_Open(port,0);
    DclSerialPort_Control(handle,SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
    return data.u2RetSize;
}
void rj_gps_main(ilm_struct *ilm_ptr)
{
	
    if (MSG_ID_TIMER_EXPIRY == ilm_ptr->msg_id)
    {
       	rj_timer_expiry(ilm_ptr);
    }
    
}
kal_uint8 uart3_buf_temp[30];
void mtk_bird_uart3_poll(void)   // Port i/o interface object  hejulang
{
    kal_uint32 result_count, actual_written_len;
    kal_uint8 status;
    DCL_HANDLE handle;
	UART_CTRL_GET_BYTES_T data;

        do
        {
            data.u4OwenrId = MOD_BIRD;
	        data.u2Length = 30;
	        data.puBuffaddr = &uart3_buf_temp[0];
            data.pustatus = &status;	
	        handle = DclSerialPort_Open(2, 0);
	        DclSerialPort_Control(handle, SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
	        result_count = data.u2RetSize;	
        } while (result_count == 30);        

}

static void bird_task_main(task_entry_struct *task_entry_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
   ilm_struct	current_ilm;
   kal_uint32 my_index;
       kal_uint16 avail;
    kal_uint16 readSize,readTotal,bsize,sentSize;
    kal_uint8* buf;
   kal_get_my_task_index(&my_index);
   stack_set_active_module_id(my_index, MOD_BIRD);
    
    kal_prompt_trace(MOD_SOC," bird_task_main ");
    while (1)
    {
      	  receive_msg_ext_q(task_info_g[task_entry_ptr->task_indx].task_ext_qid, &current_ilm);
	  stack_set_active_module_id(my_index, current_ilm.dest_mod_id);
        //rj_gps_main((void*)&current_ilm);
	switch(current_ilm.msg_id)
      {
	  	case MSG_ID_TIMER_EXPIRY:
	        	rj_timer_expiry(&current_ilm);
	   		break; 
#if 1 //def BIRD_UART2_SUPPORT
		case MSG_ID_UART_READY_TO_READ_IND:  
                    //kal_prompt_trace(MOD_SOC," bird_task_main MSG_ID_UART_READY_TO_READ_IND");
			mtk_bird_uart3_poll();
                          break;
 
                          case MSG_ID_UART_READY_TO_WRITE_IND:
                                 kal_prompt_trace(MOD_SOC," bird_task_main MSG_ID_UART_READY_TO_WRITE_IND");
                                 sentSize = UART_PutBytes(2, (kal_uint8 *)buf, bsize, MOD_BIRD);
                                if(sentSize < bsize)
                                 {
                                          //如果此条件满足，表明实际发送的数据比期望发送的数据少，即期望发送bsize个数据，而实际发送了sentsize个数据。
                                         //所以需要在下一个MSG_ID_UART_READY_TO_WRITE_IND来临后调用UART_PutBytes发送剩余的(bsize-sentsize)的数据
                                 }
	   		break;
#endif
		default:
			break;
	}
        free_ilm(&current_ilm);
   
    }

}



kal_bool bird_task_init(task_indx_type task_indx)
{
    rj_context_p->event_scheduler_ptr = evshed_create("GPS_base_timer", MOD_BIRD, 0, 254 );
    return KAL_TRUE;
}


kal_bool bird_task_create(comptask_handler_struct **handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    static const comptask_handler_struct custom1_handler_info = 
    {
        bird_task_main,   /* task entry function */
        bird_task_init,   /* task initialization function */
        NULL,                   /* task configuration function */
        NULL,                   /* task reset handler */
        NULL,                   /* task termination handler */
    };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *handle = (comptask_handler_struct*) & custom1_handler_info;
    return KAL_TRUE;
}





static void rj_timer_expiry(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    	//stack_timer_struct *stack_timer_ptr;
 	//stack_timer_ptr = (stack_timer_struct*) ilm_ptr->local_para_ptr;
    	evshed_timer_handler(rj_context_p->event_scheduler_ptr);
    	//stack_process_time_out(&birdbasetimer);
    //}

}
/*****************************************************************************
 * FUNCTION
 *  gps_timer_expiry_callback
 * DESCRIPTION
 *  timer exiry callback
 * PARAMETERS
 *  arg     [IN]   callback function parameter   
 * RETURNS
 *  void
 *****************************************************************************/
static void rj_timer_expiry_callback(void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 *timer_id = (kal_uint8*) arg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ((kal_uint32) timer_id < (kal_uint32) MAX_NUM_OF_RJ_TIMER)
    {
        
        rj_timer_table[(kal_uint32) (timer_id)].event_id = NULL;
        (rj_timer_table[(kal_uint32) (timer_id)].callback_func) (rj_timer_table[(kal_uint32) (timer_id)].arg);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gps_start_timer
 * DESCRIPTION
 *  start timer
 * PARAMETERS
 *  timer_id            [IN]      timer id  
 *  period              [IN]       period 
 *  timer_expiry        [IN]    delay time in ticks    
 *  arg                 [IN]      callback function parameter      
 * RETURNS
 *  void
 *****************************************************************************/
void Rj_start_timer(U16 timer_id, U32 period, oslTimerFuncPtr timer_expiry, void *arg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint32 temp = period * 13 / 60 + 1;    /* ~= x 1/4.615 */
	//kal_prompt_trace(MOD_SOC,"Rj_start_timer period ,temp %d,%d",period,temp);
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	TRACE_P_3D("Rj_start_timer");
/*
    rj_timer_table[timer_id].callback_func = timer_expiry;
    rj_timer_table[timer_id].arg = arg;
    rj_timer_table[timer_id].event_id = evshed_set_event(
                                            rj_context_p->event_scheduler_ptr,
                                            (kal_timer_func_ptr) rj_timer_expiry_callback,
                                            (void*)timer_id,
                                            temp );
*/
    StartTimerEx(timer_id, period, timer_expiry,arg);
}


/*****************************************************************************
 * FUNCTION
 *  gps_stop_timer
 * DESCRIPTION
 *  stop timer
 * PARAMETERS
 *  timer_id        [IN]    timer id    
 * RETURNS
 *  void
 *****************************************************************************/
void Rj_stop_timer(U16 timer_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
    if (rj_timer_table[timer_id].event_id != NULL)
    {
        evshed_cancel_event(rj_context_p->event_scheduler_ptr, &(rj_timer_table[timer_id].event_id));
        rj_timer_table[timer_id].event_id = NULL;
    }
    */
     StopTimer(timer_id);
}
#endif


/********************************************************************/
/*SECOND TASK OF BIRD 	in 2017-08-09*/
/*MOD_BIRD2*/
/********************************************************************/
rj_context_struct rj_second_context;
rj_context_struct *rj_second_context_p = &rj_second_context;
rj_timer_table_struct rj_second_timer_table[MAX_RJ_SECOND_TIMER];

static void rj_second_timer_expiry(ilm_struct *ilm_ptr)
{
	evshed_timer_handler(rj_second_context_p->event_scheduler_ptr);
}

static void rj_second_timer_expiry_callback(void *arg)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	kal_uint8 *timer_id = (kal_uint8*)arg;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	if ((kal_uint32) timer_id < (kal_uint32) MAX_RJ_SECOND_TIMER)
	{
		rj_second_timer_table[(kal_uint32) (timer_id)].event_id = NULL;
		(rj_second_timer_table[(kal_uint32) (timer_id)].callback_func) (rj_second_timer_table[(kal_uint32) (timer_id)].arg);
	}
}


kal_uint8 uart2_buf_temp[100];
void mtk_bird_uart2_poll(void)   // Port i/o interface object  hejulang
{
    kal_uint32 result_count, actual_written_len;
    kal_uint8 status;
    DCL_HANDLE handle;
	UART_CTRL_GET_BYTES_T data;

        do
        {
            data.u4OwenrId = MOD_BIRD2;
	        data.u2Length = 100;
	        data.puBuffaddr = &uart2_buf_temp[0];
            data.pustatus = &status;	
	        handle = DclSerialPort_Open(1, 0);
	        DclSerialPort_Control(handle, SIO_CMD_GET_BYTES, (DCL_CTRL_DATA_T*)&data);
	        result_count = data.u2RetSize;	
        } while (result_count == 100);        

}

static void bird_second_task_main(task_entry_struct *task_entry_ptr)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	ilm_struct	current_ilm;
	kal_uint32 my_index;
	kal_uint16 readSize,readTotal,bsize,sentSize;
	kal_uint8* buf;
	kal_get_my_task_index(&my_index);
	stack_set_active_module_id(my_index, MOD_BIRD2);

	kal_prompt_trace(MOD_SOC," bird_second_task_main ");
	while (1)
	{
		receive_msg_ext_q(task_info_g[task_entry_ptr->task_indx].task_ext_qid, &current_ilm);
		stack_set_active_module_id(my_index, current_ilm.dest_mod_id);
		switch(current_ilm.msg_id)
		{
			case MSG_ID_TIMER_EXPIRY:
				rj_second_timer_expiry(&current_ilm);
				break; 
			case MSG_ID_UART_READY_TO_READ_IND:  
				kal_prompt_trace(MOD_SOC," bird_second_task_main MSG_ID_UART_READY_TO_READ_IND");
				mtk_bird_uart2_poll();
				break;
			case MSG_ID_UART_READY_TO_WRITE_IND:
				kal_prompt_trace(MOD_SOC," bird_second_task_main MSG_ID_UART_READY_TO_WRITE_IND");
				sentSize = UART_PutBytes(1, (kal_uint8 *)buf, bsize, MOD_BIRD2);
				if(sentSize < bsize)
				{
				//如果此条件满足，表明实际发送的数据比期望发送的数据少，即期望发送bsize个数据，而实际发送了sentsize个数据。
				//所以需要在下一个MSG_ID_UART_READY_TO_WRITE_IND来临后调用UART_PutBytes发送剩余的(bsize-sentsize)的数据
				}
				break;
			default:
				break;
		}
		free_ilm(&current_ilm);
	}
}

static kal_bool bird_second_task_init(void)
{
	rj_second_context_p->event_scheduler_ptr = evshed_create("UART_base_timer", MOD_BIRD2, 0, 254 );
	return KAL_TRUE;
}

kal_bool bird_second_task_create(comptask_handler_struct **handle)
{
	static const comptask_handler_struct bird_second_task_info = 
		{
			bird_second_task_main,
			bird_second_task_init,
			NULL,
			NULL,
			NULL
		};
	*handle = (comptask_handler_struct*)&bird_second_task_info;
	return KAL_TRUE;
}

void Rj_second_start_timer(kal_uint8 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	kal_uint32 temp = period * 13 / 60 + 1;    /* ~= x 1/4.615 */
	//kal_prompt_trace(MOD_SOC,"Rj_start_timer period ,temp %d,%d",period,temp);
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//	TRACE_P_3D("Rj_start_timer");
	rj_second_timer_table[timer_id].callback_func = timer_expiry;
	rj_second_timer_table[timer_id].arg = arg;
	rj_second_timer_table[timer_id].event_id = evshed_set_event(
                                            rj_second_context_p->event_scheduler_ptr,
                                            (kal_timer_func_ptr) rj_second_timer_expiry_callback,
                                            (void*)timer_id,
                                            temp /** KAL_TICKS_10_MSEC */ );
}

void Rj_second_stop_timer(kal_uint8 timer_id)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (rj_second_timer_table[timer_id].event_id != NULL)
	{
		evshed_cancel_event(rj_second_context_p->event_scheduler_ptr, &(rj_second_timer_table[timer_id].event_id));
		rj_second_timer_table[timer_id].event_id = NULL;
	}
}
