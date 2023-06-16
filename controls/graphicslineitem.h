#ifndef GRAPHICSLINE_H
#define GRAPHICSLINE_H
#include<QGraphicsLineItem>
#include<QGraphicsRectItem>
#include<QLine>
#include "sizehandlerect.h"
class GraphicsLineItem:public QGraphicsLineItem
{
public:
    GraphicsLineItem(const QLine & line1 ,const QLine & line2 ,QGraphicsLineItem * parent);
    GraphicsLineItem(int x11,int y11,int x12,int y12 ,int x21,int y21,int x22,int y22 ,QGraphicsLineItem * parent);

    void updateGeometry();


    SizeHandleRect::Direction  hitTest( const QPointF & point );
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setState(SizeHandleRect::SelectionHandleState st);
    void setPoint11Pos(QPoint pt);
    void setPoint12Pos(QPoint pt);
    void setPoint21Pos(QPoint pt);
    void setPoint22Pos(QPoint pt);
    ~GraphicsLineItem()
    {
        delete  m_SizeHandle[0];
        delete  m_SizeHandle[1];
        delete  m_SizeHandle[2];
        delete  m_SizeHandle[3];

    }


private:

    SizeHandleRect* m_SizeHandle[4];
    QLine m_line[2];
    int m_Width=10;
    SizeHandleRect::SelectionHandleState m_state;

};

#endif // GRAPHICSLINE_H
