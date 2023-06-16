#include "dlgphotoelectricitycfg.h"
#include "ui_dlgphotoelectricitycfg.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include <qtextcodec.h>
#include<QDoubleValidator>

//#pragma execution_character_set("utf-8")

dlgphotoelectricitycfg::dlgphotoelectricitycfg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgphotoelectricitycfg)
{
    ui->setupUi(this);
    this->setWindowTitle("光电配置");
    InitStyle();
    InitControl();

    connect(this,&dlgphotoelectricitycfg::updataQZADCValueSig,this,&dlgphotoelectricitycfg::showQZAngleAndADCValue);
    connect(this,&dlgphotoelectricitycfg::updataKTADCValueSig,this,&dlgphotoelectricitycfg::showKTAngleAndADCValue);

    m_bQZisRuning=false;
    m_bKTisRuning=false;
    m_txtCnt=0;

    m_ktangle=0;
    ktprobv1=0;
    ktprobv2=0;
    ktprobv3=0;

    m_qzangle=0;
    qzprobv1=0;
    qzprobv2=0;
    qzprobv3=0;

    ui->radioButton_runMode->setChecked(true);

    m_bRuning=true;
    ui->groupBox_6->setEnabled(false);
    ui->groupBox_20->setEnabled(false);
    ui->stackedWidget_QZ->setCurrentIndex(1);
    ui->stackedWidget_KT->setCurrentIndex(1);
    ui->stackedWidget_QZ2->setCurrentIndex(0);
    ui->stackedWidget_KT2->setCurrentIndex(0);


//    QVector<int> temp;
//    for(int i=0;i<20;i++)
//    {
//        temp.push_back(i);
//    }
//    m_ktMap.insert("20221010",temp);
//    m_ktMap.insert("20221011",temp);
//    m_ktMap.insert("20221012",temp);
//    m_ktMap.insert("20221013",temp);
//    m_ktMap.insert("20221014",temp);
//    m_ktMap.insert("20221015",temp);

//    m_qzMap.insert("20221010",temp);
//    m_qzMap.insert("20221011",temp);
//    m_qzMap.insert("20221012",temp);
//    m_qzMap.insert("20221013",temp);
//    m_qzMap.insert("20221014",temp);
//    m_qzMap.insert("20221015",temp);







    m_lqzTotalNum=0;
    m_lqzNGNum=0;
    m_qzQue.enqueue(QPair<int,int>(0,0));
    m_qzQue.enqueue(QPair<int,int>(0,0));

    m_lktTotalNum=0;
    m_lktNGNum=0;
    m_ktQue.enqueue(QPair<int,int>(0,0));
    m_ktQue.enqueue(QPair<int,int>(0,0));

    m_modelQZ=new QStandardItemModel();
    m_modelKT=new QStandardItemModel();

    m_qzCurIndex=0;
    m_ktCurIndex=0;




    ui->tableViewQZ->verticalHeader()->hide();
    ui->tableViewQZ->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewQZ->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableViewKT->verticalHeader()->hide();
    ui->tableViewKT->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewKT->setEditTriggers(QAbstractItemView::NoEditTriggers);



    //填充列表
    this->m_modelQZ->clear();
    QStringList listmodelRecords;

    ui->tableViewQZ->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(100,100,100);font:8pt '宋体';color: white;};");
    listmodelRecords<<"No"<<"Time";




    m_modelQZ->setHorizontalHeaderLabels(listmodelRecords);
    ui->tableViewQZ->setModel(m_modelQZ);

    ui->tableViewQZ->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableViewQZ->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    //    ui->tableViewQZ->setColumnWidth(0,30);
    //    ui->tableViewQZ->setColumnWidth(1,140);



    //填充列表
    this->m_modelKT->clear();
    ui->tableViewKT->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(100,100,100);font:8pt '宋体';color: white;};");
    m_modelKT->setHorizontalHeaderLabels(listmodelRecords);
    ui->tableViewKT->setModel(m_modelKT);



    ui->tableViewKT->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableViewKT->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    //    ui->tableViewKT->setColumnWidth(0,30);
    //    ui->tableViewKT->setColumnWidth(1,140);
}

dlgphotoelectricitycfg::~dlgphotoelectricitycfg()
{
    delete ui;
}




