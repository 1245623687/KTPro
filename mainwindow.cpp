﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QButtonGroup>
#include<QDateTime>

#include"dsdebug.h"
#include<QResizeEvent>
#include<QButtonGroup>
#include<QDateTime>
#include<QDesktopWidget>
#include"params/dssystemparam.h"
#include"qtCommon/frmmessagebox.h"
#include"packagechecker.h"
#include"config/dlgbrandmanage.h"
#include"config/dlgsystemconfig.h"
#include"dlgimagerecord.h"
#include"user/dlgusermanage.h"
#include"dlgCheckOperator/dlgcheckoperator.h"
#include"CheckOperator/imgtobaccocontrol.h"
#include"database/recorddialog.h"
#include"database/dlgoperatorlog.h"
#include"Camera/dlgcameraconfig.h"
#include<QtConcurrent>
#include<QtGlobal>


#define MAINWINDOW_VIEWSUBWID 858
#define MAINWINDOW_VIEWSUBHEI 391
#define MAINWINDOW_VIEWMAINWID 954
#define MAINWINDOW_VIEWMAINHEI 497

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("运行界面");


    for(int i=0;i<CAMERANUM_MAX;++i)
    {
        PackageChecker::getInstance()->RunParam_CalcNumAllCams[i]=0;//当前班次处理总数
        PackageChecker::getInstance()->RunParam_CalcNumNgCams[i]=0;//当前班次的Ng数量
        PackageChecker::getInstance()->RunParam_CalcTimeCams[i]=0;//当前班次的Ng数量

        PackageChecker::getInstance()-> RunParam_CalcNumNgGDs[i]=0;//当前班次的Ng数量
        PackageChecker::getInstance()->RunParam_CalcNumNgTotals[i]=0;//当前班次的Ng数量
    }


    updateStatistics(0);
    ui->lcdNumCurTime->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    int sunSec=QDateTime::currentDateTime().toTime_t()-PackageChecker::m_TimeSystemStart.toTime_t();
    QTime dt;
    dt.setHMS((int)(sunSec/3600),(sunSec%3600)/60,(sunSec%3600)%60);
    m_Timer= new QTimer(this);
    m_Timer1=new QTimer(this);
    m_Timer2=new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(m_Timer1, SIGNAL(timeout()), this, SLOT(onTimeout1()));
    connect(m_Timer2, SIGNAL(timeout()), this, SLOT(onTimeout2()));
    m_Timer->start(1000);


    //数据采集
    m_TimerShucai= new QTimer(this);
    connect(m_TimerShucai, SIGNAL(timeout()), this, SLOT(onTimeoutShuCai()));
    m_TimerShucai->start(PackageChecker::getInstance()->Options->getSendInterval()*1000);


    PackageChecker::getInstance()->m_cBaseServerTCP=new CBaseServer();
    PackageChecker::getInstance()->m_cBaseServerTCP->setHostIP(PackageChecker::getInstance()->Options->getIPAddress());
    PackageChecker::getInstance()->m_cBaseServerTCP->setHostPort(static_cast<uint16_t>(PackageChecker::getInstance()->Options->getIPPort()));
    PackageChecker::getInstance()->m_cBaseServerTCP->startListen();




    this->ui->lcdNumRunTime->display("00:00:00");
    ui->lcdNumCalcuTime->display(QString::number(0));

    CAMERA_NUM_USER=PackageChecker::getInstance()->Cameras.size();
    for(int idx=0;idx<CAMERA_NUM_USER;++idx)
    {
        m_ThreadRun[idx]=new KThreadProc;
        m_ThreadRun[idx]->setCamera(idx+1);
        connect(m_ThreadRun[idx],&KThreadProc::inforUpdate,this,&MainWindow::updateInfomation);
        connect(m_ThreadRun[idx],&KThreadProc::inforUpdate,this,&MainWindow::updateStatistics);
        connect(m_ThreadRun[idx],&KThreadProc::inforUpdateRect,this,&MainWindow::updateSceneRect);

        connect(m_ThreadRun[idx],&KThreadProc::sigupdateGPIO,this,&MainWindow::updateGPIO);
        connect(m_ThreadRun[idx],&KThreadProc::sigCamOffLine,this,&MainWindow::doCameraOffline);
    }


    QSettings setting(DSSystemParam::AppPath+"/settings.ini",QSettings::IniFormat);
    m_dScale1=setting.value("Scale/ScaleRatio1",0.4).toDouble();
    m_dScale2=setting.value("Scale/ScaleRatio2",0.4).toDouble();

    //窗口1
    m_deleteTime=setting.value("DeleteTime/Time","02:00:00").toString();
    m_phePosArr[0][0][0]=setting.value("DispPosition1/X1",122).toDouble();
    m_phePosArr[0][0][1]=setting.value("DispPosition1/Y1",60).toDouble();
    m_phePosArr[0][0][2]=setting.value("DispPosition1/D1",150).toDouble();

    m_phePosArr[0][1][0]=setting.value("DispPosition1/X2",50).toDouble();
    m_phePosArr[0][1][1]=setting.value("DispPosition1/Y2",240).toDouble();
    m_phePosArr[0][1][2]=setting.value("DispPosition1/D2",150).toDouble();

    m_phePosArr[0][2][0]=setting.value("DispPosition1/X3",50).toDouble();
    m_phePosArr[0][2][1]=setting.value("DispPosition1/Y3",440).toDouble();
    m_phePosArr[0][2][2]=setting.value("DispPosition1/D3",150).toDouble();


    //窗口2
    m_phePosArr[1][0][0]=setting.value("DispPosition2/X1",122).toDouble();
    m_phePosArr[1][0][1]=setting.value("DispPosition2/Y1",60).toDouble();
    m_phePosArr[1][0][2]=setting.value("DispPosition2/D1",150).toDouble();

    m_phePosArr[1][1][0]=setting.value("DispPosition2/X2",50).toDouble();
    m_phePosArr[1][1][1]=setting.value("DispPosition2/Y2",240).toDouble();
    m_phePosArr[1][1][2]=setting.value("DispPosition2/D2",150).toDouble();

    m_phePosArr[1][2][0]=setting.value("DispPosition2/X3",50).toDouble();
    m_phePosArr[1][2][1]=setting.value("DispPosition2/Y3",440).toDouble();
    m_phePosArr[1][2][2]=setting.value("DispPosition2/D3",150).toDouble();




    InitStyle();
    InitControl();
    ui->labelBrandName->setText(DSSystemParam::BrandName);
    QScreen *screen=QGuiApplication::primaryScreen();
    QRect mm=screen->availableGeometry();

    //     QRect mm=screen->geometry();
    int screen_width=mm.width();
    int screen_height=mm.height();
    this->setGeometry(int(screen_width*0.05),int(screen_height*0.05),int(screen_width*0.9),int(screen_height*0.9)) ;
    buttonJudge(9);
    on_pushButtonRun_clicked();

    onWarning=false;
    countSeconds=0;

    ui->lcdNumAllNum_Cam3->setVisible(false);
    ui->lcdNumAllNum_Cam4->setVisible(false);

    ui->lcdNumNgNum_Cam3->setVisible(false);
    ui->lcdNumNgNum_Cam4->setVisible(false);
    ui->lcdNumCalcuTime_3->setVisible(false);
    ui->lcdNumCalcuTime_4->setVisible(false);

    ui->label_17->setVisible(false);
    ui->label_32->setVisible(false);

    ui->lcdNumNgPercetange_Cam3->setVisible(false);
    ui->lcdNumNgPercetange_Cam4->setVisible(false);


    ui->lcdNumNgNum_GD3->setVisible(false);
    ui->lcdNumNgNum_GD4->setVisible(false);

    ui->widget_14->setVisible(false);

    ui->lcdNumNgNum_Total3->setVisible(false);
    ui->lcdNumNgNum_Total4->setVisible(false);
    ui->widget_15->setVisible(false);





}


