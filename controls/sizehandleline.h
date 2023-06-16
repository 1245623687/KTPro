#ifndef SIZEHANDLELINE_H
#define SIZEHANDLELINE_H



#include <QGraphicsRectItem>


enum ENUMSIZEHANDLELINESTATE
{
    ENUMSIZEHANDLELINESTATE_OFF,
    ENUMSIZEHANDLELINESTATE_INACTIVE,
    ENUMSIZEHANDLELINESTATE_ACTIVE
};

class SizeHandleLine: public QGraphicsRectItem
{

public:
    SizeHandleLine(QGraphicsItem* parent , QRectF rect,int index);
    SizeHandleLine(SizeHandleLine& another)
    {
        this->m_state=another.state();
        this->m_Index=another.index();
    }
    ENUMSIZEHANDLELINESTATE state(){return this->m_state;}
    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );
    void setState(ENUMSIZEHANDLELINESTATE st);

    int index(){return m_Index;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:

    ENUMSIZEHANDLELINESTATE m_state;
    int m_Index;
};

#endif // SIZEHANDLELINE_H