void dlgphotoelectricitycfg::upDateCheckRet(int type)
{
    if(type==1)
    {
        //        PackageChecker::getInstance()->m_MutexRetPheQZ.lock();
        bool ret;
        QVector<int> retVec;

        ret=PackageChecker::getInstance()->m_pBaseCom->getQZData(retVec);
        DSDEBUG_<<"upDateCheckRet QZ Ret:"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheQZ,0,sizeof (int)*20);

        if(!ret)
        {
            //PackageChecker::getInstance()->m_MutexRetPheQZ.unlock();
            return;
        }

        m_lqzTotalNum++;
        ui->label_qzTotalNum->setText(QString::number(m_lqzTotalNum));


        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];


        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

        bool checkRet=false;
        for(int i=2;i<22;i++)
        {
            PackageChecker::getInstance()->RetPheQZ[i-2]=retVec[i];

            m_qzLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            m_qzLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");

            unsigned int thresh= pheConfig->getProThreshold(1,i-2)*pheConfig->getRatio(1);
            if(retVec[i]<thresh)
            {
                checkRet=true;

                m_qzLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_qzQue.enqueue(tmpPari);
                m_qzQue.dequeue();

                DSDEBUG__<<i<<",qz thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }
        }

        ui->label_qzLastNGValue2->setText(QString("%1-%2").arg(m_qzQue[0].first,2,10,QChar('0')).arg(m_qzQue[0].second,4,10,QChar('0')));
        ui->label_qzLastNGValue1->setText(QString("%1-%2").arg(m_qzQue[1].first,2,10,QChar('0')).arg(m_qzQue[1].second,4,10,QChar('0')));

        if(checkRet)
        {
            m_lqzNGNum++;

            retVec.erase(retVec.begin(),retVec.begin()+2);

            m_mutexMap.lock();
            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_qzMap.insert(date,retVec);
            if(m_qzMap.size()>20)
            {
                m_qzMap.erase(m_qzMap.begin());
            }
            m_mutexMap.unlock();
        }
        ui->label_qzNGNum->setText(QString::number(m_lqzNGNum));

        double ratio=(m_lqzNGNum/(m_lqzTotalNum*1.0))*100;
        ui->label_qzNGRatio->setText(QString::number(ratio,'f',2));
        // PackageChecker::getInstance()->m_MutexRetPheQZ.unlock();
    }

    if(type==2)
    {
        //PackageChecker::getInstance()->m_MutexRetPheKT.lock();

        bool ret;
        QVector<int> retVec;

        ret=PackageChecker::getInstance()->m_pBaseCom->getKTData(retVec);
        DSDEBUG_<<"upDateCheckRet KT Ret :"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheKT,0,sizeof (int)*20);
        if(!ret)
        {
            //PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
            return;
        }
        m_lktTotalNum++;


        ui->label_ktTotalNum->setText(QString::number(m_lktTotalNum));
        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];

        bool checkRet=false;
        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
        for(int i=2;i<22;i++)
        {
            PackageChecker::getInstance()->RetPheKT[i-2]=retVec[i];

            m_ktLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            m_ktLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");
            unsigned int thresh= pheConfig->getProThreshold(2,i-2)*pheConfig->getRatio(2);

            if(retVec[i]<thresh)
            {
                checkRet=true;
                m_ktLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_ktQue.enqueue(tmpPari);
                m_ktQue.dequeue();

                DSDEBUG__<<i<<",kt thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }
        }



        ui->label_ktLastNGValue2->setText(QString("%1-%2").arg(m_ktQue[0].first,2,10,QChar('0')).arg(m_ktQue[0].second,4,10,QChar('0')));
        ui->label_ktLastNGValue1->setText(QString("%1-%2").arg(m_ktQue[1].first,2,10,QChar('0')).arg(m_ktQue[1].second,4,10,QChar('0')));



        if(checkRet)
        {
            m_lktNGNum++;

            retVec.erase(retVec.begin(),retVec.begin()+2);


            m_mutexMap.lock();
            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_ktMap.insert(date,retVec);
            if(m_ktMap.size()>20)
            {
                m_ktMap.erase(m_ktMap.begin());
            }
            m_mutexMap.unlock();
        }
        ui->label_ktNGNum->setText(QString::number(m_lktNGNum));

        double ratio=(m_lktNGNum/(m_lktTotalNum*1.0))*100;
        ui->label_ktNGRatio->setText(QString::number(ratio,'f',2));

        //        PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
    }
}

void dlgphotoelectricitycfg::sendRunCheckCommand(int type)
{
    if(type==1)
    {

        QByteArray sendText=QByteArray::fromHex("AA02000100000000");
        bool  ret= PackageChecker::getInstance()->m_pBaseCom->sendAndGetQZCommand(sendText);

        DSDEBUG_<<"Command QZ: "<<"AA02000100000000"<<"ret,"<<ret<<endl;

        // ret= PackageChecker::getInstance()->m_pBaseCom->m_QZPort.waitForReadyRead();

    }
    else
    {
        QByteArray sendText=QByteArray::fromHex("AA02000100000000");
        bool  ret= PackageChecker::getInstance()->m_pBaseCom->sendAndGetKTCommand(sendText);
        DSDEBUG_<<"Command KT: "<<"AA02000100000000"<<"ret,"<<ret<<endl;
        // ret= PackageChecker::getInstance()->m_pBaseCom->m_QZPort.waitForReadyRead();
    }

}



void dlgphotoelectricitycfg::showKeyBoard(KControlsBase *pLineEdit)
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

void dlgphotoelectricitycfg::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint);

    location = this->geometry();
    max = false;
    mousePressed = false;

    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);
    m_listModel=new QStringListModel();
    // ui->listView->setModel(m_listModel);
}

