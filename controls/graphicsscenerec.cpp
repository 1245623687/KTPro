#include "graphicsscenerec.h"
#include <QGraphicsView>
#include<QToolTip>
#include<QGraphicsLineItem>
#include"dsdebug.h"
#include<QGraphicsRectItem>


GraphicsSceneRec::GraphicsSceneRec(int width, int height,unsigned char* pImgCalc)
{

    if(m_CalcItem==nullptr)
    {
       m_CalcItem =new GraphicsCalcItem(PackageChecker::getInstance()->ImgHei,PackageChecker::getInstance()->ImgWid,pImgCalc);
       this->addItem(m_CalcItem);
    }
}


void GraphicsSceneRec::SetBackGroundImage(QPixmap pix, int width, int height)
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


void GraphicsSceneRec::clear()
{

    QGraphicsScene::clear();
}















GraphicsSceneRec::~GraphicsSceneRec()
{
    this->clear();
}







