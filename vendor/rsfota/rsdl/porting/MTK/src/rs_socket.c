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


#include "rs_socket.h"
#include "rs_error.h"
#include "rs_debug.h"
#include <stdio.h>
#include "rs_std_fun.h"

// MTK header
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "MMI_features.h"
#include "kal_general_types.h"
#include "soc_api.h"
#include "soc_consts.h"
#include "cbm_consts.h"
#include "cbm_api.h"
#include "MMIDataType.h"

sockaddr_struct rs_soc_addr;
unsigned long nHostPort;

RS_SOCKET_MSG_HANDLER_CALLBACK g_socket_callback = RS_NULL;
void *g_rs_handle = RS_NULL;
kal_int8 g_rs_socket_handle = -1;

kal_uint32 fota_data_account = 0;
unsigned char fota_app_id = 0;

// External func
extern kal_uint32 cbm_encode_data_account_id(kal_uint32 acct_id, cbm_sim_id_enum sim_id, kal_uint8 app_id, kal_bool always_ask);
extern kal_int8 cbm_register_app_id(kal_uint8 *app_id);

//#define RS_HTTPS_SUPPORT
#ifdef RS_HTTPS_SUPPORT
#include "Tls_app_enums.h"
#include "tls_api.h"
#include "mmi_rp_app_mre_def.h"

static kal_int32 g_ctx_id = -1;
static tls_cipher_enum ciphers[] = {AES128_SHA, AES256_SHA, RC4_MD5, RC4_SHA, DES_CBC3_SHA};
static kal_uint32 certs[] = {1, 2, 3, 4};


static void error_handling(func_name,error_code)
{
	RS_PORITNG_LOG("[TLS]function_name is %s and error_code= %d\n\r", func_name,error_code);
    return;
}

static void create_tls_context()
{
	rs_s32 ret;
	//TLS_ERR_NO_FREE_CTX            = -2,  /* No free global context slots. */
	//TLS_ERR_INVALID_CONTEXT        = -4,  /* Invalid global context id. */	
	if ((g_ctx_id = tls_new_ctx(TLS_ALL_VERSIONS, TLS_CLIENT_SIDE, MOD_MMI, NULL)) < 0)
	{
 		error_handling("tls_new_ctx",g_ctx_id);
 		return;
 	}
    
	RS_PORITNG_LOG("[TLS]create_tls_context g_ctx_id = %d \n\r" , g_ctx_id);
	if ((ret = tls_set_ciphers(g_ctx_id,
	            		ciphers,
				sizeof(ciphers) / sizeof(tls_cipher_enum))) < 0)
	{
		error_handling("tls_set_ciphers" , ret);
	}
	

	
}

static void delete_tls_context()
{
	rs_s32 ret;
	if(g_ctx_id >= 0){
		ret = tls_delete_ctx(g_ctx_id);
		if(ret < 0){
			error_handling("delete_tls_context",ret);
			return ;
		}

		error_handling("delete_tls_context",ret);
	}
	
	
}

