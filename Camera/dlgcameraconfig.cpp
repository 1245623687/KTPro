#include "dlgcameraconfig.h"
#include "ui_dlgcameraconfig.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/iconhelper.h"

#include<QValidator>
#include<QDoubleValidator>
#include"checkoperator/ImgProControl.h"
#include"checkoperator/ImgTobaccoControl.h"
#include"qtCommon/frmmessagebox.h"
#include"dsdebug.h"
#include<time.h>
#include "board/Xmodem.h"
#include "board/SerialCom.h"
#include "board/CMDHost.h"


#define DLGCAMERACFG_IMGHEI 480
#define DLGCAMERACFG_IMGWID 640


DlgCameraConfig::DlgCameraConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCameraConfig)
{
    ui->setupUi(this);
    // ui->pushButton->setVisible(false);
    this->setWindowTitle("相机设置");

    InitStyle();
    InitControls();


    m_Thread=new KThreadCameraConfig;
    connect(m_Thread,SIGNAL(inforUpdate()),this,SLOT(updateImage()));

#ifdef FALG_PHE
    ui->radioButton_TestModel->setVisible(true);
#else
    ui->radioButton_TestModel->setVisible(false);
#endif


}


void DlgCameraConfig::SaveParam()
{
    //    PrintChecker::getInstance()->CameraConfig->setGain(1,this->ui->lineEditGain->text().toDouble());
    //    PrintChecker::getInstance()->CameraConfig->setExposureTime(1,this->ui->lineEditExpose->text().toDouble());
    //    PrintChecker::getInstance()->CameraConfig->save();
    PackageChecker::getInstance()->CameraConfig->setGain(m_CurCameraNo,this->ui->lineEditGain->text().toInt());
    PackageChecker::getInstance()->CameraConfig->setExposureTime(m_CurCameraNo,this->ui->lineEditExpose->text().toInt());

    //    int lightCode=0;
    //    if(ui->checkBoxR->checkState()==Qt::Checked)
    //        lightCode+=4;
    //    if(ui->checkBoxG->checkState()==Qt::Checked)
    //        lightCode+=2;
    //    if(ui->checkBoxB->checkState()==Qt::Checked)
    //        lightCode+=1;

    //    PackageChecker::getInstance()->CameraConfig->setLightCode(m_CurCameraNo,lightCode);


}





DlgCameraConfig::~DlgCameraConfig()
{
    QMap<int,GraphicsSceneCapture*>::iterator itor= m_SceneArr.begin();
    for (;itor!=m_SceneArr.end();++itor)
    {
        itor.value()->clear();

    }
    m_SceneArr.clear();


    delete this->m_Thread;


    delete ui;

}

void DlgCameraConfig::updateImage()
{
    //   cv::imwrite("d:/117.bmp",CodeChecker::getInstance()->CurImage[1].ImageRGB);
    //    for (int i=1;i<CodeChecker::getInstance()->CurImage.size();++i) {
    //        m_SceneArr[m_CurCameraNo]->updateImage(CodeChecker::getInstance()->CurImage[i]);
    //    }
    m_SceneArr[m_CurCameraNo]->updateImage(PackageChecker::getInstance()->CurImage[m_CurCameraNo]);

}

void DlgCameraConfig::updatePixInfo(int x,int y,int pix)
{
    this->ui->labelPos_x->setText(QString::number(x));
    this->ui->labelPos_y->setText(QString::number(y));
    this->ui->labelPos_pix->setText(QString::number(pix));
}


