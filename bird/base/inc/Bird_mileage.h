#ifndef BIRD_MILEAGE_H
#define BIRD_MILEAGE_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef S32
typedef signed int S32;
#endif

U8 Yd_get_lc_static_count();
/*描述：获取里程上报控制时持续静止的次数*/

U8 Yd_get_lc_static_time();
/*描述：判断里程上报控制时持续静止时间是否超过4分钟*/

U8 Yd_get_lc_station_count();
/*描述：获取里程上报控制时非休眠状态，卫星持续不可用次数*/

void bird_set_lc_tj();
/*描述：统计里程数*/

S32 bird_get_lc_tj();
/*描述：获取里程数*/

void bird_ini_lc_tj();
/*描述：初始化里程位置信息结构体*/

void Yd_lc();
/*描述：里程上报控制函数*/

#endif
