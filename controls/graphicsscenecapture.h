#ifndef GRAPHICSSCENECAPTURE_H
#define GRAPHICSSCENECAPTURE_H
#include<QGraphicsScene>
#include<QGraphicsItem>
#include"ImageHandle/opencvimage.h"
#include<QPainter>
#include<QGraphicsSceneMouseEvent>




class GraphicsSceneCapture:public QGraphicsScene
{
    Q_OBJECT
private:
    class ItemImage:public QGraphicsItem
    {
    public:
        ItemImage(QImage* image,QGraphicsItem* parent):QGraphicsItem(parent)
        {
            m_ItemImage=image;
        }

    private:

        virtual QRectF boundingRect() const
        {
            return QRect(0,0,m_ItemImage->width(),m_ItemImage->height());
        }
        virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
        {
            painter->drawImage(QRect(0,0,m_ItemImage->width(),m_ItemImage->height()),*m_ItemImage);
        }


    private:

        QImage * m_ItemImage;

    };

public:
    GraphicsSceneCapture(int hei,int wid);

    ~GraphicsSceneCapture(){
        this->clear();
        delete  m_Image;
    }

    void updateImage(OpencvImage& images);


public slots:
    void slotUpdateImage(OpencvImage& images);



protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {

       int x=event->scenePos().x();
       int y=event->scenePos().y();
       if(x<m_Image->width()&&x>=0&&y>=0&&y<m_Image->height())
           emit updatePix(event->scenePos().x(),event->scenePos().y(),m_Image->pixelIndex(event->scenePos().x(),event->scenePos().y()));
       else
           emit updatePix(0,0,0);

       QGraphicsScene::mouseMoveEvent(event);
    }
signals:
    void updatePix(int x,int y,int pix);

private:

    QImage* m_Image;
    ItemImage * m_ItemImage;

};

#endif // GRAPHICSSCENEIMAGE_H
