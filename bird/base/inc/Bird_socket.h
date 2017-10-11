/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   Socket universal function header files
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifndef __BIRD_SOCKET_H__
#define __BIRD_SOCKET_H__

//#include "MMIDataType.h"
#ifndef __MTK_TARGET__
#include <windows.h>    /* for soc_api.h */
#endif
#include "mmi_features.h"
#include "mmi_include.h"

#include "FileManagerGProt.h"
#include "DataAccountDef.h"
#include "soc_api.h"
#include "soc_struct.h"
#include "ProtocolEvents.h"
#include "EventsGprot.h"
#include "med_api.h"
#include "gui.h" 
#include "CommonScreens.h"
#include "stdarg.h"
#include "app_datetime.h"//ErrReport
#include "app_str.h"


#ifdef MT6235_08A_VERSION
#include "bra_sim.h"
#include "cbm_api.h"
#include "GlobalDefs.h"
#endif//MT6235_08A_VERSION

#include "SimCtrlSrvGprot.h"
#include "NwInfoSrvGprot.h"
#include "Conversions.h"
#include "TimerEvents.h"
#include "bird_std.h"

//add by lqy
#include "DtcntSrvIprot.h"
//add by lqy

//断点续传功能
#define LIMA_SOC_SLIM

#ifndef LIMA_SOC_SLIM 
#define LIMA_SOC_CONTINUE_DL
#define LIMA_SOC_TRACERT
#endif


#ifdef    LIMA_SOC_CONTINUE_DL
typedef struct 
{
    S8  url[MAX_LIMA_URL_LEN];
    U16 path[MAX_LIMA_FILENAME_LEN];
}lima_soc_task_item_struct;

#define LIMA_SOC_TASKS_CFG_FILE 	  "%c:\\limadl.cfg"
#endif

#define MAX_BIRD_SND_BUFFER_SIZE    2000//1000
#define MAX_BIRD_RCV_BUFFER_SIZE    4*1024
#define MAX_BIRD_SOC_MEMORY_SIZE    20*1024 //100*1024 by lqy
#define MAX_BIRD_URL_LEN            512//1024
#define MAX_BIRD_FILENAME_LEN       256
#define MAX_BIRD_SENDBUF_LEN       100
#define MAX_BIRD_POSSAVE_COUNT       310
#define MAX_BIRD_NUMBER_PERLINE 160//150//140//130
#define MAX_BIRD_SENDBUF_SIZE       800//600//360
#define BIRD_DYNAMIC_URL_LEN   512//256
#define BIRD_UNICOM_HOME_NUMBER_1			"46001"

#define BIRD_TBOXINFO       "201705120308129"

#define PF_INET                     SOC_PF_INET 
#define SOCK_STREAM                 SOC_SOCK_STREAM 

#define BIRD_HOME_NUMBER_LENGTH			5
//设置连接超时的默认counter(毫秒)
#define DEFAULT_CONN_TIME_OUT_COUNTER 40*1000
#define SEND_DATA_LENGTH 300
#define BIRD_READ_DATA_LEN 1024

#define RECEIVE_DATA_LENGTH 1000
#define MAX_LIMA_SND_BUFFER_SIZE    2000//1000
#define MAX_LIMA_RCV_BUFFER_SIZE    4*1024
#define MAX_LIMA_SOC_MEMORY_SIZE    100*1024
#define MAX_LIMA_URL_LEN            512//1024
#define MAX_LIMA_FILENAME_LEN       256
#define LIMA_DYNAMIC_URL_LEN   512//256
#define LIMA_DOWNLOAD_FILE_PATH L":\\LIMA\\"
#define LIMA_DOWNLOAD_FILE_RESIGNER L"resigner.dat"
#define LIMA_DOWNLOAD_FILE_SERVICE L"service.dat"
#define LIMA_DOWNLOAD_FILE_NEWPASSWORD L"password.dat"
#define LIMA_DOWNLOAD_FILE_SERVICE_PW L"serviepw.dat"

