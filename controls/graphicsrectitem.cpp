#include "graphicsrectitem.h"

GraphicsRectItem::GraphicsRectItem(QRect rect,QGraphicsItem* parent):
    m_Rect(rect),
    QGraphicsRectItem(parent)
{
//    this->setPos(rect.x(),rect.y());
}


