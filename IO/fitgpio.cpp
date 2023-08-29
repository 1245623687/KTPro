#include "fitgpio.h"

Fitgpio::Fitgpio()
{
    init();
}

int Fitgpio::init()
{
    int ret=0;
    m_cnt=0;
    hinstLib = LoadLibrary(TEXT("FitSdk_x64.dll"));
    if (hinstLib == NULL)
    {
        DSDEBUG<<"Load fail FitSdk_x64.dll"<<endl;
        printf("Load fail FitSdk_x64.dll\n");
        return 1;
    }

    ProcAdd = (SETVOIDPROC) GetProcAddress(hinstLib, "GetNowLPCICID");
    if (NULL != ProcAdd)
    {
        unsigned int IcId = (*ProcAdd)();
        printf("Read ID: %X\n", IcId);
    }
    else
    {
        DSDEBUG<<"GetNowLPCICID failed"<<endl;
        return 1;
    }
    GPIO_Out = (SETUCHAR2PROC)GetProcAddress(hinstLib, "SetLPCGpioOutputDataIndividual");
    GPIO_Int = (GETUCHAR2PROC)GetProcAddress(hinstLib, "GetLPCGpioStatusIndividual");
    if (NULL == GPIO_Out||NULL == GPIO_Int)
    {
         DSDEBUG<<"GPIO_Out or GPIO_Int failed"<<endl;
        return 1;
    }
    return ret;
}

Fitgpio::~Fitgpio()
{

}

int Fitgpio::setDirection(unsigned char iPin, unsigned int idTypeVal, unsigned int setVal)
{
    return  0;
}

int Fitgpio::setLevel(unsigned char iPin, unsigned int idTypeVal, unsigned int setVal)
{

//    if(iPin==0)
//        m_cnt++;
    int val=setVal==0xFF?0:1;

    if (!(*GPIO_Out)(iPin, val))
    {
//        if(iPin==0)
        DSDEBUG<<QString("gpio setLevel_failed,index_%1,val_%2 ,cnt_%3").arg(iPin).arg(val).arg(m_cnt)<<endl;

        return 1;
    }
    else
    {
//        if(iPin==0)
        DSDEBUG<< QString("gpio setLevel_sucess,index_%1,val_%2,cnt_%3").arg(iPin).arg(val).arg(m_cnt)<<endl;
        return 0;
    }
}


int Fitgpio::getLevel(int iPin, int &getVal)
{
    // if(iPin==0)
    // m_cnt++;
    unsigned char value;
    if (!(*GPIO_Int)(iPin, &value))
    {
        //        if(iPin==0)
        //        DSDEBUG<<QString("gpio setLevel_failed,index_%1,val_%2 ,cnt_%3").arg(iPin).arg(val).arg(m_cnt)<<endl;
        return 1;
    }
    else
    {
        //        if(iPin==0)
        //        DSDEBUG<< QString("gpio setLevel_sucess,index_%1,val_%2,cnt_%3").arg(iPin).arg(val).arg(m_cnt)<<endl;
        getVal=value;
        return 0;
    }
}




