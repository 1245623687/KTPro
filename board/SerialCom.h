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
* Description:		 声明串行通讯模块.
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
#ifndef SERIAL_COM_H
	#define SERIAL_COM_H

	#ifdef __cplusplus
	extern "C"
	{
	#endif //end of __cplusplus

		#include <stdio.h>
		#include <stdlib.h>
		
		#include "windows.h"

		/******************************************************
		*
		* Function Name: OpenPort      
		* Purpose:       Open the serial port as a file 
						 and initialize the opened serial port.       
		* Inputs:        1) int nPort, the ID of the serial port, 1~2
						 2) int nBaud, current baud rate       
		* Outputs:       none       
		* Return Value:  TRUE, success to open serial port
						 FALSE, fail to open serial port
		*
		*******************************************************/
		bool OpenPort(int nPort, int nBaud);
		
		/*****************************************************
		*
		* Function Name: ClosePort        
		* Purpose:       Close the serial port.        
		* Inputs:        none       
		* Outputs:       none       
		* Return Value:  TRUE, success to close serial port
						 FALSE, fail to close serial port
		* Comment:	     10ms timeout
		*
		*******************************************************/
		bool ClosePort();
		
		/*****************************************************
		*
		* Function Name: SendByte        
		* Purpose:       Transmit a byte through the serial port.       
		* Inputs:        1) unsigned char byteData, the byte to send       
		* Outputs:       none
		* Return Value:  TRUE, success to send the byte
						 FALSE, fail to send the byte
		* Comment:	     10ms timeout
		*
		*****************************************************/
		bool SendByte(unsigned char byteData);
		
		/*****************************************************
		*
		* Function Name: ReceiveByte       
		* Purpose:       Receive a byte through the serial port.       
		* Inputs:        1) unsigned char * pByte
		* Outputs:       none       
		* Return Value:  TRUE, success to receive a byte
						 FALSE, fail to receive a byte
		*
		*****************************************************/
		bool ReceiveByte(unsigned char * pByte);
		
		/*****************************************************
		*
		* Function Name: SerialReset()
		* Purpose:       Reset the serial port.        
		* Inputs:        none
		* Outputs:       none       
		* Return Value:  TRUE
						 FALSE
		*
		*****************************************************/
		bool SerialReset(void);


	#ifdef __cplusplus
	}
	#endif //end of __cplusplus

#endif 