void MainWindow::updateInfomation(int cameraIdx)
{

    m_SceneArr[cameraIdx]->updateRectState(PackageChecker::getInstance()->ErrMatrix[cameraIdx]);


    m_SceneArr[cameraIdx]->updateShow();
    if( PackageChecker::getInstance()->CheckRet[cameraIdx-1]==1)
    {
        m_SceneArr[cameraIdx+2]->updateRectState(PackageChecker::getInstance()->ErrMatrix[cameraIdx],true);
        m_SceneArr[cameraIdx+2]->updateShow();
    }



     //主界面显示光电数据
      if(static_cast<int>(PackageChecker::getInstance()->Options->ephDisplay()) )
      {

          int size=PackageChecker::getInstance()->QueRetPheKT.size();


           int size2=PackageChecker::getInstance()->QueRetMapPheKT.size();

          //空头
          if(PackageChecker::getInstance()->QueRetPheKT.size()>0 &&PackageChecker::getInstance()->QueRetMapPheKT.size()>0&& cameraIdx==2)
          {

              PackageChecker::getInstance()->m_MutexRetPheKT.lock();
              std::vector<int> tmp;
              tmp=PackageChecker::getInstance()->QueRetPheKT.front();
              PackageChecker::getInstance()->QueRetPheKT.pop();

              std::vector<int> tmp2;
              tmp2=PackageChecker::getInstance()->QueRetMapPheKT.front();
              PackageChecker::getInstance()->QueRetMapPheKT.pop();


              m_SceneArr[cameraIdx]->updateTextIterm(tmp.data(),tmp2.data());

              if(tmp[20]==1)
                  m_SceneArr[cameraIdx+2]->updateTextIterm(tmp.data(),tmp2.data());

                PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
          }

           if((PackageChecker::getInstance()->QueRetPheKT.size()==0 || PackageChecker::getInstance()->QueRetMapPheKT.size()==0) && cameraIdx==2)
           {
               m_SceneArr[cameraIdx]->hidePheText();
               m_SceneArr[cameraIdx+2]->hidePheText();
           }



          //缺支
          if(PackageChecker::getInstance()->QueRetPheQZ.size()>0 &&PackageChecker::getInstance()->QueRetMapPheQZ.size()>0&&cameraIdx==1)
          {


              PackageChecker::getInstance()->m_MutexRetPheQZ.lock();

              std::vector<int> tmp;
              tmp=PackageChecker::getInstance()->QueRetPheQZ.front();
              PackageChecker::getInstance()->QueRetPheQZ.pop();

              std::vector<int> tmp2;
              tmp2=PackageChecker::getInstance()->QueRetMapPheQZ.front();
              PackageChecker::getInstance()->QueRetMapPheQZ.pop();



              m_SceneArr[cameraIdx]->updateTextIterm(tmp.data(),tmp2.data());

              if(tmp[20]==1)
                  m_SceneArr[cameraIdx+2]->updateTextIterm(tmp.data(),tmp2.data());

                 PackageChecker::getInstance()->m_MutexRetPheQZ.unlock();
          }

           if((PackageChecker::getInstance()->QueRetPheQZ.size()==0 || PackageChecker::getInstance()->QueRetMapPheQZ.size()==0)&& cameraIdx==1)
           {
               m_SceneArr[cameraIdx]->hidePheText();
               m_SceneArr[cameraIdx+2]->hidePheText();
           }
      }
      else
      {
           m_SceneArr[cameraIdx]->hidePheText();
           m_SceneArr[cameraIdx+2]->hidePheText();
      }

}

void MainWindow::doCameraOffline(int camIndex)
{
    //停止线程运行
    //    m_ThreadRun[camIndex]->stop();

    //关闭相机重新打开
    PackageChecker::getInstance()->Cameras[camIndex+1]->Close();

    while (true)
    {
        QThread::msleep(1000);
        bool ret=false;
        ret=  PackageChecker::getInstance()->Cameras[camIndex+1]->Open();
        if(ret)
            break;
    }

    PackageChecker::getInstance()->Cameras[camIndex+1]->SetGain( PackageChecker::getInstance()->CameraConfig->getGain(camIndex+1));
    PackageChecker::getInstance()-> Cameras[camIndex+1]->SetExposureTime( PackageChecker::getInstance()->CameraConfig->getExposureTime(camIndex+1));



    PackageChecker::getInstance()->Cameras[camIndex+1]->m_bIsOffline=false;

    //开启线程
    m_ThreadRun[camIndex]->start(QThread::HighestPriority);

    PackageChecker::getInstance()->RunParam_CalcNumAll=0;
    PackageChecker::getInstance()->RunParam_CalcNumNg=0;

    int calNum=0;
    for(int i=0;i<CAMERANUM_MAX;++i)
    {
        if(PackageChecker::getInstance()->RunParam_CalcNumAllCams[i]>calNum)
        {
            calNum=PackageChecker::getInstance()->RunParam_CalcNumAllCams[i];
        }
    }
    PackageChecker::getInstance()->RunParam_CalcNumAllCams[camIndex]=calNum;

    //    for(int idx_i=0;idx_i<CAMERANUM_MAX;++idx_i)
    //    {
    //        for(int idx_j=0;idx_j<CHECKOPERATORNUM_MAX;++idx_j)
    //        {
    //            pc->ErrRecord[idx_i][idx_j]=0;
    //        }
    //    }
    LOG(INFO)<<"断线重连完成"<<PackageChecker::getInstance()->Cameras[camIndex+1]->GetIPAddress();

}




void MainWindow::updateMainSceen()
{
#ifdef FALG_PHE

    //光电显示窗口
    if(m_imgPhe)
    {
        //memcpy(PackageChecker::getInstance()->RetPheKT,rets,sizeof (int)*20);

        PackageChecker::getInstance()->m_MutexRetPheKT.lock();
        m_SceneArr[m_imgPhe]->updateTextIterm(PackageChecker::getInstance()->RetPheKT,PackageChecker::getInstance()->RetMapPheKT);


        if(PackageChecker::getInstance()->RetPheKT[20])
        {
            m_SceneArr[m_imgPhe+2]->updateTextIterm(PackageChecker::getInstance()->RetPheKT,PackageChecker::getInstance()->RetMapPheKT);

            PackageChecker::getInstance()->RunParam_CalcNumNgTotals[m_imgPhe-1]++;

            PackageChecker::getInstance()->RunParam_CalcNumNgGDs[m_imgPhe-1]++;
            PackageChecker::getInstance()->RunParam_CalcNumNgCams[m_imgPhe-1]++;
        }

        PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
        m_SceneArr[m_imgPhe]->updateShow();
        m_SceneArr[m_imgPhe+2]->updateShow();



        PackageChecker::getInstance()->RunParam_CalcNumAllCams[m_imgPhe-1]++;



        updateStatistics( m_imgPhe);

    }
#endif

}

void MainWindow::InitControl()
{

#ifdef FALG_PHE

    connect(PackageChecker::getInstance(),&PackageChecker::updateCheckRetSig,&form_phe,&dlgphotoelectricitycfg::upDateCheckRet,Qt::BlockingQueuedConnection);
    connect(PackageChecker::getInstance(),&PackageChecker::sendCheckCommandSig,&form_phe,&dlgphotoelectricitycfg::sendRunCheckCommand);


    //    if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20)
    {
        //光电界面更新完成光电数据后，更新主界面光电数据，只有在20个探头情况下才会发出
        connect(&form_phe,&dlgphotoelectricitycfg::updateMainSceenSig,this,&MainWindow::updateMainSceen );
    }




#endif


    ui->btnMainImageDistory->setVisible(false);
    m_btnGroup=new QButtonGroup;
    m_btnGroup->addButton(this->ui->btnMainChangeBrand,1);
    m_btnGroup->addButton(this->ui->btnMainDistory,2);
    m_btnGroup->addButton(this->ui->btnMainImageDistory,3);
    m_btnGroup->addButton(this->ui->btnMainCheckConfig,4);
    m_btnGroup->addButton(this->ui->btnMainCameraSetting,5);
    m_btnGroup->addButton(this->ui->btnMainUserManage,6);
    m_btnGroup->addButton(this->ui->btnMainSystemConfig,7);
    m_btnGroup->addButton(this->ui->btnMainMin,8);
    m_btnGroup->addButton(this->ui->btnMainMax,9);
    m_btnGroup->addButton(this->ui->btnMainExit,10);
    m_btnGroup->addButton(this->ui->btnMainPhotoelecSetting,11);

#ifdef FALG_PHE
    this->ui->btnMainPhotoelecSetting->setVisible(true);
#else

    this->ui->btnMainPhotoelecSetting->setVisible(false);
#endif

    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));


    m_ViewArrSub[1]=ui->graphicsViewSub1;
    m_ViewArrSub[2]=ui->graphicsViewSub2;

    m_ViewArrSub[3]=ui->graphicsViewSub1_2;
    m_ViewArrSub[4]=ui->graphicsViewSub2_2;

    m_ViewArrMain[1]=ui->graphicsViewMain1;
    m_ViewArrMain[2]=ui->graphicsViewMain2;

    m_ViewArrMain[3]=ui->graphicsViewMain1_2;
    m_ViewArrMain[4]=ui->graphicsViewMain2_2;

    m_WidgetArr[1]=ui->widgetSub1;
    m_WidgetArr[2]=ui->widgetSub2;

    m_WidgetArr[3]=ui->widgetSub1_2;
    m_WidgetArr[4]=ui->widgetSub2_2;



    PackageChecker* pc=PackageChecker::getInstance();

    std::list<ImgPro*>::iterator itoImgPro=pc->ImgTobaccoRun->LstImgPro.begin();


    //图像显示窗口
    for (int i=0;itoImgPro!=pc->ImgTobaccoRun->LstImgPro.end();++itoImgPro,i++)
    {
        int imgIdx=(*itoImgPro)->ImgIndex;

        if(imgIdx==1)
            m_dScale=m_dScale1;
        if(imgIdx==2)
            m_dScale=m_dScale2;

#ifdef IMG_TYPE_GRAY
        GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.ptr<uchar>(0),
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.cols,
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.rows,imgIdx);
#endif
#ifdef IMG_TYPE_RGB
        GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.ptr<uchar>(0),
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.cols,
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.rows,imgIdx);



        GraphicsSceneMain* newGraphicsScene2=new GraphicsSceneMain(
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.ptr<uchar>(0),
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.cols,
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.rows,imgIdx);

#endif


        m_SceneArr[imgIdx]=newGraphicsScene;
        m_SceneArr[imgIdx+2]=newGraphicsScene2;
        for (list<CheckOperatorBasic*>::iterator itorCo= (*itoImgPro)->LstCheckOperator.begin();itorCo!= (*itoImgPro)->LstCheckOperator.end();++itorCo)
        {
            newGraphicsScene->addRect((*itorCo));
            newGraphicsScene2->addRect((*itorCo));
        }
        updateCheckRect();


