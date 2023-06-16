#ifndef KLABEL_H
#define KLABEL_H

#include <QObject>
#include<QLabel>
class KLabel : public QWidget
{
public:
    KLabel(QWidget* parent=nullptr);
    void setText(QString text)
    {
        m_Info=text;
    }
    void setstate(bool b)
    {
        m_IsNg=b;
        update();
    }

    QPoint CustomRotate(QPointF point,qreal from_angle,qreal rotate);
    void paintEvent(QPaintEvent * event) override;

private:
    QString m_Info;
    bool m_IsNg=false;
};

#endif // KLABEL_H
