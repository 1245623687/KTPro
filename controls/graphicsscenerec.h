#ifndef GRAPHICSSCENEREC_H
#define GRAPHICSSCENEREC_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMap>
#include"graphicspixmapitem.h"
#include"sizehandlerect.h"
#include"graphicsitembase.h"
#include"dsdebug.h"
#include"controls/graphicsqimageitem.h"
#include<QGraphicsItem>
#include<QPainter>
#include<QImage>
#include"packagechecker.h"
class GraphicsSceneRec : public QGraphicsScene
{
    Q_OBJECT
private:
    class GraphicsCalcItem:public QGraphicsItem
    {
    public:
        GraphicsCalcItem(int hei,int wid,unsigned char* pImg,QGraphicsItem* parent=nullptr):
            QGraphicsItem(parent)
        {
            m_Hei=hei;
            m_Wid=wid;
            m_pImg=pImg;


#ifdef IMG_TYPE_GRAY
             m_QImage=new QImage(wid,hei,QImage::Format_Grayscale8);
#endif
#ifdef IMG_TYPE_RGB
              m_QImage=new QImage(wid,hei,QImage::Format_RGB888);
#endif
        }

        ~GraphicsCalcItem()
        {
            if(m_QImage!=nullptr)
            {
                delete  m_QImage;
            }
        }

        void updatethis()
        {
            for (int row=0;row<m_Hei;++row)
            {

#ifdef IMG_TYPE_GRAY
                memcpy(m_QImage->scanLine(row),m_pImg+m_Wid*row,m_Wid);
#endif
#ifdef IMG_TYPE_RGB
                memcpy(m_QImage->scanLine(row),m_pImg+m_Wid*row*3,m_Wid*3);
#endif

            }
        }

        virtual QRectF boundingRect() const{return QRectF(0,0,m_Wid,m_Hei);}


        virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
        {
            painter->drawImage(QRect(0,0,m_Wid,m_Hei),*m_QImage);
        }


    private:
        int m_Wid;
        int m_Hei;
        unsigned char* m_pImg;
        QImage* m_QImage=nullptr;

    };




public:
    GraphicsSceneRec(int width, int height,unsigned char* pImgCalc);

    void updatethis()
    {
        m_CalcItem->updatethis();
    }
private:
    void SetBackGroundImage(QPixmap pix, int width, int height);

public:
    virtual void clear();
    ~GraphicsSceneRec() override;

signals:

public slots:




private:

    int m_MaxWidth;
    int m_MaxHeight;

    GraphicsCalcItem * m_CalcItem=nullptr;

};

#endif // GRAPHICSSCENE_H