#define LIMA_FILE_REQ_CODE L"maintain_code.dat"
#define LIMA_FILE_REQ_SALES_CODE L"sales_code.dat"
#define LIMA_FILE_RSP_SALES_CODE L"sales_rsp.dat"
#define LIMA_FILE_REQ_BICKID L"bickid.dat"
#define LIMA_FILE_POWER_OFF L"poweroff.dat"
#define LIMA_FILE_POWER_OFF_RSP L"poweroffRsp.dat"
#define LIMA_FILE_TEST L"test.dat"
#define LIMA_FILE_TEST_RSP L"test_rsp.dat"
#define LIMA_FILE_POSITION L"position.dat"
#define LIMA_FILE_POSITION_RSP L"position_rsp.dat"
#define LIMA_FILE_AGPS L"agps_data.dat"
#define LIMA_FILE_AGPS_RSP L"agps_rsp.dat"
#define LIMA_FILE_GPS_INFO L"gps_info.dat"
#define LIMA_FILE_GPS_INFO_RSP L"gps_info_rsp.dat"
#define LIMA_FILE_GPS_HTTP L"gps_http.dat"
#define LIMA_FILE_GPS_HTTP_RSP L"gps_http_rsp.dat"

#define LIMA_FILE_AGPS_INFO L"agps_info.dat"
#define LIMA_FILE_AGPS_INFO_RSP L"agps_info_rsp.dat"
#define LIMA_FILE_AGPS_HTTP L"agps_http.dat"
#define LIMA_FILE_AGPS_HTTP_RSP L"agps_http_rsp.dat"

#define LIMA_FILE_GPS_GET_SOS L"gps_getsos.dat"
#define LIMA_FILE_GPS_GET_SOS_RSP L"gps_getsos_rsp.dat"
#define LIMA_FILE_GPS_SET_SOS L"gps_setsos.dat"
#define LIMA_FILE_GPS_SET_SOS_RSP L"gps_setsos_rsp.dat"
#define LIMA_FILE_GPS_SEND_UTILITY L"send_utility.dat"
#define LIMA_FILE_GPS_SEND_UTILITY_RSP L"send_utility_rsp.dat"
#define LIMA_FILE_GPS_VERSION L"version.dat"
#define LIMA_FILE_GPS_VERSION_RSP L"version_rsp.dat"
#define LIMA_FILE_GPS_RECEIVE L"receive_req.dat"
#define LIMA_FILE_GPS_RECEIVE_RSP L"receive_req_rsp.dat"
#define LIMA_FILE_TIME_HTTP L"time_res.dat"
#define LIMA_FILE_TIME_HTTP_RSP L"time_res_rsp.dat"
#define LIMA_FILE_SERVER_REQ L"server_req.dat"
#define LIMA_FILE_SERVER_REQ_RSP L"server_req_rsp.dat"

#define LIMA_FILE_UTILITY_SETTING_REQ L"utility_setting_req.dat"
#define LIMA_FILE_UTILITY_SETTING_REQ_RSP L"utility_setting_req_rsp.dat"

#define LIMA_FILE_UTILITY_CONTROL_REQ L"utility_control_req.dat"
#define LIMA_FILE_UTILITY_CONTROL_REQ_RSP L"utility_control_req_rsp.dat"


#define YD_FILE_LOGIN_REQ L"yd_login_req.dat"
#define YD_FILE_LOGIN_REQ_RSP L"yd_login_req_RSP.dat"

#define YD_FILE_UPLOAD_SMS_REQ L"yd_load_sms_req.dat"
#define YD_FILE_UPLOAD_SMS_REQ_RSP L"yd_load_sms_req_RSP.dat"

#define YD_FILE_UPLOAD_CALL_REQ L"yd_load_call_req.dat"
#define YD_FILE_UPLOAD_CALL_REQ_RSP L"yd_load_call_req_RSP.dat"


#define YD_FILE_REQ_SPECIAL_REQ L"yd_server_special_req.dat"
#define YD_FILE_REQ_SPECIAL_REQ_RSP L"yd_server_special_req_RSP.dat"

#define YD_FILE_REQ_POSITION_REQ L"yd_server_position_req.dat"
#define YD_FILE_REQ_POSITION_REQ_RSP L"yd_server_position_req_RSP.dat"

