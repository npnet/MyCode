/*============================================================
        Headfile about MCP2515   V1.00
==============================================================
Chip:      MCP2515
Function:    The controller of CAN-BUS
Writer:     lqy
Data:      2014-09-18
Reference:   "mcp2515.h" of Fabian Greif
=============================================================*/
#ifndef _MCP2515_H_
#define _MCP2515_H_
//********************* 
//* 用户类型定义区    * 
//*********************
//********************* 
//* 头 文 件 配 置 区 * 
//*********************

//******************** 
//*  系 统 宏 定 义  * 
//******************** 
//#ifndef U8
//   #define U8 unsigned char
//#endif

#include "MMIDataType.h"

// 定义寄存器地址
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define BFPCTRL  0x0C
#define TXRTSCTRL 0x0D
#define CANSTAT  0x0E
#define CANCTRL  0x0F
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B
#define TEC    0x1C
#define REC       0x1D
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EID8 0x22
#define RXM0EID0 0x23
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM1EID8 0x26
#define RXM1EID0 0x27
#define CNF3   0x28
#define CNF2   0x29
#define CNF1   0x2A
#define CANINTE  0x2B
#define CANINTF  0x2C
#define EFLG   0x2D
#define TXB0CTRL 0x30
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define TXB0EID8 0x33
#define TXB0EID0 0x34
#define TXB0DLC  0x35
#define TXB0D0  0x36
#define TXB0D1  0x37
#define TXB0D2  0x38
#define TXB0D3  0x39
#define TXB0D4  0x3A
#define TXB0D5  0x3B
#define TXB0D6  0x3C
#define TXB0D7  0x3D
#define TXB1CTRL 0x40
#define TXB1SIDH 0x41
#define TXB1SIDL 0x42
#define TXB1EID8 0x43
#define TXB1EID0 0x44
#define TXB1DLC  0x45
#define TXB1D0  0x46
#define TXB1D1  0x47
#define TXB1D2  0x48
#define TXB1D3  0x49
#define TXB1D4  0x4A
#define TXB1D5  0x4B
#define TXB1D6  0x4C
#define TXB1D7  0x4D
#define TXB2CTRL 0x50
#define TXB2SIDH 0x51
#define TXB2SIDL 0x52
#define TXB2EID8 0x53
#define TXB2EID0 0x54
#define TXB2DLC  0x55
#define TXB2D0  0x56
#define TXB2D1  0x57
#define TXB2D2  0x58
#define TXB2D3  0x59
#define TXB2D4  0x5A
#define TXB2D5  0x5B
#define TXB2D6  0x5C
#define TXB2D7  0x5D
#define RXB0CTRL 0x60
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC  0x65
#define RXB0D0  0x66
#define RXB0D1  0x67
#define RXB0D2  0x68
#define RXB0D3  0x69
#define RXB0D4  0x6A
#define RXB0D5  0x6B
#define RXB0D6  0x6C
#define RXB0D7  0x6D
#define RXB1CTRL 0x70
#define RXB1SIDH 0x71
#define RXB1SIDL 0x72
#define RXB1EID8 0x73
#define RXB1EID0 0x74
#define RXB1DLC  0x75
#define RXB1D0  0x76
#define RXB1D1  0x77
#define RXB1D2  0x78
#define RXB1D3  0x79
#define RXB1D4  0x7A
#define RXB1D5  0x7B
#define RXB1D6  0x7C
#define RXB1D7  0x7D
//定义寄存器BFPCTRL位信息
#define B1BFS  5
#define B0BFS  4
#define B1BFE  3
#define B0BFE  2
#define B1BFM  1
#define B0BFM  0
//定义寄存器TXRTSCTRL位信息
#define B2RTS  5
#define B1RTS  4
#define B0RTS  3
#define B2RTSM  2
#define B1RTSM  1
#define B0RTSM  0
//定义寄存器CANSTAT位信息
#define OPMOD2  7
#define OPMOD1  6
#define OPMOD0  5
#define ICOD2  3
#define ICOD1  2
#define ICOD0  1

//定义寄存器CANCTRL位信息
#define REQOP2  7
#define REQOP1  6
#define REQOP0  5
#define ABAT  4
#define CLKEN  2
#define CLKPRE1  1
#define CLKPRE0  0

 //定义寄存器CNF3位信息
#define WAKFIL  6
#define PHSEG22  2
#define PHSEG21  1
#define PHSEG20  0

 //定义寄存器CNF2位信息
#define BTLMODE  7
#define SAM   6
#define PHSEG12  5
#define PHSEG11  4
#define PHSEG10  3
#define PHSEG2  2
#define PHSEG1  1
#define PHSEG0  0

 //定义寄存器CNF1位信息
#define SJW1  7
#define SJW0  6
#define BRP5  5
#define BRP4  4
#define BRP3  3
#define BRP2  2
#define BRP1  1
#define BRP0  0

 //定义寄存器CANINTE位信息
#define MERRE  7
#define WAKIE  6
#define ERRIE  5
#define TX2IE  4
#define TX1IE  3
#define TX0IE  2
#define RX1IE  1
#define RX0IE  0

 //定义寄存器CANINTF位信息
