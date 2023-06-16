#include "klineedit.h"

KLineEdit::KLineEdit(QWidget *parent):
    QLineEdit(parent)
{

}
void KLineEdit::focusInEvent(QFocusEvent *e)
{
    emit signalFocusIn();
    QLineEdit::focusInEvent(e);
}

void KLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit signalFocusOut();
    QLineEdit::focusOutEvent(e);
}