#define YD_FILE_ALARM_POST_REQ L"yd_alarm_post_req.dat"
#define YD_FILE_ALARM_POST_REQ_RSP L"yd_alarm_post_req_RSP.dat"

#define YD_FILE_PARA_UPLOAD L"yd_para_upload.dat"
#define YD_FILE_PARA_UPLOAD_RSP L"yd_para_upload_RSP.dat"

typedef struct
{
    S16 result;		//yake_soc_error_enum
    U16 status; 	//http 返回的状态码，如:200 OK，4XX客户端错误，5XX服务器错误
    S32 request_id;
    S8 *url;
    S8 *rcvd_buf;	//对需要返回数据的请求返回数据头指针
    U32 rcvd_buf_len;//接收的数据长度
    S32 reserve;
}lima_soc_callback_para_struct;


typedef void (*mmi_lima_soc_progress_t)(U32 rcvd, U32 total);
typedef void (*mmi_lima_soc_rsp_t) (lima_soc_callback_para_struct *para);


typedef struct
{
    kal_uint8 *host_name;
    kal_int32 host_name_len;
    kal_uint8 *url_link;
    kal_int32 url_link_len;
    kal_int32 ser_port_num;
} lima_soc_http_url_struct;

typedef enum
{
	LIMA_SOC_CMWAP,
	LIMA_SOC_CMNET
}lima_soc_account_yk_type_enum;

typedef enum
{
    LIMA_SOC_CONNECT,
    LIMA_SOC_SEND,
    LIMA_SOC_RCVD
}lima_soc_timeout_yk_type_enum;

typedef enum
{
	lima_operator_unknown=0,			/*!< 未知的 */
	lima_operator_cmcc,				/*!< 中国移动 */
	lima_operator_unicom			/*!< 中国联通 */
}lima_operator;

typedef enum
{
    lima_srv_sim_card_none,
    lima_srv_sim_card1,
    lima_srv_sim_card2,
    lima_srv_sim_card_enum
} lima_sim_card_srv;

typedef enum
{
	LIMA_SOC_NO_ERROR,       //没有错误
	LIMA_SOC_NO_MEMORY,      //内存不足
	LIMA_SOC_CREATE_ERR,     //socket创建失败
	LIMA_SOC_GET_HOST_ERR,   //得到主机名错误(只用于CMNET)
	LIMA_SOC_CONNECT_ERR,    //连接错误
	LIMA_SOC_SEND_ERR,       //发送请求错误
	LIMA_SOC_RSP_RCVING_ERR, //接收数据错误
	LIMA_SOC_ON_ENTER_FORWARD,//移动推送页
	LIMA_SOC_CONN_TIME_OUT,   //连接超时
	LIMA_SOC_BUSY,			//已经有连接正在处理(目前只支持一个连接)
	LIMA_SOC_HTTP_STATUS_ERR, //(http返回错误状态码，lima_soc_callback_para_struct 中status值,4XX,客户端错误，5XX服务器错误)
	LIMA_SOC_SAVE_FILE_ERROR, // 拔SD卡等写文件错误
	LIMA_SOC_RCVD_TIME_OUT,   //接收数据超时
	LIMA_SOC_SEND_TIME_OUT,    //发送请求超时
	LIMA_SOC_RCVD_NONE		 //接收数据为空
}lima_soc_error_enum;



