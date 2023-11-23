#include "dlgsystemconfig.h"
#include "ui_dlgsystemconfig.h"
#include"qtCommon/frmmessagebox.h"
#include"qtCommon/iconhelper.h"
#include"dsdebug.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"packagechecker.h"
#include"params/clscommoninput.h"
#include<QMap>
#include<QFileDialog>
#include"opencv.hpp"

DlgSystemConfig::DlgSystemConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSystemConfig)
{
    ui->setupUi(this);
    this->setWindowTitle("参数设置");
    InitStyle();

    m_BtnGroup=new QButtonGroup(this);

    ClsCommonInput* clsCommonInput=ClsCommonInput::getInstance();
    clsCommonInput->load();

    std::vector<QString> lstNames= clsCommonInput->getMachineNames();
    QGridLayout* layout=new QGridLayout(this);
    for (int i=0;i<lstNames.size();++i)
    {
        int row=i/4;
        int col=i%4;
        QPushButton* btn=new QPushButton(lstNames[i]);
        btn->setStyleSheet("QPushButton{border-style: none;border: 0px; color: #F0F0F0;padding: 5px; min-height: 20px;border-radius:5px; background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);}"
                           "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #636363, stop:1 #575757);}"
                           "QPushButton:pressed{ background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);}");
        layout->addWidget(btn,row,col);
        m_BtnGroup->addButton(btn,i);
    }
    ui->widgetButtons->setLayout(layout);


    connect(m_BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));



    ui->tabWidget->style()->unpolish(ui->tabWidget);
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);

    initControl();

    ui->tabWidget->setCurrentIndex(0);

#ifndef FALG_PHE

    ui->groupBoxResult_2->setVisible(false);
    ui->groupBox_5->setVisible(false);
#endif
    ui->radioButtonPicAndEph->setVisible(false);
}

void DlgSystemConfig::initControl()
{



    ui->tabWidget->setTabEnabled(2, false);




    PackageChecker* pc=PackageChecker::getInstance();
    this->ui->lineEditMachineName->setText( pc->Options->MachineName());
    this->ui->lineEditImgSavePath->setText(pc->Options->ImgSavePath());
    this->ui->spinBoxNgNum->setValue(pc->Options->MaxSaveImgSize());

    this->ui->timeEditShiftMorning->setTime(pc->Options->ShiftMorning());
    this->ui->timeEditShiftAfternoon->setTime(pc->Options->ShiftAfternoon());
    this->ui->timeEditShiftNight->setTime(pc->Options->ShiftNight());

    QButtonGroup * btnGroupImgSave=new QButtonGroup(this->ui->groupBoxImgSave);
    btnGroupImgSave->addButton(this->ui->radioButtonSaveNo,0);
    btnGroupImgSave->addButton(this->ui->radioButtonSaveAll,1);
    btnGroupImgSave->addButton(this->ui->radioButtonSaveNG,2);
    connect(btnGroupImgSave,SIGNAL(buttonClicked(int)),this,SLOT(buttonSaveTypeJudge(int)));

    QButtonGroup * btnGroupOutput=new QButtonGroup(this);
    btnGroupOutput->addButton(this->ui->radioButtonOutResult,0);
    btnGroupOutput->addButton(this->ui->radioButtonNotOutResult,1);
    connect(btnGroupOutput,SIGNAL(buttonClicked(int)),this,SLOT(buttonOutputTypeJudge(int)));

    QButtonGroup * btnDisplay=new QButtonGroup(this);
    btnDisplay->addButton(this->ui->radioButtonAllAreaDisplay,0);
    btnDisplay->addButton(this->ui->radioButtonDefectAreaDisplay,1);
    btnDisplay->addButton(this->ui->radioButtonNoDisplay,2);
    connect(btnDisplay,SIGNAL(buttonClicked(int)),this,SLOT(buttonDisplayTypeJudge(int)));


    QButtonGroup * btnCheckMode=new QButtonGroup(this);
    btnCheckMode->addButton(this->ui->radioButtonOnlyPic,0);
    btnCheckMode->addButton(this->ui->radioButtonOnlyEph,1);
    btnCheckMode->addButton(this->ui->radioButtonPicAndEph,2);
    btnCheckMode->addButton(this->ui->radioButtonPicOrEph,3);
    connect(btnCheckMode,SIGNAL(buttonClicked(int)),this,SLOT(buttonCheckModeJudge(int)));


    QButtonGroup * btnEphDisplay=new QButtonGroup(this);
    btnEphDisplay->addButton(this->ui->radioButtonEphDisplay,0);
    btnEphDisplay->addButton(this->ui->radioButtonEphNoDisplay,1);
    connect(btnEphDisplay,SIGNAL(buttonClicked(int)),this,SLOT(buttonEphDisplayJudge(int)));


    ui->spinBoxSaveDaysNum->setValue(pc->Options->SaveDaysNum());
    switch (pc->Options->SaveType())
    {
    case 0:
        this->ui->radioButtonSaveNo->click();
        break;
    case 1:
        this->ui->radioButtonSaveAll->click();
        break;
    case 2:
        this->ui->radioButtonSaveNG->click();
        break;
    }

    switch (pc->Options->OutputType()) {
    case 0:
        this->ui->radioButtonOutResult->click();
        break;
    case 1:
        this->ui->radioButtonNotOutResult->click();
        break;
    }

    switch (pc->Options->DisplayType()) {
    case 0:
        this->ui->radioButtonAllAreaDisplay->click();
        break;
    case 1:
        this->ui->radioButtonDefectAreaDisplay->click();
        break;
    case 2:
        this->ui->radioButtonNoDisplay->click();
        break;
    }

    m_CheckModeType=pc->Options->checkMode();
    switch ( static_cast<int>(pc->Options->checkMode()))
    {
    case 0:

        ui->radioButtonOnlyPic->setChecked(true);
        break;
    case 1:
        ui->radioButtonOnlyEph->setChecked(true);
        break;
    case 2:
        this->ui->radioButtonPicAndEph->setChecked(true);
        break;
    case 3:
        this->ui->radioButtonPicOrEph->setChecked(true);
        break;
    }


    switch (static_cast<int>(pc->Options->ephDisplay()) ) {
    case 0:
        this->ui->radioButtonEphDisplay->click();
        break;
    case 1:
        this->ui->radioButtonEphNoDisplay->click();
        break;
    }


    ui->label_7->setVisible(false);
    ui->spinBoxNgNum->setVisible(false);
    ui->checkBoxNGNum->setVisible(false);


    //数据采集
    ui->lineEditIP->setText(pc->Options->getIPAddress());
    ui->lineEditPort->setText(QString::number(pc->Options->getIPPort()));
    ui->lineEditSendInterval->setText(QString::number(pc->Options->getSendInterval()));

    ui->label_ConnectStatus->setText(pc->m_cBaseServerTCP->m_strConnectStatus);
    if(pc->m_cBaseServerTCP->m_strConnectStatus=="连接成功")
    {
         ui->label_ConnectStatus->setStyleSheet("color:rgb(0,255,0)");
    }
    else
    {
        ui->label_ConnectStatus->setStyleSheet("color:rgb(255,0,0)");
    }
}

