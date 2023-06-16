#include "graphicsscene.h"

GraphicsScene::GraphicsScene()
{

}

void GraphicsScene::SetBackGroundImage(QPixmap pix, int width, int height)
{


    this->clear();
    GraphicsPixmapItem* _backGroundItem=new GraphicsPixmapItem(nullptr);
    _backGroundItem->setPixmap(pix);
    qreal scale = 0.0;
    if((double)width/height > (double)pix.width()/pix.height())
    {
        scale = (double)height/pix.height();
    }
    else
    {
        scale = (double)width/pix.width();
    }
    _backGroundItem->setScale(scale);
    this->addItem(_backGroundItem);

    this->m_MaxWidth=width;
    this->m_MaxHeight=height;
}
