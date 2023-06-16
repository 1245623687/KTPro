#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>
#include<QStandardItemModel>
#include"mainrecord.h"
namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();
public:
    void initModel();
    void init();
    void loadPage();
    void updateDefectView(QString mainRecordId);
private:
    void InitStyle();
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
private slots:
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Close_clicked();protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *)override;


private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_btnPre_clicked();

    void on_btnNext_clicked();

    void on_toolButtonCoSet_clicked();

private:
    Ui::RecordDialog *ui;

    QStandardItemModel* modelMain;
    QStandardItemModel* modelDefect;
    QStandardItemModel* modelDetail;

    std::vector<MainRecord>  m_VRec;


    int m_PageIdx=0;
    int m_PageRowNum=16;

};

#endif // RECORDDIALOG_H