#ifdef FALG_PHE
        //主界面显示光电数据

        //767
        if( static_cast<int>(PackageChecker::getInstance()->Options->getArrangeType()) ==0)
        {
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][0][2]*(i)+m_phePosArr[imgIdx-1][0][0],m_phePosArr[imgIdx-1][0][1]);
                newGraphicsScene->addPheTextIterm(i,pf);
                newGraphicsScene2->addPheTextIterm(i,pf);
            }
            for(int i=0;i<6;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][1][2]*(i)+m_phePosArr[imgIdx-1][1][0],m_phePosArr[imgIdx-1][1][1]);
                newGraphicsScene->addPheTextIterm(i+7,pf);
                newGraphicsScene2->addPheTextIterm(i+7,pf);
            }
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][2][2]*(i)+m_phePosArr[imgIdx-1][2][0],m_phePosArr[imgIdx-1][2][1]);
                newGraphicsScene->addPheTextIterm(i+13,pf);
                newGraphicsScene2->addPheTextIterm(i+13,pf);
            }
        }
        //677
        if( static_cast<int>(PackageChecker::getInstance()->Options->getArrangeType()) ==1)
        {
            for(int i=0;i<6;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][0][2]*(i)+m_phePosArr[imgIdx-1][0][0],m_phePosArr[imgIdx-1][0][1]);
                newGraphicsScene->addPheTextIterm(i,pf);
                newGraphicsScene2->addPheTextIterm(i,pf);
            }
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][1][2]*(i)+m_phePosArr[imgIdx-1][1][0],m_phePosArr[imgIdx-1][1][1]);
                newGraphicsScene->addPheTextIterm(i+6,pf);
                newGraphicsScene2->addPheTextIterm(i+6,pf);
            }
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][2][2]*(i)+m_phePosArr[imgIdx-1][2][0],m_phePosArr[imgIdx-1][2][1]);
                newGraphicsScene->addPheTextIterm(i+13,pf);
                newGraphicsScene2->addPheTextIterm(i+13,pf);
            }
        }
        //1010
        if( static_cast<int>(PackageChecker::getInstance()->Options->getArrangeType()) ==2)
        {
            for(int i=0;i<10;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][0][2]*(i)+m_phePosArr[imgIdx-1][0][0],m_phePosArr[imgIdx-1][0][1]);
                newGraphicsScene->addPheTextIterm(i,pf);
                newGraphicsScene2->addPheTextIterm(i,pf);
            }
            for(int i=0;i<10;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-1][1][2]*(i)+m_phePosArr[imgIdx-1][1][0],m_phePosArr[imgIdx-1][1][1]);
                newGraphicsScene->addPheTextIterm(i+10,pf);
                newGraphicsScene2->addPheTextIterm(i+10,pf);
            }

        }





        if(static_cast<int>(PackageChecker::getInstance()->Options->ephDisplay())==0 )
        {
            newGraphicsScene->hidePheText();
            newGraphicsScene2->hidePheText();
        }

#endif

        m_ViewArrSub[imgIdx]->setScene(newGraphicsScene);
        m_ViewArrMain[imgIdx]->setScene(newGraphicsScene);

        m_ViewArrSub[imgIdx+2]->setScene(newGraphicsScene2);
        m_ViewArrMain[imgIdx+2]->setScene(newGraphicsScene2);


        DSDEBUG<<(double)PackageChecker::getInstance()->ImgHei<<"   "<<(double)MAINWINDOW_VIEWSUBWID/PackageChecker::getInstance()->ImgWid;
        // if((double)PackageChecker::getInstance()->ImgHei/PackageChecker::getInstance()->ImgWid>(double)MAINWINDOW_VIEWSUBHEI/MAINWINDOW_VIEWSUBWID)
        {
            this->m_ViewArrSub[imgIdx]->scaleto(m_dScale,m_dScale);
            m_ViewArrSub[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            this->m_ViewArrSub[imgIdx+2]->scaleto(m_dScale,m_dScale);
            m_ViewArrSub[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }


        if((double)PackageChecker::getInstance()->ImgHeis[i]/PackageChecker::getInstance()->ImgWids[i]>(double)MAINWINDOW_VIEWMAINHEI/MAINWINDOW_VIEWMAINWID)
        {
            this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                 (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
            m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],                                                (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
            m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }
        else
        {
            this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                 (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);
            m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));

            this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                   (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);
            m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }
    }



    //模拟板 在加一个光电显示窗口
    if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20&&pc->ImgTobaccoRun->LstImgPro.size()==1 )
    {
        int imgIdx=(*(--pc->ImgTobaccoRun->LstImgPro.end()))->ImgIndex;


        int i=imgIdx-1;
        if(imgIdx==1)
            m_dScale=m_dScale1;
        if(imgIdx==2)
            m_dScale=m_dScale2;
        imgIdx=imgIdx+1;
        m_imgPhe=imgIdx;


#ifdef IMG_TYPE_GRAY
        GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.ptr<uchar>(0),
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.cols,
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.rows,imgIdx);
#endif
#ifdef IMG_TYPE_RGB
        GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.ptr<uchar>(0),
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.cols,
                    PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.rows,imgIdx);

        GraphicsSceneMain* newGraphicsScene2=new GraphicsSceneMain(
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.ptr<uchar>(0),
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.cols,
                    PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.rows,imgIdx);

#endif
        m_SceneArr[imgIdx]=newGraphicsScene;
        m_SceneArr[imgIdx+2]=newGraphicsScene2;

#ifdef FALG_PHE

        newGraphicsScene->clearPheText();



        for(int i=0;i<6;i++)
        {
            QPointF pf(m_phePosArr[imgIdx-2][0][2]*(i)+m_phePosArr[imgIdx-2][0][0],m_phePosArr[imgIdx-2][0][1]);
            newGraphicsScene->addPheTextIterm(i,pf);
            newGraphicsScene2->addPheTextIterm(i,pf);
        }
        for(int i=0;i<7;i++)
        {
            QPointF pf(m_phePosArr[imgIdx-2][1][2]*(i)+m_phePosArr[imgIdx-2][1][0],m_phePosArr[imgIdx-2][1][1]);
            newGraphicsScene->addPheTextIterm(i+6,pf);
            newGraphicsScene2->addPheTextIterm(i+6,pf);
        }
        for(int i=0;i<7;i++)
        {
            QPointF pf(m_phePosArr[imgIdx-2][2][2]*(i)+m_phePosArr[imgIdx-2][2][0],m_phePosArr[imgIdx-2][2][1]);
            newGraphicsScene->addPheTextIterm(i+13,pf);
            newGraphicsScene2->addPheTextIterm(i+13,pf);
        }




#endif
        m_ViewArrSub[imgIdx]->setScene(newGraphicsScene);
        m_ViewArrMain[imgIdx]->setScene(newGraphicsScene);

        m_ViewArrSub[imgIdx+2]->setScene(newGraphicsScene2);
        m_ViewArrMain[imgIdx+2]->setScene(newGraphicsScene2);

        DSDEBUG<<(double)PackageChecker::getInstance()->ImgHei<<"   "<<(double)MAINWINDOW_VIEWSUBWID/PackageChecker::getInstance()->ImgWid;
        // if((double)PackageChecker::getInstance()->ImgHei/PackageChecker::getInstance()->ImgWid>(double)MAINWINDOW_VIEWSUBHEI/MAINWINDOW_VIEWSUBWID)
        {
            this->m_ViewArrSub[imgIdx]->scaleto(m_dScale,m_dScale);
            m_ViewArrSub[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            this->m_ViewArrSub[imgIdx+2]->scaleto(m_dScale,m_dScale);
            m_ViewArrSub[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }


        if((double)PackageChecker::getInstance()->ImgHeis[i]/PackageChecker::getInstance()->ImgWids[i]>(double)MAINWINDOW_VIEWMAINHEI/MAINWINDOW_VIEWMAINWID)
        {
            this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                 (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
            m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                   (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
            m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }
        else
        {
            this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                 (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);

            m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));


            this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                   (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);

            m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
        }

        int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size()+1;
        for (int i=1;i<=tmpCameraNum;++i)
        {
            if(i<=tmpCameraNum)
            {
                m_WidgetArr[i]->setVisible(true);
                m_WidgetArr[i+2]->setVisible(true);
            }
            else
            {
                m_WidgetArr[i]->setVisible(false);
                m_WidgetArr[i+2]->setVisible(false);
            }
        }
    }

    else
    {
        int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size();


        if(tmpCameraNum==1)
        {
            m_WidgetArr[1]->setVisible(true);
            m_WidgetArr[3]->setVisible(true);

            m_WidgetArr[2]->setVisible(false);
            m_WidgetArr[4]->setVisible(false);
        }

        //        for (int i=1;i<=tmpCameraNum;++i)
        //        {
        //            if(i<=tmpCameraNum)
        //            {
        //                m_WidgetArr[i]->setVisible(true);
        //                m_WidgetArr[i+2]->setVisible(true);
        //            }
        //            else
        //            {
        //                m_WidgetArr[i]->setVisible(false);
        //                m_WidgetArr[i+2]->setVisible(false);
        //            }
        //        }

    }



    ui->stackedWidget->setCurrentIndex(0);
}




void MainWindow::updateSceneRect()
{
    m_MutexScene.lock();
    QMap<int,GraphicsSceneMain*>::iterator itor= m_SceneArr.begin();

    int i=0;
    for (;itor!=m_SceneArr.end();++itor)
    {
        if(i<PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size()){i++;}
        else{break;}


        itor.value()->clearRect();
        ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoRun);
        int key=itor.key();
        if(key>=3)
            key=key-2;
        ImgPro* imgProTmp=control.getImgPro(key);
        for (list<CheckOperatorBasic*>::iterator itorCo=imgProTmp->LstCheckOperator.begin();itorCo!= imgProTmp->LstCheckOperator.end();++itorCo)
        {
            itor.value()->addRect((*itorCo));
        }
    }
    m_MutexScene.unlock();
    updateCheckRect();
}