void dlgphotoelectricitycfg::InitControl()
{
    m_qzLabels.clear();
    m_qzLabels2.clear();

    //    for (int i=0;i<20;i++)
    {
        m_qzLabels.insert(0,ui->label_qz1);
        m_qzLabels.insert(1,ui->label_qz2);
        m_qzLabels.insert(2,ui->label_qz3);
        m_qzLabels.insert(3,ui->label_qz4);
        m_qzLabels.insert(4,ui->label_qz5);
        m_qzLabels.insert(5,ui->label_qz6);
        m_qzLabels.insert(6,ui->label_qz7);
        m_qzLabels.insert(7,ui->label_qz8);
        m_qzLabels.insert(8,ui->label_qz9);
        m_qzLabels.insert(9,ui->label_qz10);

        m_qzLabels.insert(10,ui->label_qz11);
        m_qzLabels.insert(11,ui->label_qz12);
        m_qzLabels.insert(12,ui->label_qz13);
        m_qzLabels.insert(13,ui->label_qz14);
        m_qzLabels.insert(14,ui->label_qz15);
        m_qzLabels.insert(15,ui->label_qz16);
        m_qzLabels.insert(16,ui->label_qz17);
        m_qzLabels.insert(17,ui->label_qz18);
        m_qzLabels.insert(18,ui->label_qz19);
        m_qzLabels.insert(19,ui->label_qz20);

        //显示记录数据
        m_qzLabels2.insert(0,ui->label_qz1_2);
        m_qzLabels2.insert(1,ui->label_qz2_2);
        m_qzLabels2.insert(2,ui->label_qz3_2);
        m_qzLabels2.insert(3,ui->label_qz4_2);
        m_qzLabels2.insert(4,ui->label_qz5_2);
        m_qzLabels2.insert(5,ui->label_qz6_2);
        m_qzLabels2.insert(6,ui->label_qz7_2);
        m_qzLabels2.insert(7,ui->label_qz8_2);
        m_qzLabels2.insert(8,ui->label_qz9_2);
        m_qzLabels2.insert(9,ui->label_qz10_2);

        m_qzLabels2.insert(10,ui->label_qz11_2);
        m_qzLabels2.insert(11,ui->label_qz12_2);
        m_qzLabels2.insert(12,ui->label_qz13_2);
        m_qzLabels2.insert(13,ui->label_qz14_2);
        m_qzLabels2.insert(14,ui->label_qz15_2);
        m_qzLabels2.insert(15,ui->label_qz16_2);
        m_qzLabels2.insert(16,ui->label_qz17_2);
        m_qzLabels2.insert(17,ui->label_qz18_2);
        m_qzLabels2.insert(18,ui->label_qz19_2);
        m_qzLabels2.insert(19,ui->label_qz20_2);


    }

    m_ktLabels.clear();
    m_ktLabels2.clear();
    {
        m_ktLabels.insert(0,ui->label_kt1);
        m_ktLabels.insert(1,ui->label_kt2);
        m_ktLabels.insert(2,ui->label_kt3);
        m_ktLabels.insert(3,ui->label_kt4);
        m_ktLabels.insert(4,ui->label_kt5);
        m_ktLabels.insert(5,ui->label_kt6);
        m_ktLabels.insert(6,ui->label_kt7);
        m_ktLabels.insert(7,ui->label_kt8);
        m_ktLabels.insert(8,ui->label_kt9);
        m_ktLabels.insert(9,ui->label_kt10);

        m_ktLabels.insert(10,ui->label_kt11);
        m_ktLabels.insert(11,ui->label_kt12);
        m_ktLabels.insert(12,ui->label_kt13);
        m_ktLabels.insert(13,ui->label_kt14);
        m_ktLabels.insert(14,ui->label_kt15);
        m_ktLabels.insert(15,ui->label_kt16);
        m_ktLabels.insert(16,ui->label_kt17);
        m_ktLabels.insert(17,ui->label_kt18);
        m_ktLabels.insert(18,ui->label_kt19);
        m_ktLabels.insert(19,ui->label_kt20);

        //显示空头记录数据
        m_ktLabels2.insert(0,ui->label_qz1_3);
        m_ktLabels2.insert(1,ui->label_qz2_3);
        m_ktLabels2.insert(2,ui->label_qz3_3);
        m_ktLabels2.insert(3,ui->label_qz4_3);
        m_ktLabels2.insert(4,ui->label_qz5_3);
        m_ktLabels2.insert(5,ui->label_qz6_3);
        m_ktLabels2.insert(6,ui->label_qz7_3);
        m_ktLabels2.insert(7,ui->label_qz8_3);
        m_ktLabels2.insert(8,ui->label_qz9_3);
        m_ktLabels2.insert(9,ui->label_qz10_3);

        m_ktLabels2.insert(10,ui->label_qz11_3);
        m_ktLabels2.insert(11,ui->label_qz12_3);
        m_ktLabels2.insert(12,ui->label_qz13_3);
        m_ktLabels2.insert(13,ui->label_qz14_3);
        m_ktLabels2.insert(14,ui->label_qz15_3);
        m_ktLabels2.insert(15,ui->label_qz16_3);
        m_ktLabels2.insert(16,ui->label_qz17_3);
        m_ktLabels2.insert(17,ui->label_qz18_3);
        m_ktLabels2.insert(18,ui->label_qz19_3);
        m_ktLabels2.insert(19,ui->label_qz20_3);
    }



    connect(ui->lineEdit_qzcapangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzkickangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzkickstep,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qztrigangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzselfangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzprob,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzprobRatio,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    //ui->lineEdit_qzprobRatio->setValidator(new QDoubleValidator(0.0,2.0,2,this));

    connect(ui->lineEdit_ktcapangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktkickangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktkickstep,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_kttrigangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktselfangle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktprob,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktprobRatio,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    //    ui->lineEdit_ktprobRatio->setValidator(new QDoubleValidator(0.0,2.0,6,this));
    //    QRegExp rx3("1|([0-0]{1}[\.][0-9]{1,2})");
    //    QRegExpValidator *pReg3 = new QRegExpValidator(rx3, this);
    //    ui->lineEdit_ktprobRatio->setValidator(pReg3);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    ui->lineEdit_qzcapangle->setText(QString::number(pheConfig->getCapAngle(1,0)));
    unsigned int kickAngle,kickStep;
    pheConfig->getKickAngleAndStep(1,kickAngle,kickStep);
    ui->lineEdit_qzkickangle->setText(QString::number(kickAngle));
    ui->lineEdit_qzkickstep->setText(QString::number(kickStep));
    ui->lineEdit_qztrigangle->setText(QString::number(pheConfig->getTrigAngle(1)));
    ui->lineEdit_qzselfangle->setText(QString::number(pheConfig->getSelfCheckAngle(1)));

    double ratio=pheConfig->getRatio(1);
    ui->lineEdit_qzprob->setText(QString::number(int(pheConfig->getProThreshold(1,0)*ratio)));

    ui->lineEdit_qzprobRatio->setText(QString::number(pheConfig->getRatio(1),'f',2));
    if(pheConfig->getIsSendFaltSig(1))
    {
        ui->radioButton_qzfaultsigsend->setChecked(true);
    }
    else
    {
        ui->radioButton_qzfaultsignosend->setChecked(true);
    }
    ui->comboBox_qzprob->setCurrentIndex(0);


    //    ui->comboBox_ktprob->clear();
    //    for (int i=0;i<20;i++)
    //    {
    //          ui->comboBox_ktprob->addItem(QString("空头门槛值%1").arg(i+1));
    //    }


    ui->comboBox_ktprob->setCurrentIndex(0);
    ui->comboBox_ktcapangle->setCurrentText(0);

    ui->lineEdit_ktcapangle->setText(QString::number(pheConfig->getCapAngle(2,0)));
    pheConfig->getKickAngleAndStep(2,kickAngle,kickStep);
    ui->lineEdit_ktkickangle->setText(QString::number(kickAngle));
    ui->lineEdit_ktkickstep->setText(QString::number(kickStep));
    ui->lineEdit_kttrigangle->setText(QString::number(pheConfig->getTrigAngle(2)));

    unsigned int sef=pheConfig->getSelfCheckAngle(2);

    ui->lineEdit_ktselfangle->setText(QString::number(pheConfig->getSelfCheckAngle(2)));
    ratio=pheConfig->getRatio(2);
    ui->lineEdit_ktprob->setText(QString::number(int(pheConfig->getProThreshold(2,0)*ratio)));
    ui->lineEdit_ktprobRatio->setText(QString::number(pheConfig->getRatio(2),'f',2));


    if(pheConfig->getIsSendFaltSig(2))
    {
        ui->radioButton_ktfaultsigsend->setChecked(true);
    }
    else
    {
        ui->radioButton_ktfaultsignosend->setChecked(true);
    }

}

void dlgphotoelectricitycfg::appendInfoText(QString settingName,bool ret,QByteArray text,QByteArray Receivetext)
{

    m_txtCnt++;
    if(m_txtCnt==100)
    {
        ui->plainTextEdit->clear();
        m_txtCnt=0;
    }

    QString txtTip="";
    if(ret)
    {
        txtTip=settingName+",Set Success,"+"send,"+QString(text)+",receive,"+QString(Receivetext);
    }
    else
    {
        txtTip=settingName+",Set Failed,"+"send,"+QString(text)+",receive,"+QString(Receivetext);
    }
    ui->plainTextEdit->appendPlainText(txtTip);


}

void dlgphotoelectricitycfg::appendInfoText(QString settingName , bool ret, QString text)
{
    m_txtCnt++;
    if(m_txtCnt==100)
    {
        ui->plainTextEdit->clear();
        m_txtCnt=0;
    }

    QString txtTip="";
    if(ret)
    {
        txtTip=settingName+",Set Success,"+text;
    }
    else
    {
        txtTip=settingName+",Set Failed,"+text;
    }
    ui->plainTextEdit->appendPlainText(txtTip);

}

QByteArray dlgphotoelectricitycfg::genSendByArray(int OperaCode, int v1, int v2)
{
    QByteArray sendText=QByteArray::fromHex("AA01");

    QString strOperaCode=QString("%1").arg(OperaCode,2,16,QLatin1Char('0')).toUtf8();
    QByteArray byOperaCode=QByteArray::fromHex(QByteArray(strOperaCode.toLatin1()));
    sendText.append(byOperaCode);

    QString strHexV1=QString("%1").arg(v1,2,10,QLatin1Char('0')).toUtf8();
    QByteArray byHexV1=QByteArray::fromHex(QByteArray(strHexV1.toLatin1()));
    sendText.append(byHexV1);

    QString strHexV2=QString("%1").arg(v2,4,16,QLatin1Char('0')).toUtf8();
    QByteArray byHexV2=QByteArray::fromHex(QByteArray(strHexV2.toLatin1()));
    sendText.append(byHexV2);

    QByteArray byCRC=QByteArray::fromHex("0000");
    sendText.append(byCRC);

    // QByteArray sendCommand= sendText.toHex().toUpper();

    return sendText.toHex().toUpper();
}

void dlgphotoelectricitycfg::startQZTestRunModel()
{
    while (m_bQZisRuning)
    {
        QThread::msleep(200);

        emit updataQZADCValueSig();
    }
}

void dlgphotoelectricitycfg::startKTTestRunModel()
{

    while (m_bKTisRuning)
    {

        //调试模式，读取当前角度，ADC采样值
        QThread::msleep(200);

        emit updataKTADCValueSig();
    }
}

bool dlgphotoelectricitycfg::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::MouseButtonDblClick) {
        //        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);

}

void dlgphotoelectricitycfg::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void dlgphotoelectricitycfg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void dlgphotoelectricitycfg::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void dlgphotoelectricitycfg::on_btnMenu_Close_clicked()
{

    on_pushButton_qzStopApply_clicked();
    on_pushButton_ktStopApply_clicked();

    ui->radioButton_runMode->setChecked(true);


    PackageChecker::getInstance()->m_pBaseCom->togleConnect();

    this->close();
}

void dlgphotoelectricitycfg::on_pushButton_qzcapangleset_clicked()
{
    //采集编号，采集角度
    int indexAgl=ui->comboBox_qzcapangle->currentIndex()+1;
    int setCurAngle=ui->lineEdit_qzcapangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setCapAngle(1,indexAgl-1,setCurAngle);

    QByteArray sendText=genSendByArray(1,indexAgl,setCurAngle);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);


    if(receivedByArray==sendText)
    {
        ret=true;
    }
    else
    {
        ret=false;
    }

    appendInfoText( "QZ", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;

}

void dlgphotoelectricitycfg::on_pushButton_qzkickset_clicked()
{

    //剔除步数,剔除角度
    int KickStep=ui->lineEdit_qzkickstep->text().toInt();
    int KickAngle=ui->lineEdit_qzkickangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setKickAngleAndStep(1,KickAngle,KickStep);

    QByteArray sendText=genSendByArray(2,KickStep,KickAngle);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);

    appendInfoText( "QZ", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qztrigangleset_clicked()
{

    //相机编号，触发角度
    int CamIndex=0;
    int CamTrgAngle=ui->lineEdit_qztrigangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setTrigAngle(1,CamTrgAngle);

    QByteArray sendText=genSendByArray(3,CamIndex,CamTrgAngle);
    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qzselfangleset_clicked()
{

    //自检角度，自检角度设定值
    int SelfCheck=0;
    int SelfCheckValue=ui->lineEdit_qzselfangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setSelfCheckAngle(1,SelfCheckValue);

    QByteArray sendText=genSendByArray(8,SelfCheck,SelfCheckValue);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;

}

void dlgphotoelectricitycfg::on_pushButton_qzprob_clicked()
{
    //探头编号，探头设定值
    int ProbIndex=ui->comboBox_qzprob->currentIndex()+1;
    int ProbThresh=ui->lineEdit_qzprob->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProThreshold(1,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArray(4,ProbIndex,ProbThresh);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qzfaultsigset_clicked()
{

    //是否发送故障信号，信号值

    int ProbIndex=ui->radioButton_qzfaultsigsend->isChecked();
    int FaultSigVal=0;


    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setIsSendFaltSig(1,ProbIndex);

    QByteArray sendText=genSendByArray(5,ProbIndex,FaultSigVal);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}


void dlgphotoelectricitycfg::on_pushButton_ktcapangleset_clicked()
{
    //空头
    //采集编号，采集角度
    int indexAgl=ui->comboBox_ktcapangle->currentIndex()+1;
    int setCurAngle=ui->lineEdit_ktcapangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setCapAngle(2,indexAgl-1,setCurAngle);

    QByteArray sendText=genSendByArray(1,indexAgl,setCurAngle);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktkickset_clicked()
{
    //空头
    //剔除步数,剔除角度
    int KickStep=ui->lineEdit_ktkickstep->text().toInt();
    int KickAngle=ui->lineEdit_ktkickangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setKickAngleAndStep(2,KickAngle,KickStep);

    QByteArray sendText=genSendByArray(2,KickStep,KickAngle);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_kttrigangleset_clicked()
{
    //空头
    //相机编号，触发角度
    int CamIndex=0;
    int CamTrgAngle=ui->lineEdit_kttrigangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setTrigAngle(2,CamTrgAngle);

    QByteArray sendText=genSendByArray(3,CamIndex,CamTrgAngle);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktselfangleset_clicked()
{
    //空头
    //自检角度，自检角度设定值
    int SelfCheck=0;
    int SelfCheckValue=ui->lineEdit_ktselfangle->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setSelfCheckAngle(2,SelfCheckValue);

    QByteArray sendText=genSendByArray(8,SelfCheck,SelfCheckValue);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktprob_clicked()
{
    //空头
    //探头编号，探头设定值
    int ProbIndex=ui->comboBox_ktprob->currentIndex()+1;
    int ProbThresh=ui->lineEdit_ktprob->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProThreshold(2,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArray(4,ProbIndex,ProbThresh);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktfaultsigset_clicked()
{
    //空头
    //是否发送故障信号，信号值

    int ProbIndex=ui->radioButton_ktfaultsigsend->isChecked();
    int FaultSigVal=0;

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setIsSendFaltSig(2,ProbIndex);

    QByteArray sendText=genSendByArray(5,ProbIndex,FaultSigVal);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);

    qDebug()<<"ret,"<<ret<<endl;
}



void dlgphotoelectricitycfg::on_pushButton_qzApply_clicked()
{
    m_bQZisRuning=true;
    if(m_QZFuture.isRunning())
    {
        return;
    }

    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值

    int ModelIndex=1;
    int ModelVal=0;

    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);

    m_QZFuture=QtConcurrent::run(this,&dlgphotoelectricitycfg::startQZTestRunModel);

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qzStopApply_clicked()
{
    //终止线程的读取值
    m_bQZisRuning=false;
    m_QZFuture.waitForFinished();


    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值
    int ModelIndex=0;
    int ModelVal=0;
    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);
    QByteArray receivedByArray;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZ", ret, sendText,receivedByArray);


    // QThread::msleep(50);
    // PackageChecker::getInstance()->m_pBaseCom->m_QZPort.readAll();

    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktApply_clicked()
{

    m_bKTisRuning=true;
    if(m_KTFuture.isRunning())
    {
        return;
    }

    //空头
    //模式选择(调试，正常)，值

    int ModelIndex=1;
    int ModelVal=0;

    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);
    //开启线程间断100ms读取值


    m_KTFuture=QtConcurrent::run(this,&dlgphotoelectricitycfg::startKTTestRunModel);
    qDebug()<<"ret,"<<ret<<endl;


}

void dlgphotoelectricitycfg::on_pushButton_ktStopApply_clicked()
{

    //终止线程的读取值

    m_bKTisRuning=false;
    m_KTFuture.waitForFinished();
    //空头
    //模式选择(调试，正常)，值

    int ModelIndex=0;
    int ModelVal=0;


    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KT", ret, sendText,receivedByArray);

    // qDebug()<<"ret,"<<ret<<endl;
}



void dlgphotoelectricitycfg::showQZAngleAndADCValue()
{

    //调试模式，读取当前角度，ADC采样值
    QByteArray sendText=QByteArray::fromHex("AA02010100000000");

    int  angle=0;
    int probv1=0;
    int probv2=0;
    int probv3=0;

    sendText=sendText.toHex().toUpper();

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendAndGetQZCommand(sendText,angle,probv1,probv2,probv3);

    if(ret)
    {
        m_qzangle=angle;
        qzprobv1=probv1;
        qzprobv2=probv2;
        qzprobv3=probv3;

        ui->label_qzcurangle->setText(QString::number(m_qzangle));

        ui->label_qzcurvalue1->setText(QString::number(qzprobv1));

        ui->label_qzcurvalue2->setText(QString::number(qzprobv2));

        ui->label_qzcurvalue3->setText(QString::number(qzprobv3));

        QString text=QString("angle: %1,prob1: %2,prob2: %3,prob3: %4").arg(angle).arg(probv1).arg(probv2).arg(probv3);

        appendInfoText("QZ",ret,text);
    }


}

void dlgphotoelectricitycfg::showKTAngleAndADCValue()
{


    //调试模式，读取当前角度，ADC采样值
    QByteArray sendText=QByteArray::fromHex("AA02010100000000");

    int  angle=0;
    int probv1=0;
    int probv2=0;
    int probv3=0;
    sendText=sendText.toHex().toUpper();

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendAndGetKTCommand(sendText,angle,probv1,probv2,probv3);


    if(ret)
    {
        //if(angle!=0)
        m_ktangle=angle;
        //if(probv1!=0)
        ktprobv1=probv1;
        //if(probv2!=0)
        ktprobv2=probv2;
        // if(probv3!=0)
        ktprobv3=probv3;

        ui->label_ktcurangle->setText(QString::number(m_ktangle));

        ui->label_ktcurvalue1->setText(QString::number(ktprobv1));

        ui->label_ktcurvalue2->setText(QString::number(ktprobv2));

        ui->label_ktcurvalue3->setText(QString::number(ktprobv3));

        QString text=QString("angle: %1,prob1: %2,prob2: %3,prob3: %4").arg(angle).arg(probv1).arg(probv2).arg(probv3);

        appendInfoText("KT",ret,text);
    }

}

void dlgphotoelectricitycfg::on_radioButton_runMode_toggled(bool checked)
{
    if(ui->radioButton_runMode->isChecked())
    {
        if(m_bRuning==true)
            return;

        on_pushButton_qzStopApply_clicked();
        on_pushButton_ktStopApply_clicked();

        ui->stackedWidget_QZ->setCurrentIndex(1);
        ui->stackedWidget_KT->setCurrentIndex(1);

        PackageChecker::getInstance()->m_pBaseCom->togleConnect();
        ui->groupBox_6->setEnabled(false);
        ui->groupBox_20->setEnabled(false);
        m_bRuning=true;
    }
    else
    {
        if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
        {
//            frmMessageBox *msg = new frmMessageBox;
//            QTextCodec *codec = QTextCodec::codecForName("GBK");//修改这两行
//            msg->SetMessage(codec->toUnicode("Please Stop Main Run First"), 0);
//            msg->exec();
//            ui->radioButton_runMode->setChecked(true);
//            return;
        }

        PackageChecker::getInstance()->m_pBaseCom->togleDisconnect();

        ui->stackedWidget_QZ->setCurrentIndex(0);
        ui->stackedWidget_KT->setCurrentIndex(0);

        ui->groupBox_6->setEnabled(true);
        ui->groupBox_20->setEnabled(true);

        m_bRuning=false;
    }
}

void dlgphotoelectricitycfg::on_pushButtonSaveCfg_clicked()
{
    ui->pushButtonSaveCfg->setEnabled(false);
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->save();
    ui->pushButtonSaveCfg->setEnabled(true);
}

void dlgphotoelectricitycfg::on_comboBox_qzcapangle_currentIndexChanged(int index)
{

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    ui->lineEdit_qzcapangle->setText(QString::number(pheConfig->getCapAngle(1,index)));

    // QString str=QString::number(pheConfig->getCapAngle(1,index));
}

void dlgphotoelectricitycfg::on_comboBox_qzprob_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    double ratio=pheConfig->getRatio(1);

    ui->lineEdit_qzprob->setText(QString::number(int(pheConfig->getProThreshold(1,index)*ratio)));
}

void dlgphotoelectricitycfg::on_comboBox_ktcapangle_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    ui->lineEdit_ktcapangle->setText(QString::number(pheConfig->getCapAngle(2,index)));
}

void dlgphotoelectricitycfg::on_comboBox_ktprob_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    double ratio= pheConfig->getRatio(2);
    ui->lineEdit_ktprob->setText(QString::number(int(pheConfig->getProThreshold(2,index)*ratio)));
}



void dlgphotoelectricitycfg::on_pushButtonQZRun_clicked()
{
    ui->stackedWidget_QZ2->setCurrentIndex(0);
}


void dlgphotoelectricitycfg::on_pushButtonQZRecord_clicked()
{
    ui->stackedWidget_QZ2->setCurrentIndex(1);
    void on_pushButtonQZRefresh_clicked();
}

void dlgphotoelectricitycfg::on_pushButtonQZResetNum_clicked()
{
    m_lqzTotalNum=0;
    m_lqzNGNum=0;
    ui->label_qzTotalNum->setText("0");
    ui->label_qzNGNum->setText("0");
    ui->label_qzNGRatio->setText("0");
}

void dlgphotoelectricitycfg::on_pushButtonKTRun_clicked()
{
    ui->stackedWidget_KT2->setCurrentIndex(0);
}

void dlgphotoelectricitycfg::on_pushButtonKTRecord_clicked()
{

    ui->stackedWidget_KT2->setCurrentIndex(1);
}

void dlgphotoelectricitycfg::on_pushButtonKTResetNum_clicked()
{
    m_lktTotalNum=0;
    m_lktNGNum=0;

    ui->label_ktTotalNum->setText("0");
    ui->label_ktNGNum->setText("0");
    ui->label_ktNGRatio->setText("0");
}

void dlgphotoelectricitycfg::on_pushButtonQZRefresh_clicked()
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    m_qzCurIndex=0;


    m_mutexMap.lock();
    qzMap.clear();
    qzMap=m_qzMap;
    m_mutexMap.unlock();

    if(qzMap.empty())
        return;

    QMap<QString,QVector<int>>::iterator itor=qzMap.end();

    QString key=  (--itor).key();

    //

    for (int i=0;i<itor.value().size();i++)
    {
       unsigned int thresh=pheConfig->getProThreshold(1,i)*pheConfig->getRatio(1);

        m_qzLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_qzLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_qzLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }


    //    //填充列表
    this->m_modelQZ->clear();
    QStringList listmodelRecords;

    ui->tableViewQZ->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(100,100,100);font:8pt '宋体';color: white;};");

    listmodelRecords<<"No"<<"Time";
    m_modelQZ->setHorizontalHeaderLabels(listmodelRecords);

    ui->tableViewQZ->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableViewQZ->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);




    for (int i=0;;--itor,i++)
    {
        QStandardItem* itemOrder=new QStandardItem(QString::number(i+1));
        itemOrder->setTextAlignment(Qt::AlignCenter);


        QStandardItem* itemDatetime=new QStandardItem(itor.key());
        itemDatetime->setTextAlignment(Qt::AlignCenter);

        m_modelQZ->setItem(i,0,itemOrder);
        m_modelQZ->setItem(i,1,itemDatetime);
        if(itor==qzMap.begin())
            break;
    }


    //列表定位第一张

    QModelIndex modelIdxTmp=ui->tableViewQZ->model()->index(0,0);

    on_tableViewQZ_clicked(modelIdxTmp);
    ui->tableViewQZ->selectRow(0);
    //
}

void dlgphotoelectricitycfg::on_pushButtonKTRefresh_clicked()
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    m_ktCurIndex=0;

    m_mutexMap.lock();
    ktMap.clear();
    ktMap=m_ktMap;
    m_mutexMap.unlock();

    if(ktMap.empty())
        return;

    QMap<QString,QVector<int>>::iterator itor=ktMap.end();
    itor--;
    //
    QString str=itor.key();

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(2,i)*pheConfig->getRatio(2);
        m_ktLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));

        m_ktLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_ktLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }

    //    //填充列表
    this->m_modelKT->clear();
    QStringList listmodelRecords;

    ui->tableViewKT->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(100,100,100);font:8pt '宋体';color: white;};");

    listmodelRecords<<"No"<<"Time";
    m_modelKT->setHorizontalHeaderLabels(listmodelRecords);

    ui->tableViewKT->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableViewKT->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    //    ui->tableViewKT->setColumnWidth(0,30);
    //    ui->tableViewKT->setColumnWidth(1,140);

    for (int i=0;;itor--,i++)
    {
        QStandardItem* itemOrder=new QStandardItem(QString::number(i+1));
        itemOrder->setTextAlignment(Qt::AlignCenter);

        QStandardItem* itemDatetime=new QStandardItem(itor.key());
        itemDatetime->setTextAlignment(Qt::AlignCenter);

        m_modelKT->setItem(i,0,itemOrder);
        m_modelKT->setItem(i,1,itemDatetime);
        if(itor==ktMap.begin())
            break;
    }

    //列表定位第一张
    QModelIndex modelIdxTmp=ui->tableViewKT->model()->index(0,0);
    on_tableViewKT_clicked(modelIdxTmp);
    ui->tableViewKT->selectRow(0);
}

