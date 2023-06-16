#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H
#include<QGraphicsItem>
#include"controlscommon.h"
class GraphicsTextItem : public QGraphicsTextItem
{
public:
    GraphicsTextItem(QString str,QGraphicsItem*  parent=nullptr);

    int type() const override{ return GRAPHICSITEMTYPE_TEXT;}

private:
    QString m_Str;
};

#endif // GRAPHICSTEXTITEM_H
