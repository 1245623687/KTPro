#ifndef DLGASSOCIATEDINSPECT2_H
#define DLGASSOCIATEDINSPECT2_H

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
#include"ImageHandle/opencvimage.h"

#include<QTableView>
#include<QStyledItemDelegate>
#include<QPen>
#include<QApplication>
#include<QPainter>
#include<QMouseEvent>
#include<QEvent>
#include<QCheckBox>

namespace Ui {
class DlgAssociatedInspect2;
}


class QCheckBoxDelegate : public QStyledItemDelegate
{
public:
    QCheckBoxDelegate(QObject*);
        QMap<int,int> mapValue;
protected:
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) ;
private:
    QPen         pen;
    QTableView*  view;

};

class DlgAssociatedInspect2 : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAssociatedInspect2(CheckOperatorAssociatedInspect2* const pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x=0,int basis_y=0,int mode=0,QWidget *parent = nullptr);

    void initControl();
    void updateCheckRes();
    void saveParam();

    QString getCheckOperatorName(ENUMCHECKOPERATORTYPE t);
    void loadHVPos();

    QCheckBoxDelegate *checkboxDelegate;

    ~DlgAssociatedInspect2();

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
    void on_pushBtnAutoCalc_clicked();

    void on_pushBtnSaveReturn_clicked();

    void on_pushBtnCancel_clicked();

//    void on_tableViewH_doubleClicked(const QModelIndex &index);

//    void on_tableViewV_doubleClicked(const QModelIndex &index);

    void on_toolButtonMin_clicked();

    void on_toolButtonMax_clicked();

    void on_toolButtonInit_clicked();

    void on_spinBoxBinThresh_valueChanged(int arg1);

    void on_spinBoxMinArea_valueChanged(int arg1);

    void on_radioBtnIsBlackBlob_clicked();

    void on_radioBtnIsWhiteBlob_clicked();

    void showKeyBoard(KControlsBase* pLineEdit);

    void showPixelInfo(int x,int y);
    void on_radioBtnDefectAll_toggled(bool checked);

private:
    Ui::DlgAssociatedInspect2 *ui;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    DSGraphicsItemImage<OpencvImage> *m_ItemImageInit;
    GraphicsScene* m_Scene;
    GraphicsScene* m_SceneInit;
    CheckOperatorAssociatedInspect2* m_CheckOperator;
    ImgPro* m_ImgPro;
    OpencvImage thisImage;
    QStandardItemModel * m_ModelHPos;
    QStandardItemModel * m_ModelVPos;
    int m_Res;
    int m_Mode;

};

#endif // DLGDEFECTINSPECT2_H
