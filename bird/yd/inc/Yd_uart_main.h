/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      Yd_uart.h
  Author:       Version:        Date:
  Description:   YD uart function header files
  Others:
  Function List:
  History:
    1. Date:2017-01-10 first creation
       Author:
       Modification:
****************************************************************************/
#ifdef BIRD_ECU_MMI_SUPPORT

#include "Bird_std.h"

#define NORES_TIME 2 /* 无响应时间 */
#define NORES_RESENDTIMES 3 /* 无响应重发次数 */
#define TO_SERVER_NOK 0
#define TO_SERVER_OK 1
#define TO_SERVER_NORES 2
#define UART_RATIO 0.4

typedef enum
{
    BIRD_UART_GPS_SEARCH = 0,
    BIRD_UART_GPS_LOCA_FAIL,
    BIRD_UART_GPS_LOCA_SUC,
    BIRD_UART_GPS_CLOSE
} BIRD_UART_GPS_ENUM;

typedef enum
{
    BIRD_UART_GSM_SEARCH = 0,
    BIRD_UART_GSM_NOSIGNAL,
    BIRD_UART_GSM_OK
} BIRD_UART_GSM_ENUM;

typedef enum
{
    BIRD_UART_SIM_NOK = 0,
    BIRD_UART_SIM_OK
} BIRD_UART_SIM_ENUM;

extern void Yd_ecu_findcar_res(U32 id);
extern void Yd_ecu_limit_speed_res(U8 value,U32 id);
extern void Yd_ecu_lock_res(U8 *flag);
extern void Yd_ecu_openaz_res(U32 id);
extern void Yd_ecu_carcheck_res(U32 id);
extern void Yd_ecu_carstart_res(U32 id,U8 flag);

extern U8 get_uart_charging_status();
extern U8 get_uart_connect_status();
extern U8 get_uart_battery_percent_ext();
extern void yd_uart_search_c9_04();
extern void yd_uart_run_status(U8 run_state);
extern U8* yd_get_uart_model(BIRD_URAT_DATA_ENUM uart_data_type);
extern U8* yd_get_T37_vendor();
extern U8* yd_get_uart_buf(BIRD_URAT_DATA_ENUM uart_data_type);
extern U8 yd_get_uart_buf_length(BIRD_URAT_DATA_ENUM uart_data_type);
extern U8 get_car_status();
extern void Yd_set_uart_msg_handler();

#endif
