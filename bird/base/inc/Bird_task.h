
#include "kal_public_api.h"
#include "kal_general_types.h"
#include "MMIDataType.h"


typedef enum
{
    RJ_TIMER = 0,
    /*
    Bird_task_login_Timer,
    Bird_task_logintxbox_Timer,
    Bird_task_calibtime_Timer,
    Bird_task_24calib_Timer,
       Bird_task_main_Timer,
       Bird_task_heart_Timer,
       BIRD_TASK_GPRS_TIMER_START,
       BIRD_TASK_VIBRATION_TIMER_COUNT,
       BIRD_TASK_YD_NORMAL_SLEEP_TIMER,
       BIRD_TASK_SOCKET_SEND,
       BIRD_TASK_SOCKET_RECON,
       RJ_TASK_GPS_RESET_TIMER,
       BIRD_TASK_SET_DEFENSE_DELAY_TIMER,
       BIRD_TASK_POWER_OFF_DELAY,
       BIRD_TASK_DW_TIMER,
       BIRD_TASK_YD_DEEP_SLEEP_TIMER,
       Bird_task_sleep_dinisocket_Timer,
       BIRD_TASK_REST_TIMER,
       BIRD_TASK_24REST_TIMER,
       BIRD_TASK_LC_TJ_TIMER,
       BIRD_TASK_POS_WAIT_TIMER,
       BIRD_TASK_SLEEPPOS_WAIT_TIMER,
       BIRD_TASK_ALARM_LOW_WAIT_TIMER,
       BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER,
       BIRD_TASK_ALARM_MOVE_WAIT_TIMER,
       BIRD_TASK_ALARM_RUN_WAIT_TIMER,
       BIRD_TASK_SEAL_COUNT_WAIT_TIMER,
       BIRD_TASK_HEART_WAIT_TIMER,
       Bird_task_ringoff_timer,
       BIRD_TASK_DELAY_SENDLC_TIMER,
       BIRD_TASK_OC_MAIN_HEART_TIMER,
       BIRD_TASK_OC_RUN_TIMER,
       */
    MAX_NUM_OF_RJ_TIMER
} rj_timer_enum;


/*************************************************/
/*SECOND TASK OF BIRD 	in 2017-08-09*/
/*Rj_second_start_timer & Rj_second_stop_timer*/
/*************************************************/
typedef enum
{
    RJ_SECOND_TIMER,

    MAX_RJ_SECOND_TIMER
} rj_second_timer_enum;
typedef struct
{
    eventid event_id;
    kal_timer_func_ptr callback_func;
    void *arg;
} rj_timer_table_struct;



typedef struct
{
    //gps_state_enum gps_state;
    event_scheduler *event_scheduler_ptr;
} rj_context_struct;

extern void Rj_start_timer(U16 timer_id, U32 period, oslTimerFuncPtr timer_expiry, void *arg);
extern void Rj_stop_timer(U16 timer_id);
