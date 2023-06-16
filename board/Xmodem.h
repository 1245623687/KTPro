/****************************************************************************
*
* Copyright (c) 2009  X&D NanJing Co.Ltd.  All Rights Reserved
*
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF  X&D NanJing Co.Ltd.
* The copyright notice above does not evidence any actual or intended
* publication of such source code.
*
*****************************************************************************/

/*****************************************************************************
* File Name  :      Xmodem.h
*
* Description:      Xmodem module header file.
*      
* Author     :      XuDeng  xd6181754@yahoo.com.cn                   
*
* Revision History:
*
*  ---------------------------------------------------------------------------
* | Date        | Author | Version |  Changes                                 |
*  --------------------------------------------------------------------------- 
* | 26.Feb.2009 | XD     | 1.0     | Initial version                          |
*  --------------------------------------------------------------------------- 
* | 03.Jun.2009 | XD     | 2.0     | 128/128L CoreConfigLib added             |
*  ---------------------------------------------------------------------------
* | 09.Apr.2010 | XD     | 2.1     | Modfiy for IAR                           |  
*  ---------------------------------------------------------------------------
*
******************************************************************************/
#ifndef XMODEM_H
    #define XMODEM_H
	#include "xddefs.h"
	#include "CRC.h"
	#include "Update_SerialPort.h"

	#define _MASTER_MODE_

	/* 定义物理层 */
    #define XModemWriteByte     SerialWriteByte 
    #define XModemReadByte      SerialReadByte 
    #define XModemClearLine     SerialClearLine
	#define XM_MAX_READ_TIME	100
	#define XM_MAX_WRITE_TIME	100
    
    /* 定义通讯状态灯和RS485通讯时的RW口 */
  #ifdef _BOARD_AW01A2_
    #define COMM_STATE_LED_ON()     
    #define COMM_STATE_LED_OFF()    
      #ifdef _RS485_
        #define XM_RS485_RW_PORT     PORTE                   
        #define XM_RS485_RW_BIT      2                       
      #endif
  #else
	#define COMM_STATE_LED_ON()     
    #define COMM_STATE_LED_OFF() 
  #endif

    /* Block的大小 */
    #define XM_DATA_MAX_SIZE		32
    #define XM_BLOCK_HEAD	        3
    #define XM_BLOCK_TAIL			2
    #define XM_BLOCK_MAX_SIZE       (XM_BLOCK_HEAD + XM_DATA_MAX_SIZE + XM_BLOCK_TAIL)
    
    /* 定义网络地址 
    MASTER:0xFE;    SLAVE1:0xFC;    SLAVE2:0xFA;
    */
    typedef enum XM_ADDR_CODE_S
    {
        MASTER_CODE             = 0xFE,
        SLAVE1_CODE             = 0xFC,
        SLAVE2_CODE             = 0xFA
    }XM_ADDR_CODE_T;
    
    /* 定义功能码
    读取：R_CODE 0XEE;  写入：W_CODE 0xEC; 
    */
    typedef enum XM_FUN_CODE_S
    {
        R_CODE                  = 0xEE,
        W_CODE                  = 0xEC,
        ACK_CODE                = 0xEA
    }XM_FUN_CODE_T;
    
    /* Logical Communications Controls */  
    typedef enum XM_LOG_CODE_S
    {
        ACK						= 0xDE, //acknowledge 
        NAK						= 0xDC,	//negative acknowledge
        C						= 0xDA,	//start of text
        STX						= 0xD8,	//start of text
        EOT						= 0xD6	//end of transmission 
    }XM_LOG_CODE_T;
    
    /* 定义错误码 */
    typedef enum XM_ERROR_CODE_S
    {
        XM_ERR_NO_ERROR			,	//没有错误
        XM_ERR_BAD_CALL         ,   //呼叫失败
        XM_ERR_TIMEOUT			,	//超时
        XM_ERR_BAD_ADR	        ,	//网络地址错误
        XM_ERR_BAD_PARAM		,	//参数错误
        XM_ERR_BUF_OVERFLOW		,	//缓存溢出
        XM_ERR_BAD_CRC_CHK		,	//CRC校验错误
        XM_ERR_MAX_RETRY		,	//超过最大重试次数
        XM_ERR_BAD_BLK_HEADER   ,   //Block的头错误
        XM_END_OF_TRANSMIT			//传输结束
    }XM_ERROR_CODE_T;

    /* 表示一个XModem接收器 */
    typedef struct XMODEM_RECEIVER_S
    {
        UINT8		blockBuffer[XM_BLOCK_MAX_SIZE];
        UINT16		bytesReceived;   
    }XMODEM_RECEIVER_T;

    /* 表示一个XModem发送器 */
    typedef struct XMODEM_TRANSMITTER_S
    {
        UINT8		blockBuffer[XM_BLOCK_MAX_SIZE];
        UINT16		bytesTransmitted;    
    }XMODEM_TRANSMITTER_T;
    
    /* Declare Comm control variables 
    commPulseWidth：成功通讯一次的时间。不同的波特率，参考对应表如下：
    RS232: 19200->; 9600->; 4800->;
    RS485: 2400->;
    */
    extern UINT16 linkCallCyc;
    extern UINT16 readOutTimer;
    extern UINT16 commPulseTimer;
    extern UINT16 commPulseWidth;
    
    /* 延时函数 */
    void XModemDelayMs(UINT16 ms);    
    
    /* 初始化收发器 */
    XM_ERROR_CODE_T XModemReceiveTransmitInit(
        XMODEM_RECEIVER_T       * pReceiver,
        XMODEM_TRANSMITTER_T	* pTransmitter
        );

    /* 从接收 */
    XM_ERROR_CODE_T XModemSlaveReceive( 
        XM_ADDR_CODE_T          slaveAdr,
        UINT8					* pDataBuffer,
        XMODEM_RECEIVER_T		* pReceiver,
        XMODEM_TRANSMITTER_T	* pTransmitter
        );

    /* 主发送 */
    XM_ERROR_CODE_T XModemMasterTransmit(
        XM_ADDR_CODE_T          slaveAdr,
        XM_FUN_CODE_T           wrCode,
        UINT8                   dataLen,
        UINT8					* pDataBuffer,
        XMODEM_RECEIVER_T		* pReceiver,
        XMODEM_TRANSMITTER_T	* pTransmitter
        );
    
    /* 通讯引擎 */
    #ifdef _MASTER_MODE_
    bool XModemCommEngine(
        XM_ADDR_CODE_T          slaveAdr,
        XM_FUN_CODE_T           wrCode,
        UINT8                   dataLen,
        UINT8					* pDataBuffer
        );
    #else  //slave mode
    bool XModemCommEngine(
        XM_ADDR_CODE_T          slaveAdr,
        UINT8					* pDataBuffer
        ); 
	#endif 

#endif