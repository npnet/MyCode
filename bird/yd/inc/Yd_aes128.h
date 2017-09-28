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

 //* Number of columns (32-bit words) comprising the State. For this standard, Nb = 4.
#define Nb 4 //分组长度

 //Number of 32-bit words comprising the Cipher Key. For this standard, Nk = 4, 6, or 8.
#define Nk 4 //密钥长度

//* Number of rounds, which is a function of  Nk  and  Nb (which is * fixed). For this standard, Nr = 10, 12, or 14.
#define Nr 10 //轮数 

extern void bird_set_aes128_encode(U8 *data,U16 l);
extern void bird_set_inv_aes128_encode(U8 *data,U16 l);