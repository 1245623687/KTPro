#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include<QGraphicsPixmapItem>
#include"controlscommon.h"
class GraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    GraphicsPixmapItem(QGraphicsItem* parent);

    int type() const override
    {
        return GRAPHICSITEMTYPE_BACKGROUNDIMAGE;
    }

};

#endif // GRAPHICSPIXMAPITEM_H
