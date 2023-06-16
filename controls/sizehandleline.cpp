#include "sizehandleline.h"
#include<QPainter>


SizeHandleLine::SizeHandleLine(QGraphicsItem* parent , QRectF rect,int index):
    QGraphicsRectItem(rect, parent),
    m_state(ENUMSIZEHANDLELINESTATE_OFF)
{
    setParentItem(parent);
    hide();
    show();
}

bool SizeHandleLine::hitTest( const QPointF & point )
{
    QPointF pt = mapFromScene(point);
    return rect().contains(pt);
}

void SizeHandleLine::move(qreal x, qreal y )
{
     setPos(x,y);
}


void SizeHandleLine::setState(ENUMSIZEHANDLELINESTATE st)
{
    if (st == m_state)
        return;
    switch (st) {
    case ENUMSIZEHANDLELINESTATE_OFF:
        hide();
        break;
    case ENUMSIZEHANDLELINESTATE_INACTIVE:
    case ENUMSIZEHANDLELINESTATE_ACTIVE:
        show();
        break;
    }
    m_state = st;
}


void SizeHandleLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c = QColor("yellow");
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(c));
    painter->drawRect(rect());
}