void DlgSystemConfig::buttonJudge(int idx)
{
    this->ui->lineEditMachineName->setText(ui->lineEditMachineName->text().trimmed()+m_BtnGroup->button(idx)->text());
}

void DlgSystemConfig::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf013), 12);

}


bool DlgSystemConfig::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgSystemConfig::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgSystemConfig::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgSystemConfig::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

//void DlgSystemConfig::on_btnMenu_Max_clicked()
//{
//    if (max) {
//        this->setGeometry(location);
//        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
//        ui->btnMenu_Max->setToolTip("最大化");
//    } else {
//        location = this->geometry();
//        this->setGeometry(qApp->desktop()->availableGeometry());
//        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
//        ui->btnMenu_Max->setToolTip("还原");
//    }
//    max = !max;
//}

//void DlgSystemConfig::on_btnMenu_Min_clicked()
//{
//    this->showMinimized();
//}

void DlgSystemConfig::on_btnMenu_Close_clicked()
{
    this->close();
}

DlgSystemConfig::~DlgSystemConfig()
{
    delete ui;
}

void DlgSystemConfig::buttonSaveTypeJudge(int type)
{
    //DSDEBUG<<type<<endl;
    this->m_ImgSaveType=static_cast<ENUMIMGSAVETYPE>(type);

}

void DlgSystemConfig::on_pushButtonOpen_clicked()
{
    QString dirPath= QFileDialog::getExistingDirectory();
    // DSDEBUG<<"dirPath:"<<dirPath<<endl;
    if (dirPath!="")
    {this->ui->lineEditImgSavePath->setText(dirPath);}
}

void DlgSystemConfig::buttonDisplayTypeJudge(int type)
{

    this->m_DisplayType=static_cast<ENUMDISPLAYTYPE>(type);

}

