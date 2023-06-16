#ifndef LINEHANDLERECT_H
#define LINEHANDLERECT_H
#include<QGraphicsRectItem>

class LineHandleRect : public QGraphicsRectItem
{

public:
    enum Direction { PT1,PT2,None};
     enum SelectionHandleState { SelectionHandleOff, SelectionHandleActive };
    LineHandleRect( QRectF rect,Direction dir,QGraphicsItem* parent);
    bool hitTest( const QPointF & point );

    Direction dir() const
    {
        return  this->m_dir;
    }
    void move(qreal x, qreal y );
    void setState(bool visual);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:

    const Direction m_dir;
    SelectionHandleState m_state;
};

#endif // LINEHANDLERECT_H
