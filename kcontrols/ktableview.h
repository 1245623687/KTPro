#ifndef KTABLEVIEW_H
#define KTABLEVIEW_H

#include <QObject>
#include<QTableView>
#include<QVector>
class KTableView : public QTableView
{
    Q_OBJECT
public:
    KTableView(QWidget *parent = nullptr);
    void setColumnScaleWidth(QVector<int>& arrWid);
    void setColumnFixWidth(QVector<int>& arrWid);

    ~KTableView() override
    {
        m_WidthFix.clear();
        m_WidthFix.clear();
    }
protected:
    void resizeEvent(QResizeEvent * e) override;


private:
    QVector<int> m_WidthFix;
    QVector<int> m_WidthScale;

};

#endif // KTABLEVIEW_H
