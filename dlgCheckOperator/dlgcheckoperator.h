#ifndef DLGCHECKOPERATOR_H
#define DLGCHECKOPERATOR_H

#include <QDialog>
#include<QStandardItemModel>
#include<QButtonGroup>
#include"PackageChecker.h"
#include"controls/graphicssceneconfig.h"
#include"thread/kthreadgrab.h"
#include"controls/graphicsview.h"
namespace Ui {
class DlgCheckOperator;
}

class DlgCheckOperator : public QDialog
{
    Q_OBJECT
private:
    enum ENUMDLGCHECKOPERATORMODE
    {
        ENUMDLGCHECKOPERATORMODE_SET=0,
        ENUMDLGCHECKOPERATORMODE_ANA=1
    };

    enum ENUMSHOWMODE
    {
        ENUMSHOWMODE_ALL=0,
        ENUMSHOWMODE_CUR=1
    };
    class ImageInfo
    {
    public:
        ImageInfo()
        {

        }

        ImageInfo(QString basefileName)
        {
            this->m_baseFileName=basefileName;
            QStringList lst=basefileName.split('_');
            this->m_datetime=QDateTime::fromString(lst[0],"yyyyMMdd-hhmmss-zzz");
            this->m_cameraNo=lst[2].toInt();
            this->m_iNgType=lst[3].toInt();
            // this->m_isNg=m_type=="N"?true:false;
        }

        ImageInfo(const char*  basefileName)
        {
            QString fileName(basefileName);
            this->m_baseFileName=basefileName;
            QStringList lst=fileName.split('_');
            this->m_datetime=QDateTime::fromString(lst[0],"yyyyMMdd-hhmmss-zzz");
            this->m_cameraNo=lst[2].toInt();
            this->m_iNgType=lst[3].toInt();
            //  this->m_isNg=m_type=="N"?true:false;
        }


        static QString format(int year,int month,int day,int hour,int min,int sec,int misec,int cameraNo,bool isNg)
        {
            QDateTime datetime(QDate(year,month,day),QTime(hour,min,sec,misec));
            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
            ret+="_"+QString::number(cameraNo)+"_"+(isNg?"N":"P");
            return  ret;
        }

        static QString format(QDateTime datetime,int cameraNo,bool isNg)
        {
            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
            ret+="_"+QString::number(cameraNo)+"_"+(isNg?"N":"P");
            return  ret;
        }

        static QString format(QDate date,QTime time,int cameraNo,bool isNg)
        {
            QDateTime datetime(date,time);
            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
            ret+="_"+QString::number(cameraNo)+"_"+(isNg?"N":"P");
            return  ret;
        }

        static int compare(const ImageInfo& c1,const ImageInfo& c2)
        {
            if(c1.m_datetime!=c2.m_datetime)return c1.m_datetime>=c2.m_datetime  ;
            else
            {
                return  c1.m_cameraNo<c2.m_cameraNo;
            }
        }


    public:
        QString m_baseFileName;
        QDateTime m_datetime;
        bool m_isNg;
        //QString m_type;
        int m_cameraNo;
        int m_iNgType;
    };


public:
    explicit DlgCheckOperator(QWidget *parent = nullptr);
    ~DlgCheckOperator();
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:
//    void on_btnMenu_Max_clicked();
//    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();
    void on_toolButtonAnaImg_clicked();
    void on_toolButtonCoSet_clicked();
    void on_toolButtonCoSetLstInfo_clicked();
    void on_toolButtonCoAnaLstInfo_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;
private:
    Ui::DlgCheckOperator *ui;


private slots:
    void buttonModeJudge(int mode);
    void buttonCameraJudge(int idx);
    void buttonShowModeJudge(int mode);
    void changeCameraEnable(bool b);
    void showDlgCheckOperator(int itemIndex);
    void changeSelectedRow(int row);
    void updateCheckoperatorOption();

    void on_tableViewImgLst_clicked(const QModelIndex &index);

    void on_dateEditChose_dateChanged(const QDate &date);

    void on_comboBoxCamera_currentIndexChanged(int index);

    void on_tableViewCheckOperatorSet_clicked(const QModelIndex &index);

    void on_tableViewCheckOperatorAna_clicked(const QModelIndex &index);

    void on_pushButtonImgPre_clicked();

    void on_pushButtonImgNext_clicked();

    void on_toolButtonImgMax_clicked();

    void on_toolButtonImgMin_clicked();

    void on_toolButtonImgInitial_clicked();

    void on_tableViewCheckOperatorSet_doubleClicked(const QModelIndex &index);

    void on_tableViewCheckOperatorAna_doubleClicked(const QModelIndex &index);

    void on_toolButtonUpdate_clicked();

    void on_toolButtonShowAll_clicked();

    void on_toolButtonShowNg_clicked();

    void on_PushBtnCO_Template_clicked();

    void on_PushBtnCO_EdgePositioning_clicked();

    void on_PushBtnCO_DefectInspect_clicked();

    void on_PushBtnCO_GrayMeasurement_clicked();

    void on_PushBtnCO_BrokenLineInspect_clicked();

    void on_PushBtnCO_AssociatedInspect_clicked();

    void on_PushBtnCO_StayInspect_clicked();

    void on_PushBtnCO_DefectInspect2_clicked();
    void on_PushBtnCO_DoubleStayInspect_clicked();

    void on_pushButtonCO_CircleInspect_clicked();
     void on_PushBtnCO_DLObjectDetect_clicked();
    void on_toolButtonSave_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void InitControl();
    void InitCheckOperatorSetModel();
    void InitCheckOperatorAnaModel();
    void InitImgLstModel();
    void loadCheckOperatorSetModel(int cameraNo);
    void loadCheckOperatorAnaModel(int cameraNo);
    void InitScene();
    void loadSceneSetRect(int cameraNo);
    void loadSceneAnaRect(int cameraNo);

    void loadSceneRectAll();
    void loadAnaImageInfo(int channel);
    void updateImgLstModel();

    void loadAnalysisLstModel(int imgInfoNo);

    void doCaculate();

    void adjustCheckOperator(int itemIndex);

private slots:
    void onTimeout();
    void on_toolButtonCancel_clicked();

    void on_toolButtonSaveReturn_clicked();

    void on_toolButtonCapture_clicked();

    void on_pushButtonShowAna_clicked();

    void on_pushButtonAnaDel_clicked();






private:

    QButtonGroup * btnGroupCamera;
    QStandardItemModel* m_ModelCheckOperatorSet;
    QStandardItemModel* m_ModelCheckOperatorAna;
    QStandardItemModel* m_ModelImgLst;
    QMap<int,GraphicsSceneConfig*> m_SceneArr;

    QMap<int,GraphicsSceneConfig*> m_SceneAnaArr;
    GraphicsSceneConfig* m_SceneAna=nullptr;



   QMap<int,GraphicsView*> m_ViewArrMain;

private:
    ENUMDLGCHECKOPERATORMODE m_Mode;
    ENUMSHOWMODE m_ShowMode=ENUMSHOWMODE_ALL;
    int m_CurCameraNo=1;
    int m_AnalysisImgInfoIdx=0;
    int SelectCheckOperatorNo=1;
    QString m_DirAnalysis;//分析图片的文件夹地址
    OpencvImage m_AnalysisImage;
    QVector<ImageInfo> m_VImageInfo;

    QTimer* m_Timer=nullptr;
    int m_TimerImageNum=0;
    bool m_Capturing=false;
    static unsigned char AnalysisCalcResult[256];

     KThreadGrab * m_ThreadGrab;
};

#endif // DLGCHECKOPERATOR_H
