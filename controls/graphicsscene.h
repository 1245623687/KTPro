#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include"graphicspixmapitem.h"
class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene();


    void SetBackGroundImage(QPixmap pix, int width, int height);


    ~GraphicsScene()
    {
        this->clear();
    }

//    void getPixInfo(int x,int y,int * pixel);
private:
    int m_MaxWidth;
    int m_MaxHeight;
};

#endif // GRAPHICSSCENE_H
