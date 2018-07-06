/*============================================================
        C file about MCP2515   V1.00
==============================================================
Chip:      MCP2515
Function:    The controller of CAN-BUS
Writer:     lqy
Data:      2014.09.18
Reference:   "mcp2515.c" of Fabian Greif
=============================================================*/
//********************* 
//* 头 文 件 配 置 区 * 
//*********************
#include "mcp2515.h"
#include "dcl.h"
#include "spi_hal.h"


// 变量定义区
static SPI_HANDLE handle;
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8  tempdata;	
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8  readdata;	
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8 temp[4] = { 0, 0, 0, 0 };	
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8 temp3[3] = { 0, 0, 0 };	
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8 temp2[2] = { 0, 0};	
__attribute__ ((section ("NONCACHEDRW"), aligned(4))) 	
    U8 pdata[40] = { 0 };	
//********************** 
//*   函 数 声 明 区   * 
//**********************
/*
U8 SPI_MasterTransmit(U8 data);
void mcp2515_init(void);
void mcp2515_write_register(U8 adress, U8 data);
U8 mcp2515_read_register(U8 adress);
void mcp2515_bit_modify(U8 adress, U8 mask, U8 data);
void mcp2515_write_register_p(U8 adress, U8 *data, U8 length);
void mcp2515_close(void); 
*/

//********************** 
//*   函 数 定 义 区   * 
//**********************

U8 SPI_MasterTransmit(U8 data)
{
    SPI_RESULT status;

    U8 retdata = NULL;
    tempdata = data;

        // CS low ,MCP2515 enable
        //GPIO_WriteIO(0,SPI_CS_PORT);	//默认低电平// by lqy
       status = spi_write(handle, &tempdata, 1, 1, NULL);
	 if(status !=  SPI_RESULT_OK)
        {
             kal_prompt_trace(MOD_SOC, "SPI_MasterTransmit write fail");
        }
        else 
        {
               kal_prompt_trace(MOD_SOC, "SPI_MasterTransmit write ok data=%x",data);
		kal_prompt_trace(MOD_SOC, "SPI_MasterTransmit write tempdata=%x",tempdata);	   
        }	      
	//CS high ,MCP2515 disable
      //GPIO_WriteIO(1,SPI_CS_PORT);	//默认高电平// by lqy	
    return retdata;	
}

//**********************************************************// 
//   函数说明：MCP2515写控制寄存器程序                      // 
//   输入：    寄存器地址，写入数据                         // 
//   输出：    无                                          // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_write_register(U8 adress, U8 data)
{
 SPI_RESULT status;
 // CS low ,MCP2515 enable
 //GPIO_WriteIO(0,SPI_CS_PORT);	//默认低电平// by lqy
/*
 SPI_MasterTransmit(SPI_WRITE); // 发送SPI写寄存器控制字 
 
 SPI_MasterTransmit(adress);  //发送寄存器地址
 
 SPI_MasterTransmit(data);   //发送寄存器数据
*/
 temp3[0] = SPI_WRITE;
 temp3[1] = adress;
 temp3[2] = data;
 status = spi_write(handle, &temp3, 3, 1, NULL);

 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_write_register fail");
  }
  else 
  {
     //kal_prompt_trace(MOD_SOC, "mcp2515_write_register ok");
     //kal_prompt_trace(MOD_SOC, "temp3[0],[1],[2]=%x,%x,%x",temp3[0],temp3[1],temp3[2]);
   }	   
 //CS high ,MCP2515 disable
 //GPIO_WriteIO(1,SPI_CS_PORT);	//默认高电平// by lqy
}


//**********************************************************// 
//   函数说明：MCP2515读控制寄存器程序                      // 
//   输入：    寄存器地址，                             // 
//   输出：    寄存器数据                                  // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
U8 mcp2515_read_register(U8 adress)
{
 SPI_RESULT status;
 
 // CS low ,MCP2515 enable
// GPIO_WriteIO(0,26);	//默认低电平// by lqy
 /*
 SPI_MasterTransmit(SPI_READ); // 发送SPI写寄存器控制字
 
 SPI_MasterTransmit(adress); //发送寄存器地址
 
 data = SPI_MasterTransmit(0xff); //回读寄存器数据
*/
 temp3[0] = SPI_READ;
 temp3[1] = adress;
 temp3[2] = 0xff;
 
 status = spi_readwrite(handle, &temp3, &temp3, 3, 1, NULL);

 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_read_register spi_readwrite fail = %d",status);
  }
  else 
  {
     //kal_prompt_trace(MOD_SOC, "mcp2515_read_register spi_readwrite ok");
     //kal_prompt_trace(MOD_SOC, "temp2[0],[1],readdata=%x,%x,%x",temp3[0],temp3[1],temp3[2]);	 
   }	   
/* status = spi_read(handle, &data, 1, 1, NULL);

 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_read_register spi_read fail");
  }
  else 
  {
     kal_prompt_trace(MOD_SOC, "mcp2515_read_register spi_read ok");
     kal_prompt_trace(MOD_SOC, "data=%x",data);	 
   }*/	   
 //CS high ,MCP2515 disable
 //GPIO_WriteIO(1,26);	//默认高电平// by lqy
 
 return temp3[2];
}
//**********************************************************// 
//   函数说明：读MCP2515接收缓冲器程序                      // 
//   输入：    缓冲器地址，                             // 
//   输出：    缓冲器数据                                  // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
U8 mcp2515_read_rx_buffer(U8 adress)
{
 U8 data;
 
 // 判断adress是否有效，除了1，2位，其余都应为0
 if (adress & 0xF9)
  return 0;
 
 // CS low ,MCP2515 enable
//GPIO_WriteIO(0,26);	//默认低电平// by lqy
 
 SPI_MasterTransmit(SPI_READ_RX | adress); //发送读取控制字

 //CS high ,MCP2515 disable
 //GPIO_WriteIO(1,26);	//默认高电平// by lqy

 data = SPI_MasterTransmit(0xff); //读回数据
 
//CS high ,MCP2515 disable
 //GPIO_WriteIO(1,26);	//默认高电平// by lqy
 
 return data;
}
//**********************************************************// 
//   函数说明：MCP2515控制寄存器位修改程序                // 
//   输入：    寄存器地址，修改位，修改数据                // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_bit_modify(U8 adress, U8 mask, U8 data)
{
 // CS low ,MCP2515 enable
 SPI_RESULT status;
 
 //GPIO_WriteIO(0,26);	//默认低电平// by lqy
 /*
 SPI_MasterTransmit(SPI_BIT_MODIFY); //SPI位修改指令
 
 SPI_MasterTransmit(adress);    //发送寄存器地址
 
 SPI_MasterTransmit(mask);     //发送屏蔽字节，
                  //屏蔽字节中"1"表示允许对相应位修改，"0"表示禁止修改
 SPI_MasterTransmit(data);     //发送数据字节
 */
  temp[0] = SPI_BIT_MODIFY;
 temp[1] = adress;
 temp[2] = mask;
 temp[3] = data;
 status = spi_write(handle, &temp, 4, 1, NULL);

 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_bit_modify fail");
  }
  else 
  {
     //kal_prompt_trace(MOD_SOC, "mcp2515_bit_modify ok");
     //kal_prompt_trace(MOD_SOC, "temp[0],[1],[2],[3]=%d,%d,%d,%d",temp[0],temp[1],temp[2],temp[3]);	 
   }	   
