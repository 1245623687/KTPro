#ifndef DLGCAMERACONFIG_H
#define DLGCAMERACONFIG_H

#include <QDialog>
#include"controls/graphicsscenecapture.h"
#include<QButtonGroup>
#include<QStandardItemModel>
#include"thread/kthreadcameraconfig.h"
#include"kcontrols/klineeditkb.h"
#include"kcontrols/dlgkeyboard.h"
#include"PackageChecker.h"
#include"controls/graphicsview.h"
#include<QMap>

namespace Ui {
class DlgCameraConfig;
}

class DlgCameraConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCameraConfig(QWidget *parent = nullptr);
    ~DlgCameraConfig();


public slots:
    void showKeyBoard(KControlsBase* pLineEdit);
    void updateImage();
private:
    void InitControls();
    void SaveParam();

    void updateModel(int scale,int starty,int endy,char* pRes,int len);

private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:
    void buttonJudge(int idx);
    void changeCurCameraNo(int idx);
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();
    void on_toolButtonImgMin_clicked();

    void on_toolButtonImgMax_clicked();

    void on_toolButtonImgInitial_clicked();

    void on_lineEditGain_textChanged(const QString &arg1);

    void on_lineEditExpose_textChanged(const QString &arg1);

    //    void on_pushButtonCapture_clicked();

    void on_pushButtonCfSave_clicked();

    void on_pushButtonCfSaveRet_clicked();

    void on_pushButtonCfCancel_clicked();

    void updatePixInfo(int x,int y,int pix);





    void on_toolButtonCapture_clicked();


    void on_pushButton_clicked();

    void on_radioButton_TestModel_toggled(bool checked);

    void on_radioButton_RunModel_toggled(bool checked);

    void on_lineEditTrgGap_textChanged(const QString &arg1);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgCameraConfig *ui;
    QButtonGroup * m_BtnGroup;

    bool m_IsCapture=false;


    QByteArray genSendByArray(int OperaCode, int v1, int v2);


private:
    QMap<int,GraphicsSceneCapture*> m_SceneArr;
    int m_CurCameraNo=1;
//    QMap<int,OpencvImage> m_OpencvImage;
    QButtonGroup* m_RadioBtnGroup;
    KThreadCameraConfig * m_Thread;

    QMap<int,GraphicsView*> m_ViewArrMain;

};

#endif // DLGCAMERACONFIG_H