//6.3.4	TLS event handlers
static void fota_tls_ind_handler(void *msg)
{
	//记录创建时的socket id, 进行判断处理,因为其他的消息也会用到
	app_tls_notify_ind_struct *ind = (app_tls_notify_ind_struct*) msg;
	
	RS_PORITNG_LOG("%s start, ind->sock_id[%d]\n\r", __func__, (rs_s8)(ind->sock_id));

	if (g_rs_socket_handle != (rs_s8)(ind->sock_id))
	{
		return;
	}
	RS_PORITNG_LOG("%s start, ind->sock_id[%d] valid\n\r", __func__, (rs_s8)(ind->sock_id));

	switch(ind->event)
	{
	case TLS_HANDSHAKE_READY:
		//app_tls_handshake_ready_ind_hdlr(ind->sock_id, ind->result);
		RS_PORITNG_LOG("[TLS]app_tls_ind_handler TLS_HANDSHAKE_READY\n\r");
		break;

	case TLS_HANDSHAKE_DONE:
		//app_tls_handshake_done_ind_hdlr(ind->sock_id, ind->result);
		RS_PORITNG_LOG("%s TLS_HANDSHAKE_DONE, ind->result=%d\n\r", __func__, ind->result);
		if(ind->result != 0)
		{
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
		}
		else
		{
			soc_close(ind->sock_id);
			RS_PORITNG_LOG("%s, SOC_READ error and soc_close\n\r", __func__);

			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
			}
		}
			
		break;

	case TLS_READ:
		RS_PORITNG_LOG("%s TLS_READ, ind->result=%d\n\r", __func__, ind->result);

		if(ind->result != 0)
		{
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
		}
		else
		{
			soc_close(ind->sock_id);
			RS_PORITNG_LOG("%s, TLS_READ error and soc_close\n\r", __func__);

			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
			}
		}
		
		RS_PORITNG_LOG("%s TLS_READ end\n\r", __func__);
		
		break;

	case TLS_WRITE:
		//app_tls_write_ind_hdlr(ind->sock_id, ind->result);
		RS_PORITNG_LOG("%s TLS_WRITE, ind->result=%d\n\r", __func__, ind->result);
		break;

	//case TLS_CLOSE:
		//kal_prompt_trace(MOD_TLS, "[TLS]app_tls_ind_handler TLS_CLOSE");
		//dbg_print( "[TLS]app_tls_ind_handler TLS_CLOSE\n\r");
		//app_tls_close_ind_hdlr(ind->sock_id, ind->result);
		//break;
	}
}

#endif

// gethostbyname handler
MMI_BOOL fota_get_ip_handle(void* inMsg)
{
	app_soc_get_host_by_name_ind_struct* dns_ind = (app_soc_get_host_by_name_ind_struct*)inMsg;
	kal_uint8 domain_app_id;
	
	domain_app_id = cbm_get_app_id(dns_ind->account_id);
	if(domain_app_id != fota_app_id)
	{
		RS_PORITNG_LOG("%s, recv dns but app id not match, so discard. recv domain_app_id=%d, fota_app_id=%d\n\r", __func__, domain_app_id, fota_app_id);
		return KAL_FALSE;
	}
	
	RS_PORITNG_LOG("%s,dns_ind->result=%d, dns->ind->request_id=%d, fota_app_id=%d\n\r", __func__, dns_ind->result,dns_ind->request_id,fota_app_id);
	
		if (dns_ind && dns_ind->result != FALSE)
		{
			int i = 0;
			
			rs_memset(&rs_soc_addr, 0x00, sizeof(rs_soc_addr));
			rs_memcpy(rs_soc_addr.addr, dns_ind->addr, dns_ind->addr_len);
			rs_soc_addr.addr_len = dns_ind->addr_len;
			rs_soc_addr.port = nHostPort;
			rs_soc_addr.sock_type = SOC_SOCK_STREAM;
			
			RS_PORITNG_LOG("%s addr_len=%d, port[%d]\n\r", __func__, dns_ind->addr_len, nHostPort);
			
			for(i = 0; i < dns_ind->addr_len; i++)
			{
				RS_PORITNG_LOG("%s addr[%d]=%d,%d\r\n", __func__, i, rs_soc_addr.addr[i], dns_ind->addr[i]);
			}
			
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
			
			return MMI_TRUE;
	  } 
	  else
	  {// dns analyse error.
	  		#ifdef RS_HTTPS_SUPPORT
			delete_tls_context();
			#endif 
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
			}
			
			RS_PORITNG_LOG("%s get host from dns return fail\r\n", __func__);
		}
	
	 return MMI_FALSE;
}

