#ifdef BIRD_CAN_SUPPORT
#include "can.h"
#include    "eint.h"
#include 	"stack_common.h"  
#include 	"stack_msgs.h"
#include 	"app_ltlcom.h"       /* Task message communiction */
#include 	"syscomp_config.h"
#include 	"task_config.h"
#include 	"drv_comm.h"
#include    "drvsignals.h"
#include "mcp2515.h"
#include "bird_app.h"
#include "app_datetime.h"
#include "rj_dw02.h"
#include "bird_can_data_shanshan.h"

extern const unsigned char CAN2_EINT_NO;
extern const unsigned char CAN1_EINT_NO;
extern const unsigned char PUSH_EINT_NO;
extern can_data_save_struct car_can_data;

static kal_bool can1_eint_state = (kal_bool)KAL_FALSE;
static kal_bool can2_eint_state = (kal_bool)KAL_FALSE;
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
static kal_bool can_send_eint_state = (kal_bool)KAL_TRUE;
static kal_bool can_normal_data = (kal_bool)KAL_FALSE;
extern void can_simulate_eint_hisr(void);
#endif

volatile applib_time_struct cur_time;
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
typedef struct
{
	kal_int32 can_id;	
	kal_uint8 can_dlc;
	kal_uint8 can_data[8];					 /*ack number*/
}can_data_struct;
#ifdef CAN_SHANSHAN_SUPPORT
#define CAN_DATA_NUM 37

