#include "kspinboxkb.h"
#include<QMouseEvent>
#include"dsdebug.h"
#include<QLineEdit>
KSpinBoxKb::KSpinBoxKb(QWidget *parent): QSpinBox(parent),KControlsBase(ENUMKCONTROLTYPE_KSPINBOX)
{
    this->installEventFilter(this);
    this->installEventFilter(this);
    QObjectList o_list = this->children();
    for(int i = 0; i < o_list.length(); i++)
    {
        QLineEdit *cast = qobject_cast<QLineEdit*>(o_list[i]);
        if(cast)
            cast->installEventFilter(this);
    }
}

//void KSpinBoxKb::mousePressEvent(QMouseEvent *e)
//{

//    QSpinBox::mousePressEvent(e);
//}

bool KSpinBoxKb::eventFilter(QObject *obj, QEvent *event)
{

    if (obj->metaObject()->className() == QStringLiteral("QLineEdit"))
    {
        if(event->type()==QEvent::MouseButtonPress)
        {

            emit clicked(this);
        }


        return  QSpinBox::eventFilter(this,event);
    }
    return  QSpinBox::eventFilter(this,event);
}