// send recv connect handler
void fota_event_handle(void *msg_ptr)
{
	#ifdef RS_HTTPS_SUPPORT
	rs_s32 ret;
	rs_s32 tls_has_error = 0;
	#endif
//记录创建时的socket id, 进行判断处理,因为其他的消息也会用到
	app_soc_notify_ind_struct *soc_notify = (app_soc_notify_ind_struct*) msg_ptr;
	
	RS_PORITNG_LOG("%s start, soc_notify->socket_id[%d]\n\r", __func__, (rs_s8)(soc_notify->socket_id));

	if (g_rs_socket_handle != (rs_s8)(soc_notify->socket_id))
	{
		return;
	}

	switch (soc_notify->event_type) 
	{
		case SOC_WRITE:
			RS_PORITNG_LOG("%s SOC_WRITE, soc_notify->result=%d\n\r", __func__, soc_notify->result);
			
			//start read 
			if(soc_notify->result != 0)
			{
				RS_PORITNG_LOG("%s, SOC_WRITE success\n\r", __func__);
	
				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
				}
			}
			else
			{
				soc_close(soc_notify->socket_id);
				RS_PORITNG_LOG("%s, SOC_WRITE error soc_close socketid=%d\n\r", __func__, soc_notify->socket_id);
	
				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
				}
			}
			
			RS_PORITNG_LOG("%s SOC_WRITE end\n\r", __func__);
		
			break;
		
		case SOC_READ:
			RS_PORITNG_LOG("%s SOC_READ, soc_notify->result=%d\n\r", __func__, soc_notify->result);

			if(soc_notify->result != 0)
			{
				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
				}
			}
			else
			{
				soc_close(soc_notify->socket_id);
				RS_PORITNG_LOG("%s, SOC_READ error and soc_close\n\r", __func__);

				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
				}
			}
			
			RS_PORITNG_LOG("%s SOC_READ end\n\r", __func__);
		
			break;
		case SOC_CONNECT:
			/* send REQ data */
			RS_PORITNG_LOG("%s SOC_CONNECT, soc_notify->result=%d\n\r", __func__, soc_notify->result);

			if(soc_notify->result != 0) {
				 #ifdef RS_HTTPS_SUPPORT
							ret = tls_new_conn(g_ctx_id, soc_notify->socket_id, &rs_soc_addr);
						    if(ret<0)
							{
								error_handling("tls_new_conn",ret);
								goto ERROR1;
							}
							RS_PORITNG_LOG("[TLS]tls_new_conn is OK\n\r");
							//soc_send(s, …);		// start to send data, or
							ret = tls_check_invalid_cert(soc_notify->socket_id,KAL_FALSE);
							if(ret<0)
							{
								error_handling("tls_check_invalid_cert",ret);
								goto ERROR2;
							}
							ret=tls_handshake(soc_notify->socket_id);		// perform handshake
							if(ret<-1)
							{
								error_handling("tls_handshake",ret);
								goto ERROR2;
							 	
								
							}
							goto NO_ERROR;
					ERROR2://tls_check_invalid_cert error
						tls_delete_conn((kal_int8)soc_notify->socket_id);				   		
					ERROR1://tls_new_conn error 
						soc_close(soc_notify->socket_id);
						tls_delete_ctx((kal_uint8)g_ctx_id);
						RS_PORITNG_LOG("%s SOC_CONNECT error and soc_close on tls \n\r", __func__);	
						tls_has_error = 1;
					NO_ERROR:
						if(tls_has_error == 0){
							//tls建立成功
							//不执行g_socket_callback
							//等待tls_handshake完成回调函数执行时发送消息给引擎
							RS_PORITNG_LOG("%s waite for  tls_handshake done \n\r", __func__);	
						}else{
							//tls建立出错了，立即发送失败消息给引擎
							if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
							{
								g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
							}
						}
				#else	
				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
				}
				#endif
			} else {
				soc_close(soc_notify->socket_id);
				RS_PORITNG_LOG("%s SOC_CONNECT error and soc_close\n\r", __func__);

				if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
				{
					g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
				}
			}
			
			RS_PORITNG_LOG("%s SOC_CONNECT end\n\r", __func__);
			
			break;
		/*
		case SOC_CLOSE:
			soc_close(soc_notify->socket_id);
			RS_PORITNG_LOG("%s, SOC_CLOSE read done soc_close socketid=%d == Really not closed\n\r", __func__, soc_notify->socket_id);
			if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
			{
				g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
			}
			break;
		*/
	}
}


