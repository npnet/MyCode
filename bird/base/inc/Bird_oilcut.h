#ifndef BIRD_OILCUT_H
#define BIRD_OILCUT_H


#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif


void bird_init_oilcut_param(void);
/*描述：对断油指令参数的初始化*/

void bird_set_ocin_state(U8 flag);
/*描述：设置断油消息标识(0位恢复油路、1为断开油路)*/

U8 bird_get_ocin_state(void);
/*描述：获取断油消息标识*/

void bird_set_oc_instruct(U32 instruct);
/*描述：设置断油指令ID*/

U32 bird_get_oc_instruct(void);
/*描述：获取断油指令ID*/

void bird_set_oc_exec_state(U8 flag);
/*描述：设置断油消息执行状态*/

U8 bird_get_oc_exec_state(void);
/*描述：获取断油消息执行状态*/

void bird_set_oc_main_heart_time(U8 flag);
/*描述：设置断油指令查询的延时开启时间*/

U8 bird_get_oc_main_heart_time(void);
/*描述：获取断油指令查询的延时开启时间*/

void bird_ocin_init(void);
/*描述：初始化驱动断油状态*/

void bird_set_dy_state(U8 flag);
/*描述：设置驱动断油状态*/

void Yd_oc_heart_handle(void);
/*描述：断油指令查询函数*/

void bird_oilcut_accsleep_handler(void);
/*描述：ACC休眠时对断油指令的处理函数*/

void bird_oc_logic_res(void);
/*描述：断油消息预处理函数*/

void bird_ocin_state_res(U8 flag);
/*描述：断油指令设置断油消息标识的函数*/

#endif
