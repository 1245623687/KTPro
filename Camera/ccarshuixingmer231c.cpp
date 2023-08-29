#include "ccarshuixingmer231c.h"

unsigned int cCarShuixingMer231C::m_nOpenedNum;

cCarShuixingMer231C::cCarShuixingMer231C()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    //初始化库
    status = GXInitLib();
    if (status!= GX_STATUS_SUCCESS)
    {
        //    return 0;
    }
    m_hDevice=NULL;
    m_bIsOffline=false;
    m_bIsSnap=false;
    m_IsOpen=false;
}

int cCarShuixingMer231C::loadSetting(std::string filePath)
{

    if (!this->IsOpen())
    {
        /*LOG(ERROR) << "camera has not opened!" << endl;*/
        return -1;
    }

    return 0;
}

CCameraBase& cCarShuixingMer231C::operator=(const CCameraBase &another)
{
    if (this == &another)
    {
        return *this;
    }
}

bool cCarShuixingMer231C::operator ==(const CCameraBase &another) const
{
    if (another.GetIPAddress() != this->GetIPAddress()) return false;
    return true;
}


bool cCarShuixingMer231C::operator !=(const CCameraBase &another) const
{
    if (another.GetIPAddress() == this->GetIPAddress()) return false;
    return true;
}

CCameraBase *cCarShuixingMer231C::Clone() const
{
    //用到了再补充
    cCarShuixingMer231C * ret = new cCarShuixingMer231C;
    return  ret;
}

bool cCarShuixingMer231C::Copy(const CCameraBase &obj)
{
    return true;
}

int cCarShuixingMer231C::GetObjType() const
{
    return static_cast<int>(ENUM_CAMERA_TYPE_MER2);
}

bool cCarShuixingMer231C::IsOpen() const
{
    return  m_IsOpen;
}

bool cCarShuixingMer231C::Open()
{
    // TODO: Add your control notification handler code here
    GX_STATUS      emStatus     = GX_STATUS_SUCCESS;
    uint32_t       nDeviceNum   = 0;
    int64_t        nPixelFormat = 0;
    GX_OPEN_PARAM  stOpenParam;

    // 枚举设备个数
    emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);
    // 判断当前连接设备个数
    if (nDeviceNum <= 0)
    {
        return  false;
    }

    // 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
    if (m_hDevice != NULL)
    {
        emStatus = GXCloseDevice(m_hDevice);

        m_hDevice = NULL;
    }

    // 打开枚举列表中的第一台设备
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode   = GX_OPEN_SN;
    char* sn=_strdup( GetIPAddress().c_str());

    stOpenParam.pszContent = sn;
    emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);

    if(emStatus!=GX_STATUS_SUCCESS)
    {
        return  false;
    }


    // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
    // 以提高网络相机的采集性能,设置方法参考以下代码。
    {
        bool	 bImplementPacketSize = false;
        uint32_t unPacketSize		  = 0;

        // 判断设备是否支持流通道数据包功能
        emStatus = GXIsImplemented(m_hDevice, GX_INT_GEV_PACKETSIZE, &bImplementPacketSize);
        //            GX_VERIFY(emStatus);

        if (bImplementPacketSize)
        {
            // 获取当前网络环境的最优包长值
            emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
            //                GX_VERIFY(emStatus);
            // 将最优包长值设置为当前设备的流通道包长值
            emStatus = GXSetInt(m_hDevice, GX_INT_GEV_PACKETSIZE, unPacketSize);
            //                GX_VERIFY(emStatus);
        }
    }

    //掉线重连功能打开
    GXSetBool(m_hDevice, GX_DEV_BOOL_ENABLE_AUTOCONNECT, false);

//    emStatus = GXRegisterDeviceAutoConncetCallback(m_hDevice, this, OnDeviceAutoConnectCallBack);

     emStatus =GXRegisterDeviceOfflineCallback(m_hDevice, this,OnDeviceOfflineCallbackFun, &m_hCB);

    // 注册采集回调函数
    emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);

    m_bIsOffline=false;
    m_IsOpen = true;

    return  true;
}

