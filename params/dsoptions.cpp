#include "dsoptions.h"
#include<QTextCodec>
#include<QString>
#include"params/dssystemparam.h"
#include"config/dlgsysteminit.h"
#include"dsdebug.h"

#include"packagechecker.h"
#include"qtCommon/frmmessagebox.h"
mutex DSClsOptions::m_Mutex;
DSClsOptions* DSClsOptions::m_Instance = nullptr;
DSClsOptions ::GC DSClsOptions ::gc;
DSClsOptions::DSClsOptions()
{
    load();
}

void DSClsOptions::load()
{
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/Options.ini";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name =code->fromUnicode(path).data();

    //std::string name=path.toStdString();

    QFileInfo fi(path);
    if(!fi.exists())
    {
        ENUMCAMERATYPE tmpcameraType=ENUMCAMERATYPE_AVT;
        int tmpcameraNum=0;
        ENUMIOTYPE tmpIOType=ENUMIOTYPE_YANHUAGPIO;

        DlgSystemInit dlg(&tmpcameraType,&tmpcameraNum,&tmpIOType);
        int ret=dlg.exec();
        if(!ret)
        {
            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage("系统配置失败!", 2);
            msg->exec();
            this->m_Mutex.unlock();
            throw PackageChecker::PackageCheckerException(-1,"未进行系统配置");
        }


//        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"Options(backup).ini");
//        QThread::msleep(50);
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"Options.ini");
        this->m_IniFile.Open(name);

        bool tmpb;
        std::string tmpval;

        tmpb=this->m_IniFile.GetValue("Options", "MachineName", tmpval);
        if (tmpb) setMachineName(QString::fromStdString(tmpval));
        else setMachineName("Default");

        tmpb=this->m_IniFile.GetValue("Options", "MaxSaveImageSize", tmpval);
        if (tmpb) setMaxSaveImgSize(atoi(tmpval.c_str()));
        else setMaxSaveImgSize(1024);

        tmpb=this->m_IniFile.GetValue("Options", "IsLimitMaxImageSize", tmpval);
        if (tmpb) setMaxSaveImgSize(atoi(tmpval.c_str()));
        else setIsLimitMaxSize(0);



        tmpb = this->m_IniFile.GetValue("Options", "ImgSaveType", tmpval);
        if (tmpb) setSaveType(static_cast<ENUMIMGSAVETYPE>(atoi(tmpval.c_str())));
        else setSaveType(ENUMIMGSAVETYPE_NG);

        tmpb = this->m_IniFile.GetValue("Options", "CameraType", tmpval);
        if (tmpb) setCameraType(static_cast<ENUMCAMERATYPE>(atoi(tmpval.c_str())));
        else setCameraType(tmpcameraType);

        tmpb = this->m_IniFile.GetValue("Options", "ImgSaveFormat", tmpval);
        if (tmpb) setImgSaveFormat(static_cast<ENUMIMGSAVEFORMAT>(atoi(tmpval.c_str())));
        else setImgSaveFormat(ENUMIMGSAVEFORMAT_BMP);

        tmpb = this->m_IniFile.GetValue("Options", "IOType", tmpval);
        if (tmpb) setIOTYPE(static_cast<ENUMIOTYPE>(atoi(tmpval.c_str())));
        else setIOTYPE(tmpIOType);

        tmpb=this->m_IniFile.GetValue("Options", "MaxCameraNum", tmpval);
        if (tmpb) setMaxCameraNum(atoi(tmpval.c_str()));
        else setMaxCameraNum(tmpcameraNum);

        tmpb=this->m_IniFile.GetValue("Options", "ImgSavePath", tmpval);
        if (tmpb) setImgSavePath(QString::fromStdString(tmpval));
        else setImgSavePath(DSSystemParam::AppPath+"/图像保存");

        tmpb = this->m_IniFile.GetValue("Options", "OutputType", tmpval);
        if (tmpb) setOutputType(static_cast<ENUMOUTPUTTYPE>(atoi(tmpval.c_str())));
        else setOutputType(ENUMOUTPUTTYPE_NOOUT);

        tmpb = this->m_IniFile.GetValue("Options", "DisplayType", tmpval);
        if (tmpb) setDisplayType(static_cast<ENUMDISPLAYTYPE>(atoi(tmpval.c_str())));
        else setDisplayType(ENUMDISPLAYTYPE_DEFECTAREA);


        tmpb = this->m_IniFile.GetValue("Options", "SaveDaysNum", tmpval);
        if (tmpb) setSaveDaysNum(atoi(tmpval.c_str()));
        else setSaveDaysNum(30);

        QTime tmpTime;
        tmpb = this->m_IniFile.GetValue("Options", "ShiftMorning", tmpval);
        tmpTime.fromString(QString::fromStdString(tmpval));
        if (tmpb) setShiftMorning(tmpTime);
        else setShiftMorning(QTime(0,0,0));

        tmpb = this->m_IniFile.GetValue("Options", "ShiftAfternoon", tmpval);
        tmpTime.fromString(QString::fromStdString(tmpval));
        if (tmpb) setShiftAfternoon(tmpTime);
        else setShiftAfternoon(QTime(0,0,0));

        tmpb = this->m_IniFile.GetValue("Options", "ShiftNight", tmpval);
        tmpTime.fromString(QString::fromStdString(tmpval));
        if (tmpb) setShiftNight(tmpTime);
        else setShiftNight(QTime(0,0,0));

        tmpb = this->m_IniFile.GetValue("Options", "CheckMode", tmpval);
        if (tmpb) setCheckMode(static_cast<ENUMCHECKMODETYPE>(atoi(tmpval.c_str())) );
        else setCheckMode(ENUMCHECKMODETYPE_PIC);

        tmpb = this->m_IniFile.GetValue("Options", "EphDisplay", tmpval);
        if (tmpb) setEphDisplay(static_cast<ENUMEPHDISPLAYTYPE>(atoi(tmpval.c_str())));
        else setSaveDaysNum(ENUMEPHDISPLAYTYPE_DISPLAY);

       // savebackup();
        QThread::msleep(50);
        save();
    }


    this->m_IniFile.Open(name);

    bool b;
    std::string val;

    b=this->m_IniFile.GetValue("Options", "MachineName", val);
    if (b) setMachineName(QString::fromStdString(val));
    else setMachineName("Default");

    b=this->m_IniFile.GetValue("Options", "MaxSaveImageSize", val);
    if (b) setMaxSaveImgSize(atoi(val.c_str()));
    else setMaxSaveImgSize(1024);

    b=this->m_IniFile.GetValue("Options", "IsLimitMaxImageSize", val);
    if (b) setIsLimitMaxSize(atoi(val.c_str()));
    else setIsLimitMaxSize(0);

    b = this->m_IniFile.GetValue("Options", "ImgSaveType", val);
    if (b) setSaveType(static_cast<ENUMIMGSAVETYPE>(atoi(val.c_str())));
    else setSaveType(ENUMIMGSAVETYPE_NG);

    b = this->m_IniFile.GetValue("Options", "CameraType", val);
    if (b) setCameraType(static_cast<ENUMCAMERATYPE>(atoi(val.c_str())));
    else setCameraType(ENUMCAMERATYPE_AVT);

    b = this->m_IniFile.GetValue("Options", "ImgSaveFormat", val);
    if (b) setImgSaveFormat(static_cast<ENUMIMGSAVEFORMAT>(atoi(val.c_str())));
    else setImgSaveFormat(ENUMIMGSAVEFORMAT_BMP);

    b = this->m_IniFile.GetValue("Options", "IOType", val);
    if (b) setIOTYPE(static_cast<ENUMIOTYPE>(atoi(val.c_str())));
    else setIOTYPE(ENUMIOTYPE_YANHUAGPIO);

    b=this->m_IniFile.GetValue("Options", "MaxCameraNum", val);
    if (b) setMaxCameraNum(atoi(val.c_str()));
    else setMaxCameraNum(16);

    b=this->m_IniFile.GetValue("Options", "SaveDaysNum", val);
    if (b) setSaveDaysNum(atoi(val.c_str()));
    else setSaveDaysNum(7);


    b=this->m_IniFile.GetValue("Options", "ImgSavePath", val);
    if (b) setImgSavePath(QString::fromStdString(val));
    else setImgSavePath(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/图像保存");


    b = this->m_IniFile.GetValue("Options", "OutputType", val);
    if (b) setOutputType(static_cast<ENUMOUTPUTTYPE>(atoi(val.c_str())));
    else setOutputType(ENUMOUTPUTTYPE_NOOUT);

    b = this->m_IniFile.GetValue("Options", "DisplayType", val);
    if (b) setDisplayType(static_cast<ENUMDISPLAYTYPE>(atoi(val.c_str())));
    else setDisplayType(ENUMDISPLAYTYPE_DEFECTAREA);


    QTime tmpTime2;
    b = this->m_IniFile.GetValue("Shift", "ShiftMorning", val);
    tmpTime2=QTime::fromString(QString::fromStdString(val),"hh:mm:ss");
    if (b) setShiftMorning(tmpTime2);
    else setShiftMorning(QTime(0,0,0));

    b = this->m_IniFile.GetValue("Shift", "ShiftAfternoon", val);
     tmpTime2=QTime::fromString(QString::fromStdString(val),"hh:mm:ss");
    if (b) setShiftAfternoon(tmpTime2);
    else setShiftAfternoon(QTime(0,0,0));

    b = this->m_IniFile.GetValue("Shift", "ShiftNight", val);
     tmpTime2=QTime::fromString(QString::fromStdString(val),"hh:mm:ss");
    if (b) setShiftNight(tmpTime2);
    else setShiftNight(QTime(0,0,0));


    b = this->m_IniFile.GetValue("Other", "WaitTime", val);
    if (b) setWaitTime(atoi(val.c_str()));
    else setWaitTime(69);

    b = this->m_IniFile.GetValue("Options", "CheckMode", val);
    if (b) setCheckMode(static_cast<ENUMCHECKMODETYPE>(atoi(val.c_str())) );
    else setCheckMode(ENUMCHECKMODETYPE_PIC);

    b = this->m_IniFile.GetValue("Options", "EphDisplay", val);
    if (b) setEphDisplay(static_cast<ENUMEPHDISPLAYTYPE>(atoi(val.c_str())));
    else setSaveDaysNum(ENUMEPHDISPLAYTYPE_DISPLAY);

}


