#include "cHKUSBcamera.h"
#include<QDebug>


cHKUSBCamera::GC cHKUSBCamera::gc;

cHKUSBCamera::cHKUSBCamera()
{
    m_hkHandle=NULL;
    memset(&m_hkDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    m_isGrabing=false;
    m_IsOpen=false;
}

cHKUSBCamera::~cHKUSBCamera()
{
    if (this->IsOpen())
    {
        Close();
    }
}

int cHKUSBCamera::loadSetting(std::string filePath)
{
    int ret=0;

    return ret;

}

//CCameraBase & cHKUSBCamera::operator=(const CCameraBase &another)
//{

//    //this->Copy()
//    return *this;

//}

CCameraBase* cHKUSBCamera::Clone() const
{
    cHKUSBCamera *ret=new cHKUSBCamera();

    return  ret;
}

bool cHKUSBCamera::Copy(const CCameraBase &obj)
{


    return true;
}

int cHKUSBCamera::GetObjType() const
{
    return static_cast<int>(ENUM_CAMERA_TYPE::ENUM_CAMERA_TYPE_HKUSB);
}

bool cHKUSBCamera::IsOpen() const
{
    return m_IsOpen;
}

bool cHKUSBCamera::Open()
{
    bool ret=true;
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO* pDeviceInfo=nullptr;

    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_hkDeviceList);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Enum HK Devices Failed"<<endl;
    }
    if (m_hkDeviceList.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < m_hkDeviceList.nDeviceNum; i++)
        {
            pDeviceInfo = m_hkDeviceList.pDeviceInfo[i];

            int size=sizeof (pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
            char* serialNum=new char[size];

            memcpy(serialNum,pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber,sizeof (pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber));
            if(strcmp(serialNum,GetIPAddress().c_str()) ==0)
            {
                break;
            }
        }
    }
    else
    {
        return false;
        LOG(ERROR)<<"Find 0 HK Devices "<<endl;
    }

    // ch:选择设备并创建句柄 | Select device and create handle
    nRet = MV_CC_CreateHandle(&m_hkHandle, pDeviceInfo);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Create Handle fail  nRet:   "<<nRet<<endl;
    }

    // ch:打开设备 | Open device
    nRet = MV_CC_OpenDevice(m_hkHandle);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Open Device fail  nRet:   "<<nRet<<endl;
    }

    // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(m_hkHandle);
        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValue(m_hkHandle,"GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK)
            {
                LOG(ERROR)<<"Warning: Set Packet Size fail nRet:   "<<nRet<<endl;
            }
        }
        else
        {
            LOG(ERROR)<<"Warning: Get Packet Size fail nRet:   "<<nRet<<endl;
        }
    }

    // ch:注册抓图回调 | en:Register image callback
    nRet = MV_CC_RegisterImageCallBackEx(m_hkHandle, DoOnImageCaptured, this);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Register Image CallBack fail! nRet  "<<nRet<<endl;
    }



  //   MV_CC_SetEnumValue(IN void* handle,IN const char* strKey,IN unsigned int nValue);

    nRet = MV_CC_SetEnumValue(m_hkHandle,"PixelFormat",static_cast<int>(MvGvspPixelType::PixelType_Gvsp_RGB8_Packed));


    // ch:开始取流 | en:Start grab image
    nRet = MV_CC_StartGrabbing(m_hkHandle);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Start Grabbing fail! nRet  "<<nRet<<endl;
    }

    if(nRet!=MV_OK)
        ret=false;
    else
    {
        m_IsOpen=true;
    }
    return ret;
    
}

void cHKUSBCamera::StartGrab()
{

    if(m_isGrabing)
        return;

    int nRet = MV_OK;

    nRet = MV_CC_SetCommandValue(m_hkHandle,"AcquisitionStart");

    if (MV_OK != nRet)
    {
        m_isGrabing=false;
        LOG(ERROR)<<"Start Grabbing fail! nRet  "<<nRet<<endl;
    }
    if(MV_OK == nRet)
    {
        m_isGrabing=true;
    }
}