void MainWindow::updateGPIO(int pin, int val)
{
    PackageChecker* pc=PackageChecker::getInstance();
    pc->IOContol->setLevel(pin,0xFFFFFFFF,val);
}

void MainWindow::updateStatistics(int cameraIdx)
{
    PackageChecker* pc=PackageChecker::getInstance();

    if(cameraIdx==1)
    {
        pc->RunParam_CalcNumAll=pc->RunParam_CalcNumAllCams[cameraIdx-1];
        pc->RunParam_CalcNumNg=pc->RunParam_CalcNumNgCams[cameraIdx-1];


        ui->lcdNumAllNum_Cam1->display(QString::number(pc->RunParam_CalcNumAllCams[cameraIdx-1]));
        ui->lcdNumNgNum_Cam1->display(QString::number(pc->RunParam_CalcNumNgCams[cameraIdx-1]));

        ui->lcdNumNgNum_GD1->display(QString::number(pc->RunParam_CalcNumNgGDs[cameraIdx-1]));
        ui->lcdNumNgNum_Total1->display(QString::number(pc->RunParam_CalcNumNgTotals[cameraIdx-1]));

        if(pc->RunParam_CalcNumAllCams[cameraIdx-1]!=0)
            ui->lcdNumNgPercetange_Cam1->display(QString("%1").arg(QString::number(((double)pc->RunParam_CalcNumNgTotals[cameraIdx-1]/pc->RunParam_CalcNumAllCams[cameraIdx-1])*100,'f',2)));
        else
            ui->lcdNumNgPercetange_Cam1->display(QString("%1").arg(QString::number(0,'f',4)));
        //ui->speedPanel->setCurentSpeed(pc->RunParam_CalcTime);
        ui->lcdNumCalcuTime->display(QString::number(pc->RunParam_CalcTimeCams[cameraIdx-1]));

    }

    if(cameraIdx==2)
    {
        ui->lcdNumAllNum_Cam2->display(QString::number(pc->RunParam_CalcNumAllCams[cameraIdx-1]));
        ui->lcdNumNgNum_Cam2->display(QString::number(pc->RunParam_CalcNumNgCams[cameraIdx-1]));

        ui->lcdNumNgNum_GD2->display(QString::number(pc->RunParam_CalcNumNgGDs[cameraIdx-1]));
        ui->lcdNumNgNum_Total2->display(QString::number(pc->RunParam_CalcNumNgTotals[cameraIdx-1]));

        if(pc->RunParam_CalcNumAllCams[cameraIdx-1]!=0)
            ui->lcdNumNgPercetange_Cam2->display(QString("%1").arg(QString::number(((double)pc->RunParam_CalcNumNgTotals[cameraIdx-1]/pc->RunParam_CalcNumAllCams[cameraIdx-1])*100,'f',2)));
        else
            ui->lcdNumNgPercetange_Cam2->display(QString("%1").arg(QString::number(0,'f',4)));
        //  ui->speedPanel->setCurentSpeed(pc->RunParam_CalcTime);
        ui->lcdNumCalcuTime_2->display(QString::number(pc->RunParam_CalcTimeCams[cameraIdx-1]));
    }

    if(cameraIdx==3)
    {
        ui->lcdNumAllNum_Cam3->display(QString::number(pc->RunParam_CalcNumAllCams[cameraIdx-1]));
        ui->lcdNumNgNum_Cam3->display(QString::number(pc->RunParam_CalcNumNgCams[cameraIdx-1]));


        ui->lcdNumNgNum_GD3->display(QString::number(pc->RunParam_CalcNumNgGDs[cameraIdx-1]));

        ui->lcdNumNgNum_Total3->display(QString::number(pc->RunParam_CalcNumNgTotals[cameraIdx-1]));

        if(pc->RunParam_CalcNumAllCams[cameraIdx-1]!=0)
            ui->lcdNumNgPercetange_Cam3->display(QString("%1").arg(QString::number(((double)pc->RunParam_CalcNumNgTotals[cameraIdx-1]/pc->RunParam_CalcNumAllCams[cameraIdx-1])*100,'f',2)));
        else
            ui->lcdNumNgPercetange_Cam3->display(QString("%1").arg(QString::number(0,'f',4)));
        //  ui->speedPanel->setCurentSpeed(pc->RunParam_CalcTime);
        ui->lcdNumCalcuTime_3->display(QString::number(pc->RunParam_CalcTimeCams[cameraIdx-1]));
    }

    if(cameraIdx==4)
    {
        ui->lcdNumAllNum_Cam4->display(QString::number(pc->RunParam_CalcNumAllCams[cameraIdx-1]));
        ui->lcdNumNgNum_Cam4->display(QString::number(pc->RunParam_CalcNumNgCams[cameraIdx-1]));

        ui->lcdNumNgNum_GD4->display(QString::number(pc->RunParam_CalcNumNgGDs[cameraIdx-1]));

        ui->lcdNumNgNum_Total4->display(QString::number(pc->RunParam_CalcNumNgTotals[cameraIdx-1]));

        if(pc->RunParam_CalcNumAllCams[cameraIdx-1]!=0)
            ui->lcdNumNgPercetange_Cam4->display(QString("%1").arg(QString::number(((double)pc->RunParam_CalcNumNgTotals[cameraIdx-1]/pc->RunParam_CalcNumAllCams[cameraIdx-1])*100,'f',2)));
        else
            ui->lcdNumNgPercetange_Cam4->display(QString("%1").arg(QString::number(0,'f',4)));
        //  ui->speedPanel->setCurentSpeed(pc->RunParam_CalcTime);
        ui->lcdNumCalcuTime_4->display(QString::number(pc->RunParam_CalcTimeCams[cameraIdx-1]));
    }

}



MainWindow::~MainWindow()
{
    DSClsLastConfig * lastConfig=DSClsLastConfig::getInstance();
    lastConfig->setLastBrand(DSSystemParam::BrandName);
    lastConfig->setLastCheckNum(PackageChecker::getInstance()->RunParam_CalcNumAll);
    lastConfig->setLastNgNum(PackageChecker::getInstance()->RunParam_CalcNumNg);
    lastConfig->setLastShift(PackageChecker::getInstance()->RunParam_Shift);
    lastConfig->setDate(QDate::currentDate());

    lastConfig->save();

    for(int idx=0;idx<CAMERA_NUM_USER;++idx)
    {
        delete  this->m_ThreadRun[idx];
    }
    //    PackageChecker::getInstance()->IOContol->setLevel(RUNPIN,0xFFFFFFFF,0xFF);
    delete ui;
}

