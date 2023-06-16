/****************************************************************************
*
* Copyright (c) 2009 NanJing DaShu Co.Ltd.  All Rights Reserved
*
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF NanJing DaShu Co.Ltd.
* The copyright notice above does not evidence any actual or intended
* publication of such source code.
*
*****************************************************************************/

/*****************************************************************************
* File Name  :		 SerialCom.h
*
* Description:		 Update serial port header file.
*      
* Author     :       XuDeng  xd6181754@yahoo.com.cn                   
*
* Revision History:
*
*  ---------------------------------------------------------------------------
* | Date        | Author | Version |  Changes                                 |
*  --------------------------------------------------------------------------- 
* | 26.Feb.2009 | XD     | 1.0     | Initial version                          |
*  --------------------------------------------------------------------------- 
* |             |        |	       |                                          |
*  ---------------------------------------------------------------------------
* |             |        |         |                                          |
*	---------------------------------------------------------------------------	
*
******************************************************************************/
#ifndef __UPDATE_SERIAL_PORT_H__
	#define __UPDATE_SERIAL_PORT_H__

	#include "xddefs.h"
	
	/***********************************************************************************************
	*
	* Function Name:  SerialPortInit()
	* Purpose:        初始化串口。
	* Inputs:         None
	* Outputs:        None           
	* Return Values:  TRUE								初始化成功。
					  FALSE								初始化失败。
	* Comments:  	  Baudrate		11520
					  Data Bits		8
					  Stop Bits		1
					  Flow Control	No
					  Parity		No
	* See Also:       None
	*
	************************************************************************************************/
	bool SerialPortInit(void);
	
	/***********************************************************************************************
	*
	*  Function Name:  SerialReadByte()
	*  Purpose:        从串口读取一个字节。
	*  Inputs:         timeout							超时时间，单位为毫秒，0表示没有超时。
	*  Outputs:        pOneByte							保存读取的数据。
	*  Return Values:  TRUE								初始化成功。
					   FALSE							初始化失败。
	*  Comments:       None
	*  See Also:       SerialWriteByte()
	*
	************************************************************************************************/
	bool SerialReadByte(UINT8 * pOneByte, UINT32 timeout);
	
	/***********************************************************************************************
	*
	* Function Name:  SerialWriteByte()
	* Purpose:        向串口写一个字节。
	* Inputs:         oneByte							将被发送的数据。
					  timeout							超时时间，单位为毫秒，0表示没有超时。
	* Outputs:        None
	* Return Values:  TRUE								初始化成功。
					  FALSE								初始化失败。
	* Comments:       None
	* See Also:       SerialReadByte()
	*
	************************************************************************************************/
	bool SerialWriteByte(UINT8  oneByte, UINT32 timeout);
	
	/***********************************************************************************************
	*
	*  Function Name:  SerialClearLine()
	*  Purpose:        从串口读取一个字节。
	*  Inputs:         none							
	*  Outputs:        none							
	*  Return Values:  TRUE								初始化成功。
					   FALSE							初始化失败。
	*  Comments:       None
	*  See Also:       SerialReadByte()
	*
	************************************************************************************************/
	bool SerialClearLine(void);

	/***********************************************************************************************
	*
	*  Function Name:  MsDelay()
	*  Purpose:        延时1ms
	*  Inputs:         times
	*  Outputs:        None
	*  Return Values:  None
	*  Comments:       None
	*  See Also:       None
	*
	************************************************************************************************/
	void MsDelay(UINT32 times);

#endif 

