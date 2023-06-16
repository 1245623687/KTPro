#ifndef KSPEEDPANEL_H
#define KSPEEDPANEL_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QLinearGradient>
#include <qmath.h>
#include <QTimer>
#include <QDebug>
#include <QLCDNumber>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
class KSpeedPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KSpeedPanel(QWidget *parent = nullptr);

    ~KSpeedPanel(){}
    void setCurentSpeed(int speed);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    //void slot_setSpeed();

    void slot_updateSpeed();
private:
    void drawFrame(QPainter& painter);//绘制边框与背景
    void drawNumberIndicator(QPainter& painter);//绘制指示数字
    void drawDividing(QPainter& painter);//绘制刻度
    void drawNumberSpeed(QPainter &painter);//显示数字速度
    void drawIndicator(QPainter& painter);//绘制速度指针

private:
    int m_startAngle;
    int m_endAngle;

    int m_refSize; //绘制的参考大小
    int m_radius;

    int m_scale=1;

    int m_minSpeed;
    int m_maxSpeed;
    int m_curSpeed;
    double m_anglePerVel;//每KM 对应的 角度
    //QLCDNumber *m_lcd_Mileage;//显示总里程
    QLabel *m_lbl_Mileage;
    QHBoxLayout* m_layout_mileage;
    QWidget* m_wgt_mileage;

//    QTimer* m_TimerUpdate;
};

#endif // KSPEEDPANEL_H
