#include "cCarShuixingMer231.h"
#include"dsdebug.h"
#include"IGXFactory.h"
#include<QThread>
unsigned int cCarShuixingMer231::m_nOpenedNum = 0;
cCarShuixingMer231::GC cCarShuixingMer231::gc;

cCarShuixingMer231::cCarShuixingMer231()
{
    m_IsOpen = false;
    m_bIsOffline=false;


}

void cCarShuixingMer231::reset()
{
    //    IGXFactory::GetInstance().Init();
    //    IGXFactory::GetInstance().GigEResetDevice(GxIAPICPP::gxstring(m_szIPAddress.c_str()),GX_MANUFACTURER_SPECIFIC_RESET);

}


bool cCarShuixingMer231::Open()
{

#ifdef FLAG_GIGE
    //大恒wagnkou
    try
    {
        IGXFactory::GetInstance().Init();

        DSDEBUG<<"open camere:"<<QString::fromStdString(m_szIPAddress);
        //  ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceByMAC(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_CONTROL);
        ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceByIP(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_CONTROL);
        ObjStreamPtr = ObjDevicePtr->OpenStream(0);
        ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
#if 1
        GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
        if (GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
                ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            }
        }
#endif // 0

        ObjStreamPtr->RegisterCaptureCallback(this, nullptr);
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
        ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
        DSDEBUG<<"心跳时间:"<<ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->GetValue();
        ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(10000);

        m_IsOpen = true;
        return  true;
    }

    catch (CGalaxyException&e)
    {
        std::cout << "错误码: " << e.GetErrorCode() << std::endl;
        std::cout << "错误描述信息: " << e.what() << std::endl;
        m_IsOpen=false;
    }
    m_IsOpen=false;
    return false;


#else
    //大恒usb
    try
    {
        IGXFactory::GetInstance().Init();


        GxIAPICPP::gxdeviceinfo_vector m_vectorDeviceInfo;          ///< 枚举到的设备信息

        IGXFactory::GetInstance().UpdateDeviceList(1000,m_vectorDeviceInfo);

        DSDEBUG<<"open camere:"<<QString::fromStdString(m_szIPAddress);
        //ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceByIP(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_CONTROL);
        ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_EXCLUSIVE);

        ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
#if 1
        GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
        if (GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
                ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            }

            DSDEBUG<<"心跳时间:"<<ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->GetValue();
            ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(10000);
        }
#endif // 0


        //注册掉线回调
//        m_hCB = ObjDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);

        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
        ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");

        ObjStreamPtr = ObjDevicePtr->OpenStream(0);
        ObjStreamPtr->RegisterCaptureCallback(this, nullptr);
        m_IsOpen = true;
        return  true;
    }

    catch (CGalaxyException&e)
    {
        std::cout << "错误码: " << e.GetErrorCode() << std::endl;
        std::cout << "错误描述信息: " << e.what() << std::endl;
        m_IsOpen=false;
    }
    m_IsOpen=false;
    return false;
#endif

}


void cCarShuixingMer231::Close()
{

    if (!m_IsOpen) return;

    SetTrigger(0);

    ObjStreamPtr->UnregisterCaptureCallback();

    //释放资源
    ObjStreamPtr->Close();
    ObjDevicePtr->Close();

    m_IsOpen = false;
    cCarShuixingMer231::m_nOpenedNum--;
    cCarShuixingMer231::m_AllOpenedNum--;
    while (m_ImageBufArr.size() > 0)
    {
        m_ImageBufArr.dequeue();
    }



}

void cCarShuixingMer231::triggerbySoftware()
{
    int sumRes = 0;

}

bool cCarShuixingMer231::IsOpen() const
{
    return  m_IsOpen;
}


CCameraBase& cCarShuixingMer231::operator=(const CCameraBase &another)
{
    if (this == &another)
    {
        return *this;
    }
}


bool cCarShuixingMer231::operator ==(const CCameraBase &another) const
{
    if (another.GetIPAddress() != this->GetIPAddress()) return false;
    return true;
}


bool cCarShuixingMer231::operator !=(const CCameraBase &another) const
{
    if (another.GetIPAddress() == this->GetIPAddress()) return false;
    return true;
}


CCameraBase * cCarShuixingMer231::Clone() const
{
    //用到了再补充
    cCarShuixingMer231 * ret = new cCarShuixingMer231;
    return  ret;
}


