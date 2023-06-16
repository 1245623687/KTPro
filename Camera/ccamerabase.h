#pragma once
#include"opencv.hpp"
#include <iostream>
#include<QQueue>
#include "glog/logging.h"
#include"dsdebug.h"

typedef enum ENUM_CAMERA_TYPE
{
    ENUM_CAMERA_TYPE_MER2 = 0,
    
    ENUM_CAMERA_TYPE_HKUSB=1,

} ENUM_CAMERA_TYPE;


class  CCameraBase
{
//public:
//    class TestImage
//    {
//    public:
//        TestImage(int len)
//        {
//            Buf=new unsigned char[len];
//        }
//        ~TestImage()
//        {
//            delete [] Buf;
//        }
//        unsigned char* Buf;
//    };

public:
    CCameraBase();
    CCameraBase(const CCameraBase &obj);
    virtual void reset()=0;

    virtual ~CCameraBase();
    virtual int loadSetting(std::string filePath)=0;
    //! Operator overload
    CCameraBase& operator=(const CCameraBase &another);
    bool operator ==(const CCameraBase &another) const;
    bool operator !=(const CCameraBase &another) const;
    
    
    virtual CCameraBase * Clone() const = 0;
    virtual bool Copy(const CCameraBase& obj) = 0;
    virtual int GetObjType() const = 0;
    //-------- Open camera ---------
    virtual bool IsOpen() const = 0;
    //! Open camera
    virtual bool Open() = 0;
    //! Close camera
    virtual void Close() = 0;

    virtual void StartGrab(){};
    virtual void StopGrab(){};

    virtual int  getBufSize()
    {
        return   0;
    }


    //-------- Setting parameter -----------
    //! Set/Get IP address
    void SetIPAddress(std::string szIPAddress) { m_szIPAddress = szIPAddress; }
    std::string GetIPAddress() const { return m_szIPAddress; }

    //! Gain
    virtual int GetGain() = 0;
    virtual void SetGain(int iGain) = 0;
    virtual void GetGainRange(int &iMin, int &iMax)  = 0;
    //! Exposure
    virtual int GetExposureTime()  = 0;
    virtual void SetExposureTime(int iExposure) = 0;
    virtual void GetExposureRange(int &iMin, int &iMax)  = 0;

    //! White balance
    virtual void GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain) ;
    virtual void SetWhiteBalance(int iRGain, int iGGain, int iBGain);
    virtual void GetWhiteBalanceRange(int &iMin, int &iMax) ;


    //! Is color camera
    bool IsColorCamera() { return m_bColorCamera; }


    //! Trigger
    virtual void SetTrigger(int iTriggerMode) = 0;


    //-------- Grab image ------------------
    //! Grab image
    //virtual bool Grab(IFvxImage2Ptr pFvxImage1, IFvxImage2Ptr pFvxImage2, IFvxImage2Ptr pFvxImage3) = 0;
    virtual bool GrabEx() = 0;
    virtual bool HasImage(unsigned char * pImage) = 0;

    //! Get channel camera number
    virtual int GetChannelCamNum() const = 0;

    virtual void clearBuf()=0;


   virtual void setImgHei(int ihei)=0;
   virtual void setImgWid(int iWid)=0;
   virtual void setBufSize(int isize)=0;

    virtual void setCamIndex(int camIndex){ m_iCamIndex=camIndex;}


     bool m_bIsOffline;

    int m_iCamIndex;

protected:
    //! Camera ID
    std::string m_szIPAddress;
    //! Gain
    int m_iGain;
    int m_iRGain;
    int m_iGGain;
    int m_iBGain;
    //! Exposure time
    int m_iExposureTime;
    //! Is color camera
    bool m_bColorCamera;
    //! Current camera index
    unsigned int m_nCurCameraIndex;
    //! Opened camera count 所有类型的相机数量
    static unsigned int m_AllOpenedNum;
public:
     clock_t startTime;
public:
   QQueue<cv::Mat> m_ImageBufArr;
   // std::queue<TestImage*>  m_ImageBufArr2;
   int count=0;
};