void cHKUSBCamera::StopGrab()
{
    if(!m_isGrabing)
        return;
    int nRet = MV_OK;
    nRet = MV_CC_SetCommandValue(m_hkHandle,"AcquisitionStop");
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Stop Grab fail! nRet  "<<nRet<<endl;
    }
    else
    {
        m_isGrabing=false;
    }

}

void cHKUSBCamera::Close()
{
    if (!m_IsOpen) return;


    int nRet = MV_OK;
    // ch:停止取流 | en:Stop grab image
    nRet = MV_CC_StopGrabbing(m_hkHandle);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Stop Grab fail! nRet  "<<nRet<<endl;
    }

    m_IsOpen = false;
    while (m_ImageBufArr.size() > 0)
    {
        m_ImageBufArr.dequeue();
    }


    // ch:注销抓图回调 | en:Unregister image callback
    nRet = MV_CC_RegisterImageCallBackEx(m_hkHandle, NULL, NULL);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Unregister Image CallBack fail! nRe  "<<nRet<<endl;
    }

    // ch:关闭设备 | en:Close device
    nRet = MV_CC_CloseDevice(m_hkHandle);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Close Device fail! nRet  "<<nRet<<endl;
    }

    // ch:销毁句柄 | en:Destroy handle
    nRet = MV_CC_DestroyHandle(m_hkHandle);
    if (MV_OK != nRet)
    {
        LOG(ERROR)<<"Destroy Handle fail! nRet  "<<nRet<<endl;
    }
    else
    {
         m_IsOpen=false;
        m_hkHandle=NULL;
    }



}

void cHKUSBCamera::reset()
{

}

int cHKUSBCamera::GetGain()
{
    int nRet = MV_OK;
    MVCC_FLOATVALUE tempFLOATVALUE;
    nRet = MV_CC_GetFloatValue(m_hkHandle,"Gain",&tempFLOATVALUE);
    return  (int)tempFLOATVALUE.fCurValue;

}


void cHKUSBCamera::SetGain(int iGain)
{
    int nRet = MV_OK;

    nRet = MV_CC_SetFloatValue(m_hkHandle,"Gain",(float(iGain)));
}

void cHKUSBCamera::GetGainRange(int &iMin, int &iMax)
{
    iMin=0.0;
    iMax=20;
}

int cHKUSBCamera::GetExposureTime()
{
    int nRet = MV_OK;
    MVCC_FLOATVALUE tempFLOATVALUE;
    nRet = MV_CC_GetFloatValue(m_hkHandle,"ExposureTime",&tempFLOATVALUE);
    return  (int)tempFLOATVALUE.fCurValue;
}

void cHKUSBCamera::SetExposureTime(int iExposure)
{
    int nRet = MV_OK;
    iExposure=iExposure>50?iExposure:50;
    nRet = MV_CC_SetFloatValue(m_hkHandle,"ExposureTime",(float(iExposure)));
}

void cHKUSBCamera::GetExposureRange(int &iMin, int &iMax)
{

    iMin=50;
    iMax=1000000;
}

void cHKUSBCamera::SetTrigger(int iTriggerMode)
{

    int nRet = MV_OK;
    switch (iTriggerMode)
    {
    case 0://停止chu'fa
        qDebug()<<"停止chu'fa "<<endl;

        //        ObjStreamPtr->StopGrab();
        //        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        StopGrab();
        break;

    case 1://外部line触发

        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
        if (MV_OK != nRet)
        {
            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        }
        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0);
        if (MV_OK != nRet)
        {
            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        }
        break;
    case 2://软件chufa stop
        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
        if (MV_OK != nRet)
        {
            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        }
        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
        if (MV_OK != nRet)
        {
            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        }
        break;
    case 3://自由chufa
        // ch:设置触发模式为off | eb:Set trigger mode as off
        nRet = MV_CC_SetEnumValue(m_hkHandle, "TriggerMode", MV_TRIGGER_MODE_OFF);
        if (MV_OK != nRet)
        {
            LOG(ERROR)<<"Set Trigger Mode fail! nRet "<<nRet<<endl;
        }
        StartGrab();
        break;
    }
}

