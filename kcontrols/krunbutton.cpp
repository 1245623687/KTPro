#include "krunbutton.h"
#include<QPainter>
KRunButton::KRunButton(QWidget *parent) : QWidget(parent)
{
    m_refSize = 200;
    m_radius = m_refSize /2;
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}



void KRunButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    float scale = qMin(width(),height());
    painter.scale(scale/m_refSize,scale/m_refSize);
    painter.translate(m_refSize/2,m_refSize/2); //设置坐标原点
    drawFrame(painter);
}

void KRunButton::drawFrame(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient lg1(-m_radius,-m_radius,m_radius,m_radius);
    lg1.setColorAt(0.1,Qt::white);
    lg1.setColorAt(1,Qt::black);
    painter.setBrush(lg1);
    painter.drawEllipse(-m_radius,-m_radius,m_refSize,m_refSize);

    painter.setBrush(QColor(66,66,66));
    painter.drawEllipse(QPoint(0,0),90,90);

    painter.restore();
    painter.drawText(QPoint(0,0),"开始");
}
