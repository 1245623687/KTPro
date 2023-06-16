#include "ciobase.h"
#include"advantenchgpio.h"
#include"AAEONGPIO.h"
#include"eqgpio.h"
#include"dsdebug.h"
#include"ced_bc09_2a_gpio.h"
#include"fitgpio.h"
#include"iodaqnavi.h"
std::mutex CIOBase::m_Mutex;
CIOBase* CIOBase::m_Instance = nullptr;

CIOBase::CIOBase()
{

}


CIOBase* CIOBase::getInstance(ENUMIOTYPE type)
{
    m_Mutex.lock();
    if (m_Instance == nullptr)
    {
        switch (type)
        {
        case ENUMIOTYPE_YANHUAGPIO:
            m_Instance=new AdvantenchGPIO();
            break;
        case ENUMIOTYPE_AAEONGPIO:
            m_Instance=new AAEONGPIO;
            break;

        case ENUMIOTYPE_HUAHANGPIO:
            m_Instance=new EQGPIO;
            break;
        case ENUMIOTYPE_CED_BC09_2A_GPIO:
            m_Instance=new CED_BC09_2A_GPIO;
            break;
        case ENUMIOTYPE_FIT_GPIO:
            m_Instance=new Fitgpio;
            break;
        case ENUMIOTYPE_YANHUAHiGPIO:
            m_Instance=new IODAQNavi;
            break;
        }
    }
    m_Mutex.unlock();
    return m_Instance;
}