void MainWindow::onTimeout()
{
    ui->lcdNumCurTime->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    int sunSec=QDateTime::currentDateTime().toTime_t()-PackageChecker::m_TimeSystemStart.toTime_t();

    //    sunSec=sunSec+60*3600;
    //    QTime dt;
    //    dt.setHMS((int)(sunSec/3600),(sunSec%3600)/60,(sunSec%3600)%60);
    //dt.time().setHMS((int)(sunSec/3600),(sunSec%3600)/60,(sunSec%3600)%60);
    //DSDEBUG<<dt.toString("hh:mm:ss")<<endl;


    int hour=sunSec/3600;
    int muniter=(sunSec-hour*3600)/60;
    int second=sunSec%60;
    QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(muniter,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    QString totalRuntime=QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(muniter,2,10,QChar('0')).arg(second,2,10,QChar('0'));

    ui->lcdNumRunTime->display(totalRuntime);

    if(QTime::currentTime().toString("hh:mm:ss")==m_deleteTime)
    {
        QtConcurrent::run(this,&MainWindow::runDeleteImgs);
    }

    if(onWarning)
    {
        //10秒后消除报警
        countSeconds++;
        if(countSeconds==10)
        {
            PackageChecker* pc=PackageChecker::getInstance();
            pc->IOContol->setLevel(6,0xFFFFFFFF,0xFF);
            onWarning=false;
            for (int i=0;i<CAMERANUM_MAX;i++)
            {
                pc->WarningFlag[i]=0;
            }
            countSeconds=0;
        }
    }


    //    //夜里1点停止检测
    //    if(QTime::currentTime().toString("hh:mm:ss")=="01:00:00")
    //    {
    //        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
    //         on_pushButtonRun_clicked();

    //    }
    //     //早上7点开始检测
    //    if(QTime::currentTime().toString("hh:mm:ss")=="07:00:00")
    //    {
    //        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_STOPPING)
    //         on_pushButtonRun_clicked();
    //    }


    //    //获取针脚电平信号
    int getVal=-1;
    //获取输入0号针脚的电平值,缺支剔除异常

    PackageChecker* pc=PackageChecker::getInstance();
    pc->IOContol->getLevel(0x00,getVal);
    if(m_lastPinVal==1&&getVal==0)
    {
        //高电平，要报警
        m_Timer1->start(500);
        m_bIsWarning1=true;
    }
    m_lastPinVal=getVal;




    //获取输入1号针脚的电平值,空头剔除异常
    int getVal2=-1;
    pc->IOContol->getLevel(0x01,getVal2);

    //    DSDEBUG<<"getPinVal: "<<getVal2<<endl;

    if(m_lastPinVal2==1&&getVal2==0)
    {
        //高电平，要报警
        m_Timer2->start(500);
        m_bIsWarning2=true;
    }


    m_lastPinVal2=getVal2;
}

void MainWindow::onTimeout1()
{
    m_warningCnt1++;
    if( m_warningCnt1%2==0)
    {
        ui->label->setStyleSheet("color:rgb(255,0,0)");
        ui->label->setText(tr("剔除异常报警！"));
        ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    else
    {

        ui->label->setStyleSheet("color:rgb(255,0,255)");
        ui->label->setText(tr("点击消除报警"));
        ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

}

void MainWindow::onTimeout2()
{
    m_warningCnt2++;
    if( m_warningCnt2%2==0)
    {
        ui->label->setStyleSheet("color:rgb(255,0,0)");
        ui->label->setText(tr("剔除异常报警！"));
        ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    else
    {
        ui->label->setStyleSheet("color:rgb(255,0,255)");
        ui->label->setText(tr("点击消除报警"));
        ui->label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

}




void MainWindow::runDeleteImgs()
{
    QVector<QString> files;
    //删除NG图像
    QString saveName=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/NG/";
    FileHelper::getAllFileFolder(saveName,files);
    QDate curDate=QDate::currentDate();
    int days=PackageChecker::getInstance()->Options->SaveDaysNum();

    for(int idx=0;idx<files.size();++idx)
    {
        QDate tmpDate=QDate::fromString(files[idx],"yyyy-MM-dd");
        if(tmpDate.addDays(days)<curDate)
        {
            FileHelper::deleteDirAll(saveName+files[idx]);
        }
    }

    //删除OK图像
    files.clear();
    saveName=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/OK/";
    FileHelper::getAllFileFolder(saveName,files);
    curDate=QDate::currentDate();
    days=PackageChecker::getInstance()->Options->SaveDaysNum();

    for(int idx=0;idx<files.size();++idx)
    {
        QDate tmpDate=QDate::fromString(files[idx],"yyyy-MM-dd");
        if(tmpDate.addDays(days)<curDate)
        {
            FileHelper::deleteDirAll(saveName+files[idx]);
        }
    }

    //    //删除视频
    //     saveName=PackageChecker::getInstance()->Options->ImgSavePath()+"/视频保存/"+DSSystemParam::BrandName+"/NG/";
    //     files.clear();
    //     FileHelper::getAllFileFolder(saveName,files);
    //     curDate=QDate::currentDate();
    //     days=PackageChecker::getInstance()->Options->SaveDaysNum();

    //    for(int idx=0;idx<files.size();++idx)
    //    {
    //        QDate tmpDate=QDate::fromString(files[idx],"yyyy-MM-dd");
    //        if(tmpDate.addDays(days)<curDate)
    //        {
    //            FileHelper::deleteDirAll(saveName+files[idx]);
    //        }
    //    }
}

void MainWindow::updateRunLog()
{
    //    m_ModelLog->clear();
    //    QStringList lstStr;
    //    QQueue<QString> queueLog=PackageChecker::getInstance()->systemRunLog->getInfo();
    //    QStringList strLst;
    //    while(queueLog.size()>0)
    //    {
    //        lstStr.push_back(queueLog.head()) ;
    //        queueLog.dequeue();
    //    }
    //    int nCount = lstStr.size();
    //    for(int i = 0; i < nCount; i++)
    //    {
    //        QString string = static_cast<QString>(lstStr.at(i));
    //        QStandardItem *item = new QStandardItem(string);
    //        if(i % 2 == 1)
    //        {
    //            QLinearGradient linearGrad(QPointF(0, 0), QPointF(100, 100));
    //            linearGrad.setColorAt(0, Qt::darkGreen);
    //            linearGrad.setColorAt(1, Qt::yellow);
    //            QBrush brush(linearGrad);
    //            item->setBackground(brush);
    //        }
    //        else
    //        {
    //            QBrush brush(QColor(255,255,255));
    //            item->setBackground(brush);
    //        }
    //        m_ModelLog->appendRow(item);
    //    }

    //    ui->listViewRunLog->update();
    //    ui->listViewRunLog->scrollToBottom();
}




void MainWindow::updateCheckRect()
{
    m_MutexScene.lock();
    PackageChecker* pc=PackageChecker::getInstance();
    QMap<int,GraphicsSceneMain*>::iterator itor= m_SceneArr.begin();
    for(;itor!=m_SceneArr.end();++itor)
    {
        if(itor.key()>=3)
        {
            itor.value()->updateRectState(PackageChecker::getInstance()->ErrMatrix[itor.key()],true);
            itor.value()->updateShow();
        }
        else
        {
            itor.value()->updateRectState(PackageChecker::getInstance()->ErrMatrix[itor.key()],false);
            itor.value()->updateShow();
        }
    }

    m_MutexScene.unlock();
}


void MainWindow::buttonJudge(int idx)
{
    switch (idx)
    {
    case 1:
    {
        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            return;
        }

        QString oldBrand=DSSystemParam::BrandName;

        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("系统正在运行,请先停止运行!"), 0);
            msg->exec();
            return;
        }

        DlgBrandManage dlg;
        dlg.exec();

        if(DSSystemParam::BrandName==oldBrand) return;


        PackageChecker::getInstance()->changeBrand(DSSystemParam::BrandName);
        ui->labelBrandName->setText(DSSystemParam::BrandName);
        QMap<int,GraphicsSceneMain*>::iterator itorScene= m_SceneArr.begin();
        {
            for (;itorScene!=m_SceneArr.end();++itorScene)
            {
                (*itorScene)->clear();
            }
        }
        m_SceneArr.clear();

        PackageChecker::getInstance()->changeBrand(DSSystemParam::BrandName);
        for (int i=1;i<=PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size();++i)
        {

            GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                        PackageChecker::getInstance()->CurImage[i].ImageRGB.ptr<uchar>(0),
                        PackageChecker::getInstance()->CurImage[i].ImageRGB.cols,
                        PackageChecker::getInstance()->CurImage[i].ImageRGB.rows,i);

            GraphicsSceneMain* newGraphicsScene2=new GraphicsSceneMain(
                        PackageChecker::getInstance()->m_qmapCurBadImage[i].ImageRGB.ptr<uchar>(0),
                        PackageChecker::getInstance()->CurImage[i].ImageRGB.cols,
                        PackageChecker::getInstance()->CurImage[i].ImageRGB.rows,i);

            PackageChecker* pc=  PackageChecker::getInstance();
            //  cv::imwrite("d:/1.bmp",PackageChecker::getInstance()->CurImage[i].ImageGray);

            ImgTobaccoControl  imgTobaccoControl (PackageChecker::getInstance()->ImgTobaccoRun);
            for (list<CheckOperatorBasic*>::iterator itorCo= imgTobaccoControl.getImgPro(i)->LstCheckOperator.begin();itorCo!=  imgTobaccoControl.getImgPro(i)->LstCheckOperator.end();++itorCo)
            {
                newGraphicsScene->addRect((*itorCo));
                newGraphicsScene2->addRect((*itorCo));
            }

            m_SceneArr[i]=newGraphicsScene;
            m_SceneArr[i+2]=newGraphicsScene2;
            m_ViewArrSub[i]->setScene(newGraphicsScene);
            m_ViewArrMain[i]->setScene(newGraphicsScene);
            m_ViewArrSub[i+2]->setScene(newGraphicsScene2);
            m_ViewArrMain[i+2]->setScene(newGraphicsScene2);

            //  if((double)PackageChecker::getInstance()->ImgHei/PackageChecker::getInstance()->ImgWid>(double)MAINWINDOW_VIEWSUBHEI/MAINWINDOW_VIEWSUBWID)
            {
                this->m_ViewArrSub[i]->scaleto(m_dScale,m_dScale);
                m_ViewArrSub[i]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i-1],(double)PackageChecker::getInstance()->ImgHeis[i-1]));
                this->m_ViewArrSub[i+2]->scaleto(m_dScale,m_dScale);
                m_ViewArrSub[i+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i-1],(double)PackageChecker::getInstance()->ImgHeis[i-1]));
            }

            if((double)PackageChecker::getInstance()->ImgHeis[i]/PackageChecker::getInstance()->ImgWids[i]>(double)MAINWINDOW_VIEWMAINHEI/MAINWINDOW_VIEWMAINWID)
            {
                this->m_ViewArrMain[i]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
                m_ViewArrMain[i]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWid,(double)PackageChecker::getInstance()->ImgHei));

                this->m_ViewArrMain[i+2]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                  (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
                m_ViewArrMain[i+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));

            }
            else
            {
                this->m_ViewArrMain[i]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);

                m_ViewArrMain[i]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));

                this->m_ViewArrMain[i+2]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                  (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);
                m_ViewArrMain[i+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            }
        }





        //模拟板 在加一个光电显示窗口

        if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20&& PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size()==1)
        {
            PackageChecker* pc=  PackageChecker::getInstance();

            int imgIdx=(*(--pc->ImgTobaccoRun->LstImgPro.end()))->ImgIndex;

            int i=imgIdx-1;
            if(imgIdx==1)
                m_dScale=m_dScale1;
            if(imgIdx==2)
                m_dScale=m_dScale2;
            imgIdx=imgIdx+1;
            m_imgPhe=imgIdx;


#ifdef IMG_TYPE_GRAY
            GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.ptr<uchar>(0),
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.cols,
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageGray.rows,imgIdx);
#endif
#ifdef IMG_TYPE_RGB
            GraphicsSceneMain* newGraphicsScene=new GraphicsSceneMain(
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.ptr<uchar>(0),
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.cols,
                        PackageChecker::getInstance()->CurImage[imgIdx].ImageRGB.rows,imgIdx);

            GraphicsSceneMain* newGraphicsScene2=new GraphicsSceneMain(
                        PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.ptr<uchar>(0),
                        PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.cols,
                        PackageChecker::getInstance()->m_qmapCurBadImage[imgIdx].ImageRGB.rows,imgIdx);