void dlgphotoelectricitycfg::on_pushButtonQZPre_clicked()
{
    if((m_qzCurIndex==0)|| qzMap.empty())
        return;

    m_qzCurIndex--;
    ui->tableViewQZ->selectRow(m_qzCurIndex);
    QModelIndex indextmp=ui->tableViewQZ->model()->index(m_qzCurIndex,1);
    QString str=ui->tableViewQZ->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= qzMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(1,i)*pheConfig->getRatio(1);
        m_qzLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_qzLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_qzLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }


}

void dlgphotoelectricitycfg::on_pushButtonQZNext_clicked()
{



    if((m_qzCurIndex==qzMap.size()-1)|| qzMap.empty())
        return;
    m_qzCurIndex++;


    ui->tableViewQZ->selectRow(m_qzCurIndex);
    QModelIndex indextmp=ui->tableViewQZ->model()->index(m_qzCurIndex,1);
    QString str=ui->tableViewQZ->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= qzMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(1,i)*pheConfig->getRatio(1);
        m_qzLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_qzLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_qzLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }
}

void dlgphotoelectricitycfg::on_pushButtonKTPre_clicked()
{
    if((m_ktCurIndex==0)|| ktMap.empty())
        return;

    m_ktCurIndex--;

    ui->tableViewKT->selectRow(m_ktCurIndex);
    QModelIndex indextmp=ui->tableViewKT->model()->index(m_ktCurIndex,1);
    QString str=ui->tableViewKT->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= ktMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(2,i)*pheConfig->getRatio(2);
        m_ktLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_ktLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_ktLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }
}

