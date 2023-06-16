#include "kpercentage.h"
#include <QPainter>
#include<QLCDNumber>
#include<QFont>

KPercentage::KPercentage(QWidget *parent) : QWidget(parent)
  , m_angle(0)
{

    m_LcdPer=new QLCDNumber(this);
    m_LcdPer->setDigitCount(7);
    m_LcdPer->setSegmentStyle(QLCDNumber::Flat);    // 设置显示外观
    m_LcdPer->setStyleSheet("color: rgb(67,241,235); background: transparent;");


    this->setStyleSheet("background:transparent;");
    m_timeLine = new QTimeLine(1, this);
    m_timeLine->setCurveShape(QTimeLine::EaseOutCurve);
    connect(m_timeLine, &QTimeLine::frameChanged, [=](int index) {
        m_angle = index;
        update();
    });
}

void KPercentage::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //drawBackground(painter);
    painter.translate(width()/2, height()/2);
    drawOutLine(painter);
    drawBottomCircle(painter);
    drawTopCircle(painter);
    drawPercentage(painter);
}

void KPercentage::drawBackground(QPainter& painter)
{
    painter.save();
    painter.fillRect(0,0,width(),height(),m_backgroundColor);
//    painter.setBrush(QBrush(QColor(255,0,0)));
//    painter.drawEllipse(0,0,width()-20,height()-20);
    painter.restore();
}

void KPercentage::drawPercentage(QPainter& painter)
{

    m_LcdPer->setGeometry(width()/2,height()/2,width()*7/11,width()*2/11);
    m_LcdPer->setGeometry((width()-m_LcdPer->geometry().width())/2,(height()-m_LcdPer->geometry().height())/2,m_LcdPer->width(),m_LcdPer->height());


//    painter.save();


//    QPen pen(QColor(67,241,235));
//    pen.setWidth(3);
//    painter.setPen(pen);
//    painter.setFont(QFont("黑体",18,QFont::Bold));
//    QString per = QString("%1").arg(QString::number(m_CurPer, 'f', 2));
//    QFontMetricsF fm(this->font());
//    qreal w = fm.size(Qt::TextSingleLine,per).width();
//    painter.drawText(-w/2-width()/10,0,per);
//    painter.restore();
}


void KPercentage::drawOutLine(QPainter& painter)
{
    painter.save();
    QPen pen;
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);
    painter.rotate(-180);
    int d = width() > height()? height():width();
    int radius = d/2;
    int outEdgePos = radius - radius*1/20;
    QPoint start(0, outEdgePos);
    int lineWidth = radius*2/20;
    QPoint end(0, outEdgePos - lineWidth);
    int tickCount = 40;
    for(int i = 0; i < tickCount; ++i)
    {
        painter.save();
        int angle = 360 * i /40;
        painter.rotate(angle);
        if(angle < m_angle)
            pen.setColor(m_outLineAddColor);
        else
            pen.setColor(m_outLineSubColor);
        painter.setPen(pen);
        painter.drawLine(start, end);
        painter.restore();
    }
    painter.restore();
}

void KPercentage::drawBottomCircle(QPainter& painter)
{
    painter.save();
    QPen pen;
    pen.setColor(m_CircleSubColor);
    pen.setWidth(10);
    painter.setBrush(QBrush(QColor(150,150,150)));
    painter.setPen(pen);
    int d = width() > height()? height():width();
    int radius = d/2;
    int circleRadius = radius*7/10;
    painter.drawEllipse(-circleRadius, -circleRadius, circleRadius*2,circleRadius*2);
    painter.restore();
}

void KPercentage::drawTopCircle(QPainter& painter)
{
    painter.save();
    QPen pen;
    pen.setColor(m_CircleAddColor);
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.rotate(-90);
    int d = width() > height()? height():width();
    int radius = d/2;
    int circleRadius = radius*7/10;
    painter.drawArc(-circleRadius, -circleRadius, circleRadius*2,circleRadius*2, 0, -m_angle*16);
    painter.restore();
}

void KPercentage::setAngle(int angle)
{
    m_timeLine->setFrameRange(m_angle, angle);
    m_timeLine->start();
    update();
}

void KPercentage::setPercentage(double val)
{
    int angle=val*360;
    this->m_CurPer=val;
    this->m_LcdPer->display(QString::number(m_CurPer,'f',3));
    setAngle(angle);
}

void KPercentage::setOutLineSubColor(QColor color)
{
    m_outLineSubColor = color;
}

void KPercentage::setOutLineAddColor(QColor color)
{
    m_outLineAddColor = color;
}

void KPercentage::setCircleSubColor(QColor color)
{
    m_CircleSubColor = color;
}

void KPercentage::setCircleAddColor(QColor color)
{
    m_CircleAddColor = color;
}

void KPercentage::setBackgroundColor(QColor color)
{
    m_backgroundColor = color;
}
