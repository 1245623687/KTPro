#include "kthreadproc.h"
#include"dsdebug.h"
#include<QMap>
#include"params/dssystemparam.h"
#include<time.h>
#include"thread/kthreadsaveimage.h"
#include<QDateTime>
#include "glog/logging.h"
#include"kthreaddatabase.h"
#include"CheckOperator/imgtobaccocontrol.h"


//#define CKDSDEBUG qDebug()<<"file_"<<__FILE__<<" line_"<<__LINE__<<"cameraIdx: "<<IdxCamera<<" ";
std::mutex KThreadProc::m_Mutex;
KThreadProc::KThreadProc()
{

}

void KThreadProc::run()
{
    DSDEBUG<<"~~~~~~~~~~~~~~~~运行线程启动！";

    PackageChecker* pc=PackageChecker::getInstance();

    KThreadSaveImage * threadSave=KThreadSaveImage::getInstance();
    threadSave->start();

    DSDEBUG<<"存图线程已开启!";

    KThreadDatabase::getInstance();



    //清理相机缓存

    //        pc->Cameras[IdxCamera]->clearBuf();



    pc->Cameras[IdxCamera]->SetTrigger(1);
    DSDEBUG<<QString("相机(%1):已设置触发").arg(QString::fromStdString(pc->Cameras[IdxCamera]->GetIPAddress()));


    int prsPin=0;
    int resPin=0;

    prsPin=(IdxCamera-1)*2;
    resPin=(IdxCamera-1)*2+1;



    pc->WarningFlag[0]=0;
    pc->WarningFlag[2]=0;


    int count=0;
    //    pc->IOContol->setLevel(RUNPIN,0xFFFFFFFF,0x00);

    while (!isInterruptionRequested())
    {

        //DSDEBUG<<"开始循环:"<<count++;

        bool firstImg=false;
        clock_t startcount=LONG_MAX;


        while(!firstImg&& !pc->Cameras[IdxCamera]->m_bIsOffline)
        {

            if(pc->Cameras[IdxCamera]->m_ImageBufArr.size()>0||isInterruptionRequested())
            {
                firstImg=true;
                // DSDEBUG<<"取得图片:"<<QString::fromStdString(pc->Cameras[IdxCamera]->GetIPAddress());
            }
            std::this_thread::sleep_for( std::chrono::microseconds(1));
        }

        if(pc->Cameras[IdxCamera]->m_bIsOffline)
        {
            emit sigCamOffLine(IdxCamera-1);
            break;
        }



        if(pc->UpdateCheckOperator)
        {
            m_Mutex.lock();
            if(pc->UpdateCheckOperator)
            {
                DSDEBUG<<"开始在线更新算子资料,CamIndex: "<<IdxCamera<<endl;

                LOG(INFO)<<"开始在线更新算子资料";
                clock_t updateClock=clock();

                QElapsedTimer elapstime;
                elapstime.restart();
                //正常模式
                while (true)
                {
                    std::this_thread::sleep_for( std::chrono::microseconds(1));
                    if(elapstime.elapsed()>300)
                    {
                        break;
                    }

                    bool ret=true;
                    for(int i=0;i<DSSystemParam::CameraNum;i++)
                    {
                        ret=ret&pc->IsCalcaulateFinish[i];
                    }
                    if(ret)
                        break;
                }
                pc->ImgTobaccoAnalysis->copyto( pc->ImgTobaccoRun);
                emit inforUpdateRect();
                pc->UpdateCheckOperator=0;
                LOG(INFO)<<"在线更新算子资料完成,时间:"<<clock()-updateClock<<"ms";
                DSDEBUG<<"在线更新算子资料完成,时间:"<<clock()-updateClock<<"ms"<<"CamIndex,:"<<IdxCamera<<endl;
            }
            m_Mutex.unlock();
        }



        if(isInterruptionRequested()) break;
        {
            //DSDEBUG<<"正常模式";
            if(pc->RunParam_Shift!=DSSystemParam::getCurrentShift())
            {
                m_Mutex.lock();
                if(pc->RunParam_Shift!=DSSystemParam::getCurrentShift())
                {
                    clock_t shiftClock=clock();
                    pc->RunParam_Shift=DSSystemParam::getCurrentShift();
                    pc->RunParam_CalcNumAll=0;
                    pc->RunParam_CalcNumNg=0;


                    for(int i=0;i<CAMERANUM_MAX;++i)
                    {
                        pc->RunParam_CalcNumAllCams[i]=0;//当前班次处理总数
                        pc->RunParam_CalcNumNgCams[i]=0;//当前班次的Ng数量

                        PackageChecker::getInstance()-> RunParam_CalcNumNgGDs[i]=0;//当前班次的Ng数量
                        PackageChecker::getInstance()->RunParam_CalcNumNgTotals[i]=0;//当前班次的Ng数量
                    }

                    for(int idx_i=0;idx_i<CAMERANUM_MAX;++idx_i)
                    {
                        for(int idx_j=0;idx_j<CHECKOPERATORNUM_MAX;++idx_j)
                        {
                            pc->ErrRecord[idx_i][idx_j]=0;
                        }
                    }
                    LOG(INFO)<<"切换班次信息:新班次为"<<DSSystemParam::getCurrentShift()<<"   切换时间："<<clock()-shiftClock<<"  切换完成";
                    DSDEBUG<<"切换班次信息:新班次为"<<DSSystemParam::getCurrentShift()<<"   切换时间："<<clock()-shiftClock;
                }
                m_Mutex.unlock();
            }
        }


        //发送心跳信号
        {
            emit sigupdateGPIO(prsPin ,0x00);
            msleep(1);
            emit sigupdateGPIO(prsPin ,0xFF);
        }

        pc->Cameras[IdxCamera]->HasImage(pc->CurImage[IdxCamera].ImageRGB.ptr(0));
        //        if(IdxCamera==2)
        //          cv::rotate(pc->CurImage[IdxCamera].ImageRGB,pc->CurImage[IdxCamera].ImageRGB,cv::ROTATE_180);

        clock_t startTime=clock();

        //DSDEBUG<<QString("相机(%1):已取到图片").arg(QString::fromStdString((*itor)->GetIPAddress()));
        //pc->Cameras[IdxCamera]->clearBuf();
        ImgTobaccoControl control(pc->ImgTobaccoRun);
        control.setCalcImage(IdxCamera, pc->CurImage[IdxCamera]);

        //std::string strname=QString("%1.bmp").arg(IdxCamera).toStdString();

        // LOG(INFO)<<"设置计算图片完成,开始执行calculate";
        memset(pc->ErrMatrix[IdxCamera],0,CHECKOPERATORNUM_MAX);

        int  errAllNum;
        pc->IsCalcaulateFinish[IdxCamera-1]=0;
        {
            errAllNum=control.calculate(IdxCamera,pc->ErrMatrix);
        }
        pc->IsCalcaulateFinish[IdxCamera-1]=1;



        //图像与光电结果逻辑运算
#ifdef FALG_PHE
        if(IdxCamera==1)
            msleep(0);
        if(IdxCamera==2)
            msleep(0);

        //DSDEBUG_<<IdxCamera<<","<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"before emit "<<endl;
//        if(IdxCamera==2)
        {
            emit PackageChecker::getInstance()->updateCheckRetSig(4);
        }
#endif

        // DSDEBUG_<<IdxCamera<<","<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"emit Finished "<<endl;

        int retType=0;
        {
#ifdef FALG_PHE
            int phe=PackageChecker::getInstance()->ErrPhe[IdxCamera-1];
            if(phe>0)
            {
                pc->RunParam_CalcNumNgGDs[IdxCamera-1]++;
            }

            retType =getLogicRet(errAllNum,phe);
            if(retType)
            {
                pc->RunParam_CalcNumNgTotals[IdxCamera-1]++;

                //   DSDEBUG<<"错误烟条";
                if(pc->Options->OutputType()==ENUMOUTPUTTYPE_OUT)
                    emit sigupdateGPIO(resPin ,0x00);
            }
            else
            {
                // DSDEBUG<<"正确烟条";
                emit sigupdateGPIO(resPin ,0xFF);
            }
#else
            if(errAllNum>0)
            {
                pc->RunParam_CalcNumNgTotals[IdxCamera-1]++;
                //DSDEBUG<<"错误烟条";
                if(pc->Options->OutputType()==ENUMOUTPUTTYPE_OUT)
                    emit sigupdateGPIO(resPin ,0x00);
                retType=1;

            }
            else
            {
                // DSDEBUG<<"正确烟条";
                emit sigupdateGPIO(resPin ,0xFF);
                retType=0;
            }
#endif
        }

        //save img begin
        if(PackageChecker::DynamicGrab)
        {
            QString saveName=pc->Options->ImgSavePath()+"/图像保存/"+DSSystemParam::BrandName+"/分析图像/"+QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz")+QString("_%1_%2.bmp").arg(DSSystemParam::getCurrentShift()).arg(IdxCamera);
            threadSave->addImg(saveName,pc->CurImage[IdxCamera]);
        }

        // DSDEBUG<<"time:"<<clock()-startTime<<"  错误总数为:"<<errAllNum<<endl;
        switch (pc->Options->SaveType())
        {
        case ENUMIMGSAVETYPE_NOSAVE:

            break;
        case ENUMIMGSAVETYPE_ALL:
        {
            if(retType>0)
            {
                QString saveName=pc->Options->ImgSavePath()+"/图像保存/"+DSSystemParam::BrandName+"/NG/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"/"+QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz")+QString("_%1_%2_%3.bmp").arg(DSSystemParam::getCurrentShift()).arg(IdxCamera).arg(retType);
                threadSave->addImg(saveName,pc->CurImage[IdxCamera]);
            }
            else
            {
                QString saveName=pc->Options->ImgSavePath()+"/图像保存/"+DSSystemParam::BrandName+"/OK/"+QString("Camera%1").arg(IdxCamera)+"/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"/"+QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz")+QString("_%1_%2_%3.bmp").arg(DSSystemParam::getCurrentShift()).arg(IdxCamera).arg(retType);
                threadSave->addImg(saveName,pc->CurImage[IdxCamera]);
            }
        }
            break;
        case  ENUMIMGSAVETYPE_NG:
        {
            if(retType>0)
            {
                QString saveName=pc->Options->ImgSavePath()+"/图像保存/"+DSSystemParam::BrandName+"/NG/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+"/"+QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz")+QString("_%1_%2_%3.bmp").arg(DSSystemParam::getCurrentShift()).arg(IdxCamera).arg(retType);
                threadSave->addImg(saveName,pc->CurImage[IdxCamera]);
            }
        }
            break;
        }

        // DSDEBUG<<"time:"<<clock()-startTime<<"  错误总数为:"<<errAllNum<<endl;
        //数据上传
        clock_t  saveClock=clock();

        if(errAllNum>0)
        {
            pc->RunParam_CalcNumNg++;
            pc->RunParam_CalcNumNgCams[IdxCamera-1]++;
            pc->CheckRet[IdxCamera-1]=1;


            if(IdxCamera==1)
            {
                memcpy(pc->m_qmapCurBadImage[IdxCamera].ImageRGB.ptr<uchar>(0), pc->CurImage[IdxCamera].ImageRGB.ptr<uchar>(0),  pc->Cameras[IdxCamera]->getBufSize());

                control.getCurBadImage(IdxCamera,pc->m_qmapCurBadImage[IdxCamera]);
            }


            if(IdxCamera==2)
            {
                //不做标记
                memcpy(pc->m_qmapCurBadImage[IdxCamera].ImageRGB.ptr<uchar>(0), pc->CurImage[IdxCamera].ImageRGB.ptr<uchar>(0),  pc->Cameras[IdxCamera]->getBufSize());
                //做标记
                control.getCurBadImage(IdxCamera,pc->m_qmapCurBadImage[IdxCamera]);
                //cv::imwrite("./m_qmapCurBadImage.bmp",pc->m_qmapCurBadImage[IdxCamera].ImageRGB);
            }
        }
        pc->RunParam_CalcNumAllCams[IdxCamera-1]++;
        pc->RunParam_CalcTime=(clock()-startTime);
        pc->RunParam_CalcTimeCams[IdxCamera-1]=(clock()-startTime);


        if(IdxCamera==2)
            DSDEBUG__<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"CalTime "<< pc->RunParam_CalcTimeCams[IdxCamera-1]<<endl;


        for(int i=0;i<CAMERANUM_MAX;i++)
        {
            for(int j=0;j<CHECKOPERATORNUM_MAX;j++)
            {
                if(pc->ErrMatrix[i][j]==1)
                {
                    pc->ErrRecord[i-1][j-1]++;
                }
            }
        }
        emit inforUpdate(this->IdxCamera);
    }

    pc->Cameras[IdxCamera]->SetTrigger(0);
    pc->IOContol->setLevel(prsPin,0xFFFFFFFF,0xFF);
    pc->IOContol->setLevel(resPin,0xFFFFFFFF,0xFF);
    //pc->IOContol->setLevel(RUNPIN,0xFFFFFFFF,0xFF);

}

//返回结果，0OK，1图像检测，2光电检出，3图像光电同时检出
int KThreadProc::getLogicRet(int pic, int eph)
{
    switch (static_cast<int>(PackageChecker::getInstance()->Options->checkMode()))
    {
    case 0:
    {
        if(pic>0)
            pic=1;
        return pic;
    }

    case 1:
    {
        if(eph>0)
            eph=2;
        return eph;
    }

    case 2:
        return pic&&eph;
    case 3:
    {
        if(pic==0&&eph==0)
            return 0;
        if(pic>0&&eph==0)
            return 1;
        if(pic==0&&eph>0)
            return 2;
        if(pic>0&&eph>0)
            return 3;

    }

    }
}
