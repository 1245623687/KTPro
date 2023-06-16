#include "dlgbrandadd.h"
#include "ui_dlgbrandadd.h"

#include"params/dssystemparam.h"
#include"common/filehelper.h"
#include"params/clscameraconfig.h"
#include"packagechecker.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"CheckOperator/imgtobaccocontrol.h"
#include"params/clscommoninput.h"
#include<QGridLayout>
DlgBrandAdd::DlgBrandAdd(QString brandName,QWidget *parent) :
    LastBrandName(brandName),
    QDialog(parent),
    ui(new Ui::DlgBrandAdd)
{
    ui->setupUi(this);
    this->setWindowTitle("品牌添加");
    InitStyle();

    m_BtnGroup=new QButtonGroup(this);

    ClsCommonInput* clsCommonInput=ClsCommonInput::getInstance();
    clsCommonInput->load();
    std::vector<QString> lstNames= clsCommonInput->getProductName();
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
    ui->widgetCommonInput->setLayout(layout);


    connect(m_BtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));



    m_CameraNum=0;
    QButtonGroup * btnGroupCameras=new QButtonGroup(this);
    btnGroupCameras->addButton(this->ui->radioButton_1,1);
    btnGroupCameras->addButton(this->ui->radioButton_2,2);
    btnGroupCameras->addButton(this->ui->radioButton_3,3);
    btnGroupCameras->addButton(this->ui->radioButton_4,4);
    btnGroupCameras->addButton(this->ui->radioButton_5,5);
    btnGroupCameras->addButton(this->ui->radioButton_6,6);
    btnGroupCameras->addButton(this->ui->radioButton_7,7);
    btnGroupCameras->addButton(this->ui->radioButton_8,8);


    connect(btnGroupCameras,SIGNAL(buttonClicked(int)),this,SLOT(buttonCameraJudge(int)));
    if(DSSystemParam::BrandName!="Default")
    {
        btnGroupCameras->button(PackageChecker::getInstance()->ImgTobaccoRun->LstImgPro.size())->click();
    }
    else
    {
        btnGroupCameras->button(8)->click();
    }


    QButtonGroup * btnGroupNames=new QButtonGroup(this);


    ui->lineEditBrandName->setText(LastBrandName);
    ui->lineEditBrandName->setFocus();

    memset(ImgWids,0,sizeof (int)*8);
    memset(ImgHeis,0,sizeof (int)*8);
}


void DlgBrandAdd::buttonJudge(int idx)
{
    this->ui->lineEditBrandName->setText(ui->lineEditBrandName->text().trimmed()+m_BtnGroup->button(idx)->text());
}

void DlgBrandAdd::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgBrandAdd::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgBrandAdd::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgBrandAdd::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgBrandAdd::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

//void DlgBrandAdd::on_btnMenu_Max_clicked()
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

//void DlgBrandAdd::on_btnMenu_Min_clicked()
//{
//    this->showMinimized();
//}

void DlgBrandAdd::on_btnMenu_Close_clicked()
{
    this->close();
}



void DlgBrandAdd::buttonCameraJudge(int cameraNum)
{
    m_CameraNum=cameraNum;
    DSDEBUG<<"CAMERA NUM:"<<m_CameraNum<<endl;

    ui->comboBoxCamNum->clear();
    for(int i=0;i<cameraNum;i++)
    {
        ui->comboBoxCamNum->addItem(QString("相机%1").arg(i+1));
    }


}

DlgBrandAdd::~DlgBrandAdd()
{
    delete ui;
}

void DlgBrandAdd::on_pushButtonCancel_clicked()
{
    this->close();
}

