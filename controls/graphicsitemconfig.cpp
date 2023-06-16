#include "graphicsitemconfig.h"
#include<QPainter>
#include"PackageChecker.h"

GraphicsItemConfig::GraphicsItemConfig(const QRect &rect,int index, GraphicsItemBase *parent,int camIdx,ENUMRECTTYPE type,const QVector<QPoint>& vPt):
    GraphicsItemBase(parent),m_Index(index),m_Type(type),m_changeable(true)
{


    m_MaxWidth=PackageChecker::getInstance()->ImgWids[camIdx-1];
    m_MaxHeight=PackageChecker::getInstance()->ImgHeis[camIdx-1];

    this->m_State=ENUMSTATE_OK;
    m_Selected=true;
    m_rect = rect;

    m_handles.reserve(SizeHandleRect::None);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i)
    {
        SizeHandleRect *shr = new SizeHandleRect(this, QRectF(0,0,selection_handle_size,selection_handle_size), static_cast<SizeHandleRect::Direction>(i));
        m_handles.push_back(shr);
    }

    switch (type)
    {
    case ENUMRECTTYPE_SQUARE:
    {
        if(vPt.size()==0)
        {
            m_SquarePt.push_back(QPoint(50,0));
            m_SquarePt.push_back(QPoint(100,0));
            m_SquarePt.push_back(QPoint(100,50));
            m_SquarePt.push_back(QPoint(100,100));
            m_SquarePt.push_back(QPoint(50,100));
            m_SquarePt.push_back(QPoint(0,100));
            m_SquarePt.push_back(QPoint(0,50));
            m_SquarePt.push_back(QPoint(0,0));
        }
        else
        {
            for(int i_dx=0;i_dx<vPt.size();++i_dx)
                m_SquarePt.push_back(vPt[i_dx]);
        }

        for (int i = SizeHandleRect::SquarePt1; i <= SizeHandleRect::SquarePt8; ++i)
        {
            SizeHandleRect *shr = new SizeHandleRect(this, QRectF(0,0,selection_handle_size,selection_handle_size), static_cast<SizeHandleRect::Direction>(i));
            m_SquareHandles.push_back(shr);
        }
    }

        break;
    }




    setSelectState(false);
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}


QRectF GraphicsItemConfig::boundingRect() const
{
    return m_rect;
}


void GraphicsItemConfig::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen =painter->pen();
    pen.setWidth(1);
    switch (m_State)
    {
    case  ENUMSTATE_INVALID:
        pen.setColor(QColor(0, 0, 255));
        break;
    case  ENUMSTATE_OK:
        pen.setColor(QColor(0, 255, 0));
        break;
    case  ENUMSTATE_NG:
        pen.setColor(QColor(255, 0, 0));
        break;
    }
    pen.setStyle(m_Selected?Qt::SolidLine:Qt::DotLine);
    painter->setPen(pen);


    QFont tmpFont("黑体",15,QFont::Normal,false);
    painter->setFont(tmpFont);
    switch(this->type())
    {
    case ENUMRECTTYPE_RECTR:
        painter->drawText(QPoint(-1,-1),QString("R%1").arg(this->m_Index));
        painter->drawRect(rect());
        break;
    case ENUMRECTTYPE_CIRCLE:
        painter->drawText(QPoint(-1,-1),QString("C%1").arg(this->m_Index));
        painter->drawEllipse(0,0,rect().width(),rect().height());
        break;
    case ENUMRECTTYPE_RECTC:
    case ENUMRECTTYPE_RECTLINE:
        painter->drawRect(rect());
        painter->drawText(QPoint(-1,-1),QString("C%1").arg(this->m_Index));
        break;
    case ENUMRECTTYPE_RECTBLANCK:
        painter->drawRect(rect());
        break;

    case ENUMRECTTYPE_SQUARE:
        //DSDEBUG<<"drawLine";
        painter->drawText(QPoint(-1,-1),QString("C%1").arg(this->m_Index));
        painter->drawLine(m_SquarePt[0].x(),m_SquarePt[0].y(),m_SquarePt[1].x(),m_SquarePt[1].y());
        painter->drawLine(m_SquarePt[1].x(),m_SquarePt[1].y(),m_SquarePt[2].x(),m_SquarePt[2].y());
        painter->drawLine(m_SquarePt[2].x(),m_SquarePt[2].y(),m_SquarePt[3].x(),m_SquarePt[3].y());

        painter->drawLine(m_SquarePt[3].x(),m_SquarePt[3].y(),m_SquarePt[4].x(),m_SquarePt[4].y());
        painter->drawLine(m_SquarePt[4].x(),m_SquarePt[4].y(),m_SquarePt[5].x(),m_SquarePt[5].y());
        painter->drawLine(m_SquarePt[5].x(),m_SquarePt[5].y(),m_SquarePt[6].x(),m_SquarePt[6].y());
        painter->drawLine(m_SquarePt[6].x(),m_SquarePt[6].y(),m_SquarePt[7].x(),m_SquarePt[7].y());

        painter->drawLine(m_SquarePt[7].x(),m_SquarePt[7].y(),m_SquarePt[0].x(),m_SquarePt[0].y());

        break;
    }
}


