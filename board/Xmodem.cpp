
#include "Xmodem.h"

UINT16 linkCallCyc = 0;
UINT16 readOutTimer = 0;
UINT16 commPulseTimer = 0;
UINT16 commPulseWidth = 0;

/*******************************************************************************
*
* TIME0溢出中断服务函数 
* #pragma vector=TIMER0_OVF_vect
* __interrupt void TIMER0_OVF_INTERRUPT (void)
*
******************************************************************************
SIGNAL(TIMER0_OVF_vect)
{
    TCNT0 = time0InitValue;		//TCNT0 = (0xFF-F_CPU/1024/1000*x);
    
    linkCallCyc++;
    readOutTimer++;
    commPulseTimer++;  
    if (linkCallCyc >= 65530)
    {
        linkCallCyc = 65530;
    }
    if (readOutTimer >= 65530)
    {
        readOutTimer = 65530;
    }
    if (commPulseTimer >= 65530)
    {
        commPulseTimer = 65530;
    }
}*/

/*******************************************************************************
* 
* 延时函数
* 功能：实现程序延时
* 参数说明：ms
*
*******************************************************************************/
void XModemDelayMs(UINT16 ms)
{
    register UINT16 i;
    while( ms-- )
    {
        for(i=0; i<1000; i++)
        {
            ;
        }		
    }
}

/*******************************************************************************
*
* Function Name:  XModemRecTransInit()
* Purpose:        初始化收发送器。
* Inputs:         pReceiver             表示一个接收器。
                  pTransmitter			表示一个发送器。 
* Outputs:        None
* Return Values:  XM_ERR_NO_ERROR		初始化成功。
                  XM_ERR_BAD_PARAM		pReceiver为NULL。
* Comments:       None
* See Also:       None
*
*******************************************************************************/
XM_ERROR_CODE_T XModemRecTransInit(
    XMODEM_RECEIVER_T	        * pReceiver,                                      
    XMODEM_TRANSMITTER_T		* pTransmitter
    )
{
    UINT8 i = 0;    
    if ((pTransmitter == NULL) || (pReceiver == NULL))
    {
        return XM_ERR_BAD_PARAM;
    }
    for (i = 0; i < XM_DATA_MAX_SIZE; i++)
    {
        pTransmitter->blockBuffer[i] = 0x00;
        pReceiver->blockBuffer[i] = 0x00;
    }   
    pTransmitter->bytesTransmitted = 0x00;
    pReceiver->bytesReceived= 0x00;

    return XM_ERR_NO_ERROR;
}

