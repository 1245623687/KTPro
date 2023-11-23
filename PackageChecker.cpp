#include "packagechecker.h"
#include"CheckOperator/imgtobaccocontrol.h"


#include"CheckOperator/Onnx.h"


#include"config/dlgbrandmanage.h"
#include"Camera/camerafactory.h"
#include"database/dsfilerecord.h"

#include "board/Xmodem.h"
#include "board/SerialCom.h"
#include "board/CMDHost.h"
#include"qtCommon/frmmessagebox.h"
bool PackageChecker::DynamicGrab=false;
std::mutex PackageChecker::m_Mutex;
PackageChecker* PackageChecker::m_Instance = nullptr;
QDateTime PackageChecker::m_TimeSystemStart=QDateTime::currentDateTime();
unsigned char PackageChecker::ErrMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]={0};
int PackageChecker::ErrRecord[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]={0};
QAtomicInt::QAtomicInteger PackageChecker::ErrPhe[2]={0,0};
int PackageChecker::RetPheQZ[21]={0};

int PackageChecker::RetPheKT[21]={0};

int PackageChecker::RetMapPheQZ[20]={0};

int PackageChecker::RetMapPheKT[20]={0};

QMutex PackageChecker::m_MutexRetPheQZ;
QMutex PackageChecker::m_MutexRetPheKT;

PackageChecker ::GC PackageChecker ::gc;

//std::mutex PackageChecker::m_BadImgMutex;
QMutex PackageChecker::m_BadImgMutex;


 std::queue<std::vector<int>>  PackageChecker::QueRetPheKT;
 std::queue<std::vector<int>>   PackageChecker::QueRetMapPheKT;

 std::queue<std::vector<int>>   PackageChecker::QueRetPheQZ;
 std::queue<std::vector<int>>  PackageChecker::QueRetMapPheQZ;

PackageChecker::PackageChecker()
{
    memset(RetPheQZ,0,sizeof (int)*20);
    memset(RetPheKT,0,sizeof (int)*20);

    this->ImgTobaccoRun=nullptr;
    ImgTobaccoAnalysis=nullptr;

    CurBadImage.insert(1,NULL);
    CurBadImage.insert(2,NULL);
    CheckRet[0]=0;
    CheckRet[1]=0;

    UpdateCheckOperator=0;
    m_TimerLog=new QTimer;
    connect(m_TimerLog, SIGNAL(timeout()), this, SLOT(timerlog()));

    for(int i=0;i<CAMERANUM_MAX;i++)
    {
        IsCalcaulateFinish[i]=0;
        RunParam_CalcNumAllCams[i]=0;//当前班次处理总数
        RunParam_CalcNumNgCams[i]=0;//当前班次的Ng数量
        RunParam_CalcTimeCams[i]=0;//当前班次的Ng数量

        RunParam_CalcNumNgGDs[i]=0;//当前班次的Ng数量
        RunParam_CalcNumNgTotals[i]=0;//当前班次的Ng数量
    }


    m_mapProbVal.insert(1,3);
    m_mapProbVal.insert(2,6);
    m_mapProbVal.insert(3,9);
    m_mapProbVal.insert(4,12);
    m_mapProbVal.insert(5,15);
    m_mapProbVal.insert(6,18);
    m_mapProbVal.insert(7,2);
    m_mapProbVal.insert(8,5);
    m_mapProbVal.insert(9,8);
    m_mapProbVal.insert(10,11);
    m_mapProbVal.insert(11,14);
    m_mapProbVal.insert(12,17);
    m_mapProbVal.insert(13,20);
    m_mapProbVal.insert(14,1);
    m_mapProbVal.insert(15,4);
    m_mapProbVal.insert(16,7);
    m_mapProbVal.insert(17,10);
    m_mapProbVal.insert(18,13);
    m_mapProbVal.insert(19,16);
    m_mapProbVal.insert(20,19);

    //    m_isUpdateEPH=false;
}

OpencvImage *PackageChecker::cloneOpencvImg(int index,OpencvImage & another)
{
    //  m_BadImgMutex.lock();
    CurBadImage.insert(index,another.clone());
    // m_BadImgMutex.unlock();
    return CurBadImage.value(index);
}

void PackageChecker::deleteCloneOpencvImage(int index)
{
    m_BadImgMutex.lock();
    if(CurBadImage.value(index)!=NULL)
    {
        delete  CurBadImage.value(index);
        CurBadImage.insert(index,NULL);
    }
    m_BadImgMutex.unlock();
    return ;
}

//OpencvImage* PackageChecker::cloneOpencvImg(OpencvImage & another)
//{
//    return another.clone();
//}

void PackageChecker::timerlog()
{
    DSDEBUG<<"用户退出";

    DSDEBUG<<"用户退出";

    this->user.setUserName(tr("操作员"));
    this->user.setUserGroup(ENUMUSERGROUP_OPERATOR);
    m_TimerLog->stop();
}


PackageChecker::~PackageChecker()
{

    //        CamerasCfgParams.clear();
    QMap<int,CCameraBase*>::iterator itor= Cameras.begin();
    for(;itor!=Cameras.end();++itor)
    {
        if(itor.value()->IsOpen())
        {
            (*itor)->SetTrigger(0);
            (*itor)->Close();
        }
        delete itor.value();
        //            AVTCamera* tmp=static_cast<AVTCamera*>(itor.value());
        //            delete  tmp;
    }
    Cameras.clear();
    if(ImgTobaccoAnalysis!=nullptr){delete  ImgTobaccoAnalysis;ImgTobaccoAnalysis=nullptr;}
    if(ImgTobaccoRun!=nullptr){delete  ImgTobaccoRun;ImgTobaccoRun=nullptr;}
    CurImage.clear();
    DSDEBUG<<"~PackageChecker finish";
}