void dlgphotoelectricitycfg::on_pushButtonKTNext_clicked()
{


    if((m_ktCurIndex==ktMap.size()-1)|| ktMap.empty())
        return;

    m_ktCurIndex++;

    ui->tableViewKT->selectRow(m_ktCurIndex);
    QModelIndex indextmp=ui->tableViewKT->model()->index(m_ktCurIndex,1);
    QString str=ui->tableViewKT->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= ktMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(2,i)*pheConfig->getRatio(2);
        m_ktLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_ktLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_ktLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }
}

void dlgphotoelectricitycfg::on_tableViewQZ_clicked(const QModelIndex &index)
{

    int row=index.row();
    if(row==-1)
        return;
    m_qzCurIndex=row;


    QModelIndex indextmp=ui->tableViewQZ->model()->index(row,1);
    QString str=ui->tableViewQZ->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= qzMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(1,i)*pheConfig->getRatio(1);
        m_qzLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_qzLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_qzLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }

    }



}

void dlgphotoelectricitycfg::on_tableViewKT_clicked(const QModelIndex &index)
{
    int row=index.row();
    if(row==-1)
        return;
    m_ktCurIndex=row;

    QModelIndex indextmp=ui->tableViewKT->model()->index(row,1);
    QString str=ui->tableViewKT->model()->data(indextmp).toString();

    QMap<QString,QVector<int>>::iterator itor= ktMap.find(str);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for (int i=0;i<itor.value().size();i++)
    {
        unsigned int thresh=pheConfig->getProThreshold(2,i)*pheConfig->getRatio(2);
        m_ktLabels2[i]->setText(QString("%1").arg(itor.value()[i],4,10,QLatin1Char('0')));
        m_ktLabels2[i]->setStyleSheet("color:rgb(0,0,0);font:16pt;");
        if(itor.value()[i]<thresh)
        {
            m_ktLabels2[i]->setStyleSheet("color:rgb(200,0,0);font:16pt;");
        }
    }
}

