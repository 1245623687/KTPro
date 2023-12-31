﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include<QTimer>
#include<QStandardItemModel>
#include<QButtonGroup>
#include"controls/graphicsscenemain.h"
#include"controls/graphicsview.h"
#include"thread/kthreadproc.h"
#include<mutex>
#include<QScreen>
#include<QSettings>
#define CAMERA_NUM_MAX 8

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
private:


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void InitControl();

public slots:
    void updateStatistics(int cameraIdx);

    void updateSceneRect();

private slots:
    void updateCheckRect();

    void onTimeout();

    void buttonJudge(int idx);

    void updateRunLog();
    void on_pushButtonRun_clicked();



    void on_pushButton_clicked();

    void updateInfomation(int cameraIdx);



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
    void showEvent(QShowEvent *event) override;
private:

    void changeShowMode(int mode,int index);

private:
    Ui::MainWindow *ui;
private:
    int SHOWWID=696;
    int SHOWHEI=223;


    QTimer* m_Timer;

private:

    std::mutex m_MutexScene;
    QStandardItemModel* m_ModelLog;
    QButtonGroup* m_btnGroup;
    QMap<int,GraphicsView*> m_ViewArrSub;
    QMap<int,GraphicsView*> m_ViewArrMain;
    QMap<int,GraphicsSceneMain*> m_SceneArr;
    QMap<int,QWidget*> m_WidgetArr;
    int CAMERA_NUM_USER=8;
    KThreadProc* m_ThreadRun[CAMERA_NUM_MAX];

    double m_dScale;



};

#endif // MAINWINDOW_H
