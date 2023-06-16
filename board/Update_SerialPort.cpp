
#include "Update_SerialPort.h"
#include "SerialCom.h"

#define TIMEOUT_PER_OPERATION	10	// ms

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
bool SerialPortInit(void)
{
	return TRUE;
}


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
bool SerialReadByte(UINT8 * pOneByte, UINT32 timeout)
{
	UINT32		i;
	UINT32		retries;
	bool		status;

	retries = timeout / TIMEOUT_PER_OPERATION;

	i = 0;
	do
	{
		status = ReceiveByte(pOneByte);

		if (status == TRUE)
		{
			break;
		}
		else
		{
			i++;
		}
	}while (i < retries);

	return status;
}


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
bool SerialWriteByte(UINT8  oneByte, UINT32 timeout)
{
	UINT32		i;
	UINT32		retries;
	bool		status;

	retries = timeout / TIMEOUT_PER_OPERATION;

	i = 0;
	do
	{
		status = SendByte(oneByte);

		if (status == TRUE)
		{
			break;
		}
		else
		{
			i++;
		}
	}while (i < retries);

	return status;
}


/***********************************************************************************************
*
*  Function Name:  SerialClearLine()
*  Purpose:        从串口读取一个字节。
*  Inputs:         timeout							超时时间，单位为毫秒，0表示没有超时。
*  Outputs:        pOneByte							保存读取的数据。
*  Return Values:  TRUE								初始化成功。
				   FALSE							初始化失败。
*  Comments:       None
*  See Also:       SerialWriteByte()
*
************************************************************************************************/
bool SerialClearLine(void)
{
	UINT8		oneByte;
	UINT16		i;
	UINT16		retries;
	bool		status;

	retries = 65530;

	i = 0;
	do
	{
		status = ReceiveByte(&oneByte);

		if (status == FALSE)
		{
			break;
		}
		else
		{
			i++;
		}
	}while (i < retries);

	return status;
}


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
void MsDelay(UINT32 times)
{
	volatile UINT32 i;
	UINT32 j;

	j = 0;
	while (j < times)
	{
		i = 0;
		while (i < 200000)
		{
			i++;
		}

		j++;
	}

	return;
}