/*******************************************************************************
*
*  Function Name:  XModemSlaveReceive()
*  Purpose:        从接收
*  Inputs:         slaveAdr					从机本地地址
                   pReceiver				表示一个接收器。
                   pTransmitter				表示一个发送器。            
*  Outputs:        pDataBuffer				用以保存收发数据。
*  Return Values:  XM_ERR_NO_ERROR
                   XM_ERR_BAD_PARAM
                   XM_ERR_BUF_OVERFLOW
                   XM_ERR_TIMEOUT			超时；
                   XM_ERR_OUT_OF_SYNC		线路失去同步
*  Comments:       None
*  See Also:       XModemReceiveBlock()
*
*******************************************************************************/
XM_ERROR_CODE_T XModemSlaveReceive( 
    XM_ADDR_CODE_T              slaveAdr,
    UINT8						* pDataBuffer,
    XMODEM_RECEIVER_T	        * pReceiver,
    XMODEM_TRANSMITTER_T		* pTransmitter
    )
{
    UINT8		i = 0; 
    UINT8       recBlockSize = 0;
    UINT8       transBlockSize = 0;
    UINT16		chksumCRC = 0;
    if ((pDataBuffer == NULL) || (pReceiver == NULL) || (pTransmitter == NULL))
    {
        return XM_ERR_BAD_PARAM;
    }

    /* Receive block data */
  #ifdef _RS485_
    ClrBit(XM_RS485_RW_PORT, XM_RS485_RW_BIT);
  #endif      
    for (i=0; i<(XM_BLOCK_HEAD + XM_BLOCK_TAIL); i++)
    {
        if (0 == XModemReadByte(&(pReceiver->blockBuffer[i]), XM_MAX_READ_TIME))
        {
             return XM_ERR_TIMEOUT;
        }
    }
    if (pReceiver->blockBuffer[1] == (UINT8)R_CODE)
    {
        recBlockSize   = (XM_BLOCK_HEAD + XM_BLOCK_TAIL);
        transBlockSize = (XM_BLOCK_HEAD + pReceiver->blockBuffer[2] + XM_BLOCK_TAIL);
    }
    else if (pReceiver->blockBuffer[1] == (UINT8)W_CODE)
    {
        recBlockSize   = (XM_BLOCK_HEAD + pReceiver->blockBuffer[2] + XM_BLOCK_TAIL);
        transBlockSize = (XM_BLOCK_HEAD + XM_BLOCK_TAIL);
    }
    for (i=(XM_BLOCK_HEAD + XM_BLOCK_TAIL); i<recBlockSize; i++)
    {
        if (0 == XModemReadByte(&(pReceiver->blockBuffer[i]), XM_MAX_READ_TIME))
        {
             return XM_ERR_TIMEOUT;
        }
    }
    //Check data field 
    chksumCRC = (((pReceiver->blockBuffer[recBlockSize - 2])<<8) 
                | (pReceiver->blockBuffer[recBlockSize - 1]));
    if ((chksumCRC == CRC16(&(pReceiver->blockBuffer[0]), (recBlockSize-2)))
    && ((pReceiver->blockBuffer[0]) == (UINT8)slaveAdr))
    {
        for (i=0; i<(recBlockSize - XM_BLOCK_HEAD - XM_BLOCK_TAIL); i++)
        {
            //pDataBuffer[i] = pReceiver->blockBuffer[i+XM_BLOCK_HEAD]; 
            *(pDataBuffer+i) = pReceiver->blockBuffer[i+XM_BLOCK_HEAD]; 
        }                 
    }
    else
    {
        XModemClearLine();
        return XM_ERR_BAD_CRC_CHK;
    }

    /* Wait for Block data.
    [<slaveAdr><wrCode><dataLen><data>..<data>]<CRCH><CRCL>..
    */
    pTransmitter->blockBuffer[0] = (UINT8)slaveAdr;        //地址码
    pTransmitter->blockBuffer[1] = (UINT8)ACK_CODE;        //响应码
    pTransmitter->blockBuffer[2] = (transBlockSize - XM_BLOCK_HEAD - XM_BLOCK_TAIL);//长度码
    for (i=0; i<(transBlockSize - XM_BLOCK_HEAD - XM_BLOCK_TAIL); i++)
    {
        //pTransmitter->blockBuffer[i+XM_BLOCK_HEAD] = pDataBuffer[i];
        pTransmitter->blockBuffer[i+XM_BLOCK_HEAD] = *(pDataBuffer+i); 
    }   
    //CRC chksum.
    chksumCRC = CRC16(&(pTransmitter->blockBuffer[0]), (transBlockSize-2));
    pTransmitter->blockBuffer[transBlockSize - 2] = ((chksumCRC>>8)&0xFF); //高字节
    pTransmitter->blockBuffer[transBlockSize - 1] = (chksumCRC&0xFF);      //低字节   
	
    /* Transmit 
    [<slaveAdr><wrCode><dataLen><data>..<data>]<CRCH><CRCL>..
    */
  #ifdef _RS485_
    /*485通讯，确保对方处于接收状态
    115200->1;   19200->50;    2400->100;
    */
    XModemDelayMs(100);
    SetBit(XM_RS485_RW_PORT, XM_RS485_RW_BIT);
  #endif
    for (i=0; i<transBlockSize; i++)
    {
        XModemWriteByte(pTransmitter->blockBuffer[i], XM_MAX_WRITE_TIME);
        /*485通讯，确保对方接收完成。
        115200->1;   19200->3;    2400->10;
        */
      #ifdef _RS485_
        XModemDelayMs(10);
      #endif
    }  
    
    return XM_ERR_NO_ERROR;
}

