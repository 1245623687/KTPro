#ifndef GRAPHICSITEMBASE_H
#define GRAPHICSITEMBASE_H

#include<QGraphicsItem>
#include"sizehandlerect.h"
#include"controlscommon.h"


class GraphicsItemBase : public QGraphicsItem
{
private:

public:
    GraphicsItemBase(QGraphicsItem* parent);

    virtual QRectF boundingRect() const=0;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)=0;

    virtual void updateRect()=0;

    virtual bool selected(){return  m_Selected;}

    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point )=0;
    virtual SizeHandleRect::Direction  hitTest( const QPointF & point ) const=0;
    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir )=0;
    virtual void setSelectState(bool b)=0;


    int MaxWidth(){return 1248;}
    int MaxHeight(){return  384;}

    virtual ~GraphicsItemBase()
    {

    }

protected:

   bool m_Selected=false;

};

#endif // GRAPHICSITEMBASE_H
