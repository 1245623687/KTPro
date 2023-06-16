#ifndef SIZEHANDLERECT_H
#define SIZEHANDLERECT_H


#include <QGraphicsRectItem>



class SizeHandleRect : public QGraphicsRectItem
{
public:
    enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };
    enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, PT11,PT12,PT21,PT22,SquarePt1,SquarePt2,SquarePt3,SquarePt4,
                     SquarePt5,SquarePt6,SquarePt7,SquarePt8,None};

    SizeHandleRect(QGraphicsItem* parent , QRectF rect, Direction dir);
    Direction dir() const;
    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );
    void setState(SelectionHandleState st);
    ~SizeHandleRect();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    const Direction m_dir;
    SelectionHandleState m_state;


};

#endif // SIZEHANDLERECT_H
