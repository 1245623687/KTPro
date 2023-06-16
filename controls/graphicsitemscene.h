#ifndef GRAPHICSITEMSCENE_H
#define GRAPHICSITEMSCENE_H

#include<QGraphicsScene>
#include<QGraphicsView>
#include"controls/graphicssinglelineitem.h"
#include"controls/dsgraphicsitemimage.hpp"
class GraphicsItemScene : public QGraphicsScene
{
    public:
    enum SelectMode
    {
        none,
        move,
        size //改变大小

    };

public:



    GraphicsItemScene();
    void SetBackGroundImage(QPixmap pix, int width, int height);
    void setBackGroundImage(DSGraphicsItemImage<OpencvImage>* image);
    void setCursor(const QCursor &cursor);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;

};

#endif // GRAPHICSITEMSCENE_H
