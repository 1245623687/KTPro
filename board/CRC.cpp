
#include "CRC.h"

/****************************************************************************
*
* 获得CRC校验码 
*
******************************************************************************/
unsigned short int CRC16(unsigned char * pSendBuf, unsigned short int nLen)
{ 
    register unsigned char i,j;
    unsigned short int wCrc = 0xFFFF;
    for(i=0; i<nLen; i++)
    {
        wCrc ^= (unsigned short int)(pSendBuf[i]);
        for(j=0; j<8; j++)
        {
            if( (wCrc&0x0001) == 1 )
            {
                wCrc >>= 1; 
                wCrc ^= 0xA001; 
            }
            else
                wCrc >>= 1; 
        }
    }
    return wCrc;
}
