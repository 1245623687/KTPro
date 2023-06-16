#ifndef PACKAGECHECKER_H
#define PACKAGECHECKER_H

#include<mutex>
#include<QMap>
#include<QObject>
#include"params/dssystemparam.h"
#include"params/dsoptions.h"
#include"params/dslastconfig.h"
#include"dsbase.h"
#include"ImageHandle/opencvimage.h"
#include"CheckOperator/imgtobacco.h"
#include"user/userinfo.h"
#include<QTimer>
#include"params/clscameraconfig.h"
#include"params/clsphototelecconfig.h"
#include"Camera/ccarshuixingmer231c.h"
#include"IO/ciobase.h"
#include"COM/basecom.h"

#include <iostream>
#include<string>
#include <fstream>
#include<QMutex>
#include<QVector>




class PackageChecker: public QObject
{
    Q_OBJECT

signals:
   // void updateCheckRetSig(int,QVector<int>&);

    void updateCheckRetSig(int);
    void sendCheckCommandSig(int);

public:
    enum ENUMPACKAGECHECKERERR
    {
        ENUMPACKAGECHECKERERR_UNKNOWNERR=1,
        ENUMPACKAGECHECKERERR_OPENCAMERAFAIL=2,
        ENUMPACKAGECHECKERERR_LOADOPTIONSFAIL=3,
        ENUMPACKAGECHECKERERR_LOADLASTCONFIGFAIL=4,
        ENUMPACKAGECHECKERERR_ADVANTENCHGPIOFAIL=5,
        ENUMPACKAGECHECKERERR_OPENCOMFAIL=6,
    };


    class PackageCheckerException:public exception
    {
    public:
        PackageCheckerException(unsigned int code, const std::string &what_arg)
            : m_code(code)
            , m_what_arg(what_arg)
        {
        }
        PackageCheckerException(unsigned int code)
            : m_code(code)
            , m_what_arg(getErrInfo(static_cast<ENUMPACKAGECHECKERERR>(m_code)))
        {
        }
        virtual const char * what(void) const noexcept override
        {
            auto text = "错误代码：" + std::to_string(m_code) + "\r\n错误信息：" + m_what_arg;
            return text.c_str();
        }
    private:



        std::string getErrInfo(ENUMPACKAGECHECKERERR errid)
        {

            switch (errid) {
            case ENUMPACKAGECHECKERERR_UNKNOWNERR:
                return "位置错误!";
                break;
            case ENUMPACKAGECHECKERERR_OPENCAMERAFAIL:
                return "相机打开失败!";
            case ENUMPACKAGECHECKERERR_LOADOPTIONSFAIL:
                return "加载Options失败!";
            case ENUMPACKAGECHECKERERR_LOADLASTCONFIGFAIL:
                return "加载LastConfig失败!!";
                break;
            }
        }
    private:
        unsigned int m_code;
        std::string m_what_arg;

    };


    /*--------------------------华丽的分割线-------------------------------*/


public:


    static PackageChecker* getInstance();

    void init();
    bool testIsXmlfileBroken(QString xmlpath);
    bool copyBackUpFiletoDest(QString backupfile,QString copytofile);

    void loadRunBrand();
    void loadAnalysisBrand(QString brandName);

    void changeBrand(QString brandName);

    ~PackageChecker();

public:



    static QMutex m_BadImgMutex;
    static unsigned int getRunTime(){return QDateTime::currentDateTime().toTime_t()-m_TimeSystemStart.toTime_t();}
private slots:
    void timerlog();
private:
   static std::mutex m_Mutex;




    static PackageChecker* m_Instance;

    class GC
    {
    public :
        ~GC()
        {
            if (m_Instance != NULL )
            {
                delete m_Instance;
                m_Instance = NULL ;
            }
        }
    };
    static GC gc;


private:
    PackageChecker();

public:
    CIOBase * IOContol;
    DSClsOptions * Options;
    DSClsLastConfig* LastConfig;
    BaseCom *m_pBaseCom;

    QTimer * m_TimerLog;

    UserInfo user;

    int ImgWid;
    int ImgHei;


    int ImgWids[CAMERANUM_MAX];
    int ImgHeis[CAMERANUM_MAX];


    //    QMap<int,ClsCameraConfig::StuCameraParam> CamerasCfgParams;
    QMap<int,CCameraBase*> Cameras;
    ClsCameraConfig * CameraConfig;

     ClsPhototElecConfig* PhotoElecConfig;


    ImgTobacco * ImgTobaccoAnalysis=nullptr;
    ImgTobacco * ImgTobaccoRun;


    QMap<int,OpencvImage> CurImage;//当前图片



    QMap<int, OpencvImage*> CurBadImage;

    QMap<int, OpencvImage> m_qmapCurBadImage;


    OpencvImage* cloneOpencvImg(int,OpencvImage&);



    void  deleteCloneOpencvImage(int );

    QAtomicInt::QAtomicInteger RunParam_CalcTime;//当前计算时间ms

    QAtomicInt::QAtomicInteger RunParam_CalcNumAll;//当前班次处理总数
    QAtomicInt::QAtomicInteger RunParam_CalcNumNg;//当前班次的Ng数量

    QAtomicInt::QAtomicInteger RunParam_CalcNumAllCams[CAMERANUM_MAX];//当前班次处理总数
    QAtomicInt::QAtomicInteger RunParam_CalcNumNgCams[CAMERANUM_MAX];//当前班次的Ng数量
    QAtomicInt::QAtomicInteger RunParam_CalcTimeCams[CAMERANUM_MAX];//当前班次的Ng数量


    QAtomicInt::QAtomicInteger RunParam_CalcNumNgGDs[CAMERANUM_MAX];//当前班次的Ng数量
    QAtomicInt::QAtomicInteger RunParam_CalcNumNgTotals[CAMERANUM_MAX];//当前班次的Ng数量




    ENUMSHIFT RunParam_Shift;
    QAtomicInt::QAtomicInteger UpdateCheckOperator;
    QAtomicInt::QAtomicInteger UpdateCheckOperator2;

    QAtomicInt::QAtomicInteger IsCalcaulateFinish[CAMERANUM_MAX];

    QAtomicInt::QAtomicInteger WarningFlag[CAMERANUM_MAX];




    QAtomicInt::QAtomicInteger CheckRet[CAMERANUM_MAX];//检测结果

    static unsigned char ErrMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX];
    static int ErrRecord[CAMERANUM_MAX][CHECKOPERATORNUM_MAX];
    static QAtomicInt::QAtomicInteger ErrPhe[2];

    static int RetPheQZ[20];
   static QMutex m_MutexRetPheQZ;
   static QMutex m_MutexRetPheKT;

    static int RetPheKT[20];



    static bool DynamicGrab;
public:

    static QDateTime m_TimeSystemStart;


};

#endif // PACKAGECHECKER_H
