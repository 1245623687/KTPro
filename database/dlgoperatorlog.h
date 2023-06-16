#ifndef DLGOPERATORLOG_H
#define DLGOPERATORLOG_H

#include <QDialog>
#include<QStandardItemModel>

namespace Ui {
class DlgOperatorLog;
}

class DlgOperatorLog : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOperatorLog(QWidget *parent = nullptr);
    ~DlgOperatorLog();
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
    Ui::DlgOperatorLog *ui;

private:
    void InitControl();

private:
    QStandardItemModel* m_Model;
};

#endif // DLGOPERATORLOG_H
