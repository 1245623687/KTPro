#include "ced_bc09_2a_gpio.h"

CED_BC09_2A_GPIO::CED_BC09_2A_GPIO()
{
    m_OutPort0 = 0;
    m_OutPort1 = 0;

    init();
}

int CED_BC09_2A_GPIO::init()
{
    int ret=0;
    ret=m_cedbc092aFun.init();

	if (!m_cedbc092aFun.CED_BC09_2A_InitIO())
	{
		ret = 1;
	}
    ret=m_cedbc092aFun.CED_BC09_2A_SetPortVal(0xA07, 0x00, 1);

    return  ret;
}

CED_BC09_2A_GPIO::~CED_BC09_2A_GPIO()
{
    m_cedbc092aFun.CED_BC09_2A_ShutDownIO();
}

int CED_BC09_2A_GPIO::setDirection(unsigned char iPin, unsigned int idTypeVal, unsigned int setVal)
{
    return  0;
}

//pin 5678固定为输出，pin1234固定为输入

//level 0x00板子灯亮，其他板子灯灭
int CED_BC09_2A_GPIO::setLevel(unsigned char iPin, unsigned int idTypeVal, unsigned int level)
{
    int ret=0;
    switch (iPin+1)
    {
    case 5:		// port 5
        if (0xFF == level)
            //设置针脚高电压 针脚5设置1 针脚输出高电压
            //针脚输出高电压板子灯灭，输出低电压板子灯亮
            m_OutPort0 = m_OutPort0 | 0x10;
        else
            m_OutPort0 = m_OutPort0 & 0xEF;
        break;
    case 6:		// port 6
        if (0xFF == level)
            m_OutPort0 = m_OutPort0 | 0x20;
        else
            m_OutPort0 = m_OutPort0 & 0xDF;
        break;
    case 7:		// port 7
        if (0xFF == level)
            m_OutPort0 = m_OutPort0 | 0x40;
        else
            m_OutPort0 = m_OutPort0 & 0xBF;
        break;
    case 8:		// port 8
        if (0xFF == level)
            m_OutPort0 = m_OutPort0 | 0x80;
        else
            m_OutPort0 = m_OutPort0 & 0x7F;
        break;
    default:
        // Do nothing
        break;
    }


    if(ERRORCODE_SUCESS == m_cedbc092aFun.CED_BC09_2A_SetPortVal(0xA07, m_OutPort0,1))
    {
        ret= 0;
    }
    else
    {
        ret= -1;
    }
    return  ret;
}
