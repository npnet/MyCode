/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   BIRD GPS station data processing function header files
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/


#include "mdi_gps.h"
#include "math.h"
//#include "supl2mmi_struct.h"
#include "mmi_gps_nvram_def.h"
#include "Bird_socket.h"

#define RJ_GPS_MAX_NMEA 				17
#define RJ_GPS_MAX_NMEA_LEN 			300

#define BD_HDDZ    // add by lqy for beijing huada  dainzi  beidou

#ifdef HD8020_GPS_SUPPORT
#define BIRD_AGPS_DEBUG  1
#define BIRD_WRITE_AGPS_DATA_DELAY 20
#define BIRD_GPS_CLOSE_OPEN_TEST 0
#define BIRD_AGPS_WRITE_5_GROUPS  0
//#define BIRD_AGPS_WRITE_POSTIME 0
#define BIRD_COLD_START_ENABLE 1
#define BIRD_COLD_START_ACK_COUNT 2  
#define BIRD_COLD_START_COUNT 2  
#define BIRD_AGPS_WRITE_COUNT 2  

#ifndef __GPS_SUPPORT__
#ifndef __BT_GPS_SUPPORT__
#define MDI_GPS_UART_PORT_MAX               3
#define MDI_GPS_UART_APP_PER_PORT_MAX       5
#define MDI_GPS_DUMMY_HANDLE                0xFF
#define MDI_GPS_DUMMY_PORT                  0xFF

#define MDI_GPS_NMEA_CATCHER_LOGGING_FLAG       (1<<0)
#define MDI_GPS_NMEA_FILE_LOGGING_FLAG          (1<<1)
#define MDI_GPS_NMEA_AGENT_LOGGING_FLAG         (1<<2)
#define MDI_GPS_NMEA_DEBUG_INFO_FLAG            (1<<3)
#define MDI_GPS_NMEA_POWER_SAVING_FLAG          (1<<4)
#define MDI_GPS_NMEA_HOTSTILL_FLAG              (1<<5)
#define MDI_GPS_NMEA_HOTSTILL_DEBUG_FLAG        (1<<6)
#define MDI_GPS_NMEA_LOGGING_DEFAULT_FLAG       (0x01)
#define MDI_GPS_NMEA_MESSAGE_MAX_LENGTH         (100)
#define MDI_GPS_NMEA_FILENAME_LENGTH            (60)
#define MDI_GPS_NMEA_FILE_LOGGING_WRITE_TIME    (20)
#define MDI_GPS_NMEA_FILE_DEBUGINOF_WRITE_TIME  (200)

#define MDI_GPS_RAW_DATA_PER_CB            (256)
#define MDI_GPS_RAW_DATA_PER_EXTEND             (1024*5)

#define MDI_GPS_NMEA_MAX_SVVIEW             20
#define MDI_GPS_NMEA_GL_MAX_SVVIEW          20
#define MDI_GPS_NMEA_GA_MAX_SVVIEW          20
#define MDI_GPS_PARSER_P_VERSION_MAX        32
#define MDI_GPS_OPEN_MODE_ERROR             0


/*Data type for callback function of NMEA data or parserred data*/
typedef enum
{
    MDI_GPS_PARSER_RAW_DATA = 0,    /*Raw data of NMEA*/
    MDI_GPS_PARSER_NMEA_GGA,        /*Data structure of GGA info*/
    MDI_GPS_PARSER_NMEA_GLL,        /*Data structure of GLL info*/
    MDI_GPS_PARSER_NMEA_GSA,        /*Data structure of GSA info*/
    MDI_GPS_PARSER_NMEA_GSV,        /*Data structure of GSV info*/
    MDI_GPS_PARSER_NMEA_RMC,        /*Data structure of RMC info*/
    MDI_GPS_PARSER_NMEA_VTG,        /*Data structure of VTG info*/
    MDI_GPS_PARSER_NMEA_GAGSA,       /*Data structure of GAGSA info*/
    MDI_GPS_PARSER_NMEA_GAGSV,       /*Data structure of GAGSV info*/
    MDI_GPS_PARSER_NMEA_GLGSA,       /*Data structure of GLGSA info*/
    MDI_GPS_PARSER_NMEA_GLGSV,       /*Data structure of GLGSV info*/
    MDI_GPS_PARSER_NMEA_TXT,        /*Data structure of TXT info*/
    MDI_GPS_PARSER_NMEA_SENTENCE,
    MDI_GPS_UART_EVENT_VPORT_LOST,  /*Virtual port is lost, maybe bluetooth connection is break(not support current)*/
    MDI_GPS_SHOW_AGPS_ICON,
    MDI_GPS_HIDE_AGPS_ICON,
    MDI_GPS_PARSER_NMEA_ACC,        /*Data structure of ACCURACY info*/
    MDI_GPS_PARSER_NMEA_END,
    MDI_GPS_PARSER_MA_STATUS = 255
} mdi_gps_parser_info_enum;