bool cCarShuixingMer231::Copy(const CCameraBase& obj)
{

    return true;
}


int cCarShuixingMer231::GetObjType() const
{
    return static_cast<int>(ENUM_CAMERA_TYPE_MER2);
}



void cCarShuixingMer231::DoOnImageCaptured(CImageDataPointer&objImageDataPointer, void* pUserParam)
{
    //    startTime=clock();
    //    DSDEBUG<<QString::fromStdString(this->m_szIPAddress)<<QString("  图像次数:%1").arg(++this->count)<<"    clock:"<<startTime;
    //    //DSDEBUG<<QString::fromStdString(this->m_szIPAddress)<< "收到一帧图像:" <<clock();
    //    cv::Mat tmp = cv::Mat::zeros(objImageDataPointer->GetHeight(), objImageDataPointer->GetWidth(), CV_8UC1);
    //    memcpy(tmp.ptr<uchar>(0), objImageDataPointer->GetBuffer(), objImageDataPointer->GetPayloadSize());
    //    cv::cvtColor(tmp, tmp, CV_BayerBG2BGR);
    //    this->m_Mutex.lock();
    //    this->m_ImageBufArr.enqueue(tmp);
    //    //DSDEBUG <<QString::fromStdString(m_szIPAddress)<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size();
    //    this->m_Mutex.unlock();


    startTime=clock();
    //    DSDEBUG<<QString::fromStdString(this->m_szIPAddress)<<QString("  图像次数:%1").arg(++this->count)<<"    clock:"<<startTime;
    //DSDEBUG<<QString::fromStdString(this->m_szIPAddress)<< "收到一帧图像:" <<clock();
    //    DSDEBUG<<objImageDataPointer->GetHeight();
    //     DSDEBUG<<objImageDataPointer->GetWidth();
    cv::Mat tmp = cv::Mat::zeros(objImageDataPointer->GetHeight(), objImageDataPointer->GetWidth(), CV_8UC1);
    memcpy(tmp.ptr<uchar>(0), objImageDataPointer->GetBuffer(), objImageDataPointer->GetPayloadSize());



#ifdef FLAG_GIGE
    cv::cvtColor(tmp, tmp, cv::COLOR_BayerRG2RGB);
#else
    cv::cvtColor(tmp, tmp, cv::COLOR_BayerRG2BGR);
#endif


//    std::string imgName=(this->m_szIPAddress)+"_.bmp";
//    cv::imwrite(imgName,tmp);

    this->m_Mutex.lock();
    this->m_ImageBufArr.enqueue(tmp);
//    DSDEBUG__ <<QString::fromStdString(m_szIPAddress)<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size();
//    LOG(INFO)<<QString::fromStdString(m_szIPAddress).toStdString()<< "   in m_BufArr.SIZE:" << m_ImageBufArr.size()<<endl;
    this->m_Mutex.unlock();
//    DSDEBUG__ <<QString::fromStdString(m_szIPAddress)<< "   out m_BufArr.SIZE:" << m_ImageBufArr.size();
//    LOG(INFO)<<QString::fromStdString(m_szIPAddress).toStdString()<< "   out m_BufArr.SIZE:" << m_ImageBufArr.size()<<endl;


}

int cCarShuixingMer231::GetGain() 
{
    return (int)ObjFeatureControlPtr->GetFloatFeature("Gain")->GetValue();

}

void cCarShuixingMer231::SetGain(int iGain)
{
    if(!m_IsOpen) return;
    ObjFeatureControlPtr->GetFloatFeature("Gain")->SetValue(iGain);
}
void cCarShuixingMer231::GetGainRange(int &iMin, int &iMax)
{
    iMin = (int)ObjFeatureControlPtr->GetFloatFeature("Gain")->GetMin();
    iMax = (int)ObjFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
}

int cCarShuixingMer231::GetExposureTime()
{
    return (int)ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();

}
void cCarShuixingMer231::SetExposureTime(int iExposure)
{
    if(!m_IsOpen) return;
    ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(iExposure);
}
void cCarShuixingMer231::GetExposureRange(int &iMin, int &iMax)
{
    iMin=(int)ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();
    iMax = (int)ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();

}

//! White balance
void cCarShuixingMer231::GetWhiteBalance(int &iRGain, int &iGGain, int &iBGain) 
{

}
void cCarShuixingMer231::SetWhiteBalance(int iRGain, int iGGain, int iBGain)
{

}
void cCarShuixingMer231::GetWhiteBalanceRange(int &iMin, int &iMax) 
{

}