can_data_struct can_send_data_normal[CAN_DATA_NUM] = {
	{0x18F1FB27, 0x8, {0x48, 0x0, 0x0, 0x0, 0xB8, 0xB, 0x0, 0x85}},/*车辆状态 DCDC 加速踏板行程值 制动踏板状态*/
	{0x1818D0F3, 0x8, {0x57, 0x6, 0x4A, 0x7D, 0xFA, 0x0, 0x0, 0x0}},/*总电压 总电流 SOC SOC过高报警 绝缘报警 储能装置电压/电流*/
	{0x1819D0F3, 0x8, {0x38, 0x10, 0x4C, 0x10, 0x43, 0x43, 0x6E, 0x4E}}, /*电池单体电压最高/低值 最高/低温度值*/
	{0x181AD0F3, 0x8, {0x1, 0xA, 0x1, 0x6, 0x1, 0x1, 0x1, 0x1}},/*最高/低电压电池子系统 最高/低电压电池单体 最高/低温度子系统 探针单体代号*/
	{0x181BD0F3, 0x8, {0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x25}},/*温度差异 电池高温 储能装置过/欠压 SOC低 单体电池过/欠压 SOC跳变 储能系统不匹配 单体一致性差 储能装置过充 充电状态 绝缘电阻*/
	{0x0C1127EF, 0x8, {0x2, 0x0, 0xF8, 0x3, 0xA2, 0x0, 0xEB, 0x3}},/*电机实际转速/转矩 母线电压 相电流*/    /*一秒没有*/
	{0x0C1227EF, 0x8, {0xB4, 0x0, 0xB4, 0x0, 0x4A, 0x44, 0x64, 0x19}},/*电机控制器温度 电机温度*/
	{0x0C1327EF, 0x8, {0x58, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},/*驱动电机状态 电机控制器温度报警 电机温度报警*/
	{0x18FA01FB, 0x8, {0x0 ,0x0  ,0x13 ,0x88 ,0x47 ,0x1  ,0x0  ,0x0 }},
	{0x0801EF27, 0x8, {0x1 ,0x67 ,0x3  ,0x67 ,0x5  ,0x67 ,0x7  ,0x67}},
	{0x10088A9E, 0x8, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},/*DCDC 状态报警*/
	{0x10098A9E, 0x8, {0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},/*车速*/
	{0x10098B9E, 0x8, {0x63, 0x0, 0x0, 0x0, 0x0, 0x43, 0x0, 0x0}},/*档位信息*/
	{0x18FA01FC, 0x8, {0xA6, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},/*累计里程*/
	{0x18FA01FA, 0x8, {0x29, 0x67, 0x2B, 0x67, 0x2D, 0x67, 0x2F,0x67}},
	{0x18FF50E5, 0x8, {0x31, 0x67, 0x33, 0x67, 0x35, 0x67, 0x37,0x67}},
	{0x1801A7E5, 0x8, {0x39, 0x67, 0x3B, 0x67, 0x3D, 0x67, 0x3F,0x67}},
	{0x18FDFF2A, 0x8, {0x41, 0x67, 0x43, 0x67, 0x45, 0x67, 0x47,0x67}},
	{0x18FDFF1E, 0x8, {0x49, 0x67, 0x4B, 0x67, 0x4D, 0x67, 0x4F,0x67}},
	{0x18FDFF13, 0x8, {0x51, 0x67, 0x53, 0x67, 0x55, 0x67, 0x57,0x67}},
	{0x18FDFF1A, 0x8, {0x59, 0x67, 0x5B, 0x67, 0x5D, 0x67, 0x5F,0x67}},
	
	{0x18FF97D2, 0x8, {0x1, 0x68, 0xE, 0x68, 0xE, 0x68, 0xE, 0xFE}}, /*单体电压序号 单体电压1....单体电压N*/
	{0x18FF97D2, 0x8, {0x4, 0x6A, 0xE, 0x6A, 0xE, 0x6A, 0xE, 0xFB}}, /*单体电压序号 单体电压4....单体电压N*/
	{0x18FF97D2, 0x8, {0x7, 0x82, 0xE, 0x82, 0xE, 0x82, 0xE, 0xF8}}, /*单体电压序号 单体电压7....单体电压N*/
	{0x18FF97D2, 0x8, {0xA, 0x82, 0xE, 0x82, 0xE, 0x82, 0xE, 0xF5}}, /*单体电压序号 单体电压A....单体电压N*/
	{0x18FF97D2, 0x8, {0xD, 0x82, 0xE, 0x80, 0xE, 0x85, 0xE, 0xF2}}, /*单体电压序号 单体电压D....单体电压N*/
	{0x18FF97D2, 0x8, {0x10, 0x82, 0xE, 0x83, 0xE, 0x82, 0xE, 0xEF }}, /*单体电压序号 单体电压10....单体电压N*/  
	{0x18FF97D2, 0x8, {0x13, 0x84, 0xE, 0x82, 0xE, 0x82, 0xE, 0xEC }}, /*单体电压序号 单体电压13....单体电压N*/  
	{0x18FF97D2, 0x8, {0x16, 0x82, 0xE, 0x82, 0xE, 0x83, 0xE, 0xE9 }}, /*单体电压序号 单体电压16....单体电压N*/  
	{0x18FF97D2, 0x8, {0x19, 0x81, 0xE, 0x82, 0xE, 0x82, 0xE, 0xE6 }}, /*单体电压序号 单体电压19....单体电压N*/  
	{0x18FF97D2, 0x8, {0x1F, 0x81, 0xE, 0x81, 0xE, 0x81, 0xE, 0xE0 }}, /*单体电压序号 单体电压1F....单体电压N*/	
	{0x18FF97D2, 0x8, {0x1C, 0x7F, 0xE, 0x80, 0xE, 0x81, 0xE, 0xE3 }}, /*单体电压序号 单体电压1C....单体电压N*/  
	{0x18FF97D2, 0x8, {0x22, 0x81, 0xE, 0x81, 0xE, 0x81, 0xE, 0xDD }}, /*单体电压序号 单体电压22....单体电压N*/  
	{0x18FF97D2, 0x8, {0x25, 0x80, 0xE, 0x81, 0xE, 0x81, 0xE, 0xDA }}, /*单体电压序号 单体电压25....单体电压N*/	
	
	{0x18FF98D2, 0x8, {0x1, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0xFE}},/*温度序号 温度1.....温度N*/
	{0x18FF98D2, 0x8, {0x7, 0x43, 0x43, 0x43, 0x43, 0xFF, 0xFF, 0xF8}},/*温度序号 温度7.....温度N*/
	{0x1806E5F4, 0x8, {0x1 ,0x67 ,0x1  ,0x66 ,0x4B ,0x4A ,0x0  ,0x0 }},
};

can_data_struct can_send_data[CAN_DATA_NUM] = {
	{0x18F1FB27, 0x8, {0x48 ,0x55, 0x10 ,0x0  ,0x27 ,0x10 ,0x19 ,0x1 }},/*车辆状态 DCDC 加速踏板行程值 制动踏板状态*/
	{0x1818D0F3, 0x8, {0x25 ,0x00, 0x15, 0x00, 0x90, 0x40 ,0x20, 0x0 }},/*总电压 总电流 SOC SOC过高报警 绝缘报警 储能装置电压/电流*/
	{0x1819D0F3, 0x8, {0x18, 0x0, 0x14, 0x0, 0x50, 0x0A, 0x0, 0x0}}, /*电池单体电压最高/低值 最高/低温度值*/
	{0x181AD0F3, 0x8, {0x06, 0x42, 0x04, 0x2c, 0x09, 0x08, 0x03, 0x02}},/*最高/低电压电池子系统 最高/低电压电池单体 最高/低温度子系统 探针单体代号*/
	{0x181BD0F3, 0x8, {0x44, 0x11, 0x12,0x00, 0x01, 0x01, 0x23, 0x01}},/*温度差异 电池高温 储能装置过/欠压 SOC低 单体电池过/欠压 SOC跳变 储能系统不匹配 单体一致性差 储能装置过充 充电状态 绝缘电阻*/
	{0x0C1127EF, 0x8, {0x2, 0x0, 0xF8, 0x3, 0xA2, 0x0, 0xEB, 0x3}},/*电机实际转速/转矩 母线电压 相电流*/    /*一秒没有*/
	{0x0C1227EF, 0x8, {0xB4, 0x0, 0xB4, 0x0, 0x4A, 0x44, 0x64, 0x19}},/*电机控制器温度 电机温度*/
	{0x0C1327EF, 0x8, {0x58, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}},/*驱动电机状态 电机控制器温度报警 电机温度报警*/
	{0x18FA01FB, 0x8, {0x0 ,0x0  ,0x13 ,0x88 ,0x47 ,0x1  ,0x0  ,0x0 }},
	{0x0801EF27, 0x8, {0x1 ,0x67 ,0x3  ,0x67 ,0x5  ,0x67 ,0x7  ,0x67}},
	{0x10088A9E, 0x8, {0x00, 0x00, 0x00,0x00, 0x00, 0x08, 0x00, 0x00}},/*DCDC 状态报警*/
	{0x10098A9E, 0x8, {0x84, 0x03, 0x00, 0x67, 0x15, 0x67, 0x17,0x67}},/*车速*/
	{0x10098B9E, 0x8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},/*档位信息*/
	{0x18FA01FC, 0x8, {0x40, 0x1F, 0x00, 0x00, 0x25, 0x67, 0x27,0x67}},/*累计里程*/
	{0x18FA01FA, 0x8, {0x29, 0x67, 0x2B, 0x67, 0x2D, 0x67, 0x2F,0x67}},
	{0x18FF50E5, 0x8, {0x31, 0x67, 0x33, 0x67, 0x35, 0x67, 0x37,0x67}},
	{0x1801A7E5, 0x8, {0x39, 0x67, 0x3B, 0x67, 0x3D, 0x67, 0x3F,0x67}},
	{0x18FDFF2A, 0x8, {0x41, 0x67, 0x43, 0x67, 0x45, 0x67, 0x47,0x67}},
	{0x18FDFF1E, 0x8, {0x49, 0x67, 0x4B, 0x67, 0x4D, 0x67, 0x4F,0x67}},
	{0x18FDFF13, 0x8, {0x51, 0x67, 0x53, 0x67, 0x55, 0x67, 0x57,0x67}},
	{0x18FDFF1A, 0x8, {0x59, 0x67, 0x5B, 0x67, 0x5D, 0x67, 0x5F,0x67}},
	
	{0x18FF97D2, 0x8, {0x1, 0x68, 0xE, 0x68, 0xE, 0x68, 0xE, 0xFE}}, /*单体电压序号 单体电压1....单体电压N*/
	{0x18FF97D2, 0x8, {0x4, 0x6A, 0xE, 0x6A, 0xE, 0x6A, 0xE, 0xFB}}, /*单体电压序号 单体电压4....单体电压N*/
	{0x18FF97D2, 0x8, {0x7, 0x82, 0xE, 0x82, 0xE, 0x82, 0xE, 0xF8}}, /*单体电压序号 单体电压7....单体电压N*/
	{0x18FF97D2, 0x8, {0xA, 0x82, 0xE, 0x82, 0xE, 0x82, 0xE, 0xF5}}, /*单体电压序号 单体电压A....单体电压N*/
	{0x18FF97D2, 0x8, {0xD, 0x82, 0xE, 0x80, 0xE, 0x85, 0xE, 0xF2}}, /*单体电压序号 单体电压D....单体电压N*/
	{0x18FF97D2, 0x8, {0x10, 0x82, 0xE, 0x83, 0xE, 0x82, 0xE, 0xEF }}, /*单体电压序号 单体电压10....单体电压N*/  
	{0x18FF97D2, 0x8, {0x13, 0x84, 0xE, 0x82, 0xE, 0x82, 0xE, 0xEC }}, /*单体电压序号 单体电压13....单体电压N*/  
	{0x18FF97D2, 0x8, {0x16, 0x82, 0xE, 0x82, 0xE, 0x83, 0xE, 0xE9 }}, /*单体电压序号 单体电压16....单体电压N*/  
	{0x18FF97D2, 0x8, {0x19, 0x81, 0xE, 0x82, 0xE, 0x82, 0xE, 0xE6 }}, /*单体电压序号 单体电压19....单体电压N*/  
	{0x18FF97D2, 0x8, {0x1F, 0x81, 0xE, 0x81, 0xE, 0x81, 0xE, 0xE0 }}, /*单体电压序号 单体电压1F....单体电压N*/	
	{0x18FF97D2, 0x8, {0x1C, 0x7F, 0xE, 0x80, 0xE, 0x81, 0xE, 0xE3 }}, /*单体电压序号 单体电压1C....单体电压N*/  
	{0x18FF97D2, 0x8, {0x22, 0x81, 0xE, 0x81, 0xE, 0x81, 0xE, 0xDD }}, /*单体电压序号 单体电压22....单体电压N*/  
	{0x18FF97D2, 0x8, {0x25, 0x80, 0xE, 0x81, 0xE, 0x81, 0xE, 0xDA }}, /*单体电压序号 单体电压25....单体电压N*/	
	
	{0x18FF98D2, 0x8, {0x1, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0xFE}},/*温度序号 温度1.....温度N*/
	{0x18FF98D2, 0x8, {0x7, 0x43, 0x43, 0x43, 0x43, 0xFF, 0xFF, 0xF8}},/*温度序号 温度7.....温度N*/
	{0x1806E5F4, 0x8, {0x1 ,0x67 ,0x1  ,0x66 ,0x4B ,0x4A ,0x0  ,0x0 }},
};
#else
#define CAN_DATA_NUM 27
can_data_struct can_send_data[CAN_DATA_NUM] = {
{0x1801D0F3, 0x8, {0x7D ,0x0  ,0x7D ,0x0  ,0x13 ,0x88 ,0x13 ,0x88}},
{0x180228D0, 0x8, {0x1  ,0x0  ,0x0  ,0x0  ,0x0  ,0x19 ,0x0  ,0x0 }},
{0x180328D0, 0x8, {0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0 }},
{0x18FF98D2, 0x8, {0x13 ,0x0  ,0x0  ,0xFF ,0xFF ,0xFF ,0xFF ,0xEC}},
{0x180528F3, 0x8, {0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0 }},
{0x18FF9CD2, 0x8, {0xF  ,0x0  ,0x0  ,0xFF ,0xFF ,0xFF ,0xFF ,0xF0}},
{0x18FF9AD2, 0x8, {0x0  ,0x0  ,0x7D ,0x0  ,0x0  ,0x47 ,0x0  ,0xFF}},
{0x18FF9AD2, 0x8, {0x1  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0xFE}},
{0x18FF9AD2, 0x8, {0x2  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0xFD}},
{0x18FF9AD2, 0x8, {0x3  ,0x0  ,0x0  ,0x20 ,0x3  ,0xE6 ,0xA  ,0xFC}},
{0x18FF9AD2, 0x8, {0x4  ,0xFF ,0xFF ,0xFF ,0xFF ,0x53 ,0x14 ,0xFB}},
{0x18FF9AD2, 0x8, {0x5  ,0x23 ,0x7  ,0x8  ,0x5  ,0x40 ,0x1  ,0xFA}},
{0x18FF90D2, 0x8, {0xF0 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xD2}},
{0x18FF90D2, 0x8, {0xA5 ,0xFF ,0xFF ,0xFF ,0xCF ,0xFF ,0xFF ,0xD2}},
{0x181B28F3, 0x8, {0x0  ,0x0  ,0x53 ,0x5D ,0xFF ,0xFF ,0x0  ,0x0 }},
{0x182756F4, 0x2, {0x48 ,0xD  ,0x53 ,0x5D ,0xFF ,0xFF ,0x0  ,0x0 }},
{0x1800D0F3, 0x8, {0x7  ,0x13 ,0x88 ,0x0  ,0x0  ,0x5  ,0x47 ,0x0 }},
{0x18DF28F3, 0x8, {0x38 ,0x0  ,0x3A ,0x0  ,0x3C ,0x0  ,0x3E ,0x0 }},
{0x181C28F3, 0x8, {0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0 }},
{0x18FF97D2, 0x8, {0x16 ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0xE9}},
{0x18FF9CD2, 0x8, {0x10 ,0x0  ,0x0  ,0xFF ,0xFF ,0xFF ,0xFF ,0xEF}},
{0x18FF9AD2, 0x8, {0x6  ,0x0  ,0x23 ,0x2D ,0x1E ,0x37 ,0x0  ,0xF9}},
{0x18FF9AD2, 0x8, {0x8  ,0x0  ,0x0  ,0x0  ,0xFD ,0xFF ,0x30 ,0xF7}},
{0x18FF9AD2, 0x8, {0x9  ,0xFA ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0xF6}},
{0x18FF9AD2, 0x8, {0xA  ,0xC0 ,0x0  ,0x0  ,0xFF ,0xFF ,0xFF ,0xF5}},
{0x18FF9AD2, 0x8, {0xB  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0x0  ,0xF4}},
{0x18FF9AD2, 0x8, {0x7  ,0x36 ,0x1  ,0x32 ,0x0  ,0xC2 ,0x1  ,0xF8}},
};
#endif
#endif 

	
#define CANINTF_MERRF 0x80
#define CANINTF_WAKIF 0x40
#define CANINTF_ERRIF 0x20
#define CANINTF_TX2IF 0x10
#define CANINTF_TX1IF 0x08
#define CANINTF_TX0IF 0x04
#define CANINTF_RX1IF 0x02
#define CANINTF_RX0IF 0x01

#define RXBCTRL_OFF 0
#define RXBSIDH_OFF 1
#define RXBSIDL_OFF 2
#define RXBEID8_OFF 3
#define RXBEID0_OFF 4
#define RXBDLC_OFF  5
#define RXBDAT_OFF  6

#define CANINTF_RX (CANINTF_RX0IF | CANINTF_RX1IF)
#define CANINTF_TX (CANINTF_TX2IF | CANINTF_TX1IF | CANINTF_TX0IF)
#define CANINTF_ERR (CANINTF_ERRIF)

#define RXBCTRL(n)  (((n) * 0x10) + 0x60 + RXBCTRL_OFF)
#define CAN_FRAME_MAX_DATA_LEN	8
#define SPI_TRANSFER_BUF_LEN	(6 + CAN_FRAME_MAX_DATA_LEN)
#define CAN_FRAME_MAX_BITS	128

#define RXBDLC_LEN_MASK  0x0f

#define CAN_ID_NUM   	65
#define CAN_UNIT_NUM   	12
#define CAN_DATA_LENGTH 8
kal_uint8 can_rx_buf[CAN_ID_NUM][CAN_UNIT_NUM] = {0};
kal_uint8 can_rx_temp_buf[CAN_ID_NUM][CAN_UNIT_NUM] = {0};

extern void can_rx_data_check();

void can_id_fill(kal_uint8 *unit, kal_uint32 id)
{
	kal_uint8 i = 0;
	
	unit[0] = (id & 0xff000000) >>24;
	unit[1] = (id & 0xff0000) >>16;
	unit[2] = (id & 0xff00)>>8;
	unit[3] = (id & 0xff);
}

void can_table_create(void)
{
	kal_uint8 i = 0, j=0;
	
	kal_prompt_trace(MOD_USB, "can_table_create");
#ifdef CAN_SHANSHAN_SUPPORT
	can_table_create_shanshan();
#else
	for (i = 0; i<CAN_ID_NUM;i++)
	{
		for(j = 0;j < CAN_UNIT_NUM; j++)
		{
			can_rx_temp_buf[i][j] = 0;
		}
	}
	can_id_fill(can_rx_temp_buf[0],	0x180128D0);  
	can_id_fill(can_rx_temp_buf[1],	0x180228D0);  
	can_id_fill(can_rx_temp_buf[2],	0x180328D0);  
	can_id_fill(can_rx_temp_buf[3],	0x180428D0);  
	can_id_fill(can_rx_temp_buf[4],	0x180528D0);  
	can_id_fill(can_rx_temp_buf[5],	0x181C28F3);  
	can_id_fill(can_rx_temp_buf[6],	0x1801D0F3);  
	can_id_fill(can_rx_temp_buf[7],	0x1803D0F3);  
	can_id_fill(can_rx_temp_buf[8],	0x1800D028);  
	can_id_fill(can_rx_temp_buf[9],	0x1801D08F);  
	can_id_fill(can_rx_temp_buf[10],	0x180228F3);
	can_id_fill(can_rx_temp_buf[11],	0x181B28F3);
	can_id_fill(can_rx_temp_buf[12],	0x181D28F3);
	can_id_fill(can_rx_temp_buf[13],	0xC00D0EF );
	can_id_fill(can_rx_temp_buf[14],	0x180028E5);
	can_id_fill(can_rx_temp_buf[15],	0x18E628F3);    // charg
	can_id_fill(can_rx_temp_buf[16],	0x18E528F3);
	can_id_fill(can_rx_temp_buf[17],	0x18E428F3);
	can_id_fill(can_rx_temp_buf[18],	0x18E328F3);
	can_id_fill(can_rx_temp_buf[19],	0x18E228F3);
	can_id_fill(can_rx_temp_buf[20],	0x18E128F3);
	can_id_fill(can_rx_temp_buf[21],	0x18E028F3);
	can_id_fill(can_rx_temp_buf[22],	0x18DF28F3);
	can_id_fill(can_rx_temp_buf[23],	0x18DE28F3);
	can_id_fill(can_rx_temp_buf[24],	0x18DD28F3);
	can_id_fill(can_rx_temp_buf[25],	0x18DC28F3);
	can_id_fill(can_rx_temp_buf[26],	0x18DB28F3);
	can_id_fill(can_rx_temp_buf[27],	0x18DA28F3);
	can_id_fill(can_rx_temp_buf[28],	0x18D928F3);
	can_id_fill(can_rx_temp_buf[29],	0x18D828F3);
	can_id_fill(can_rx_temp_buf[30],	0x18D728F3);
	can_id_fill(can_rx_temp_buf[31],	0x18D628F3);
	can_id_fill(can_rx_temp_buf[32],	0x18D528F3);
	can_id_fill(can_rx_temp_buf[33],	0x18D428F3);
	can_id_fill(can_rx_temp_buf[34],	0x18D328F3);
	can_id_fill(can_rx_temp_buf[35],	0x18D228F3);
	can_id_fill(can_rx_temp_buf[36],	0x18D128F3);
	can_id_fill(can_rx_temp_buf[37],	0x18D028F3);
	can_id_fill(can_rx_temp_buf[38],	0x18CF28F3);
	can_id_fill(can_rx_temp_buf[39],	0x18CE28F3);
	can_id_fill(can_rx_temp_buf[40],	0x18CD28F3);
	can_id_fill(can_rx_temp_buf[41],	0x18CC28F3);
	can_id_fill(can_rx_temp_buf[42],	0x18CB28F3);
	can_id_fill(can_rx_temp_buf[43],	0x18CA28F3);
	can_id_fill(can_rx_temp_buf[44],	0x18C928F3);
	can_id_fill(can_rx_temp_buf[45],	0x18C828F3);
	can_id_fill(can_rx_temp_buf[46],	0x18C728F3);
	can_id_fill(can_rx_temp_buf[47],	0x18C628F3);
	can_id_fill(can_rx_temp_buf[48],	0x18C528F3);
	can_id_fill(can_rx_temp_buf[49],	0x18C428F3);
	can_id_fill(can_rx_temp_buf[50],	0x18C328F3);
	can_id_fill(can_rx_temp_buf[51],	0x18C228F3);
	can_id_fill(can_rx_temp_buf[52],	0x18C128F3);
	can_id_fill(can_rx_temp_buf[53],	0x18C028F3);
	can_id_fill(can_rx_temp_buf[54],	0x18BF28F3);
	can_id_fill(can_rx_temp_buf[55],	0x18BE28F3);
	can_id_fill(can_rx_temp_buf[56],	0x18BD28F3);
	can_id_fill(can_rx_temp_buf[57],	0x18BC28F3);
	can_id_fill(can_rx_temp_buf[58],	0x18BB28F3);
	can_id_fill(can_rx_temp_buf[59],	0x18BA28F3);
	can_id_fill(can_rx_temp_buf[60],	0x18B928F3);
	can_id_fill(can_rx_temp_buf[61],	0x18B828F3);
	can_id_fill(can_rx_temp_buf[62],	0x18B728F3);
	can_id_fill(can_rx_temp_buf[63],	0x18B628F3);
	can_id_fill(can_rx_temp_buf[64],	0x18B528F3);
#endif	
}
kal_uint8 can_selfcheck_flag =0;
void can_unit_fill(kal_uint32 id, kal_uint8 *data)
{
	kal_uint8 i = 0, id_value[4] = {0};
	static kal_uint8 old_sec = 0,cnt = 0;

	id_value[0] = (id & 0xff000000) >>24;
	id_value[1] = (id & 0xff0000) >>16;
	id_value[2] = (id & 0xff00)>>8;
	id_value[3] = (id & 0xff);

	kal_prompt_trace(MOD_USB, "can_unit_fill id_value[0] = %x, id_value[1]= %x, id_value[2]= %x, id_value[3]= %x", id_value[0], id_value[1], id_value[2], id_value[3]);	
#ifdef CAN_SHANSHAN_SUPPORT	
	if (id == 0x18FF97D2)
	{
		kal_prompt_trace(MOD_USB, "can_unit_fill data[0] = %d", data[0]);
		memcpy(&can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][4], data, CAN_DATA_LENGTH);
#if	SHANSHAN_CAN_DEBUG	
		kal_prompt_trace(MOD_USB, "can_unit_fill data[5] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][5]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[6] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][6]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[7] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][7]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[8] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][8]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[9] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][9]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[10] = %x", can_rx_temp_buf[CAN_VOLTAGE_NUM + (data[0]-1)/CAN_VOLTAGE_DATA_LENGTH][10]);
#endif		
	}
	else if (id == 0x18FF98D2)
	{
		kal_prompt_trace(MOD_USB, "can_unit_fill data[0] = %d", data[0]);
		memcpy(&can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][4], data, CAN_DATA_LENGTH);
#if SHANSHAN_CAN_DEBUG		
		kal_prompt_trace(MOD_USB, "can_unit_fill data[5] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][5]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[6] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][6]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[7] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][7]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[8] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][8]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[9] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][9]);
		kal_prompt_trace(MOD_USB, "can_unit_fill data[10] = %x", can_rx_temp_buf[CAN_TEMPERATURE_NUM + (data[0]-1)/CAN_TEMPERATURE_DATA_LENGTH][10]);
#endif		
	}	
#endif
	for(i = 0; i< CAN_SINGLE_UNIT_NUM; i++)
	{
		if((can_rx_temp_buf[i][0] == id_value[0]) && (can_rx_temp_buf[i][1] == id_value[1]) 
			&& (can_rx_temp_buf[i][2] == id_value[2]) && (can_rx_temp_buf[i][3] == id_value[3]))
		{
			memcpy(&can_rx_temp_buf[i][4], data, CAN_DATA_LENGTH);
			can_selfcheck_flag =1;
		}
	}
	
    	applib_dt_get_rtc_time(&cur_time);
	kal_prompt_trace(MOD_USB, "cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);		
	if(cnt = 0)
	{
	old_sec = cur_time.nSec;
	cnt = 1;
	}
	if(cur_time.nSec != old_sec)
	{
		car_can_data.date[0] = cur_time.nYear %100;    
		car_can_data.date[1] = cur_time.nMonth;
		car_can_data.date[2] = cur_time.nDay;
		car_can_data.date[3] = cur_time.nHour;
		car_can_data.date[4] = cur_time.nMin;
		car_can_data.date[5] = cur_time.nSec;
		memcpy(can_rx_buf,can_rx_temp_buf, CAN_ID_NUM*CAN_UNIT_NUM);
		bird_send_message(MSG_ID_CAN_RX_CHECK);
	//	can_rx_data_check();
		kal_prompt_trace(MOD_USB, "can_rx_data_check  cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);	
		old_sec = cur_time.nSec;
	}
	
}


kal_uint8 bird_can_selfcheck_main()
{
	return can_selfcheck_flag;
}
void get_time_check_can_data(void)
{
	static kal_uint8 old_sec = 0,cnt = 0;
    	applib_dt_get_rtc_time(&cur_time);
	kal_prompt_trace(MOD_USB, "cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nMin,cur_time.nSec);		
	if(cnt = 0)
	{
	old_sec = cur_time.nSec;
	cnt = 1;
	}
	if(cur_time.nSec != old_sec)
	{
		car_can_data.date[0] = cur_time.nYear %100;    
		car_can_data.date[1] = cur_time.nMonth;
		car_can_data.date[2] = cur_time.nDay;
		car_can_data.date[3] = cur_time.nHour;                  
		car_can_data.date[4] = cur_time.nMin;
		car_can_data.date[5] = cur_time.nSec;
		
	//	memcpy(can_rx_buf,can_rx_temp_buf, CAN_ID_NUM*CAN_UNIT_NUM);
		bird_send_message(MSG_ID_CAN_RX_CHECK);
	//	can_rx_data_check();
		kal_prompt_trace(MOD_USB, "can_rx_data_check nYear = %d, nMonth = %d, nDay = %d, nHour = %d, cur_time.nMin = %d , cur_time.nSec = %d ,", cur_time.nYear,car_can_data.date[1],car_can_data.date[2],car_can_data.date[3],cur_time.nMin,cur_time.nSec);	
		old_sec = cur_time.nSec;
	}
}


void can1_eint_hisr(void)
{	
	ilm_struct       *aux_ilm = NULL;
	
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_CAN1_HW_RX, NULL); 
	msg_send_ext_queue(aux_ilm);  
	kal_prompt_trace(MOD_USB, "can1_eint_hisr");	
}
kal_uint16 CAN2_SID=0x00;
kal_uint32 CAN2_EID=0x0000;
kal_uint8 CAN2_DATA[8]= {0};
kal_uint8 CAN2_DLC = 0x00;

kal_uint16 CAN1_SID=0x00;
kal_uint32 CAN1_EID=0x0000;
kal_uint8 CAN1_DATA[8]= {0};
kal_uint8 CAN1_DLC = 0x00;

void can2_hw_rx(void)
{
	kal_uint8  data = 0, intf = 0, i = 0, j = 0;
	kal_uint8 buf[SPI_TRANSFER_BUF_LEN];
	S8 doublechar[40];
	kal_uint32 rec_id = 0;

	intf = MCP2515_byte_read2(CANINTF);
	if (intf == 0)
		return;
#if CAN_DEBUG		
	kal_prompt_trace(MOD_USB, "can2_hw_rx CANINTF = %x", intf);	
#endif
	/* mask out flags we don't care about */
	intf &= CANINTF_RX | CANINTF_TX | CANINTF_ERR;
	/* receive buffer 0 */
	if (intf & CANINTF_RX0IF) {
		kal_prompt_trace(MOD_USB, "can2_hw_rx receive buffer 0");	
		for (i = 1; i < RXBDAT_OFF; i++){
			buf[i] = MCP2515_byte_read2(RXBCTRL(0) + i);
#if CAN_DEBUG			
			kal_prompt_trace(MOD_USB, "can2_hw_rx buf[%d] = %x", i, buf[i]);	
#endif
		}

		CAN2_DLC = (kal_uint8)(buf[5] & 0x0F);
		for (j = 0; i < (RXBDAT_OFF + CAN2_DLC); i++, j++){
			CAN2_DATA[j] = MCP2515_byte_read2(RXBCTRL(0) + i);
#if CAN_DEBUG		
			kal_prompt_trace(MOD_USB, "can2_hw_rx DATA[%d] = %x", j, CAN2_DATA[j]);
#endif
		}

	       CAN2_SID = (unsigned int)(buf[1]<<3);
		CAN2_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);
		
		CAN2_EID = (kal_uint32)((buf[2] & 0x03)<<16);
		CAN2_EID |= (kal_uint32)(buf[3]<<8);
		CAN2_EID |= (kal_uint32)(buf[4]);
		
		CAN2_DLC = (kal_uint8)(buf[5] & 0x0F);
		
		rec_id = CAN2_EID & 0x3ffff;
		rec_id |= CAN2_SID << 18;
		can_unit_fill(rec_id, CAN2_DATA);
		
		kal_prompt_trace(MOD_USB, "rec_id = %x, CAN2_DLC = %x, data:%x %x %x %x %x %x %x %x", rec_id, CAN2_DLC, CAN2_DATA[0], CAN2_DATA[1], CAN2_DATA[2], CAN2_DATA[3], CAN2_DATA[4], CAN2_DATA[5], CAN2_DATA[6], CAN2_DATA[7]);
		MCP2515_byte_write2(CANINTF, 0x00);		
	}

	/* receive buffer 1 */
	if (intf & CANINTF_RX1IF) {
		kal_prompt_trace(MOD_USB, "can2_hw_rx receive buffer 1");
		for (i = 1; i < RXBDAT_OFF; i++)
			buf[i] = MCP2515_byte_read2(RXBCTRL(1) + i);

		//len = get_can_dlc(buf[RXBDLC_OFF] & RXBDLC_LEN_MASK);
		for (; i < (RXBDAT_OFF + 8); i++)
			buf[i] = MCP2515_byte_read2(RXBCTRL(1) + i);
	       CAN2_SID = (unsigned int)(buf[1]<<3);
		CAN2_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);
		
		CAN2_EID = (kal_uint32)((buf[2] & 0x03)<<16);
		CAN2_EID |= (kal_uint32)(buf[3]<<8);
		CAN2_EID |= (kal_uint32)(buf[4]);
		
		CAN2_DLC = (kal_uint8)(buf[5] & 0x0F);
		
		rec_id = CAN2_EID & 0x3ffff;
		rec_id |= CAN2_SID << 18;
		can_unit_fill(rec_id, CAN2_DATA);
		
		kal_prompt_trace(MOD_USB, "rec_id = %x, CAN2_DLC = %x, data:%x %x %x %x %x %x %x %x", rec_id, CAN2_DLC, CAN2_DATA[0], CAN2_DATA[1], CAN2_DATA[2], CAN2_DATA[3], CAN2_DATA[4], CAN2_DATA[5], CAN2_DATA[6], CAN2_DATA[7]);
		MCP2515_byte_write2(CANINTF, 0x00);		
	}	
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT	
		/* complete send buffer 1 */
	if (intf & CANINTF_TX0IF) {
		MCP2515_byte_write2(CANINTF, 0x00);	
		kal_sleep_task(1);
		can2_send_data();
	}
#endif	
}

static void Delayus(kal_uint32 data)
{
	kal_uint16 i;
	while(data--)
	{
		for(i=0;i<90;i++){}
	}
}

void can1_hw_rx(void)
{
	kal_uint8  data = 0, intf = 0, i = 0, j = 0;
	kal_uint8 buf[SPI_TRANSFER_BUF_LEN];
	kal_uint32 rec_id = 0;

	intf = MCP2515_byte_read1(CANINTF);
	if (intf == 0)
		return;		
#if CAN_DEBUG		
	kal_prompt_trace(MOD_USB, "can_hw1_rx CANINTF = %x", intf);		
#endif
	/* mask out flags we don't care about */
	intf &= CANINTF_RX | CANINTF_TX | CANINTF_ERR;
	/* receive buffer 0 */
	if (intf & CANINTF_RX0IF) {
#if CAN_DEBUG			
		kal_prompt_trace(MOD_USB, "can_hw1_rx receive buffer 0");	
#endif
		for (i = 1; i < RXBDAT_OFF; i++){
			buf[i] = MCP2515_byte_read1(RXBCTRL(0) + i);
#if CAN_DEBUG				
			kal_prompt_trace(MOD_USB, "can_hw1_rx buf[%d] = %x", i, buf[i]);	
#endif
		}

		CAN1_DLC = (kal_uint8)(buf[5] & 0x0F);
		for (j = 0; i < (RXBDAT_OFF + CAN1_DLC); i++, j++){
			CAN1_DATA[j] = MCP2515_byte_read1(RXBCTRL(0) + i);
#if CAN_DEBUG				
			kal_prompt_trace(MOD_USB, "can_hw1_rx DATA[%d] = %x", j, CAN1_DATA[j]);
#endif
		}

	       CAN1_SID = (unsigned int)(buf[1]<<3);
		CAN1_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);

		CAN1_EID = (kal_uint32)((buf[2] & 0x03)<<16);
		CAN1_EID |= (kal_uint32)(buf[3]<<8);
		CAN1_EID |= (kal_uint32)(buf[4]);
		
		CAN1_DLC = (kal_uint8)(buf[5] & 0x0F);
		rec_id = CAN1_EID & 0x3ffff;
		rec_id |= CAN1_SID << 18;
		
		can_unit_fill(rec_id, CAN1_DATA);	
		kal_prompt_trace(MOD_USB, "rec_id = %x, CAN1_DLC = %x, data:%x %x %x %x %x %x %x %x", rec_id, CAN1_DLC, CAN1_DATA[0], CAN1_DATA[1], CAN1_DATA[2], CAN1_DATA[3], CAN1_DATA[4], CAN1_DATA[5], CAN1_DATA[6], CAN1_DATA[7]);
		MCP2515_byte_write1(CANINTF, 0x00);		
	}

	/* receive buffer 1 */
	if (intf & CANINTF_RX1IF) {
#if CAN_DEBUG			
		kal_prompt_trace(MOD_USB, "can_hw1_rx receive buffer 1");
#endif
		for (i = 1; i < RXBDAT_OFF; i++)
			buf[i] = MCP2515_byte_read1(RXBCTRL(1) + i);

		for (; i < (RXBDAT_OFF + 8); i++)
			buf[i] = MCP2515_byte_read1(RXBCTRL(1) + i);
	       CAN1_SID = (unsigned int)(buf[1]<<3);
		CAN1_SID  |=  (unsigned int)((buf[2] & 0xE0)>>5);

		CAN1_EID = (kal_uint32)((buf[2] & 0x03)<<16);
		CAN1_EID |= (kal_uint32)(buf[3]<<8);
		CAN1_EID |= (kal_uint32)(buf[4]);
		
		CAN1_DLC = (kal_uint8)(buf[5] & 0x0F);
		rec_id = CAN1_EID & 0x3ffff;
		rec_id |= CAN1_SID << 18;
		
		can_unit_fill(rec_id, CAN1_DATA);	
		kal_prompt_trace(MOD_SOC, "rec_id = %x, CAN1_DLC = %x, data:%x %x %x %x %x", rec_id, CAN1_DLC, CAN1_DATA[0], CAN1_DATA[1], CAN1_DATA[2], CAN1_DATA[3], CAN1_DATA[4]);
		MCP2515_byte_write1(CANINTF, 0x00);		
	}	
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
		/* complete send buffer 1 */
	if (intf & CANINTF_TX0IF) {
		//can1_send_data();
		MCP2515_byte_write1(CANINTF, 0x00);	
		//kal_sleep_task(1);
		Delayus(1000);
		can1_send_data();
	}
#endif	
}

void can2_eint_hisr(void)
{	
	ilm_struct       *aux_ilm = NULL;
 
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, MSG_ID_CAN2_HW_RX, NULL); 
	msg_send_ext_queue(aux_ilm);  
	kal_prompt_trace(MOD_USB, "can2_eint_hisr");	
}



