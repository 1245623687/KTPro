#ifndef DLGBRANDADD_H
#define DLGBRANDADD_H

#include <QDialog>
#include"dsdebug.h"
#include<QButtonGroup>
//#include"CheckOperator/CheckOperatorBasic.h"
//#include"CheckOperator/CheckOperatorControlBase.h"
//#include"CheckOperator/imgpro.h"
//#include"CheckOperator/imgtobaccocontrol.h"
namespace Ui {
class DlgBrandAdd;
}

class DlgBrandAdd : public QDialog
{
    Q_OBJECT


private:

public:
    explicit DlgBrandAdd(QString brandName,QWidget *parent = nullptr);
    ~DlgBrandAdd();

private slots:
    void buttonCameraJudge(int cameraNum);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonSave_clicked();

    void on_toolButtonClose_clicked();
    void buttonJudge(int idx);

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


    void on_pushButtonSetSize_clicked();

    void on_comboBoxCamNum_currentIndexChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgBrandAdd *ui;

private:
    int m_CameraNum;
    QButtonGroup * m_BtnGroup;
    QString LastBrandName;

    int ImgWids[8];
    int ImgHeis[8];
};

#endif // DLGBRANDADD_H
