#ifndef KSPINBOXKB_H
#define KSPINBOXKB_H

#include <QObject>
#include<QSpinBox>
#include"kcontrolsbase.h"

class KSpinBoxKb : public QSpinBox,public KControlsBase
{
      Q_OBJECT
public:
    KSpinBoxKb(QWidget *parent = nullptr);


protected:
    //添加并重写鼠标点击事件函数
//    virtual void mousePressEvent(QMouseEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);


signals:
    //点击信号函数
    void clicked(KControlsBase* pKSpinBoxKb);
};

#endif // KSPINBOXKB_H
