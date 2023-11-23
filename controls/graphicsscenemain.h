#ifndef GRAPHICSSCENEMain_H
#define GRAPHICSSCENEMain_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMap>
#include<QMutex>
#include"graphicspixmapitem.h"
#include"graphicsitembase.h"
#include"dsdebug.h"
#include<QPainter>
#include"graphicsrectitem.h"
#include"controls/graphicsitemconfig.h"
#include "packagechecker.h"
#include<QGraphicsSimpleTextItem>
#include<QList>
class GraphicsItemMainImage:public QGraphicsItem
{
public:
    GraphicsItemMainImage(unsigned char* pImg,int wid,int hei,int type=1):
        m_Wid(wid),
        m_Hei(hei),
        m_pImage(pImg),m_Type(type)
    {


    }


    void setImagePtr(bool b_IsbadSceen,OpencvImage * ptrOpencvImg)
    {
        m_mutex.lock();
        m_pImage=ptrOpencvImg->ImageRGB.ptr<uchar>(0);
        m_bIsBadSceen=b_IsbadSceen;
        m_ptrOpencvImg=ptrOpencvImg;
        m_mutex.unlock();
    }

    QRectF boundingRect() const override
    {
        return QRectF(0,0,this->m_Wid,this->m_Hei);
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {


        if(m_pImage==nullptr) return;
        if(m_Type==0)
        {
//            if(m_isUpdate)
            {
                QImage img(m_pImage,m_Wid,m_Hei,QImage::Format_Grayscale8);
                painter->drawImage(QRect(0,0,m_Wid,m_Hei),img);

            }

        }

        else if(m_Type==1)
        {
//            if(m_isUpdate)
            {
                QImage img(m_pImage,m_Wid,m_Hei,QImage::Format_RGB888);
                img.save("./img.bmp","BMP");
                painter->drawImage(QRect(0,0,m_Wid,m_Hei),img);
            }
        }


    }

    void isUpdate(bool ret)
    {
        m_isUpdate=ret;
    }


private:
    int m_Wid;
    int m_Hei;
    unsigned char * m_pImage=nullptr;
    int m_Type;
    bool  m_bIsBadSceen=false;
    OpencvImage* m_ptrOpencvImg=NULL;
    QMutex m_mutex;
    bool m_isUpdate=false;;
};



class GraphicsSceneMain : public QGraphicsScene
{
    Q_OBJECT
public:
    //    enum GRAPHICSSCENEMAINMODE
    //    {
    //        GRAPHICSSCENEMAINMODE_HIDE=0,
    //        GRAPHICSSCENEMAINMODE_SHOW=1,

    //        GRAPHICSSCENEMAINMODE_NEED=2
    //    };

public:
    GraphicsSceneMain(unsigned char *pImg,int img_Wid,int img_Hei,int camIdx=1,int type=1);
    void updateShow();
    void setImagePtr(bool  m_bIsBadSceen,OpencvImage* ptrOpencvImg)
    {
        itemImage->setImagePtr(m_bIsBadSceen,ptrOpencvImg);
    }
    //    void setMode(GRAPHICSSCENEMAINMODE mode);
    void updateRectState(unsigned char * pState,bool bIsBadSceem=false);
    void addRect(CheckOperatorBasic * checkoperator);
    void clearRect();

    void addPheTextIterm(int index,QPointF& qpoint);
    void clearPheText();
     void hidePheText();
    void updateTextIterm(int * rets,int* retsMap);



private:
    GraphicsItemMainImage* itemImage=NULL;

    QMap<int,GraphicsItemConfig*> m_RectArr2;
    QMap<int,QGraphicsSimpleTextItem*> m_TextArr;

    //    QMap<int,GraphicsRectItem*> m_RectArr;
    //    GRAPHICSSCENEMAINMODE m_Mode;
    int m_Type;
    int m_CamIdx;
};

#endif // GRAPHICSSCENEMain_H
