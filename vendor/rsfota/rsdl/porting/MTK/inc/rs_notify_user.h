#ifndef _RS_NOTIFY_USER_H_
#define _RS_NOTIFY_USER_H_

#include "rs_datatype.h"
#include "rs_sdk_api.h"
#include "rs_state.h"




#define USER_CHANGE_STATE			1
#define USER_DOWNLOAD_PROGRESS		2
#define USER_DOWNLOAD_BREAK			3
#define USER_STARTED				4

#ifdef __cplusplus
  extern "C"
  {
#endif


/**
����˵������������ָʾ 
����˵������current����ʾ��ǰ�Ѿ����ص�����
  ��total����ʾ����
����ֵ��VOID
*/
void rs_cb_progress(rs_s32 current, rs_s32 total);

/**
����˵��������֮��,����ϴο���������ĳ��״̬����,��ô����֮�������û�ȥ����
����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_break_state( RS_FWDL_BREAK_STATE state);

/**
����˵��������ģ��ִ��״̬֪ͨ�����翪ʼ����ʱ��ᷢһ����Ϣ����ʼ���ص�ʱ��ᷢһ����Ϣ
����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_state(RS_FWDL_STATE state, rs_s32 code);

/**
����˵������ʾOTA�ĺ���ģ���Ѿ��������
����˵����
����ֵ��VOID
*/
void rs_cb_started();

#ifdef __cplusplus
  }
#endif

#endif