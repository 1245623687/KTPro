#include "graphicssceneconfig.h"
#include<QGraphicsView>

GraphicsSceneConfig::GraphicsSceneConfig(int camIdx)
{
    //QGraphicsSimpleTextItem *newIterm=new QGraphicsSimpleTextItem();

//    m_ptxtIterm= new QGraphicsSimpleTextItem();
//    QFont font("Times", 28, QFont::Thin);
//    m_ptxtIterm->setFont(font);
//    m_ptxtIterm->setPos(QPoint(10,5));
//    m_ptxtIterm->setBrush(QBrush(QColor(255,255,0)));

    m_CamIdx=camIdx;
    m_ptxtIterm=NULL;
    qDebug()<<"GraphicsSceneConfig:"<<endl;
}

void GraphicsSceneConfig::addCheckOperator(CheckOperatorBasic* checkoperator)
{
    QRect rect(0,
               0,
               checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
               checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ys);
    QPoint rectPos(checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                   checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ys);
    int index_i=checkoperator->stu_CheckOperatorBasic->Index;

    switch (checkoperator->CheckType)
    {

    case  ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    {
        CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch=static_cast<CheckOperatorTemplateMatch*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorTemplateMatch;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    {
        CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorEdgePositioning;

    }
        break;
    case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
    {
        CheckOperatorDefectInspect* pCheckOperatorDefectInspect=static_cast<CheckOperatorDefectInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorDefectInspect;

    }
        break;
    case  ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
    {
        CheckOperatorCircleInspect* pCheckOperatorCircleInspect=static_cast<CheckOperatorCircleInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorCircleInspect;

    }
        break;
    case  ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
    {
        CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect=static_cast<CheckOperatorBrokenlineInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorBrokenlineInspect;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
    {
        CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement=static_cast<CheckOperatorGrayMeasurement*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorGrayMeasurement;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_LINEINSPECT:
    {
        CheckOperatorLineInspect* pCheckOperatorLineInspect=static_cast<CheckOperatorLineInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorLineInspect;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
    {
        CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect=static_cast<CheckOperatorAssociatedInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorAssociatedInspect;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_STAYINSPECT:
    {
        CheckOperatorStayInspect* pCheckOperatorStayInspect=static_cast<CheckOperatorStayInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorStayInspect;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
    {
        CheckOperatorDoubleStayInspect* pCheckOperatorStayInspect=static_cast<CheckOperatorDoubleStayInspect*>(checkoperator);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorStayInspect;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
    {
        CheckOperatorDefectInspect2* pCheckOperatorDefectInspect2=static_cast<CheckOperatorDefectInspect2*>(checkoperator);
        QVector<QPoint> pts;
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X1,pCheckOperatorDefectInspect2->Y1));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X2,pCheckOperatorDefectInspect2->Y2));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X3,pCheckOperatorDefectInspect2->Y3));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X4,pCheckOperatorDefectInspect2->Y4));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X5,pCheckOperatorDefectInspect2->Y5));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X6,pCheckOperatorDefectInspect2->Y6));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X7,pCheckOperatorDefectInspect2->Y7));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X8,pCheckOperatorDefectInspect2->Y8));

        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_SQUARE,pts);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorDefectInspect2;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
    {
        CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2=static_cast<CheckOperatorAssociatedInspect2*>(checkoperator);
        QVector<QPoint> pts;
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X1,pCheckOperatorAssociatedInspect2->Y1));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X2,pCheckOperatorAssociatedInspect2->Y2));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X3,pCheckOperatorAssociatedInspect2->Y3));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X4,pCheckOperatorAssociatedInspect2->Y4));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X5,pCheckOperatorAssociatedInspect2->Y5));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X6,pCheckOperatorAssociatedInspect2->Y6));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X7,pCheckOperatorAssociatedInspect2->Y7));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X8,pCheckOperatorAssociatedInspect2->Y8));

        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_SQUARE,pts);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorAssociatedInspect2;
    }
        break;

    case  ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
    {
        CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect=static_cast<CheckOperatorDLObjectDetect*>(checkoperator);

        // QRect rect(0,0,m_MaxWidth,m_MaxHeight);
        GraphicsItemConfig* _item=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(_item);
        _item->setPos(rectPos);
        _mapItem2Checkoperator[_item]=pCheckOperatorDLObjectDetect;
    }
        break;
    }

    update();
}

