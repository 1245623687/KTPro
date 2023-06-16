#include <iostream>
#include "Xmodem.h"
#include "SerialCom.h"
#include "CMDHost.h"
int main111()
{
	UINT8 packetBuf[PACKET_BUF_SIZE];
	memset(packetBuf, 0, PACKET_BUF_SIZE * sizeof(UINT8));
	OpenPort(1, 115200);
	XModemCommEngine(SLAVE1_CODE, R_CODE, UPLOAD_DATA_LEN, packetBuf);
	packetBuf[1] = 255;
	XModemCommEngine(SLAVE1_CODE, W_CODE, DOWNLOAD_DATA_LEN, packetBuf);
	ClosePort();
	return 0;
}


