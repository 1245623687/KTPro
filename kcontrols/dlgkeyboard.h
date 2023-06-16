#ifndef DLGKEYBOARD_H
#define DLGKEYBOARD_H

#include <QDialog>
#include<QButtonGroup>
#include"klineeditkb.h"
#include"kspinboxkb.h"
#include"kcontrolsbase.h"
namespace Ui {
class DlgKeyBoard;
}

class DlgKeyBoard : public QDialog
{
    Q_OBJECT

public:

    explicit DlgKeyBoard(KControlsBase* lineEdit,QWidget *parent = nullptr);


    ~DlgKeyBoard();
private:

    void initStyle();
    void initControl();

private slots:
    void btnJudge(int idx);
private:
    Ui::DlgKeyBoard *ui;

private:

    QButtonGroup * m_btnGroup;
   KControlsBase * m_InPut;
};

#endif // DLGKEYBOARD_H
