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

kal_bool defend_switch_status=KAL_FALSE; 
kal_bool  b_defend_switch_status = KAL_FALSE;

void Bird_C9_CMD1_INQUIRE(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_C9_CMD1_INQUIRE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_C9_CMD1_INQUIRE");	  
}
void Bird_C9_CMD2_INQUIRE(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_C9_CMD2_INQUIRE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_C9_CMD2_INQUIRE");	  
}
void Bird_C9_CMD3_INQUIRE(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_C9_CMD3_INQUIRE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_C9_CMD3_INQUIRE");	  
}
void Bird_C9_CMD4_INQUIRE(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_C9_CMD4_INQUIRE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_C9_CMD4_INQUIRE");	  
}
void Bird_ecu_data_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_DATA_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_unlock_electric_motor_alarm");

}
void defend_switch_on(void)
{
       ilm_struct       *aux_ilm = NULL;

       b_defend_switch_status = KAL_TRUE;	
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_DEFEND_OPEN, NULL);       
	msg_send_ext_queue(aux_ilm);
      RJ_GPS_log("defend_switch_on ");

}
//���������Ϣ
void mute_defend_switch_on(void)
{
       ilm_struct       *aux_ilm = NULL;
	
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_MUTE_DEFEND_OPEN, NULL);       
	msg_send_ext_queue(aux_ilm);
      RJ_GPS_log("mute_defend_switch_on ");

}
void defend_switch_off(void)
{
    kal_uint32 ntimer = 30000;	
    ilm_struct       *aux_ilm = NULL;

       b_defend_switch_status = KAL_FALSE;   
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_DEFEND_CLOSED, NULL);       
	msg_send_ext_queue(aux_ilm);     
       RJ_GPS_log("defend_switch_off ");
}


void ECU_connect_alarm(void)
{
    kal_uint32 ntimer = 30000;	
    ilm_struct       *aux_ilm = NULL;

       b_defend_switch_status = KAL_FALSE;   
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_ECU_CONNECT_ALARM, NULL);       
	msg_send_ext_queue(aux_ilm);     
       RJ_GPS_log("ECU_connect_alarm ");
}

void ECU_disconnect_alarm(void)
{
    kal_uint32 ntimer = 30000;	
    ilm_struct       *aux_ilm = NULL;

       b_defend_switch_status = KAL_FALSE;   
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_ECU_DISCONNECT_ALARM, NULL);       
	msg_send_ext_queue(aux_ilm);     
       RJ_GPS_log("ECU_disconnect_alarm ");
}

void rj_ecu_running_alarm(void)
{

      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_RUNNING, NULL);       
	msg_send_ext_queue(rj_ilm);
   //   TRACE_P_3D("rj_3d_sensor_running_alarm");    
      RJ_GPS_log("rj_ecu_running_alarm");
}

void rj_ecu_static_alarm(void)
{

      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_STATIC, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("rj_ecu_static_alarm");
}
/***********************��  ����********************************************/
void Bird_vibrate_is_on_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_VIBRATE_IS_ON, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("Bird_vibrate_is_on_alarm");

}


/**********************�ж���������ϵͳ״̬********************************************/
void Bird_electric_motor_disconnect_status(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_DISCONNECT_MOTOR_STATUS, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_electric_motor_disconnect_status");

}
/**********************��������ϵͳ����״̬********************************************/
void Bird_electric_motor_normal_status(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_NORMAL_MOTOR_STATUS, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_electric_motor_normal_status");

}

void Bird_speed_limit_on(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SPEED_LIMIT_ON, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_speed_limit_on");

}
void Bird_speed_limit_off(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SPEED_LIMIT_OFF, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_speed_limit_off");

}
void Bird_ECU_ACC_on(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_ACC_ON, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_ACC_on");

}
void Bird_ECU_ACC_off(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_ACC_OFF, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_ACC_off");
}
void Bird_ECU_start(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_START, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_start");

}
//������ģʽ
void Bird_soft_start(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SOFT_START, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_soft_start");

}
//Ӳ����
void Bird_hard_start(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_HARD_START, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_hard_start");

}
//��������
void Bird_speed_start(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SPEED_START, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_speed_start");

}
//�Զ�������
void Bird_selfdefine_start(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SELFDEFINE_START, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_selfdefine_start");

}
void Bird_ECU_close(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_CLOSE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_close");

}
//�������Ϣ
void Bird_ecu_charging_on(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_CHARGING_ON, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ecu_charging_on");

}
//δ�����Ϣ
void Bird_ecu_uncharging(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_ECU_UNCHARGING, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ecu_uncharging");

}
//ECU�·�������Ϣ
void Bird_ECU_sent_restart(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_RESTART_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_sent_restart");

}
//ECU�·��ػ���Ϣ
void Bird_ECU_sent_close(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_CLOSE_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_sent_close");

}
//ECU�·�������Ϣ
void Bird_ECU_sent_playtone(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_PLAYTONE_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_sent_playtone");

}
//ECU�·�������Ȩ��Ϣ
void Bird_ECU_bt_accredit(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_BT_ACCREDIT, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_bt_accredit");

}
//���յ�A0 ָ���·���Ϣ
void Bird_ECU_A0_cmd_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_A0_CMD_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_A0_cmd_alarm");

}