void GraphicsSceneConfig::setCalcResState(unsigned char arrRes[256])
{
    //    QMap<int,GraphicsCheckOperatorItem*>::iterator itor=m_mapRectItem.begin();
    //    for (;itor!=m_mapRectItem.end();++itor)
    //    {
    //        (*itor)->setCalcState(arrRes[itor.key()]);
    //    }
    //    update();

    QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor= _mapItem2Checkoperator.begin();
    for (;itor!=_mapItem2Checkoperator.end();++itor)
    {
        itor.key()->setCalcState(arrRes[itor.value()->stu_CheckOperatorBasic->Index]);
    }
    update();
}

void GraphicsSceneConfig::setText(QString text)
{
//    if(m_ptxtIterm!=nullptr)
//    {
////        QFont font("Times", 28, QFont::Thin);
////        m_ptxtIterm->setFont(font);
////        m_ptxtIterm->setPos(QPoint(10,5));
////        m_ptxtIterm->setBrush(QBrush(QColor(255,255,0)));
////          m_ptxtIterm->setText(text);

//    }
//    else
    {
        m_ptxtIterm= this->addSimpleText(text);
        QFont font("Times", 28, QFont::Thin);
        m_ptxtIterm->setFont(font);
        m_ptxtIterm->setPos(QPoint(10,5));
        m_ptxtIterm->setBrush(QBrush(QColor(255,255,0)));
//        m_ptxtIterm->setText(text);
//        this->addItem(m_ptxtIterm);

    }



}

void GraphicsSceneConfig::SetBackGroundImage(QPixmap pix, int width, int height)
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

void GraphicsSceneConfig::clear()
{
    _mapItem2Checkoperator.clear();
    QGraphicsScene::clear();
}

GraphicsSceneConfig::~GraphicsSceneConfig()
{
    this->clear();
}


int GraphicsSceneConfig::selectedIndex()
{
    int ret=-1;
    QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator  itor= _mapItem2Checkoperator.begin();
    for (;itor!= _mapItem2Checkoperator.end();++itor)
    {
        if(itor.key()->selected())
        {
            ret=itor.value()->stu_CheckOperatorBasic->Index;
            break;
        }

    }
    return  ret;
}


void GraphicsSceneConfig::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    QList<QGraphicsItem *> items = this->selectedItems();
    if(items.size()<1) return;
    if(items.count() == 1)
    {
        GraphicsItemConfig *item = qgraphicsitem_cast<GraphicsItemConfig*>(items.first());

        //      emit sceneClick(_mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Index-1);
        emit sceneDoubleClick(_mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Index);


        //        int kk=0;
    }
    else {
        emit sceneDoubleClick(-1);
        emit sceneClick(-1);
    }
}

void GraphicsSceneConfig::deleteItem(int index)
{
    DSDEBUG<<"enter deleteItem"<<endl;
    //先拷贝进临时数组

    QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor=this->_mapItem2Checkoperator.begin();
    for (;itor!=_mapItem2Checkoperator.end();++itor)
    {
        if(itor.value()->stu_CheckOperatorBasic->Index==index)
        {
            _mapItem2Checkoperator.remove(itor.key());
            break;
        }
    }

}

void GraphicsSceneConfig::setRectMode(int mode)
{
    this->m_Mode=static_cast<ENUMRECTMODE>(mode);

    switch (this->m_Mode)
    {
    case  ENUMRECTMODE_ALL:
    {
        this->clearSelection();
        QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor= _mapItem2Checkoperator.begin();
        for (;itor!=this->_mapItem2Checkoperator.end();itor++)
        {
            itor.key()->show();
        }


    }
        break;
    case  ENUMRECTMODE_CUR:
    {
        this->clearSelection();
        QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor= _mapItem2Checkoperator.begin();
        for (;itor!=this->_mapItem2Checkoperator.end();itor++)
        {
            itor.key()->hide();
        }
    }
        break;
    }
    this->update();
}

