#include "graphicssinglelineitem.h"
#include<QPainter>
#include"dsdebug.h"
GraphicsSingleLineItem::GraphicsSingleLineItem(QLine line,QGraphicsItem * parent):
    QGraphicsItem(parent)
{


    m_SizeHandle[0]=new  LineHandleRect( QRectF(0,0,8,8),LineHandleRect::PT1,this);
    m_SizeHandle[1]=new  LineHandleRect( QRectF(0,0,8,8),LineHandleRect::PT2,this);

    m_SizeHandle[0]->setParentItem(this);
    m_SizeHandle[1]->setParentItem(this);
    this->m_line=line;

    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);
    this->m_Selected=false;
}

QPainterPath GraphicsSingleLineItem::shape() const
{
    //DSDEBUG<<"SHAPE";
    QPainterPath path;
//    path.addEllipse(boundingRect());
    QVector<QPointF> vPts;
    vPts.push_back(QPointF(this->m_line.x1()-3,this->m_line.y1()));
    vPts.push_back(QPointF(this->m_line.x1()+3,this->m_line.y1()));
    vPts.push_back(QPointF(this->m_line.x2()+3,this->m_line.y2()));
    vPts.push_back(QPointF(this->m_line.x2()-3,this->m_line.y2()));
    QPolygonF polygon(vPts);
    path.addPolygon(polygon);
    return path;
}

  void GraphicsSingleLineItem::resizeTo(LineHandleRect::Direction dir, const QPointF & point )
  {

     // qDebug()<<"enter resizeto"<<endl;
      QPointF local = mapFromScene(point);
      QString dirName;

      switch (dir)
      {
      case LineHandleRect::PT1:
          this->m_line.setP1(QPoint(local.x(),local.y()));
          break;
       case LineHandleRect::PT2:
          this->m_line.setP2(QPoint(local.x(),local.y()));
          break;
      default:
          break;
      }
      prepareGeometryChange();

      updateGeometry();
  }


LineHandleRect::Direction  GraphicsSingleLineItem::hitTest( const QPointF & point ) const
{
    for (int i=0;i< 2; i++)
    {
        if (m_SizeHandle[i]->hitTest(point) )
        {
            return m_SizeHandle[i]->dir();
        }
    }
}


void GraphicsSingleLineItem::updateGeometry()
{
   // DSDEBUG<<"X:"<<this->m_line.p1().x()<<"Y:"<<this->m_line.p1().y()<<endl;
   // DSDEBUG<<"X2:"<<this->m_line.p2().x()<<"Y2:"<<this->m_line.p2().y()<<endl;


    this->m_SizeHandle[0]->move(this->m_line.p1().x()-this->m_SizeHandle[0]->rect().width()/2+1,this->m_line.p1().y()-this->m_SizeHandle[0]->rect().height()/2);
    this->m_SizeHandle[1]->move(this->m_line.p2().x()-this->m_SizeHandle[1]->rect().width()/2+1,this->m_line.p2().y()-this->m_SizeHandle[1]->rect().height()/2);
}


GraphicsSingleLineItem::GraphicsSingleLineItem(int x1,int y1,int x2,int y2,QGraphicsItem * parent):
    QGraphicsItem(parent)
{
    m_SizeHandle[0]=new  LineHandleRect( QRectF(0,0,8,8),LineHandleRect::PT1,this);
    m_SizeHandle[1]=new  LineHandleRect( QRectF(0,0,8,8),LineHandleRect::PT2,this);

    m_SizeHandle[0]->setParentItem(this);
    m_SizeHandle[1]->setParentItem(this);
    this->m_line.setP1(QPoint(x1,y1));
    this->m_line.setP2(QPoint(x2,y2));
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);

}

QVariant GraphicsSingleLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    //qDebug()<<"enter itemChange"<<endl;
   // DSDEBUG<<"enter itemChange"<<endl;
    if ( change == QGraphicsItem::ItemSelectedHasChanged )
    {
        //qDebug()<<" Item Selected : " << value.toString();
        //setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
         DSDEBUG<<"enter itemChange: this->m_Selected:"<< this->m_Selected<<endl;
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



QRectF GraphicsSingleLineItem::boundingRect() const
{
    //DSDEBUG<<"boundingRect";

    int leftx=m_line.x1()<=m_line.x2()?m_line.x1():m_line.x2();
    int rightx=m_line.x1()<=m_line.x2()?m_line.x2():m_line.x1();
    int upy=m_line.y1()<=m_line.y2()?m_line.y1():m_line.y2();
    int downy=m_line.y1()<=m_line.y2()?m_line.y2():m_line.y1();
    int startx=leftx-m_SizeHandle[0]->rect().width()/2;
    int starty=upy-m_SizeHandle[0]->rect().height()/2;
    int width=rightx-leftx+this->m_SizeHandle[0]->rect().width();
    int height=downy-upy+this->m_SizeHandle[0]->rect().height();

    return QRect(startx,starty,width,height);
}

void GraphicsSingleLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(QColor(0,0,255));
    //DSDEBUG<<"SELECT:"<<m_Selected<<endl;
    pen.setStyle(m_Selected?Qt::DotLine:Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(this->m_line);
    updateGeometry();
}