/*
**	Get IP Address. 
*/
static int rs_socket_extract_ip_addr(char* szDstBuf, const char* szSrcURL, unsigned long *nHostPort )
{
	int ret = 0;
	char portBuf[10];
	char* portPtr;

	// check "http" 
	if ( rs_memcmp(szSrcURL, "http://", 7) == 0 )
	{
		szSrcURL += 7;
		*nHostPort = 80;//http 缺省端口号是80
	}
	else if ( rs_memcmp(szSrcURL, "https://", 8) == 0 )
	{
		szSrcURL += 8;
		ret = 1;
		*nHostPort = 443;//https 缺省端口号是443
	}

	// copy data 
	while ( *szSrcURL != 0 && *szSrcURL != ':' && *szSrcURL != '/' )
	{
		*szDstBuf++ = *szSrcURL++;
	}
	*szDstBuf = 0;

	if (*szSrcURL == ':')
	{
		long port = 0;
		portPtr = &portBuf[0];
		szSrcURL++;
		*portPtr++ = *szSrcURL++;
		while ( *szSrcURL != 0 && *szSrcURL != '/' )
		{
			*portPtr++ = *szSrcURL++;
		}
		*portPtr = 0;
		port = rs_atoi(portBuf);
		*nHostPort = port;
	}
	else
	{
		//*nHostPort = 80;
	}
	
	return ret;
}

rs_s32 rs_socket_init(RS_SOCKET_MSG_HANDLER_CALLBACK callback, void* handle)
{
	g_socket_callback = callback;
	g_rs_handle = handle;

	
	RS_PORITNG_LOG("before fota_data_account[%d]\n\r", fota_data_account);
	
	if(fota_data_account == 0) {
		 // 1.  Get data account

	        cbm_app_info_struct info = {0};
	        kal_int8 result = 0;


	        if(0 == fota_app_id)
	        {
	            memset(&info, 0, sizeof(info));            
	            info.app_str_id = 0;
	            info.app_icon_id = 0;
	            info.app_type = DTCNT_APPTYPE_MRE_NET;//DTCNT_APPTYPE_NTP;//DTCNT_APPTYPE_MRE_NET;//DTCNT_APPTYPE_BRW_HTTP;//DTCNT_APPTYPE_PLAYER;
			 
	            result = cbm_register_app_id_with_app_info(&info, &fota_app_id);
	            RS_PORITNG_LOG("cbm_register_app_id_with_app_info, result = [%d] fota_app_id = %d \n\r", result ,fota_app_id);
		     
	            if(fota_app_id == 0)
	            {
	                RS_PORITNG_LOG( "[%s]app_id:%d, RETURN!", __func__, fota_app_id);
	                return -1;
	            }
	        }
	        
	        fota_data_account = cbm_encode_data_account_id(CBM_DEFAULT_ACCT_ID, 0/*sim 0*/, fota_app_id, 0);//CBM_DEFAULT_ACCT_ID 0x34 


		// 2. Register event handler
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)fota_get_ip_handle, MMI_TRUE);//共享模式
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)fota_event_handle, MMI_TRUE);//共享模式

		#ifdef RS_HTTPS_SUPPORT
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_TLS_NOTIFY_IND, (PsIntFuncPtr)fota_tls_ind_handler, MMI_TRUE);
	    //mmi_frm_set_protocol_event_handler(MSG_ID_APP_TLS_INVALID_CERT_IND, (PsIntFuncPtr)app_tls_invalid_cert_ind_hdlr, MMI_TRUE);
  		//mmi_frm_set_protocol_event_handler(MSG_ID_APP_TLS_CLIENT_AUTH_IND, (PsIntFuncPtr)app_tls_client_auth_ind_hdlr, MMI_TRUE);

		#endif
	}

	return 0;
}

