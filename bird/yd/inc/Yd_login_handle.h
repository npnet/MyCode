#ifndef YD_LOGIN_HANDLE_H
#define YD_LOGIN_HANDLE_H


#ifndef U8
typedef unsigned char U8;
#endif


U8 bird_get_islogin();
/*描述：获取登陆成功标志位*/

void Yd_login();
/*描述：终端登录到平台的处理函数*/

void Yd_login_res(U8 *datetime,U8 *flag);
/*描述：平台返回的登陆消息处理函数*/


#endif
