#include "csimucanera.h"


//std::mutex cSimuCanera::m_Mutex;

cSimuCanera::cSimuCanera()
{

    m_bExitGrab=false;
    m_iCurImgIndex=0;
}

int cSimuCanera::loadSetting(string filePath)
{
    int ret=0;

    return ret;
}

CCameraBase *cSimuCanera::Clone() const
{
    cSimuCanera *pcSimuCamera=new cSimuCanera();
    return pcSimuCamera;
}

bool cSimuCanera::Copy(const CCameraBase &obj)
{
    bool ret=true;
    return  ret;
}

int cSimuCanera::GetObjType() const
{
    int ret=0;

    return ret;
}

bool cSimuCanera::IsOpen() const
{
    bool ret=true;
    return  ret;
}

bool cSimuCanera::Open()
{
    bool ret=true;
    getAllFiles();
    return  ret;
}

void cSimuCanera::StartGrab()
{
    //    std::thread thread(runGrab);
    //    if(m_grabRunFuture.isPaused())
    //    {
    //        m_grabRunFuture.resume();
    //    }
    m_bExitGrab=false;

    if(!m_grabRunFuture.isRunning())
    {
        m_grabRunFuture=QtConcurrent::run(this,&cSimuCanera::runGrab);
    }
}

void cSimuCanera::StopGrab()
{
//    if(m_grabRunFuture.isRunning())
//    {
//        m_grabRunFuture.pause();
//    }
    m_bExitGrab=true;
    m_grabRunFuture.waitForFinished();
}

void cSimuCanera::Close()
{

}

void cSimuCanera::reset()
{

}

int cSimuCanera::GetGain()
{
    int ret=0;
    return  ret;
}

void cSimuCanera::SetGain(int iGain)
{

}

void cSimuCanera::GetGainRange(int &iMin, int &iMax)
{

}

int cSimuCanera::GetExposureTime()
{
    int ret=0;
    return  ret;
}

void cSimuCanera::SetExposureTime(int iExposure)
{

}

void cSimuCanera::GetExposureRange(int &iMin, int &iMax)
{

}

void cSimuCanera::SetTrigger(int iTriggerMode)
{

    int nRet =0;
    switch (iTriggerMode)
    {
    case 0://停止chu'fa
        qDebug()<<"停止chu'fa "<<endl;

        //        ObjStreamPtr->StopGrab();
        //        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        StopGrab();
        break;

    case 1://外部line触发

        StartGrab();
        break;
        //        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
        //        if (MV_OK != nRet)
        //        {
        //            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        //        }
        //        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0);
        //        if (MV_OK != nRet)
        //        {
        //            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        //        }
        break;
    case 2://软件chufa stop
        //        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
        //        if (MV_OK != nRet)
        //        {
        //            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        //        }
        //        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
        //        if (MV_OK != nRet)
        //        {
        //            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        //        }
        break;
    case 3://自由chufa
        // ch:设置触发模式为off | eb:Set trigger mode as off
        //        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_OFF);
        //        if (MV_OK != nRet)
        //        {
        //            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        //        }
        StartGrab();
        break;
    }
}

bool cSimuCanera::GrabEx()
{
    bool ret=true;
    return  ret;
}

bool cSimuCanera::HasImage(unsigned char *pImage)
{
    try {

        this->m_Mutex.lock();
        // DSDEBUG<<QString("%1进入获取图片,获取到锁").arg(QString::fromStdString(this->GetIPAddress()));

//        cv::Mat pTmp = m_ImageBufArr.back();
        if(m_ImageBufArr.size()>0)
        {
            cv::Mat pTmp = m_ImageBufArr.dequeue();
            memcpy(pImage, pTmp.ptr<uchar>(0), m_BufferSize);
        }
//       cv::imwrite("./pTmp.bmp",pTmp);
//       m_ImageBufArr.clear();
        this->m_Mutex.unlock();

    }
    catch (std::exception ex)
    {
        DSDEBUG << ex.what() << endl;
    }
    catch (...)
    {
        DSDEBUG << "unknown err!" << endl;
    }

    return true;
}

int cSimuCanera::GetChannelCamNum() const
{
    int ret=0;
    return  ret;
}

void cSimuCanera::triggerbySoftware()
{

}

void cSimuCanera::clearBuf()
{
    //DSDEBUG<<QString("%1进入清理函数").arg(QString::fromStdString(this->GetIPAddress()));
    this->m_Mutex.lock();
    //DSDEBUG<<QString("%1进入清理函数,获取到锁,清理缓存数量：%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());
    this->m_ImageBufArr.clear();
    this->m_Mutex.unlock();
    //DSDEBUG<<QString("%1进入清理函数,释放锁,缓存数量%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());

}

int cSimuCanera::RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight)
{
    if ( NULL == pRgbData )
    {
        return 1;
    }

    for (unsigned int j = 0; j < nHeight; j++)
    {
        for (unsigned int i = 0; i < nWidth; i++)
        {
            unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
            pRgbData[j * (nWidth * 3) + i * 3]     = pRgbData[j * (nWidth * 3) + i * 3 + 2];
            pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
        }
    }
    return 0;
}

void cSimuCanera::getAllFiles()
{
    cv::glob(m_szIPAddress,m_vecimgfiles);
}

void cSimuCanera::runGrab()
{
    while (!m_bExitGrab)
    {

        cv::Mat readMat=cv::imread(m_vecimgfiles[m_iCurImgIndex],1);
//        cv::cvtColor(readMat,readMat,cv::COLOR_RGB2BGR);
        m_iCurImgIndex++;
        m_Mutex.lock();



        m_ImageBufArr.enqueue(readMat);
        m_Mutex.unlock();

//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        QThread::msleep(1000);
        if(m_iCurImgIndex==m_vecimgfiles.size()-2)
        {
            m_iCurImgIndex=0;
        }
    }

}
