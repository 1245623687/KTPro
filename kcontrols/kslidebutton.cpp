#include "kslidebutton.h"
#include<QPainter>
#include"qtCommon/frmmessagebox.h"
#include"dsdebug.h"
KSlideButton::KSlideButton(QWidget *parent) : QWidget(parent)
{  
    //     widthSize=300;
    //     heightSize=100;

    timer=new QTimer(this);
    timer->setInterval(50);
    connect(timer,SIGNAL(timeout()),this,SLOT(begainAnimation()));
    if(ison)
    {
        currentValue=widthSize-0.95*heightSize;
    }
    else
    {
        currentValue=0.05*heightSize;
    }
}


void KSlideButton::resizeEvent(QResizeEvent *event)
{


    widthSize=this->geometry().width();
    heightSize=this->geometry().height();
    if(ison)
    {
        currentValue=widthSize-0.95*heightSize;
    }
    else
    {
        currentValue=0.05*heightSize;
    }
    QWidget::resizeEvent(event);
}


void KSlideButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!ison)
    {
        if(msg!="")
        {
            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage(this->msg, 1);
            if(!msg->exec()) return;
        }

    }
    ison=!ison;
    timer->start(3);
}

void KSlideButton::begainAnimation(){
    int i=0.05*heightSize;
    int n=widthSize-0.95*heightSize;
    if(ison)
    {
        currentValue+=1;
        if(currentValue>n-i)
        {
            timer->stop();
            emit enable(ison);
        }
    }
    else
    {
        currentValue-=1;
        if(currentValue<i)
        {
            timer->stop();
            emit enable(ison);
        }
    }
    update();

}

void KSlideButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)


    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    if(ison)
    {
        painter.save();
        painter.setBrush(QColor(102, 255, 102));
        QRectF greenRect=QRectF(0,0,widthSize,heightSize);
        painter.drawRoundedRect(greenRect,0.5*heightSize,0.5*heightSize);
        painter.restore();
        painter.save();
        painter.setBrush(Qt::white);
        painter.drawEllipse(currentValue,0.05*heightSize,0.9*heightSize,0.9*heightSize);
        painter.restore();
    }
    else
    {
        painter.save();
        QColor grayColor(199,199,199);
        painter.setBrush(grayColor);
        QRectF roundRect=QRectF(0,0,widthSize,heightSize);
        painter.drawRoundedRect(roundRect,0.5*heightSize,0.5*heightSize);
        painter.restore();

        painter.save();
        painter.setBrush(Qt::lightGray);
        QRectF redRect=QRectF(heightSize*0.05,heightSize*0.05,widthSize-heightSize*0.1,heightSize*0.9);
        painter.drawRoundedRect(redRect,0.45*heightSize,0.45*heightSize);

        painter.restore();
        painter.save();
        painter.setBrush(Qt::white);
        painter.drawEllipse(currentValue,0.05*heightSize,0.9*heightSize,0.9*heightSize);
        painter.restore();
    }

}