typedef enum
{
    MDI_GPS_PARSER_P_VERSION = 0,
    MDI_GPS_PARSER_P_GPS_MODE_SWITCH_OK,
    MDI_GPS_PARSER_P_SYSTEM_START_OK,
    MDI_GPS_PARSER_P_INTERNAL_VERSION,
    MDI_GPS_PARSER_P_GPS_FIXED,
    MDI_GPS_PARSER_P_GPS_FIXED_AND_TIME_OK,
    MDI_GPS_PARSER_P_END
} mdi_gps_parser_p_info_enum;

/*Enum of GPS work mode*/
typedef enum
{
    MDI_GPS_UART_MODE_RAW_DATA = 0,         /*Just need raw data*/
    MDI_GPS_UART_MODE_LOCATION,             /*Just need location*/
    MDI_GPS_UART_MODE_LOCATION_WITH_QOP     /*Need AGPS data with Qop*/
} mdi_gps_uart_work_mode_enum;

/*Enum of GPS command*/
typedef enum
{
    MDI_GPS_UART_GPS_WARM_START = 0,        /*Let GPS do warm start*/
    MDI_GPS_UART_GPS_HOT_START,             /*Let GPS do hot start*/
    MDI_GPS_UART_GPS_COLD_START,            /*Let GPS do cold start*/
    MDI_GPS_UART_GPS_VERSION,
    MDI_GPS_UART_GPS_ENABLE_DEBUG_INFO,    
    MDI_GPS_UART_GPS_SWITCH_MODE_MA,
    MDI_GPS_UART_GPS_SWITCH_MODE_MB,
    MDI_GPS_UART_GPS_SWITCH_MODE_NORMAL,
    MDI_GPS_UART_GPS_QUERY_POS,
    MDI_GPS_UART_GPS_QUERY_MEAS,
    MDI_GPS_UART_GPS_CLEAR_NVRAM,           /*Clear GPS NVRAM*/
    MDI_GPS_UART_GPS_AGPS_START,            /*Clear GPS data*/
    MDI_GPS_UART_GPS_SLEEP,                 /*Let GPS chip goto sleep mode*/
    MDI_GPS_UART_GPS_STOP,                  /*Let GPS chip stop*/
    MDI_GPS_UART_GPS_WAKE_UP,               /*Let GPS chip wake up from sleep mode*/
    MDI_GPS_UART_GPS_DUMMY = -1
} mdi_gps_uart_cmd_type_enum;

typedef struct
{            
    kal_int8      hour;
    kal_int8      minute;
    kal_int8      second;
    kal_int16     millisecond;
} mdi_gps_nmea_utc_time_struct;

typedef struct
{            
    kal_int8      year;
    kal_int8      month;
    kal_int8      day;
} mdi_gps_nmea_utc_date_struct;

/*GPTXT -- Global Positioning System Fix Data*/
typedef struct
{            
	int hed_0;
	int hed_1;
	int hed_2;
	char hed_string[20];	
} mdi_gps_nmea_txt_struct;

