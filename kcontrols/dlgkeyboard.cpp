#include "dlgkeyboard.h"
#include "ui_dlgkeyboard.h"
#include"dsdebug.h"


DlgKeyBoard::DlgKeyBoard(KControlsBase* lineEdit,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgKeyBoard)
{
    ui->setupUi(this);
    this->setWindowTitle("数字键盘");
    initStyle();
    initControl();

    m_InPut=lineEdit;

    //this->setGeometry(inputLineEdit->x(),inputLineEdit->y()+inputLineEdit->geometry().height()+2,this->geometry().width(),this->geometry().height());
}



void DlgKeyBoard::initControl()
{
    m_btnGroup=new QButtonGroup;
    m_btnGroup->addButton(ui->toolButton_0,0);
    m_btnGroup->addButton(ui->toolButton_1,1);
    m_btnGroup->addButton(ui->toolButton_2,2);
    m_btnGroup->addButton(ui->toolButton_3,3);
    m_btnGroup->addButton(ui->toolButton_4,4);
    m_btnGroup->addButton(ui->toolButton_5,5);
    m_btnGroup->addButton(ui->toolButton_6,6);
    m_btnGroup->addButton(ui->toolButton_7,7);
    m_btnGroup->addButton(ui->toolButton_8,8);
    m_btnGroup->addButton(ui->toolButton_9,9);
    m_btnGroup->addButton(ui->toolButtonSub,10);
    m_btnGroup->addButton(ui->toolButton_pt,11);
    m_btnGroup->addButton(ui->toolButtonBack,12);
    m_btnGroup->addButton(ui->toolButton_enter,13);
    m_btnGroup->addButton(ui->toolButtonExit,14);

    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(btnJudge(int)));

}

void DlgKeyBoard::btnJudge(int idx)
{
    QString str=ui->lineEdit->text();
    switch (idx)
    {
    case 0:
        ui->lineEdit->setText(str+"0");
        break;
    case 1:
        ui->lineEdit->setText(str+"1");
        break;
    case 2:
        ui->lineEdit->setText(str+"2");
        break;
    case 3:
        ui->lineEdit->setText(str+"3");
        break;
    case 4:
        ui->lineEdit->setText(str+"4");
        break;
    case 5:
        ui->lineEdit->setText(str+"5");
        break;
    case 6:
        ui->lineEdit->setText(str+"6");
        break;
    case 7:
        ui->lineEdit->setText(str+"7");
        break;
    case 8:
        ui->lineEdit->setText(str+"8");
        break;
    case 9:
        ui->lineEdit->setText(str+"9");
        break;
    case 10:
    {
        if(str.mid(str.length()-1)=="-"|str.contains("."))
        {
            ui->lineEdit->setText(str);
        }
        else
        {
            ui->lineEdit->setText(str+"-");
        }

    }
        break;
    case 11:

        if(str.contains(".")|str.contains("-"))
        {
            ui->lineEdit->setText(str);
        }
        else
        {
            ui->lineEdit->setText(str+".");
        }
        break;
    case 12:
        ui->lineEdit->setText(str.length()>0?str.mid(0,str.length()-1):"");
        break;
    case 13:
    {
        switch (m_InPut->type())
        {
        case ENUMKCONTROLTYPE_KSPINBOX:
        {
            KSpinBoxKb* pIn=static_cast<KSpinBoxKb*>(m_InPut);
            pIn->setValue(str.toInt());
        }
            break;
        case ENUMKCONTROLTYPE_KLINEEDIT:
        {
            KLineEditKb* pIn=static_cast<KLineEditKb*>(m_InPut);
            pIn->setText(str);
        }
            break;
        }


        this->close();
    }
        break;
    case 14:
        this->close();
        break;

    }


}


void DlgKeyBoard::initStyle()
{

    this->setWindowFlags(Qt::FramelessWindowHint);

}

DlgKeyBoard::~DlgKeyBoard()
{
    delete ui;
}