void cCarShuixingMer231::SetTrigger(int iTriggerMode)
{

    switch (iTriggerMode)
    {
    case 0://停止chu'fa

        ObjStreamPtr->StopGrab();
        m_bIsSnap=false;
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        break;
    case 1://外部line触发
        DSDEBUG<<QString::fromStdString(m_szIPAddress)<<"  设置外触发成功";
        ObjStreamPtr->StartGrab();
        m_bIsSnap=true;
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
        break;
    case 2://软件chufa
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
        ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Software");
        ObjStreamPtr->StartGrab();
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

        break;
    case 3://软件chufa stop
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
        ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
        ObjStreamPtr->StopGrab();
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

        break;

    }

}

int cCarShuixingMer231::loadSetting(std::string filePath)
{

    if (!this->IsOpen())
    {
        /*LOG(ERROR) << "camera has not opened!" << endl;*/
        return -1;
    }

    return 0;
}

void cCarShuixingMer231::clearBuf()
{
    //DSDEBUG<<QString("%1进入清理函数").arg(QString::fromStdString(this->GetIPAddress()));
    this->m_Mutex.lock();
    //DSDEBUG<<QString("%1进入清理函数,获取到锁,清理缓存数量：%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());
    this->m_ImageBufArr.clear();
    this->m_Mutex.unlock();
    //DSDEBUG<<QString("%1进入清理函数,释放锁,缓存数量%2").arg(QString::fromStdString(this->GetIPAddress())).arg(this->m_ImageBufArr.size());

}


bool cCarShuixingMer231::GrabEx()
{

    //this->m_Camera->QueueFrame(this->m_FramePtr);

    /*this->m_Mutex.lock();
    while (1)
    {
        if (this->m_BufArr.size() > 0) break;;
        Sleep(1);
    }
    std::cout << "size:"<<m_BufArr.size() << std::endl;
    this->m_BufArr.pop();
    this->m_Mutex.unlock();
    std::cout << "获取到!:" << this->m_szIPAddress << std::endl;

    cv::Mat m(this->m_Height, this->m_Width, CV_8UC1);
    memcpy(m.ptr<uchar>(0), this->m_Buf, this->m_BufferSize);
    cv::imwrite("e:/save/" + this->m_szIPAddress +"__" +std::to_string(index++)+".bmp",m);*/

    return true;
}

bool cCarShuixingMer231::HasImage(unsigned char * pImage)
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
int cCarShuixingMer231::GetChannelCamNum() const
{
    return m_nCurCameraIndex;
}

//void cCarShuixingMer231::__ProcessOffline() throw()
//{

//    try
//    {
//        printf("**********************Process Offline**********************\r");
//        //判断设备是否停止采集
//        if (m_bIsSnap)
//        {
//            //			cout<<"\n<Send stop snap command to device>"<<endl;
//            ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
//        }
//    }

//    catch(CGalaxyException &e)
//    {
//        //		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
//    }
//    catch(std::exception &e)
//    {
//        //		cout<<"<"<<e.what()<<">"<<endl;
//    }

//    try
//    {
//        //判断设备是否停止采集
//        if (m_bIsSnap)
//        {
//            ObjStreamPtr->StopGrab();
//            m_bIsSnap = false;
//        }
//    }
//    catch(CGalaxyException)
//    {
//        //do noting
//    }
//    catch(std::exception)
//    {
//        //do noting
//    }

//    try
//    {
//        //判断设备是否打开
//        if (m_IsOpen)
//        {
//            //注销掉线回调函数
//            //			cout<<"<Unregister device Offline callback>"<<endl;
//            ObjDevicePtr->UnregisterDeviceOfflineCallback(m_hCB);

//            //关闭流和设备
//            //			cout<<"<Close Device>"<<endl;
//            ObjStreamPtr->Close();
//            ObjDevicePtr->Close();
//            m_IsOpen = false;

//        }
//    }
//    catch(CGalaxyException)
//    {
//        //do noting
//    }
//    catch(std::exception)
//    {
//        //do noting
//    }
//}

//void cCarShuixingMer231::__Recovery()
//{
//    while(true)
//    {
//        QThread::msleep(100);

//        bool bIsDeviceOpen = false;              ///< 设备是否已打开标识
//        bool bIsStreamOpen = false;              ///< 设备流是否已打开标识

