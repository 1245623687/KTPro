#ifndef KRUNBUTTON_H
#define KRUNBUTTON_H

#include <QObject>
#include <QWidget>

class KRunButton : public QWidget
{
    Q_OBJECT
public:
    explicit KRunButton(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

private:
    void drawFrame(QPainter &painter);

    int m_refSize; //绘制的参考大小
    int m_radius;
};

#endif // KRUNBUTTON_H