#endif
            m_SceneArr[imgIdx]=newGraphicsScene;
            m_SceneArr[imgIdx+2]=newGraphicsScene2;

#ifdef FALG_PHE

            newGraphicsScene->clearPheText();
            for(int i=0;i<6;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-2][0][2]*(i)+m_phePosArr[imgIdx-2][0][0],m_phePosArr[imgIdx-2][0][1]);
                newGraphicsScene->addPheTextIterm(i,pf);
                newGraphicsScene2->addPheTextIterm(i,pf);
            }
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-2][1][2]*(i)+m_phePosArr[imgIdx-2][1][0],m_phePosArr[imgIdx-2][1][1]);
                newGraphicsScene->addPheTextIterm(i+6,pf);
                newGraphicsScene2->addPheTextIterm(i+6,pf);
            }
            for(int i=0;i<7;i++)
            {
                QPointF pf(m_phePosArr[imgIdx-2][2][2]*(i)+m_phePosArr[imgIdx-2][2][0],m_phePosArr[imgIdx-2][2][1]);
                newGraphicsScene->addPheTextIterm(i+13,pf);
                newGraphicsScene2->addPheTextIterm(i+13,pf);
            }

#endif
            m_ViewArrSub[imgIdx]->setScene(newGraphicsScene);
            m_ViewArrMain[imgIdx]->setScene(newGraphicsScene);

            m_ViewArrSub[imgIdx+2]->setScene(newGraphicsScene2);
            m_ViewArrMain[imgIdx+2]->setScene(newGraphicsScene2);

            DSDEBUG<<(double)PackageChecker::getInstance()->ImgHei<<"   "<<(double)MAINWINDOW_VIEWSUBWID/PackageChecker::getInstance()->ImgWid;
            // if((double)PackageChecker::getInstance()->ImgHei/PackageChecker::getInstance()->ImgWid>(double)MAINWINDOW_VIEWSUBHEI/MAINWINDOW_VIEWSUBWID)
            {
                this->m_ViewArrSub[imgIdx]->scaleto(m_dScale,m_dScale);
                m_ViewArrSub[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
                this->m_ViewArrSub[imgIdx+2]->scaleto(m_dScale,m_dScale);
                m_ViewArrSub[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            }


            if((double)PackageChecker::getInstance()->ImgHeis[i]/PackageChecker::getInstance()->ImgWids[i]>(double)MAINWINDOW_VIEWMAINHEI/MAINWINDOW_VIEWMAINWID)
            {
                this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                     (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
                m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
                this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i],
                                                       (double)MAINWINDOW_VIEWMAINHEI/(double)PackageChecker::getInstance()->ImgHeis[i]);
                m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            }
            else
            {
                this->m_ViewArrMain[imgIdx]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                     (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);

                m_ViewArrMain[imgIdx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));


                this->m_ViewArrMain[imgIdx+2]->scaleto((double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i],
                                                       (double)MAINWINDOW_VIEWMAINWID/PackageChecker::getInstance()->ImgWids[i]);

                m_ViewArrMain[imgIdx+2]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[i],(double)PackageChecker::getInstance()->ImgHeis[i]));
            }

            int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size()+1;
            for (int i=1;i<=tmpCameraNum;++i)
            {
                if(i<=tmpCameraNum)
                {
                    m_WidgetArr[i]->setVisible(true);
                    m_WidgetArr[i+2]->setVisible(true);
                }
                else
                {
                    m_WidgetArr[i]->setVisible(false);
                    m_WidgetArr[i+2]->setVisible(false);
                }
            }
        }

        else
        {
            int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size();

            if(tmpCameraNum==1)
            {
                m_WidgetArr[1]->setVisible(true);
                m_WidgetArr[3]->setVisible(true);

                m_WidgetArr[2]->setVisible(false);
                m_WidgetArr[4]->setVisible(false);
            }


            //            for (int i=1;i<=tmpCameraNum;++i)
            //            {
            //                if(i<=tmpCameraNum)
            //                {
            //                    m_WidgetArr[i]->setVisible(true);
            //                    m_WidgetArr[i+2]->setVisible(true);
            //                }
            //                else
            //                {
            //                    m_WidgetArr[i]->setVisible(false);
            //                    m_WidgetArr[i+2]->setVisible(false);
            //                }
            //            }

        }


        for(int idx_r=0;idx_r<CAMERANUM_MAX;++idx_r)
        {
            for(int idx_c=0;idx_c<CHECKOPERATORNUM_MAX;++idx_c)
            {
                PackageChecker::getInstance()->ErrMatrix[idx_r][idx_c]=0;
            }
        }

        auto itor= m_SceneArr.begin();
        for(;itor!=m_SceneArr.end();++itor)
        {
            if(itor.key()>=3)
            {
                itor.value()->updateShow();
                itor.value()->updateRectState(PackageChecker::getInstance()->ErrMatrix[itor.key()],true);
            }
            else
            {
                itor.value()->updateShow();
                itor.value()->updateRectState(PackageChecker::getInstance()->ErrMatrix[itor.key()],false);
            }
        }




        ui->lcdNumCalcuTime->display(QString::number(0));
        DSClsLastConfig * lastConfig=DSClsLastConfig::getInstance();
        lastConfig->setLastBrand(DSSystemParam::BrandName);
        lastConfig->save();

    }
        break;
    case 2:
    {
        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            return;
        }

        RecordDialog dlg;
        dlg.exec();
    }
        break;
    case 3:
    {
        DlgImageRecord form;
        form.exec();
    }
        break;
    case 4:
    {
        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            return;
        }

        DlgCheckOperator frm;frm.exec();
        if(DSSystemParam::SystemState!=DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
        {
            updateSceneRect();
            updateCheckRect();
        }
    }
        break;
    case 5:
    {

        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("系统正在运行,请先停止运行!"), 0);
            msg->exec();
            //delete  msg;
            return;
        }

        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            //delete  msg;
            return;
        }

        DlgCameraConfig frm;
        //DlgCameraLightConfig frm;
        frm.exec();

    }
        break;
    case 6:
    {
        DlgUserManage form;
        form.exec();
    }
        break;
    case 7:
    {

        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            return;
        }

        DlgSystemConfig form;
        connect(PackageChecker::getInstance()->m_cBaseServerTCP, SIGNAL(sendConnectStatus(int)), &form, SLOT(updateConncetStatus(int)));
        form.exec();

        m_MutexScene.lock();
        PackageChecker* pc=PackageChecker::getInstance();
        QMap<int,GraphicsSceneMain*>::iterator itor= m_SceneArr.begin();
        for(;itor!=m_SceneArr.end();++itor)
        {
            itor.value()->updateRectState(PackageChecker::getInstance()->ErrMatrix[itor.key()]);
            itor.value()->updateShow();
        }

        //        m_SceneArr[1]->updateShow(true);
        //        m_SceneArr[2]->updateShow(true);
        //        m_SceneArr[3]->updateShow(false);
        //        m_SceneArr[4]->updateShow(false);

        //        m_SceneArr[1]->updateRectState(PackageChecker::getInstance()->ErrMatrix[1]);
        //        m_SceneArr[2]->updateRectState(PackageChecker::getInstance()->ErrMatrix[2]);
        //    m_SceneArr[3]->updateRectState(false);
        //    m_SceneArr[4]->updateRectState(false);


        m_MutexScene.unlock();
    }
        break;
    case 8:
    {
        this->showMinimized();
    }
        break;
    case 9:
    {
        if (max)
        {
            this->setGeometry(location);
            ui->btnMainMax->setToolTip("最大化");
            this->ui->btnMainMax->setStyleSheet("QPushButton#btnMainMax{border-radius:0px;border-image:url(:/最大化.png); background-color:transparent; border-style:none;}"
                                                "QPushButton#btnMainMax:hover{border-radius:0px;border-image:url(:/最大化.png); background-color:#5F5F5F;border-style:none;}");
            QMap<int,GraphicsView*>::iterator it= m_ViewArrSub.begin();
            for(;it!=m_ViewArrSub.end();++it)

            {
                it.value()->scaleMin();
                it.value()->scaleMin();
            }
            QMap<int,GraphicsView*>::iterator itmain= m_ViewArrMain.begin();
            for(;itmain!=m_ViewArrMain.end();++itmain)
            {
                //itmain.value()->scaleto(1,1);
                itmain.value()->scaleMin();
                itmain.value()->scaleMin();
            }
        }
        else {
            location = this->geometry();
            this->setGeometry(qApp->desktop()->availableGeometry());

            ui->btnMainMax->setToolTip(tr("还原"));
            this->ui->btnMainMax->setStyleSheet("QPushButton#btnMainMax{border-radius:0px;border-image:url(:/最大化还原.png); background-color:transparent; border-style:none;}"
                                                "QPushButton#btnMainMax:hover{border-radius:0px;border-image:url(:/最大化还原.png); background-color:#5F5F5F;border-style:none;}");
            QMap<int,GraphicsView*>::iterator it= m_ViewArrSub.begin();
            for(;it!=m_ViewArrSub.end();++it)
            {
                it.value()->scaleMax();
                it.value()->scaleMax();
            }
            QMap<int,GraphicsView*>::iterator itmain= m_ViewArrMain.begin();
            for(;itmain!=m_ViewArrMain.end();++itmain)
            {
                itmain.value()->scaleMax();
                itmain.value()->scaleMax();
            }
        }
        max = !max;
    }
        break;
    case 10:
        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("系统正在运行,请先停止运行!"), 0);
            msg->exec();
            //delete  msg;
            return;
        }
        close();
        break;
    case 11:
    {
#ifdef FALG_PHE

        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(tr("请使用管理员账户登录!"), 0);
            msg->exec();
            //delete  msg;
            return;
        }
        PackageChecker::getInstance()->m_isUpdateEPH=true;
        form_phe.exec();