#include "gpio_var.c"
void can_int_register()
{
	kal_prompt_trace(MOD_USB, "can_int_register");	
	GPIO_ModeSetup(CAN1_EINT_GPIO_NUM, 0);
	GPIO_ModeSetup(CAN2_EINT_GPIO_NUM, 0);	
	GPIO_ModeSetup(CAN_SIMULATE_EINT_GPIO_NUM, 0);	
	
	EINT_Registration(CAN1_EINT_NO,KAL_FALSE, can1_eint_state,can1_eint_hisr, KAL_TRUE);
	EINT_Set_Sensitivity(CAN1_EINT_NO, EDGE_SENSITIVE);
	EINT_Set_Polarity(CAN1_EINT_NO, can1_eint_state);

	EINT_Registration(CAN2_EINT_NO,KAL_FALSE, can2_eint_state,can2_eint_hisr, KAL_TRUE);
	EINT_Set_Sensitivity(CAN2_EINT_NO, EDGE_SENSITIVE);
	EINT_Set_Polarity(CAN2_EINT_NO, can2_eint_state);
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
	EINT_Registration(PUSH_EINT_NO,KAL_FALSE, can_send_eint_state,can_simulate_eint_hisr, KAL_TRUE);
	EINT_Set_Polarity(PUSH_EINT_NO, can_send_eint_state);
	EINT_SW_Debounce_Modify(PUSH_EINT_NO,20);

	GPIO_ModeSetup(CAN_SIMULATE_EINT_GPIO_NUM, 1);	
#endif	
	GPIO_ModeSetup(CAN1_EINT_GPIO_NUM, 2);
	GPIO_ModeSetup(CAN2_EINT_GPIO_NUM, 2);	
}

