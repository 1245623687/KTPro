#ifndef DLGBASE_H
#define DLGBASE_H

#include <QObject>
#include"kcontrols/kcontrolsbase.h"
class DlgBase:public QObject
{
    Q_OBJECT
public:
    DlgBase();

protected slots:

    void showKeyBoard(KControlsBase* pLineEdit);



};

#endif // DLGBASE_H