//CS high ,MCP2515 disable
// GPIO_WriteIO(1,26);	//默认高电平// by lqy
}
//**********************************************************// 
//   函数说明：对MCP2515连续寄存器进行连续写操作            // 
//   输入：    连续寄存器起始地址，数据指针，数据长度      // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_write_register_p(U8 adress, U8 *data, U8 length)
{
 U8 i;
 SPI_RESULT status;
 
 // CS low ,MCP2515 enable
 GPIO_WriteIO(0,26);	//默认低电平// by lqy
 /*
 SPI_MasterTransmit(SPI_WRITE);  //发送SPI写指令
 
 SPI_MasterTransmit(adress);    //发送起始寄存器地址
 
 for (i=0; i<length ;i++ )
  SPI_MasterTransmit(*data++);  //发送数据
 */

 pdata[0] = SPI_WRITE;
 pdata[1] = adress;
 for (i=0; i<length ;i++ )
 	pdata[i+2] = *data++;
 status = spi_write(handle, pdata, length+2, 1, NULL);

 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_write_register_p fail");
  }
  else 
  {
     kal_prompt_trace(MOD_SOC, "mcp2515_write_register_p ok");
     kal_prompt_trace(MOD_SOC, "pdata[0],[1],[2],[3],[4],[5] = %x,%x,%x,%x,%x,%x",pdata[0],pdata[1],pdata[2],pdata[3],pdata[4],pdata[5]);	 
   }	   
 //CS high ,MCP2515 disable
 GPIO_WriteIO(1,26);	//默认高电平// by lqy
}
//**********************************************************// 
//   函数说明：对MCP2515连续寄存器进行连续读操作            // 
//   输入：    连续寄存器起始地址，数据指针，数据长度      // 
//   输出：    无                                      // 
//   调用函数：SPI发送程序SPI_MasterTransmit                //
//**********************************************************//
void mcp2515_read_register_p( U8 adress, U8 *data, U8 length )
{
 U8 i;
 SPI_RESULT status;
 
 // CS low ,MCP2515 enable
GPIO_WriteIO(0,26);	//默认低电平// by lqy
 /*
 SPI_MasterTransmit(SPI_READ);  //发送SPI读指令
 
 SPI_MasterTransmit(adress);   //发送起始寄存器地址
 
 for (i=0; i<length ;i++ )
  *data++ = SPI_MasterTransmit(0xff);  //数据保存
 */
 pdata[0] = SPI_READ;
 pdata[1] = adress;
 pdata[2] = 0xff;
 status = spi_readwrite(handle, pdata, pdata, 40, 1, NULL);
 data = pdata;
 if(status !=  SPI_RESULT_OK)
 {
      kal_prompt_trace(MOD_SOC, "mcp2515_read_register_p fail");
  }
  else 
  {
     kal_prompt_trace(MOD_SOC, "mcp2515_read_register_p ok");
      kal_prompt_trace(MOD_SOC, "pdata = %s",pdata);	 	 
   }	   
 //CS high ,MCP2515 disable
 GPIO_WriteIO(1,26);	//默认高电平// by lqy
}

void mcp2515_close(void)
{
   spi_close(handle);
}
	


//from hgx  for spi Simulation
void spi_send(U8 dat)
{
	U8 i;

	for(i=0;i<8;i++)
	{
		GPIO_WriteIO(0,SPI_SCK_PORT); //SCK=0;
		if(dat&0x80) GPIO_WriteIO(1,SPI_MOSI_PORT);//SO=1;//for 2515 SO=1 MOSI mater out slave in
			else GPIO_WriteIO(0,SPI_MOSI_PORT);//SO=0; // for 2515 SO=0
		dat=dat<<1;
		GPIO_WriteIO(1,SPI_SCK_PORT);//SCK=1;	//上升沿送入数据
		//kal_prompt_trace( MOD_SOC,"SCK1 = %d",GPIO_ReadIO(SPI_SCK_PORT));
	}
	GPIO_WriteIO(0,SPI_MOSI_PORT);//SO=0;
	//kal_prompt_trace( MOD_SOC,"SO0 = %d",GPIO_ReadIO(SPI_MOSI_PORT));
	GPIO_WriteIO(0,SPI_SCK_PORT); //SCK=0;
	//kal_prompt_trace( MOD_SOC,"SCK0 = %d",GPIO_ReadIO(SPI_SCK_PORT));
}

U8 spi_rcv()
{
	U8 i,j;
	U8 SI;

	j=0;
	for(i=0;i<8;i++)
	{
		GPIO_WriteIO(0,SPI_SCK_PORT); //SCK=0; //SCK=0;
		j=j<<1;
		SI=GPIO_ReadIO(SPI_MISO_PORT);
		if(SI) j|=0x01;	//数据在下降沿送出
		GPIO_WriteIO(1,SPI_SCK_PORT);//SCK=1;	
	}
	GPIO_WriteIO(0,SPI_SCK_PORT); //SCK=0; //SCK=0;
	return j;
}

U8 MCP2515_byte_read1(U8 adr)
{
	U8 temp;

	GPIO_WriteIO(0,SPI_CS_PORT1);	//CS=0;
	spi_send(SPI_READ);
	spi_send(adr);
	temp=spi_rcv();
	GPIO_WriteIO(1,SPI_CS_PORT1);	//CS=1;

	return temp;
}

void MCP2515_byte_write1(U8 adr,U8 dat)
{
	GPIO_WriteIO(0,SPI_CS_PORT1);	//CS=0;
	spi_send(SPI_WRITE);
	spi_send(adr);	//清零所有中断标志
	spi_send(dat);
	GPIO_WriteIO(1,SPI_CS_PORT1);	//CS=1;
	
}