void can_start(void)
{
	/*disable can1 spi cs*/
	GPIO_ModeSetup(30, 0);	
	GPIO_InitIO(1, 30);
	GPIO_WriteIO(1, 30);	
	/*Enable stb2*/
	GPIO_ModeSetup(CAN_STBY_PORT2, 0);	
	GPIO_InitIO(1, CAN_STBY_PORT2);
	GPIO_WriteIO(0, CAN_STBY_PORT2);		
	/*Enable stb1*/
	GPIO_ModeSetup(CAN_STBY_PORT1, 0);	
	GPIO_InitIO(1, CAN_STBY_PORT1);
	GPIO_WriteIO(0, CAN_STBY_PORT1);
	
	can_table_create();
	
	mcp2515_init_gpio_spi();
	can_int_register();
}
#ifdef BIRD_CAN_SIMULATE_SEND_SUPPORT
void can_simulate_eint_hisr(void)
{	
	kal_uint8  data = 0;
	if (can_send_eint_state == KAL_TRUE)
	{
		can_send_eint_state = (kal_bool)!can_send_eint_state;
		EINT_Set_Polarity(PUSH_EINT_NO, can_send_eint_state);
		kal_prompt_trace(MOD_USB, "can_simulate_eint_hisr 0");	
		can_normal_data = KAL_TRUE;
	}
	else
	{
		can_send_eint_state = (kal_bool)!can_send_eint_state;
		EINT_Set_Polarity(PUSH_EINT_NO, can_send_eint_state);       
		kal_prompt_trace(MOD_USB, "can_simulate_eint_hisr 1");	 
		can_normal_data = KAL_FALSE;
	}	
}
void can1_send_data(void)
{
	kal_uint8  data = 0, id0, id1, id2,id3,dlc = 0;
	kal_uint32 send_id1 = 0;
	kal_uint8 send_data1[8] = {0};	
	unsigned char HSID,LSID,EID8,EID0;
	static kal_uint8 count = 0;
	char gpio_data = 0;

       if (can_normal_data == KAL_FALSE)
       {
		send_id1 = can_send_data[count].can_id;
		dlc = can_send_data[count].can_dlc;
		memcpy(send_data1, can_send_data[count].can_data, 8);
       }
	else
	{
		send_id1 = can_send_data_normal[count].can_id;
		dlc = can_send_data_normal[count].can_dlc;
		memcpy(send_data1, can_send_data_normal[count].can_data, 8);		
	}

	kal_prompt_trace(MOD_USB, "can1_send_data  send_id1= %x, dlc = %d, send_data1 = %x %x %x %x %x %x %x %x", send_id1, dlc, send_data1[0], 
		send_data1[1], send_data1[2], send_data1[3], send_data1[4], send_data1[5], send_data1[6], send_data1[7]);
	count++;
	if(count >= CAN_DATA_NUM)
		count = 0;
	id0 = send_id1 >>24;
	id1 = send_id1 >>16;
	id2 = send_id1 >>8;
	id3 = send_id1;

	HSID = (id0<<3) + (id1>>5);
	LSID= ((id1<<3)&0xe0)+(id1 & 0x03)+0x08;
	EID8 = id2;
	EID0 = id3;	
	
	MCP2515_byte_write1(TXB0SIDH,HSID);   // SID10--SID3  
	MCP2515_byte_write1(TXB0SIDL,LSID);   //SID2--SID0  EID17--SID16 
	MCP2515_byte_write1(TXB0EID8,EID8);   // EID15--SID8  
	MCP2515_byte_write1(TXB0EID0,EID0);   //EID7--EID0  
	
	MCP2515_byte_write1(TXB0DLC, 8);     
	MCP2515_byte_write1(TXB0D0,send_data1[0]);  
	MCP2515_byte_write1(TXB0D1,send_data1[1]);   
	MCP2515_byte_write1(TXB0D2,send_data1[2]);   
	MCP2515_byte_write1(TXB0D3,send_data1[3]);   
	MCP2515_byte_write1(TXB0D4,send_data1[4]);   
	MCP2515_byte_write1(TXB0D5,send_data1[5]);   
	MCP2515_byte_write1(TXB0D6,send_data1[6]);   
	MCP2515_byte_write1(TXB0D7,send_data1[7]);  	
	MCP2515_can1_rts_tx0();
}

