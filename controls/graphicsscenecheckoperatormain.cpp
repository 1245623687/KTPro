#include "graphicsscenecheckoperatormain.h"
#include"dsdebug.h"
#include<QToolTip>
#include <QGraphicsView>
GraphicsSceneCheckOperatorMain::SelectMode selectMode = GraphicsSceneCheckOperatorMain::none;
SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;


GraphicsSceneCheckOperatorMain::GraphicsSceneCheckOperatorMain()
{


}



void GraphicsSceneCheckOperatorMain::createRect()
{

    this->clearSelection();
    GraphicsCheckOperatorItem* rectItem = new GraphicsCheckOperatorItem(QRect(0, 0, 200, 100), this->m_mapRectItem.size()+1,NULL);
    this->addItem(rectItem);
    rectItem->setSelected(true);
    this->m_mapRectItem.insert(this->m_mapRectItem.size()+1,rectItem);
    this->update();
}

void GraphicsSceneCheckOperatorMain::setItemModified(int idx,bool modify)
{
    this->m_mapRectItem[idx]->setModified(modify);
    this->m_mapRectItem[idx]->setMoveable(modify);
}

void GraphicsSceneCheckOperatorMain::setCalcResState(unsigned char arrRes[256])
{
    QMap<int,GraphicsCheckOperatorItem*>::iterator itor=m_mapRectItem.begin();
    for (;itor!=m_mapRectItem.end();++itor)
    {
        (*itor)->setCalcState(arrRes[itor.key()]);
    }
    update();
}

void GraphicsSceneCheckOperatorMain::createRect(int checkNo,QRect rect,GraphicsCheckOperatorItem::ENUMRECTTYPE type,bool modify,const QVector<QPoint>& pts)
{

    this->clearSelection();
    GraphicsCheckOperatorItem* rectItem=nullptr;
    if(type==GraphicsCheckOperatorItem::ENUMRECTTYPE_SQUARE&&pts.size()==4)
    {

        rectItem=new GraphicsCheckOperatorItem(QRect(0,0,rect.width(),rect.height()),checkNo, NULL,type,pts);
    }
    else
        rectItem=new GraphicsCheckOperatorItem(QRect(0,0,rect.width(),rect.height()),checkNo, NULL,type);

    this->m_mapRectItem.insert(checkNo,rectItem);
    DSDEBUG<<rect.x()<<rect.y();
    this->addItem(rectItem);
    rectItem->setPos(rect.x(),rect.y());

    rectItem->setSelected(true);
    rectItem->setModified(true);
    rectItem->setMoveable(true);
    this->update();
}


void GraphicsSceneCheckOperatorMain::setRectMode(int mode)
{
    this->m_Mode=static_cast<ENUMRECTMODE>(mode);

    switch (this->m_Mode)
    {
    case  ENUMRECTMODE_ALL:
    {
        this->clearSelection();
        QMap<int,GraphicsCheckOperatorItem *>::iterator itor=this->m_mapRectItem.begin();
        for (;itor!=this->m_mapRectItem.end();itor++)
        {

            itor.value()->show();
        }
    }
        break;
    case  ENUMRECTMODE_CUR:
    {
        this->clearSelection();
        QMap<int,GraphicsCheckOperatorItem *>::iterator itor=this->m_mapRectItem.begin();
        for (;itor!=this->m_mapRectItem.end();itor++)
        {
            itor.value()->hide();

        }

    }
        break;
    }
    this->update();
}

void GraphicsSceneCheckOperatorMain::clear()
{
    if(m_mapRectItem.size()>0)
    {

        qDeleteAll(this->m_mapRectItem);
        this->m_mapRectItem.clear();
    }
    QGraphicsScene::clear();

}