QRectF GraphicsItemConfig::rect() const
{
    return m_rect;
}
void GraphicsItemConfig::updateRectPos()
{
    prepareGeometryChange();
    this->setPos( qMax(0,qMin(m_MaxWidth-qRound(this->rect().width()),qRound(this->scenePos().x()))),
                  qMax(0,qMin(m_MaxHeight-qRound(this->rect().height()),qRound(this->scenePos().y()))));
    updateGeometry();
    this->update();
}

void GraphicsItemConfig::updateRect()
{
    switch (m_Type)
    {
    case ENUMRECTTYPE_SQUARE:
    {

        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        QVector<QPoint>::iterator itor= m_SquarePt.begin();
        for(;itor!=m_SquarePt.end();++itor)
        {
            (*itor).setX((*itor).x()-rect().x());
            (*itor).setY((*itor).y()-rect().y());
        }

        this->setPos(this->scenePos().x()+rect().x(),this->scenePos().y()+rect().y());
        m_rect.setRect(0,0,m_rect.width(),m_rect.height());
        updateGeometry();
        this->update();

    }
        break;
    default:
        prepareGeometryChange();
        QPoint ptLeftUp=QPoint(this->rect().x(),this->rect().y());
        QPoint ptRightDown=QPoint(this->rect().x()+rect().width()+1,this->rect().y()+rect().height()+1);
        QPoint ptLeftDown=QPoint(this->rect().x(),this->rect().y()+rect().height()+1);
        QPoint ptRightUp=QPoint(this->rect().x()+rect().width()+1,this->rect().y());
        int newPos_x=INT_MAX;
        int newPos_y=INT_MAX;
        if(mapToScene(ptLeftUp).x()<newPos_x) newPos_x=qRound (mapToScene(ptLeftUp).x());
        if(mapToScene(ptRightDown).x()<newPos_x) newPos_x=qRound(mapToScene(ptRightDown).x());
        if(mapToScene(ptLeftDown).x()<newPos_x) newPos_x=qRound(mapToScene(ptLeftDown).x());
        if(mapToScene(ptRightUp).x()<newPos_x) newPos_x=qRound(mapToScene(ptRightUp).x());
        if(mapToScene(ptLeftUp).y()<newPos_y) newPos_y=qRound(mapToScene(ptLeftUp).y());
        if(mapToScene(ptRightDown).y()<newPos_y) newPos_y=qRound(mapToScene(ptRightDown).y());
        if(mapToScene(ptLeftDown).y()<newPos_y) newPos_y=qRound(mapToScene(ptLeftDown).y());
        if(mapToScene(ptRightUp).y()<newPos_y) newPos_y=qRound(mapToScene(ptRightUp).y());

        this->setPos(QPoint(newPos_x,newPos_y));
        this->m_rect=QRect(0,0,qAbs(rect().width()),qAbs(rect().height()));

        this->setPos( qMax(0,qMin(m_MaxWidth-qRound(this->rect().width()),qRound(this->scenePos().x()))),
                      qMax(0,qMin(m_MaxHeight-qRound(this->rect().height()),qRound(this->scenePos().y()))));

        updateGeometry();
        this->update();
        break;
    }

}





void GraphicsItemConfig::setState(SizeHandleRect::SelectionHandleState st)
{
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->setState(st);
    switch (this->type()) {
    case ENUMRECTTYPE_RECTR:
    case ENUMRECTTYPE_RECTC:
    case ENUMRECTTYPE_CIRCLE:
    case ENUMRECTTYPE_RECTBLANCK:
        break;
        //    case ENUMRECTTYPE_RECTLINE:
        //        m_VLineItem->setState(st);
        //        break;
    case ENUMRECTTYPE_SQUARE:
        for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
            (*it)->setState(SizeHandleRect::SelectionHandleOff);
        for(QVector<SizeHandleRect*>::iterator itor=m_SquareHandles.begin();itor!=m_SquareHandles.end();++itor)
        {
            SizeHandleRect *hndl = *itor;
            hndl->setState(st);
        }
        break;
    }
}


