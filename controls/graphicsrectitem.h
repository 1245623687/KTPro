#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include<QGraphicsRectItem>
#include"controlscommon.h"
#include<QPainter>
class GraphicsRectItem : public QGraphicsRectItem
{
public:

public:
    GraphicsRectItem(QRect rect,QGraphicsItem* parent);

    int type() const override
    {
        return GRAPHICSITEMTYPE_RECT;
    }

protected:
    QRectF GraphicsRectItem::boundingRect() const
    {
        return QRectF(0,0,m_Rect.width(),m_Rect.height());
    }
    void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(0,0,m_Rect.width(),m_Rect.height());
    }




private:
    QRect m_Rect;
};

#endif // GRAPHICSRECTITEM_H
