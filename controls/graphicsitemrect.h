#ifndef GRAPHICSITEMRECT_H
#define GRAPHICSITEMRECT_H

#include"graphicsitembase.h"
#include"sizehandlerect.h"



class GraphicsItemRect : public GraphicsItemBase
{
public:
    GraphicsItemRect(QRect rect,GraphicsItemBase* parent);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    QRectF rect() const;
    virtual void updateRect() override;

    int type() const
    {
        return  GRAPHICSITEMTYPE_RECT;
    }
protected:
    void setState(SizeHandleRect::SelectionHandleState st);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


protected:
    void updateGeometry();
public:
    virtual void resizeTo(SizeHandleRect::Direction dir,int pix);
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    virtual SizeHandleRect::Direction  hitTest( const QPointF & point ) const;
    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir );
    virtual void setSelectState(bool b){
        setState(b? SizeHandleRect::SelectionHandleActive : SizeHandleRect::SelectionHandleOff);
        this->m_Selected=b;
        this->setSelected(b);
    }
    void setChangeable(bool b);
private:
    typedef QVector<SizeHandleRect*> Handles;
    Handles m_handles;

    int selection_handle_size = 16;

private:
    QRectF m_rect;

    //    bool m_Selected;
    bool m_IsChangeable=true;
    int m_MaxWidth=640;
    int m_MaxHeight=480;

};

#endif // GRAPHICSITEMRECT_H
