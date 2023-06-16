#ifndef DSGRAPHICSITEMIMAGE_H
#define DSGRAPHICSITEMIMAGE_H


#include<QGraphicsItem>
#include"CheckOperator/ImageBase.hpp"
#include<QPainter>
#include<QImage>
#include"dsdebug.h"
#include<QGraphicsTextItem>
#include<QVector>
template<class ImageType>
class DSGraphicsItemImage : public QGraphicsItem
{
public:
    enum ENUMTEXTSTATE
    {
        ENUMTEXTSTATE_OK=0,
        ENUMTEXTSTATE_NG=1
    };

private:
    class GraphicsTextItem:public QGraphicsTextItem
    {

    private:
        ENUMTEXTSTATE m_State;

    public:
        GraphicsTextItem(const QString & text,QGraphicsItem *parent = nullptr):QGraphicsTextItem(text,parent)
        {
            this->setTextWidth(50);
            this->setFont(QFont("SimSun", 25));
            m_State=ENUMTEXTSTATE_OK;
        }
        GraphicsTextItem(QGraphicsItem *parent = nullptr):QGraphicsTextItem(parent)
        {
            this->setTextWidth(50);
            this->setFont(QFont("SimSun", 25));
            m_State=ENUMTEXTSTATE_OK;
        }
        void setItemText(const QString& text)
        {
            this->setPlainText(text);
        }


        void setTextState(int state)
        {
            ENUMTEXTSTATE s=static_cast<ENUMTEXTSTATE>(state);
            this->m_State=s;

        }

    protected:

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
        {
            switch (this->m_State) {
            case  ENUMTEXTSTATE_OK:
                painter->setBrush(Qt::green);
                break;
            case  ENUMTEXTSTATE_NG:
                painter->setBrush(Qt::red);
                break;

            }

            painter->drawRect(boundingRect());
            QGraphicsTextItem:: paint(painter,option,widget);
        }

    };

public:


    DSGraphicsItemImage()
    {
        m_pQImage=nullptr;
        m_TextItem=new GraphicsTextItem("");
        m_TextItem->setY(-60);
        m_TextItem->setParentItem(this);
        m_TextItem->hide();
    }

    DSGraphicsItemImage(ImageType& image,int iWidth,int iHeight,int idx=0):m_Image(image),m_Width(iWidth),m_Height(iHeight)
    {
        m_pQImage=nullptr;
        QString typeName=typeid(ImageType).name();
        if(typeName=="class OpencvImage")
        {
            switch (image.Format)
            {
            case  ENUMDSIMAGEFORMAT_GRAY:

                m_pQImage=new QImage(iWidth,iHeight,QImage::Format_Indexed8);
                image.toQImage(m_pQImage);
                break;
            case ENUMDSIMAGEFORMAT_RGB:
                m_pQImage=new QImage(iWidth,iHeight,QImage::Format_RGB888);
                image.toQImage(m_pQImage);
                break;
            default:
                m_pQImage=new QImage(iWidth,iHeight,QImage::Format_RGB888);
                image.toQImage(m_pQImage);
                break;
            }

        }
        m_TextItem=new GraphicsTextItem("");
        m_TextItem->setParentItem(this);
        m_TextItem->hide();

        m_EdgeLines.clear();
        m_EdgeLines.push_back(QPoint(0,0));
        m_EdgeLines.push_back(QPoint(m_pQImage->width(),0));
        m_EdgeLines.push_back(QPoint(m_pQImage->width(),m_pQImage->height()));
        m_EdgeLines.push_back(QPoint(0,m_pQImage->height()));
        m_EdgeLines.push_back(QPoint(0,0));

    }


    void setEdgeLine(QVector<QPoint>& vPts)
    {
        if(vPts.size()<4) return;
        this->m_EdgeLines.clear();
        for(int i=0;i<vPts.size();++i)
        {
            this->m_EdgeLines.push_back(vPts[i]);
        }
        this->m_EdgeLines.push_back(vPts[0]);
        //DSDEBUG<<this->m_EdgeLines.size();
    }

    int type() const
    {
        return 888;
    }
    //    DSGraphicsItemImage<ImageType>* GetRectImage(QRect rect)
    //    {

    //        QString typeName=typeid(ImageType).name();
    //        if(typeName=="class OpenCvImage")
    //        {
    //            switch (this->m_Image.Format)
    //            {
    //            case  ENUMDSIMAGEFORMAT_GRAY:




    //                break;

    //            }

    //        }

    //    }

    QRectF boundingRect() const override
    {
        return QRectF(0,0,this->m_Width,this->m_Height);
    }


    void setState(int state)
    {
        this->m_TextItem->setTextState(state);
    }

    void setShowInfoEnable(bool b)
    {
        if(b) m_TextItem->show();
        else m_TextItem->hide();
    }

    void setInfo(const QString& text )
    {
        m_TextItem->setItemText(text);
    }



    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        if(m_pQImage==nullptr) return;
        //        painter->drawImage(QRect(-this->m_pQImage->width()/2,-this->m_pQImage->height()/2,this->m_pQImage->width(),this->m_pQImage->height()),*this->m_pQImage);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter->drawImage(QRect(0,0,this->m_pQImage->width(),this->m_pQImage->height()),*this->m_pQImage);

        QPen pen;
        pen.setColor(QColor(200,200,200));
        painter->setPen(pen);
        //painter->drawRect(QRect(0,0,this->m_pQImage->width(),this->m_pQImage->height()));
//        if(this->m_EdgeLines.size())
//            painter->drawPolyline(this->m_EdgeLines);

    }

    void setIndex(int idx)
    {
        this->m_Idx=idx;
        setInfo(QString("相机%1").arg(idx));
    }

    int Index()
    {
        return this->m_Idx;
    }

    void reLoadImage(ImageType& image,int iWidth,int iHeight)
    {

        QString typeName=typeid(ImageType).name();
        if(typeName=="class OpencvImage")
        {
            if(m_pQImage==nullptr)
            {

#ifdef IMG_TYPE_GRAY
                  m_pQImage=new QImage(iWidth,iHeight,QImage::Format_Grayscale8);
#endif
#ifdef IMG_TYPE_RGB
                    m_pQImage=new QImage(iWidth,iHeight,QImage::Format_RGB888);
#endif
            }
            image.toQImage(m_pQImage);
//            m_pQImage->save("D:/2.bmp");

            this->m_Width=iWidth;
            this->m_Height=iHeight;

            if(m_EdgeLines.size()<4)
            {
                m_EdgeLines.push_back(QPoint(0,0));
                m_EdgeLines.push_back(QPoint(m_pQImage->width(),0));
                m_EdgeLines.push_back(QPoint(m_pQImage->width(),m_pQImage->height()));
                m_EdgeLines.push_back(QPoint(0,m_pQImage->height()));
                m_EdgeLines.push_back(QPoint(0,0));
            }
        }
        this->update();
    }



    int getHeight()
    {
        return this->m_Height;
    }

    int getWidth()
    {
        return this->m_Width;
    }

    ImageType& getImage()
    {
        return this->m_Image;
    }

    ~DSGraphicsItemImage()
    {
        if(m_TextItem!=nullptr)
        {
            delete  m_TextItem;
        }
        if(m_pQImage!=nullptr)
        {delete  m_pQImage;}
    }
    QImage* m_pQImage;//存放要显示的运算结果图像;
private:
    int m_Height;
    int m_Width;
    ImageType m_Image;

    int m_Idx;
    GraphicsTextItem * m_TextItem;
    QVector<QPoint> m_EdgeLines;
};

#endif // DSGRAPHICSITEMIMAGE_H
