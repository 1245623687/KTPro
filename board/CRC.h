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
* File Name  :		 CRC.h
*
* Description:		 CRC module header file.
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

#ifndef CRC_H
	#define CRC_H

	unsigned short int CRC16(unsigned char * pSendBuf, unsigned short int nLen);	/*获得CRC校验码*/

#endif 