void GraphicsSceneCheckOperatorMain::selectRectItem(int idx)
{
    switch (this->m_Mode)
    {
    case  ENUMRECTMODE_ALL:
    {
        this->clearSelection();
        QMap<int,GraphicsCheckOperatorItem *>::iterator itor=this->m_mapRectItem.begin();
        for (;itor!=this->m_mapRectItem.end();itor++) {
            if(idx==itor.key())
            {
                itor.value()->setSelected(true);
                break;
            }
        }
    }
        break;
    case  ENUMRECTMODE_CUR:
    {
        this->clearSelection();
        QMap<int,GraphicsCheckOperatorItem *>::iterator itor=this->m_mapRectItem.begin();
        for (;itor!=this->m_mapRectItem.end();itor++)
        {
            if(idx==itor.key())
            {
                itor.value()->show();
                itor.value()->setSelected(true);

            }
            else
            {
                itor.value()->hide();
            }
        }

    }
        break;
    }




    this->update();
}




void GraphicsSceneCheckOperatorMain::deleteItem(int index)
{
    DSDEBUG<<"enter deleteItem"<<endl;
    //先拷贝进临时数组
    QMap<int,GraphicsCheckOperatorItem*> mtemp;
    QMap<int,GraphicsCheckOperatorItem*>::iterator itor=this->m_mapRectItem.begin();
    for (;itor!=this->m_mapRectItem.end();itor++)
    {
        mtemp.insert(itor.key(),itor.value());
    }

    //删除
    //    QMap<int,GraphicsRectItem *>::iterator itdel=this->m_mapRectItem.begin();
    //    for (;itdel!=this->m_mapRectItem.end();itdel++) {
    //        if(index==itdel.key())
    //        {
    //            this->removeItem(itdel.value());
    //        }
    //    }

    //清空原有
    this->m_mapRectItem.clear();
    //拷贝进去
    itor=mtemp.begin();
    for (;itor!=mtemp.end();itor++)
    {
        if(itor.key()<index)
        {
            this->m_mapRectItem.insert(itor.key(),itor.value());
        }
        else if(itor.key()==index)
        {
            this->removeItem(itor.value());
        }
        else
        {
            this->m_mapRectItem.insert(itor.key()-1,itor.value());
        }
    }
    mtemp.clear();
}



void GraphicsSceneCheckOperatorMain::SetBackGroundImage(QPixmap pix, int width, int height)
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


    //    _backGroundItem->setPos(0,0);
}

void GraphicsSceneCheckOperatorMain::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView*> views  = this->views();
    if ( views.count() > 0 )
    {
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}

//鼠标按下去的位置，如果是落在方向框之内，那就是改变大小，如果没有，则是拖动
void GraphicsSceneCheckOperatorMain::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->selectedItems();
    GraphicsCheckOperatorItem *item = 0;
    // DSDEBUG<<"items.count():"<<items.count()<<endl;
    if ( items.count() == 1 )
    {
        item = qgraphicsitem_cast<GraphicsCheckOperatorItem*>(items.first());

        nDragHandle = item->hitTest(event->scenePos());
        DSDEBUG<<"nDragHandle:"<<nDragHandle<<endl;

        if ( nDragHandle !=SizeHandleRect::None)
            selectMode = size;
        else
            selectMode =  move;
    }
    if(selectMode == move || selectMode == none)
    {
        QGraphicsScene::mousePressEvent(event);
    }
}

void GraphicsSceneCheckOperatorMain::clearRectItem()
{
    this->m_mapRectItem.clear();
}