bool cHKUSBCamera::GrabEx()
{
    return  true;
}

bool cHKUSBCamera::HasImage(unsigned char *pImage)
{
    try {

        this->m_Mutex.lock();
        // DSDEBUG<<QString("%1进入获取图片,获取到锁").arg(QString::fromStdString(this->GetIPAddress()));
        //unsigned char * pTmp=m_ImageBufArr.dequeue();
        cv::Mat pTmp = m_ImageBufArr.back();

      //  cv::cvtColor(pTmp,pTmp,CV_RGB2BGR);
        //pTmp.type()
        memcpy(pImage, pTmp.ptr<uchar>(0), m_BufferSize);
        this->m_Mutex.unlock();
        //DSDEBUG<<QString("%1进入获取图片,释放锁").arg(QString::fromStdString(this->GetIPAddress()));

    }
    catch (std::exception ex)
    {
      //  DSDEBUG << ex.what() << endl;
    }
    catch (...)
    {
          //  DSDEBUG << "unknown err!" << endl;
    }

    return true;
}

int cHKUSBCamera::GetChannelCamNum() const
{
    return m_nCurCameraIndex;
}

void cHKUSBCamera::triggerbySoftware()
{
    int sumRes = 0;
}

void cHKUSBCamera::clearBuf()
{
    //DSDEBUG<<QString("%1进入清理函数").arg(QString::fromStdString(this->GetIPAddress()));
    this->m_Mutex.lock();
    //DSDEBUG<<QString("%1进入清理函数,获取到锁,清理缓存数量：%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());
    this->m_ImageBufArr.clear();
    this->m_Mutex.unlock();
    //DSDEBUG<<QString("%1进入清理函数,释放锁,缓存数量%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());

}

int cHKUSBCamera::RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight)
{
    if ( NULL == pRgbData )
       {
           return MV_E_PARAMETER;
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

       return MV_OK;

}



void DoOnImageCaptured(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUserParam)
{
       cHKUSBCamera * pcHKUSBCamera=static_cast<cHKUSBCamera *>(pUserParam);

      //  startTime=clock();

       MvGvspPixelType p=pFrameInfo->enPixelType;
       p = pFrameInfo->enPixelType;

      // cv::Mat srcImage=cv::Mat::zeros(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
       cv::Mat srcImage;
       if ( pFrameInfo->enPixelType == PixelType_Gvsp_Mono8 )
       {
           srcImage = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
       }

       else if ( pFrameInfo->enPixelType == PixelType_Gvsp_RGB8_Packed )
       {
         //pcHKUSBCamera->RGB2BGR(pData, pFrameInfo->nWidth, pFrameInfo->nHeight);
           srcImage = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
       }
       else
       {
            LOG(ERROR)<<"Convert Image Failed Unknom img format nRet "<<endl;
       }

//        cv::Mat tmp = cv::Mat::zeros(objImageDataPointer->GetHeight(), objImageDataPointer->GetWidth(), CV_8UC1);
//        memcpy(tmp.ptr<uchar>(0), objImageDataPointer->GetBuffer(), objImageDataPointer->GetPayloadSize())

//        cv::cvtColor(tmp, tmp, CV_BayerRG2BGR);
        pcHKUSBCamera->m_Mutex.lock();

       cv::resize(srcImage,srcImage,cv::Size(pFrameInfo->nWidth/4,pFrameInfo->nHeight/4));
        pcHKUSBCamera->m_ImageBufArr.enqueue(srcImage);

      //  DSDEBUG <<QString::fromStdString(m_szIPAddress)<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size();
        pcHKUSBCamera->m_Mutex.unlock();


}

cHKUSBCamera::GC::~GC()
{

    //DSDEBUG <<QString::fromStdString(m_szIPAddress)<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size();
}