/*GPGGA -- Global Positioning System Fix Data*/
typedef struct
{            
    double  latitude;               /*Latitude South<0  North>0*/
    double  longitude;              /*Longitude West<0 east>0*/
    float   h_precision;            /*Horizontal Dilution of precision*/
    double  altitude;               /*Antenna Altitude above/below mean-sea-level (geoid)*/
    float   unit_of_altitude;       /*Units of antenna altitude, meters*/
    float   geoidal ;               /*Geoidal separation, the difference between the WGS-84 earth*/
    float   unit_of_geoidal;        /*Units of geoidal separation, meters*/
    float   gps_age;                /*Age of differential GPS data, time in seconds since last SC104*/
    kal_uint16    station_id;       /*Differential reference station ID, 0000-1023*/
    kal_uint8     sat_in_view;      /*Number of satellites in use*/
    mdi_gps_nmea_utc_time_struct    utc_time;       /*Time (UTC)*/
    kal_int8      north_south;      /*north or south*/
    kal_int8      east_west;        /*east or west*/
    kal_int8      quality;          /*GPS Quality Indicator*/
} mdi_gps_nmea_gga_struct;


 /*GPGSA -- GNSS DOP and Active Satellites*/
typedef struct 
{           
    float   pdop;       /*PDOP in meters*/
    float   hdop;       /*HDOP in meters*/
    float   vdop;       /*VDOP in meters*/
    kal_uint16     sate_id[12]; /*ID of satellites*/
    kal_int8      op_mode;      /*Selection mode: A=auto M=manual*/
    kal_int8      fix_mode;     /*Mode*/
} mdi_gps_nmea_gsa_struct;


/*GPGSV -- GNSS Satellites in View*/
typedef struct 
{            
    kal_int16     msg_sum;              /*total number of messages*/
    kal_int16     msg_index;            /*message number*/
    kal_int16     sates_in_view;        /*satellites in view*/
    kal_int16     max_snr;              /*Max snr*/
    kal_int16     min_snr;              /*Min snr*/
    kal_int16     num_sv_trk;           /*satellite number*/
    struct
    {
        kal_uint8 sate_id;              /*satellite id*/
        kal_uint8 elevation;            /*elevation in degrees*/
        kal_int16 azimuth;              /*azimuth in degrees to true*/
        kal_uint8 snr;                  /*SNR in dB*/
    }       rsv[MDI_GPS_NMEA_MAX_SVVIEW];
} mdi_gps_nmea_gsv_struct;


/*GPRMC -- Recommended Minimum Specific GNSS Data*/
typedef struct
{            
    double   latitude;          /*latitude*/
    double   longitude;         /*longitude*/
    float   ground_speed;       /*Speed over ground, knots*/
    float   trace_degree;       /*Track mode degrees,north is 0*/
    float   magnetic;
    mdi_gps_nmea_utc_time_struct      utc_time; /*UTC time*/
    mdi_gps_nmea_utc_date_struct      utc_date; /*UTC date*/
    kal_int8      status;       /*Status, V = Navigation receiver warning*/
    kal_int8      north_south;  /*N or S*/
    kal_int8      east_west;    /*E or W*/
    kal_int8      magnetic_e_w; /*Magnetic E or W*/
    kal_int8      cmode;        /*Mode*/
    kal_int8      nav_status;   /*navigational status*/
} mdi_gps_nmea_rmc_struct;


/*GPGLL -- Geographic Position - Latitude/Longitude*/
typedef struct
{            
    double  latitude;               /*latitude*/ 
    double  longitude;              /*longitude*/
    mdi_gps_nmea_utc_time_struct      utc_time;     /*UTC time*/
    kal_int8      north_south;      /*N or S*/
    kal_int8      east_west;        /*E or W*/
    kal_int8      status;           /*Status A - Data Valid, V - Data Invalid*/
    kal_int8      mode;             /*mode*/
} mdi_gps_nmea_gll_struct;


/*GPVTG -- VTG Data*/
typedef struct
{
    float   true_heading;   /*Track Degrees*/
    float   mag_heading;    /*Magnetic Track Degrees*/
    float   hspeed_knot;    /*Speed Knots*/
    float   hspeed_km;      /*Speed Kilometers Per Hour*/
    kal_int8      mode;     /*Mode*/
} mdi_gps_nmea_vtg_struct;

#endif
#endif

#ifdef BD_DGZN
#define BD_AGNSS "http://agnss.zsouth.com/api/GetGPS?usr=bird001&pwd=bird001&type=all"
#endif  //add by lqy

#ifdef BD_BJHK
#define BD_INITOUT "$PCAS03,5,5,5,5,5,5,5,5*02\r\n"
#endif  //add by lqy


