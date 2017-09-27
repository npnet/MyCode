/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */
#ifndef RS_SOCKET_H
#define RS_SOCKET_H

#include "rs_datatype.h"

typedef unsigned int				rs_socket_handle;
#define RS_SOC_INVALID_SOCKET_H     ((rs_socket_handle)-1)

#define SOCKET_MSG_COME	2

/**
函数说明：socket 模块初始化
参数说明：无 

返回值：成功RS_ERR_OK（值为0），其它为失败
特殊说明:例如MTK平台的socket在使用之前需要创建相应的account、消息回调接口等等。
*/
typedef  void (*RS_SOCKET_MSG_HANDLER_CALLBACK)(void*handle, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2);
rs_s32 rs_socket_gethostbyname(const rs_s8 *uri);
rs_s32 rs_socket_init(RS_SOCKET_MSG_HANDLER_CALLBACK callback, void* handle);

/**
函数说明：获取socket模式
参数说明：无 
返回值：返回0，表示正常模式，类似于windows或linuxsocket使用方式；1表示回调模式，connect、send、recv、gethostbyname都需要通过回调来知道是否执行结果
*/

rs_s32 rs_socket_getMode();

/**
函数说明：创建socket句柄
参数说明：无 
返回值：成功返回socket句柄，失败返回-1
*/
rs_socket_handle rs_socket_create();

/**
函数说明：连接服务器
参数说明：socketHandle socket句柄
		 Uri 要连接服务器的地址 
返回值：正常模式，1表示成功，-1表示失败
	    回调模式，1表示成功，0表示回调触发，-1表示失败
*/
rs_s32 rs_socket_connect(rs_socket_handle socketHandle);

/**
函数说明：发送数据 
参数说明：sockHndl socket句柄
		 sendData 需要发送的数据
		 size 需要发送的数据的长度 
返回值：正常模式，>0表示成功发送的数量，0表示socket没有准备好，还需要继续发送，-1表示失败
	    回调模式，>0表示成功发送的数量，0表示socket没有发送完成等待回调，-1表示失败
*/
rs_s32 rs_socket_send(rs_socket_handle sockHndl, const rs_s8* sendData, rs_s32 size);

/**
函数说明：接收数据 
参数说明：sockHndl socket句柄
		 recvBuf 接收数据的缓冲区
		 size 缓冲区长度
返回值：正常模式，>0表示成功接收的数量，0表示socket没有可以接收的数据，-1表示失败
	    回调模式，>0表示成功发送的数量，0表示socket没有可以接收的数据等待回调，-1表示失败
*/
rs_s32 rs_socket_recv(rs_socket_handle sockHndl, rs_s8 *buffer, rs_s32 bufLen);

/**
函数说明：轮训数据，主要是在recv的时候使用
参数说明：sockHndl socket句柄
		 timeOut 轮训超时时间
返回值：正常模式，1表示有数据可以读取，0超时，-1表示失败
	    回调模式，不需要select 
*/
rs_s32 rs_socket_select(rs_socket_handle sockHndl, rs_s32 nTimeOut);

/**
函数说明：关闭socket
参数说明：sockHndl socket句柄
返回值：正常模式，0表示成功，-1表示失败
	    回调模式，0表示成功，-1表示失败
*/
rs_s32 rs_socket_close(rs_socket_handle sockHndl);

/**
函数说明：只有当socket回调触发的时候才会去读取数据
参数说明：sockHndl socket句柄
返回值：1表示是，0表示当接收到数据之后，还会主动在读取数据一次，直到block
*/
rs_s32 rs_socket_only_wait_callback();

void rs_socket_wait_moment();

#endif
