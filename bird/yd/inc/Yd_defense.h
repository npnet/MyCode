#ifndef YD_DEFENSE_H
#define YD_DEFENSE_H

#include "Kal_general_types.h"

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef MMI_BOOL
#define MMI_BOOL kal_bool
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void yd_init_defense_param();
/*描述：对设防参数的初始化*/

U8 yd_tk001_get_defense();
/*描述：获取设防标志位*/

void bird_set_defense_delay_value(U32 time);
/*描述：设置设防延时时间*/

U32 bird_get_defense_delay_value();
/*描述：获取设防延时时间*/

void yd_tk001_defense_ini();
/*描述：根据当前设防状态删除设防位置信息并设置设防位置信息更新控制位*/

void yd_tk001_set_defense(MMI_BOOL flag);
/*描述：根据当前设防状态设置设防标志位、设防位置信息和控制位*/

void yd_tk001_set_server_defense(U8 flag);
/*描述：设置服务器设防标志位*/

MMI_BOOL yd_tk001_defense_is_set_by_server();
/*描述：判断是否为服务器永久设防*/

void yd_tk001_post_soft_sf();
/*描述：设防函数*/

void bird_set_cf_res();
/*描述：无ECU或有DW23时，服务器撤防指令的处理函数*/

void bird_set_sf_res();
/*描述：无ECU或有DW23时，服务器设防指令的处理函数*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_ecu_defend_to_uart();
/*描述：ECU通知uart设防撤防函数*/

void yd_ecu_defend_to_server(U8 flag);
/*描述：ECU设防撤防结果反馈到服务器的函数*/

void yd_ecu_defend_deal();
/*描述：ECU设防撤防处理函数*/

void bird_ecu_defend_res(U8 defend);
/*描述：有ECU且无DW23时，服务器设防撤防的预处理函数*/
#endif

void bird_set_defense_res(U8 rtn);
/*描述：服务器永久设防处理函数*/

void yd_set_defend_on_msg();
/*描述：DW23中设置驱动设防消息句柄*/

void yd_set_defend_off_msg();
/*描述：DW23中设置驱动撤防消息句柄*/

#ifdef BIRD_ECU_MMI_SUPPORT
void yd_set_uart_defend_on_msg();
/*描述：设置驱动设防消息句柄*/

void yd_set_uart_defend_off_msg();
/*描述：设置驱动撤防消息句柄*/
#endif

void yd_mute_sf();
/*描述：静音设防处理函数*/

void yd_set_mute_defend_on_msg();
/*描述：设置静音设防消息句柄*/

#endif
