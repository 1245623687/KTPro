#include "linehandlerect.h"
#include<QPainter>

LineHandleRect::LineHandleRect(QRectF rect,Direction dir,QGraphicsItem* parent):
    m_dir(dir),
    QGraphicsRectItem(rect, parent)
{
    setParentItem(parent);
}

bool LineHandleRect::hitTest( const QPointF & point )
{
    QPointF pt = mapFromScene(point);
    return rect().contains(pt);
}

void LineHandleRect::move(qreal x, qreal y )
{
    setPos(x,y);

}
void LineHandleRect::setState(bool visual)
{
    if(visual) this->show();
    else this->hide();
}

void LineHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c = QColor("limegreen");
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(c));
    painter->drawRect(rect());
}