/*******************************************************************************
*
*  Function Name:  XModemMasterTransmit()
*  Purpose:        主发送
*  Inputs:         slaveAdr                     指定发送地址
                   wrCode                       指定功能，WR
                   dataLen                      发送接收长度。
                   pReceiver					表示一个接收器。
                   pTransmitter                 表示一个发送器。
*  Outputs:        pDataBuffer					用以保存收发数据。
*  Return Values:  XM_ERR_NO_ERROR
                   XM_ERR_BAD_PARAM
                   XM_ERR_BUF_OVERFLOW
                   XM_ERR_TIMEOUT				超时；
                   XM_ERR_OUT_OF_SYNC			线路失去同步
*  Comments:       None
*  See Also:       XModemReceiveBlock()
*
*******************************************************************************/
XM_ERROR_CODE_T XModemMasterTransmit(
    XM_ADDR_CODE_T              slaveAdr,
    XM_FUN_CODE_T               wrCode,
    UINT8                       dataLen,
    UINT8						* pDataBuffer,
    XMODEM_RECEIVER_T	        * pReceiver,
    XMODEM_TRANSMITTER_T		* pTransmitter
    )
{
    UINT8		i = 0; 
    UINT8       recBlockSize = 0;
    UINT8       transBlockSize = 0;
    UINT16		chksumCRC = 0;
    if ((dataLen > XM_DATA_MAX_SIZE) || (dataLen == 0))
    {
        return XM_ERR_BAD_PARAM;
    }
    if ((pDataBuffer == NULL) || (pReceiver == NULL) || (pTransmitter == NULL))
    {
        return XM_ERR_BAD_PARAM;
    }  
	
    /* 计算收发BlockSize */
    if (wrCode == R_CODE)
    {
        transBlockSize = (XM_BLOCK_HEAD + XM_BLOCK_TAIL);
        recBlockSize   = (XM_BLOCK_HEAD + dataLen + XM_BLOCK_TAIL);
    }
    else if (wrCode == W_CODE)
    {   
        transBlockSize = (XM_BLOCK_HEAD + dataLen + XM_BLOCK_TAIL);
        recBlockSize   = (XM_BLOCK_HEAD + XM_BLOCK_TAIL);
    }

    /* Wait for Block data.
    [<slaveAdr><wrCode><dataLen><data>..<data>]<CRCH><CRCL>..
    */
    pTransmitter->blockBuffer[0] = (UINT8)slaveAdr;        //地址码
    pTransmitter->blockBuffer[1] = (UINT8)wrCode;          //功能码
    pTransmitter->blockBuffer[2] = dataLen;                //长度码
    for (i=0; i<(transBlockSize - XM_BLOCK_HEAD - XM_BLOCK_TAIL); i++)
    {
        //pTransmitter->blockBuffer[i+XM_BLOCK_HEAD] = pDataBuffer[i];
        pTransmitter->blockBuffer[i+XM_BLOCK_HEAD] = *(pDataBuffer+i); 
    }   
    //CRC chksum.
    chksumCRC = CRC16(&(pTransmitter->blockBuffer[0]), (transBlockSize-2));
    pTransmitter->blockBuffer[transBlockSize - 2] = ((chksumCRC>>8)&0xFF); //高字节
    pTransmitter->blockBuffer[transBlockSize - 1] = (chksumCRC&0xFF);      //低字节 
	
    /* Transmit 
    [<slaveAdr><wrCode><dataLen><data>..<data>]<CRCH><CRCL>..
    */
  #ifdef _RS485_
    SetBit(XM_RS485_RW_PORT, XM_RS485_RW_BIT);
  #endif
    for (i=0; i<transBlockSize; i++)
    {
        XModemWriteByte(pTransmitter->blockBuffer[i], XM_MAX_WRITE_TIME);
        /*485通讯，确保对方接收完成。
        115200->1;   19200->3;    2400->10;
        */
      #ifdef _RS485_
        XModemDelayMs(10);
      #endif
    }
  #ifdef _RS485_
    ClrBit(XM_RS485_RW_PORT, XM_RS485_RW_BIT);
  #endif
    /* Wait for responses*/        
    for (i = 0; i < recBlockSize; i++)
    {
        if (0 == XModemReadByte(&(pReceiver->blockBuffer[i]), XM_MAX_READ_TIME))
        {
             return XM_ERR_TIMEOUT;
        }
    }
    //Check data field 
    chksumCRC = (((pReceiver->blockBuffer[recBlockSize - 2])<<8) 
                | (pReceiver->blockBuffer[recBlockSize - 1]));
    if ((chksumCRC == CRC16(&(pReceiver->blockBuffer[0]), (recBlockSize - 2)))
     && (pReceiver->blockBuffer[0] == (UINT8)slaveAdr))
    {
        for (i=0; i<(recBlockSize - XM_BLOCK_HEAD - XM_BLOCK_TAIL); i++)
        {
            //pDataBuffer[i] = pReceiver->blockBuffer[i+XM_BLOCK_HEAD];
            *(pDataBuffer+i) = pReceiver->blockBuffer[i+XM_BLOCK_HEAD]; 
        }
        return XM_ERR_NO_ERROR;                 
    }
    else
    {
        XModemClearLine();
    }    
  #ifdef _RS485_
    /*485通讯，确保对方处于接收状态
    115200->1;   19200->50;    2400->100;
    */
    XModemDelayMs(100);
  #endif
    
    return XM_ERR_TIMEOUT;
}