#ifdef BD_HDDZ
#define BD_AGNSS_AGPS_0 "ftp://agps.hdbds.com/agps/file_gnss_aid_eph.hdb"
#define BD_AGNSS_AGPS_1 "http://doc.gpslink.cn/bddata/hdgps2.data" //"http://agps.hdbds.com/agps/file_gnss_aid_eph.hdb"
#define BD_AGNSS_ABDS_0 "ftp://agps.hdbds.com/abds/file_local_bd_aid_eph.hdb"
#define BD_AGNSS_ABDS_1 "http://doc.gpslink.cn/bddata/hdbd.data"//"http://agps.hdbds.com/abds/file_local_bd_aid_eph.hdb"
#endif
#endif

/* GPS */
typedef struct
{
	//base data
	S32 conn_port_handle;
	S32 port;
	volatile S8 satellite_num;								//卫星数
	S32 gps_status;									//GPS当前状态
	
	//GPS data
	//格林威治标准时间
	mdi_gps_nmea_utc_time_struct      utc_time; 			/* UTC time */
	mdi_gps_nmea_utc_date_struct      utc_date; 			/* UTC date */
	double latitude;									/* 纬度*/
	double longitude;									/* 经度*/
	float ground_speed;								/* 速度*/
	float drection;									/* 方位角 真北参照系*/
	S8 north_south;                                                          /* N or S*/
	S8 east_west;                                                             /* E or W*/

       kal_bool is_use_agps;
	U8 nmea_start;
	U8 nmea_end;
	S8 nmea_buffer[RJ_GPS_MAX_NMEA][RJ_GPS_MAX_NMEA_LEN];

    #if defined(__AGPS_USER_PLANE__) || defined(__AGPS_CONTROL_PLANE__)
    U8 gps_pos_type;
#endif


    #if defined(__AGPS_USER_PLANE__)
    supl_mmi_position_struct minigps_ma_position;
#endif
#if defined(__AGPS_CONTROL_PLANE__)
    U16 molr_req_id1;

    nvram_ef_agps_cp_config_struct agps_cp_config;
#endif
}RJ_gps_context_struct;

typedef struct
{
        kal_uint8 sate_id;              /*satellite id*/
        kal_uint8 snr;                  /*SNR in dB*/
        
}RJ_gps_engineer_satinfo;
#ifdef HD8020_GPS_SUPPORT
typedef struct
{
	S8 agps_buf[MAX_BIRD_RCV_BUFFER_SIZE];   /*peph data*/
	kal_int32 agps_len;						 /*total peph data len*/

	kal_int32 peph_len;						 /*one peph len*/	
	kal_int32 peph_pos;	

	U8 cold_start_ack_num;					 /*ack number*/
	U8 cold_start_times;						/*do cold start times*/
	U8 write_peph_times;					/*write peph times*/
	U8 write_peph_start;						/*start to write peph flag*/
	U8 peph_ack_ok_num;					/*peph ack ok number*/
	U8 peph_ack_err_num;					/*peph ack err number*/
	kal_int32 index;							/*peph buf indx*/
	U8 total_item;									/*peph data total item*/
	applib_time_struct get_peph_last_time;		/*get peph data last time*/
	applib_time_struct peph_write_over_time;	/*write peph over time*/
	applib_time_struct gps_close_time;	/*write peph over time*/
}peph_op_struct;

#endif
void bird_set_gpspositon(S8 updatekind);
extern RJ_gps_context_struct RJ_Gps_Info;
extern U8 RJ_GPS_is_working(void);
#ifdef HD8020_GPS_SUPPORT
extern void BIRD_Pre_Write_Agps_Data();
void bird_get_agps_pos(kal_uint32 *lati, kal_uint32 *logi, kal_uint32 *alti);
void bird_get_agps_llapos(kal_uint32 *lati, kal_uint32 *logi, kal_uint32 *alti);
void bird_set_agps_pos(double lati, double logi, double alti);
void BIRD_Gps_Stop(void);
void RJ_GET_AGPS(peph_op_struct *peph_op);
void HD8020_AGPS(peph_op_struct *peph_op);
#ifdef HD8020_ACK_CHECK
void bird_cold_start_check(peph_op_struct *peph_op);
#endif
void BIRD_Write_Agps_Data(peph_op_struct *peph_op);
void BIRD_GPS_Write_Peph_Data_Proc(void);
void bird_socket_write_agps_data(void);
#endif