void DSClsOptions::save()
{

    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/Options.ini";
    QFileInfo fi(path);
    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"Options.ini");
    }

   // QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = path.toStdString();
    this->m_IniFile2.Open(path);

    m_IniFile2.SetValue("Options", "MachineName", this->MachineName());
    m_IniFile2.SetValue("Options", "MaxSaveImageSize",QString::number((int)this->MaxSaveImgSize()));
    m_IniFile2.SetValue("Options", "IsLimitMaxImageSize",QString::number((int)this->getIsLimteMaxSize()));

    m_IniFile2.SetValue("Options", "CameraType",QString::number((int)this->CameraType()));
    m_IniFile2.SetValue("Options", "ImgSaveType",QString::number((int)this->SaveType()));

    m_IniFile2.SetValue("Options", "ImgSaveFormat",QString::number((int)this->ImgSaveFormat()));
    m_IniFile2.SetValue("Options", "IOType",QString::number((int)this->IOTYPE()));

    m_IniFile2.SetValue("Options", "MaxCameraNum",QString::number((int)this->MaxCameraNum()));

    m_IniFile2.SetValue("Options", "ImgSavePath", this->ImgSavePath());

    m_IniFile2.SetValue("Options", "OutputType",QString::number((int)this->OutputType()));
    m_IniFile2.SetValue("Options", "DisplayType",QString::number((int)this->DisplayType()));

    m_IniFile2.SetValue("Options", "SaveDaysNum",QString::number(this->m_SaveDaysNum));

    m_IniFile2.SetValue("Shift", "ShiftMorning",this->m_ShiftMorning.toString("hh:mm:ss"));
    m_IniFile2.SetValue("Shift", "ShiftAfternoon",this->m_ShiftAfternoon.toString("hh:mm:ss"));
    m_IniFile2.SetValue("Shift", "ShiftNight",this->m_ShiftNight.toString("hh:mm:ss"));

    m_IniFile2.SetValue("Options", "CheckMode",QString::number((int)this->checkMode()));
    m_IniFile2.SetValue("Options", "EphDisplay",QString::number((int)this->ephDisplay()));


    m_IniFile2.Save();
}


DSClsOptions* DSClsOptions::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == NULL) {
        m_Instance = new DSClsOptions();
    }
    m_Mutex.unlock();
    return m_Instance;
}

