/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD GPS station data processing function header files
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/
#ifdef RJ_GPS_APP_AGPS

#include "Bird_app.h"

/* cell info */
typedef struct
{
        kal_uint16 mcc;                                                       /* MCC(PLMN) */
        kal_uint16 mnc;                                                       /* MNC(PLMN) */
        kal_uint16 lac;                                                         /* LAC */
        kal_uint16 cell_id;                                                   /* cell ID */
        kal_uint8 bsic;                                                         /* BSIC */
        kal_uint16 arfcn;                                                     /* ARFCN */
        kal_int16 rx_level;                                                  /* Rx Level */
        
} nbr_Cell_Info_struct;

extern nbr_Cell_Info_struct rj_agps_send_data;
extern void RJ_GPS_GetAGPSData();
extern void RJ_GPS_AGPS_Start();
#endif