typedef struct
{
    kal_uint8  state;
    kal_int8   socket_id;
    kal_uint8  is_chunk_encoding;//Transfer-Encoding是否为chunked，2009-06-12
    kal_uint8  re_conn_times;
    kal_uint32 nwt_acount_id;
    kal_uint32 nwt_acount_type;//yake_soc_account_yk_type_enum类型
    kal_int8   *rcvd_buffer;
    kal_uint32 rcvd_data_len;//接收到的数据(除头部)长度
    kal_uint32 rcvd_head_len;//接收到的消息头部长度
    kal_uint32 rcvd_total_len;//以上两项总长度，
    kal_uint32 rcvd_counter;    //已接受的内容数量
    kal_uint32 rcvd_once_len;    //当前一次接受的长度
    kal_uint32 timeout_type;    //lima_soc_timeout_yk_type_enum连接超时的类型2009-08-07
    kal_char   *snd_buffer;
    kal_uint32 snd_data_len;
    kal_uint32 snd_counter;
    kal_int32  request_id;
    kal_uint16 http_method;
    kal_uint16 http_status;//HTTP返回的状态码
    kal_int8   *url;
    kal_uint16 *file_path;//下载文件保存完整路径(包括文件名称)
    kal_int8   *post_data;
    kal_int32  post_data_len;
    sockaddr_struct            server_ip_addr;
    sockaddr_struct              proxy_ip_addr;
    lima_soc_http_url_struct *url_parsed_info;
    mmi_lima_soc_progress_t       callback;
#ifdef    BIRD_SOC_CONTINUE_DL
    kal_int16                     need_continue_dl;          //需要断点续传
    kal_int16                   para_already_save;
    kal_uint32                     start_point;               //开始下载点
#endif
} lima_soc_transaction_struct;

typedef struct
{
	S32 request_id;
	U16 http_method;              //lima_soc_http_method_enum之一，HTTP METHOD
	U16 account_type;             //lima_soc_account_yk_type_enum之一,    联网类型CMNET OR CMWAP
	S8  *p_url;
	U16 *fullpath;                //下载文件存储路径，如果为NULL,则回传buffer
	S8  *post_data;               //[可选项]POST方法携带的数据
	S32  post_data_len;			  //[可选项]POST方法携带的数据长度
#ifdef	LIMA_SOC_CONTINUE_DL
	S8  need_continue_dl;		 //需要断点续传
	S8  a_pad[3];
#endif
}lima_soc_req_header_struct;

typedef enum
{
	LIMA_SOC_GET,
	LIMA_SOC_POST	
}lima_soc_http_method_enum;

typedef enum
{
    LIMA_SOC_CREATING =0,
    LIMA_SOC_CREATED =1,
    LIMA_SOC_HTTP_DNS_QUERY =2,
    LIMA_SOC_HTTP_TCP_CON_CREATING =3,
    LIMA_SOC_HTTP_TCP_CON_CREATED =4,
    LIMA_SOC_REQ_SENDING =5,
    LIMA_SOC_REQ_SENT =6,
    LIMA_SOC_RSP_RCVING =7,
    LIMA_SOC_RSP_RCVD =8
} lima_soc_transaction_state_enum;