void GraphicsSceneCheckOperatorMain::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    emit infoCoordinate(event->scenePos());

    //    QList<QGraphicsItem *> tmpItems = this->items();
    //    try {
    //        foreach (QGraphicsItem *item, tmpItems)
    //        {
    //            switch(item->type())
    //            {
    //            case 888:
    //            {
    //                DSGraphicsItemImage<OpenCvImage> * p= static_cast<DSGraphicsItemImage<OpenCvImage>*>(item);
    //                int tmpW=p->getImage().ImageGray.cols;
    //                int tmpH=p->getImage().ImageGray.rows;

    //                int tmp_x=(int)event->scenePos().x();
    //                int tmp_y=(int)event->scenePos().y();
    //                if(tmp_x>=0&&tmp_x<tmpW&&tmp_y>=0&&tmp_y<tmpH)
    //                {
    //                    QString tooltip=QString("(%1,%2):%3").arg(tmp_x).arg(tmp_y).arg(p->getImage().ImageGray.at<uchar>(tmp_y,tmp_x));
    //                    QToolTip::showText(QPoint(event->screenPos()),tooltip);
    //                }
    //                throw 0;
    //            }
    //                break;
    //            }


    //        }
    //    }
    //    catch(int exception)
    //    {

    //    }
    //    catch (...)
    //    {
    //        DSDEBUG<<"unknow err!"<<endl;
    //    }



    QList<QGraphicsItem *> items = this->selectedItems();

    if(items.count() == 1)
    {

        GraphicsCheckOperatorItem *item = qgraphicsitem_cast<GraphicsCheckOperatorItem*>(items.first());
        if(!item->modified()) return;
        //调整大小
        if ( nDragHandle != SizeHandleRect::None && selectMode == size )
        {
            item->resizeTo(nDragHandle,event->scenePos());
        }
        else if(nDragHandle == SizeHandleRect::None && selectMode == none )
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if ( handle != SizeHandleRect::None)
            {
                setCursor(item->getCursor(handle));
            }
            else
            {
                setCursor(Qt::ArrowCursor);
            }
        }
        else if(nDragHandle == SizeHandleRect::None && selectMode == move )
        {
            //不在这里处理，最后直接获取在scene上的坐标即可！
            //            item->setPos(event->scenePos());
            //            qDebug()<<"item.scenepos:"<<item->pos()<<endl;
            //            qDebug()<<"event->scenePos()"<<event->scenePos()<<endl;
        }
    }

    this->update();
    QGraphicsScene::mouseMoveEvent(event);
}





void GraphicsSceneCheckOperatorMain::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QList<QGraphicsItem *> items = this->selectedItems();
    if(items.count() == 1)
    {
        GraphicsCheckOperatorItem *item = qgraphicsitem_cast<GraphicsCheckOperatorItem*>(items.first());
        if ( nDragHandle != SizeHandleRect::None && selectMode == size )
        {
            item->updateRect();
        }

        QMap<int,GraphicsCheckOperatorItem*>::iterator itor=this->m_mapRectItem.begin();
        for(;itor!=this->m_mapRectItem.end();itor++)
        {
            if(itor.value()->equal(item))
            {
                sceneClick(itor.key()-1);
                break;
            }
        }
    }


    setCursor(Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    emit sceneMouseRelease();
    QGraphicsScene::mouseReleaseEvent(event);
}




void GraphicsSceneCheckOperatorMain::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    QList<QGraphicsItem *> items = this->selectedItems();
    if(items.size()<1) return;

    GraphicsCheckOperatorItem *item = qgraphicsitem_cast<GraphicsCheckOperatorItem*>(items.first());
    QMap<int,GraphicsCheckOperatorItem*>::iterator itor=this->m_mapRectItem.begin();
    for(;itor!=this->m_mapRectItem.end();itor++)
    {
        if(itor.value()->equal(item))
        {
            break;
        }
    }
    if(itor==m_mapRectItem.end())
    {
        DSDEBUG<<"err!!!  find no item in scene!"<<endl;

        return;
    }
    DSDEBUG<<itor.key()<<"  "<<itor.value()->scenePos()<<endl;
    emit sceneDoubleClick(itor.key());
    emit sceneClick(itor.key()-1);
}

void GraphicsSceneCheckOperatorMain::setBackGroundImage(OpencvImage image)
{
    QPixmap pixMap;
    image.toQPixmap(pixMap);
    this->SetBackGroundImage(pixMap,pixMap.width(),pixMap.height());
}


GraphicsSceneCheckOperatorMain::~GraphicsSceneCheckOperatorMain()
{
    if(this->m_mapRectItem.size()>0)
    {
        qDeleteAll(m_mapRectItem);
        m_mapRectItem.clear();
    }
    QList<QGraphicsItem *> items = this->selectedItems();
    if(items.size()>0)
    {
        qDeleteAll(items);
        this->clear();
    }
}



