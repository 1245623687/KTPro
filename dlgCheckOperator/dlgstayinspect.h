#ifndef DLGSTAYINSPECT_H
#define DLGSTAYINSPECT_H

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
class DlgStayInspect;
}

class DlgStayInspect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgStayInspect(CheckOperatorStayInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);
    ~DlgStayInspect();


public:
    void initControl();
    void updateCheckRes();
    void saveParam();

    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void loadHVPos();
    void updateRecommentThresh();

private slots:

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnAutoCalc_clicked();

    void on_tableViewH_doubleClicked(const QModelIndex &index);

    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void on_comboBoxLineDir_currentIndexChanged(int index);

    void on_pushBtnCancel_clicked();


    void showKeyBoard(KControlsBase* pLineEdit);


private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Close_clicked();

    void on_spinBoxBinThreshold_valueChanged(int arg1);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;
private:
    Ui::DlgStayInspect *ui;

    int m_Mode;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    GraphicsSceneCheckOperatorMain* m_Scene;
    CheckOperatorStayInspect* m_CheckOperator;
    ImgPro* m_ImgPro;

    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;

    QGraphicsLineItem * m_LineItem;
    int m_Res;
};


#endif // DLGSTAYINSPECT_H