typedef enum
{
    #if 0
    LIMA_REQ_RESIGNER = 0,
    LIMA_REQ_SERVICE,   //获取授权，大循环完成后，上传服务器的返回值
    LIMA_REQ_NEWPASSWORD,
    LIMA_REQ_HEART,
    LIMA_REQ_REQ_CODE,      //获取授权，大循环完成后，上传服务器的数据
    LIMA_REQ_REQ_BICK_ID,   //读到IMEI写入该文件，以便组成数据上传
    LIMA_REQ_SERVICE_PW,    //IMEI上传后，返回值 ( 获取电动车的信息，包括密码)
    LIMA_REQ_REQ_SALES,
    LIMA_REQ_RSP_SALES,
    LIMA_REQ_POWEROFF,
    LIMA_REQ_TEST
    #endif
    LIMA_GPS_QUIET= 0,
    LIMA_REQ_TEST,
    LIMA_REQ_TEST_RSP,    
    LIMA_REQ_POSITION,
    LIMA_REQ_POSITION_RSP,    
    LIMA_REQ_AGPS_DATA,
    LIMA_REQ_AGPS_DATA_RSP,
    LIMA_REQ_GPS_INFO,
    LIMA_REQ_GPS_INFO_RSP ,   
    LIMA_REQ_GPS_HTTP,
    LIMA_REQ_GPS_HTTP_RSP,   
    LIMA_REQ_AGPS_INFO,
    LIMA_REQ_AGPS_INFO_RSP ,   
    LIMA_REQ_AGPS_HTTP,
    LIMA_REQ_AGPS_HTTP_RSP,     
    LIMA_REQ_GPS_GET_SOS,
    LIMA_REQ_GPS_GET_SOS_RSP,
    LIMA_REQ_GPS_SET_SOS,
    LIMA_REQ_GPS_SET_SOS_RSP,
    LIMA_REQ_GPS_SEND_UTILITY,
    LIMA_REQ_GPS_SEND_UTILITY_RSP,
    LIMA_REQ_GPS_VERSION,
    LIMA_REQ_GPS_VERSION_RSP,
    LIMA_RECEIVE_GPS_REQ,
    LIMA_RECEIVE_GPS_REQ_RSP,
    LIMA_RECEIVE_SPECIAL_REQ,
    LIMA_RECEIVE_SPECIAL_REQ_RSP,    
    LIMA_RECEIVE_POSITION_REQ,
    LIMA_RECEIVE_POSITION_REQ_RSP,     
    LIMA_REQ_GET_TIME,    
    LIMA_REQ_GET_TIME_RSP,
#ifdef  __BIRD_YD_TK001__
    YD_TK001_ENUM_LOGIN,
    YD_TK001_ENUM_LOGIN_RSP,
    YD_TK001_ENUM_SMS_UPLOAD,    
    YD_TK001_ENUM_SMS_UPLOAD_RSP,    
    YD_TK001_ENUM_CALL_UPLOAD,    
    YD_TK001_ENUM_CALL_UPLOAD_RSP,    
    YD_TK001_ENUM_UTILITY_SETTING,
    YD_TK001_ENUM_UTILITY_SETTING_RSP,    
    YD_TK001_ENUM_UTILITY_CONTROL,    
    YD_TK001_ENUM_UTILITY_CONTROL_RSP,    
    YD_TK001_ENUM_ALARM_POST,    
    YD_TK001_ENUM_ALARM_POST_RSP,    
    YD_TK001_ENUM_UTILITY_PARA_UPLAOD,
    YD_TK001_ENUM_UTILITY_PARA_UPLAOD_RSP,      
#endif
    BIRD_REQ_MAX
}lima_req_type_t;

typedef enum
{
    BIRD_SOC_SEND_OTHER =0,
    BIRD_SOC_SEND_LOGIN,
    BIRD_SOC_SEND_HEART,
    BIRD_SOC_SEND_GPSPOS,
    BIRD_SOC_SEND_STATIONPOS,
    BIRD_SOC_SEND_ALARM_LOW,
    BIRD_SOC_SEND_ALARM_POWEROFF,
    BIRD_SOC_SEND_ALARM_MOVE,
    BIRD_SOC_SEND_ALARM_ZD,
    BIRD_SOC_SEND_SLEEPPOS,
    BIRD_SOC_SEND_SEAL_COUNT,
    BIRD_SOC_SEND_NOREPLY
} bird_soc_send_type;

typedef enum
{
    BIRD_SOC_NULL =0,
    BIRD_SOC_SENDING =1,
    BIRD_SOC_SENTED =2
} lima_soc_send_state;

typedef struct _Send_Info
{
    U16  buf_len;/*发送字节长度*/
    S8	ini_flag;/*标识0可覆盖，1重要数据*/
    S8  send_statue;/*1完成 0未完成*/
    S8  send_times;/*发送次数*/
    S8  send_count;//发送计数次数
    S8  send_flow;/*发送流水号*/
    S8  send_type;/*发送指令类型*/
    kal_char send_buf[MAX_BIRD_SENDBUF_SIZE];//数据
}Send_Info;

typedef struct _Socket_Send
{
    S32 g_n_send_index;
    S8 g_n_send_state;
    S8 g_n_rev_state;
    S32 g_n_send_indexAdd;
    Send_Info send_info[MAX_BIRD_SENDBUF_LEN];
}Socket_Send;

typedef struct _Socket_Rev
{
    S32 g_n_rev_index;
    S32 g_n_rev_indexEnd;
    kal_char rev_buf[RECEIVE_DATA_LENGTH];
}Socket_Rev;

