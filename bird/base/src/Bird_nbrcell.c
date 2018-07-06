/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD GPS station data processing function
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifdef RJ_GPS_APP_AGPS

#include "Bird_nbrcell.h"
#include "Bird_app.h"

nbr_Cell_Info_struct g_mmi_nbr_service_cell_info[2];
nbr_Cell_Info_struct g_mmi_nbr_neighbor_cell_info[2][6];
nbr_Cell_Info_struct rj_agps_data;
nbr_Cell_Info_struct rj_agps_send_data;

/*-------------------------------------------------------
           发送 MSG_ID_L4C_NBR_CELL_INFO_DEREG_REQ，
           打开网络信息上报
-------------------------------------------------------*/
/*-------------------------------------------------------
            解析AGPS 数据
-------------------------------------------------------*/
void mmi_Cell_Info_parsing_data(void *info, int mod_src)
{
    l4c_nbr_cell_info_ind_struct* cell_info = NULL; //lrf
    int valid_cell;
    int j;

    cell_info = (l4c_nbr_cell_info_ind_struct *)info;

    // serving cell information : PLMN, Cell ID, LAC
    g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mcc = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.serv_info.gci.mcc;
    g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mnc = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.serv_info.gci.mnc;
    g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].lac = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.serv_info.gci.lac;
    g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].cell_id = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.serv_info.gci.ci;

    /*
    RJ_GPS_log("xxx=%d",(mod_src-MOD_L4C));
    RJ_GPS_log("mcc=%d",g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mcc);
    RJ_GPS_log("mnc=%d",g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mnc);
    RJ_GPS_log("lac=%d",g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].lac);
    RJ_GPS_log("cell_id=%d",g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].cell_id);
    */

    memset(&rj_agps_data, 0, sizeof(nbr_Cell_Info_struct));

    rj_agps_data.mcc = g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mcc;
    rj_agps_data.mnc = g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].mnc;
    rj_agps_data.lac = g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].lac;
    rj_agps_data.cell_id = g_mmi_nbr_service_cell_info[mod_src-MOD_L4C].cell_id;

    /*
    RJ_GPS_log("xxx=%d",(mod_src-MOD_L4C));
    RJ_GPS_log("mcc=%d",rj_agps_data.mcc);
    RJ_GPS_log("mnc=%d",rj_agps_data.mnc);
    RJ_GPS_log("lac=%d",rj_agps_data.lac);
    RJ_GPS_log("cell_id=%d",rj_agps_data.cell_id);
    */

    // neighbor cell information : PLMN, Cell ID, LAC, BSIC, Rx Level, Arfcn
    if (cell_info->is_nbr_info_valid)
    {
        for (j = 0; j < cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_num; j++)
        {
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].mcc = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].gci.mcc;
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].mnc = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].gci.mnc;
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].lac = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].gci.lac;
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].cell_id = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].gci.ci;

            if (cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].nbr_meas_rslt_index>= cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_meas_rslt.nbr_meas_num)
            {
                continue;
            }

            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].rx_level = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_meas_rslt.nbr_cells[cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].nbr_meas_rslt_index].rxlev-110;
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].arfcn = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_meas_rslt.nbr_cells[cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].nbr_meas_rslt_index].arfcn;
            g_mmi_nbr_neighbor_cell_info[mod_src-MOD_L4C][j].bsic = cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_meas_rslt.nbr_cells[cell_info->ps_nbr_cell_info_union.gas_nbr_cell_info.nbr_cell_info[j].nbr_meas_rslt_index].bsic;
        }
    }
}

void mmi_Cell_Info_start_req(module_type mod_id)
{
    RJ_GPS_log("mmi_Cell_Info_start_req");

    mmi_frm_set_protocol_event_handler(MSG_ID_L4C_NBR_CELL_INFO_REG_CNF, mmi_Cell_Info_parsing_data, MMI_FALSE);
    mmi_frm_set_protocol_event_handler(MSG_ID_L4C_NBR_CELL_INFO_IND, mmi_Cell_Info_parsing_data, MMI_FALSE);

    mmi_frm_send_ilm(mod_id,MSG_ID_L4C_NBR_CELL_INFO_REG_REQ, NULL, NULL);
}

/*-------------------------------------------------------
           获取需要发送的AGPS 数据
-------------------------------------------------------*/
void RJ_GPS_GetAGPSData()
{
    memset(&rj_agps_send_data, 0, sizeof(rj_agps_send_data));

    rj_agps_send_data.mcc = rj_agps_data.mcc;
    rj_agps_send_data.mnc = rj_agps_data.mnc;
    rj_agps_send_data.lac = rj_agps_data.lac;
    rj_agps_send_data.cell_id = rj_agps_data.cell_id;
}

void bird_set_agps_data(U8* sendBuffer, U8 index)
{
    nbr_Cell_Info_struct agps_array;

    switch(index)
    {
    case 1:
        memcpy(&agps_array, &rj_agps_send_data, sizeof(agps_array));
        break;

    case 2:
        memcpy(&agps_array, &g_mmi_nbr_neighbor_cell_info[0][0], sizeof(agps_array));
        break;

    case 3:
        memcpy(&agps_array, &g_mmi_nbr_neighbor_cell_info[0][1], sizeof(agps_array));
        break;

    default:
        memcpy(&agps_array, &rj_agps_send_data, sizeof(agps_array));
        break;
    }

    memcpy(sendBuffer, ((U8 *)&agps_array.mcc) +1, 1);
    memcpy(sendBuffer+1, ((U8 *)&agps_array.mcc), 1);
    memcpy(sendBuffer+2, ((U8 *)&agps_array.mnc), 1);
    memcpy(sendBuffer+3, ((U8 *)&agps_array.lac)+1, 1);
    memcpy(sendBuffer+4, ((U8 *)&agps_array.lac), 1);
    memcpy(sendBuffer+5, ((U8 *)&agps_array.cell_id)+1, 1);
    memcpy(sendBuffer+6, ((U8 *)&agps_array.cell_id), 1);
}

void RJ_GPS_AGPS_Start()
{
    RJ_GPS_log("RJ_GPS_AGPS_Start");

    mmi_Cell_Info_start_req(MOD_L4C);
}
#endif
