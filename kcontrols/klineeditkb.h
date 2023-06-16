#ifndef KLINEEDITKB_H
#define KLINEEDITKB_H

#include <QObject>
#include<QLineEdit>
#include"kcontrolsbase.h"
class KLineEditKb : public QLineEdit,public KControlsBase
{
    Q_OBJECT
public:
    KLineEditKb(QWidget *parent = nullptr);

protected:
    //添加并重写鼠标点击事件函数
    virtual void mousePressEvent(QMouseEvent *e);
signals:
    //点击信号函数
    void clicked(KControlsBase* pLineEdit);

};

#endif // KLINEEDITKB_H