rs_s32 rs_socket_gethostbyname(const rs_s8 *uri)
{
	rs_s8 szHostAddr[64];
	sockaddr_struct soc_addr;
	kal_int8 len_addr=0;
	char ret_soc = 0;

	rs_socket_extract_ip_addr(szHostAddr, uri, &nHostPort);
	RS_PORITNG_LOG("%s url:%s\n\r", __func__, uri);
	RS_PORITNG_LOG("%s szHostAddr:%s, port:%d\n\r", __func__, szHostAddr, nHostPort);

	ret_soc = soc_gethostbyname(0, MOD_MMI, fota_app_id, szHostAddr, soc_addr.addr, &len_addr, 0, fota_data_account);//ERROR
	RS_PORITNG_LOG("%s, ret_soc[%d]\n\r", __func__, ret_soc);
	if((ret_soc-256 == SOC_WOULDBLOCK) ||(ret_soc == SOC_WOULDBLOCK)) {// block
		RS_PORITNG_LOG("%s, wait and then fota_get_ip_handle\n\r", __func__);

		#ifdef RS_HTTPS_SUPPORT
			create_tls_context();
		#endif
		
		return RS_ERR_OK;
	} else {
		RS_PORITNG_LOG("%s, error\n\r", __func__);
		
		return RS_ERR_FAILED;
	}
}

rs_s32 rs_socket_getMode()
{
	return 1;
}

rs_socket_handle rs_socket_create()
{
	rs_s8 ret_soc = 0;
	rs_s8 val = 0;
	kal_int8  hSocket;

	// 3. create Socket
	//SOC_SOCK_STREAM = 0  SOC_PF_INET = 0
 	hSocket = soc_create(SOC_PF_INET, SOC_SOCK_STREAM, 0, MOD_MMI, fota_data_account);
 	RS_PORITNG_LOG("%s, fota_socket_id[%d]\n\r", __func__, hSocket);

	// 4. Set socket options    
 	val = 1; //SOC_NBIO==4
 	ret_soc = soc_setsockopt(hSocket , SOC_NBIO, &val, sizeof(val));//0x01<<2
	//val = SOC_READ|SOC_WRITE|SOC_CONNECT|SOC_CLOSE;
	val = SOC_READ|SOC_WRITE|SOC_CONNECT;
	
	ret_soc = soc_setsockopt(hSocket, SOC_ASYNC, &val, sizeof(val));
	RS_PORITNG_LOG("soc_setsockopt, ret_soc[%d]\n\r", ret_soc);

	g_rs_socket_handle = hSocket;
	
	return (rs_socket_handle)hSocket;
}

rs_s32 rs_socket_connect(rs_socket_handle  socketHandle)
{
	rs_s8 ret_soc = soc_connect((kal_int8)socketHandle, &rs_soc_addr);
	
	switch (ret_soc) {
		case SOC_SUCCESS:
			RS_PORITNG_LOG("%s soc_connect return SOC_SUCCESS\n\r", __func__);
			return 1;
		
		case SOC_WOULDBLOCK:
		case SOC_WOULDBLOCK+256:
			RS_PORITNG_LOG("%s soc_connect return wait and then fota_event_handle\n\r", __func__);
			return 0;
			
		default:
			RS_PORITNG_LOG("connect fail\n");
			return -1;
	}
}

rs_s32 rs_socket_send(rs_socket_handle sockHndl, const rs_s8 *sendData, rs_s32 size)
{
	rs_s32 ret, datalen;

	RS_PORITNG_LOG("%s size[%d]\n\r", __func__, size);
	#ifdef RS_HTTPS_SUPPORT
	ret = tls_write(sockHndl, (const void*)sendData, size);		// start to send data, or
	if(ret > 0) {
		RS_PORITNG_LOG("%s tls_write,send len=%d\n\r", __func__, ret);
		//rs_srv_watch_trace("liujw socket tls_write,len=%d, content=%s\n", ret, sendData);
		RS_PORITNG_LOG("liujw socket tls_write,len=%d, content=%s\n", ret, sendData);
		
		return ret;
	}
	else if (ret == TLS_ERR_WOULDBLOCK)
	{
			RS_PORITNG_LOG("%s, wait and then fota_tls_ind_handler\n\r", __func__);
			return 0;
	}
	//else if (ret == SOC_WOULDBLOCK+256)
	//{
	//		RS_PORITNG_LOG("%s, wait and then fota_tls_ind_handler\n\r", __func__);
	//		return 0;
	//}
	else
	{
			RS_PORITNG_LOG("%s tls_write error[%x]\n\r", __func__, ret);
			return -1;
	}
	#else
	ret = soc_send(sockHndl, sendData, size, 0);
	if(ret > 0) {
		RS_PORITNG_LOG("%s soc_send,send len=%d\n\r", __func__, ret);
		RS_PORITNG_LOG("liujw socket send data,len=%d, content=%s\n", ret, sendData);
		
		return ret;
	}
	else if (ret == SOC_WOULDBLOCK)
	{
			RS_PORITNG_LOG("%s, wait and then fota_event_handle\n\r", __func__);
			return 0;
	}
	else if (ret == SOC_WOULDBLOCK+256)
	{
			RS_PORITNG_LOG("%s, wait and then fota_event_handle\n\r", __func__);
			return 0;
	}
	else
	{
			RS_PORITNG_LOG("%s send error[%d]\n\r", __func__, ret);
			return -1;
	}

	#endif
}