void DlgCameraConfig::InitControls()
{
    ui->widget_11->setVisible(false);

    connect(ui->lineEditGain,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEditExpose,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    connect(ui->lineEditTrgGap,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));



        m_ViewArrMain[1]=ui->graphicsView;
        m_ViewArrMain[2]=ui->graphicsView_2;
        m_ViewArrMain[3]=ui->graphicsView_3;
        m_ViewArrMain[4]=ui->graphicsView_4;

    //场景
    int tmpHei=PackageChecker::getInstance()->ImgHei;
    int tmpWid=PackageChecker::getInstance()->ImgWid;


    int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.size();
    for (int idx=1;idx<=tmpCameraNum;++idx)
    {
        int tmpHei=PackageChecker::getInstance()->ImgHeis[idx-1];
        int tmpWid=PackageChecker::getInstance()->ImgWids[idx-1];

        GraphicsSceneCapture* _scene=new GraphicsSceneCapture(tmpHei,tmpWid);
        // _scene->updateImage((*CodeChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.begin())->CurImage);
        _scene->updateImage(ImgTobaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis).getImgPro(idx)->RefOpencvImage);
        connect(_scene,SIGNAL(updatePix(int,int,int)),this,SLOT(updatePixInfo(int ,int ,int)));
        m_SceneArr[idx]=_scene;


        m_ViewArrMain[idx]->setScene(m_SceneArr[m_CurCameraNo]);
        m_ViewArrMain[idx]->setSceneRect(0,0,tmpWid,tmpHei);



        if((double)PackageChecker::getInstance()->ImgHeis[idx-1]/PackageChecker::getInstance()->ImgWids[idx-1]>(double)DLGCAMERACFG_IMGHEI/DLGCAMERACFG_IMGWID)
        {

            m_ViewArrMain[idx]->scaleto((double)DLGCAMERACFG_IMGHEI/(double)PackageChecker::getInstance()->ImgHeis[idx-1],
                                            (double)DLGCAMERACFG_IMGHEI/(double)PackageChecker::getInstance()->ImgHeis[idx-1]);



           m_ViewArrMain[idx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[idx-1],(double)PackageChecker::getInstance()->ImgHeis[idx-1]));
        }
        else
        {
            m_ViewArrMain[idx]->scaleto((double)DLGCAMERACFG_IMGWID/PackageChecker::getInstance()->ImgWids[idx-1],
                                            (double)DLGCAMERACFG_IMGWID/PackageChecker::getInstance()->ImgWids[idx-1]);

            m_ViewArrMain[idx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[idx-1],(double)PackageChecker::getInstance()->ImgHeis[idx-1]));
        }



    }




    ui->lineEditGain->setValidator(new QIntValidator(0,34,this));
    ui->lineEditExpose->setValidator(new QIntValidator(1,500,this));

    m_BtnGroup=new QButtonGroup(this);
    m_BtnGroup->addButton(ui->toolButtonGainMin,1);
    m_BtnGroup->addButton(ui->toolButtonGainMax,2);
    m_BtnGroup->addButton(ui->toolButtonExposeMin,3);
    m_BtnGroup->addButton(ui->toolButtonExposeMax,4);
    m_BtnGroup->addButton(ui->toolButtonGapMin,5);
    m_BtnGroup->addButton(ui->toolButtonGapMax,6);
    connect(m_BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));

    m_RadioBtnGroup=new QButtonGroup(this);
    m_RadioBtnGroup->addButton(ui->radioButton_1,1);
    m_RadioBtnGroup->addButton(ui->radioButton_2,2);
    m_RadioBtnGroup->addButton(ui->radioButton_3,3);
    m_RadioBtnGroup->addButton(ui->radioButton_4,4);
    m_RadioBtnGroup->addButton(ui->radioButton_5,5);
    m_RadioBtnGroup->addButton(ui->radioButton_6,6);
    m_RadioBtnGroup->addButton(ui->radioButton_7,7);
    m_RadioBtnGroup->addButton(ui->radioButton_8,8);
    connect(m_RadioBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(changeCurCameraNo(int)));

    for (int i=1;i<=8;++i)
    {
        if(i<=tmpCameraNum)
            m_RadioBtnGroup->button(i)->setVisible(true);
        else
            m_RadioBtnGroup->button(i)->setVisible(false);
    }
    m_RadioBtnGroup->button(1)->click();


    ui->radioButton_RunModel->setChecked(true);
    ui->lineEditTrgGap->setText("100");

}

