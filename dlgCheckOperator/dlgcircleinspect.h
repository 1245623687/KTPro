#ifndef DLGCIRCLEINSPECT_H
#define DLGCIRCLEINSPECT_H

#include <QDialog>
#include <QImage>
#include<QGraphicsScene>
#include<QStandardItemModel>
#include "controls/dsgraphicsitemimage.hpp"
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/imgpro.h"
#include "opencv2/opencv.hpp"
#include"CheckOperator/imgpro.h"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgprocontrol.h"
#include"controls/graphicsscene.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
#include"dlgbase.h"
namespace Ui {
class DlgCircleInspect;
}

class DlgCircleInspect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCircleInspect(CheckOperatorCircleInspect* const pCheckOperator,ImgPro* imgPro,OpencvImage & image,int mode=0,int basis_x=0,int basis_y=0,QWidget *parent = nullptr);
    ~DlgCircleInspect();
    void initControl();
    void updateCheckRes();
     QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
     void loadHVPos();
     void saveParams();
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

private slots:

    void showKeyBoard(KControlsBase* pLineEdit);
    void on_toolButtonMin_clicked();

    void on_toolButtonMax_clicked();

    void on_toolButtonInit_clicked();

    void on_spinBoxBinThresh_valueChanged(int arg1);



    void on_horizontalSliderspinBoxPrecOut_valueChanged(int value);
    void on_horizontalSliderspinBoxPrecIn_valueChanged(int value);
    void on_spinBoxPrecIn_valueChanged(int arg1);
    void on_spinBoxPrecOut_valueChanged(int arg1);


    void on_spinBoxPrecIn_2_valueChanged(int arg1);
    void on_horizontalSliderspinBoxPrecIn_2_valueChanged(int value);
    void on_spinBoxPrecOut_2_valueChanged(int arg1);
    void on_horizontalSliderspinBoxPrecOut_2_valueChanged(int value);



    void on_horizontalSliderPrecCircle_valueChanged(int value);
    void on_spinBoxPrecCircle_valueChanged(int arg1);



    void on_spinBoxBinThreshOutLow_valueChanged(int arg1);

    void on_spinBoxBinThreshOutHigh_valueChanged(int arg1);

    void on_spinBoxBinThreshInLow_valueChanged(int arg1);

    void on_spinBoxBinThreshInHigh_valueChanged(int arg1);

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

    void showPixelInfo(int x,int y);





    void on_spinBoxPrecOut_4_valueChanged(int arg1);

    void on_horizontalSliderspinBoxPrecOut_4_valueChanged(int value);

//    void on_spinBoxPrecIn_3_valueChanged(int arg1);

//    void on_spinBoxPrecIn_4_valueChanged(int arg1);

//    void on_horizontalSliderspinBoxPrecIn_3_valueChanged(int value);

//    void on_horizontalSliderspinBoxPrecIn_4_valueChanged(int value);

    void on_spinBoxquezhiArea_valueChanged(int arg1);

    void on_horizontalSliderspinBoxquezhiArea_valueChanged(int value);

    void on_spinBoxquezhiGrayVal_valueChanged(int arg1);

    void on_horizontalSliderspinBoxquezhiGrayVal_valueChanged(int value);

private:
    Ui::DlgCircleInspect *ui;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    DSGraphicsItemImage<OpencvImage> *m_ItemImageInit;
    GraphicsScene* m_Scene;
    GraphicsScene* m_SceneInit;
    CheckOperatorCircleInspect* m_CheckOperator;
    ImgPro* m_ImgPro;
    OpencvImage thisImage;
    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    int m_Res;
    int m_Mode;

};

#endif // DLGCIRCLEINSPECT_H