#endif

    }
        break;
    default:
        break;
    }
}



void MainWindow::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    location = this->geometry();
    max = false;
    mousePressed = false;

    ui->label->installEventFilter(this);
    ui->graphicsViewSub1->installEventFilter(this);
    ui->graphicsViewSub2->installEventFilter(this);
    ui->graphicsViewSub1_2->installEventFilter(this);
    ui->graphicsViewSub2_2->installEventFilter(this);

    //    ui->graphicsViewSub3->installEventFilter(this);
    //    ui->graphicsViewSub4->installEventFilter(this);
    //    ui->graphicsViewSub5->installEventFilter(this);
    //    ui->graphicsViewSub6->installEventFilter(this);
    //    ui->graphicsViewSub7->installEventFilter(this);
    //    ui->graphicsViewSub8->installEventFilter(this);
    //    ui->graphicsViewSub9->installEventFilter(this);

    ui->graphicsViewMain1->installEventFilter(this);
    ui->graphicsViewMain2->installEventFilter(this);
    ui->graphicsViewMain1_2->installEventFilter(this);
    ui->graphicsViewMain2_2->installEventFilter(this);
    //    ui->graphicsViewMain3->installEventFilter(this);
    //    ui->graphicsViewMain4->installEventFilter(this);
    //    ui->graphicsViewMain5->installEventFilter(this);
    //    ui->graphicsViewMain6->installEventFilter(this);
    //    ui->graphicsViewMain7->installEventFilter(this);
    //    ui->graphicsViewMain8->installEventFilter(this);
    //    ui->graphicsViewMain9->installEventFilter(this);




    this->ui->btnMainUserManage->setToolTip("用户管理");
    this->ui->btnMainChangeBrand->setToolTip("品牌管理");
    this->ui->btnMainDistory->setToolTip("统计查询");
    this->ui->btnMainCheckConfig->setToolTip("算子配置");
    this->ui->btnMainImageDistory->setToolTip("图片查询");
    this->ui->btnMainExit->setToolTip("系统关闭");
    this->ui->btnMainPhotoelecSetting->setToolTip("光电设置");
    this->ui->btnMainSystemConfig->setToolTip("参数设置");
    this->ui->btnMainCameraSetting->setToolTip("相机设置");


    //    this->ui->btnMainChangeBrand->setStyleSheet("QPushButton#btnMainChangeBrand{border-radius:0px;border-image:url(:/品牌设置.png); background-color:transparent; border-style:none;}"
    //                                                "QPushButton#btnMainChangeBrand:hover{border-radius:0px;border-image:url(:/品牌设置.png); background-color:#5F5F5F;border-style:none;}");
    //    this->ui->btnMainChangeBrand->setToolTip("品牌管理");
    //    this->ui->btnMainDistory->setStyleSheet("QPushButton#btnMainDistory{border-radius:0px;border-image:url(:/统计查询.png); background-color:transparent; border-style:none;}"
    //                                            "QPushButton#btnMainDistory:hover{border-radius:0px;border-image:url(:/统计查询.png); background-color:#5F5F5F;border-style:none;}");
    //    this->ui->btnMainDistory->setToolTip("统计查询");

    //    this->ui->btnMainImageDistory->setStyleSheet("QPushButton#btnMainImageDistory{border-radius:0px;border-image:url(:/历史图片.png); background-color:transparent; border-style:none;}"
    //                                                 "QPushButton#btnMainImageDistory:hover{border-radius:0px;border-image:url(:/历史图片.png); background-color:#5F5F5F;border-style:none;}");

    //    this->ui->btnMainCameraSetting->setStyleSheet("QPushButton#btnMainCameraSetting{border-radius:0px;border-image:url(:/相机设置.png); background-color:transparent; border-style:none;}"
    //                                                  "QPushButton#btnMainCameraSetting:hover{border-radius:0px;border-image:url(:/相机设置.png); background-color:#5F5F5F;border-style:none;}");

    //    this->ui->btnMainCheckConfig->setStyleSheet("QPushButton#btnMainCheckConfig{border-radius:0px;border-image:url(:/算子设置.png); background-color:transparent; border-style:none;}"
    //                                                "QPushButton#btnMainCheckConfig:hover{border-radius:0px;border-image:url(:/算子设置.png); background-color:#5F5F5F;border-style:none;}");

    //    this->ui->btnMainUserManage->setStyleSheet("QPushButton#btnMainUserManage{border-radius:0px;border-image:url(:/用户管理.png); background-color:transparent; border-style:none;}"
    //                                               "QPushButton#btnMainUserManage:hover{border-radius:0px;border-image:url(:/用户管理.png); background-color:#5F5F5F;border-style:none;}");

    //    this->ui->btnMainSystemConfig->setStyleSheet("QPushButton#btnMainSystemConfig{border-radius:0px;border-image:url(:/系统设置.png); background-color:transparent; border-style:none;}"
    //                                                 "QPushButton#btnMainSystemConfig:hover{border-radius:0px;border-image:url(:/系统设置.png); background-color:#5F5F5F;border-style:none;}");

    this->ui->btnMainMin->setStyleSheet("QPushButton#btnMainMin{border-radius:0px;border-image:url(:/最小化.png); background-color:transparent; border-style:none;}"
                                        "QPushButton#btnMainMin:hover{border-radius:0px;border-image:url(:/最小化.png); background-color:#5F5F5F;border-style:none;}");


    this->ui->btnMainMax->setStyleSheet("QPushButton#btnMainMax{border-radius:0px;border-image:url(:/最大化.png); background-color:transparent; border-style:none;}"
                                        "QPushButton#btnMainMax:hover{border-radius:0px;border-image:url(:/最大化.png); background-color:#5F5F5F;border-style:none;}");



    this->ui->btnMainExit->setStyleSheet("QPushButton#btnMainExit{border-radius:0px;border-image:url(:/系统关闭.png); background-color:transparent; border-style:none;}"
                                         "QPushButton#btnMainExit:hover{border-radius:0px;border-image:url(:/系统关闭.png); background-color:#5F5F5F;border-style:none;}");

    ui->pushButtonRun->setStyleSheet("QPushButton#pushButtonRun{border-image:url(:/启动common.png); background:transparent; border-style:none;}"
                                     "QPushButton#pushButtonRun:hover{border-image:url(:/启动hover.png); background:transparent;border-style:none;}"
                                     "QPushButton#pushButtonRun:pressed{border-image:url(:/启动pressed.png); background:transparent;border-style:none;}");

}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    //    DSDEBUG<<"TYPE:"<<event->type();
    if(obj==ui->label)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            //            buttonJudge(9);
            return true;
        }
    }
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        //        DSDEBUG<<"ENETR  ";
        DSDEBUG<<obj;
        if(obj==ui->graphicsViewSub1)
        {
            changeShowMode(1,1);
        }
        if(obj==ui->graphicsViewSub2)
        {
            changeShowMode(1,2);
        }

        if(obj==ui->graphicsViewSub1_2)
        {
            changeShowMode(1,3);
        }
        if(obj==ui->graphicsViewSub2_2)
        {
            changeShowMode(1,4);
        }


        //        if(obj==ui->graphicsViewSub3)
        //        {
        //            changeShowMode(1,3);
        //        }
        //        if(obj==ui->graphicsViewSub4)
        //        {
        //            changeShowMode(1,4);
        //        }
        //        if(obj==ui->graphicsViewSub5)
        //        {
        //            changeShowMode(1,5);
        //        }
        //        if(obj==ui->graphicsViewSub6)
        //        {
        //            changeShowMode(1,6);
        //        }
        //        if(obj==ui->graphicsViewSub7)
        //        {
        //            changeShowMode(1,7);
        //        }
        //        if(obj==ui->graphicsViewSub8)
        //        {
        //            changeShowMode(1,8);
        //        }
        //        if(obj==ui->graphicsViewSub9)
        //        {
        //            changeShowMode(1,9);
        //        }

        if(obj==ui->graphicsViewMain1)
        {
            changeShowMode(0,1);
        }
        if(obj==ui->graphicsViewMain2)
        {
            changeShowMode(0,1);
        }
        if(obj==ui->graphicsViewMain1_2)
        {
            changeShowMode(0,1);
        }
        if(obj==ui->graphicsViewMain2_2)
        {
            changeShowMode(0,1);
        }
        //        if(obj==ui->graphicsViewMain3)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain4)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain5)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain6)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain7)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain8)
        //        {
        //            changeShowMode(0,1);
        //        }
        //        if(obj==ui->graphicsViewMain9)
        //        {
        //            changeShowMode(0,1);
        //        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::changeShowMode(int mode,int index)
{
    //    DSDEBUG<<"MODE:"<<mode<<"   index:"<<index;
    if(mode==0)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidgetMain->setCurrentIndex(index-1);
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        //        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{

    //    if(ui->label->rect().contains(e->pos()))
    //    {

    //        QString strCommand="AA010E0000000000";
    //        QByteArray receiveByQZ;
    //        QByteArray receiveByKT;

    //        if( m_bIsWarning1)
    //        {
    //            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receiveByQZ);
    //           QThread::msleep(150);
    //            ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receiveByQZ);

    //            if(!ret)
    //                return;


    //            m_bIsWarning1=false;
    //            ui->label->setStyleSheet("color:rgb(252, 252, 0)");
    //            m_Timer1->stop();
    //            m_warningCnt1=0;
    //            ui->label->setText(" 大树智能烟支检测系统");
    //            ui->label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    //        }
    //        if( m_bIsWarning2)
    //        {
    //            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receiveByKT);
    //            QThread::msleep(150);
    //            ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receiveByKT);
    //            if(!ret)
    //                return;

    //            m_bIsWarning2=false;
    //            ui->label->setStyleSheet("color:rgb(252, 252, 0)");
    //            m_warningCnt2=0;
    //            m_Timer2->stop();
    //            ui->label->setText(" 大树智能烟支检测系统");
    //            ui->label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    //        }

    //    }

    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MainWindow::showEvent(QShowEvent *event)
{
    int wid=this->ui->graphicsViewSub1->geometry().width();
    int hei=ui->graphicsViewSub1->geometry().height();

    wid=ui->graphicsViewMain1->geometry().width();
    hei=ui->graphicsViewMain2->geometry().height();

}

void MainWindow::on_pushButtonRun_clicked()
{


    //    if(ui->stackedWidget->currentIndex()==0)
    //        ui->stackedWidget->setCurrentIndex(1);
    //    else
    //        ui->stackedWidget->setCurrentIndex(0);

#ifdef KOFFLINE
    this->ui->pushButtonRun->setEnabled(false);
    setCursor(Qt::BusyCursor);

    switch (DSSystemParam::SystemState)
    {
    case DSSystemParam::ENUMSYSTEMSTATE_RUNNING:
        m_ThreadTest->stop();
        DSSystemParam::SystemState=DSSystemParam::ENUMSYSTEMSTATE_STOPPING;
        ui->pushButtonRun->setStyleSheet("QPushButton#pushButtonRun{border-image:url(:/启动common.png); background:transparent; border-style:none;}"
                                         "QPushButton#pushButtonRun:hover{border-image:url(:/启动hover.png); background:transparent;border-style:none;}"
                                         "QPushButton#pushButtonRun:pressed{border-image:url(:/启动pressed.png); background:transparent;border-style:none;}");
        break;
    case DSSystemParam::ENUMSYSTEMSTATE_STOPPING:


        m_ThreadTest->start(QThread::HighestPriority);
        DSSystemParam::SystemState=DSSystemParam::ENUMSYSTEMSTATE_RUNNING;
        ui->pushButtonRun->setStyleSheet("QPushButton#pushButtonRun{border-image:url(:/停止common.png); background:transparent; border-style:none;}"
                                         "QPushButton#pushButtonRun:hover{border-image:url(:/停止hover.png); background:transparent;border-style:none;}"
                                         "QPushButton#pushButtonRun:pressed{border-image:url(:/停止pressed.png); background:transparent;border-style:none;}");
        break;

    }
    this->ui->pushButtonRun->setEnabled(true);
    setCursor(Qt::ArrowCursor);

#else
    this->ui->pushButtonRun->setEnabled(false);
    setCursor(Qt::BusyCursor);

    switch (DSSystemParam::SystemState)
    {
    case DSSystemParam::ENUMSYSTEMSTATE_RUNNING:
        //        if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
        //        {
        //            frmMessageBox *msg = new frmMessageBox;
        //            msg->SetMessage("请使用管理员账户登录!", 0);
        //            msg->exec();

        //            this->ui->pushButtonRun->setEnabled(true);
        //            setCursor(Qt::ArrowCursor);
        //            //delete  msg;
        //            return;
        //        }
        //断开连接否则会出现死锁现象，主线程等待run现场结束，run线程等待主线程结束
        disconnect(PackageChecker::getInstance(),&PackageChecker::updateCheckRetSig,&form_phe,&dlgphotoelectricitycfg::upDateCheckRet);

        for(int idx=0;idx<CAMERA_NUM_USER;++idx)
        {
            m_ThreadRun[idx]->stop();
        }
        connect(PackageChecker::getInstance(),&PackageChecker::updateCheckRetSig,&form_phe,&dlgphotoelectricitycfg::upDateCheckRet,Qt::BlockingQueuedConnection);

        DSSystemParam::SystemState=DSSystemParam::ENUMSYSTEMSTATE_STOPPING;
        ui->pushButtonRun->setStyleSheet("QPushButton#pushButtonRun{border-image:url(:/启动common.png); background:transparent; border-style:none;}"
                                         "QPushButton#pushButtonRun:hover{border-image:url(:/启动hover.png); background:transparent;border-style:none;}"
                                         "QPushButton#pushButtonRun:pressed{border-image:url(:/启动pressed.png); background:transparent;border-style:none;}");
        ui->lcdNumCalcuTime->display(QString::number(0));

        break;
    case DSSystemParam::ENUMSYSTEMSTATE_STOPPING:


        PackageChecker* pc=PackageChecker::getInstance();
        QMap<int,CCameraBase*>::iterator itor=pc->Cameras.begin();
        itor=pc->Cameras.begin();
        for(;itor!=pc->Cameras.end();itor++)
        {
            if(!(*itor)->IsOpen())
            {

                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(tr("相机%1无法打开,请检查相机是否连接!")).arg(itor.key()), 0);
                msg->exec();
                this->ui->pushButtonRun->setEnabled(true);
                setCursor(Qt::ArrowCursor);
                return;
            }
        }

        for(int idx=0;idx<CAMERA_NUM_USER;++idx)
        {
            m_ThreadRun[idx]->start(QThread::HighestPriority);
        }

        DSSystemParam::SystemState=DSSystemParam::ENUMSYSTEMSTATE_RUNNING;
        ui->pushButtonRun->setStyleSheet("QPushButton#pushButtonRun{border-image:url(:/停止common.png); background:transparent; border-style:none;}"
                                         "QPushButton#pushButtonRun:hover{border-image:url(:/停止hover.png); background:transparent;border-style:none;}"
                                         "QPushButton#pushButtonRun:pressed{border-image:url(:/停止pressed.png); background:transparent;border-style:none;}");
        break;
    }
    this->ui->pushButtonRun->setEnabled(true);
    setCursor(Qt::ArrowCursor);
#endif

}