void dlgphotoelectricitycfg::on_pushButton_qzprobRatio_clicked()
{
    QString strCommand="AA010A00";
    QByteArray byCommand=strCommand.toLatin1();
    QString linestr=ui->lineEdit_qzprobRatio->text();
    double ratio=linestr.toDouble();

    QByteArray value=  QString("%1").arg(int(ratio*100),4,16,QChar('0')).toLatin1();
    byCommand=byCommand.append(value);

    byCommand=byCommand.append(QString("0000").toLatin1()).toUpper();


    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(byCommand,receivedByArray);
    appendInfoText( "QZ", ret, byCommand,receivedByArray);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    //保存参数
    pheConfig->setRatio(1,ratio);
    //    for(int i=0;i<20;i++)
    //    {
    //        int ProbThresh= pheConfig->getProThreshold(1,i)*ratio;

    //        pheConfig->setProThreshold(1,i,ProbThresh);
    //    }

    //调整显示

    int index=ui->comboBox_qzprob->currentIndex();

    int ProbThresh= pheConfig->getProThreshold(1,index)*ratio;

    ui->lineEdit_qzprob->setText(QString::number(ProbThresh));


}

void dlgphotoelectricitycfg::on_pushButton_ktprobRatio_clicked()
{

    QString strCommand="AA010A00";
    QByteArray byCommand=strCommand.toLatin1();
    QString linestr=ui->lineEdit_ktprobRatio->text();
    double ratio=linestr.toDouble();

    QByteArray value=  QString("%1").arg(int(ratio*100),4,16,QChar('0')).toLatin1();
    byCommand=byCommand.append(value);

    byCommand=byCommand.append(QString("0000").toLatin1());

    byCommand=byCommand.toUpper();

    QByteArray receivedByArray;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(byCommand,receivedByArray);
    appendInfoText( "KT", ret, byCommand,receivedByArray);


    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    //保存参数
    pheConfig->setRatio(2,ratio);
    //    for(int i=0;i<20;i++)
    //    {
    //        int ProbThresh= pheConfig->getProThreshold(2,i)*ratio;
    //        pheConfig->setProThreshold(2,i,ProbThresh);
    //    }

    //调整显示

    int index=ui->comboBox_ktprob->currentIndex();

    int ProbThresh= pheConfig->getProThreshold(2,index)*ratio;

    ui->lineEdit_ktprob->setText(QString::number(ProbThresh));



}
