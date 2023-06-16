#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = 0);
    void scaleInit();
    void scaleMax();void scaleMin();
//    void btnMove(QPointF delta);
    void setScaledEnabled(bool b);
    void setScrollBarVisuable(bool b);

    void scaleto(qreal sx, qreal sy)
    {

        this->scale(1/m_InitScaleX,1/m_InitScaleY);
        //DSDEBUG<<QString("scaleto: %1,%2").arg(sx).arg(sy);
        this->m_InitScaleX=sx;
        this->m_InitScaleY=sy;
        this->scale(m_InitScaleX,m_InitScaleY);
    }

signals:
    void infoCoordinate(int x,int y);

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event)
    {
        QGraphicsView::mouseDoubleClickEvent(event);
    }

   virtual void mouseMoveEvent(QMouseEvent *event) override
    {

        emit infoCoordinate(this->mapToScene(event->pos()).x(),this->mapToScene(event->pos()).y());
        QGraphicsView::mouseMoveEvent(event);
    }

//    virtual void mousePressEvent(QMouseEvent *event) override;
//    virtual void mouseReleaseEvent(QMouseEvent *event) override;



private:
    double m_InitHeight;
    double m_InitWidth;

    int m_Zoom=0;
    double m_Scale=1.2;
    bool m_EnableScale=true;

    qreal m_InitScaleX=1.0;
    qreal m_InitScaleY=1.0;

};

#endif // GRAPHICSVIEW_H
