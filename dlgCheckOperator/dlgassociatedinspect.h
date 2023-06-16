#ifndef DLGASSOCIATEDINSPECT_H
#define DLGASSOCIATEDINSPECT_H

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
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
namespace Ui {
class DlgAssociatedInspect;
}

class DlgAssociatedInspect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAssociatedInspect(CheckOperatorAssociatedInspect* const pCheckOperator,ImgPro* imgPro,int mode,QWidget *parent = nullptr);

    void  loadHVPos();
    void initControl();
    void loadAss();
    void updateCheckRes();
    void saveParam();

    ~DlgAssociatedInspect();

public:
    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);

private slots:
    void on_tableViewH_doubleClicked(const QModelIndex &index);

    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void on_tableViewAss_doubleClicked(const QModelIndex &index);

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

    void on_spinBoxMaxShift_valueChanged(int arg1);

    void on_pushButton_clicked();

    void showKeyBoard(KControlsBase* pLineEdit);

private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;


private slots:
    void on_btnMenu_Close_clicked();

    void on_comboBox_currentIndexChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private:
    Ui::DlgAssociatedInspect *ui;
    int m_Mode;
    CheckOperatorAssociatedInspect* m_CheckOperator;
    ImgPro* m_ImgPro;
    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    QStandardItemModel* m_Model;
    int m_CalcRes;
};

#endif // DLGASSOCIATEDINSPECT_H