/******************************************************************************* 
*
* Function Name:  XModemCommEngine()
* Purpose:        通讯引擎
* Inputs:         slaveAdr                  指定发送地址
                  wrCode                    指定功能，WR
                  dataLen                   发送接收长度。
* Outputs:        pDataBuffer				用以保存收发数据。                         
* Return Values:  none
* Comments:       None
* See Also:       XModemMasterTransmit()
                  XModemSlaveReceive()  
*
*******************************************************************************/ 
#ifdef _MASTER_MODE_
bool XModemCommEngine(
    XM_ADDR_CODE_T			slaveAdr,
    XM_FUN_CODE_T			wrCode,
    UINT8					dataLen,
    UINT8					* pDataBuffer
    )
{  
    XMODEM_RECEIVER_T	    masterRec;
    XMODEM_TRANSMITTER_T    masterTrans;
    XModemRecTransInit(&masterRec, &masterTrans);
    
    COMM_STATE_LED_OFF();
    commPulseTimer = 0;
    if (XM_ERR_NO_ERROR == XModemMasterTransmit(slaveAdr, wrCode, dataLen, pDataBuffer, &masterRec, &masterTrans))
    {   
        COMM_STATE_LED_ON();
        commPulseWidth = commPulseTimer;
        commPulseTimer = 0;
        
        return 1;
    }
    return 0;
}

#else //slave mode 
bool XModemCommEngine(
	XM_ADDR_CODE_T			slaveAdr, 
	UINT8					* pDataBuffer
	)
{  
    XMODEM_RECEIVER_T	    slaveRec;
    XMODEM_TRANSMITTER_T    slaveTrans;
    XModemRecTransInit(&slaveRec, &slaveTrans);
    
    COMM_STATE_LED_OFF();
    commPulseTimer = 0;
  #ifdef _RS485_
    ClrBit(XM_RS485_RW_PORT, XM_RS485_RW_BIT);
  #endif
    if (XM_ERR_NO_ERROR == XModemSlaveReceive(slaveAdr, pDataBuffer, &slaveRec, &slaveTrans))
    {
        COMM_STATE_LED_ON();
        commPulseWidth = commPulseTimer;
        commPulseTimer = 0;
            
        return TRUE;           
    }
    return FALSE;
}
#endif