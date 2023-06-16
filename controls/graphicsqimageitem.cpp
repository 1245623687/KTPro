#include "graphicsqimageitem.h"
#include<QPainter>
GraphicsQImageItem::GraphicsQImageItem( QImage& image,GraphicsItemBase* parent):
    GraphicsItemBase(parent),
    m_Image(image)
{


}


void GraphicsQImageItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    //this->m_Image.fill(Qt::darkYellow);
    //painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
    //painter->drawImage(QRectF(0,0,this->m_Image.width(),this->m_Image.height()),this->m_Image);


    //painter->setCompositionMode(QPainter::CompositionMode_SourceIn);
//    painter->setOpacity(m_Opacity);
//    painter->drawImage(QRectF(0,0,m_ImageCur.width(),m_ImageCur.height()),m_ImageCur);

//    painter->setOpacity(0);
    painter->drawImage(QRectF(0,0,m_Image.width(),m_Image.height()),m_Image);
}