//���յ�C0 0B ָ���·���Ϣ
void Bird_ECU_battary_cmd_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_BATTERY_CMD_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_battary_cmd_alarm");

}

//���յ�BT 0B MAC ָ���·���Ϣ
void Bird_BT_MAC_data_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_BT_MAC_DATA_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_BT_MAC_data_alarm");

}
//���յ�C0 0A ָ���·���Ϣ
void Bird_ECU_hardware_config_cmd_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_HARDWARE_CONFIG_CMD_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_battary_cmd_alarm");

}

//���յ�C0 05 ָ���·���Ϣ
void Bird_ECU_selfdefine_cmd_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_SELFDEFINE_DATA_CMD, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_selfdefine_cmd_alarm");

}
//���յ�C0 03 ָ���·���Ϣ
void Bird_ECU_fault_cmd_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_FAULT_CMD_ALARM, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_battary_cmd_alarm");

}

//���յ�������Ӧ�� ָ���·���Ϣ
void Bird_ECU_bt_sensor_open(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_BT_SENSOR_OPEN, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_bt_sensor_open");

}

//���յ�������Ӧ�� ָ���·���Ϣ
void Bird_ECU_bt_sensor_close(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_BT_SENSOR_CLOSE, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_bt_sensor_close");

}

void Bird_remoter_learn_ok(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_REMOTER_LEARN_SUCCESS, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_remoter_learn_ok");
}
//���յ�C0 06 ָ���·���Ϣ
void Bird_ECU_alarm_cmd(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_ALARM_CMD, NULL);       
	msg_send_ext_queue(rj_ilm); 
      RJ_GPS_log("Bird_ECU_battary_cmd_alarm");

}
/***************************�͵籨��begin************************************************/
void rj_low_volt_alarm(void)
{

      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_RJ_LOW_VOLT, NULL);       
	msg_send_ext_queue(rj_ilm);
      TRACE_P_3D("rj_low_volt_alarm");    



}

/****************************�͵籨��end**************************************************/

/***********************�˶�/��ֹ����begin********************************************/
void rj_3d_sensor_running_alarm(void)
{

      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_NORMAL_RUNNING, NULL);       
	msg_send_ext_queue(rj_ilm);
      TRACE_P_3D("rj_3d_sensor_running_alarm");    
      RJ_GPS_log("rj_3d_sensor_running_alarm");




}

void rj_3d_sensor_static_alarm(void)
{

      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_NORMAL_STATIC, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("rj_3d_sensor_static_alarm");
      TRACE_P_3D("rj_3d_sensor_static_alarm");    

}
void rj_3d_sensor_collision_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_NORMAL_COLLISION, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("rj_3d_sensor_collision_alarm");
}
/***********************sos on  ����********************************************/
void Bird_sos_is_on_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SOS_IS_ON, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("Bird_sos_is_on_alarm");

}
/***********************sos off  ����********************************************/
void Bird_sos_is_off_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SOS_IS_OFF, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("Bird_sos_is_off_alarm");

}
/***********************��������  ����********************************************/
void Bird_az_is_close_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_AZ_CLOSE, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("Bird_az_is_close_alarm");

}
/***********************������ ����********************************************/
void Bird_az_is_open_alarm(void)
{
      ilm_struct       *rj_ilm = NULL;

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_AZ_OPEN, NULL);       
	msg_send_ext_queue(rj_ilm);
      RJ_GPS_log("Bird_az_is_open_alarm");

}

void acc_delay_defense()
{
	 ilm_struct       *aux_ilm = NULL; //lrf
	kal_prompt_trace(MOD_SOC,"ACC set defense Acc off");  
	if(!yd_tk001_get_defense() && !get_Acc_onoff())
	{
		kal_prompt_trace(MOD_SOC,"ACC set defense Acc off");  
		DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_KEY_CLOSED, NULL);
		msg_send_ext_queue(aux_ilm);
	}		
}

void ACC_SWITCH_ON(void)
{
     ilm_struct       *aux_ilm = NULL;
     BIRD_id_struct    *bird_id_data = NULL; //lrf


     bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);

      bird_id_data->para_setting_id = 1;  
     
     DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_AUX_KEY_OPEN, bird_id_data);       
      msg_send_ext_queue(aux_ilm);     
 
  
      RJ_GPS_log("ACC_SWITCH_ON ");

}

void ACC_SWITCH_OFF(void)
{
     ilm_struct       *aux_ilm = NULL;
     BIRD_id_struct    *bird_id_data = NULL; //lrf

      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 0;  
      DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_ACC_KEY_CLOSED, bird_id_data);       
      msg_send_ext_queue(aux_ilm);     

     RJ_GPS_log("ACC_SWITCH_OFF ");
}

void BIRD_UART_MESSAGE_DATA(void)
{
     ilm_struct       *aux_ilm = NULL;
     BIRD_id_struct    *bird_id_data = NULL; //lrf

      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 0;  
      DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_BIRD_UART_MESSAGE_DATA, bird_id_data);       
      msg_send_ext_queue(aux_ilm);     

     TRACE_P_GPS("BIRD_UART_MESSAGE_DATA ");
}

