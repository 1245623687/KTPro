#include "dlgsysteminit.h"
#include "ui_dlgsysteminit.h"
#include"common/filehelper.h"
#include"params/dssystemparam.h"
DlgSystemInit::DlgSystemInit(ENUMCAMERATYPE* cameraType,int* cameraNum, ENUMIOTYPE* IOType,QWidget *parent) :
    m_CameraType(cameraType),
    m_CameraNum(cameraNum),
    m_IOType(IOType),
    QDialog(parent),
    ui(new Ui::DlgSystemInit)
{
    ui->setupUi(this);
    this->setWindowTitle("系统配置");


InitStyle();
    this->ui->comboBoxCameraType->addItem("水星",0);

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
    this->ui->comboBoxIOType->addItem("研华GPIO",0);
    this->ui->comboBoxIOType->addItem("研扬GPIO",1);
    this->ui->comboBoxIOType->addItem("桦汉GPIO",2);
    this->ui->comboBoxIOType->addItem("研扬高速GPIO",3);
    this->ui->comboBoxIOType->addItem("桦汉高速GPIO",4);
    this->ui->comboBoxIOType->addItem("研华高速GPIO",5);


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




void DlgSystemInit::on_pushButtonOK_clicked()
{
   *this->m_CameraType=static_cast<ENUMCAMERATYPE>(this->ui->comboBoxCameraType->currentIndex());
   *this->m_CameraNum=this->ui->comboBoxCameraNum->currentData().toInt();
   *this->m_IOType=static_cast<ENUMIOTYPE>(this->ui->comboBoxIOType->currentIndex());



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
