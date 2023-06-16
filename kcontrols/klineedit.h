#ifndef KLINEEDIT_H
#define KLINEEDIT_H

#include<QLineEdit>

class KLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    KLineEdit(QWidget *parent = nullptr);

signals:
    void signalFocusIn();
    void signalFocusOut();

protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
};

#endif // KLINEEDIT_H