//    #ifdef FLAG_GIGE
//        //大恒wagnkou
//        try
//        {
//            IGXFactory::GetInstance().Init();

//            DSDEBUG<<"open camere:"<<QString::fromStdString(m_szIPAddress);
//            //  ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceByMAC(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_CONTROL);
//            ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceByIP(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_CONTROL);
//            ObjStreamPtr = ObjDevicePtr->OpenStream(0);
//            ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
//    #if 1
//            GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
//            if (GX_DEVICE_CLASS_GEV == objDeviceClass)
//            {
//                if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
//                {
//                    int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
//                    ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
//                }
//            }
//    #endif // 0

//            ObjStreamPtr->RegisterCaptureCallback(this, nullptr);
//            ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
//            ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
//            DSDEBUG<<"心跳时间:"<<ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->GetValue();
//            ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(10000);

//            m_IsOpen = true;
//            return  true;
//        }

//        catch (CGalaxyException&e)
//        {
//            std::cout << "错误码: " << e.GetErrorCode() << std::endl;
//            std::cout << "错误描述信息: " << e.what() << std::endl;
//            m_IsOpen=false;
//        }
//        m_IsOpen=false;
//        return false;


//    #else
//        //大恒usb
//        try
//        {
//            IGXFactory::GetInstance().Init();


//            GxIAPICPP::gxdeviceinfo_vector m_vectorDeviceInfo;          ///< 枚举到的设备信息

//            IGXFactory::GetInstance().UpdateDeviceList(1000,m_vectorDeviceInfo);

//            DSDEBUG<<"open camere:"<<QString::fromStdString(m_szIPAddress);
//            ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(GxIAPICPP::gxstring(m_szIPAddress.c_str()), GX_ACCESS_EXCLUSIVE);
//             bIsDeviceOpen = true;

//            ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

//            //判断流个数
//            int32_t nStreamCount    = ObjDevicePtr->GetStreamCount();
//            if (nStreamCount > 0)
//            {
//                ObjStreamPtr = ObjDevicePtr->OpenStream(0);
//                bIsStreamOpen = true;
//            }
//            else
//            {
//                throw ("Find 0 device");
//    //             bIsDeviceOpen = false;
//    //            bIsStreamOpen=false;
//    //            m_IsOpen=false;
//    //            m_bIsSnap=false;
//            }


//    #if 1
//            GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
//            if (GX_DEVICE_CLASS_GEV == objDeviceClass)
//            {
//                if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
//                {
//                    int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
//                    ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
//                }

//                DSDEBUG<<"心跳时间:"<<ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->GetValue();
//                ObjFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(10000);
//            }
//    #endif // 0

//            //注册掉线回调
//            m_hCB = ObjDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);

//            ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
//            ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");

//            ObjStreamPtr->RegisterCaptureCallback(this, nullptr);
//            ObjStreamPtr->StartGrab();


//             m_bIsSnap=true;
//             m_bIsOffline=false;
//             m_IsOpen = true;
//        }


//        catch(CGalaxyException &e)
//        {
//            //判断设备流是否已打开
//            if (bIsStreamOpen)
//            {
//                ObjStreamPtr->Close();
//            }

//            //判断设备是否已打开
//            if (bIsDeviceOpen)
//            {
//                ObjDevicePtr->Close();
//            }

//            m_bIsSnap=false;
//            m_bIsOffline=true;
//            m_IsOpen = false;
//            continue;

//        }
//        catch(std::exception &e)
//        {
//            //判断设备流是否已打开
//            if (bIsStreamOpen)
//            {
//                ObjStreamPtr->Close();
//            }

//            //判断设备是否已打开
//            if (bIsDeviceOpen)
//            {
//                ObjDevicePtr->Close();
//            }
//            m_bIsSnap=false;
//            m_bIsOffline=true;
//            m_IsOpen = false;
//            continue;
//        }
//        m_bHasRecoveryed=true;

//       break;


//    #endif
//    }

//}



cCarShuixingMer231::~cCarShuixingMer231()
{
    if (this->IsOpen())
    {
        Close();
    }
    //    if(m_Buf!=nullptr)
    //    {
    //        delete []  m_Buf;
    //        m_Buf=nullptr;
    //    }
}

cCarShuixingMer231::GC::~GC()
{
    IGXFactory::GetInstance().Uninit();
}