QVariant GraphicsItemConfig::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged )
    {
        setState(value.toBool() ? SizeHandleRect::SelectionHandleActive : SizeHandleRect::SelectionHandleOff);
        this->m_Selected=value.toBool();
    }
    else if ( change == QGraphicsItem::ItemRotationHasChanged )
    {
        //qDebug()<<"Item Rotation Changed:" << value.toString();
    }
    else if ( change == QGraphicsItem::ItemTransformOriginPointHasChanged )
    {
        //qDebug()<<"ItemTransformOriginPointHasChanged:" << value.toPointF();
    }
    return value;
}

void GraphicsItemConfig::updateGeometry()
{
    const QRectF &rect = this->boundingRect();
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
    {
        SizeHandleRect *hndl = *it;;
        switch (hndl->dir())
        {
        case SizeHandleRect::LeftTop:
            hndl->setPos(rect.x() - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
            break;
        case SizeHandleRect::Top:
            hndl->setPos(rect.x() + rect.width() / 2 - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
            break;
        case SizeHandleRect::RightTop:
            hndl->setPos(rect.x() + rect.width() - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
            break;
        case SizeHandleRect::Right:
            hndl->setPos(rect.x() + rect.width() - selection_handle_size / 2, rect.y() + rect.height() / 2 - selection_handle_size / 2);
            break;
        case SizeHandleRect::RightBottom:
            hndl->setPos(rect.x() + rect.width() - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
            break;
        case SizeHandleRect::Bottom:
            hndl->setPos(rect.x() + rect.width() / 2 - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
            break;
        case SizeHandleRect::LeftBottom:
            hndl->setPos(rect.x() - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
            break;
        case SizeHandleRect::Left:
            hndl->setPos(rect.x() - selection_handle_size / 2, rect.y() + rect.height() / 2 - selection_handle_size / 2);
            break;
        case SizeHandleRect::Center:
            hndl->setPos(rect.center().x()  - selection_handle_size / 2 , rect.center().y() - selection_handle_size / 2);
            break;
        case SizeHandleRect::PT11:
            hndl->setPos(10  - selection_handle_size / 2 , 10- selection_handle_size / 2);
            break;
        case SizeHandleRect::PT12:
            hndl->setPos(20  - selection_handle_size / 2 , 20- selection_handle_size / 2);
            break;
        case SizeHandleRect::PT21:
            hndl->setPos(30  - selection_handle_size / 2 , 30- selection_handle_size / 2);
            break;
        case SizeHandleRect::PT22:
            hndl->setPos(40  - selection_handle_size / 2 , 40- selection_handle_size / 2);
            break;
        default:
            break;
        }
    }

    int idx=0;
    for(QVector<SizeHandleRect*>::iterator itor=m_SquareHandles.begin();itor!=m_SquareHandles.end();++itor,++idx)
    {
        SizeHandleRect* hndl = *itor;
        hndl->setPos(m_SquarePt[idx].x()-selection_handle_size/2,m_SquarePt[idx].y()-selection_handle_size/2);
    }
}


void GraphicsItemConfig::resizeTo(SizeHandleRect::Direction dir, const QPointF & point )
{
    //qDebug()<<"enter resizeto"<<endl;
    QPointF local = mapFromScene(point);
    QString dirName;
    QRect delta =m_rect.toRect();

    QPoint maxPoint=mapFromScene(QPointF(this->m_MaxWidth-1,this->m_MaxHeight-1)).toPoint();
    QPoint minPoint=mapFromScene(QPointF(0,0)).toPoint();
    //DSDEBUG<<local.toPoint();
    switch (dir)
    {
    case SizeHandleRect::LeftTop:
        delta.setTopLeft(QPoint(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())),
                                qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y()))));
        break;
    case SizeHandleRect::Top:
        dirName = "Top";
        delta.setTop(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        break;
    case SizeHandleRect::RightTop:
        dirName = "RightTop";
        delta.setTopRight(QPoint(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())),
                                 qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y()))));
        break;
    case SizeHandleRect::Left:
        dirName = "Left";
        delta.setLeft(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        break;
    case SizeHandleRect::Right:
        dirName = "Rigth";
        delta.setRight(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        break;
    case SizeHandleRect::LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(QPoint(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())),
                                   qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y()))));
        break;
    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        delta.setBottom(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        break;
    case SizeHandleRect::RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(QPoint(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())),
                                    qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y()))));
        break;
    case SizeHandleRect::SquarePt1:
    {
        //        DSDEBUG<<"354   resizeto  squarePt1";

        m_SquarePt[0].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[0].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);

    }
        break;
    case SizeHandleRect::SquarePt2:
    {

        m_SquarePt[1].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[1].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt3:
    {

        m_SquarePt[2].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[2].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();


            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt4:
    {

        m_SquarePt[3].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[3].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt5:
    {

        m_SquarePt[4].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[4].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt6:
    {

        m_SquarePt[5].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[5].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt7:
    {

        m_SquarePt[6].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[6].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;
    case SizeHandleRect::SquarePt8:
    {

        m_SquarePt[7].setX(qMin(maxPoint.x(),qMax(local.toPoint().x(),minPoint.x())));
        m_SquarePt[7].setY(qMin(maxPoint.y(),qMax(local.toPoint().y(),minPoint.y())));
        int x_start=INT_MAX;
        int y_start=INT_MAX;
        int x_end=INT_MIN;
        int y_end=INT_MIN;
        for(int idx=0;idx<m_SquarePt.size();++idx)
        {
            if(m_SquarePt[idx].x()<x_start)
                x_start=m_SquarePt[idx].x();
            if(m_SquarePt[idx].x()>x_end)
                x_end=m_SquarePt[idx].x();
            if(m_SquarePt[idx].y()<y_start)
                y_start=m_SquarePt[idx].y();
            if(m_SquarePt[idx].y()>y_end)
                y_end=m_SquarePt[idx].y();
        }
        delta.setLeft(x_start);
        delta.setRight(x_end);
        delta.setTop(y_start);
        delta.setBottom(y_end);
    }
        break;

    default:
        break;
    }

    prepareGeometryChange();
    m_rect =delta;
    updateGeometry();
}


SizeHandleRect::Direction  GraphicsItemConfig::hitTest( const QPointF & point ) const
{
    switch(this->m_Type)
    {
    case ENUMRECTTYPE_RECTR:
    case ENUMRECTTYPE_RECTC:
    case ENUMRECTTYPE_CIRCLE:
    case ENUMRECTTYPE_RECTBLANCK:
    {
        const Handles::const_iterator hend =  m_handles.end();
        for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
        {
            if ((*it)->hitTest(point) )
            {
                return (*it)->dir();
            }
        }
    }
        break;
    case ENUMRECTTYPE_SQUARE:
    {
        for(QVector<SizeHandleRect*>::const_iterator itor=m_SquareHandles.begin();itor!=m_SquareHandles.end();++itor)
        {
            if ((*itor)->hitTest(point) )
            {
                return (*itor)->dir();
            }
        }
    }

        break;
    }
    return SizeHandleRect::None;
}

void GraphicsItemConfig::setCalcState(int state)
{
    this->m_State=static_cast<ENUMSTATE>(state);
}

void GraphicsItemConfig::setSelectState(bool b)
{
    this->m_Selected=b;
    this->setSelected(b);
}

Qt::CursorShape GraphicsItemConfig::getCursor(SizeHandleRect::Direction dir )
{
    switch (dir) {
    case SizeHandleRect::Right:
        return Qt::SizeHorCursor;
    case SizeHandleRect::RightTop:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::RightBottom:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::LeftBottom:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::Bottom:
        return Qt::SizeVerCursor;
    case SizeHandleRect::LeftTop:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::Left:
        return Qt::SizeHorCursor;
    case SizeHandleRect::Top:
        return Qt::SizeVerCursor;
    case SizeHandleRect::SquarePt1:
    case SizeHandleRect::SquarePt2:
    case SizeHandleRect::SquarePt3:
    case SizeHandleRect::SquarePt4:
    case SizeHandleRect::SquarePt5:
    case SizeHandleRect::SquarePt6:
    case SizeHandleRect::SquarePt7:
    case SizeHandleRect::SquarePt8:
        return Qt::SizeAllCursor;

    default:
        break;
    }
    return Qt::ArrowCursor;
}




