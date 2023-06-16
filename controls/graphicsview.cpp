#include "graphicsview.h"
GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    //this->m_InitWidth=640;
    //this->m_InitHeight=480;
    //this->setFixedWidth(m_InitWidth);
    //this->setFixedHeight(m_InitHeight);
    this->setStyleSheet("background: rgb(100,100,100);border:0px");
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHint(QPainter::Antialiasing, true);
    //    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::setScaledEnabled(bool b)
{
    this->m_EnableScale=b;
}

void GraphicsView::setScrollBarVisuable(bool b)
{
    if(!b)
    {
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }

}

void GraphicsView::scaleInit()
{
    if(!m_EnableScale) return;

    this->scale(pow(1.2,-m_Zoom),pow(1.2,-m_Zoom));
    m_Zoom=0;
}

void GraphicsView::scaleMax()
{
    if(!m_EnableScale) return;
    this->scale(this->m_Scale,this->m_Scale);
    m_Zoom++;
}

void GraphicsView::scaleMin()
{
    if(!m_EnableScale) return;
    this->scale(1/this->m_Scale,1/this->m_Scale);
    m_Zoom--;
}

//void GraphicsView::mouseMoveEvent(QMouseEvent *event)
//{
//    QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(this->lastMousePos);
//    if(m_FlagMove)
//    {
//        this->btnMove(mouseDelta);
//    }
//    this->lastMousePos = event->pos();
//    QGraphicsView::mouseMoveEvent(event);
//}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(!m_EnableScale) return;
    switch (event->key())
    {
    case Qt::Key_Plus :
        this->m_Zoom++;
        scale(m_Scale,m_Scale);
        break;
    case Qt::Key_Minus :
        this->m_Zoom--;
        scale(1 /m_Scale, 1 / m_Scale);
        break;
    case Qt::Key_Right :
        //rotate(30);
        break;
    }
    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if(!m_EnableScale) return;
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    //scrollAmount.y() > 0 ? scale(m_Scale,m_Scale): scale(1 / m_Scale, 1 / m_Scale);
    if(scrollAmount.y() > 0 )
    {
        scale(m_Scale,m_Scale) ;
        m_Zoom++;
    }
    else
    {
        scale(1 / m_Scale, 1 / m_Scale);
        m_Zoom--;
    }
}

//void GraphicsView::btnMove(QPointF delta)
//{
//    delta *= this->transform().m11();

//    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    this->centerOn(this->mapToScene(QPoint(this->viewport()->rect().width()/ 2 - delta.x(),
//                                           this->viewport()->rect().height()/ 2 - delta.y())));
//    this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//}

//void GraphicsView::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton)
//        m_FlagMove=true;

//    QGraphicsView::mousePressEvent(event);

//}

//void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton)
//        m_FlagMove=false;
//    QGraphicsView::mouseReleaseEvent(event);
//}
