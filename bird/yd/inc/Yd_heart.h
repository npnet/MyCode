#ifndef YD_HEART_H
#define YD_HEART_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void bird_set_heart_interval_value(U32 interval);
/*描述：设置心跳间隔时间*/

U32 bird_get_heart_interval_value(void);
/*描述：获取心跳间隔时间*/

void Yd_heart_res(U8 *datetime);
/*描述：平台回复心跳消息的处理函数*/

void Yd_heart();
void Yd_tbox_heart();
/*描述：心跳处理函数*/


#endif
