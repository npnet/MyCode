/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:
  Author:       Version:        Date: 2013-05-29
  Description:   YD business data processing module header file
  Others:
  Function List:
  History:
    1. Date:
       Author:
       Modification:
****************************************************************************/


#include "Bird_app.h"

/*���ݵ�Ԫ���ܷ�ʽ:
0x01�����ݲ����ܣ�
0x02�����ݾ��� RSA �㷨���ܣ�
0x03�����ݾ��� AES128 λ�㷨���ܣ�
0xFE ��ʾ�쳣��
0xFF ��ʾ��Ч������Ԥ��
*/
#define ENCRYPT_NULL 0x01
#define ENCRYPT_RSA 0x02
#define ENCRYPT_AES 0x03
#define ENCRYPT_ERROR 0xfe
#define ENCRYPT_INVALID 0xff

void bird_soc_send_tboxsearch(U8* time,U8 count, U8* param, U8 rtn);
void bird_soc_send_tboxset(U8* time,U8 count, U8* param, U8 rtn);
void bird_soc_send_tboxcontrol(U8* time,U8 count, U8* param, U8 rtn);

