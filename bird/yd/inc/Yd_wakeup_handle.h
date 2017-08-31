#ifndef YD_WAKEUP_HANDLE_H
#define YD_WAKEUP_HANDLE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void yd_init_wakeup_param();
/*描述：对唤醒参数的初始化*/

void bird_set_wakeup_value(U8 value);
/*描述：设置唤醒控制位*/

U8 bird_get_wakeup_value(void);
/*描述：获取唤醒控制位*/

void bird_set_wakeupt_value(U32 value);
/*描述：设置休眠发送心跳控制值*/

U32 bird_get_wakeupt_value(void);
/*描述：获取休眠发送心跳控制值*/

void Yd_run_for_wakeup_device();
/*描述：设备唤醒函数*/

void yd_tk001_wakeup_function();
/*描述：唤醒处理函数*/


#endif
