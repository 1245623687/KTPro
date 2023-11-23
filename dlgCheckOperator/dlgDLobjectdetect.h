#ifndef DLGDLOBJECTDETECT_H
#define DLGDLOBJECTDETECT_H

#include <QDialog>
#include <QImage>
#include<QGraphicsScene>
#include<QStandardItemModel>
#include "controls/dsgraphicsitemimage.hpp"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include"CheckOperator/imgpro.h"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgprocontrol.h"
#include"controls/graphicsscene.h"
//#include"kcontrols/kdspinboxkb.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
#include"dlgbase.h"
#

namespace Ui {
class DlgDLObjectDetect;
}

class DlgDLObjectDetect :public QDialog
{
    Q_OBJECT

public:
    explicit DlgDLObjectDetect(CheckOperatorDLObjectDetect* const pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);
    void initControl();
    void updateCheckRes();
    void saveParam();

    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void loadHVPos();
    ~DlgDLObjectDetect();

private slots:
    void on_pushBtnAutoCalc_clicked();

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

    void on_tableViewH_doubleClicked(const QModelIndex &index);

    void showKeyBoard(KControlsBase* pLineEdit);

    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void on_toolButtonMin_clicked();

    void on_toolButtonMax_clicked();

    void on_toolButtonInit_clicked();

    void on_spinBoxMinArea_valueChanged(int arg1);

    void on_radioBtnIsWhiteBlob_clicked();

    void on_radioBtnIsBlackBlob_clicked();
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Close_clicked();

     void showPixelInfo(int x,int y);


     void on_spinBoxConfidence_valueChanged(int arg1);

     void on_horizontalSliderConfidence_valueChanged(int value);

     void on_spinBoxGrayVal_valueChanged(int arg1);

     void on_horizontalSliderGrayVal_valueChanged(int value);

     void on_spinBoxKtMinArea_valueChanged(int arg1);

     void on_horizontalSliderKtMinArea_valueChanged(int value);

     void on_spinBoxGrayValUpLimit_valueChanged(int arg1);

     void on_horizontalspinBoxGrayValUpLimit_valueChanged(int value);


     void on_spinBoxCigaTotalNum_valueChanged(int arg1);

     void on_horizontalSliderCigaTotalNum_valueChanged(int value);




     void on_radioButton_accuracyN_toggled(bool checked);


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgDLObjectDetect *ui;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    DSGraphicsItemImage<OpencvImage> *m_ItemImageInit;
    GraphicsScene* m_Scene;
    GraphicsScene* m_SceneInit;
    CheckOperatorDLObjectDetect* m_CheckOperator;
    ImgPro* m_ImgPro;
    OpencvImage thisImage;
    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    int m_Res;
    int m_Mode;
    cv::Rect m_curRect;
};

#endif // DLGDEFECTINSPECT_H