rs_s32 rs_socket_recv(rs_socket_handle sockHndl, rs_s8 *buffer, rs_s32 bufLen)
{
	rs_s32 ret = 0;
	rs_s32 read_len = 0;
	MMI_BOOL is_ready = MMI_FALSE;

	#ifdef RS_HTTPS_SUPPORT
	ret = tls_read(sockHndl, buffer, bufLen);
		if (ret > 0)
		{
			RS_PORITNG_LOG("%s read success, read len=%d\n\r", __func__, ret);
			//rs_srv_watch_trace("liujw socket recv data, realy read, read len=%d, content=%s\n", ret, buffer);
			RS_PORITNG_LOG("liujw socket tls_read, realy read, read len=%d, content=%s\n", ret, buffer);
			return ret;
		}
		else if (ret == TLS_ERR_WOULDBLOCK )
		{
			RS_PORITNG_LOG("%s, wait and then fota_tls_ind_handler\n\r", __func__);
			return 0;
		}
		else
		{
			RS_PORITNG_LOG("%s, tls_read error[%x]\n\r", __func__, ret);
			return -1;
		}

	#else

	is_ready = soc_ready_for_read(sockHndl);
	RS_PORITNG_LOG("%s, is_ready[%d]\n\r", __func__, is_ready);

	if(is_ready)
	{
		ret = soc_recv(sockHndl, buffer, bufLen, 0);
		if (ret > 0)
		{
			RS_PORITNG_LOG("%s read success, read len=%d\n\r", __func__, ret);
			RS_PORITNG_LOG("liujw socket recv data, realy read, read len=%d, content=%s\n", ret, buffer);
			return ret;
		}
		else if (ret == SOC_WOULDBLOCK || ret == (SOC_WOULDBLOCK + 256))
		{
			RS_PORITNG_LOG("%s, wait and then fota_event_handle\n\r", __func__);
			return 0;
		}
		else
		{
			RS_PORITNG_LOG("%s, unkown error[%x]\n\r", __func__, ret);
			return -1;
		}
	}
	else
	{
		RS_PORITNG_LOG("%s read not ready\n\r", __func__);
		return 0;
	}

	#endif


}

rs_s32 rs_socket_select(rs_socket_handle sockHndl, rs_s32 nTimeOut)
{
	// not support
	return -1;
}

rs_s32 rs_socket_close(rs_socket_handle sockHndl)
{
	RS_PORITNG_LOG("%s socket close, socket id = %d\n\r", __func__, sockHndl);

	#ifdef RS_HTTPS_SUPPORT
		tls_delete_conn((kal_int8)sockHndl);
		soc_close(sockHndl);
		delete_tls_context(g_ctx_id); 
	#else
	soc_close(sockHndl);
	#endif
	g_rs_socket_handle = -1;
	
	return RS_ERR_OK;
}

rs_s32 rs_socket_only_wait_callback()
{
	return 0;
}

void rs_socket_wait_moment()
{
	// 根据实际情况sleep一会
}