void DlgSystemConfig::buttonCheckModeJudge(int type)
{
    this->m_CheckModeType=static_cast<ENUMCHECKMODETYPE>(type);
#ifdef FALG_PHE

    PackageChecker* pc=PackageChecker::getInstance();
    //光电剔除开与关，使用7号针脚
    //只打开图像剔除
    PackageChecker::getInstance()->m_pBaseCom->togleDisconnect();
    if(m_CheckModeType==ENUMCHECKMODETYPE_PIC ||   m_CheckModeType==ENUMCHECKMODETYPE_PICANDPHE)
    {

        //光电剔除关闭
        pc->IOContol->setLevel(7,0xFFFFFFFF,0x00);
        QString strCommand="AA01090100000000";
        QByteArray receive=strCommand.toLatin1();
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        DSDEBUG__ <<"光电剔除关闭 " <<strCommand.toLatin1() <<endl;
        //图像剔除打开
        strCommand="AA01090200010000";
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        DSDEBUG__ <<"图像剔除打开 " <<strCommand.toLatin1() <<endl;

    }


    //只打开光电剔除
    if(m_CheckModeType==ENUMCHECKMODETYPE_PHE)
    {
        pc->IOContol->setLevel(7,0xFFFFFFFF,0xff);
        QString strCommand="AA01090100010000";
        QByteArray receive;
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        DSDEBUG__ <<"光电剔除打开 " <<strCommand.toLatin1() <<endl;

        //图像剔除关闭
        strCommand="AA01090200000000";
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);
        DSDEBUG__ <<"图像剔除关闭 "  <<strCommand.toLatin1() <<endl;


    }
    //图像，光电剔除都开
    if( m_CheckModeType==ENUMCHECKMODETYPE_PICORPHE)
    {
        //光电剔除打开
        pc->IOContol->setLevel(7,0xFFFFFFFF,0xff);
        QString strCommand="AA01090100010000";
        QByteArray receive;
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);
        DSDEBUG__ <<"光电剔除打开  "  <<strCommand.toLatin1() <<endl;

        //图像剔除打开
        strCommand="AA01090200010000";
        pc->m_pBaseCom->sendQZCommand(strCommand.toLatin1(),receive);
        pc->m_pBaseCom->sendKTCommand(strCommand.toLatin1(),receive);

        DSDEBUG__ <<"图像剔除打开  "  <<strCommand.toLatin1() <<endl;
    }

    PackageChecker::getInstance()->m_pBaseCom->togleConnect();
#endif

}

void DlgSystemConfig::buttonEphDisplayJudge(int type)
{
    this->m_EphDispType=static_cast<ENUMEPHDISPLAYTYPE>(type);
}


void DlgSystemConfig::on_pushButtonSaveRet_clicked()
{
    PackageChecker* pc=PackageChecker::getInstance();
    pc->Options->setMachineName(this->ui->lineEditMachineName->text());
    pc->Options->setImgSavePath(this->ui->lineEditImgSavePath->text());
    pc->Options->setMaxSaveImgSize(this->ui->spinBoxNgNum->value());
    pc->Options->setSaveType(this->m_ImgSaveType);
    pc->Options->setOutputType(this->m_OutputType);
    pc->Options->setDisplayType(this->m_DisplayType);
    pc->Options->setShiftMorning(ui->timeEditShiftMorning->time());
    pc->Options->setShiftAfternoon(ui->timeEditShiftAfternoon->time());
    pc->Options->setShiftNight(ui->timeEditShiftNight->time());
    pc->Options->setSaveDaysNum(ui->spinBoxSaveDaysNum->value());

    pc->Options->setCheckMode(this->m_CheckModeType);
    pc->Options->setEphDisplay(this->m_EphDispType);

    //数据采集
    pc->Options->setIPAddress(ui->lineEditIP->text());
    pc->Options->setIPPort(ui->lineEditPort->text().toInt());
    pc->Options->setSaveDaysNum(ui->lineEditSendInterval->text().toInt());


    pc->Options->save();
    close();
}

void DlgSystemConfig::buttonOutputTypeJudge(int type)
{
    // DSDEBUG<<type<<endl;
    this->m_OutputType=static_cast<ENUMOUTPUTTYPE>(type);

}


void DlgSystemConfig::on_pushButtonCancel_clicked()
{
    close();
}

