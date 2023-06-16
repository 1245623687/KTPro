#ifndef DLGEDGEPOSITIONING_H
#define DLGEDGEPOSITIONING_H

#include <QDialog>
#include <QImage>
#include<QGraphicsScene>
#include "Controls/dsgraphicsitemimage.hpp"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include<QGraphicsLineItem>
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgpro.h"
#include"controls/graphicsscenecheckoperatormain.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
namespace Ui {
class DlgEdgePositioning;
}

class DlgEdgePositioning : public QDialog
{
    Q_OBJECT

public:
    explicit DlgEdgePositioning(CheckOperatorEdgePositioning* pCheckOperator,ImgPro* imgPro,OpencvImage&  image,int mode,QWidget *parent = nullptr);

     void initControl();
    void updateCheckRes();


    void updateRecommentThresh();
    ~DlgEdgePositioning();

    void saveParam();
private slots:
    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

    void on_pushBtnAutoCalc_clicked();

    void on_comboBoxDir_currentIndexChanged(int index);

    void on_spinBoxThresh_valueChanged(int arg1);
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Close_clicked();
 void showKeyBoard(KControlsBase* pLineEdit);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgEdgePositioning *ui;

    int m_Mode;

    CheckOperatorEdgePositioning * m_CheckOperator;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    GraphicsSceneCheckOperatorMain* m_Scene;
    QGraphicsLineItem * m_LineItem;
    ImgPro * m_ImgPro;
    int m_Res=-1;

};

#endif // DLGEDGEPOSITIONING_H
