#include "graphicsitemscene.h"
#include<QGraphicsPixmapItem>
#include"dsdebug.h"
#include<QGraphicsSceneMouseEvent>
#include<QToolTip>

GraphicsItemScene::SelectMode selectMode = GraphicsItemScene::none;
LineHandleRect::Direction nDragHandle = LineHandleRect::None;

GraphicsItemScene::GraphicsItemScene()
{

}

void GraphicsItemScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QList<QGraphicsItem *> items = this->selectedItems();
    GraphicsSingleLineItem *item = 0;
    DSDEBUG<<"items.count():"<<items.count()<<endl;
    if ( items.count() == 1 )
    {
        item = qgraphicsitem_cast<GraphicsSingleLineItem*>(items.first());

        nDragHandle = item->hitTest(event->scenePos());
        DSDEBUG<<"nDragHandle:"<<nDragHandle<<endl;

        if ( nDragHandle !=LineHandleRect::None)
            selectMode = size;
        else
            selectMode =  move;
    }
    if(selectMode == move || selectMode == none)
    {
        QGraphicsScene::mousePressEvent(event);
    }

}

void GraphicsItemScene::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView*> views  = this->views();
    if ( views.count() > 0 )
    {
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}

void GraphicsItemScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QList<QGraphicsItem *> tmpItems = this->items();
    try {
        foreach (QGraphicsItem *item, tmpItems)
        {
            switch(item->type())
            {
            case 888:
            {

#ifdef IMG_TYPE_GRAY
                DSGraphicsItemImage<OpencvImage> * p= static_cast<DSGraphicsItemImage<OpencvImage>*>(item);
                int tmpW=p->getImage().ImageGray.cols;
                int tmpH=p->getImage().ImageGray.rows;

                int tmp_x=(int)event->scenePos().x();
                int tmp_y=(int)event->scenePos().y();
                if(tmp_x>=0&&tmp_x<tmpW&&tmp_y>=0&&tmp_y<tmpH)
                {
                    QString tooltip=QString("(%1,%2):%3").arg(tmp_x).arg(tmp_y).arg(p->getImage().ImageGray.at<uchar>(tmp_y,tmp_x));
                    QToolTip::showText(QPoint(event->screenPos()),tooltip);
                }
                throw 0;
#endif
#ifdef IMG_TYPE_RGB
                DSGraphicsItemImage<OpencvImage> * p= static_cast<DSGraphicsItemImage<OpencvImage>*>(item);
                int tmpW=p->getImage().ImageRGB.cols;
                int tmpH=p->getImage().ImageRGB.rows;

                int tmp_x=(int)event->scenePos().x();
                int tmp_y=(int)event->scenePos().y();
                if(tmp_x>=0&&tmp_x<tmpW&&tmp_y>=0&&tmp_y<tmpH)
                {
                    QString tooltip=QString("(%1,%2):(%3,%4,%5)").arg(tmp_x).arg(tmp_y).arg(p->getImage().ImageRGB.at<cv::Vec3b>(tmp_y,tmp_x)[0]).arg(p->getImage().ImageRGB.at<cv::Vec3b>(tmp_y,tmp_x)[1]).arg(p->getImage().ImageRGB.at<cv::Vec3b>(tmp_y,tmp_x)[2]);
                    QToolTip::showText(QPoint(event->screenPos()),tooltip);
                }
                throw 0;
#endif


            }
                break;
            }


        }
    }
    catch(int exception)
    {

    }
    catch (...)
    {
        DSDEBUG<<"unknow err!"<<endl;
    }

//    QString tooltip=QString("(%1,%2)").arg(event->scenePos().x()).arg(event->scenePos().y());
//    QToolTip::showText(QPoint(event->screenPos()),tooltip);

    //qDebug()<<"scene:"<< event->scenePos()<<endl;
    QList<QGraphicsItem *> items = this->selectedItems();
    //qDebug()<<"items.count()"<<items.count()<<endl;
    //qDebug()<<"all items.count()"<<this->items().count()<<endl;

    if(items.count() == 1)
    {
        GraphicsSingleLineItem *item = qgraphicsitem_cast<GraphicsSingleLineItem*>(items.first());

        if ( nDragHandle != LineHandleRect::None && selectMode == size )
        {
            item->resizeTo(nDragHandle,event->scenePos());
        }
        else if(nDragHandle == LineHandleRect::None && selectMode == none )
        {
            LineHandleRect::Direction handle = item->hitTest(event->scenePos());
            if ( handle != LineHandleRect::None)
            {
                //DSDEBUG<<"CURSOR:"<<item->getCursor(handle)<<endl;
                setCursor(item->getCursor(handle));
            }
            else
            {
                setCursor(Qt::ArrowCursor);
            }
        }
        else if(nDragHandle == LineHandleRect::None && selectMode == move )
        {
            //不在这里处理，最后直接获取在scene上的坐标即可！
            //            item->setPos(event->scenePos());
            //            qDebug()<<"item.scenepos:"<<item->pos()<<endl;
            //            qDebug()<<"event->scenePos()"<<event->scenePos()<<endl;
        }
    }

    this->update();
  //  QGraphicsScene::mouseMoveEvent(event);


}
void GraphicsItemScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //DSDEBUG<<"enter mouseReleaseEvent"<<endl;
    QList<QGraphicsItem *> items = this->selectedItems();

//    if(items.count() == 1)
//    {
//        GraphicsRectItem *item = qgraphicsitem_cast<GraphicsRectItem*>(items.first());
//        if ( nDragHandle != SizeHandleRect::None && selectMode == size )
//        {
//            item->updateRect();
//        }
//        //QMap<int,GraphicsRectItem*>::iterator iit=m_mapRectItem.begin();

//        //  for (;iit!=m_mapRectItem.end();iit++) {
//        //      if(iit.value()->equal(item))
//        //      {
//        //          DSDEBUG<<"item pos equal:"<<iit.value()->scenePos()<<endl;
//        //      }
//        //  }
//        //DSDEBUG<<"item pos:"<<item->scenePos()<<endl;
//    }
    setCursor(Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = LineHandleRect::None;

    QGraphicsScene::mouseReleaseEvent(event);

}



void GraphicsItemScene::setBackGroundImage(DSGraphicsItemImage<OpencvImage>* image)
{
    QPixmap pixMap;
    image->getImage().toQPixmap(pixMap);
    this->SetBackGroundImage(pixMap,pixMap.width(),pixMap.height());
}

void GraphicsItemScene::SetBackGroundImage(QPixmap pix, int width, int height)
{
    this->clear();
    QGraphicsPixmapItem* _backGroundItem= this->addPixmap(pix);

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
}
