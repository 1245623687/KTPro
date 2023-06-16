#include "dlgoperatorlog.h"
#include "ui_dlgoperatorlog.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include<QStringList>
DlgOperatorLog::DlgOperatorLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOperatorLog)
{
    ui->setupUi(this);

    InitStyle();

    InitControl();

}

void DlgOperatorLog::InitControl()
{

    this->m_Model=new QStandardItemModel;
    ui->tableView->setModel(m_Model);
    QStringList listmodelMain;
    listmodelMain<<"时间"<<"操作"<<"人员";
    this->m_Model->setHorizontalHeaderLabels(listmodelMain);

    QVector<int> vArr;
    vArr.push_back(100);
    vArr.push_back(0);
    vArr.push_back(100);
    ui->tableView->setColumnFixWidth(vArr);


}

void DlgOperatorLog::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);



}

bool DlgOperatorLog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgOperatorLog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgOperatorLog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgOperatorLog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}


void DlgOperatorLog::on_btnMenu_Close_clicked()
{
    this->close();
}


DlgOperatorLog::~DlgOperatorLog()
{
    delete ui;
}
