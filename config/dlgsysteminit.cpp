#include "dlgsysteminit.h"
#include "ui_dlgsysteminit.h"
#include"common/filehelper.h"
#include"params/dssystemparam.h"
DlgSystemInit::DlgSystemInit(ENUMCAMERATYPE* cameraType,int* cameraNum, ENUMIOTYPE* IOType,ENUMPROBNUM *ProbNum,ENUMARRANGETYPE *ArrangeType,bool *Com1,bool *Com2,bool *Com3,bool *Com4,QWidget *parent) :
    m_CameraType(cameraType),
    m_CameraNum(cameraNum),
    m_IOType(IOType),
    m_ProNum(ProbNum),
    m_ArrangeType(ArrangeType),
    m_Com1(Com1),
    m_Com2(Com2),
    m_Com3(Com3),
    m_Com4(Com4),
    QDialog(parent),
    ui(new Ui::DlgSystemInit)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("系统配置"));


    InitStyle();
    this->ui->comboBoxCameraType->addItem(tr("水星"),0);

    this->ui->comboBoxCameraNum->addItem("1",1);
    this->ui->comboBoxCameraNum->addItem("2",2);
    this->ui->comboBoxCameraNum->addItem("3",3);
    this->ui->comboBoxCameraNum->addItem("4",4);
    this->ui->comboBoxCameraNum->addItem("5",5);
    this->ui->comboBoxCameraNum->addItem("6",6);
    this->ui->comboBoxCameraNum->addItem("7",7);
    this->ui->comboBoxCameraNum->addItem("8",8);
    this->ui->comboBoxCameraNum->addItem("9",9);
    //    this->ui->comboBoxCameraNum->addItem("10",10);
    //    this->ui->comboBoxCameraNum->addItem("11",11);
    this->ui->comboBoxIOType->addItem(tr("研华GPIO"),0);
    this->ui->comboBoxIOType->addItem(tr("研扬GPIO"),1);
    this->ui->comboBoxIOType->addItem(tr("桦汉GPIO"),2);
    this->ui->comboBoxIOType->addItem(tr("研扬高速GPIO"),3);
    this->ui->comboBoxIOType->addItem(tr("桦汉高速GPIO"),4);
    this->ui->comboBoxIOType->addItem(tr("研华高速GPIO"),5);


    m_btnGroup=new QButtonGroup;
    m_btnGroup->addButton(this->ui->radioButton767,0);
    m_btnGroup->addButton(this->ui->radioButton677,1);
    m_btnGroup->addButton(this->ui->radioButton1010,2);

    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));


    ui->radioButton767->setChecked(true);
    ui->checkBoxCom1->setChecked(true);
    ui->checkBoxCom2->setChecked(true);
    ui->checkBoxCom3->setChecked(false);
    ui->checkBoxCom4->setChecked(false);

}

DlgSystemInit::~DlgSystemInit()
{
    delete ui;
}
void DlgSystemInit::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgSystemInit::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        // this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgSystemInit::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgSystemInit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgSystemInit::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}


void DlgSystemInit::on_btnMenu_Close_clicked()
{
    this->close();
}

void DlgSystemInit::buttonJudge(int i)
{
    m_iArrangeMode=i;
}




void DlgSystemInit::on_pushButtonOK_clicked()
{
    *this->m_CameraType=static_cast<ENUMCAMERATYPE>(this->ui->comboBoxCameraType->currentIndex());
    *this->m_CameraNum=this->ui->comboBoxCameraNum->currentData().toInt();
    *this->m_IOType=static_cast<ENUMIOTYPE>(this->ui->comboBoxIOType->currentIndex());
    *this->m_ProNum=static_cast<ENUMPROBNUM>(this->ui->comboBoxIProbNum->currentIndex());
    *this->m_ArrangeType=static_cast<ENUMARRANGETYPE>(m_iArrangeMode);


    *this->m_Com1=(this->ui->checkBoxCom1->isChecked());
    *this->m_Com2=(this->ui->checkBoxCom2->isChecked());
    *this->m_Com3=(this->ui->checkBoxCom3->isChecked());
    *this->m_Com4=(this->ui->checkBoxCom4->isChecked());



    this->accept();
}

void DlgSystemInit::on_pushButtonCancel_clicked()
{
    this->reject();
}

void DlgSystemInit::on_toolButton_clicked()
{
    this->close();
}
