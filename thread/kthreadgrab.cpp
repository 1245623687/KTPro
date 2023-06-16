#include "kthreadgrab.h"
#include"PackageChecker.h"
#include"CheckOperator/imgtobaccocontrol.h"
KThreadGrab::KThreadGrab()
{

}


void KThreadGrab::run()
{
    DSDEBUG<<"~~~~~~~~~~~~~~~~采集线程启动！"<<endl;

    PackageChecker* pc=PackageChecker::getInstance();
    //    std::map<int,OpencvImage> mapImage;

    //    for(int idx=1;idx<=DSSystemParam::CameraNum;idx++)
    //    {
    //        mapImage[idx].Format=ENUMDSIMAGEFORMAT_GRAY;
    //        mapImage[idx].Width=640;
    //        mapImage[idx].Height=480;
    //        mapImage[idx].ImageGray= cv::Mat::zeros(480, 640, CV_8UC1);
    //    }



    QMap<int,CCameraBase*>::iterator itor=pc->Cameras.begin();
    //清理相机缓存
    for(;itor!=pc->Cameras.end();itor++)
    {
        itor.value()->clearBuf();
    }

    CCameraBase * camera=pc->Cameras[cameraIdx];
    camera->SetTrigger(1);



    while (!isInterruptionRequested())
    {
        bool firstImg=false;
        clock_t startcount=LONG_MAX;
        while(!firstImg)
        {
            if(camera->m_ImageBufArr.size()>0||isInterruptionRequested())
            {
                firstImg=true;
            }
            std::this_thread::sleep_for( std::chrono::microseconds(1));
        }


        if(isInterruptionRequested()) break;


        DSDEBUG<<"正常模式";
        camera->HasImage(pc->CurImage[cameraIdx].ImageRGB.ptr(0));
        camera->clearBuf();

        ImgTobaccoControl control(pc->ImgTobaccoAnalysis);
        control.setRefImage(cameraIdx, pc->CurImage[cameraIdx]);

        emit inforUpdate();

    }
    camera->SetTrigger(0);

}
