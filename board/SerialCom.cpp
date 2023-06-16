
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "windows.h"
#include "SerialCom.h"

#define PORT_NAME_LEN     4
#define DCB_BYTE_SIZE     8
  
//Serial device handle
HANDLE hComDev = INVALID_HANDLE_VALUE; 

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
bool OpenPort(int nPort, int nBaud)
{
    DCB  dcb;
    COMMTIMEOUTS commTimeouts;
    char portName[PORT_NAME_LEN + 1] = "COM0";
    
    portName[3] += nPort;

    if (hComDev != INVALID_HANDLE_VALUE)
    {
        //Error,a port has already be opened.
        return FALSE;
    }

    //Open the serial port as a file
    hComDev = CreateFileA(portName,
                         GENERIC_READ|GENERIC_WRITE, 
                         0, 
                         NULL, 
                         OPEN_EXISTING, 
                         0,//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, 
                         NULL);
    if (hComDev == INVALID_HANDLE_VALUE)
    {
        //Error,fail to open
        return FALSE;
    }

    //Setup the serial port
    //Timeout setting
    commTimeouts.ReadIntervalTimeout         = 5;

    commTimeouts.ReadTotalTimeoutConstant    = 5;
    commTimeouts.ReadTotalTimeoutMultiplier  = 5;

    commTimeouts.WriteTotalTimeoutConstant   = 5;
    commTimeouts.WriteTotalTimeoutMultiplier = 5;

    if (SetCommTimeouts(hComDev, &commTimeouts) == FALSE)
    {
        //Error,SetCommTimeouts() fail
        return FALSE;
    }

    //DCB setting
    dcb.DCBlength = sizeof(DCB);
    if (GetCommState(hComDev, &dcb) == FALSE)
    {
        //Error,GetCommState() fail
        return FALSE;
    }
    dcb.BaudRate = nBaud;
    dcb.ByteSize = DCB_BYTE_SIZE;
    dcb.fParity  = NOPARITY;
    dcb.StopBits = ONESTOPBIT; 
    if (SetCommState(hComDev, &dcb) == FALSE)
    {
        //Error,SetCommState() fail
        return FALSE;
    }

    //Serial port buffer setting
    if (SetupComm(hComDev, 1024, 1024) == FALSE)
    {
        //Error,SetupComm() fail
        return FALSE;
    }

	PurgeComm(hComDev, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

    //Finish opening the serial port
    return TRUE;
}


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
bool ClosePort()
{
    bool bState;

    if (hComDev == INVALID_HANDLE_VALUE)
    {
        bState = FALSE; 
    }
    else if (CloseHandle(hComDev) == FALSE)
    {
        bState = FALSE; 
    }
    else
    {
        bState = TRUE;
		hComDev = INVALID_HANDLE_VALUE;
    }

    return bState;
}


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
bool SendByte(unsigned char byteData)
{
    bool bState = TRUE;
    bool bWriteState;
    DWORD  dwBytesWritten;

    if (hComDev == NULL)
    {
        //Error,the serial port have not be opened
        return FALSE;
    }

    //Send the byte to the seiral port
    bWriteState = WriteFile(hComDev, &byteData, 1, &dwBytesWritten, NULL);

	if ((bWriteState == TRUE) && (dwBytesWritten == 1))
	{
		bState = TRUE;
	}
	else
	{
		bState = FALSE;
	}

    return bState;
}


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
bool ReceiveByte(unsigned char * pByte)
{
    bool bState = TRUE;
    bool bReadState;
    DWORD dwErrorFlags;
    DWORD dwBytesRead;
    COMSTAT comStat;
    
    if (hComDev == NULL)
    {
        //Error,the serial port have not be opened
        return FALSE;
    }

    if (ClearCommError(hComDev, &dwErrorFlags, &comStat) == FALSE)
    {
        //Error,ClearCommError() fail
        return FALSE;
    }
    
	bReadState = ReadFile(hComDev, pByte, 1, &dwBytesRead, NULL);
	
	if ((bReadState == TRUE) && (dwBytesRead == 1))
	{
		bState = TRUE;
	}
	else
	{
		bState = FALSE;
	}
	
    return bState;
}


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
bool SerialReset(void)
{
	return PurgeComm(hComDev, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
}