void DlgBrandAdd::on_pushButtonSave_clicked()
{
    QVector<QString> foldersNames;
    FileHelper::getAllFileFolder(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath),foldersNames);
    for(int idx=0;idx<foldersNames.size();++idx)
    {
        if(ui->lineEditBrandName->text().trimmed()==foldersNames[idx])
        {
            frmMessageBox *msg = new frmMessageBox;

            msg->SetMessage("当前料号已经存在,请从新输入料号!", 2);
            msg->exec();
            ui->lineEditBrandName->setFocus();
            return;
        }
    }


    //copy camera and config file

    if(m_CameraNum==0)
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请选择相机数量!", 2);
        msg->exec();
        return;
    }
    if(this->ui->lineEditBrandName->text()=="")
    {
        frmMessageBox *msg = new frmMessageBox;

        msg->SetMessage("请输入产品名称!", 2);
        msg->exec();
        return;
    }
    for (int i=0;i<m_CameraNum;i++)
    {
       if(ImgHeis[i]==0 || ImgWids[i]==0)
       {
           frmMessageBox *msg = new frmMessageBox;

           msg->SetMessage(QString("图像%1尺寸设置错误!").arg(i+1), 2);
           msg->exec();
           return;
       }
    }


    QDir dirtmp(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+LastBrandName));
    if((DSSystemParam::BrandName=="Default"||(ImgTobaccoControl(PackageChecker::getInstance()->ImgTobaccoRun).getImgProNum()!=m_CameraNum)))
    {
        QString xmlParentDir=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();
        FileHelper::makeDir(xmlParentDir);
        QString xmlPath=xmlParentDir+"/config.xml";


        ImgTobaccoControl::generateInitXmlFile(xmlPath,this->ui->lineEditBrandName->text(),ImgHeis,ImgWids,m_CameraNum);
        ClsCameraConfig* cameraConfig=ClsCameraConfig::getInstance();
        cameraConfig->createCameraConfigFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text(),m_CameraNum);

        //保存ref图像
        for (int idx_img=1;idx_img<=m_CameraNum;++idx_img)
        {
            QString filename=QString("%1/ImgPro%2.bmp").arg(xmlParentDir).arg(idx_img);
            QTextCodec *code = QTextCodec::codecForName("GB2312");
            std::string name = code->fromUnicode(filename.toStdString().c_str()).data();
            cv::Mat tmpimg=cv::Mat::zeros(ImgHeis[idx_img-1],ImgWids[idx_img-1],CV_8UC1);
            cv::imwrite(name,tmpimg);
        }



        QString backupPath1=DSSystemParam::AppPath+"/backup/backup1/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();
        QString backupPath2=DSSystemParam::AppPath+"/backup/backup2/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();

        QString srcpath=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text());
        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text()),
                                    backupPath1
                                    );
        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text()),
                                    backupPath2
                                    );



        this->close();
    }
    else
    {

        ImgTobacco* tmp=new ImgTobacco;

        ImgTobaccoControl control(tmp);
        control.loadConfig(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+LastBrandName));

        tmp->ProductName=this->ui->lineEditBrandName->text().toStdString();

        QString xmlParentDir=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();


        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+LastBrandName),
                                    xmlParentDir
                                    );

        control.saveConfigtoXmlFile(xmlParentDir);


        QString backupPath1=DSSystemParam::AppPath+"/backup/backup1/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();

        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text()),
                                    backupPath1
                                    );
        QString backupPath2=DSSystemParam::AppPath+"/backup/backup2/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text();

        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+this->ui->lineEditBrandName->text()),
                                    backupPath2 );
        this->close();
    }
}

void DlgBrandAdd::on_toolButtonClose_clicked()
{
    this->close();
}



void DlgBrandAdd::on_pushButtonSetSize_clicked()
{
    int curCamIdx=ui->comboBoxCamNum->currentIndex();
    ImgWids[curCamIdx]=ui->spinBoxImgWid->value();
    ImgHeis[curCamIdx]= ui->spinBoxImgHei->value();
}

void DlgBrandAdd::on_comboBoxCamNum_currentIndexChanged(int index)
{
    ui->spinBoxImgWid->setValue(ImgWids[index]);
    ui->spinBoxImgHei->setValue(ImgHeis[index]);
}
