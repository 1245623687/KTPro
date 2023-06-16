#include "klabel.h"
#include<QPaintEvent>
#include<QPainter>
#include<QtMath>
KLabel::KLabel(QWidget* parent):QWidget(parent)
{

}
/*  point: 文字所在的点
 *  from_angle : 文字所在的度数
 *  rotate : 需要旋转的角度,值为-360~360(为负数表示逆时针旋转,为正数表示顺时针旋转)
 */
QPoint KLabel::CustomRotate(QPointF point,qreal from_angle,qreal rotate)
{
    qreal PI=3.141592653589;
    QPointF Tmp;
    qreal arc = (rotate-from_angle)/180*PI;
    qreal Length = qSqrt(point.x()*point.x() +point.y()*point.y());
    Tmp.setX(Length*qCos(arc));
    Tmp.setY(Length*qSin(arc));
    return Tmp.toPoint();
}
void KLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.save();
    painter.rotate(50);
    painter.setFont(QFont("Microsoft YaHei"));
    painter.setPen(QColor(255,255,255));
    painter.drawText(25,18,m_Info);

    if(m_IsNg)
    {
        painter.setBrush(QBrush(QColor(255,0,0)));
        painter.setPen(QColor(255,0,0));
    }
    else
    {
    painter.setBrush(QBrush(QColor(0,255,0)));
    painter.setPen(QColor(0,255,0));
    }
    painter.drawRect(14,0,10,10);

    painter.restore();
}
