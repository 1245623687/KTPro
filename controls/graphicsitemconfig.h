#ifndef GRAPHICSITEMCONFIG_H
#define GRAPHICSITEMCONFIG_H

#include <QObject>
#include"graphicsitembase.h"
#include"CheckOperator/CheckOperator.hpp"

class GraphicsItemConfig : public GraphicsItemBase
{

public:
    enum ENUMRECTTYPE
    {
        ENUMRECTTYPE_RECTR,//单矩形
        ENUMRECTTYPE_RECTC,//单矩形
        ENUMRECTTYPE_RECTLINE,//有描线的举行
        ENUMRECTTYPE_RECTBLANCK,//没有描述符的矩形
        ENUMRECTTYPE_SQUARE,//不规则四边形
        ENUMRECTTYPE_CIRCLE//不规则四边形
    };

    enum ENUMSTATE
    {
        ENUMSTATE_OK=0,
        ENUMSTATE_NG=1,
        ENUMSTATE_INVALID=2,
    };




public:
    GraphicsItemConfig(const QRect & rect ,int index,GraphicsItemBase * parent,int camIdx=1,ENUMRECTTYPE type=ENUMRECTTYPE_RECTR,const QVector<QPoint>& vPt=QVector<QPoint>());

    virtual QRectF boundingRect() const override;

    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point ) override;

    void move(const QPointF & point);

    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir ) override;

    SizeHandleRect::Direction  hitTest( const QPointF & point ) const override;

    virtual void setSelectState(bool b) override;

    QRectF rect() const;

    ENUMRECTTYPE type(){return  this->m_Type;}

    void setCalcState(int state);

    virtual void updateRect() override;

    void updateRectPos();

//    bool selected(){return  m_Selected;}

    int type() const override
    {
        return  GRAPHICSITEMTYPE_RECT;
    }

    void setChangeable(bool changeable)
    {
        m_changeable=changeable;
    }

protected:
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void setState(SizeHandleRect::SelectionHandleState st);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


protected:
    void updateGeometry();





private:
    typedef QVector<SizeHandleRect*> Handles;
    Handles m_handles;
    int selection_handle_size = 8;


    QVector<SizeHandleRect*> m_SquareHandles;

private:
    int m_Index;
    QRectF m_rect;
    ENUMRECTTYPE m_Type;
    CheckOperatorBasic* m_CheckOperator;
    ENUMSTATE m_State;
    int m_MaxWidth=640;
    int m_MaxHeight=480;
    bool m_changeable;
public:
    QVector<QPoint> m_SquarePt;

};

#endif // GRAPHICSITEMCONFIG_H
