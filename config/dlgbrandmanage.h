#ifndef DLGCREATEBRAND_H
#define DLGCREATEBRAND_H

//#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//# pragma execution_character_set("utf-8")
//#endif

#include <QDialog>
#include<QVector>
#include<QStandardItemModel>
#include"params/dssystemparam.h"
#include"common/filehelper.h"
#include"controls/graphicsscene.h"
#include<QButtonGroup>
namespace Ui {
class DlgBrandManage;
}

class DlgBrandManage : public QDialog
{
    Q_OBJECT
private:
    typedef struct StuBrand
    {
    public:
        int No;
        QString BrandName;
        int CameraNo;
    }StuBrand;


public:
    explicit DlgBrandManage(QWidget *parent = nullptr);
    ~DlgBrandManage();

private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

private slots:
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;

private slots:
    void on_pushButtonAddBrand_clicked();

    void on_pushButtonDelBrand_clicked();

    void on_pushButtonChange_clicked();

    void on_pushButtonCancel_clicked();

    void on_tableViewBrand_clicked(const QModelIndex &index);

    void on_toolButtonClose_clicked();

    void changeCameraNo(int idx)
    {
        m_ShowCameraNo=idx;
        showImg(m_CurRow);

    }

private:
    void loadTableModel();

    void showImg(int row);


private:
    Ui::DlgBrandManage *ui;
    QStandardItemModel * m_Model;
    GraphicsScene * m_Scene;
    QButtonGroup * m_GroupButton;
    int m_ShowCameraNo=1;
    int m_CurRow=0;


};

#endif // DLGCREATEBRAND_H
