#ifndef CSIMUCANERA_H
#define CSIMUCANERA_H

#include"./ccamerabase.h"
#include"opencv.hpp"
#include<thread>

#include<mutex>
using namespace  std;
#include<QtConcurrent>


class cSimuCanera: public CCameraBase
{
public:
    cSimuCanera();

    int loadSetting(std::string filePath);
    //! Operator overload
    //    CCameraBase& operator=(const CCameraBase &another);
    //    bool operator ==(const CCameraBase &another) const;
    //    bool operator !=(const CCameraBase &another) const;



    virtual CCameraBase * Clone() const;
    virtual bool Copy(const CCameraBase& obj);


    virtual int GetObjType() const;
    //-------- Open camera ---------
    virtual bool IsOpen() const;
    //! Open camera
    virtual bool Open();

    virtual void StartGrab() ;

    virtual void StopGrab() ;

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
    //    virtual void GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain) ;
    //    virtual void SetWhiteBalance(int iRGain, int iGGain, int iBGain);
    //    virtual void GetWhiteBalanceRange(int &iMin, int &iMax) ;


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


    int RGB2BGR( unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight );

    void getAllFiles();
//     void pushQueue(cv::Mat& readMat);

      std::mutex m_Mutex;
private:
    //! Opened camera count
    static unsigned int m_nOpenedNum;
    bool m_IsOpen;

    unsigned int m_Height;//图片高度
    unsigned int m_Width;//图片宽度
    unsigned int m_BufferSize;//图片的内存占用 字节数

    unsigned char * m_Buf;


    std::condition_variable m_Cond;

    int camCount = 0;
    bool m_bExitGrab;
    int m_iCurImgIndex=0;
    QFuture<void> m_grabRunFuture;


private:

    void* m_hkHandle;
  //  MV_CC_DEVICE_INFO_LIST m_hkDeviceList;
    bool m_isGrabing;

   std:: vector<std::string> m_vecimgfiles;
    void  runGrab();

private:
    class GC
    {
    public:
        ~GC();
    };
    static GC gc;

};

#endif // CSIMUCANERA_H
