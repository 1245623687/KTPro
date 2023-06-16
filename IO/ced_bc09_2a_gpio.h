#ifndef CED_BC09_2A_GPIO_H
#define CED_BC09_2A_GPIO_H

#include"ciobase.h"
#include"ced_bc09_2a_fun.h"


// Error code definition
#define	ERRORCODE_SUCESS						0x0000
#define ERRORCODE_FAILURE						0x0001
#define ERRORCODE_MCU_RESET_FAILURE				0x0002
#define ERRORCODE_PARAMETER_RESET_FAILURE		0x0003
#define ERRORCODE_INCORRECT_PORT_NUMBER			0x0004
#define ERRORCODE_INCORRECT_PARAMETER			0x0005
#define ERRORCODE_REENTRY_FAILURE				0x0006
#define ERRORCODE_OUTPUT_NPNPNP_SET_FAILURE		0x0007 //2021.01.25
#define	ERRORCODE_COM_OPEN_FAILURE				0x1001
#define ERRORCODE_COM_CLOSE_FAILURE				0x1002
#define ERRORCODE_COM_LEN_FAILURE				0x1003
#define ERRORCODE_COM_RX_FAILURE				0x1004
#define ERRORCODE_COM_TX_FAILURE				0x1005
#define ERRORCODE_COM_NOT_OPEN_FAILURE			0x1006
#define ERRORCODE_COM_READ_DATA_FAILURE			0x1007
#define ERRORCODE_COM_WRITE_CMD_FAILURE			0X1008
#define ERRORCODE_DIO_READ_DO_FAILURE			0x2001
#define ERRORCODE_DIO_WRITE_DO_FAILURE			0x2002
#define ERRORCODE_DIO_READ_DI_FAILURE			0x2003
#define ERRORCODE_LED_READ_FAILURE				0x3001
#define ERRORCODE_LED_WRITE_FAILURE				0x3002
#define ERRORCODE_PWM_READ_FAILURE				0x3003
#define ERRORCODE_PWM_WRITE_FAILURE				0x3004
#define ERRORCODE_PULSE_READ_FAILURE			0x3005
#define ERRORCODE_PULSE_WRITE_FAILURE			0x3006
#define ERRORCODE_INPUT_READ_FAILURE			0x3007
#define ERRORCODE_INPUT_WRITE_FAILURE			0x3008
#define ERRORCODE_FAST_INPUT_READ_FAILURE		0x4001
#define ERRORCODE_FAST_INPUT_WRITE_FAILURE		0x4002

#define MODE_ON									0x01
#define MODE_OFF								0x02
#define TRIGGER_RISING							0x01
#define TRIGGER_FALLING							0x02
#define MODE_FAST							    0x01
#define MODE_NORMAL							    0x02
#define MODE_PNP								0x01
#define MODE_NPN								0x02

class CED_BC09_2A_GPIO:public CIOBase
{
public:
    CED_BC09_2A_GPIO();

    friend class CIOBase;
    int init();
 ~ CED_BC09_2A_GPIO();

    int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);

    int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);


private:
        CED_BC09_2A_Fun m_cedbc092aFun;

        unsigned int m_OutPort0;
        unsigned int m_OutPort1;

};

#endif // CED_BC09_2A_GPIO_H