void PackageChecker::loadRunBrand()
{
    this->ImgTobaccoRun=new ImgTobacco;
    ImgTobaccoControl control(this->ImgTobaccoRun);
    QString dirBrand=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.loadConfig(dirBrand);
    control.loadRefImageFromDir(dirBrand);
}


void PackageChecker::changeBrand(QString brandName)
{
    DSSystemParam::BrandName=brandName;
    if(this->ImgTobaccoRun!=nullptr){delete  this->ImgTobaccoRun;this->ImgTobaccoRun=nullptr;}
    this->ImgTobaccoRun=new ImgTobacco;
    ImgTobaccoControl control(this->ImgTobaccoRun);
    QString dirBrand=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.loadConfig(dirBrand);
    control.loadRefImage(dirBrand.toStdString());
    control.loadTemplate(dirBrand);
    DSSystemParam::CameraNum= control.getImgProNum();

    CameraConfig=ClsCameraConfig::getInstance();
    CameraConfig->load();

    this->ImgTobaccoRun->copyto(ImgTobaccoAnalysis);
    RunParam_CalcTime=0;
    RunParam_CalcNumAll=0;
    RunParam_CalcNumNg=0;
    RunParam_CalcNumAllCams[0]=0;
    RunParam_CalcNumNgCams[0]=0;
    RunParam_CalcNumAllCams[1]=0;
    RunParam_CalcNumNgCams[1]=0;
    UpdateCheckOperator=0;

    ClsCameraConfig * CameraConfig=ClsCameraConfig::getInstance();
    CameraConfig->load();
    QMap<int,ClsCameraConfig::StuCameraParam> cameraCfg;
    CameraConfig->getCameraCfg(cameraCfg);
    QMap<int,CCameraBase*>::iterator itor= Cameras.begin();
    for(;itor!=Cameras.end();++itor)
    {
        itor.value()->SetGain(cameraCfg[itor.key()].Offset);
        itor.value()->SetExposureTime(cameraCfg[itor.key()].ExposeTime);
    }


    //光源设置
    //    UINT8 packetBuf[PACKET_BUF_SIZE];
    //    memset(packetBuf, 0, PACKET_BUF_SIZE * sizeof(UINT8));
    //    if(!OpenPort(1, 115200))
    //    {
    //        frmMessageBox *msg = new frmMessageBox;
    //        msg->SetMessage(QString("通信端口被占用,请先关闭板卡调试软件并重启系统!"), 2);
    //        msg->exec();
    //        return;
    //    }
    //    XModemCommEngine(SLAVE1_CODE, R_CODE, UPLOAD_DATA_LEN, packetBuf);

    //    unsigned int kk = CameraConfig->getLightCode(1);

    //    packetBuf[1] = kk & 0xFF;
    //    packetBuf[2] = (kk>>8) & 0xFF;
    //    packetBuf[3] = (kk>>16) & 0xFF;
    //    packetBuf[4] = (kk>>24) & 0xFF;
    //    int crcSum = CRC16(packetBuf, (DOWNLOAD_DATA_LEN - 2));
    //    packetBuf[ADDR_FOR_CRC_LBYTE] = (UINT8)(crcSum & 0xFF);
    //    packetBuf[ADDR_FOR_CRC_HBYTE] = (UINT8)((crcSum >> 8) & 0xFF);
    //    XModemCommEngine(SLAVE1_CODE, W_CODE, DOWNLOAD_DATA_LEN, packetBuf);
    //    ClosePort();
    //光源设置



    this->ImgHei=control.getImgHei();
    this->ImgWid=control.getImgWid();


    std::list<ImgPro*>::iterator itor_i= this->ImgTobaccoRun->LstImgPro.begin();
    for(;itor_i!=this->ImgTobaccoRun->LstImgPro.end();++itor_i)
    {
#ifdef IMG_TYPE_GRAY
        OpencvImage tmpImage;
        tmpImage.Width=(*itor_i)->RefOpencvImage.ImageGray.cols;
        tmpImage.Height=(*itor_i)->RefOpencvImage.ImageGray.rows;
        tmpImage.Format=ENUMDSIMAGEFORMAT_GRAY;
        tmpImage.ImageGray=cv::Mat::zeros(tmpImage.Height,tmpImage.Width,CV_8UC1);
        this->CurImage[(*itor_i)->ImgIndex]=tmpImage;
        this->CurImage[(*itor_i)->ImgIndex].ImageGray=(*itor_i)->RefOpencvImage.ImageGray.clone();
#endif

#ifdef IMG_TYPE_RGB
        OpencvImage tmpImage;
        tmpImage.Width=(*itor_i)->RefOpencvImage.ImageRGB.cols;
        tmpImage.Height=(*itor_i)->RefOpencvImage.ImageRGB.rows;
        tmpImage.Format=ENUMDSIMAGEFORMAT_RGB;
        tmpImage.ImageRGB=cv::Mat::zeros(tmpImage.Height,tmpImage.Width,CV_8UC3);
        this->CurImage[(*itor_i)->ImgIndex]=tmpImage;
        this->CurImage[(*itor_i)->ImgIndex].ImageRGB=(*itor_i)->RefOpencvImage.ImageRGB.clone();


        OpencvImage tmpBadImage;
        tmpBadImage.Width=(*itor_i)->RefOpencvImage.ImageRGB.cols;
        tmpBadImage.Height=(*itor_i)->RefOpencvImage.ImageRGB.rows;
        tmpBadImage.Format=ENUMDSIMAGEFORMAT_RGB;
        tmpBadImage.ImageRGB=cv::Mat::zeros(tmpBadImage.Height,tmpBadImage.Width,CV_8UC3);
        this->m_qmapCurBadImage[(*itor_i)->ImgIndex]=tmpBadImage;
        this->m_qmapCurBadImage[(*itor_i)->ImgIndex].ImageRGB=tmpBadImage.ImageRGB.clone();

#endif

    }
}
#include<vector>

