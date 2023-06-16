#include "graphicsitemrect.h"
#include<QPainter>
#include"dsdebug.h"
#include"codechecker.h"
GraphicsItemRect::GraphicsItemRect(QRect rect,GraphicsItemBase* parent):
    m_rect(rect),
    GraphicsItemBase(parent)
{
    this->m_MaxHeight=CodeChecker::getInstance()->ImgHei;
     this->m_MaxWidth=CodeChecker::getInstance()->ImgWid;
     m_Selected=false;
    m_handles.reserve(8);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i)
    {
        SizeHandleRect *shr = new SizeHandleRect(this, QRectF(0,0,selection_handle_size,selection_handle_size), static_cast<SizeHandleRect::Direction>(i));
        m_handles.push_back(shr);
    }

    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);
}

void GraphicsItemRect::setState(SizeHandleRect::SelectionHandleState st)
{

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->setState(st);
}

void GraphicsItemRect::setChangeable(bool b)
{
    this->m_IsChangeable=b;
    setFlag(QGraphicsItem::ItemIsMovable, b);
}

QVariant GraphicsItemRect::itemChange(GraphicsItemChange change, const QVariant &value)
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



void GraphicsItemRect::updateGeometry()
{

    const QRectF &rect = this->boundingRect();

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
    {
        SizeHandleRect *hndl = *it;
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

        default:
            break;
        }
    }
}

QRectF GraphicsItemRect::boundingRect() const
{

    return rect();
}

QRectF GraphicsItemRect::rect() const
{
    return QRectF(QPointF(m_rect.left(),m_rect.top()),
                  QPointF(m_rect.right(), m_rect.bottom()));
}


void GraphicsItemRect::resizeTo(SizeHandleRect::Direction dir,int pix)
{
     if(!m_IsChangeable) return;
    QString dirName;
    QRect delta =m_rect.toRect();

    switch (dir)
    {
    case SizeHandleRect::Top:
        dirName = "Top";
        delta.setTop(delta.top()+pix);
        break;
    case SizeHandleRect::Left:
        dirName = "Left";
        delta.setLeft(delta.left()+pix);
        break;
    case SizeHandleRect::Right:
        dirName = "Rigth";
        delta.setRight(delta.right()+pix);
        break;
    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        delta.setBottom(delta.bottom()+pix);
        break;
    }
    prepareGeometryChange();
    m_rect =delta;
    updateGeometry();
}

void GraphicsItemRect::resizeTo(SizeHandleRect::Direction dir, const QPointF & point )
{
     if(!m_IsChangeable) return;
    QPointF local = mapFromScene(point);
    QString dirName;
    QRect delta =m_rect.toRect();

    switch (dir)
    {
    case SizeHandleRect::LeftTop:
        delta.setTopLeft(local.toPoint());
        break;
    case SizeHandleRect::Top:
        dirName = "Top";
        delta.setTop(local.y());
        break;
    case SizeHandleRect::RightTop:
        dirName = "RightTop";
        delta.setTopRight(local.toPoint());
        break;
    case SizeHandleRect::Left:
        dirName = "Left";
        delta.setLeft(local.x());
        break;
    case SizeHandleRect::Right:
        dirName = "Rigth";
        delta.setRight(local.x());
        break;
    case SizeHandleRect::LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(local.toPoint());
        break;
    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        delta.setBottom(local.y());
        break;
    case SizeHandleRect::RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(local.toPoint());
        break;
    default:
        break;
    }
    prepareGeometryChange();
    m_rect =delta;
    updateGeometry();

}

void GraphicsItemRect::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen pen =painter->pen();
    pen.setWidth(2);
    pen.setColor(QColor(0,255,0));
    pen.setStyle(m_Selected?Qt::SolidLine:Qt::DotLine);
    painter->setPen(pen);
    painter->drawRect(rect());

}


SizeHandleRect::Direction  GraphicsItemRect::hitTest( const QPointF & point ) const
{
    const Handles::const_iterator hend =  m_handles.end();
    for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
    {
        if ((*it)->hitTest(point) )
        {
            return (*it)->dir();
        }
    }
    return SizeHandleRect::None;
}


void GraphicsItemRect::updateRect()
{
    prepareGeometryChange();
//    DSDEBUG<<m_rect;
    QPoint ptLeftUp=QPoint(this->rect().x(),this->rect().y());
    QPoint ptRightDown=QPoint(this->rect().x()+rect().width(),this->rect().y()+rect().height());
    QPoint ptLeftDown=QPoint(this->rect().x(),this->rect().y()+rect().height());
    QPoint ptRightUp=QPoint(this->rect().x()+rect().width(),this->rect().y());
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
    this->m_rect=QRect(0,0,qMin((int)m_MaxWidth,(int)qAbs(rect().width())),qMin((int)m_MaxHeight,(int)qAbs(rect().height())));


    this->setPos( qMax(0,qMin(qRound(m_MaxWidth-this->rect().width()),qRound(this->scenePos().x()))),
                  qMax(0,qMin(qRound(m_MaxHeight-this->rect().height()),qRound(this->scenePos().y()))));





//    DSDEBUG<<m_rect;
    updateGeometry();
    this->update();


}

Qt::CursorShape GraphicsItemRect::getCursor(SizeHandleRect::Direction dir )
{
    switch (dir)
    {
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
    default:
        break;
    }
    return Qt::ArrowCursor;

}
