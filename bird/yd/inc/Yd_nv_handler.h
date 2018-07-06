#ifndef YD_NV_HANDLER_H
#define YD_NV_HANDLER_H


#ifndef kal_uint8
typedef unsigned char kal_uint8;
#endif

void yd_tk001_set_record_from_NV();
/*描述：将yd_tk001_info结构体数据保存至NV*/

void yd_tk002_set_record_from_NV();
/*描述：将yd_tk002_info结构体数据保存至NV*/

#ifdef BIRD_BT_SUPPORT
void yd_tk003_set_record_from_NV();
/*描述：将yd_tk003_info结构体数据保存至NV*/
#endif

void yd_tk001_get_record_from_NV();
/*描述：从NV中读取数据存入yd_tk001_info*/

void yd_tk002_get_record_from_NV();
/*描述：从NV中读取数据存入yd_tk002_info*/

#ifdef BIRD_BT_SUPPORT
void yd_tk003_get_record_from_NV();
/*描述：从NV中读取数据存入yd_tk003_info*/
#endif

void yd_tk004_get_record_from_NV();
/**/

void yd_tk001_set_default_record_to_NV();
/*描述：将初始化后的yd_tk001_info结构体数据保存至NV*/

void yd_send_save_nv_msg();
/*描述：发送将yd_tk001_info中数据写入NV的消息*/

void yd_set_save_nv_msg();
/*描述：设置将yd_tk001_info中数据写入NV的消息句柄*/

#endif