void DlgCameraConfig::changeCurCameraNo(int idx)
{
    if(m_IsCapture)
    {
        on_toolButtonCapture_clicked();
    }

    this->m_CurCameraNo=idx;


    ui->stackedWidget->setCurrentIndex(idx-1);
    m_ViewArrMain[idx]->setScene(m_SceneArr[m_CurCameraNo]);


    QMap<int,ClsCameraConfig::StuCameraParam> cameraCfg;
    int tmpGain=ClsCameraConfig::getInstance()->getGain(m_CurCameraNo);
    int tmpExposure=ClsCameraConfig::getInstance()->getExposureTime(m_CurCameraNo);

    this->ui->lineEditGain->setText(QString::number(tmpGain));
    this->ui->lineEditExpose->setText(QString::number(tmpExposure));
    int tmpLightCode=ClsCameraConfig::getInstance()->getLightCode(m_CurCameraNo);
    ui->checkBoxR->setCheckState(tmpLightCode&0x04?Qt::Checked:Qt::Unchecked);
    ui->checkBoxG->setCheckState(tmpLightCode&0x02?Qt::Checked:Qt::Unchecked);
    ui->checkBoxB->setCheckState(tmpLightCode&0x01?Qt::Checked:Qt::Unchecked);
    updateImage();

}

void DlgCameraConfig::buttonJudge(int idx)
{
    switch (idx)
    {
    case 1:
        this->ui->lineEditGain->setText(QString::number(qMax(0,this->ui->lineEditGain->text().toInt()-1)));
        break;
    case 2:
        this->ui->lineEditGain->setText(QString::number(qMin(25,this->ui->lineEditGain->text().toInt()+1)));
        break;
    case 3:
        this->ui->lineEditExpose->setText(QString::number(qMax(20,this->ui->lineEditExpose->text().toInt()-1)));
        break;
    case 4:
        this->ui->lineEditExpose->setText(QString::number(qMin(50000,this->ui->lineEditExpose->text().toInt()+1)));
        break;
    case 5:
        this->ui->lineEditTrgGap->setText(QString::number(qMax(10,this->ui->lineEditTrgGap->text().toInt()-1)));
        break;
    case 6:
        this->ui->lineEditTrgGap->setText(QString::number(qMin(1000,this->ui->lineEditTrgGap->text().toInt()+1)));
        break;
    }
}

void DlgCameraConfig::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf030), 12);


    ui->toolButtonCapture->setStyleSheet("QToolButton#toolButtonCapture{border-image:url(:/启动common.png); background:transparent; border-style:none;}"
                                         "QToolButton#toolButtonCapture:hover{border-image:url(:/启动hover.png); background:transparent;border-style:none;}"
                                         "QToolButton#toolButtonCapture:pressed{border-image:url(:/启动pressed.png); background:transparent;border-style:none;}");

}

bool DlgCameraConfig::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgCameraConfig::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max)
    {
        //        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgCameraConfig::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgCameraConfig::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgCameraConfig::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void DlgCameraConfig::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void DlgCameraConfig::on_btnMenu_Close_clicked()
{


#ifdef FALG_PHE
    //相机测试模式(测试模式model=1，运行模式model=0),
    int model=0;
    int trigGap=ui->lineEditTrgGap->text().toInt();

    QByteArray sendText=genSendByArray(7,model,trigGap);

    bool ret;
    if(ui->radioButton_TestModel->isChecked())
    {
        QByteArray receivedByArray;
        bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);

    }
    if(ui->radioButton_TestModel->isChecked())
    {
        QByteArray receivedByArray;
        bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    }
    PackageChecker::getInstance()->m_pBaseCom->togleConnect();
#endif




    this->close();
}

void DlgCameraConfig::on_toolButtonImgMin_clicked()
{
    this->ui->graphicsView->scaleMin();
}

void DlgCameraConfig::on_toolButtonImgMax_clicked()
{
    this->ui->graphicsView->scaleMax();
}

void DlgCameraConfig::on_toolButtonImgInitial_clicked()
{
    this->ui->graphicsView->scaleInit();
}

void DlgCameraConfig::on_lineEditGain_textChanged(const QString &arg1)
{

    // qDebug()<<"gain "<<arg1<<endl;
    if(arg1.toInt()>=24)
        return;

    this->ui->lineEditGain->setText(QString::number(qMax(0,qMin(25,this->ui->lineEditGain->text().toInt()))));
    PackageChecker::getInstance()->Cameras[m_CurCameraNo]->SetGain(this->ui->lineEditGain->text().toInt());

}


