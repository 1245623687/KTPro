#ifndef KPERCENTAGE_H
#define KPERCENTAGE_H

#include <QObject>
#include <QWidget>
#include <QTimeLine>
#include<QLCDNumber>
class KPercentage : public QWidget
{
    Q_OBJECT
public:
    explicit KPercentage(QWidget *parent = nullptr);
    ~KPercentage(){ };

       void setAngle(int);
       void setPercentage(double );

       void setOutLineSubColor(QColor);
       void setOutLineAddColor(QColor);
       void setCircleSubColor(QColor);
       void setCircleAddColor(QColor);
       void setBackgroundColor(QColor);


protected:
    void paintEvent(QPaintEvent*) override;

private:
    void drawBackground(QPainter&);
    void drawOutLine(QPainter&);
    void drawBottomCircle(QPainter&);
    void drawTopCircle(QPainter&);
    void drawPercentage(QPainter&);
    qreal m_angle;//angle/360/16
    QColor m_outLineSubColor=QColor(77,77,77);
    QColor m_outLineAddColor=QColor(67,241,235);
    QColor m_CircleSubColor=QColor(77,77,77);
    QColor m_CircleAddColor=QColor(67,241,235);
    QColor m_backgroundColor=QColor(255,255,255);
    QTimeLine* m_timeLine;


    QLCDNumber* m_LcdPer;
    double m_CurPer=0.00;
};

#endif // KPERCENTAGE_H
