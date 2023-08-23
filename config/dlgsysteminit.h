#ifndef DLGSYSTEMINIT_H
#define DLGSYSTEMINIT_H

#include <QDialog>
#include"params/dsoptions.h"
#include"qtCommon/iconhelper.h"
#include"dsdebug.h"
#include<QMouseEvent>
#include<QDesktopWidget>
namespace Ui {
class DlgSystemInit;
}

class DlgSystemInit : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSystemInit(ENUMCAMERATYPE* cameraType,int* cameraNum, ENUMIOTYPE* IOType,ENUMPROBNUM *ProbNum,QWidget *parent = nullptr);
    ~DlgSystemInit();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void on_toolButton_clicked();
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:

    void on_btnMenu_Close_clicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgSystemInit *ui;
   ENUMCAMERATYPE* m_CameraType;
   int* m_CameraNum;
   ENUMIOTYPE* m_IOType;

   ENUMPROBNUM *m_ProNum;



};

#endif // DLGSYSTEMINIT_H
