#ifndef GRAPHICSQIMAGEITEM_H
#define GRAPHICSQIMAGEITEM_H

#include"graphicsitembase.h"
class GraphicsQImageItem : public GraphicsItemBase
{
public:
    GraphicsQImageItem(QImage& image,GraphicsItemBase* parent=nullptr);
    virtual QRectF boundingRect() const{return QRectF(0,0,this->m_Image.width(),this->m_Image.height());}
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    virtual void updateRect(){};
    virtual void setSelectState(bool b) override
    {}
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point ){};
    virtual SizeHandleRect::Direction  hitTest( const QPointF & point ) const{return  SizeHandleRect::None;};
    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir ){return   Qt::ArrowCursor;}

    void setQImage(QImage & image)
    {
        this->m_Image=image;this->update();
    }
    QImage& getQImage(){return m_Image;}

    void setItemOpacity(qreal val){this->m_Opacity=val;}


    int type() const override
    {
        return GRAPHICSITEMTYPE_BACKGROUNDIMAGE;
    }
private:
    QImage& m_Image;

    qreal m_Opacity=1;
};

#endif // GRAPHICSQIMAGEITEM_H
