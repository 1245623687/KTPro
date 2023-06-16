#include "klineeditkb.h"
#include<QMouseEvent>
KLineEditKb::KLineEditKb(QWidget *parent): QLineEdit(parent),KControlsBase(ENUMKCONTROLTYPE_KLINEEDIT)
{

}


void KLineEditKb::mousePressEvent(QMouseEvent *e)
{

    if (e->button() == Qt::LeftButton) {
           //触发clicked信号
           emit clicked(this);
       }
}
