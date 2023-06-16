#include "eqgpio.h"

int EQGPIO::init()
{
    int state=DIOinitialization();
    Sleep(500);
    if(state==0)
    {
        state = OpenComPort(5);
        if(state == 0)
        {
            Sleep(50);
            state = SetNPNorPNP(1);
            if(state != 0)
            {
                 //failed
            }
        }
        else
        {
            //failed
        }
    }
    return  state;
}

EQGPIO::~EQGPIO()
{
    CloseComPort();
    DIOUnInitialization();
}
//华汉不用设置方向，输入输出是分开的单独的pin角， 打开OpenComPort(5)口就行;
int EQGPIO::setDirection(unsigned char iPin, unsigned int idTypeVal, unsigned int setVal)
{
    return  0;
}

int EQGPIO::setLevel(unsigned char iPin, unsigned int idTypeVal, unsigned int setVal)
{
    int state= SetLevel( iPin,  setVal);
    return  state;
}

EQGPIO::EQGPIO()
{

    m_OutPort0 = 0;
    m_OutPort1 = 0;
   // DSDEBUG<<"gpio start init";
   if(init())
   {
      // DSDEBUG<<"GPIO 初始化失败!";
   }
    // DSDEBUG<<"gpio start init  suc!";
}


int EQGPIO::DIOinitialization()
{
    if(ERRORCODE_SUCESS !=  g_ltgFunction.init())
    {
        return -2;
    }

    if (ERRORCODE_SUCESS != g_ltgFunction.LTGInit())
    {
        return -1;
    }

    return 0;
}

int EQGPIO::DIOUnInitialization()
{
    if (ERRORCODE_SUCESS != g_ltgFunction.LTGUninit())
    {
        return -1;
    }
    return 0;
}


int EQGPIO::OpenComPort(int portnumber)
{
    int status = g_ltgFunction.LTGOpenCOMPort(portnumber);
    if (ERRORCODE_SUCESS != status)
    {
        return -1;
    }
    return 0;
}

int EQGPIO::CloseComPort()
{
    if (ERRORCODE_SUCESS == g_ltgFunction.LTGCloseCOMPort())
    {
        return 0;
    }
    return -1;
}

int EQGPIO::SetNPNorPNP(int n)
{
    BYTE mode_switch;
    switch(n)
    {
    case 0:
        mode_switch = MODE_PNP;
        break;
    case 1:
        mode_switch = MODE_NPN;
        break;
    default:
        return -2;
    }


    if(ERRORCODE_SUCESS == g_ltgFunction.LTGNPNPNPSet(mode_switch))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int EQGPIO::SetLevel(int n, int level)
{
    switch (n+1)
    {
    case 1:		// port 1
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x01;
        else
            m_OutPort0 = m_OutPort0 & 0xFE;
        break;
    case 2:		// port 2
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x02;
        else
            m_OutPort0 = m_OutPort0 & 0xFD;
        break;
    case 3:		// port 3
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x04;
        else
            m_OutPort0 = m_OutPort0 & 0xFB;
        break;
    case 4:		// port 4
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x08;
        else
            m_OutPort0 = m_OutPort0 & 0xF7;
        break;
    case 5:		// port 5
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x10;
        else
            m_OutPort0 = m_OutPort0 & 0xEF;
        break;
    case 6:		// port 6
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x20;
        else
            m_OutPort0 = m_OutPort0 & 0xDF;
        break;
    case 7:		// port 7
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x40;
        else
            m_OutPort0 = m_OutPort0 & 0xBF;
        break;
    case 8:		// port 8
        if (0x00 == level)
            m_OutPort0 = m_OutPort0 | 0x80;
        else
            m_OutPort0 = m_OutPort0 & 0x7F;
        break;
    default:
        // Do nothing
        break;
    }

    if(ERRORCODE_SUCESS == g_ltgFunction.LTGDoCtrlWriteBit(m_OutPort0, m_OutPort1))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

