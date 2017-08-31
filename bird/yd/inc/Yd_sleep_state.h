#ifndef YD_SLEEP_STATE_H
#define YD_SLEEP_STATE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif


void yd_init_sleep_param();
/*描述：对休眠标志位进行初始化的函数*/

U8 bird_get_normal_sleep_state();
/*描述：获取休眠状态的函数*/

void bird_set_normal_sleep_state(U8 flag);
/*描述：设置休眠状态的函数*/

U8 bird_is_normal_sleep_on(void);
/*描述：获取插电休眠标志位的函数*/

void bird_set_normal_sleep_flag(U8 flag);
/*描述：设置插电休眠标志位的函数*/

U32 bird_get_normal_sleep_value();
/*描述：获取插电延时休眠时间的函数*/

void bird_set_normal_sleep_value(U32 time);
/*描述：设置插电延时休眠时间的函数*/

U8 bird_is_abnormal_sleep_on(void);
/*描述：获取断电休眠标志位的函数*/

void bird_set_abnormal_sleep_flag(U8 flag);
/*描述：设置断电休眠标志位的函数*/

U32 bird_get_abnormal_sleep_value();
/*描述：获取断电延时休眠时间的函数*/

void bird_set_abnormal_sleep_value(U32 time);
/*描述：设置断电延时休眠时间的函数*/

U8 Yd_is_sleep_on(void);
/*描述：获取休眠标志位(插电则插电休眠标志，断电则断电休眠标志)*/

void Yd_sleeppos_rs(void);
/*描述：心跳过渡到休眠的处理函数*/

void Yd_sleep_sendheart();
/*描述：休眠发送心跳的函数*/

void Yd_stop_sleepTimer();
/*描述：关闭休眠定时器的函数*/

void Yd_normal_sleep();
/*描述：休眠处理函数*/

#endif
