#ifndef GRAPHICSSCENECHECKOPERATORMAIN_H
#define GRAPHICSSCENECHECKOPERATORMAIN_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include<QGraphicsScene>
#include<ImageHandle/opencvimage.h>
#include"graphicscheckoperatoritem.h"
#include"CheckOperator/CheckOperator.hpp"
class GraphicsSceneCheckOperatorMain : public QGraphicsScene
{

    Q_OBJECT
public:
    enum SelectMode
    {
        none,
        netSelect,
        move, //移动
        size, //改变大小
        rotate, //反转
        linesize//线条改变
    };

    enum ENUMRECTMODE
    {
        ENUMRECTMODE_ALL=0,
        ENUMRECTMODE_CUR=1
    };

public:
    GraphicsSceneCheckOperatorMain();



    void createRect();

    void createRect(int checkNo,QRect rect,GraphicsCheckOperatorItem::ENUMRECTTYPE type=GraphicsCheckOperatorItem::ENUMRECTTYPE_RECTR,bool modify=true,const QVector<QPoint>& pts=QVector<QPoint>());

    void SetBackGroundImage(QPixmap pix, int width, int height);

    void selectRectItem(int idx);

    void clearRectItem();

    void setRectMode(int mode);

    void setItemModified(int idx,bool modify);

    void changeItem(int index);

    void deleteItem(int index);

    void setBackGroundImage(OpencvImage image);

    void setCalcResState(unsigned char arrRes[256]);

    void clear();

    ~GraphicsSceneCheckOperatorMain();

public:
    QMap<int,GraphicsCheckOperatorItem*> m_mapRectItem;
private:
    QMap<QGraphicsItem*,CheckOperatorBasic*> _mapItem2Checkoperator;

signals:
    void sceneDoubleClick(int itemIndex);

    void sceneMouseRelease();

    void sceneClick(int itemIndex);

    void infoCoordinate(QPointF point);

private:
    void setCursor(const QCursor & cursor );

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    ENUMRECTMODE m_Mode=ENUMRECTMODE_ALL;



};

#endif // GRAPHICSSCENECHECKOPERATORMAIN_H