void MCP2515_can1_rts_tx0(void)
{
	GPIO_WriteIO(0,SPI_CS_PORT1);	//CS=0;
	spi_send(SPI_RTS_TXB0);
	GPIO_WriteIO(1,SPI_CS_PORT1);	//CS=1;	
}

U8 MCP2515_byte_read2(U8 adr)
{
	U8 temp;

	GPIO_WriteIO(0,SPI_CS_PORT2);	//CS=0;
	spi_send(SPI_READ);
	spi_send(adr);
	temp=spi_rcv();
	GPIO_WriteIO(1,SPI_CS_PORT2);	//CS=1;

	return temp;
}

void MCP2515_byte_write2(U8 adr,U8 dat)
{
	GPIO_WriteIO(0,SPI_CS_PORT2);	//CS=0;
	spi_send(SPI_WRITE);
	spi_send(adr);	//清零所有中断标志
	spi_send(dat);
	GPIO_WriteIO(1,SPI_CS_PORT2);	//CS=1;
}
void MCP2515_can2_rts_tx0(void)
{
	GPIO_WriteIO(0,SPI_CS_PORT2);	//CS=0;
	spi_send(SPI_RTS_TXB0);
	GPIO_WriteIO(1,SPI_CS_PORT2);	//CS=1;	
}
//from hgx  for spi Simulation