void can2_send_data(void)
{
	kal_uint8  data = 0, id0, id1, id2, id3, dlc = 0;
	kal_uint32 send_id = 0;
	kal_uint8 send_data[8] = {0};	
	unsigned char HSID,LSID,EID8,EID0;
	static kal_uint8 count = 0;

	return;

	send_id = can_send_data[count].can_id;
	dlc = can_send_data[count].can_dlc;
	memcpy(send_data, can_send_data[count].can_data, 8);
	count++;
	if(count >= 14)
		count = 0;	

	id0 = send_id >>24;
	id1 = send_id >>16;
	id2 = send_id >>8;
	id3 = send_id;

	HSID = id0<<3 + id1>>5;
	LSID= ((id1<<3)&0xe0)+(id1 & 0x03)+0x08;
	EID8 = id2;
	EID0 = id3;
	
	MCP2515_byte_write2(TXB0SIDH,HSID);   // SID10--SID3  
	MCP2515_byte_write2(TXB0SIDL,LSID);   //SID2--SID0  EID17--SID16 
	MCP2515_byte_write2(TXB0EID8,EID8);   // EID15--SID8  
	MCP2515_byte_write2(TXB0EID0,EID0);   //EID7--EID0  
	
	MCP2515_byte_write2(TXB0DLC, 8);     //  
	MCP2515_byte_write2(TXB0D0,send_data[0]);   
	MCP2515_byte_write2(TXB0D1,send_data[1]);  
	MCP2515_byte_write2(TXB0D2,send_data[2]);  
	MCP2515_byte_write2(TXB0D3,send_data[3]);   
	MCP2515_byte_write2(TXB0D4,send_data[4]);  
	MCP2515_byte_write2(TXB0D5,send_data[5]);   
	MCP2515_byte_write2(TXB0D6,send_data[6]);  
	MCP2515_byte_write2(TXB0D7,send_data[7]);  	

	MCP2515_can2_rts_tx0();
}
#endif
#endif