void DlgCameraConfig::showKeyBoard(KControlsBase* pLineEdit)
{

    switch (pLineEdit->type())
    {
    case ENUMKCONTROLTYPE_KSPINBOX:
    {
        KSpinBoxKb* pWidget=static_cast<KSpinBoxKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x()-70,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    case ENUMKCONTROLTYPE_KLINEEDIT:
    {
        KLineEditKb* pWidget=static_cast<KLineEditKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x()-70,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    }

}

void DlgCameraConfig::on_lineEditExpose_textChanged(const QString &arg1)
{
    this->ui->lineEditExpose->setText(QString::number(qMax(20,qMin(50000,this->ui->lineEditExpose->text().toInt()))));
    PackageChecker::getInstance()->Cameras[m_CurCameraNo]->SetExposureTime(this->ui->lineEditExpose->text().toInt());
}



void DlgCameraConfig::on_pushButtonCfSave_clicked()
{
    SaveParam();
    //    PackageChecker::getInstance()->CameraConfig->save();
}

void DlgCameraConfig::on_pushButtonCfSaveRet_clicked()
{
    SaveParam();
    PackageChecker::getInstance()->CameraConfig->save();
    close();
}

void DlgCameraConfig::on_pushButtonCfCancel_clicked()
{
    ClsCameraConfig * cameraCfg =ClsCameraConfig::getInstance();
    cameraCfg->load();
    for(int idx=1;idx<=DSSystemParam::CameraNum;++idx)
    {
        PackageChecker::getInstance()->Cameras[idx]->SetGain(cameraCfg->getGain(idx));
        PackageChecker::getInstance()->Cameras[idx]->SetExposureTime(cameraCfg->getExposureTime(idx));
    }
    close();
}

void DlgCameraConfig::on_toolButtonCapture_clicked()
{

    this->ui->toolButtonCapture->setEnabled(false);
    setCursor(Qt::BusyCursor);
    m_Thread->setCurCameraNo(m_CurCameraNo);
    if (this->m_IsCapture)//正在取图
    {
        ui->toolButtonCapture->setStyleSheet("QToolButton#toolButtonCapture{border-image:url(:/启动common.png); background:transparent; border-style:none;}"
                                             "QToolButton#toolButtonCapture:hover{border-image:url(:/启动hover.png); background:transparent;border-style:none;}"
                                             "QToolButton#toolButtonCapture:pressed{border-image:url(:/启动pressed.png); background:transparent;border-style:none;}");


        m_Thread->stop();
    }
    else
    {
        if(ui->radioButton_TestModel->isChecked())
        {
#ifdef FALG_PHE
            ui->widget_TestPanel->setVisible(true);
            //相机测试模式(测试模式model=1，运行模式model=0),
            int model=1;
            int trigGap=ui->lineEditTrgGap->text().toInt();

            QByteArray sendText=genSendByArray(7,model,trigGap);

            bool ret;
            if(ui->radioButton_1->isChecked())
            {
                QByteArray receivedByArray;
                bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
            }
            if(ui->radioButton_2->isChecked())
            {
                QByteArray receivedByArray;
                bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
            }
#endif

        }

        PackageChecker* pc=PackageChecker::getInstance();
        if(! pc->Cameras[m_CurCameraNo]->IsOpen())
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(QString("相机%1无法打开,请检查相机是否连接!").arg(m_CurCameraNo), 0);
            msg->exec();
            this->ui->toolButtonCapture->setEnabled(true);
            setCursor(Qt::ArrowCursor);
            return;
        }


        ui->toolButtonCapture->setStyleSheet("QToolButton#toolButtonCapture{border-image:url(:/停止common.png); background:transparent; border-style:none;}"
                                             "QToolButton#toolButtonCapture:hover{border-image:url(:/停止hover.png); background:transparent;border-style:none;}"
                                             "QToolButton#toolButtonCapture:pressed{border-image:url(:/停止pressed.png); background:transparent;border-style:none;}");

        m_Thread->start();
    }

    m_IsCapture=!m_IsCapture;


    this->ui->toolButtonCapture->setEnabled(true);
    setCursor(Qt::ArrowCursor);
}



void DlgCameraConfig::on_pushButton_clicked()
{

    UINT8 packetBuf[PACKET_BUF_SIZE];
    memset(packetBuf, 0, PACKET_BUF_SIZE * sizeof(UINT8));
    if(!OpenPort(1, 115200))
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString("通信端口被占用,请先关闭板卡调试软件!"), 2);
        msg->exec();
        return;
    }
    XModemCommEngine(SLAVE1_CODE, R_CODE, UPLOAD_DATA_LEN, packetBuf);



    unsigned int kk = *((unsigned int*)(packetBuf + 1));
    DSDEBUG<<kk;
    ClsCameraConfig* clsCameraConfig =ClsCameraConfig::getInstance();
    clsCameraConfig->setLightCode(1,kk);
    clsCameraConfig->save();
    packetBuf[1] = kk & 0xFF;
    packetBuf[2] = (kk>>8) & 0xFF;
    packetBuf[3] = (kk>>16) & 0xFF;
    packetBuf[4] = (kk>>24) & 0xFF;
    int crcSum = CRC16(packetBuf, (DOWNLOAD_DATA_LEN - 2));
    packetBuf[ADDR_FOR_CRC_LBYTE] = (UINT8)(crcSum & 0xFF);
    packetBuf[ADDR_FOR_CRC_HBYTE] = (UINT8)((crcSum >> 8) & 0xFF);
    XModemCommEngine(SLAVE1_CODE, W_CODE, DOWNLOAD_DATA_LEN, packetBuf);
    ClosePort();
}