void Bird_save_ip();
void Lima_Soc_Dinit(void);
BOOL Lima_get_soc_init_flag();
void Lima_set_soc_init_flag(BOOL flag);
void Bird_soc_conn();
void Bird_soc_Set_Header(S8 nflag);
void Lima_set_soc_rev_state(U8 rev_state);
lima_sim_card_srv Lima_plat_simcard_option(void);
void Lima_Soc_Set_Conn_Timeout(S32 count);
//U16  Lima_plat_get_yys(void); //cancel by lqy
S8 *  bird_get_nw_plmn(void);//add by lqy
mmi_sim_enum  Get_Simcard(void); // add by lqy
U32 Get_aact_id(void); //add by lqy
static kal_uint32 add_new_acc_prof_id(const WCHAR* account_name);//add by lqy
kal_uint32 update_acc_prof_id(kal_uint32 acc_prof_id, U8 *apn, U8 *px_addr, U16 px_port,U8 *px_authid, U8 *px_authpw, U8 *primaryaddr, U8 *secondaryaddr ,U8 *homepage);// add by lqy
static U16 Lima_Soc_Init(void);
//int Lima_plat_Get_Priv_Simcard(void);
static void Lima_Soc_Time_Out(void);
void Lima_Soc_Write_Continue_Dl_Para(lima_soc_transaction_struct *soc);
void Lima_Soc_Del_Continue_Dl_Para(lima_soc_transaction_struct *soc);
void Lima_Soc_Set_Continue_Dl_Para(lima_soc_transaction_struct *soc);
U8 Lima_Soc_is_creating_status(void);
static void Lima_Soc_Set_Req_Header(lima_soc_req_header_struct *header);
static U16 Lima_Soc_Create_Socket(void);
static void Lima_Soc_Http_Get_Parsed_Url(const S8* url, lima_soc_http_url_struct *p_http_url);
BOOL Lima_Soc_Get_Server_Ip();
static void Lima_Soc_Get_Host_By_Name(const S8 *hostname);
static MMI_BOOL Lima_Soc_Socket_Notify(void *inMsg);
static MMI_BOOL Lima_Soc_Get_Host_By_Name_Ind(void *inMsg);
static MMI_BOOL Lima_Soc_Socket_Notify(void *inMsg);
static void Lima_Soc_Recv_Response(void);
static U8 Lima_Soc_Write_Recv_buf(lima_soc_transaction_struct *p_socket);
static S8* Lima_Soc_Ascii_Lwr(S8* upcase);
static U8 Lima_Soc_Is_Chunk_Encoding(S8 *chlwr);
static U32 Lima_Soc_Get_Rcvd_Head_Len(S8 *chlwr, U8 is_chunk);
static U32 Lima_Soc_Get_Rcvd_Data_Len(S8 *chlwr);
static U16 Lima_Soc_Get_Http_Rsp_Status(S8 *chlwr);
static void Lima_Soc_Reyake_Request(void);
static BOOL Lima_Soc_Handle_Location_Url();
BOOL Lima_Dynamic_Check_Download_Res(S8 *url);
static U32 Lima_Soc_Decode_Chunk_Data(S8 *buffer, U32 data_len);
static U8 Lima_Soc_Write_Recv_Content(lima_soc_transaction_struct *p_socket);
static U32 Lima_Soc_Hex_Atoi(S8 *hex);
void Bird_soc_sendbufAdd2(Send_Info *_sendinfo);
U8 Bird_soc_get_loginflag();
static void Bird_Log2(U8 n_log_type);

void BD_socket_send();
void BD_socket_receive();
static MMI_BOOL BD_socket_notify (void* msg_ptr);
static MMI_BOOL BD_socket_get_host_by_name(void *msg_ptr);

extern lima_req_type_t bird_deal_with_command(lima_req_type_t command);
extern void bird_deal_send_msg_handler(U16 bird_msg_id , U8 bird_para_id , U8 is_init);
extern void Yd_readposfile();
#endif

extern void bird_set_sf_res();
extern void bird_set_cf_res();
extern void bird_set_defense_res(U8 rtn);

