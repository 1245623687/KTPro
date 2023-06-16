#ifndef DLGMATCHTEMPLATE_H
#define DLGMATCHTEMPLATE_H

#include <QDialog>
#include <QImage>
#include<QMap>

#include<QGraphicsScene>
#include "Controls/dsgraphicsitemimage.hpp"
#include "params/dssystemparam.h"
//#include "CheckOperator/ImgProDel.hpp"
#include "opencv2/opencv.hpp"
#include"controls/graphicscheckoperatoritem.h"
#include"controls/graphicsscenecheckoperatormain.h"
#include"common/filehelper.h"
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/imgtobacco.h"
#include"CheckOperator/CheckOperator.hpp"
#include"CheckOperator/CheckOperatorControl.hpp"
#include"CheckOperator/imgtobaccocontrol.h"
#include"kcontrols/kspinboxkb.h"
#include"kcontrols/klineeditkb.h"
namespace Ui {
class DlgMatchTemplate;
}

class DlgMatchTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMatchTemplate(int cameraNo,int checkOperatorNo,CheckOperatorTemplateMatch* pCheckOperator,ImgTobacco* Tabacco,OpencvImage & image,int mode,QWidget *parent= nullptr);
    ~DlgMatchTemplate();


    void initControl();
    void updateCheckRes();
    void loadTemplate();

    void saveParam();



private slots:
    void updateCheckoperatorOption();

    void on_pushButtonAdd_clicked();

    void on_pushButtonDel_clicked();

    void on_pushBtnCancel_clicked();

    void on_pushBtnAutoCalc_clicked();

    void on_pushBtnSaveReturn_clicked();

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
    Ui::DlgMatchTemplate *ui;
    CheckOperatorTemplateMatch* m_CheckOperator;
    DSGraphicsItemImage<OpencvImage> *m_ItemImage;
    GraphicsSceneCheckOperatorMain * m_SceneSearch;
    GraphicsSceneCheckOperatorMain * m_SceneTemplateArr[4];

    OpencvImage m_FullImage;


    int m_TemplateNum;

    int m_Mode;
    OpencvImage m_TemplateImgArr[4];
    int m_CameraNo;
    int m_CheckOperatorNo;
    ImgTobacco* m_Tabacco;

    int m_Res;
};

#endif // DLGMATCHTEMPLATE_H
