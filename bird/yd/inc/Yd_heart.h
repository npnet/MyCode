#ifndef YD_HEART_H
#define YD_HEART_H

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

void bird_set_heart_interval_value(U32 interval);
/*�����������������ʱ��*/

U32 bird_get_heart_interval_value(void);
/*��������ȡ�������ʱ��*/

void Yd_heart_res(U8 *datetime);
/*������ƽ̨�ظ�������Ϣ�Ĵ�����*/

void Yd_heart();
void Yd_tbox_heart();
/*����������������*/


#endif