void PackageChecker::init()
{


    user.setUserGroup(static_cast<ENUMUSERGROUP>(2));
    user.setUserName(tr("操作员"));


    m_TimeSystemStart=QDateTime::currentDateTime();
    DSDEBUG<<this->m_TimeSystemStart<<endl;
    try
    {

        Options=DSClsOptions::getInstance();
    }
    catch (...)
    {
        DSDEBUG<<"加载Options.ini失败"<<endl;
        throw PackageCheckerException((int)ENUMPACKAGECHECKERERR_LOADOPTIONSFAIL);
    }
    DSDEBUG<<"options参数加载完成";
    try
    {
        LastConfig=DSClsLastConfig::getInstance();
        LastConfig->load();
    }
    catch (...)
    {
        DSDEBUG<<"加载LastConfig.ini失败"<<endl;
        throw PackageCheckerException((int)ENUMPACKAGECHECKERERR_LOADLASTCONFIGFAIL);
    }
    DSDEBUG<<"上一次参数加载完成";

    int i=this->Options->IOTYPE();
    this->IOContol=CIOBase::getInstance(this->Options->IOTYPE());


    //串口
#ifdef FALG_PHE
    try
    {
        m_pBaseCom=BaseCom::getInstance();
#ifdef _RELEASE
        m_pBaseCom->setQZPort("COM1");
        m_pBaseCom->setKTPort("COM2");

        m_pBaseCom->setQZPortSim("COM3");
        m_pBaseCom->setKTPortSim("COM4");

#else

        //m_pBaseCom->setQZPort("COM1");
        //m_pBaseCom->setKTPort("COM8");
        //m_pBaseCom->setQZPort("COM1");
        m_pBaseCom->setKTPort("COM2");
        //m_pBaseCom->setQZPortSim("COM3");
        m_pBaseCom->setKTPortSim("COM4");
#endif
        //        int ret=m_pBaseCom->openComPort();

        bool ret=false;
        if(this->Options->getCom1State())
        {
            ret=m_pBaseCom->openQZComPort();
            if(!ret)
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(tr("打开串口%1失败!")).arg(1), 2);
                msg->exec();
            }
        }

        if(this->Options->getCom2State())
        {
            ret=m_pBaseCom->openKTComPort();
            if(!ret)
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(tr("打开串口%1失败!")).arg(2), 2);
                msg->exec();
            }
        }


        if(this->Options->getCom3State())
        {
            ret=m_pBaseCom->openQZComPortSim();
            if(!ret)
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(tr("打开串口%1失败!")).arg(3), 2);
                msg->exec();
            }
        }

        if(this->Options->getCom4State())
        {
            ret=m_pBaseCom->openKTComPortSim();
            if(!ret)
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(tr("打开串口%1失败!")).arg(4), 2);
                msg->exec();
            }
        }



        //        DSDEBUG<<"打开COM1,COM2失败"<<endl;
    }
    catch (...)
    {
        DSDEBUG<<"打开COM1,COM2失败"<<endl;
        throw PackageCheckerException((int)ENUMPACKAGECHECKERERR_LOADLASTCONFIGFAIL);
    }


    DSDEBUG<<"串口完成";



    //光电配置参数加载
    PhotoElecConfig=ClsPhototElecConfig::getInstance();

    //从板子获取参数
    m_pBaseCom->togleDisconnect();
    //获取参数指令
    QString strCommand="AA02030000000000";
    QByteArray receiveByQZ;
    QByteArray receiveByKT;
    bool ret= m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receiveByQZ);

    // 解析receive
    if(ret)
    {
        //触发角度
        bool ok;
        QByteArray tmpReadData= receiveByQZ.mid(4,48);
        QByteArray mid;
        for(int i=0;i<12;i++)
        {
            mid= tmpReadData.mid(0+i*4,4);
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            PhotoElecConfig->setCapAngle(1,i,value);
        }

        //剔除角度，剔除步数
        tmpReadData= receiveByQZ.mid(52,18);
        mid= tmpReadData.mid(0,4);
        int  value1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        mid= tmpReadData.mid(4,2);
        int  value2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setKickAngleAndStep(1,value1,value2);


        //相机触发角度
        mid= tmpReadData.mid(6,4);
        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setTrigAngle(1,value);

        //自检角度
        mid= tmpReadData.mid(10,4);
        value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setSelfCheckAngle(1,value);

        //门槛系数
        mid= tmpReadData.mid(14,4);
        value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setRatio(1,value/100.0);

        tmpReadData= receiveByQZ.mid(70,80);
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(0+i*4,4);
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            PhotoElecConfig->setProThreshold(1,i,value);
        }
    }


    ret= m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receiveByKT);
    // 解析receive
    if(ret)
    {
        //触发角度
        bool ok;
        QByteArray tmpReadData= receiveByKT.mid(4,48);
        QByteArray mid;
        for(int i=0;i<12;i++)
        {
            mid= tmpReadData.mid(0+i*4,4);
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            PhotoElecConfig->setCapAngle(2,i,value);
        }



        //剔除角度，剔除步数
        tmpReadData= receiveByKT.mid(52,18);
        mid= tmpReadData.mid(0,4);
        int  value1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        mid= tmpReadData.mid(4,2);
        int  value2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setKickAngleAndStep(2,value1,value2);

        //相机触发角度
        mid= tmpReadData.mid(6,4);
        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setTrigAngle(2,value);

        //自检角度
        mid= tmpReadData.mid(10,4);
        value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setSelfCheckAngle(2,value);

        //门槛系数
        mid= tmpReadData.mid(14,4);
        value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        PhotoElecConfig->setRatio(2,value/100.0);


        tmpReadData= receiveByKT.mid(70,80);
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(0+i*4,4);
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            PhotoElecConfig->setProThreshold(2,i,value);
        }
    }
    
    
    //从模拟板获取参数，缺支
    strCommand="5502030000000000";
    QByteArray receivedByArray;
    QVector<int> vecTmp;
    ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(strCommand.toLatin1(),receivedByArray,vecTmp,3);
    // 解析receive
    int sz=receivedByArray.size();
    if(ret&&PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_3)
    {
        //3个电流增益
        for(int i=0;i<3;i++)
        {
            PhotoElecConfig->setProCurrentSim(1,i,vecTmp[i*2]/100.0);
            PhotoElecConfig->setProGainSim(1,i,vecTmp[i*2+1]);
        }
    }
    if(ret&&PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20)
    {
        for(int i=0;i<20;i++)
        {
            PhotoElecConfig->setProCurrentSim(1,i,vecTmp[i*2]/100.0);
            PhotoElecConfig->setProGainSim(1,i,vecTmp[i*2+1]);

            PhotoElecConfig->setProThresholdSim(1,i,vecTmp[40+i]);
        }
        //1个系数
        PhotoElecConfig->setRatioSim(1,vecTmp[vecTmp.size()-1]/100.0);

    }


    
    //从模拟板获取参数，空头
    strCommand="5502030000000000";
    ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(strCommand.toLatin1(),receivedByArray,vecTmp,3);
    // 解析receive
    sz=receivedByArray.size();

    if(ret&&PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_3)
    {
        //3个电流增益
        for(int i=0;i<3;i++)
        {
            PhotoElecConfig->setProCurrentSim(2,i,vecTmp[i*2]/100.0);
            PhotoElecConfig->setProGainSim(2,i,vecTmp[i*2+1]);
        }
        //        //20个门槛值
        //        for(int i=0;i<20;i++)
        //        {
        //            PhotoElecConfig->setProThresholdSim(2,i,vecTmp[6+i]);
        //        }
        //        //1 个系数
        //        PhotoElecConfig->setRatioSim(2,vecTmp[vecTmp.size()-1]/100.0);

    }
    if(ret&&PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20)
    {
        for(int i=0;i<20;i++)
        {
            PhotoElecConfig->setProCurrentSim(2,i,vecTmp[i*2]/100.0);

            //            PhotoElecConfig->setProCurrentCodeValSim(2,i,vecTmp[i*2]);
            PhotoElecConfig->setProGainSim(2,i,vecTmp[i*2+1]);
            PhotoElecConfig->setProThresholdSim(2,i,vecTmp[40+i]);
        }
        //1个系数
        PhotoElecConfig->setRatioSim(2,vecTmp[vecTmp.size()-1]/100.0);
    }
    //如果获取码值失败，则将本地文件中的码值写入模拟板
    //    if(!ret)
    //    {
    //        for(int i=0;i<20;i++)
    //        {

    //            int ProbIndex=i+1;
    //            int OperaCode=14;
    //            int v1=ProbIndex;
    //            int v2=PhotoElecConfig->getProCurrentCodeValSim(2,ProbIndex-1);

    //            QByteArray sendText=QByteArray::fromHex("5501");

    //            QString strOperaCode=QString("%1").arg(OperaCode,2,16,QLatin1Char('0')).toUtf8();
    //            QByteArray byOperaCode=QByteArray::fromHex(QByteArray(strOperaCode.toLatin1()));
    //            sendText.append(byOperaCode);

    //            QString strHexV1=QString("%1").arg(v1,2,10,QLatin1Char('0')).toUtf8();
    //            QByteArray byHexV1=QByteArray::fromHex(QByteArray(strHexV1.toLatin1()));
    //            sendText.append(byHexV1);

    //            QString strHexV2=QString("%1").arg(v2,8,16,QLatin1Char('0')).toUtf8();
    //            QByteArray byHexV2=QByteArray::fromHex(QByteArray(strHexV2.toLatin1()));
    //            sendText.append(byHexV2);
    //            sendText =sendText.toHex().toUpper();

    //            QByteArray receivedByArray;
    //            QVector<int> vecTmp;

    //            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);

    //            QThread::msleep(500);
    //        }
    //    }
    m_pBaseCom->togleConnect();
    //光电配置参数加载

    //保存从板子上接收到的参数
    PhotoElecConfig->save();
    //从光电那读取参数
    //....
    //

    DSDEBUG<<"光电参数加载完成";

