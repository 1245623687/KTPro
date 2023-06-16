#ifndef DLGIMAGERECORD_H
#define DLGIMAGERECORD_H

#include <QDialog>
#include<QStandardItemModel>
#include<QStandardItem>
#include<QMap>
#include<QDate>

#include"controls/graphicsscenerec.h"
#include"packagechecker.h"
namespace Ui {
class DlgImageRecord;
}

class DlgImageRecord : public QDialog
{
    Q_OBJECT
public:

    static int compareFolder(const QString str1,const QString str2)
    {
        QStringList strlst1=str1.split('_');
        QStringList strlst2=str2.split('_');
        QDate date1=QDate::fromString(strlst1[0],"yyyy-MM-dd");
        QDate date2=QDate::fromString(strlst2[0],"yyyy-MM-dd");
        int shift1=strlst1[1].toInt();
        int shift2=strlst2[1].toInt();
        if(date1!=date2) return date1>=date2;
        else return shift1>=shift2;
    }

public:
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
            this->m_shift=lst[1].toInt();
            this->m_camerano=lst[2].toInt();

        }

        ImageInfo(const char*  basefileName)
        {
            QString fileName(basefileName);
            this->m_baseFileName=basefileName;
            QStringList lst=fileName.split('_');
            this->m_datetime=QDateTime::fromString(lst[0],"yyyyMMdd-hhmmss-zzz");
            this->m_shift=lst[1].toInt();
            this->m_camerano=lst[2].toInt();

        }


//        static QString format(int year,int month,int day,int hour,int min,int sec,int misec,int code,int shift)
//        {
//            QDateTime datetime(QDate(year,month,day),QTime(hour,min,sec,misec));
//            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
//            ret+="_"+QString::number(shift)+"_"+QString::number(code);
//            return  ret;
//        }

//        static QString format(QDateTime datetime,int code,int shift)
//        {
//            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
//            ret+="_"+QString::number(shift)+"_"+QString::number(code);
//            return  ret;
//        }

//        static QString format(QDate date,QTime time,int errinfo,int shift)
//        {
//            QDateTime datetime(date,time);
//            QString ret=datetime.toString("yyyyMMdd-hhmmss-zzz");
//            ret+="_"+QString::number(shift)+"_"+QString::number(errinfo);
//            return  ret;
//        }

        static int compare(const ImageInfo& c1,const ImageInfo& c2)
        {
            return c1.m_datetime>=c2.m_datetime  ;            
        }

    public:
        QString m_baseFileName;
        QDateTime m_datetime;

        int m_shift;
        int m_camerano;

    };


public:
    explicit DlgImageRecord(QWidget *parent = nullptr);
    ~DlgImageRecord();

private:
    void InitControl();
    void loadImageInfo();
    void updateErrInfo(QStringList& errinfo);
private:
    QString getShiftName(int shift)
    {
        switch (shift)
        {
        case 0:
            return "早班";
        case 1:
            return "中班";
        case 2:
            return "晚班";
        }
        return "NULL";
    }

    int getShiftId(QString shift)
    {
        if(shift=="早班")
            return 0;
        else if(shift=="中班")
            return 1;
        else if(shift=="晚班")
            return 2;
        return  -1;
    }
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();
    void on_changeSelectedImage(const QModelIndex &index);
    void on_toolButtonUpdate_clicked();

    void on_changeShowResMode(bool b);

    void on_dateEditChose_dateChanged(const QDate &date);

    void on_toolButtonShowAll_clicked();

    void on_toolButtonShowNg_clicked();

    void on_toolButtonShowInfo_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_toolButtonDelImg_clicked();

    void on_toolButtonDelFloder_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_toolButtonRecMin_clicked();

    void on_toolButtonRecMax_clicked();

    void on_toolButtonRecInit_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgImageRecord *ui;

    GraphicsSceneRec* m_Scene=nullptr;

    QStandardItemModel* m_modelImage=nullptr;
    QStandardItemModel* m_HandleInfo=nullptr;

    bool m_showRes=true;
    bool m_showInfoMore=true;

    unsigned char * m_pCalcImg=nullptr;


    QMap<QString,QVector<ImageInfo>> m_ImageArray;
    QString m_ImagePath;
    int m_CameraNo=0;





};

#endif // DLGIMAGERECORD_H