//**********************************************************// 
//   函数说明：MCP2515初始化程序                            // 
//   输入：    无                                   // 
//   输出：    无                                          // 
//   调用函数：                                  //
//**********************************************************//
#if 0
void mcp2515_init(void)
{
 //初始化MCU的SPI总线
 
 // MCP2515 启动前进行软件复位

 //使用位修改指令将MCP2515设置为配置模式
 //也就是将CANCTRL寄存器的REQOP[2:0]设置为100
    SPI_CONFIG_PARAM_T param;
    SPI_RESULT status;
    unsigned char i, data = 0;		

    kal_prompt_trace(MOD_SOC, "enter mcp2515_init ");
    handle = spi_open(0);
    param.cs_setup_time = 10;
    param.cs_hold_time = 10;
    param.cs_idle_time = 10;
    param.clk_low_time = 10;
    param.clk_high_time = 10;
    param.tx_msbf = SPI_MSBF_MSB; //from SPI_MSBF_LSB->SPI_MSBF_MSB lqy can must first high bit data,then low bit data;
    param.rx_msbf = SPI_MSBF_MSB;//from SPI_MSBF_LSB->SPI_MSBF_MSB lqy ;
    param.tx_endian = SPI_ENDIAN_LITTLE;
    param.rx_endian = SPI_ENDIAN_LITTLE;
    param.clk_polarity = SPI_CPOL_B0;
    param.clk_fmt = SPI_CPHA_B0;
    status = spi_configure(handle, &param);
    if(status !=  SPI_RESULT_OK)
    {
         kal_prompt_trace(MOD_SOC, "spi config fail");
    }
    else 
   {
         kal_prompt_trace(MOD_SOC, "spi config ok");
    }
  
   
 //初始化MCU的SPI总线
 //SPI_MasterInit();
 
 // MCP2515 启动前进行软件复位
/*
 GPIO_WriteIO(1,CAN_RST_PORT);	//by lqy //CAN_RST的电平拉高
 GPIO_WriteIO(0,CAN_STBY_PORT);	//by lqy //CAN_STBY的电平拉低
 GPIO_InitIO(OUTPUT,SPI_CS_PORT);         //out put  // by lqy ;
 //GPIO_WriteIO(0,26);	//by lqy //MCP2515的CS有效

 
 SPI_MasterTransmit( SPI_RESET );
 //GPIO_WriteIO(1,26);	//默认高电平// by lqy//MCP2515的CS无效
 
 //使用位修改指令将MCP2515设置为配置模式
 //也就是将CANCTRL寄存器的REQOP[2:0]设置为100
 mcp2515_bit_modify( CANCTRL, 0xE0, (1<<REQOP2) );
 */

 

 //计算并设置MCP2515的位时间
 
 // 时钟频率：Fosc  = 16MHz
 // 分频控制器 CNF1.BRP[5:0] = 7
 // 最小时间份额 TQ = 2 * ( BRP + 1 ) / Fosc   = 2*(7+1)/16M = 1uS
 // 同步段 Sync Seg   = 1TQ
 // 传播段 Prop Seg   = ( PRSEG + 1 ) * TQ  = 1 TQ
 // 相位缓冲段 Phase Seg1 = ( PHSEG1 + 1 ) * TQ = 3 TQ
 // 相位缓冲段 Phase Seg2 = ( PHSEG2 + 1 ) * TQ = 3 TQ
 // 同步跳转长度设置为 CNF1.SJW[1:0] = 00, 即 1TQ
 // 总线波特率 NBR = Fbit =  1/(sync seg + Prop seg + PS1 + PS2 )
 //                       = 1/(8TQ) = 1/8uS = 125kHz
 /*
 //设置分频控制器CNF1.BRP[5:0] = 7，同步跳转长度设置为 CNF1.SJW[1:0] = 00
 mcp2515_write_register( CNF1, (1<<BRP0)|(1<<BRP1)|(1<<BRP2) );
 // 设置传播段 Prop Seg 为00，即1TQ，相位缓冲段 Phase Seg1的长度3TQ
 mcp2515_write_register( CNF2, (1<<BTLMODE)|(1<<PHSEG11) );
 // 设置 相位缓冲段 Phase Seg2为 3TQ ， 禁用唤醒滤波器
 mcp2515_write_register( CNF3, (1<<PHSEG21) );



 //设置分频控制器CNF1.BRP[5:0] = 7，同步跳转长度设置为 CNF1.SJW[1:0] = 00
 mcp2515_write_register( CNF1, (1<<BRP0)|(1<<BRP1)|(1<<BRP2) );
 // 设置传播段 Prop Seg 为00，即1TQ，相位缓冲段 Phase Seg1的长度3TQ
 mcp2515_write_register( CNF2, (1<<BTLMODE)|(1<<PHSEG11) );
 // 设置 相位缓冲段 Phase Seg2为 3TQ ， 禁用唤醒滤波器
 mcp2515_write_register( CNF3, (1<<PHSEG21) );

 //设置为500kbps ，TQ = 1/8us
 //设置分频控制器CNF1.BRP[5:0] = 0，同步跳转长度设置为 CNF1.SJW[1:0] = 01
// mcp2515_write_register( CNF1, (1<<BRP0)|(1<<SJW0) );    // 500kbps
 mcp2515_write_register( CNF1, (1<<SJW0) );    // //1Mbps
 // 设置传播段 Prop Seg 为00，即1TQ，相位缓冲段 Phase Seg1的长度3TQ
 mcp2515_write_register( CNF2, (1<<BTLMODE)|(1<<PHSEG11) );
 // 设置 相位缓冲段 Phase Seg2为 3TQ ， 禁用唤醒滤波器
 mcp2515_write_register( CNF3, (1<<PHSEG21) );
*/

 // 设置MCP2515中断使能寄存器，禁用所有中断
// mcp2515_write_register( CANINTE, /*(1<<RX1IE)|(1<<RX0IE)*/ 0 );
/*
 // 设置MCP2515中断使能寄存器,使能接收缓冲器中断
 mcp2515_write_register( CANINTE, (1<<RX1IE)|(1<<RX0IE) ); //cancel by lqy
 
 //设置数据接收相关寄存器 
 
 // 设置RXM[1:0]=11,关闭接收缓冲器0屏蔽/滤波功能，接收所有报文；禁止滚存功能
 mcp2515_write_register( RXB0CTRL, (1<<RXM1)|(1<<RXM0) );
 
 // 设置RXM[1:0]=11,关闭接收缓冲器1屏蔽/滤波功能，接收所有报文；
 mcp2515_write_register( RXB1CTRL, (1<<RXM1)|(1<<RXM0) );
 
 //设置6个验收滤波寄存器为0，
 mcp2515_write_register_p( RXF0SIDH, temp, 4 );
 mcp2515_write_register_p( RXF1SIDH, temp, 4 );
 mcp2515_write_register_p( RXF2SIDH, temp, 4 );
 mcp2515_write_register_p( RXF3SIDH, temp, 4 );
 mcp2515_write_register_p( RXF4SIDH, temp, 4 );
 mcp2515_write_register_p( RXF5SIDH, temp, 4 );
 
 //设置2个验收滤波寄存器为0，
 mcp2515_write_register_p( RXM0SIDH, temp, 4 );
 mcp2515_write_register_p( RXM1SIDH, temp, 4 );
 
 //配置引脚
 //设置接收相关引脚控制寄存器，配置它们禁用第二功能
 mcp2515_write_register( BFPCTRL, 0 );
 
 //调试使用，设置BFPCTRL使RX0BF,RX1BF设置为数字输出。
 //mcp2515_bit_modify( BFPCTRL, (1<<B1BFE)|(1<<B0BFE)|(1<<B1BFM)|(1<<B0BFM), (1<<B1BFE)|(1<<B0BFE) );
 
 
 //设置发送相关引脚控制寄存器，配置它们禁用第二功能
 mcp2515_write_register( TXRTSCTRL, 0 );
 
 //MCP2515进入环回模式，进行功能测试
 //mcp2515_bit_modify( CANCTRL, 0XE0, (1<<REQOP1) );
 
 //MCP2515进入正常模式
 mcp2515_bit_modify( CANCTRL, 0xE0, 0);
*/

//from hgx code

       //硬件复位
	//GPIO_WriteIO(0,CAN_RST_PORT);	//by lqy //CAN_RST的电平拉低 //RST=0; hwj
	//GPIO_WriteIO(1,CAN_RST_PORT);	//by lqy //CAN_RST的电平拉高//RST=1; hwj
	GPIO_WriteIO(0,CAN_STBY_PORT1);	//by lqy //CAN_STBY的电平拉低
	
    	//先进入配置模式
 	kal_sleep_task(1);
	//data = mcp2515_read_register(CANSTAT);
	//kal_prompt_trace(MOD_SOC, "entern CANSTAT = %d", data);   	
	mcp2515_write_register(CANCTRL,REQOP_CONFIG);
	//kal_sleep_task(1);
	data = mcp2515_read_register(CANSTAT);
	kal_prompt_trace(MOD_SOC, "middle CANSTAT = %d", data);
	data = mcp2515_read_register(CANINTF);
	kal_prompt_trace(MOD_SOC, "middle CANINTF = %d", data);	

	//kal_sleep_task(1);
	data = mcp2515_read_register(CANCTRL);
	kal_prompt_trace(MOD_SOC, "middle CANCTRL = %d", data);

	//波特率
	mcp2515_write_register(CNF1,0x01);	//SJW = 1TQ, BRP set to 4, 2*(7+1)=16,BURT=16/16/8=125KHz
	mcp2515_write_register(CNF2,0x90);	//PHSEG1 = 3TQ, PRSEG = 1TQ
	mcp2515_write_register(CNF3,0x02);	//PHSEG2 = 3TQ

	//验收滤波器0
	mcp2515_write_register(RXF0SIDH,0xfd);	//RXF0SID=0x7ff
	mcp2515_write_register(RXF0SIDL,0x0);
	mcp2515_write_register(RXF0EID8,0xf1);
	mcp2515_write_register(RXF0EID0,0x10);

	//验收滤波器1
	mcp2515_write_register(RXF1SIDH,0xfd);	//RXF1SID=0x7ff
	mcp2515_write_register(RXF1SIDL,0x0);
	mcp2515_write_register(RXF1EID8,0xf1);
	mcp2515_write_register(RXF1EID0,0x10);
#if 1
	//验收滤波器2
	mcp2515_write_register(RXF2SIDH,0x7e);	//RXF2SID=0x3f0
	mcp2515_write_register(RXF2SIDL,0x00);
	mcp2515_write_register(RXF2EID8,0xff);
	mcp2515_write_register(RXF2EID0,0xff);

	//验收滤波器3
	mcp2515_write_register(RXF3SIDH,0x7e);	//RXF3SID=0x3f1
	mcp2515_write_register(RXF3SIDL,0x20);
	mcp2515_write_register(RXF3EID8,0xff);
	mcp2515_write_register(RXF3EID0,0xff);

	//验收滤波器4
	mcp2515_write_register(RXF4SIDH,0x7e);	//RXF4SID=0x3f2
	mcp2515_write_register(RXF4SIDL,0x40);
	mcp2515_write_register(RXF4EID8,0xff);
	mcp2515_write_register(RXF4EID0,0xff);

	//验收滤波器5
	mcp2515_write_register(RXF5SIDH,0x7e);	//RXF5SID=0x3f3
	mcp2515_write_register(RXF5SIDL,0x60);
	mcp2515_write_register(RXF5EID8,0xff);
	mcp2515_write_register(RXF5EID0,0xff);

	//屏蔽滤波器0，对应验收滤波器0-1
	mcp2515_write_register(RXM0SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	mcp2515_write_register(RXM0SIDL,0xff);
	mcp2515_write_register(RXM0EID8,0xff);
	mcp2515_write_register(RXM0EID0,0xff);

	//屏蔽滤波器1，对应验收滤波器2-5
	mcp2515_write_register(RXM1SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	mcp2515_write_register(RXM1SIDL,0xff);
	mcp2515_write_register(RXM1EID8,0xff);
	mcp2515_write_register(RXM1EID0,0xff);

	//发送通道0
	mcp2515_write_register(TXB0CTRL,0x03);	//Highest priority
	mcp2515_write_register(TXB0SIDH,0x7e);	//SID=0x3f0
	mcp2515_write_register(TXB0SIDL,0x00);
	mcp2515_write_register(TXB0EID8,0x00);
	mcp2515_write_register(TXB0EID0,0x00);
	mcp2515_write_register(TXB0DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) mcp2515_write_register(TXB0D0+i,0x00);	//8 bytes data

	//发送通道1
	mcp2515_write_register(TXB1CTRL,0x03);	//Highest priority
	mcp2515_write_register(TXB1SIDH,0x7f);	//SID=0x3fe
	mcp2515_write_register(TXB1SIDL,0xc0);
	mcp2515_write_register(TXB1EID8,0x00);
	mcp2515_write_register(TXB1EID0,0x00);
	mcp2515_write_register(TXB1DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) mcp2515_write_register(TXB1D0+i,0x00);	//8 bytes data

	//发送通道2
	mcp2515_write_register(TXB2CTRL,0x03);	//Highest priority
	mcp2515_write_register(TXB2SIDH,0x7f);	//SID=0x3ff
	mcp2515_write_register(TXB2SIDL,0xe0);
	mcp2515_write_register(TXB2EID8,0x00);
	mcp2515_write_register(TXB2EID0,0x00);
	mcp2515_write_register(TXB2DLC,0x00);	//数据长度=0
	for(i=0;i<8;i++) mcp2515_write_register(TXB2D0+i,0x00);	//8 bytes data

	//接收通道0，对应屏蔽滤波器0和验收滤波器0-1，由于屏蔽和验收全是1，只接收SID=0X7FF地址信号，等于不接收
	mcp2515_write_register(RXB0CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
       //p2515_write_register(RXB0CTRL,0x60);	//Receive only Standard Id's that match Masks/Filters
       
	//接收通道1，对应屏蔽滤波器1和验收滤波器2-5，接收SID=0x3f0-0x3f3的地址信息
	mcp2515_write_register(RXB1CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
#endif
	data = mcp2515_read_register(RXB0CTRL);
	kal_prompt_trace(MOD_SOC, "RXB0CTRL = %x", data);  

	data = mcp2515_read_register(RXB1CTRL);
	kal_prompt_trace(MOD_SOC, "RXB1CTRL = %x", data);  

	//输入输出口配置
	mcp2515_write_register(BFPCTRL,0x3c);	//BFP pins as digital outputs, initial state hi
	mcp2515_write_register(TXRTSCTRL,0x00);	//TXRTS pins as digital inputs

	//开通RX1的接收中断和错误中断
	//mcp2515_write_register(CANINTE,0x22);	//ERRIE and RX1IE enabled
	mcp2515_write_register(CANINTE,0x03);	//RX0IE  enabled  here must 0x01  or int can not effective
	data = mcp2515_read_register(CANINTE);
	kal_prompt_trace(MOD_SOC, "CANINTE = %x", data);  
	
	mcp2515_write_register(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED | CLKOUT_PRE_8);

	kal_prompt_trace(MOD_SOC, "exit mcp2515_init ");
	kal_sleep_task(10);
	data = mcp2515_read_register(CANSTAT);
	kal_prompt_trace(MOD_SOC, "exit CANSTAT = %d", data);      
	//spi_close(handle);

	/*
      //硬件复位
	GPIO_WriteIO(0,CAN_RST_PORT);	//by lqy //CAN_RST的电平拉低 //RST=0;
	GPIO_WriteIO(1,CAN_RST_PORT);	//by lqy //CAN_RST的电平拉高//RST=1;
	 GPIO_WriteIO(0,CAN_STBY_PORT);	//by lqy //CAN_STBY的电平拉低

	//先进入配置模式
	MCP2515_byte_write(CANCTRL,REQOP_CONFIG);

	//波特率
	MCP2515_byte_write(CNF1,0x07);	//SJW = 1TQ, BRP set to 4, 2*(7+1)=16,BURT=16/16/8=125KHz
	MCP2515_byte_write(CNF2,0x90);	//PHSEG1 = 3TQ, PRSEG = 1TQ
	MCP2515_byte_write(CNF3,0x02);	//PHSEG2 = 3TQ

	//验收滤波器0
	MCP2515_byte_write(RXF0SIDH,0xff);	//RXF0SID=0x7ff
	MCP2515_byte_write(RXF0SIDL,0xff);
	MCP2515_byte_write(RXF0EID8,0xff);
	MCP2515_byte_write(RXF0EID0,0xff);

	//验收滤波器1
	MCP2515_byte_write(RXF1SIDH,0xff);	//RXF1SID=0x7ff
	MCP2515_byte_write(RXF1SIDL,0xff);
	MCP2515_byte_write(RXF1EID8,0xff);
	MCP2515_byte_write(RXF1EID0,0xff);

	//验收滤波器2
	MCP2515_byte_write(RXF2SIDH,0x7e);	//RXF2SID=0x3f0
	MCP2515_byte_write(RXF2SIDL,0x00);
	MCP2515_byte_write(RXF2EID8,0xff);
	MCP2515_byte_write(RXF2EID0,0xff);

	//验收滤波器3
	MCP2515_byte_write(RXF3SIDH,0x7e);	//RXF3SID=0x3f1
	MCP2515_byte_write(RXF3SIDL,0x20);
	MCP2515_byte_write(RXF3EID8,0xff);
	MCP2515_byte_write(RXF3EID0,0xff);

	//验收滤波器4
	MCP2515_byte_write(RXF4SIDH,0x7e);	//RXF4SID=0x3f2
	MCP2515_byte_write(RXF4SIDL,0x40);
	MCP2515_byte_write(RXF4EID8,0xff);
	MCP2515_byte_write(RXF4EID0,0xff);

	//验收滤波器5
	MCP2515_byte_write(RXF5SIDH,0x7e);	//RXF5SID=0x3f3
	MCP2515_byte_write(RXF5SIDL,0x60);
	MCP2515_byte_write(RXF5EID8,0xff);
	MCP2515_byte_write(RXF5EID0,0xff);

	//屏蔽滤波器0，对应验收滤波器0-1
	MCP2515_byte_write(RXM0SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write(RXM0SIDL,0xff);
	MCP2515_byte_write(RXM0EID8,0xff);
	MCP2515_byte_write(RXM0EID0,0xff);

	//屏蔽滤波器1，对应验收滤波器2-5
	MCP2515_byte_write(RXM1SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write(RXM1SIDL,0xff);
	MCP2515_byte_write(RXM1EID8,0xff);
	MCP2515_byte_write(RXM1EID0,0xff);

	//发送通道0
	MCP2515_byte_write(TXB0CTRL,0x03);	//Highest priority
	MCP2515_byte_write(TXB0SIDH,0x7e);	//SID=0x3f0
	MCP2515_byte_write(TXB0SIDL,0x00);
	MCP2515_byte_write(TXB0EID8,0x00);
	MCP2515_byte_write(TXB0EID0,0x00);
	MCP2515_byte_write(TXB0DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write(TXB0D0+i,0x00);	//8 bytes data

	//发送通道1
	MCP2515_byte_write(TXB1CTRL,0x03);	//Highest priority
	MCP2515_byte_write(TXB1SIDH,0x7f);	//SID=0x3fe
	MCP2515_byte_write(TXB1SIDL,0xc0);
	MCP2515_byte_write(TXB1EID8,0x00);
	MCP2515_byte_write(TXB1EID0,0x00);
	MCP2515_byte_write(TXB1DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write(TXB1D0+i,0x00);	//8 bytes data

	//发送通道2
	MCP2515_byte_write(TXB2CTRL,0x03);	//Highest priority
	MCP2515_byte_write(TXB2SIDH,0x7f);	//SID=0x3ff
	MCP2515_byte_write(TXB2SIDL,0xe0);
	MCP2515_byte_write(TXB2EID8,0x00);
	MCP2515_byte_write(TXB2EID0,0x00);
	MCP2515_byte_write(TXB2DLC,0x00);	//数据长度=0
	for(i=0;i<8;i++) MCP2515_byte_write(TXB2D0+i,0x00);	//8 bytes data

	//接收通道0，对应屏蔽滤波器0和验收滤波器0-1，由于屏蔽和验收全是1，只接收SID=0X7FF地址信号，等于不接收
	MCP2515_byte_write(RXB0CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters

	//接收通道1，对应屏蔽滤波器0和验收滤波器2-5，接收SID=0x3f0-0x3f3的地址信息
	MCP2515_byte_write(RXB1CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters

	//输入输出口配置
	MCP2515_byte_write(BFPCTRL,0x3c);	//BFP pins as digital outputs, initial state hi
	MCP2515_byte_write(TXRTSCTRL,0x00);	//TXRTS pins as digital inputs

	//开通RX1的接收中断和错误中断
	MCP2515_byte_write(CANINTE,0x22);	//ERRIE and RX1IE enabled

	MCP2515_byte_write(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED | CLKOUT_PRE_8);
 */
// from hgx code
 

}
#endif

#define BIRD_CAN_SIMULATE_SEND_SUPPORT


void mcp2515_init_gpio_spi(void)
{

    SPI_RESULT status;
    unsigned char i, data = 0;		

	GPIO_ModeSetup(SPI_CS_PORT1, 0);
	GPIO_InitIO(1, SPI_CS_PORT1);
	GPIO_WriteIO(1, SPI_CS_PORT1);	

	GPIO_ModeSetup(SPI_CS_PORT2, 0);
	GPIO_InitIO(1, SPI_CS_PORT2);
	GPIO_WriteIO(1, SPI_CS_PORT2);	

	GPIO_ModeSetup(SPI_SCK_PORT, 0);
	GPIO_InitIO(1, SPI_SCK_PORT);
	
	GPIO_ModeSetup(SPI_MOSI_PORT, 0);
	GPIO_InitIO(1, SPI_MOSI_PORT);
	
	GPIO_ModeSetup(SPI_MISO_PORT, 0);
	GPIO_InitIO(1, SPI_MISO_PORT);	

    	//先进入配置模式
 	kal_sleep_task(1);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi config can2");   	
	MCP2515_byte_write2(CANCTRL,REQOP_CONFIG);
	//kal_sleep_task(1);
	data = MCP2515_byte_read2(CANSTAT);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CANSTAT = %d", data);
	data = MCP2515_byte_read2(CANINTF);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CANINTF = %d", data);	

	//kal_sleep_task(1);
	data = MCP2515_byte_read2(CANCTRL);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CANCTRL = %d", data);

	/*250k 波特率*/
	MCP2515_byte_write2(CNF1,0x03);  //0.5us  16M 晶振   1  2  2  3 =8T   4us
	MCP2515_byte_write2(CNF2,0x89);
	MCP2515_byte_write2(CNF3,0x02);	

	/*500k 波特率*/
	//MCP2515_byte_write2(CNF1,0x01);	//SJW = 1TQ, BRP set to 4, 2*(7+1)=16,BURT=16/16/8=125KHz
	//MCP2515_byte_write2(CNF2,0x90);	//PHSEG1 = 3TQ, PRSEG = 1TQ
	//MCP2515_byte_write2(CNF3,0x02);	//PHSEG2 = 3TQ

	//验收滤波器0
	MCP2515_byte_write2(RXF0SIDH,0xfd);	//RXF0SID=0x7ff
	MCP2515_byte_write2(RXF0SIDL,0x0);
	MCP2515_byte_write2(RXF0EID8,0xf1);
	MCP2515_byte_write2(RXF0EID0,0x10);

	//验收滤波器1
	MCP2515_byte_write2(RXF1SIDH,0xfd);	//RXF1SID=0x7ff
	MCP2515_byte_write2(RXF1SIDL,0x0);
	MCP2515_byte_write2(RXF1EID8,0xf1);
	MCP2515_byte_write2(RXF1EID0,0x10);
	//验收滤波器2
	MCP2515_byte_write2(RXF2SIDH,0x7e);	//RXF2SID=0x3f0
	MCP2515_byte_write2(RXF2SIDL,0x00);
	MCP2515_byte_write2(RXF2EID8,0xff);
	MCP2515_byte_write2(RXF2EID0,0xff);

	//验收滤波器3
	MCP2515_byte_write2(RXF3SIDH,0x7e);	//RXF3SID=0x3f1
	MCP2515_byte_write2(RXF3SIDL,0x20);
	MCP2515_byte_write2(RXF3EID8,0xff);
	MCP2515_byte_write2(RXF3EID0,0xff);

	//验收滤波器4
	MCP2515_byte_write2(RXF4SIDH,0x7e);	//RXF4SID=0x3f2
	MCP2515_byte_write2(RXF4SIDL,0x40);
	MCP2515_byte_write2(RXF4EID8,0xff);
	MCP2515_byte_write2(RXF4EID0,0xff);

	//验收滤波器5
	MCP2515_byte_write2(RXF5SIDH,0x7e);	//RXF5SID=0x3f3
	MCP2515_byte_write2(RXF5SIDL,0x60);
	MCP2515_byte_write2(RXF5EID8,0xff);
	MCP2515_byte_write2(RXF5EID0,0xff);

	//屏蔽滤波器0，对应验收滤波器0-1
	MCP2515_byte_write2(RXM0SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write2(RXM0SIDL,0xff);
	MCP2515_byte_write2(RXM0EID8,0xff);
	MCP2515_byte_write2(RXM0EID0,0xff);

	//屏蔽滤波器1，对应验收滤波器2-5
	MCP2515_byte_write2(RXM1SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write2(RXM1SIDL,0xff);
	MCP2515_byte_write2(RXM1EID8,0xff);
	MCP2515_byte_write2(RXM1EID0,0xff);

	//发送通道0
	MCP2515_byte_write2(TXB0CTRL,0x03);	//Highest priority
	MCP2515_byte_write2(TXB0SIDH,0x7e);	//SID=0x3f0
	MCP2515_byte_write2(TXB0SIDL,0x00);
	MCP2515_byte_write2(TXB0EID8,0x00);
	MCP2515_byte_write2(TXB0EID0,0x00);
	MCP2515_byte_write2(TXB0DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write2(TXB0D0+i,0x00);	//8 bytes data

	//发送通道1
	MCP2515_byte_write2(TXB1CTRL,0x03);	//Highest priority
	MCP2515_byte_write2(TXB1SIDH,0x7f);	//SID=0x3fe
	MCP2515_byte_write2(TXB1SIDL,0xc0);
	MCP2515_byte_write2(TXB1EID8,0x00);
	MCP2515_byte_write2(TXB1EID0,0x00);
	MCP2515_byte_write2(TXB1DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write2(TXB1D0+i,0x00);	//8 bytes data

	//发送通道2
	MCP2515_byte_write2(TXB2CTRL,0x03);	//Highest priority
	MCP2515_byte_write2(TXB2SIDH,0x7f);	//SID=0x3ff
	MCP2515_byte_write2(TXB2SIDL,0xe0);
	MCP2515_byte_write2(TXB2EID8,0x00);
	MCP2515_byte_write2(TXB2EID0,0x00);
	MCP2515_byte_write2(TXB2DLC,0x00);	//数据长度=0
	for(i=0;i<8;i++) MCP2515_byte_write2(TXB2D0+i,0x00);	//8 bytes data

	//接收通道0，对应屏蔽滤波器0和验收滤波器0-1，由于屏蔽和验收全是1，只接收SID=0X7FF地址信号，等于不接收
	//MCP2515_byte_write2(RXB0CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
       MCP2515_byte_write2(RXB0CTRL,0x60);	//Receive only Standard Id's that match Masks/Filters
       
	//接收通道1，对应屏蔽滤波器1和验收滤波器2-5，接收SID=0x3f0-0x3f3的地址信息
	MCP2515_byte_write2(RXB1CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
	data = MCP2515_byte_read2(RXB0CTRL);
	kal_prompt_trace(MOD_SOC, "RXB0CTRL = %x", data);  

	data = MCP2515_byte_read2(RXB1CTRL);
	kal_prompt_trace(MOD_SOC, "RXB1CTRL = %x", data);  

	//输入输出口配置
	MCP2515_byte_write2(BFPCTRL,0x3c);	//BFP pins as digital outputs, initial state hi
	MCP2515_byte_write2(TXRTSCTRL,0x00);	//TXRTS pins as digital inputs

	//开通RX1的接收中断和错误中断
	//mcp2515_write_register(CANINTE,0x22);	//ERRIE and RX1IE enabled
#if 0//def BIRD_CAN_SIMULATE_SEND_SUPPORT	
	MCP2515_byte_write2(CANINTE,0x04);	//RX0IE  enabled  here must 0x01  or int can not effective
#else
	MCP2515_byte_write2(CANINTE,0x01);
#endif
	kal_prompt_trace(MOD_SOC, "CANINTE = %x", data);  
	
	MCP2515_byte_write2(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED | CLKOUT_PRE_8);

	kal_prompt_trace(MOD_SOC, "exit mcp2515_init ");
	kal_sleep_task(10);
	data = MCP2515_byte_read2(CANSTAT);
	kal_prompt_trace(MOD_SOC, "exit CANSTAT = %d", data);      
	//#else
	/*config can1 device*/
	kal_sleep_task(1);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi config can1");   	
	MCP2515_byte_write1(CANCTRL,REQOP_CONFIG);
	//kal_sleep_task(1);
	data = MCP2515_byte_read1(CANSTAT);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CAN1STAT = %d", data);
	data = MCP2515_byte_read1(CANINTF);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CAN1INTF = %d", data);	

	//kal_sleep_task(1);
	data = MCP2515_byte_read1(CANCTRL);
	kal_prompt_trace(MOD_SOC, "mcp2515_init_gpio_spi CAN1CTRL = %d", data);

	/*250k 波特率*/
	MCP2515_byte_write1(CNF1,0x03);  //0.5us  16M 晶振   1  2  2  3 =8T   4us
	MCP2515_byte_write1(CNF2,0x89);
	MCP2515_byte_write1(CNF3,0x02);	
	
	/*500k 波特率*/
	//MCP2515_byte_write1(CNF1,0x01);	//SJW = 1TQ, BRP set to 4, 2*(7+1)=16,BURT=16/16/8=125KHz
	//MCP2515_byte_write1(CNF2,0x90);	//PHSEG1 = 3TQ, PRSEG = 1TQ
	//MCP2515_byte_write1(CNF3,0x02);	//PHSEG2 = 3TQ

	//验收滤波器0
	MCP2515_byte_write1(RXF0SIDH,0xfd);	//RXF0SID=0x7ff
	MCP2515_byte_write1(RXF0SIDL,0x0);
	MCP2515_byte_write1(RXF0EID8,0xf1);
	MCP2515_byte_write1(RXF0EID0,0x10);

	//验收滤波器1
	MCP2515_byte_write1(RXF1SIDH,0xfd);	//RXF1SID=0x7ff
	MCP2515_byte_write1(RXF1SIDL,0x0);
	MCP2515_byte_write1(RXF1EID8,0xf1);
	MCP2515_byte_write1(RXF1EID0,0x10);
	//验收滤波器2
	MCP2515_byte_write1(RXF2SIDH,0x7e);	//RXF2SID=0x3f0
	MCP2515_byte_write1(RXF2SIDL,0x00);
	MCP2515_byte_write1(RXF2EID8,0xff);
	MCP2515_byte_write1(RXF2EID0,0xff);

	//验收滤波器3
	MCP2515_byte_write1(RXF3SIDH,0x7e);	//RXF3SID=0x3f1
	MCP2515_byte_write1(RXF3SIDL,0x20);
	MCP2515_byte_write1(RXF3EID8,0xff);
	MCP2515_byte_write1(RXF3EID0,0xff);

	//验收滤波器4
	MCP2515_byte_write1(RXF4SIDH,0x7e);	//RXF4SID=0x3f2
	MCP2515_byte_write1(RXF4SIDL,0x40);
	MCP2515_byte_write1(RXF4EID8,0xff);
	MCP2515_byte_write1(RXF4EID0,0xff);

	//验收滤波器5
	MCP2515_byte_write1(RXF5SIDH,0x7e);	//RXF5SID=0x3f3
	MCP2515_byte_write1(RXF5SIDL,0x60);
	MCP2515_byte_write1(RXF5EID8,0xff);
	MCP2515_byte_write1(RXF5EID0,0xff);

	//屏蔽滤波器0，对应验收滤波器0-1
	MCP2515_byte_write1(RXM0SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write1(RXM0SIDL,0xff);
	MCP2515_byte_write1(RXM0EID8,0xff);
	MCP2515_byte_write1(RXM0EID0,0xff);

	//屏蔽滤波器1，对应验收滤波器2-5
	MCP2515_byte_write1(RXM1SIDH,0xff);	//0x7ff,屏蔽滤波器所有位都是1，则所有位都必须和验收滤波器匹配才接收
	MCP2515_byte_write1(RXM1SIDL,0xff);
	MCP2515_byte_write1(RXM1EID8,0xff);
	MCP2515_byte_write1(RXM1EID0,0xff);

	//发送通道0
	MCP2515_byte_write1(TXB0CTRL,0x03);	//Highest priority
	MCP2515_byte_write1(TXB0SIDH,0x7e);	//SID=0x3f0
	MCP2515_byte_write1(TXB0SIDL,0x00);
	MCP2515_byte_write1(TXB0EID8,0x00);
	MCP2515_byte_write1(TXB0EID0,0x00);
	MCP2515_byte_write1(TXB0DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write1(TXB0D0+i,0x00);	//8 bytes data

	//发送通道1
	MCP2515_byte_write1(TXB1CTRL,0x03);	//Highest priority
	MCP2515_byte_write1(TXB1SIDH,0x7f);	//SID=0x3fe
	MCP2515_byte_write1(TXB1SIDL,0xc0);
	MCP2515_byte_write1(TXB1EID8,0x00);
	MCP2515_byte_write1(TXB1EID0,0x00);
	MCP2515_byte_write1(TXB1DLC,0x01);	//数据长度=1
	for(i=0;i<8;i++) MCP2515_byte_write1(TXB1D0+i,0x00);	//8 bytes data

	//发送通道2
	MCP2515_byte_write1(TXB2CTRL,0x03);	//Highest priority
	MCP2515_byte_write1(TXB2SIDH,0x7f);	//SID=0x3ff
	MCP2515_byte_write1(TXB2SIDL,0xe0);
	MCP2515_byte_write1(TXB2EID8,0x00);
	MCP2515_byte_write1(TXB2EID0,0x00);
	MCP2515_byte_write1(TXB2DLC,0x00);	//数据长度=0
	for(i=0;i<8;i++) MCP2515_byte_write1(TXB2D0+i,0x00);	//8 bytes data

	//接收通道0，对应屏蔽滤波器0和验收滤波器0-1，由于屏蔽和验收全是1，只接收SID=0X7FF地址信号，等于不接收
	//MCP2515_byte_write2(RXB0CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
       MCP2515_byte_write1(RXB0CTRL,0x60);	//Receive only Standard Id's that match Masks/Filters
       
	//接收通道1，对应屏蔽滤波器1和验收滤波器2-5，接收SID=0x3f0-0x3f3的地址信息
	MCP2515_byte_write1(RXB1CTRL,0x20);	//Receive only Standard Id's that match Masks/Filters
	data = MCP2515_byte_read1(RXB0CTRL);
	kal_prompt_trace(MOD_SOC, "CAN1 RXB0CTRL = %x", data);  

	data = MCP2515_byte_read1(RXB1CTRL);
	kal_prompt_trace(MOD_SOC, "CAN1 RXB1CTRL = %x", data);  

	//输入输出口配置
	MCP2515_byte_write1(BFPCTRL,0x3c);	//BFP pins as digital outputs, initial state hi
	MCP2515_byte_write1(TXRTSCTRL,0x00);	//TXRTS pins as digital inputs

	//开通RX1的接收中断和错误中断
	//mcp2515_write_register(CANINTE,0x22);	//ERRIE and RX1IE enabled
#if 0//def BIRD_CAN_SIMULATE_SEND_SUPPORT	
	MCP2515_byte_write1(CANINTE,0x04);	//RX0IE  enabled  here must 0x01  or int can not effective
#else
	MCP2515_byte_write1(CANINTE,0x01);
#endif
	data = MCP2515_byte_read1(CANINTE);
	kal_prompt_trace(MOD_SOC, "CAN1 CANINTE = %x", data);  
	
	MCP2515_byte_write1(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED );  //| CLKOUT_PRE_8

	kal_prompt_trace(MOD_SOC, "exit mcp2515_init_gpio_spi ");
	kal_sleep_task(10);
	data = MCP2515_byte_read1(CANSTAT);
	kal_prompt_trace(MOD_SOC, "exit CANSTAT = %d", data);    
}