void GraphicsSceneConfig::selectRectItem(int idx)
{
    switch (this->m_Mode)
    {
    case  ENUMRECTMODE_ALL:
    {
        this->clearSelection();
        QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor=this->_mapItem2Checkoperator.begin();
        for (;itor!=this->_mapItem2Checkoperator.end();itor++) {
            if(idx==itor.value()->stu_CheckOperatorBasic->Index)
            {
                itor.key()->setSelected(true);
                break;
            }
        }
    }
        break;
    case  ENUMRECTMODE_CUR:
    {
        this->clearSelection();
        QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator itor=this->_mapItem2Checkoperator.begin();
        for (;itor!=this->_mapItem2Checkoperator.end();itor++)
        {
            if(idx==itor.value()->stu_CheckOperatorBasic->Index)
            {
                itor.key()->show();
                itor.key()->setSelected(true);

            }
            else
            {
                itor.key()->hide();
            }
        }

    }
        break;
    }

    this->update();
}


void GraphicsSceneConfig::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QList<QGraphicsItem *> items = this->selectedItems();
    if(items.count() == 1)
    {
        //DSDEBUG<<items.count();
        GraphicsItemConfig *item = qgraphicsitem_cast<GraphicsItemConfig*>(items.first());

        if ( nDragHandle != SizeHandleRect::None && selectMode == size )
        {
            //            if(_mapItem2Checkoperator[item]->CheckType!=ENUMCHECKOPERATORTYPE_TEMPLATEMATCH)
            //            {
            item->resizeTo(nDragHandle,event->scenePos());
            //            }
            //            else
            //            {
            //                if(static_cast<CheckOperatorTemplateMatch*>(_mapItem2Checkoperator[item])->SearchNum>0)
            //                {
            //                    return;
            //                }
            //                else
            //                {
            //                    item->resizeTo(nDragHandle,event->scenePos());
            //                }

            //            }

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
             qDebug()<<"moving "<<endl;

            //            if(_mapItem2Checkoperator[item]->CheckType==ENUMCHECKOPERATORTYPE_TEMPLATEMATCH)
            //            {
            //                 if(static_cast<CheckOperatorTemplateMatch*>(_mapItem2Checkoperator[item])->SearchNum>0)
            //                 {
            //                     return;
            //                 }
            //            }

            // this->mouseGrabberItem()->parentItem()->setPos(event->pos());
            //            QGraphicsItem* grabberItem=this->mouseGrabberItem();
            //            qDebug()<<"enter move:"<<grabberItem->type()<<endl;
            //            qDebug()<<grabberItem->scenePos();
            //            qDebug()<<grabberItem->parentItem()->scenePos();
        }
    }

    this->update();
    QGraphicsScene::mouseMoveEvent(event);
}


