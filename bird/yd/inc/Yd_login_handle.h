#ifndef YD_LOGIN_HANDLE_H
#define YD_LOGIN_HANDLE_H


#ifndef U8
typedef unsigned char U8;
#endif


U8 bird_get_islogin();
/*��������ȡ��½�ɹ���־λ*/

void Yd_login();
/*�������ն˵�¼��ƽ̨�Ĵ�����*/

void Yd_login_res(U8 *datetime,U8 *flag);
/*������ƽ̨���صĵ�½��Ϣ������*/


#endif
