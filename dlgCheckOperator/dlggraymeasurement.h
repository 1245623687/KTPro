#ifndef DLGGRAYMEASUREMENT_H
#define DLGGRAYMEASUREMENT_H

#include <QDialog>

#include <QImage>
#include<QGraphicsScene>
#include<QStandardItemModel>
#include "Controls/dsgraphicsitemimage.hpp"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgpro.h"
#include"CheckOperator/imgprocontrol.h"
#include"controls/graphicsscenecheckoperatormain.h"
#include"kcontrols/klineeditkb.h"
#include"kcontrols/kspinboxkb.h"

namespace Ui {
class DlgGrayMeasurement;
}

class DlgGrayMeasurement : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGrayMeasurement(CheckOperatorGrayMeasurement* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);
    void initControl();
    void updateCheckRes();
     void saveParam();

    void  loadHVPos();
    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    ~DlgGrayMeasurement();


private slots:
    void on_pushBtnAutoCalc_clicked();

    void on_spinBoxMeanBasis_valueChanged(int arg1);

    void on_spinBoxStddevBasis_valueChanged(int arg1);

    void on_lineEditSetStddev_textChanged(const QString &arg1);

    void on_lineEditSetMean_textChanged(const QString &arg1);

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

    void on_tableViewH_doubleClicked(const QModelIndex &index);

    void on_tableViewV_doubleClicked(const QModelIndex &index);
 void showKeyBoard(KControlsBase* pLineEdit);
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
    Ui::DlgGrayMeasurement *ui;

    int m_Mode;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    GraphicsSceneCheckOperatorMain* m_Scene;
    CheckOperatorGrayMeasurement* m_CheckOperator;
    ImgPro* m_ImgPro;

    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;

    int m_Res;
};


#endif // DLGGRAYMEASUREMENT_H
