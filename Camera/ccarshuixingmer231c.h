#ifndef CCARSHUIXINGMER231C_H
#define CCARSHUIXINGMER231C_H
#include<string>
#include<iostream>
#include <mutex>
#include "ccamerabase.h"
#include"GxIAPI.h"

class cCarShuixingMer231C:public CCameraBase
{

public:
    cCarShuixingMer231C();

    int loadSetting(std::string filePath);

    //! Operator overload
    CCameraBase& operator=(const CCameraBase &another);
    bool operator ==(const CCameraBase &another) const;
    bool operator !=(const CCameraBase &another) const;
    virtual CCameraBase * Clone() const;

    virtual bool Copy(const CCameraBase& obj);

    virtual int GetObjType() const;
    //-------- Open camera ---------
    virtual bool IsOpen() const;
    //! Open camera
    virtual bool Open();
    //! Close camera
    virtual void Close();

    virtual void reset();

    //! Gain
    virtual int GetGain() ;
    virtual void SetGain(int iGain);
    virtual void GetGainRange(int &iMin, int &iMax) ;
    //! Exposure
    virtual int GetExposureTime() ;
    virtual void SetExposureTime(int iExposure);
    virtual void GetExposureRange(int &iMin, int &iMax);

    //! White balance
    virtual void GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain) ;
    virtual void SetWhiteBalance(int iRGain, int iGGain, int iBGain);
    virtual void GetWhiteBalanceRange(int &iMin, int &iMax) ;


    //! Trigger
    virtual void SetTrigger(int iTriggerMode);


    //-------- Grab image ------------------
    //! Grab image
    //virtual bool Grab(IFvxImage2Ptr pFvxImage1, IFvxImage2Ptr pFvxImage2, IFvxImage2Ptr pFvxImage3) = 0;
    virtual bool GrabEx();
    virtual bool HasImage(unsigned char * pImage);

    //! Get channel camera number
    virtual int GetChannelCamNum() const;


    void triggerbySoftware();



    void clearBuf();

    ~cCarShuixingMer231C();


    void setImgHei(int ihei)
    {
        m_Height=ihei;
    }
    void setImgWid(int iWid)
    {
        m_Width=iWid;
    }

    void setBufSize(int isize)
    {

        m_BufferSize=isize;
    }

    int  getBufSize()
    {

        return   m_BufferSize;
    }

    //掉线重连处理

    bool m_bIsSnap;


//    GX_DEVICE_OFFLINE_CALLBACK_HANDLE  m_hCB;
//    CDeviceOfflineEventHandle*   m_pDeviceOfflineEventHandle;

    //异常在函数内被处理掉了，所以不会抛出任何异常
//    void __ProcessOffline() throw();
//    void __Recovery() throw();

    // 采集回调函数
    static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

    //掉线回调
    static void GX_STDC OnDeviceOfflineCallbackFun(void* pUserParam);

   static void GX_STDC OnDeviceAutoConnectCallBack(GX_AUTO_CONNECTION_STATUS_ENTRY nConnectStatus, void *pUserParam);



//protected:
//    void DoOnImageCaptured(CImageDataPointer&objImageDataPointer, void* pUserParam);

private:
    //! Opened camera count
    static unsigned int m_nOpenedNum;
    bool m_IsOpen;



    unsigned int m_Height;//图片高度
    unsigned int m_Width;//图片宽度
    unsigned int m_BufferSize;//图片的内存占用 字节数


    unsigned char * m_Buf;
    //std::queue<unsigned char *> m_BufArr;
    std::mutex m_Mutex;
    std::condition_variable m_Cond;


    int camCount = 0;

private:
    GX_DEV_HANDLE m_hDevice;
    GX_FEATURE_CALLBACK_HANDLE m_hCB;

//    CGXDevicePointer ObjDevicePtr;
//    CGXStreamPointer ObjStreamPtr;
//    CGXFeatureControlPointer ObjFeatureControlPtr;



    //CGXFeatureControlPointer objStreamFeatureControlPtr;
private:
    class GC
    {
    public:
        ~GC();
    };
    static GC gc;




};

#endif // CCARSHUIXINGMER231C_H
