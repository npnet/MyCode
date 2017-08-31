/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   YD business data processing module header file
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/


#include "Bird_app.h"

extern void bird_soc_sendlogin(void);
extern void bird_soc_sendheart(void);
extern void bird_soc_sendpos();
extern void bird_soc_sendsleepstatue();
extern void bird_soc_sendstate(S8 nstate);
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern void bird_soc_sendlc(U8 type);
extern void bird_search_param_res(U8 *params,U8 *params2);
extern void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id);
extern void bird_set_param_res(U8 *params,U8 *params2);
extern void bird_soc_cq_res();
extern void bird_soc_set_sf_res(U8 rtn);
extern void bird_soc_set_cf_res(U8 rtn);
extern void bird_soc_T37_send();
extern void bird_soc_uart_T37_send(BIRD_URAT_DATA_ENUM uart_data_type);
extern void bird_soc_T15_res(U8 rtn);
extern void bird_set_soc_log_info(SOC_LOG_INFO categroy,char *str,...);
extern void bird_soc_dealdw(U8 *params1,U8 *params2);
extern void bird_soc_sendjy();
extern void bird_soc_sendzc();
extern void bird_soc_sendgetdw();
extern void yd_tk001_power_off_handler();