QByteArray DlgCameraConfig::genSendByArray(int OperaCode, int v1, int v2)
{
    QByteArray sendText=QByteArray::fromHex("AA01");

    QString strOperaCode=QString("%1").arg(OperaCode,2,16,QLatin1Char('0')).toUtf8();
    QByteArray byOperaCode=QByteArray::fromHex(QByteArray(strOperaCode.toLatin1()));
    sendText.append(byOperaCode);

    QString strHexV1=QString("%1").arg(v1,2,16,QLatin1Char('0')).toUtf8();
    QByteArray byHexV1=QByteArray::fromHex(QByteArray(strHexV1.toLatin1()));
    sendText.append(byHexV1);

    QString strHexV2=QString("%1").arg(v2,4,16,QLatin1Char('0')).toUtf8();
    QByteArray byHexV2=QByteArray::fromHex(QByteArray(strHexV2.toLatin1()));
    sendText.append(byHexV2);

    QByteArray byCRC=QByteArray::fromHex("0000");
    sendText.append(byCRC);

    return sendText.toHex().toUpper();
}

void DlgCameraConfig::on_radioButton_TestModel_toggled(bool checked)
{

#ifdef FALG_PHE
    if(ui->radioButton_TestModel->isChecked())
    {
        PackageChecker::getInstance()->m_pBaseCom->togleDisconnect();

        ui->widget_TestPanel->setVisible(true);


        //相机测试模式(测试模式model=1，运行模式model=0),
        int model=1;
        int trigGap=ui->lineEditTrgGap->text().toInt();

        QByteArray sendText=genSendByArray(7,model,trigGap);

        if(ui->radioButton_1->isChecked())
        {
            QByteArray receivedByArray;
            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
        }
        if(ui->radioButton_2->isChecked())
        {
            QByteArray receivedByArray;
            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
        }
    }
    else
    {
        ui->widget_TestPanel->setVisible(false);

        int model=0;
        int trigGap=ui->lineEditTrgGap->text().toInt();

        QByteArray sendText=genSendByArray(7,model,trigGap);

        if(ui->radioButton_1->isChecked())
        {

            QByteArray receivedByArray;
            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
        }
        if(ui->radioButton_2->isChecked())
        {
            QByteArray receivedByArray;
            bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
        }
    }
#endif





}

void DlgCameraConfig::on_radioButton_RunModel_toggled(bool checked)
{
    if(ui->radioButton_RunModel->isChecked())
    {

#ifdef FALG_PHE
        PackageChecker::getInstance()->m_pBaseCom->togleConnect();
#endif
         ui->widget_TestPanel->setVisible(false);
    }
    else
    {
        ui->widget_TestPanel->setVisible(true);
    }
}

void DlgCameraConfig::on_lineEditTrgGap_textChanged(const QString &arg1)
{
    this->ui->lineEditTrgGap->setText(QString::number(qMax(10,qMin(1000,this->ui->lineEditTrgGap->text().toInt()))));



    qDebug()<<"textchanged"<<endl;
}
