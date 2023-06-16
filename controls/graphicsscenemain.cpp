#include "graphicsscenemain.h"
#include <QGraphicsView>
#include<QToolTip>
#include<QGraphicsLineItem>
#include"dsdebug.h"
#include<QGraphicsRectItem>
#include"PackageChecker.h"


GraphicsSceneMain::GraphicsSceneMain(unsigned char *pImg,int img_Wid,int img_Hei,int camidx,int type):m_Type(type)
{
    m_CamIdx=camidx;
    itemImage=new GraphicsItemMainImage(pImg,img_Wid,img_Hei);
    this->addItem(itemImage);
    update();
    //    this->m_Mode=GRAPHICSSCENEMAINMODE_NEED;
}

void GraphicsSceneMain::updateShow()
{
    this->update();
}

void GraphicsSceneMain::updateRectState(unsigned char * pState,bool bIsBadSceen)
{

    switch (PackageChecker::getInstance()->Options->DisplayType())
    {
    case  ENUMDISPLAYTYPE_NODISPLAY:
    {
        //        QMap<int,GraphicsRectItem*>::iterator itor= m_RectArr.begin();
        //        for (;itor!=m_RectArr.end();++itor)
        //        {
        //            itor.value()->hide();
        //        }

        QMap<int,GraphicsItemConfig*>::iterator itor= m_RectArr2.begin();
        for (;itor!=m_RectArr2.end();++itor)
        {
            itor.value()->hide();
        }
    }
        break;
    case  ENUMDISPLAYTYPE_ALLAREA:
    {

        QMap<int,GraphicsItemConfig*>::iterator itor= m_RectArr2.begin();

        for (;itor!=m_RectArr2.end();++itor)
        {
                if(pState[itor.key()]==1)
                {
                    itor.value()->setCalcState(1);
                    itor.value()->show();
                }
                if(pState[itor.key()]==0)
                {
                    if(bIsBadSceen)
                    {
                        itor.value()->setCalcState(0);
                        itor.value()->hide();
                    }
                    else
                    {
                        itor.value()->setCalcState(0);
                        itor.value()->show();
                    }

                }
                if(pState[itor.key()]==2)
                {
                    itor.value()->setCalcState(2);
                    itor.value()->show();
                }


        }
    }
        break;
    case  ENUMDISPLAYTYPE_DEFECTAREA:
    {
        //        QMap<int,GraphicsRectItem*>::iterator itor= m_RectArr.begin();
        //        for (;itor!=m_RectArr.end();++itor)
        //        {
        //            if(pState[itor.key()]==1)
        //                itor.value()->show();
        //            else
        //                itor.value()->hide();
        //        }


        QMap<int,GraphicsItemConfig*>::iterator itor= m_RectArr2.begin();
        for (;itor!=m_RectArr2.end();++itor)
        {
            if(pState[itor.key()]==1)
                itor.value()->show();

            else
                itor.value()->hide();
        }
    }
        break;
    }
}

//void GraphicsSceneMain::setMode(GRAPHICSSCENEMAINMODE mode)
//{
//    this->m_Mode=mode;

//}

void GraphicsSceneMain::clearRect()
{
    //    QMap<int,GraphicsRectItem*>::iterator itor= m_RectArr.begin();
    //    for (;itor!=m_RectArr.end();++itor)
    //    {
    //        this->removeItem(itor.value());
    //        delete itor.value();
    //    }

    //    m_RectArr.clear();

    QMap<int,GraphicsItemConfig*>::iterator itor= m_RectArr2.begin();
    for (;itor!=m_RectArr2.end();++itor)
    {
        this->removeItem(itor.value());
        delete itor.value();
    }

    m_RectArr2.clear();
}

void GraphicsSceneMain::addPheTextIterm(int index,QPointF& qpoint)
{
    QGraphicsSimpleTextItem *newIterm=new QGraphicsSimpleTextItem();
    QFont font("Times", 28, QFont::Thin);
    newIterm->setFont(font);
    newIterm->setPos(qpoint);
    newIterm->setBrush(QBrush(QColor(255,255,0)));
     newIterm->setText("0");
//     newIterm->setZValue(128);
     this->addItem(newIterm);

    m_TextArr.insert(index,newIterm);


}

void GraphicsSceneMain::clearPheText()
{
    m_TextArr.clear();
}

