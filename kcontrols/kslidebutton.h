#ifndef KSLIDEBUTTON_H
#define KSLIDEBUTTON_H

#include <QWidget>
#include<QTimer>
class KSlideButton : public QWidget
{
    Q_OBJECT
public:
    explicit KSlideButton(QWidget *parent = nullptr);

private:

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
public:
    void setSilderBtnState(bool b)
    {
        if(ison==b) return;
        ison=b;
         timer->start(3);
    }
    void setGeometry(const QRect & rect)
    {
        widthSize=rect.width();
        heightSize=rect.height();
        //下面是绘制参数相关
        if(ison)
        {
            currentValue=widthSize-0.95*heightSize;
        }
        else
        {
            currentValue=0.05*heightSize;
        }
        QWidget::setGeometry(rect);
    }

    bool isEnabled(){return ison;}
    void setEnableMsg(QString msg)
    {
        this->msg=msg;
    }
signals:
    void enable(bool b);


public slots:
    void begainAnimation();

private:
    bool ison=false;
    float currentValue;
    float widthSize,heightSize;
    QTimer* timer;
    QString msg;
};

#endif // KSLIDEBUTTON_H