void cCarShuixingMer231C::Close()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    if (!m_IsOpen) return;

    SetTrigger(0);
    // 注销采集回调函数
    emStatus = GXUnregisterCaptureCallback(m_hDevice);


   emStatus = GXUnregisterDeviceOfflineCallback(m_hDevice,m_hCB);

    //释放资源

    // 关闭相机
    emStatus = GXCloseDevice(m_hDevice);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        // 错误处理
    }
    m_hDevice = NULL;
    m_IsOpen = false;

    cCarShuixingMer231C::m_nOpenedNum--;
    cCarShuixingMer231C::m_AllOpenedNum--;
    while (m_ImageBufArr.size() > 0)
    {
        m_ImageBufArr.dequeue();
    }
}

void cCarShuixingMer231C::reset()
{

}

int cCarShuixingMer231C::GetGain()
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    double dValue = 0;
    status = GXGetFloat(m_hDevice, GX_FLOAT_GAIN, &dValue);

    return  dValue;
}

void cCarShuixingMer231C::SetGain(int iGain)
{
    if(!m_IsOpen) return;

    GX_STATUS status = GX_STATUS_SUCCESS;
    status =GXSetFloat( m_hDevice, GX_FLOAT_GAIN,  iGain);
}

void cCarShuixingMer231C::GetGainRange(int &iMin, int &iMax)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    GX_FLOAT_RANGE stFloatRange;
    status =GXGetFloatRange(m_hDevice, GX_FLOAT_GAIN,&stFloatRange);

    iMin=stFloatRange.dMin;
    iMax=stFloatRange.dMax;



}

int cCarShuixingMer231C::GetExposureTime()
{


    GX_STATUS status = GX_STATUS_SUCCESS;
    double dValue = 0;
    status = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &dValue);

    return  dValue;
}

void cCarShuixingMer231C::SetExposureTime(int iExposure)
{
    if(!m_IsOpen) return;

    GX_STATUS status = GX_STATUS_SUCCESS;
    status =GXSetFloat( m_hDevice, GX_FLOAT_EXPOSURE_TIME,  iExposure);

}

void cCarShuixingMer231C::GetExposureRange(int &iMin, int &iMax)
{
    GX_STATUS status = GX_STATUS_SUCCESS;
    GX_FLOAT_RANGE stFloatRange;
    status =GXGetFloatRange(m_hDevice, GX_FLOAT_EXPOSURE_TIME,&stFloatRange);

    iMin=stFloatRange.dMin;
    iMax=stFloatRange.dMax;

}

void cCarShuixingMer231C::GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain)
{

}

void cCarShuixingMer231C::SetWhiteBalance(int iRGain, int iGGain, int iBGain)
{

}

void cCarShuixingMer231C::GetWhiteBalanceRange(int &iMin, int &iMax)
{

}

void cCarShuixingMer231C::SetTrigger(int iTriggerMode)
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    switch (iTriggerMode)
    {
    case 0://停止chu'fa
        if(m_bIsSnap==true)
        {
            emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
            m_bIsSnap=false;
        }
          break;

    case 1://外部line触发
        if(m_bIsSnap==false)
        {
            emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
            m_bIsSnap=true;
        }
        break;

    case 2://软件chufa
        if(m_bIsSnap==false)
        {
            emStatus = GXSetEnum(m_hDevice,GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
            emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
        }
              break;


    }
}

bool cCarShuixingMer231C::GrabEx()
{
    return true;
}