void GraphicsSceneMain::updateTextIterm(int * rets)
{
    for(int i=0;i<m_TextArr.size();i++)
    {
        m_TextArr[i+1]->setText(QString::number(rets[i]));

        if(static_cast<int>(PackageChecker::getInstance()->Options->ephDisplay()) )
        {
            m_TextArr[i+1]->show();
        }
        else
        {
            m_TextArr[i+1]->hide();
        }
    }
}



void GraphicsSceneMain::addRect(CheckOperatorBasic * checkoperator)
{

    //    GraphicsRectItem* newItem=new GraphicsRectItem(rect,nullptr);
    //    this->addItem(newItem);
    //    newItem->setPos(rect.x(),rect.y());
    //    m_RectArr[idx]=newItem;

    QRect rect(0,
               0,
               checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
               checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ys);
    QPoint rectPos(checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                   checkoperator->stu_CheckOperatorBasic->Basic_RectValid_Ys);
    int index_i=checkoperator->stu_CheckOperatorBasic->Index;
    switch (checkoperator->CheckType) {
    case  ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    case  ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    {
        GraphicsItemConfig* newItem=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTR);
        this->addItem(newItem);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsSelectable);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsMovable);
        newItem->setCalcState(GraphicsItemConfig::ENUMSTATE_NG);
        newItem->setPos(rectPos);

        m_RectArr2[index_i]=newItem;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT :
    case  ENUMCHECKOPERATORTYPE_CIRCLEINSPECT :
    case  ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT :
    case  ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT :
    case  ENUMCHECKOPERATORTYPE_LINEINSPECT:
    case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
    case  ENUMCHECKOPERATORTYPE_STAYINSPECT:
    case  ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT :
    case  ENUMCHECKOPERATORTYPE_DLOBJECTDETECT :
    {
        GraphicsItemConfig* newItem=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_RECTC);
        this->addItem(newItem);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsSelectable);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsMovable);
        newItem->setCalcState(GraphicsItemConfig::ENUMSTATE_NG);
        newItem->setPos(rectPos);

        m_RectArr2[index_i]=newItem;
    }
        break;
    case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
    {
        CheckOperatorDefectInspect2* pCheckOperatorDefectInspect2=static_cast<CheckOperatorDefectInspect2*>(checkoperator);
        QVector<QPoint> pts;
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X1,pCheckOperatorDefectInspect2->Y1));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X2,pCheckOperatorDefectInspect2->Y2));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X3,pCheckOperatorDefectInspect2->Y3));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X4,pCheckOperatorDefectInspect2->Y4));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X5,pCheckOperatorDefectInspect2->Y5));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X6,pCheckOperatorDefectInspect2->Y6));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X7,pCheckOperatorDefectInspect2->Y7));
        pts.push_back(QPoint(pCheckOperatorDefectInspect2->X8,pCheckOperatorDefectInspect2->Y8));


        GraphicsItemConfig* newItem=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_SQUARE,pts);
        this->addItem(newItem);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsSelectable);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsMovable);
        newItem->setCalcState(GraphicsItemConfig::ENUMSTATE_NG);
        newItem->setPos(rectPos);
        m_RectArr2[index_i]=newItem;
    }
        break;

    case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
    {
        CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2=static_cast<CheckOperatorAssociatedInspect2*>(checkoperator);
        QVector<QPoint> pts;
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X1,pCheckOperatorAssociatedInspect2->Y1));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X2,pCheckOperatorAssociatedInspect2->Y2));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X3,pCheckOperatorAssociatedInspect2->Y3));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X4,pCheckOperatorAssociatedInspect2->Y4));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X5,pCheckOperatorAssociatedInspect2->Y5));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X6,pCheckOperatorAssociatedInspect2->Y6));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X7,pCheckOperatorAssociatedInspect2->Y7));
        pts.push_back(QPoint(pCheckOperatorAssociatedInspect2->X8,pCheckOperatorAssociatedInspect2->Y8));


        GraphicsItemConfig* newItem=new GraphicsItemConfig(rect,index_i,nullptr,m_CamIdx,GraphicsItemConfig::ENUMRECTTYPE_SQUARE,pts);
        this->addItem(newItem);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsSelectable);
        newItem->setFlags(newItem->flags()&~QGraphicsItem::ItemIsMovable);
        newItem->setCalcState(GraphicsItemConfig::ENUMSTATE_NG);
        newItem->setPos(rectPos);
        m_RectArr2[index_i]=newItem;
    }
        break;

    }

}