#endif



    //灯亮
    for (int i = 0; i < 4; i++)
    {
        //作为输出引脚
        //华汉针脚设置低电平
        //华汉哪个针脚设1哪个针脚高电平(负极接地，正极接输出针脚测得)
        //g_ltgFunction.LTGDoCtrlWriteBit(m_OutPort0, m_OutPort1)
        //相对于板子接法(正极接5v，负极接输出是低电平)，板子输入低电平灯亮
        //
        int prs=i*2;
        int res=i*2+1;

        //柳州第6个prs作为报警输出信号
        //        if(prs!=6)
        //        {
        this->IOContol->setLevel(prs, 0xFFFFFFFF, 0x00);
        this->IOContol->setLevel(res, 0xFFFFFFFF, 0x00);
        //        }


        //        this->IOContol->setLevel(RUNPIN, 0xFFFFFFFF, 0x00);
    }

    QThread::msleep(10);
    //灯灭
    for (int i = 0; i < 4; i++)
    {
        //作为输出引脚
        int prs=i*2;
        int res=i*2+1;

        //柳州第6个prs作为报警输出信号
        //        if(prs!=6)
        //        {
        this->IOContol->setLevel(prs, 0xFFFFFFFF, 0xFF);
        this->IOContol->setLevel(res, 0xFFFFFFFF, 0xFF);
        //        }
        //        this->IOContol->setLevel(RUNPIN, 0xFFFFFFFF, 0xFF);
    }




