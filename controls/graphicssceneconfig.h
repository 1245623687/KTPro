#ifndef GRAPHICSSCENECONFIG_H
#define GRAPHICSSCENECONFIG_H

#include <QObject>
#include<QGraphicsScene>
#include<QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include<ImageHandle/opencvimage.h>
#include"graphicsitemconfig.h"
#include"CheckOperator/CheckOperator.hpp"
#include"sizehandlerect.h"
#include"graphicspixmapitem.h"
#include<QGraphicsTextItem>

class GraphicsSceneConfig : public QGraphicsScene
{
    Q_OBJECT
public:
    enum SelectMode
    {
        none,
        move, //移动
        size, //改变大小
    };
    enum ENUMRECTMODE
    {
        ENUMRECTMODE_ALL=0,
        ENUMRECTMODE_CUR=1
    };
public:
    GraphicsSceneConfig(int camIdx=1);


    void SetBackGroundImage(QPixmap pix, int width, int height);
    virtual void clear();
    ~GraphicsSceneConfig() override;



signals:

    void changeSelectedItemIdx(int idx);

    void sceneDoubleClick(int itemIndex);

    void sceneMouseRelease();

    void sceneClick(int itemIndex);




public:
    void addCheckOperator(CheckOperatorBasic* checkoperator);

    void setRectMode(int mode);

    void deleteItem(int index);

    int selectedIndex();

    void setSelectedItem(int idx);

    void setItemChangeable(bool b);

    void selectRectItem(int idx);

    void setCalcResState(unsigned char arrRes[256]);

    void setText(QString text);




private:
    void setCursor(const QCursor & cursor );

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;

    GraphicsSceneConfig::SelectMode selectMode = GraphicsSceneConfig::none;
    SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;

private:
    QMap<GraphicsItemConfig*,CheckOperatorBasic*> _mapItem2Checkoperator;
    int m_MaxWidth;
    int m_MaxHeight;
    int m_CamIdx;
    bool m_ItemChangeable=true;

    QGraphicsSimpleTextItem * m_ptxtIterm;

    ENUMRECTMODE m_Mode=ENUMRECTMODE_ALL;


};

#endif // GRAPHICSSCENECONFIG_H