void DlgSystemConfig::on_pushButtonSave_clicked()
{
    PackageChecker* pc=PackageChecker::getInstance();
    pc->Options->setMachineName(this->ui->lineEditMachineName->text());
    pc->Options->setImgSavePath(this->ui->lineEditImgSavePath->text());
    pc->Options->setMaxSaveImgSize(this->ui->spinBoxNgNum->value());
    pc->Options->setSaveType(this->m_ImgSaveType);
    pc->Options->setOutputType(this->m_OutputType);
    pc->Options->setDisplayType(this->m_DisplayType);
    pc->Options->setShiftMorning(ui->timeEditShiftMorning->time());
    pc->Options->setShiftAfternoon(ui->timeEditShiftAfternoon->time());
    pc->Options->setShiftNight(ui->timeEditShiftNight->time());
    pc->Options->setSaveDaysNum(ui->spinBoxSaveDaysNum->value());

    pc->Options->setCheckMode(this->m_CheckModeType);
    pc->Options->setEphDisplay(this->m_EphDispType);


    //数据采集
    pc->Options->setIPAddress(ui->lineEditIP->text());
    pc->Options->setIPPort(ui->lineEditPort->text().toInt());
    pc->Options->setSaveDaysNum(ui->lineEditSendInterval->text().toInt());


    //    pc->Options->savebackup();
    //    QThread::msleep(100);
    pc->Options->save();
}





void DlgSystemConfig::on_pushButtonSaveRet1_clicked()
{
    on_pushButtonSaveRet_clicked();
}

void DlgSystemConfig::on_pushButtonCancel1_clicked()
{
    on_pushButtonCancel_clicked();
}

void DlgSystemConfig::on_pushButtonSave1_clicked()
{
    on_pushButtonSave_clicked();
}

void DlgSystemConfig::on_pushButtonClearData_clicked()
{



    PackageChecker* pc=  PackageChecker::getInstance();
    pc->RunParam_CalcNumNg=0;
    pc->RunParam_CalcNumAll=0;


    pc->RunParam_CalcNumNgCams[0]=0;
    pc->RunParam_CalcNumAllCams[0]=0;
    pc->RunParam_CalcNumNgCams[1]=0;
    pc->RunParam_CalcNumAllCams[1]=0;
    pc->RunParam_CalcTimeCams[0]=0;
    pc->RunParam_CalcTimeCams[1]=0;


    for(int i=0;i<CAMERANUM_MAX;++i)
    {
        for(int j=0;j<CHECKOPERATORNUM_MAX;++j)
        {
            PackageChecker::ErrRecord[i][j]=0;

        }
    }

}

//void DlgSystemConfig::on_pushButton_clicked()
//{
//  CIOBase* io=  PackageChecker::getInstance()->IOContol;
//  io->setDirection(0,0xFFFFFFFF,0x00);
//  io->setLevel(0, 0xFFFFFFFF, 0xFF);

//}

void DlgSystemConfig::on_pushButton_2_clicked()
{
    CIOBase* io=  PackageChecker::getInstance()->IOContol;

    io->setLevel(0, 0xFFFFFFFF, 0xFF);

}

void DlgSystemConfig::on_pushButton_3_clicked()
{
    CIOBase* io=  PackageChecker::getInstance()->IOContol;
    io->setDirection(0,0xFFFFFFFF,0x00);
    io->setLevel(0, 0xFFFFFFFF, 0x00);


}

void DlgSystemConfig::on_pushButton_4_clicked()
{
    CIOBase* io=  PackageChecker::getInstance()->IOContol;
    io->setDirection(0,0xFFFFFFFF,0x00);
    io->setLevel(0, 0xFFFFFFFF, 0xFF);
}




void DlgSystemConfig::on_pushButton_1_clicked()
{
    CIOBase* io=  PackageChecker::getInstance()->IOContol;
    //    io->setDirection(0,0xFFFFFFFF,0x00);
    io->setLevel(0, 0xFFFFFFFF, 0x00);
}



void DlgSystemConfig::on_pushButton_7_clicked()
{
    on_pushButtonSave_clicked();
    this->close();
}

void DlgSystemConfig::on_pushButton_8_clicked()
{
    this->close();
}


void DlgSystemConfig::on_checkBoxNGNum_clicked()
{

}

void DlgSystemConfig::updateConncetStatus(int )
{
    ui->label_ConnectStatus->setText(PackageChecker::getInstance()->m_cBaseServerTCP->m_strConnectStatus);

    if(PackageChecker::getInstance()->m_cBaseServerTCP->m_strConnectStatus=="连接成功")
    {
         ui->label_ConnectStatus->setStyleSheet("color:rgb(0,255,0)");
    }
    else
    {
        ui->label_ConnectStatus->setStyleSheet("color:rgb(255,0,0)");
    }
}
