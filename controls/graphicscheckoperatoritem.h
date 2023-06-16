#ifndef GRAPHICSCHECKOPERATORITEM_H
#define GRAPHICSCHECKOPERATORITEM_H
#include <QGraphicsItem>
#include "sizehandlerect.h"
#include"graphicslineitem.h"

class GraphicsCheckOperatorItem : public QGraphicsItem
{
public:
    enum ENUMRECTTYPE
    {
        ENUMRECTTYPE_RECTR,//单矩形
        ENUMRECTTYPE_RECTC,//单矩形
        ENUMRECTTYPE_RECTLINE,//有描线的举行
        ENUMRECTTYPE_RECTBLANCK,//没有描述符的矩形
        ENUMRECTTYPE_SQUARE//不规则四边形
    };

    enum ENUMSTATE
    {
      ENUMSTATE_INVALID=2,
      ENUMSTATE_OK=0,
      ENUMSTATE_NG=1,
    };
public:
    GraphicsCheckOperatorItem(const QRect & rect ,int index,QGraphicsItem * parent,ENUMRECTTYPE type=ENUMRECTTYPE_RECTR,const QVector<QPoint>& vPt=QVector<QPoint>());

    QRectF boundingRect() const override;
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    void move(const QPointF & point);
    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir );
    SizeHandleRect::Direction  hitTest( const QPointF & point ) const;

    bool equal(GraphicsCheckOperatorItem* & another);
    virtual QRectF  rect() const;

    void setWidth(int wid);
    void setHeight(int hei);

    void setMaxWidth(int wid)
    {
        this->m_MaxWidth=wid;
    }
    void setMaxHeight(int hei)
    {
        this->m_MaxHeight=hei;
    }

    ENUMRECTTYPE type(){return  this->m_type;}

    int type() const
    {
        return 999;
    }

    void setCalcState(int state);

    virtual void updateGeometry();
    QRectF m_rect;

    //获取item在scene中位置
    QPointF getPosInScene();

    void updateRect();
    bool selected(){return  m_Selected;}

    void setMainRect()
    {
        m_IsMainRect=true;
    }
    void setMoveable(bool b)
    {
        setFlag(QGraphicsItem::ItemIsMovable, b);
    }

    bool modified()
    {
        return this->m_Modified;
    }

    void setModified(bool b)
    {
        this->m_Modified=b;
    }

    ~GraphicsCheckOperatorItem() override;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void setState(SizeHandleRect::SelectionHandleState st);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


private:

    typedef QVector<SizeHandleRect*> Handles;
    Handles m_handles;
    int selection_handle_size = 8;
    bool m_Selected;
    bool m_Modified;

    ENUMRECTTYPE m_type;
    int m_Index;
    QVector<QLine> m_VLineRes;
    GraphicsLineItem * m_VLineItem;
    QVector<SizeHandleRect*> m_SquareHandles;
    ENUMSTATE m_State;
    int m_MaxWidth=640;
    int m_MaxHeight=480;

    bool m_IsMainRect=false;
public:
    QVector<QPoint> m_SquarePt;
};

#endif // GRAPHICSCHECKOPERATORITEM_H
