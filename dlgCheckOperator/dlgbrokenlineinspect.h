#ifndef DLGBROKENLINEINSPECT_H
#define DLGBROKENLINEINSPECT_H

#include <QDialog>
#include <QImage>
#include"controls/graphicsscene.h"
#include<QStandardItemModel>

#include "controls/dsgraphicsitemimage.hpp"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include "controls/graphicsitemscene.h"
#include "controls/graphicsrectitem.h"
#include"controls/graphicssinglelineitem.h"

#include"CheckOperator/imgpro.h"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgprocontrol.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
namespace Ui {
class DlgBrokenLineInspect;
}

class DlgBrokenLineInspect : public QDialog
{
    Q_OBJECT

public:
   explicit DlgBrokenLineInspect(CheckOperatorBrokenlineInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);
    ~DlgBrokenLineInspect();
    void initControl();
    void updateCheckRes();
    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void loadHVPos();
    void saveParam();
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Close_clicked();

    void on_tableViewH_doubleClicked(const QModelIndex &index);

    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void on_pushBtnSaveReturn_clicked();



    void on_pushBtnAutoCalc_clicked();

    void on_pushBtnCancel_clicked();

    void on_spinBoxSetScore_valueChanged(int arg1);

    void on_spinBoxAngleShift_valueChanged(int arg1);

    void on_spinBoxLengthThresh_valueChanged(int arg1);

    void on_spinBoxBinThreshold_valueChanged(int arg1);

 void showKeyBoard(KControlsBase* pLineEdit);


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;
private:
    Ui::DlgBrokenLineInspect *ui;
    int m_Mode;


    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    GraphicsItemScene * m_Scene;
    CheckOperatorBrokenlineInspect* m_CheckOperator;
    ImgPro* m_ImgPro;


    QGraphicsLineItem * m_LineRes;
    GraphicsSingleLineItem * m_LineItem;
    GraphicsSingleLineItem * m_LineItem2;

    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    int m_Res;
};

#endif // DLGBROKENLINEINSPECT_H
