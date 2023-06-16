#include "kthreadcameraconfig.h"
#include"dsdebug.h"
#include"PackageChecker.h"

void KThreadCameraConfig::run()
{
    DSDEBUG<<"~~~~~~~~~~~~~~~~相机参数调试线程启动！"<<endl;




    PackageChecker* pc=PackageChecker::getInstance();
    CCameraBase* curCamera=pc->Cameras[m_CurCameraNo];
    curCamera->SetTrigger(1);




    while (!isInterruptionRequested())
    {
        if(isInterruptionRequested()) break;

        //DSDEBUG<<"相机调试线程——缓存中图片数量："<<pc->Cameras[1]->m_ImageBufArr.size()<<endl;

        bool allImg=false;

        while(!allImg)
        {
            if(isInterruptionRequested()) break;

            if(curCamera->m_ImageBufArr.size()>0)
            {
                allImg=true;
            }
        }

        if(allImg)
        {
            curCamera->HasImage(pc->CurImage[m_CurCameraNo].ImageRGB.ptr<uchar>(0));
        }
        // DSDEBUG<<"写入图片36.bmp";
        // cv::imwrite("d:/36.bmp",CodeChecker::getInstance()->CurImage[1].ImageRGB);

        curCamera->clearBuf();


        //DSDEBUG<<"相机调试线程——缓存中图片数量："<<pc->Cameras[1]->m_ImageBufArr.size()<<endl;
        emit inforUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }



    curCamera->SetTrigger(0);

    DSDEBUG<<"采集线程退出";
}
