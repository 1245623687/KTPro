﻿#ifndef DSOPTIONS_H
#define DSOPTIONS_H
#include<QString>
#include<mutex>
#include<QTime>
#include"common/filehelper.h"
#include"common/IniFile.h"
#include"common/infile2.h"
#include"dssystemparam.h"
#include"DSDebug.h"
class DSClsOptions:public QObject
{
    Q_OBJECT
public:

    static DSClsOptions* getInstance();


    void save();


    void setMachineName(QString machineName){this->m_MachineName=machineName;}
    QString MachineName(){return this->m_MachineName;}


    void setMaxSaveImgSize(int size){this->m_MaxSaveImgSize=size;}
    int MaxSaveImgSize(){return this->m_MaxSaveImgSize;}


    void setIsLimitMaxSize(bool YesOrNo){this->m_LimtMaxSize=YesOrNo;}
    int getIsLimteMaxSize(){return m_LimtMaxSize;}


    void setSaveType(ENUMIMGSAVETYPE type){this->m_SaveType=type;}
    ENUMIMGSAVETYPE SaveType(){return  this->m_SaveType;}

    void setCameraType(ENUMCAMERATYPE type){this->m_CameraType=type;}
    ENUMCAMERATYPE CameraType(){return  this->m_CameraType;}

    void setImgSaveFormat(ENUMIMGSAVEFORMAT type){this->m_ImgSaveFormat=type;}
    ENUMIMGSAVEFORMAT ImgSaveFormat(){return  this->m_ImgSaveFormat;}

    void setIOTYPE(ENUMIOTYPE type){this->m_IOType=type;}
    ENUMIOTYPE IOTYPE(){return  this->m_IOType;}

    void setMaxCameraNum(int num){this->m_MaxCameraNum=num;}
    int MaxCameraNum(){return this->m_MaxCameraNum;}

    void setImgSavePath(QString savePath){this->m_ImgSavePath=savePath;}
    QString ImgSavePath(){return this->m_ImgSavePath;}

    void setOutputType(ENUMOUTPUTTYPE type){this->m_OutputType=type;}
    ENUMOUTPUTTYPE OutputType(){return  this->m_OutputType;}

    void setDisplayType(ENUMDISPLAYTYPE type){this->m_DisplayType=type;}
    ENUMDISPLAYTYPE DisplayType(){return  this->m_DisplayType;}

    void setShiftMorning(QTime time){this->m_ShiftMorning=time;}
    QTime ShiftMorning(){return this->m_ShiftMorning;}

    void setShiftAfternoon(QTime time){this->m_ShiftAfternoon=time;}
    QTime ShiftAfternoon(){return this->m_ShiftAfternoon;}

    void setShiftNight(QTime time){this->m_ShiftNight=time;}
    QTime ShiftNight(){return this->m_ShiftNight;}


    void setWaitTime(int waittime){this->m_WaitTime=waittime;}
    int WaitTime(){return  this->m_WaitTime;}

    void setSaveDaysNum(int days){this->m_SaveDaysNum=days;}
    int SaveDaysNum(){return  this->m_SaveDaysNum;}

    void setCheckMode(ENUMCHECKMODETYPE mode){this->m_CheckModeType=mode;}
    ENUMCHECKMODETYPE checkMode(){
        return m_CheckModeType;}


    void setEphDisplay(ENUMEPHDISPLAYTYPE mode){this->m_EphDispType=mode;}
    ENUMEPHDISPLAYTYPE ephDisplay(){return m_EphDispType;}


    void setProbNum(ENUMPROBNUM mode){this->m_ProbNum=mode;}
    ENUMPROBNUM getProbNum(){return m_ProbNum;}


    void setArrangeType(ENUMARRANGETYPE mode){this->m_ArrangeType=mode;}
    ENUMARRANGETYPE getArrangeType(){return m_ArrangeType;}

    void setCom1State(int  state){this->m_Com1=state;}
    int getCom1State(){return m_Com1;}

    void setCom2State(int  state){this->m_Com2=state;}
    int getCom2State(){return m_Com2;}

    void setCom3State(int  state){this->m_Com3=state;}
    int getCom3State(){return m_Com3;}

    void setCom4State(int  state){this->m_Com4=state;}
    int getCom4State(){return m_Com4;}


    //数据采集
    void setIPAddress(QString ipAddress){this->m_strIpAddress=ipAddress;}
    QString getIPAddress(){return  this->m_strIpAddress;}

    void setIPPort(uint16_t port){this->m_IpPort=port;}
    uint16_t getIPPort(){return  this->m_IpPort;}

    void setSendInterval(int interval){this->m_iInterval=interval;}
    int getSendInterval(){return  this->m_iInterval;}



    ~DSClsOptions()
    {
        DSDEBUG<<"~DSClsOptions()"<<endl;
    }

private:
    class GC
    {
    public :
        ~GC()
        {
            if (m_Instance != nullptr )
            {
                delete m_Instance;
                m_Instance = nullptr ;
            }
        }
    };
    static GC gc;

private:
    void load();
    DSClsOptions();

    static std::mutex m_Mutex;
    static DSClsOptions* m_Instance;
    INIFile m_IniFile;
    InFile2 m_IniFile2;


//    enum ENUMPUTPUTTYPE
//    {
//        ENUMPUTPUTTYPE_OUT,
//        ENUMPUTPUTTYPE_NOOUT
//    };

//    enum ENUMDISPLAYTYPE
//    {
//        ENUMDISPLAYTYPE_ALLAREA,
//        ENUMDISPLAYTYPE_DEFECTAREA,
//        ENUMDISPLAYTYPE_NODISPLAY,
//    };

private:
    QString m_MachineName;
    double m_MaxSaveImgSize;
    bool m_LimtMaxSize;

    ENUMIMGSAVETYPE m_SaveType;
    ENUMCAMERATYPE m_CameraType;
    ENUMIMGSAVEFORMAT m_ImgSaveFormat;
    ENUMIOTYPE m_IOType;

    ENUMCHECKMODETYPE m_CheckModeType;
    ENUMEPHDISPLAYTYPE m_EphDispType;

    ENUMPROBNUM m_ProbNum;

    int m_MaxCameraNum;
    QString m_ImgSavePath;
    ENUMOUTPUTTYPE m_OutputType;
    ENUMDISPLAYTYPE m_DisplayType;
    int m_SaveDaysNum=30;
    int m_WaitTime;


    int m_Com1,m_Com2,m_Com3,m_Com4;
     ENUMARRANGETYPE m_ArrangeType;


     QString m_strIpAddress;
     uint16_t m_IpPort;
     int m_iInterval;


    QTime m_ShiftMorning;
    QTime m_ShiftAfternoon;
    QTime m_ShiftNight;
};

#endif // DSOPTIONS_H
