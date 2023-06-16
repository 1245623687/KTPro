#ifndef DLGDOUBLESTAYINSPECT_H
#define DLGDOUBLESTAYINSPECT_H

#include <QDialog>
#include <QImage>
#include<QGraphicsScene>
#include<QStandardItemModel>
#include "controls/dsgraphicsitemimage.hpp"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include "controls/graphicsscene.h"
#include "controls/graphicsrectitem.h"
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgpro.h"
#include"CheckOperator/imgprocontrol.h"
#include"controls/graphicsscenecheckoperatormain.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
#include"ImageHandle/opencvimage.h"


namespace Ui {
class DlgDoubleStayInspect;
}

class DlgDoubleStayInspect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDoubleStayInspect(CheckOperatorDoubleStayInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);
    ~DlgDoubleStayInspect();

    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void initControl();
    void saveParam();
    void loadHVPos();

    void updateCheckRes();
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private slots:
    void on_btnMenu_Close_clicked();
    void on_pushBtnSaveReturn_clicked();
    void showKeyBoard(KControlsBase* pLineEdit);
    void on_tableViewH_doubleClicked(const QModelIndex &index);
    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void showCoordinate(QPointF pt);

    void on_pushBtnCancel_clicked();

    void on_spinBoxBinThreshold_valueChanged(int arg1);

    void on_spinBoxSetScore_valueChanged(int arg1);

    void on_comboBoxLineDir_currentIndexChanged(int index);

    void on_spinBoxAngleShift_valueChanged(int arg1);

    void on_spinBoxDisMin_valueChanged(int arg1);

    void on_spinBoxDisMax_valueChanged(int arg1);

    void on_pushBtnAutoCalc_clicked();

private:
    Ui::DlgDoubleStayInspect *ui;

private:

    int m_Mode;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    ImgPro* m_ImgPro;
    CheckOperatorDoubleStayInspect* m_CheckOperator;
    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    QGraphicsLineItem * m_LineItem1;
    QGraphicsLineItem * m_LineItem2;
    GraphicsSceneCheckOperatorMain* m_Scene;
    int m_Res;
};

#endif // DLGDOUBLESTAYINSPECT_H