void MainWindow::on_pushButton_clicked()
{
    if(PackageChecker::getInstance()->user.UserGroup()==ENUMUSERGROUP_OPERATOR)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(tr("请使用管理员账户登录!"), 0);
        msg->exec();
        return;
    }

    PackageChecker* pc=  PackageChecker::getInstance();
    pc->RunParam_CalcNumNg=0;
    pc->RunParam_CalcNumAll=0;

    //    pc->RunParam_CalcNumAllCams[0]=0;//当前班次处理总数
    //    pc->RunParam_CalcNumNgCams[0]=0;//当前班次的Ng数量
    //    pc->RunParam_CalcNumAllCams[1]=0;//当前班次处理总数
    //    pc->RunParam_CalcNumNgCams[1]=0;//当前班次的Ng数量

    for(int i=0;i<CAMERANUM_MAX;++i)
    {
        pc->RunParam_CalcNumAllCams[i]=0;//当前班次处理总数
        pc->RunParam_CalcNumNgCams[i]=0;//当前班次的Ng数量

        pc->RunParam_CalcNumAllCams[i]=0;//当前班次处理总数
        pc->RunParam_CalcNumNgCams[i]=0;//当前班次的Ng数量

        PackageChecker::getInstance()-> RunParam_CalcNumNgGDs[i]=0;//当前班次的Ng数量
        PackageChecker::getInstance()->RunParam_CalcNumNgTotals[i]=0;//当前班次的Ng数量


        for(int j=0;j<CHECKOPERATORNUM_MAX;++j)
        {
            PackageChecker::ErrRecord[i][j]=0;
        }
    }
}



void MainWindow::on_btnMainUnWarning_clicked()
{

    QString strCommand="AA010E0000000000";
    QByteArray receiveByQZ;
    QByteArray receiveByKT;


    m_bIsWarning1=false;
    ui->label->setStyleSheet("color:rgb(252, 252, 0)");
    m_Timer1->stop();
    m_warningCnt1=0;
    ui->label->setText(tr(" 大树智能烟支检测系统"));
    ui->label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);


    m_bIsWarning2=false;
    ui->label->setStyleSheet("color:rgb(252, 252, 0)");
    m_warningCnt2=0;
    m_Timer2->stop();
    ui->label->setText(tr(" 大树智能烟支检测系统"));
    ui->label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);




    //            if( m_bIsWarning1)
    {
        bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receiveByQZ);
        //                QThread::msleep(100);
        //                ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receiveByQZ);

    }
    //            if( m_bIsWarning2)
    {
        QThread::msleep(100);
        bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receiveByKT);
        //                QThread::msleep(100);
        //                ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receiveByKT);
    }

    ui->label->setFocus();

}

void MainWindow::onTimeoutShuCai()
{
    QString name=PackageChecker::getInstance()->Options->MachineName();
    int total=PackageChecker::getInstance()->RunParam_CalcNumAllCams[0];
    int quezhi=PackageChecker::getInstance()->RunParam_CalcNumNgCams[0];
    int kongtou=PackageChecker::getInstance()->RunParam_CalcNumNgCams[1];

    QString message=QString("%1,%2,%3,%4").arg(name).arg(total).arg(quezhi).arg(kongtou);
    if(PackageChecker::getInstance()->m_cBaseServerTCP!=nullptr)
        PackageChecker::getInstance()->m_cBaseServerTCP->sendMessage(message);
}