void GraphicsSceneConfig::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->selectedItems();
    GraphicsItemBase *item = 0;
    if ( items.count() == 1 )
    {
        item = qgraphicsitem_cast<GraphicsItemBase*>(items.first());

        item = qgraphicsitem_cast<GraphicsItemConfig*>(items.first());

        nDragHandle = item->hitTest(event->scenePos());


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

void GraphicsSceneConfig::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView*> views  = this->views();
    if ( views.count() > 0 )
    {
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}


void GraphicsSceneConfig::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> items = this->selectedItems();

    if(items.count() == 1)
    {
        GraphicsItemConfig *item = qgraphicsitem_cast<GraphicsItemConfig*>(items.first());

        if ( nDragHandle != SizeHandleRect::None && selectMode == size )
        {
            item->updateRect();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Xs=item->scenePos().x();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Ys=item->scenePos().y();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Xe=item->scenePos().x()+item->rect().width();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Ye=item->scenePos().y()+item->rect().height();
        }

        if(nDragHandle == SizeHandleRect::None)
        {
             qDebug()<<"releasing "<<endl;

            item->updateRectPos();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Xs=item->scenePos().x();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Ys=item->scenePos().y();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Xe=item->scenePos().x()+item->rect().width();
            _mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Basic_RectValid_Ye=item->scenePos().y()+item->rect().height();

        }

        if(_mapItem2Checkoperator[item]->CheckType==ENUMCHECKOPERATORTYPE_DEFECTINSPECT2)
        {

            CheckOperatorDefectInspect2* pCheckOperator=static_cast<CheckOperatorDefectInspect2*>((_mapItem2Checkoperator[item]));
            pCheckOperator->X1=item->m_SquarePt[0].x();
            pCheckOperator->X2=item->m_SquarePt[1].x();
            pCheckOperator->X3=item->m_SquarePt[2].x();
            pCheckOperator->X4=item->m_SquarePt[3].x();
            pCheckOperator->X5=item->m_SquarePt[4].x();
            pCheckOperator->X6=item->m_SquarePt[5].x();
            pCheckOperator->X7=item->m_SquarePt[6].x();
            pCheckOperator->X8=item->m_SquarePt[7].x();
            pCheckOperator->Y1=item->m_SquarePt[0].y();
            pCheckOperator->Y2=item->m_SquarePt[1].y();
            pCheckOperator->Y3=item->m_SquarePt[2].y();
            pCheckOperator->Y4=item->m_SquarePt[3].y();
            pCheckOperator->Y5=item->m_SquarePt[4].y();
            pCheckOperator->Y6=item->m_SquarePt[5].y();
            pCheckOperator->Y7=item->m_SquarePt[6].y();
            pCheckOperator->Y8=item->m_SquarePt[7].y();
        }

        if(_mapItem2Checkoperator[item]->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2)
        {

            CheckOperatorAssociatedInspect2* pCheckOperator=static_cast<CheckOperatorAssociatedInspect2*>((_mapItem2Checkoperator[item]));
            pCheckOperator->X1=item->m_SquarePt[0].x();
            pCheckOperator->X2=item->m_SquarePt[1].x();
            pCheckOperator->X3=item->m_SquarePt[2].x();
            pCheckOperator->X4=item->m_SquarePt[3].x();
            pCheckOperator->X5=item->m_SquarePt[4].x();
            pCheckOperator->X6=item->m_SquarePt[5].x();
            pCheckOperator->X7=item->m_SquarePt[6].x();
            pCheckOperator->X8=item->m_SquarePt[7].x();
            pCheckOperator->Y1=item->m_SquarePt[0].y();
            pCheckOperator->Y2=item->m_SquarePt[1].y();
            pCheckOperator->Y3=item->m_SquarePt[2].y();
            pCheckOperator->Y4=item->m_SquarePt[3].y();
            pCheckOperator->Y5=item->m_SquarePt[4].y();
            pCheckOperator->Y6=item->m_SquarePt[5].y();
            pCheckOperator->Y7=item->m_SquarePt[6].y();
            pCheckOperator->Y8=item->m_SquarePt[7].y();
        }



        sceneClick(_mapItem2Checkoperator[item]->stu_CheckOperatorBasic->Index-1);
    }
    setCursor(Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    emit sceneMouseRelease();
    emit changeSelectedItemIdx(this->selectedIndex());
    QGraphicsScene::mouseReleaseEvent(event);


}


void GraphicsSceneConfig::setSelectedItem(int idx)
{
    QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator it= _mapItem2Checkoperator.begin();
    for (;it!= _mapItem2Checkoperator.end();++it)
    {
        if(it.value()->stu_CheckOperatorBasic->Index==idx)
            it.key()->setSelectState(true);
        else
            it.key()->setSelectState(false);

    }
}

void GraphicsSceneConfig::setItemChangeable(bool b)
{
    m_ItemChangeable=b;
    QMap<GraphicsItemConfig*,CheckOperatorBasic*>::iterator it= _mapItem2Checkoperator.begin();
    for (;it!= _mapItem2Checkoperator.end();++it)
    {

    }
}
