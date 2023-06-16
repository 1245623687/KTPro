#ifndef DLGPHOTOELECTRICITYCFG_H
#define DLGPHOTOELECTRICITYCFG_H

#include <QDialog>
#include"PackageChecker.h"
#include"QByteArray"
#include<QStringListModel>
#include<QtConcurrentRun>
#include"kcontrols/dlgkeyboard.h"

#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
#include<QList>
#include<QLabel>
#include<QElapsedTimer>
#include"qtCommon/frmmessagebox.h"
#include"basecom.h"
#include<QStandardItemModel>
#include<QAtomicInteger>


namespace Ui {
class dlgphotoelectricitycfg;
}

class dlgphotoelectricitycfg : public QDialog
{
    Q_OBJECT

public:
    explicit dlgphotoelectricitycfg(QWidget *parent = nullptr);
    ~dlgphotoelectricitycfg();

public slots:

    void upDateCheckRet(int type);
    void sendRunCheckCommand(int type);
private slots:


    void showKeyBoard(KControlsBase* pLineEdit);

    void on_btnMenu_Close_clicked();


    void on_pushButton_qzcapangleset_clicked();

    void on_pushButton_qzkickset_clicked();

    void on_pushButton_qztrigangleset_clicked();

    void on_pushButton_qzselfangleset_clicked();

    void on_pushButton_qzprob_clicked();

    void on_pushButton_qzfaultsigset_clicked();



    void on_pushButton_ktcapangleset_clicked();

    void on_pushButton_ktkickset_clicked();

    void on_pushButton_kttrigangleset_clicked();

    void on_pushButton_ktselfangleset_clicked();

    void on_pushButton_ktprob_clicked();

    void on_pushButton_ktfaultsigset_clicked();




    void on_pushButton_qzApply_clicked();

    void on_pushButton_qzStopApply_clicked();

    void on_pushButton_ktApply_clicked();

    void on_pushButton_ktStopApply_clicked();

    //    void showQZAngleAndADCValue(bool,int,int,int,int);
    //    void showKTAngleAndADCValue(bool,int,int,int,int);


    void showQZAngleAndADCValue();
    void showKTAngleAndADCValue();


    void on_radioButton_runMode_toggled(bool checked);
    void on_pushButtonSaveCfg_clicked();

    void on_comboBox_qzcapangle_currentIndexChanged(int index);

    void on_comboBox_qzprob_currentIndexChanged(int index);

    void on_comboBox_ktcapangle_currentIndexChanged(int index);

    void on_comboBox_ktprob_currentIndexChanged(int index);

    void on_pushButtonQZRun_clicked();

    void on_pushButtonQZRecord_clicked();

    void on_pushButtonQZResetNum_clicked();

    void on_pushButtonKTRun_clicked();

    void on_pushButtonKTRecord_clicked();

    void on_pushButtonKTResetNum_clicked();

    void on_pushButtonQZRefresh_clicked();

    void on_pushButtonKTRefresh_clicked();

    void on_pushButtonQZPre_clicked();

    void on_pushButtonQZNext_clicked();

    void on_pushButtonKTPre_clicked();

    void on_pushButtonKTNext_clicked();

    void on_tableViewQZ_clicked(const QModelIndex &index);

    void on_tableViewKT_clicked(const QModelIndex &index);

    void on_pushButton_qzprobRatio_clicked();

    void on_pushButton_ktprobRatio_clicked();

private:
    Ui::dlgphotoelectricitycfg *ui;

    void InitStyle();
    void InitControl();

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    
    void appendInfoText(QString settingName,bool ret,QByteArray text,QByteArray Receivetext);

    void appendInfoText(QString ,bool ret,QString );

    QByteArray genSendByArray(int OperaCode,int v1,int v2 );
    QStringListModel* m_listModel;

    QFuture<void>  m_QZFuture;
    QFuture<void>  m_KTFuture;

    bool m_bQZisRuning;
    bool m_bKTisRuning;


    void  startQZTestRunModel();
    void  startKTTestRunModel();

    int m_ktangle, ktprobv1, ktprobv2, ktprobv3;
    int m_qzangle, qzprobv1, qzprobv2, qzprobv3;

    int m_txtCnt;

    QList<QLabel*> m_qzLabels;
    QList<QLabel*> m_ktLabels;

    QList<QLabel*> m_qzLabels2;
    QList<QLabel*> m_ktLabels2;



    bool m_bRuning;

    QAtomicInt:: QAtomicInteger m_lqzTotalNum;
    QAtomicInt::QAtomicInteger m_lqzNGNum;

    QQueue<QPair<int,int>> m_qzQue;
    QMap<QString,QVector<int>> m_qzMap;
    QMutex m_mutexMap;
    QMap<QString,QVector<int>> qzMap;


    QAtomicInt:: QAtomicInteger m_lktTotalNum;
    QAtomicInt:: QAtomicInteger m_lktNGNum;
    QQueue<QPair<int,int>> m_ktQue;
    QMap<QString,QVector<int>> m_ktMap;
    QMap<QString,QVector<int>> ktMap;

    QStandardItemModel* m_modelQZ=nullptr;
    QStandardItemModel* m_modelKT=nullptr;

    int m_qzCurIndex;
    int m_ktCurIndex;


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

signals:

    void updataQZADCValueSig();
    void updataKTADCValueSig();

};

#endif // DLGPHOTOELECTRICITYCFG_H