#define MERRF  7
#define WAKIF  6
#define ERRIF  5
#define TX2IF  4
#define TX1IF  3
#define TX0IF  2
#define RX1IF  1
#define RX0IF  0

 //定义寄存器EFLG位信息
#define RX1OVR  7
#define RX0OVR  6
#define TXB0  5
#define TXEP  4
#define RXEP  3
#define TXWAR  2
#define RXWAR  1
#define EWARN  0

 //定义寄存器TXBnCTRL ( n = 0, 1, 2 )位信息
#define ABTF  6
#define MLOA  5
#define TXERR  4
#define TXREQ  3
#define TXP1  1
#define TXP0  0

 //定义寄存器RXB0CTRL位信息
#define RXM1  6
#define RXM0  5
#define RXRTR  3
#define BUKT  2
#define BUKT1  1
#define FILHIT0  0
//定义发送缓冲寄存器 TXBnSIDL ( n = 0, 1 )的位信息
#define EXIDE  3
//定义接受缓冲器1控制寄存器的位信息
#define FILHIT2  2
#define FILHIT1  1
/**
 * 定义接收缓冲器n标准标示符低位 RXBnSIDL ( n = 0, 1 )的位信息
 */
#define SRR   4
#define IDE   3

// 定义接收缓冲器n数据长度码 RXBnDLC ( n = 0, 1 )的位信息
#define RTR   6
#define DLC3  3
#define DLC2  2
#define DLC1  1
#define DLC0  0
 //定义SPI控制命令字
#define SPI_RESET    0xC0
#define SPI_READ    0x03
#define SPI_READ_RX   0x90
#define SPI_WRITE    0x02
#define SPI_WRITE_TX  0x40
#define SPI_RTS     0x80
#define SPI_RTS_TXB0        0x81
#define SPI_RTS_TXB1        0x82
#define SPI_RTS_TXB2        0x84
#define SPI_READ_STATUS 0xA0
#define SPI_RX_STATUS  0xB0
#define SPI_BIT_MODIFY 0x05

//CANCTRL  by hgx
#define REQOP_CONFIG        0x80
#define REQOP_LISTEN        0x60
#define REQOP_LOOPBACK      0x40
#define REQOP_SLEEP         0x20
#define REQOP_NORMAL        0x00

#define ABORT               0x10

#define CLKOUT_ENABLED      0x04
#define CLKOUT_DISABLED     0x00
#define CLKOUT_PRE_8        0x03
#define CLKOUT_PRE_4        0x02
#define CLKOUT_PRE_2        0x01
#define CLKOUT_PRE_1        0x00
//  by hgx

#define CAN_RST_PORT  19
#define CAN_STBY_PORT1  19
#define CAN_STBY_PORT2  3

#define SPI_CS_PORT1     30
#define SPI_CS_PORT2     26
#define SPI_SCK_PORT   27
#define SPI_MOSI_PORT 28
#define SPI_MISO_PORT 29

//********************* 
//*   常 数 宏 定 义  * 
//*********************
//********************** 
//*   动 作 宏 定 义   * 
//**********************
//********************** 
//*   全局变量声明区   * 
//**********************
//********************** 
//*   函 数 声 明 区   * 
//**********************


extern U8 SPI_MasterTransmit(U8 data);

//**********************************************************// 
//   函数说明：MCP2515初始化程序                            // 
//   输入：    无                                   // 
//   输出：    无                                          // 
//   调用函数：                                  //
//**********************************************************//
void mcp2515_init(void);

void mcp2515_init_gpio_spi(void);
//**********************************************************// 
//   函数说明：MCP2515写控制寄存器程序                      // 
//   输入：    寄存器地址，写入数据                         // 
//   输出：    无                                          // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_write_register(U8 adress, U8 data);
//**********************************************************// 
//   函数说明：MCP2515读控制寄存器程序                      // 
//   输入：    寄存器地址，                             // 
//   输出：    寄存器数据                                  // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
U8 mcp2515_read_register(U8 adress);
//**********************************************************// 
//   函数说明：MCP2515控制寄存器位修改程序                // 
//   输入：    寄存器地址，修改位，修改数据                // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_bit_modify(U8 adress, U8 mask, U8 data);
//**********************************************************// 
//   函数说明：对MCP2515连续寄存器进行连续写操作            // 
//   输入：    连续寄存器起始地址，数据指针，数据长度      // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_write_register_p(U8 adress, U8 *data, U8 length);
//**********************************************************// 
//   函数说明：对MCP2515连续寄存器进行连续读操作            // 
//   输入：    连续寄存器起始地址，数据指针，数据长度      // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_read_register_p(U8 adress, U8 *data, U8 length);

void mcp2515_close(void); 

//from hgx  for spi Simulation
void spi_send(U8 dat);
U8 spi_rcv();
U8 MCP2515_byte_read(U8 adr);
void MCP2515_byte_write(U8 adr,U8 dat);
//from hgx  for spi Simulation
void MCP2515_can1_rts_tx0(void);
void MCP2515_can2_rts_tx0(void);
#endif // _MCP2515_H_

