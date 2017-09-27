/*
 *
 * ��Ȩ(c) 2015 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2015 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */
#ifndef RS_SOCKET_H
#define RS_SOCKET_H

#include "rs_datatype.h"

typedef unsigned int				rs_socket_handle;
#define RS_SOC_INVALID_SOCKET_H     ((rs_socket_handle)-1)

#define SOCKET_MSG_COME	2

/**
����˵����socket ģ���ʼ��
����˵������ 

����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
����˵��:����MTKƽ̨��socket��ʹ��֮ǰ��Ҫ������Ӧ��account����Ϣ�ص��ӿڵȵȡ�
*/
typedef  void (*RS_SOCKET_MSG_HANDLER_CALLBACK)(void*handle, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2);
rs_s32 rs_socket_gethostbyname(const rs_s8 *uri);
rs_s32 rs_socket_init(RS_SOCKET_MSG_HANDLER_CALLBACK callback, void* handle);

/**
����˵������ȡsocketģʽ
����˵������ 
����ֵ������0����ʾ����ģʽ��������windows��linuxsocketʹ�÷�ʽ��1��ʾ�ص�ģʽ��connect��send��recv��gethostbyname����Ҫͨ���ص���֪���Ƿ�ִ�н��
*/

rs_s32 rs_socket_getMode();

/**
����˵��������socket���
����˵������ 
����ֵ���ɹ�����socket�����ʧ�ܷ���-1
*/
rs_socket_handle rs_socket_create();

/**
����˵�������ӷ�����
����˵����socketHandle socket���
		 Uri Ҫ���ӷ������ĵ�ַ 
����ֵ������ģʽ��1��ʾ�ɹ���-1��ʾʧ��
	    �ص�ģʽ��1��ʾ�ɹ���0��ʾ�ص�������-1��ʾʧ��
*/
rs_s32 rs_socket_connect(rs_socket_handle socketHandle);

/**
����˵������������ 
����˵����sockHndl socket���
		 sendData ��Ҫ���͵�����
		 size ��Ҫ���͵����ݵĳ��� 
����ֵ������ģʽ��>0��ʾ�ɹ����͵�������0��ʾsocketû��׼���ã�����Ҫ�������ͣ�-1��ʾʧ��
	    �ص�ģʽ��>0��ʾ�ɹ����͵�������0��ʾsocketû�з�����ɵȴ��ص���-1��ʾʧ��
*/
rs_s32 rs_socket_send(rs_socket_handle sockHndl, const rs_s8* sendData, rs_s32 size);

/**
����˵������������ 
����˵����sockHndl socket���
		 recvBuf �������ݵĻ�����
		 size ����������
����ֵ������ģʽ��>0��ʾ�ɹ����յ�������0��ʾsocketû�п��Խ��յ����ݣ�-1��ʾʧ��
	    �ص�ģʽ��>0��ʾ�ɹ����͵�������0��ʾsocketû�п��Խ��յ����ݵȴ��ص���-1��ʾʧ��
*/
rs_s32 rs_socket_recv(rs_socket_handle sockHndl, rs_s8 *buffer, rs_s32 bufLen);

/**
����˵������ѵ���ݣ���Ҫ����recv��ʱ��ʹ��
����˵����sockHndl socket���
		 timeOut ��ѵ��ʱʱ��
����ֵ������ģʽ��1��ʾ�����ݿ��Զ�ȡ��0��ʱ��-1��ʾʧ��
	    �ص�ģʽ������Ҫselect 
*/
rs_s32 rs_socket_select(rs_socket_handle sockHndl, rs_s32 nTimeOut);

/**
����˵�����ر�socket
����˵����sockHndl socket���
����ֵ������ģʽ��0��ʾ�ɹ���-1��ʾʧ��
	    �ص�ģʽ��0��ʾ�ɹ���-1��ʾʧ��
*/
rs_s32 rs_socket_close(rs_socket_handle sockHndl);

/**
����˵����ֻ�е�socket�ص�������ʱ��Ż�ȥ��ȡ����
����˵����sockHndl socket���
����ֵ��1��ʾ�ǣ�0��ʾ�����յ�����֮�󣬻��������ڶ�ȡ����һ�Σ�ֱ��block
*/
rs_s32 rs_socket_only_wait_callback();

void rs_socket_wait_moment();

#endif
