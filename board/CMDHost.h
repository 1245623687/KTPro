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
* File Name  :		 CMDHost.h
*
* Description:		 CMDHost module header file.
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
#ifndef CMDHOST_H
	#define CMDHOST_H
	
	#define BV(bit)             (1 << (bit)) 
	#define SetBit(val,bit)     (val |=  (1<<(bit)))   // (1<<(bit))								
    #define ClrBit(val,bit)     (val &= ~(1<<(bit)))
    #define GetBit(val,bit)     (val &   (1<<(bit)))   //用逻辑真假判断

	#define TIMER1	1
	#define TIMER2	2

	#define PACKET_BUF_SIZE			(32)
	#define UPLOAD_DATA_LEN			ADDR_FOR_UPLOAD_COUNT
	#define DOWNLOAD_DATA_LEN		ADDR_FOR_DOWNLOAD_COUNT


	/* 定义数据地址 */
    enum
    {
        ADDR_FOR_MCU_SAVE_FLAG      = 0,
			
		ADDR_FOR_S_LAMP_CODE1		= 1,
        ADDR_FOR_S_LAMP_CODE2		= 2,
        ADDR_FOR_M_LAMP_CODE		= 3,
        ADDR_FOR_L_LAMP_CODE		= 4,
        ADDR_FOR_LAMP_CONTROL		= 5,
        
        ADDR_FOR_SYN1_FILTER_TIME	= 6,
        ADDR_FOR_SYN3_FILTER_TIME	= 7,		
        ADDR_FOR_LAMP_ON_TIMESLICE	= 8,
        ADDR_FOR_VALVE_ON_TIMESLICE	= 9,
        ADDR_FOR_EXPOSAL_TIMESLICE	= 10,
        ADDR_FOR_ALARM_ON_TIMESLICE	= 11,
        ADDR_FOR_MATCH_AVERAGE		= 12,
        ADDR_FOR_LINK_EL_COUNT		= 13,
        ADDR_FOR_ADD_FILTER			= 14,
        ADDR_FOR_SYN3_WORK_TIMER    = 15,
        ADDR_FOR_OPEN_VALVE_CONTROL	= 16,
        ADDR_FOR_ADJUST_TIMER		= 17,
        
        ADDR_FOR_HW_TEST_MODE		= 18,
		ADDR_FOR_SW_TEST_MODE       = 19,
	
		ADDR_FOR_CRC_LBYTE          = 20,
        ADDR_FOR_CRC_HBYTE          = 21,
                
        ADDR_FOR_DOWNLOAD_COUNT		= 22,   //Master下载数据地址总个数
        
        ADDR_FOR_IPORT1_STATUS      = 23,
        ADDR_FOR_IPORT2_STATUS      = 24,
        ADDR_FOR_GRAB_TIME          = 25,
        ADDR_FOR_TOBACCO_NUM        = 26,
        ADDR_FOR_LOW_LAST_TIME      = 27,
        ADDR_FOR_SHIFT_TIME         = 28,
        ADDR_FOR_PROCESS_TIME		= 29,
        ADDR_FOR_DISTURB_COUNT		= 30,
        ADDR_FOR_TOBLINK_COUNT		= 31,
            
        ADDR_FOR_UPLOAD_COUNT		= 32	//上载数据地址总个数
    };


#endif