#ifdef FALG_PHE
    //光电剔除开与关，使用7号针脚
    // PackageChecker::getInstance()->m_pBaseCom->togleDisconnect();

    //只打开图像剔除
    if(this->Options->checkMode()==ENUMCHECKMODETYPE_PIC ||   this->Options->checkMode()==ENUMCHECKMODETYPE_PICANDPHE)
    {

        //光电剔除关闭
        this->IOContol->setLevel(7,0xFFFFFFFF,0x00);
        QString strCommand="AA01090100000000";
        //        QByteArray receive=strCommand.toLatin1();
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        //        //图像剔除打开
        //        strCommand="AA01090200010000";
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);
    }


    //只打开光电剔除
    if(this->Options->checkMode()==ENUMCHECKMODETYPE_PHE)
    {
        this->IOContol->setLevel(7,0xFFFFFFFF,0xff);
        QString strCommand="AA01090100010000";
        //        QByteArray receive;
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        //        //图像剔除关闭
        //        strCommand="AA01090200000000";
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);


    }
    //图像，光电剔除都开
    if( this->Options->checkMode()==ENUMCHECKMODETYPE_PICORPHE)
    {
        this->IOContol->setLevel(7,0xFFFFFFFF,0xff);
        QString strCommand="AA01090100010000";
        //        QByteArray receive;
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        //        //图像剔除关闭
        //        strCommand="AA01090200010000";
        //        m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        //        m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);
    }

    //  PackageChecker::getInstance()->m_pBaseCom->togleConnect();
