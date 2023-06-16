#include "graphicslineitem.h"
#include<QPainter>
GraphicsLineItem::GraphicsLineItem(const QLine & line1 ,const QLine & line2 ,QGraphicsLineItem * parent):
    QGraphicsLineItem(parent)
{
    m_line[0]=line1;
    m_line[1]=line2;
    m_SizeHandle[0]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT11);
    m_SizeHandle[1]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT12);
    m_SizeHandle[2]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT21);
    m_SizeHandle[3]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT22);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    updateGeometry();
    show();
    setState(SizeHandleRect::SelectionHandleActive);
}

void GraphicsLineItem::setState(SizeHandleRect::SelectionHandleState st)
{
    if (st == m_state)
        return;
    switch (st) {
    case SizeHandleRect::SelectionHandleOff:
        hide();
        for(int i=0;i<4;i++)
        {
            m_SizeHandle[i]->setState(st);
        }

        break;
    case SizeHandleRect::SelectionHandleInactive:
    case SizeHandleRect::SelectionHandleActive:
        show();
        for(int i=0;i<4;i++)
        {
            m_SizeHandle[i]->setState(st);
        }
        break;
    }
    m_state = st;
}

SizeHandleRect::Direction  GraphicsLineItem::hitTest( const QPointF & point )
{
    for(int i=0;i<4;i++)
    {
        if(this->m_SizeHandle[i]->hitTest(point))
        {
            return this->m_SizeHandle[i]->dir();
        }
    }
    return SizeHandleRect::None;
}



GraphicsLineItem::GraphicsLineItem(int x11,int y11,int x12,int y12 ,int x21,int y21,int x22,int y22 ,QGraphicsLineItem * parent):
    QGraphicsLineItem(parent)
{
    m_line[0].setP1(QPoint(x11,y11));
    m_line[0].setP2(QPoint(x12,y12));
    m_line[1].setP1(QPoint(x21,y21));
    m_line[1].setP2(QPoint(x22,y22));
    m_SizeHandle[0]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT11 );
    m_SizeHandle[1]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT12);
    m_SizeHandle[2]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT21 );
    m_SizeHandle[3]=new  SizeHandleRect(this, QRectF(0,0,this->m_Width,this->m_Width),SizeHandleRect::PT22);
    updateGeometry();
    show();
    setState(SizeHandleRect::SelectionHandleActive);

}
void GraphicsLineItem::updateGeometry()
{
    this->m_SizeHandle[0]->setPos(m_line[0].p1().x()-this->m_Width/2,m_line[0].p1().y()-this->m_Width/2);
    this->m_SizeHandle[1]->setPos(m_line[0].p2().x()-this->m_Width/2,m_line[0].p2().y()-this->m_Width/2);
    this->m_SizeHandle[2]->setPos(m_line[1].p1().x()-this->m_Width/2,m_line[1].p1().y()-this->m_Width/2);
    this->m_SizeHandle[3]->setPos(m_line[1].p2().x()-this->m_Width/2,m_line[1].p2().y()-this->m_Width/2);
}

void GraphicsLineItem::setPoint11Pos(QPoint pt)
{
    this->m_line[0].setP1(pt);
    // this->m_SizeHandle[0]->setPos(QPoint(pt.x()-this->m_Width / 2,pt.y()-this->m_Width  / 2));
    updateGeometry();
}
void GraphicsLineItem::setPoint12Pos(QPoint pt)
{
    this->m_line[0].setP2(pt);
    // this->m_SizeHandle[1]->setPos(QPoint(pt.x()-this->m_Width / 2,pt.y()-this->m_Width  / 2));
    updateGeometry();
}

void GraphicsLineItem::setPoint21Pos(QPoint pt)
{
    this->m_line[1].setP1(pt);
    //   this->m_SizeHandle[2]->setPos(QPoint(pt.x()-this->m_Width / 2,pt.y()-this->m_Width  / 2));
    updateGeometry();
}

void GraphicsLineItem::setPoint22Pos(QPoint pt)
{
    this->m_line[1].setP2(pt);
    //  this->m_SizeHandle[3]->setPos(QPoint(pt.x()-this->m_Width / 2,pt.y()-this->m_Width  / 2));
    updateGeometry();
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(Qt::yellow);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(this->m_line[0]);
    painter->drawLine(this->m_line[1]);
}
