#ifndef GRAPHICSSINGLELINEITEM_H
#define GRAPHICSSINGLELINEITEM_H
#include<QGraphicsItem>
#include"linehandlerect.h"
class GraphicsSingleLineItem : public QGraphicsItem
{
public:
    GraphicsSingleLineItem(QLine line,QGraphicsItem * parent=nullptr);
    GraphicsSingleLineItem(int x1,int y1,int x2,int y2,QGraphicsItem * parent=nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const;

    LineHandleRect::Direction  hitTest( const QPointF & point ) const;

    virtual void resizeTo(LineHandleRect::Direction dir, const QPointF & point );

    Qt::CursorShape getCursor(LineHandleRect::Direction dir)
    {
        switch (dir) {
        case LineHandleRect::PT1:

            return Qt::SizeAllCursor;
        case LineHandleRect::PT2:
            return Qt::SizeAllCursor;
        case LineHandleRect::None:
            return Qt::ArrowCursor;

        default:
            break;
        }
        return Qt::ArrowCursor;
    }

    int X1(){return this->m_line.x1();}
    int X2(){return this->m_line.x2();}
    int Y1(){return this->m_line.y1();}
    int Y2(){return this->m_line.y2();}
    void updateGeometry();
    QRectF boundingRect() const override;
     QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:

    QLine m_line;
    LineHandleRect* m_SizeHandle[2];
    bool m_Selected;
};

#endif // GRAPHICSSINGLELINEITEM_H