#endif




    PackageChecker::RunParam_CalcNumAll=LastConfig->lastCheckNum();
    PackageChecker::RunParam_CalcNumNg=LastConfig->lastNgNum();
    PackageChecker::RunParam_Shift=LastConfig->lastShift();
    DSDEBUG<<PackageChecker::RunParam_CalcNumAll;
    DSDEBUG<<PackageChecker::RunParam_CalcNumNg;


    if(this->LastConfig->lastBrand()=="Default"||!QFileInfo::exists(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->LastConfig->lastBrand()+"/config.xml"))
    {
        DlgBrandManage dlgbrandManage;
        dlgbrandManage.exec();
    }
    else
    {
        DSSystemParam::BrandName=LastConfig->lastBrand();
    }
    DSDEBUG<<"牌号切换完成";


    //   this->IOContol=CIOBase::getInstance(ENUMIOTYPE_YANHUAGPIO);

    this->ImgTobaccoRun=new ImgTobacco;
    ImgTobaccoControl control(this->ImgTobaccoRun);

    QString xmlpath=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
    if(testIsXmlfileBroken(xmlpath))
    {
        QString backupfile1=DSSystemParam::AppPath+"/backup/backup1/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;

        if(!testIsXmlfileBroken(backupfile1))
        {
            QString copytofile=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
            if( !copyBackUpFiletoDest(backupfile1,copytofile))
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(DSSystemParam::BrandName +tr(" 配置文件损坏，请关闭软件，删除配置文件重新配置!")), 2);
                msg->exec();
                return;
            }
        }
        else
        {
            QString backupfile2=DSSystemParam::AppPath+"/backup/backup2/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;

            QString copytofile=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
            if( !copyBackUpFiletoDest(backupfile2,copytofile))
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(DSSystemParam::BrandName +tr(" 配置文件损坏，请关闭软件，删除配置文件重新配置!")), 2);
                msg->exec();
                return;
            }
        }
    }

    {
        //备份
        QString backupfile=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
        if(!testIsXmlfileBroken(backupfile))
        {
            QString backupfile2=DSSystemParam::AppPath+"/backup/backup2/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
            if( !copyBackUpFiletoDest(backupfile,backupfile2))
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage(QString(DSSystemParam::BrandName +tr(" 配置文件损坏，请关闭软件，删除配置文件重新配置!")), 2);
                msg->exec();
                return;
            }
        }
    }


    control.loadConfig(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    DSSystemParam::CameraNum= control.getImgProNum();

    control.loadRefImageFromDir(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.loadTemplate(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    // loadAnalysisBrand(DSSystemParam::BrandName);
    DSDEBUG<<"运行参数加载完成";
    this->ImgTobaccoAnalysis=new ImgTobacco;
    ImgTobaccoControl control2(this->ImgTobaccoAnalysis);
    control2.loadConfig(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control2.loadRefImageFromDir(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control2.loadTemplate(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    //loadAnalysisBrand(DSSystemParam::BrandName);

    DSDEBUG<<"调试参数加载完成";
    //    this->ImgHei=control.getImgHei();
    //    this->ImgWid=control.getImgWid();



    //给当前图像分配内存
    std::list<ImgPro*>::iterator itor= this->ImgTobaccoRun->LstImgPro.begin();
    for(int i=0;itor!=this->ImgTobaccoRun->LstImgPro.end();++itor,i++)
    {
        this->ImgHeis[i]=(*itor)->RefOpencvImage.ImageRGB.rows;
        this->ImgWids[i]=(*itor)->RefOpencvImage.ImageRGB.cols;

        control.setImgHei(i,this->ImgHeis[i]);
        control.setImgWid(i,this->ImgWids[i]);


        //        this->ImgHeis[i]=control.getImgHei(i);
        //        this->ImgWids[i]=control.getImgWid(i);

#ifdef IMG_TYPE_GRAY
        OpencvImage tmpImage;
        tmpImage.Width=(*itor)->RefOpencvImage.ImageGray.cols;
        tmpImage.Height=(*itor)->RefOpencvImage.ImageGray.rows;
        tmpImage.Format=ENUMDSIMAGEFORMAT_GRAY;
        tmpImage.ImageGray=cv::Mat::zeros(tmpImage.Height,tmpImage.Width,CV_8UC1);
        this->CurImage[(*itor)->ImgIndex].Format=ENUMDSIMAGEFORMAT_GRAY;
        this->CurImage[(*itor)->ImgIndex]=tmpImage;
        this->CurImage[(*itor)->ImgIndex].ImageGray=(*itor)->RefOpencvImage.ImageGray.clone();

#endif
#ifdef IMG_TYPE_RGB
        OpencvImage tmpImage;
        tmpImage.Width=(*itor)->RefOpencvImage.ImageRGB.cols;
        tmpImage.Height=(*itor)->RefOpencvImage.ImageRGB.rows;
        tmpImage.Format=ENUMDSIMAGEFORMAT_RGB;
        tmpImage.ImageRGB=cv::Mat::zeros(tmpImage.Height,tmpImage.Width,CV_8UC3);
        this->CurImage[(*itor)->ImgIndex].Format=ENUMDSIMAGEFORMAT_RGB;
        this->CurImage[(*itor)->ImgIndex]=tmpImage;
        this->CurImage[(*itor)->ImgIndex].ImageRGB=(*itor)->RefOpencvImage.ImageRGB.clone();


        OpencvImage tmpBadImage;
        tmpBadImage.Width=(*itor)->RefOpencvImage.ImageRGB.cols;
        tmpBadImage.Height=(*itor)->RefOpencvImage.ImageRGB.rows;
        tmpBadImage.Format=ENUMDSIMAGEFORMAT_RGB;
        tmpBadImage.ImageRGB=cv::Mat::zeros(tmpBadImage.Height,tmpBadImage.Width,CV_8UC3);
        this->m_qmapCurBadImage[(*itor)->ImgIndex]=tmpBadImage;
        this->m_qmapCurBadImage[(*itor)->ImgIndex].ImageRGB=tmpBadImage.ImageRGB.clone();

#endif
    }

    CameraConfig=ClsCameraConfig::getInstance();
    CameraConfig->load();
    DSDEBUG<<"相机参数加载完成";




    if(Options->CameraType()==ENUMCAMERATYPE::ENUMCAMERATYPE_SIMU)
    {
        int camnum=DSSystemParam::CameraNum;
        for(int idx=1;idx<=DSSystemParam::CameraNum;++idx)
        {
            Cameras[idx]=CameraFactory::createCamera(Options->CameraType());

            //            QString strPath =QString("C:\\Users\\Admin\\Desktop\\HengYan_Pro\\hengyan%1").arg(idx);
            //            Cameras[idx]->SetIPAddress(strPath.toStdString());

            QString strPath=DSSystemParam::AppPath+"/"+QString("whkongtouImg%1").arg(idx);
            Cameras[idx]->SetIPAddress(strPath.toStdString());
            Cameras[idx]->setCamIndex(idx);
            Cameras[idx]->reset();
            Cameras[idx]->setImgHei(ImgHeis[idx-1]);
            Cameras[idx]->setImgWid(ImgWids[idx-1]);
            Cameras[idx]->setBufSize(ImgHeis[idx-1]*ImgWids[idx-1]*3);
            if(!Cameras[idx]->Open()) continue;
            Cameras[idx]->StartGrab();
            Cameras[idx]->SetGain(CameraConfig->getGain(idx));
            Cameras[idx]->SetExposureTime(CameraConfig->getExposureTime(idx));
        }
        DSDEBUG<<"相机初始化完成";
    }
    else
    {
        for(int idx=1;idx<=DSSystemParam::CameraNum;++idx)
        {
            Cameras[idx]=CameraFactory::createCamera(ENUMCAMERATYPE_MER2);
            Cameras[idx]->SetIPAddress(CameraConfig->getIpaddress(idx).toStdString());
            Cameras[idx]->setCamIndex(idx);
            Cameras[idx]->reset();

            Cameras[idx]->setImgHei(ImgHeis[idx-1]);
            Cameras[idx]->setImgWid(ImgWids[idx-1]);
            Cameras[idx]->setBufSize(ImgHeis[idx-1]*ImgWids[idx-1]*3);
            if(!Cameras[idx]) continue;
            //#ifdef _RELEASE
            if(!Cameras[idx]->Open()) continue;
            //#endif

            Cameras[idx]->SetGain(CameraConfig->getGain(idx));
            Cameras[idx]->SetExposureTime(CameraConfig->getExposureTime(idx));
        }
        DSDEBUG<<"相机初始化完成";
    }

#ifdef FLAG_KONGTOU

    //先初始化检测一张
    //opencv读取一张，或者生产一张

    std::string imgPath=DSSystemParam::AppPath.toStdString()+"/"+"templetImg/testImgOK.jpg";
    cv::Mat initImg;
    initImg=cv::imread(imgPath,1);

    uchar* ptr= initImg.ptr();
    if(ptr==NULL)
    {
        initImg=cv::Mat::zeros(512,512,CV_8UC3);
    }

    //初始化Onnx
    itor= this->ImgTobaccoRun->LstImgPro.begin();
    vector<vector<BBoxInfo>> det_boxes;
    std::string modelPathquezhi=DSSystemParam::AppPath.toStdString()+"/"+"model/quezhi_best.onnx";
    std::string modelPathkongtou=DSSystemParam::AppPath.toStdString()+"/"+"model/kongtou_best.onnx";

    //判断这个路径的文件是否存在
    QFile file(QString::fromStdString(modelPathquezhi));
    if(file.exists())
    {
        OnnxGloable::getInstance()->onnxArray[0].m_bIsValid=true;
        OnnxGloable::getInstance()->onnxArray[0].read_onnx(modelPathquezhi);

        OnnxGloable::getInstance()->onnxArray[0].setConfidence(0.1);
        OnnxGloable::getInstance()->onnxArray[0].detect(initImg,det_boxes);
    }
    else
        OnnxGloable::getInstance()->onnxArray[0].m_bIsValid=false;


    //判断这个路径的文件是否存在
    QFile file2(QString::fromStdString(modelPathkongtou));

    itor++;
    if(file2.exists())
    {
        OnnxGloable::getInstance()->onnxArray[1].read_onnx(modelPathkongtou);
        OnnxGloable::getInstance()->onnxArray[1].m_bIsValid=true;

        OnnxGloable::getInstance()->onnxArray[1].setConfidence(0.1);
        OnnxGloable::getInstance()->onnxArray[1].detect(initImg,det_boxes);
    }
    else
        OnnxGloable::getInstance()->onnxArray[1].m_bIsValid=false;
#endif




    //change shift
    if( PackageChecker::RunParam_Shift!=DSSystemParam::getCurrentShift()||LastConfig->date()!=QDate::currentDate())
    {
        DSDEBUG<<"切换班次信息:新班次为"<<DSSystemParam::getCurrentShift()<<endl;
        PackageChecker::RunParam_Shift=DSSystemParam::getCurrentShift();
        PackageChecker::RunParam_CalcNumAll=0;
        PackageChecker::RunParam_CalcNumNg=0;
    }



    // 删除旧数据
    DsFileRecord::deleteByDate(this->Options->SaveDaysNum());
    memset(ErrRecord,0,CAMERANUM_MAX*CHECKOPERATORNUM_MAX*sizeof(int));
    DSDEBUG<<"删除旧数据完成";
    //加载当前班次的已保存信息
    DsFileRecord::loadCurRecord();
    DSDEBUG<<"加载当前统计数据完成";
    //删除过期图像

    QDate curDate=QDate::currentDate();
    int days=PackageChecker::getInstance()->Options->SaveDaysNum();
    QVector<QString> files;
    QString saveName=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存");

    FileHelper::getAllFileFolder(saveName,files);
    for(int i=0;i<files.size();i++)
    {
        QString tmp=saveName+"/"+files[i]+"/NG/";
        QVector<QString> filesTmp;
        FileHelper::getAllFileFolder(tmp,filesTmp);
        for(int j=0;j<filesTmp.size();j++)
        {
            QDate tmpDate=QDate::fromString(filesTmp[j],"yyyy-MM-dd");
            if(tmpDate.addDays(days)<curDate)
            {
                FileHelper::deleteDirAll(tmp+filesTmp[j]);
            }
        }


        tmp=saveName+"/"+files[i]+"/OK/";
        FileHelper::getAllFileFolder(tmp,filesTmp);
        for(int j=0;j<filesTmp.size();j++)
        {
            QDate tmpDate=QDate::fromString(filesTmp[j],"yyyy-MM-dd");
            if(tmpDate.addDays(days)<curDate)
            {
                FileHelper::deleteDirAll(tmp+filesTmp[j]);
            }
        }
    }
    DSDEBUG<<"删除旧图像完成";



    //光源设置
    //    UINT8 packetBuf[PACKET_BUF_SIZE];
    //    memset(packetBuf, 0, PACKET_BUF_SIZE * sizeof(UINT8));
    //    if(!OpenPort(1, 115200))
    //    {
    //        frmMessageBox *msg = new frmMessageBox;
    //        msg->SetMessage(QString("通信端口被占用,请先关闭板卡调试软件并重启系统!"), 2);
    //        msg->exec();
    //        return;
    //    }
    //    DSDEBUG<<"开始读取板卡参数";
    //    XModemCommEngine(SLAVE1_CODE, R_CODE, UPLOAD_DATA_LEN, packetBuf);
    //    DSDEBUG<<"读取板卡参数完成";
    //    unsigned int kk = CameraConfig->getLightCode(1);
    //    DSDEBUG<<"获取lightcode:"<<kk;
    //    packetBuf[1] = kk & 0xFF;
    //    packetBuf[2] = (kk>>8) & 0xFF;
    //    packetBuf[3] = (kk>>16) & 0xFF;
    //    packetBuf[4] = (kk>>24) & 0xFF;
    //    int crcSum = CRC16(packetBuf, (DOWNLOAD_DATA_LEN - 2));
    //    packetBuf[ADDR_FOR_CRC_LBYTE] = (UINT8)(crcSum & 0xFF);
    //    packetBuf[ADDR_FOR_CRC_HBYTE] = (UINT8)((crcSum >> 8) & 0xFF);
    //    DSDEBUG<<"开始写入板卡参数";
    //    XModemCommEngine(SLAVE1_CODE, W_CODE, DOWNLOAD_DATA_LEN, packetBuf);
    //    DSDEBUG<<"写入板卡参数完成";
    //    ClosePort();
    //    //光源设置
    //    DSDEBUG<<"光源初始化完成";


    user.setUserName("操作员");
#ifdef _DEBUG
    user.setUserGroup(ENUMUSERGROUP_ADMIN);
#else
    user.setUserGroup(ENUMUSERGROUP_OPERATOR);
#endif

    //    for (int i = 0; i < 4; i++)
    //    {
    //        //作为输出引脚
    //        this->IOContol->setLevel(i, 0xFFFFFFFF, 0xFF);
    //    }

}

bool PackageChecker::testIsXmlfileBroken(QString xmlpath)
{
    TinyXmlDocument doc;
    QString  xmlConfigPath=xmlpath+"/config.xml";

    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string tmpName = code->fromUnicode(xmlConfigPath).data();

    XMLError ret= doc.LoadFile(tmpName.c_str());

    //DSDEBUG<<"xmlConfigPath:"<<xmlConfigPath<<endl;


    int numCam=this->Options->MaxCameraNum();
    for(int i=1;i<= numCam;i++)
    {
        QString testimgpath=xmlpath+QString("/ImgPro%1.bmp").arg(i);

        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string tmpName = code->fromUnicode(testimgpath).data();

        cv::Mat testImg= cv::imread(tmpName);
        bool em=testImg.empty();
        if(testImg.empty())
        {
            return true;
        }
    }



    QFile fcopytofile(xmlConfigPath);
    XMLElement * eleTabacco = doc.RootElement();
    if( fcopytofile.exists() && (eleTabacco==NULL))
    {
        return true;
    }
    return false;
}

bool PackageChecker::copyBackUpFiletoDest(QString backupfile,QString copytofile)
{

    QString backupxmlpath=backupfile+"/config.xml";
    QString destxmlpath=copytofile+"/config.xml";
    QFile fbackupfile(backupxmlpath);
    if(!fbackupfile.exists())
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(DSSystemParam::BrandName +tr(" 配置文件损坏，请关闭软件，删除配置文件重新配置!")), 2);
        msg->exec();
        return false;
    }

    QFile fcopytofile(destxmlpath);
    QDir dirCopytofile(copytofile);
    if(!dirCopytofile.exists())
    {
        dirCopytofile.mkpath(copytofile);
    }

    if(fcopytofile.exists())
    {
        fcopytofile.remove();
    }

    bool ret=  fbackupfile.copy(destxmlpath);


    int numCam=this->Options->MaxCameraNum();
    for(int i=1;i<= numCam;i++)
    {
        QString backupimgpath=backupfile+QString("/ImgPro%1.bmp").arg(i);
        QString destimgpath=copytofile+QString("/ImgPro%1.bmp").arg(i);

        //img1;
        QFile fbackupimg(backupimgpath);
        if(!fbackupimg.exists())
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(QString(DSSystemParam::BrandName +tr(" 配置文件损坏，请关闭软件，删除配置文件重新配置!")), 2);
            msg->exec();
            return false;
        }

        QFile fcopytoimg(destimgpath);
        if(fcopytoimg.exists())
        {
            fcopytoimg.remove();
        }
        ret= ret& fbackupimg.copy(destimgpath);
    }
    return  ret;
}

void PackageChecker::loadAnalysisBrand(QString brandName)
{
    clock_t start= clock();
    if(ImgTobaccoAnalysis!=nullptr)
    {
        delete  ImgTobaccoAnalysis;
        ImgTobaccoAnalysis=nullptr;
    }
    ImgTobaccoAnalysis=new ImgTobacco;
    ImgTobaccoControl control(ImgTobaccoAnalysis);
    control.loadConfig(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.loadRefImageFromDir(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.loadTemplate(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);

    DSDEBUG<<"加载时间："<<clock()-start<<"ms"<<endl;
}




PackageChecker* PackageChecker::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == nullptr) {
        m_Instance = new PackageChecker();
    }
    m_Mutex.unlock();
    return m_Instance;
}
