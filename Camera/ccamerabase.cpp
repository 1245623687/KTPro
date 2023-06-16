#include "CCameraBase.h"


unsigned int CCameraBase::m_AllOpenedNum=0;

CCameraBase::CCameraBase()
{

}


CCameraBase::CCameraBase(const CCameraBase &obj)
{
    this->m_szIPAddress=obj.m_szIPAddress;
    this->m_iGain=obj.m_iGain;
    this->m_iRGain=obj.m_iRGain;
    this->m_iGGain=obj.m_iGGain;
    this->m_iBGain=obj.m_iBGain;
    this->m_iExposureTime=obj.m_iExposureTime;
    this->m_bColorCamera=obj.m_bColorCamera;
    this->m_nCurCameraIndex=obj.m_nCurCameraIndex;
}

CCameraBase::~CCameraBase()
{

}


CCameraBase& CCameraBase::operator=(const CCameraBase &another)
{

    switch (static_cast<ENUM_CAMERA_TYPE>(another.GetObjType()))
    {
    //case ENUM_CAMERA_TYPE_P2VISION_CVA11:
    //	break;
    case ENUM_CAMERA_TYPE_HKUSB:
        if (this == &another) return *this;

        return *this;
    case ENUM_CAMERA_TYPE_MER2:
        if (this == &another) return *this;
        return *this;

    }
}

bool CCameraBase::operator==(const CCameraBase &another) const
{
    return true;
}

bool CCameraBase::operator!=(const CCameraBase &another) const
{
    return true;
}

//! White balance
void CCameraBase::GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain) 
{

}

void CCameraBase::SetWhiteBalance(int iRGain, int iGGain, int iBGain)
{

}

void CCameraBase::GetWhiteBalanceRange(int &iMin, int &iMax) 
{

}