bool cCarShuixingMer231C::HasImage(unsigned char *pImage)
{
    try {

        this->m_Mutex.lock();
        //        DSDEBUG__<<QString("%1进入获取图片,获取到锁").arg(QString::fromStdString(this->GetIPAddress()));
        //        LOG(INFO)<<QString("%1进入获取图片,获取到锁").arg(QString::fromStdString(this->GetIPAddress())).toStdString()<<endl;

        //unsigned char * pTmp=m_ImageBufArr.dequeue();
        cv::Mat pTmp = m_ImageBufArr.back();
        memcpy(pImage, pTmp.ptr<uchar>(0), m_BufferSize);
        m_ImageBufArr.clear();
        this->m_Mutex.unlock();
        //        DSDEBUG__<<QString("%1进入获取图片,释放锁").arg(QString::fromStdString(this->GetIPAddress()));
        //        LOG(INFO)<<QString("%1进入获取图片,释放锁").arg(QString::fromStdString(this->GetIPAddress())).toStdString()<<endl;

    }
    catch (std::exception ex)
    {
        //LOG(INFO)<<"HasImage exception"<<endl;

        /*	DSDEBUG << ex.what() << endl;*/
    }
    catch (...)
    {
        //	DSDEBUG << "unknown err!" << endl;
    }

    return true;
}

int cCarShuixingMer231C::GetChannelCamNum() const
{
    return m_nCurCameraIndex;
}

void cCarShuixingMer231C::triggerbySoftware()
{
    int sumRes = 0;
}

void cCarShuixingMer231C::clearBuf()
{
    //DSDEBUG<<QString("%1进入清理函数").arg(QString::fromStdString(this->GetIPAddress()));
    this->m_Mutex.lock();
    this->m_ImageBufArr.clear();
    this->m_Mutex.unlock();
    //DSDEBUG<<QString("%1进入清理函数,释放锁,缓存数量%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());

}

cCarShuixingMer231C::~cCarShuixingMer231C()
{
    if (this->IsOpen())
    {
        Close();
    }
    GXCloseLib();
    //    if(m_Buf!=nullptr)
    //    {
    //        delete []  m_Buf;
    //        m_Buf=nullptr;
    //    }
}

void cCarShuixingMer231C::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
    cCarShuixingMer231C *pCarShuixingMer231C= (cCarShuixingMer231C*)(pFrame->pUserParam);

    cv::Mat tmp = cv::Mat::zeros(pFrame->nHeight, pFrame->nWidth, CV_8UC1);
    memcpy(tmp.ptr<uchar>(0), pFrame->pImgBuf, pFrame->nImgSize);


//     cv::cvtColor(tmp, tmp, cv::COLOR_BayerRG2BGR);
      cv::cvtColor(tmp, tmp, cv::COLOR_BayerRG2RGB);

    //    std::string imgName=(this->m_szIPAddress)+"_.bmp";
    //    cv::imwrite(imgName,tmp);

    pCarShuixingMer231C->m_Mutex.lock();
    pCarShuixingMer231C->m_ImageBufArr.enqueue(tmp);
    DSDEBUG__ <<" in m_CamBufArr.SIZE:" <<endl;
    //    LOG(INFO)<<QString::fromStdString(m_szIPAddress).toStdString()<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size()<<endl;
    pCarShuixingMer231C->m_Mutex.unlock();
    //    DSDEBUG__ <<QString::fromStdString(m_szIPAddress)<< "   out m_BufArr.SIZE:" << m_ImageBufArr.size();
    //    LOG(INFO)<<QString::fromStdString(m_szIPAddress).toStdString()<< "   out m_BufArr.SIZE:" << m_ImageBufArr.size()<<endl;

}

void cCarShuixingMer231C::OnDeviceOfflineCallbackFun(void *pUserParam)
{
    cCarShuixingMer231C *pCarShuixingMer231C= (cCarShuixingMer231C*)pUserParam;
    pCarShuixingMer231C->m_bIsOffline=true;

}

void cCarShuixingMer231C::OnDeviceAutoConnectCallBack(GX_AUTO_CONNECTION_STATUS_ENTRY nConnectStatus, void *pUserParam)
{
    cCarShuixingMer231C *pCarShuixingMer231C= (cCarShuixingMer231C*)pUserParam;
    if(nConnectStatus==GX_AUTO_CONNECTION_STATUS_OFF)
        pCarShuixingMer231C->m_bIsOffline=true;

}



