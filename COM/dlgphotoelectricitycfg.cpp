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

    if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_3)
    {
        m_mapProbVal.insert(1,1);
        m_mapProbVal.insert(2,2);
        m_mapProbVal.insert(3,3);
        m_mapProbVal.insert(4,4);
        m_mapProbVal.insert(5,5);
        m_mapProbVal.insert(6,6);
        m_mapProbVal.insert(7,7);
        m_mapProbVal.insert(8,8);
        m_mapProbVal.insert(9,9);
        m_mapProbVal.insert(10,10);
        m_mapProbVal.insert(11,11);
        m_mapProbVal.insert(12,12);
        m_mapProbVal.insert(13,13);
        m_mapProbVal.insert(14,14);
        m_mapProbVal.insert(15,15);
        m_mapProbVal.insert(16,16);
        m_mapProbVal.insert(17,17);
        m_mapProbVal.insert(18,18);
        m_mapProbVal.insert(19,19);
        m_mapProbVal.insert(20,20);

    }

    if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20)
    {
        m_mapProbVal.insert(1,3);
        m_mapProbVal.insert(2,6);
        m_mapProbVal.insert(3,9);
        m_mapProbVal.insert(4,12);
        m_mapProbVal.insert(5,15);
        m_mapProbVal.insert(6,18);
        m_mapProbVal.insert(7,2);
        m_mapProbVal.insert(8,5);
        m_mapProbVal.insert(9,8);
        m_mapProbVal.insert(10,11);
        m_mapProbVal.insert(11,14);
        m_mapProbVal.insert(12,17);
        m_mapProbVal.insert(13,20);
        m_mapProbVal.insert(14,1);
        m_mapProbVal.insert(15,4);
        m_mapProbVal.insert(16,7);
        m_mapProbVal.insert(17,10);
        m_mapProbVal.insert(18,13);
        m_mapProbVal.insert(19,16);
        m_mapProbVal.insert(20,19);

    }



    InitControl();

    connect(this,&dlgphotoelectricitycfg::updataQZADCValueSig,this,&dlgphotoelectricitycfg::showQZAngleAndADCValue);
    connect(this,&dlgphotoelectricitycfg::updataKTADCValueSig,this,&dlgphotoelectricitycfg::showKTAngleAndADCValue);

    connect(this,&dlgphotoelectricitycfg::updataQZADCValueSigSim,this,&dlgphotoelectricitycfg::showQZAngleAndADCValueSim);
    connect(this,&dlgphotoelectricitycfg::updataKTADCValueSigSim,this,&dlgphotoelectricitycfg::showKTAngleAndADCValueSim);

    connect(PackageChecker::getInstance()->m_pBaseCom,&BaseCom::sendCommandRet,this,&dlgphotoelectricitycfg::updateTipText);


  if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_20)
  {
        connect(PackageChecker::getInstance()->m_pBaseCom,&BaseCom::updateCheckRetSig,this,&dlgphotoelectricitycfg::upDateCheckRet);
  }




    m_bQZisRuning=false;
    m_bKTisRuning=false;
    m_bQZisRuningSim=false;
    m_bKTisRuningSim=false;
    m_txtCnt=0;

    m_ktangle=0;
    ktprobv1=0;
    ktprobv2=0;
    ktprobv3=0;

    m_qzangle=0;
    qzprobv1=0;
    qzprobv2=0;
    qzprobv3=0;

    m_bRuning=false;



    ui->radioButton_runMode->setChecked(true);


    ui->groupBox_6->setEnabled(false);
    ui->groupBox_20->setEnabled(false);

    ui->groupBox_13->setEnabled(false);
    ui->groupBox_30->setEnabled(false);



    ui->stackedWidget_QZ->setCurrentIndex(1);
    ui->stackedWidget_KT->setCurrentIndex(1);
    ui->stackedWidget_QZ2->setCurrentIndex(0);
    ui->stackedWidget_KT2->setCurrentIndex(0);




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




    //初始化
    //    for (int i=0;i<20;i++)
    //    {
    //        m_mapValKT.insert(0,0);
    //        m_mapValQZ.insert(0,0);
    //    }
    m_vecValKT.fill(0,20);
    m_vecValQZ.fill(0,20);

    m_vecKickValKT.fill(0,20);
    m_vecKickValQZ.fill(0,20);


    //
    btnGroup=new QButtonGroup(this);
    btnGroup->addButton(ui->radioButton_runMode,1);
    btnGroup->addButton(ui->radioButton_testMode,2);
    btnGroup->addButton(ui->radioButton_testModeSim,3);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge(int)));


    btnGroup2=new QButtonGroup(this);
    btnGroup2->addButton(ui->radioButton_qzcurvalRec,1);
    btnGroup2->addButton(ui->radioButton_qzmeanvalRec,2);
    btnGroup2->addButton(ui->radioButton_qzkickvalRec,3);
    connect(btnGroup2,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge2(int)));


    btnGroup3=new QButtonGroup(this);
    btnGroup3->addButton(ui->radioButton_ktcurvalRec,1);
    btnGroup3->addButton(ui->radioButton_ktmeanvalRec,2);
    btnGroup3->addButton(ui->radioButton_ktkickvalRec,3);
    connect(btnGroup3,SIGNAL(buttonClicked(int)),this,SLOT(buttonJudge3(int)));

    ui->radioButton_ktcurvalRec->setChecked(true);
    ui->radioButton_qzcurvalRec->setChecked(true);


    //加入模拟板之后，控制板中的门槛值和系数丢弃不用
    //    ui->groupBox_8->setVisible(false);
    //    ui->groupBox_12->setVisible(false);
    //    ui->groupBox_25->setVisible(false);
    //    ui->groupBox_27->setVisible(false);

    //
    ui->groupBox_16->setVisible(false);
    ui->groupBox_33->setVisible(false);


    ui->groupBox_19->setVisible(false);
    ui->groupBox_34->setVisible(false);

    //    ui->pushButton_ktcurrent->setEnabled(false);
    //    ui->pushButton_qzcurrent->setEnabled(false);



}

dlgphotoelectricitycfg::~dlgphotoelectricitycfg()
{
    delete ui;
}


void dlgphotoelectricitycfg::upDateCheckRet(int type)
{


    if(type==1)
    {

        //        QThread::msleep(5);
        //        DSDEBUG_<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"upDateCheckRet QZ Ret:"<<endl;
        //        return;

        if(!PackageChecker::getInstance()->m_pBaseCom->m_QZPort.isOpen())
            return;

        bool ret;
        QVector<int> retVec;
        QByteArray Receiveby;

        ret=PackageChecker::getInstance()->m_pBaseCom->getQZData(retVec);
        DSDEBUG_<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"upDateCheckRet QZ Ret:"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheQZ,0,sizeof (int)*20);

        if(!ret)
        {
            //PackageChecker::getInstance()->m_MutexRetPheQZ.unlock();
            return;
        }

        m_mutexMap.lock();
        m_lqzTotalNum++;
        ui->label_qzTotalNum->setText(QString::number(m_lqzTotalNum));
        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];
        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
        bool checkRet=false;
        m_cntQZ++;
        for(int i=2;i<22;i++)
        {
            m_vecValQZ[i-2]=m_vecValQZ[i-2]+retVec[i];

            PackageChecker::getInstance()->RetPheQZ[i-2]=retVec[i];


            m_qzLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");
            unsigned int thresh= pheConfig->getProThreshold(1,i-2)*pheConfig->getRatio(1);
            if(retVec[i]<thresh)
            {
                checkRet=true;
                m_vecKickValQZ[i-2]++;

                m_qzLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_qzQue.enqueue(tmpPari);
                m_qzQue.dequeue();

                DSDEBUG__<<i<<",qz thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }


            //  if(ui->radioButton_qzcurvalRec->isChecked())
            {
                m_qzLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            }

            if(ui->radioButton_qzmeanvalRec->isChecked())
            {
                m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecValQZ[i-2]/m_cntQZ,4,10,QLatin1Char('0')));
            }
            if(ui->radioButton_qzkickvalRec->isChecked())
            {
                m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecKickValQZ[i-2],4,10,QLatin1Char('0')));
            }
        }
        PackageChecker::getInstance()->RetPheQZ[20]=checkRet;


        ui->label_qzLastNGValue2->setText(QString("%1-%2").arg(m_qzQue[0].first,2,10,QChar('0')).arg(m_qzQue[0].second,4,10,QChar('0')));
        ui->label_qzLastNGValue1->setText(QString("%1-%2").arg(m_qzQue[1].first,2,10,QChar('0')).arg(m_qzQue[1].second,4,10,QChar('0')));

        if(checkRet)
        {
            m_lqzNGNum++;
            retVec.erase(retVec.begin(),retVec.begin()+2);

            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_qzMap.insert(date,retVec);
            if(m_qzMap.size()>20)
            {
                m_qzMap.erase(m_qzMap.begin());
            }

        }
        ui->label_qzNGNum->setText(QString::number(m_lqzNGNum));

        double ratio=(m_lqzNGNum/(m_lqzTotalNum*1.0))*100;
        ui->label_qzNGRatio->setText(QString::number(ratio,'f',2));


        m_mutexMap.unlock();
    }


    if(type==2)
    {

        //        QThread::msleep(5);
        //        DSDEBUG_<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"upDateCheckRet KT Ret:"<<endl;
        //        return;

        if(!PackageChecker::getInstance()->m_pBaseCom->m_KTPort.isOpen())
            return;

        bool ret;
        QVector<int> retVec;

        ret=PackageChecker::getInstance()->m_pBaseCom->getKTData(retVec);
        DSDEBUG_<<QTime::currentTime().toString("HH-mm-ss-zzz")<<","<<"upDateCheckRet KT Ret :"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheKT,0,sizeof (int)*21);
        if(!ret)
        {
            //PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
            return;
        }

        m_mutexMapKT.lock();
        m_lktTotalNum++;
        ui->label_ktTotalNum->setText(QString::number(m_lktTotalNum));
        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];
        m_cntKT++;
        bool checkRet=false;
        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
        for(int i=2;i<22;i++)
        {
            m_vecValKT[i-2]=m_vecValKT[i-2]+retVec[i];

            PackageChecker::getInstance()->RetPheKT[i-2]=retVec[i];
            m_ktLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");
            unsigned int thresh= pheConfig->getProThreshold(2,i-2)*pheConfig->getRatio(2);

            if(retVec[i]<thresh)
            {
                checkRet=true;
                m_vecKickValKT[i-2]++;

                m_ktLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_ktQue.enqueue(tmpPari);
                m_ktQue.dequeue();

                DSDEBUG__<<i<<",kt thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }

            //if(ui->radioButton_ktcurvalRec->isChecked())
            {
                m_ktLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            }

            if(ui->radioButton_ktmeanvalRec->isChecked())
            {
                m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecValKT[i-2]/m_cntKT,4,10,QLatin1Char('0')));
            }
            if(ui->radioButton_ktkickvalRec->isChecked())
            {
                m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecKickValKT[i-2],4,10,QLatin1Char('0')));
            }
        }

        PackageChecker::getInstance()->RetPheKT[20]=checkRet;


        ui->label_ktLastNGValue2->setText(QString("%1-%2").arg(m_ktQue[0].first,2,10,QChar('0')).arg(m_ktQue[0].second,4,10,QChar('0')));
        ui->label_ktLastNGValue1->setText(QString("%1-%2").arg(m_ktQue[1].first,2,10,QChar('0')).arg(m_ktQue[1].second,4,10,QChar('0')));

        if(checkRet)
        {
            m_lktNGNum++;

            retVec.erase(retVec.begin(),retVec.begin()+2);


            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_ktMap.insert(date,retVec);
            if(m_ktMap.size()>20)
            {
                m_ktMap.erase(m_ktMap.begin());
            }

        }
        ui->label_ktNGNum->setText(QString::number(m_lktNGNum));

        double ratio=(m_lktNGNum/(m_lktTotalNum*1.0))*100;
        ui->label_ktNGRatio->setText(QString::number(ratio,'f',2));

        m_mutexMapKT.unlock();
    }


    if(type==3)
    {
        //PackageChecker::getInstance()->m_MutexRetPheKT.lock();

        if(!PackageChecker::getInstance()->m_pBaseCom->m_QZPortSim.isOpen())
            return;

        bool ret;
        QVector<int> retVecTmp;
        QVector<int> retVec(22);
        QByteArray receiveby;

        ret=PackageChecker::getInstance()->m_pBaseCom->getQZDataSim(retVecTmp,receiveby);
        DSDEBUG_<<"upDateCheckRet KT Ret :"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheQZ,0,sizeof (int)*21);
        if(!ret)
        {
            //PackageChecker::getInstance()->m_MutexRetPheKT.unlock();
            return;
        }

        //改变次序
        retVec[0]=retVecTmp[0];
        retVec[1]=retVecTmp[1];
        retVec[2]=retVecTmp[4];
        retVec[3]=retVecTmp[7];
        retVec[4]=retVecTmp[10];
        retVec[5]=retVecTmp[13];
        retVec[6]=retVecTmp[16];
        retVec[7]=retVecTmp[19];
        retVec[8]=retVecTmp[3];
        retVec[9]=retVecTmp[6];
        retVec[10]=retVecTmp[9];
        retVec[11]=retVecTmp[12];
        retVec[12]=retVecTmp[15];
        retVec[13]=retVecTmp[18];
        retVec[14]=retVecTmp[21];
        retVec[15]=retVecTmp[2];
        retVec[16]=retVecTmp[5];
        retVec[17]=retVecTmp[8];
        retVec[18]=retVecTmp[11];
        retVec[19]=retVecTmp[14];
        retVec[20]=retVecTmp[17];
        retVec[21]=retVecTmp[20];

        m_mutexMap.lock();
        m_lqzTotalNum++;
        ui->label_qzTotalNum->setText(QString::number(m_lqzTotalNum));
        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];
        m_cntQZ++;
        bool checkRet=false;
        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
        for(int i=2;i<22;i++)
        {
            m_vecValQZ[i-2]=m_vecValQZ[i-2]+retVec[i];

            PackageChecker::getInstance()->RetPheQZ[i-2]=retVec[i];
            m_qzLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");
            unsigned int thresh= pheConfig->getProThresholdSim(1,m_mapProbVal[i-2+1]-1)*pheConfig->getRatioSim(1);

            if(retVec[i]<thresh)
            {
                checkRet=true;
                PackageChecker::getInstance()-> RetMapPheQZ[i-2]=true;

                m_vecKickValQZ[i-2]++;

                m_qzLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_qzQue.enqueue(tmpPari);
                m_qzQue.dequeue();
                DSDEBUG__<<i<<",qz thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }

            //if(ui->radioButton_ktcurvalRec->isChecked())
            {
                m_qzLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            }

            if(ui->radioButton_qzmeanvalRec->isChecked())
            {
                m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecValQZ[i-2]/m_cntQZ,4,10,QLatin1Char('0')));
            }
            if(ui->radioButton_qzkickvalRec->isChecked())
            {
                m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecKickValQZ[i-2],4,10,QLatin1Char('0')));
            }
        }
        PackageChecker::getInstance()->RetPheQZ[20]=checkRet;



        ui->label_qzLastNGValue2->setText(QString("%1-%2").arg(m_qzQue[0].first,2,10,QChar('0')).arg(m_qzQue[0].second,4,10,QChar('0')));
        ui->label_qzLastNGValue1->setText(QString("%1-%2").arg(m_qzQue[1].first,2,10,QChar('0')).arg(m_qzQue[1].second,4,10,QChar('0')));

        if(checkRet)
        {
            m_lqzNGNum++;

            retVec.erase(retVec.begin(),retVec.begin()+2);


            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_qzMap.insert(date,retVec);
            if(m_qzMap.size()>20)
            {
                m_qzMap.erase(m_qzMap.begin());
            }
        }
        ui->label_qzNGNum->setText(QString::number(m_lqzNGNum));

        double ratio=(m_lqzNGNum/(m_lqzTotalNum*1.0))*100;
        ui->label_qzNGRatio->setText(QString::number(ratio,'f',2));

        m_mutexMap.unlock();
    }

    if(type==4)
    {
        //PackageChecker::getInstance()->m_MutexRetPheKT.lock();

        DSDEBUG_<<"upDateCheckRet KT Ret :"<<endl;


        //测试
        //        for(int i=0;i<20;i++)
        //        {
        //            PackageChecker::getInstance()->RetPheKT[i]++;
        //            PackageChecker::getInstance()->RetMapPheKT[i]++;
        //        }
        //         PackageChecker::getInstance()->RetPheKT[20]++;
        //         emit updateMainSceenSig();



        if(!PackageChecker::getInstance()->m_pBaseCom->m_KTPortSim.isOpen())
            return;

        bool ret;
        QVector<int> retVecTmp;
        QVector<int> retVec(22);
        QByteArray receiveby;

        ret=PackageChecker::getInstance()->m_pBaseCom->getKTDataSim(retVecTmp,receiveby);
        DSDEBUG_<<"upDateCheckRet KT Ret :"<<ret<<endl;
        memset(PackageChecker::getInstance()->RetPheKT,0,sizeof (int)*21);
        memset(PackageChecker::getInstance()->RetMapPheKT,0,sizeof (int)*20);

        if(!ret)
        {

            m_lktTotalNum++;
            ui->label_ktTotalNum->setText(QString::number(m_lktTotalNum));
            emit updateMainSceenSig();
            return;
        }

        //改变次序//模拟板数据乱序
        retVec[0]=retVecTmp[0];
        retVec[1]=retVecTmp[1];
        retVec[2]=retVecTmp[4];
        retVec[3]=retVecTmp[7];
        retVec[4]=retVecTmp[10];
        retVec[5]=retVecTmp[13];
        retVec[6]=retVecTmp[16];
        retVec[7]=retVecTmp[19];
        retVec[8]=retVecTmp[3];
        retVec[9]=retVecTmp[6];
        retVec[10]=retVecTmp[9];
        retVec[11]=retVecTmp[12];
        retVec[12]=retVecTmp[15];
        retVec[13]=retVecTmp[18];
        retVec[14]=retVecTmp[21];
        retVec[15]=retVecTmp[2];
        retVec[16]=retVecTmp[5];
        retVec[17]=retVecTmp[8];
        retVec[18]=retVecTmp[11];
        retVec[19]=retVecTmp[14];
        retVec[20]=retVecTmp[17];
        retVec[21]=retVecTmp[20];



        m_mutexMapKT.lock();
        m_lktTotalNum++;
        ui->label_ktTotalNum->setText(QString::number(m_lktTotalNum));
        PackageChecker::getInstance()->ErrPhe[type-1]=retVec[1];
        m_cntKT++;


        bool checkRet=false;
        ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
        for(int i=2;i<22;i++)
        {
            m_vecValKT[i-2]=m_vecValKT[i-2]+retVec[i];

            PackageChecker::getInstance()->RetPheKT[i-2]=retVec[i];
            m_ktLabels[i-2]->setStyleSheet("color:rgb(0,0,0);font:18pt;");
            unsigned int thresh= pheConfig->getProThresholdSim(2, m_mapProbVal[i-2+1]-1)*pheConfig->getRatioSim(2);


            if(retVec[i]<thresh)
            {
                checkRet=true;
                PackageChecker::getInstance()-> RetMapPheKT[i-2]=true;

                m_vecKickValKT[i-2]++;

                m_ktLabels[i-2]->setStyleSheet("color:rgb(200,0,0);font:18pt;");
                QPair<int ,int> tmpPari(i-1,retVec[i]);
                m_ktQue.enqueue(tmpPari);
                m_ktQue.dequeue();

                DSDEBUG__<<i<<",kt thresh, value"<<thresh<<","<<retVec[i]<<endl;
            }

            //  if(ui->radioButton_ktcurvalRec->isChecked())
            {
                m_ktLabels[i-2]->setText(QString("%1").arg(retVec[i],4,10,QLatin1Char('0')));
            }

            if(ui->radioButton_ktmeanvalRec->isChecked())
            {
                m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecValKT[i-2]/m_cntKT,4,10,QLatin1Char('0')));
            }
            if(ui->radioButton_ktkickvalRec->isChecked())
            {
                m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecKickValKT[i-2],4,10,QLatin1Char('0')));
            }
        }

        PackageChecker::getInstance()->RetPheKT[20]=checkRet;



        ui->label_ktLastNGValue2->setText(QString("%1-%2").arg(m_ktQue[0].first,2,10,QChar('0')).arg(m_ktQue[0].second,4,10,QChar('0')));
        ui->label_ktLastNGValue1->setText(QString("%1-%2").arg(m_ktQue[1].first,2,10,QChar('0')).arg(m_ktQue[1].second,4,10,QChar('0')));

        if(checkRet)
        {
            m_lktNGNum++;

            retVec.erase(retVec.begin(),retVec.begin()+2);


            QString date=QDateTime::currentDateTime().toString("yyyyMMdd-HHmmsszzz");
            m_ktMap.insert(date,retVec);
            if(m_ktMap.size()>20)
            {
                m_ktMap.erase(m_ktMap.begin());
            }

        }
        ui->label_ktNGNum->setText(QString::number(m_lktNGNum));

        double ratio=(m_lktNGNum/(m_lktTotalNum*1.0))*100;
        ui->label_ktNGRatio->setText(QString::number(ratio,'f',2));

        m_mutexMapKT.unlock();
        emit updateMainSceenSig();

    }


    qDebug()<<"upDateCheckRet Finished "<<endl;
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


    //模拟板
    connect(ui->lineEdit_qzcurrent,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzgain,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzprobSim,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzprobRatioSim,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_qzcurrentRatio,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    //ui->lineEdit_qzprobRatio->setValidator(new QDoubleValidator(0.0,2.0,2,this));

    connect(ui->lineEdit_ktcurrent,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktgain,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktprobSim,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktprobRatioSim,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEdit_ktcurrentRatio,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


    m_qzLabelsSim.clear();
    m_qzLabelsCurrentSim.clear();
    m_qzLabelsGainSim.clear();
    //    for (int i=0;i<20;i++)
    {
        m_qzLabelsSim.insert(0,ui->label_qzsim1);
        m_qzLabelsSim.insert(1,ui->label_qzsim2);
        m_qzLabelsSim.insert(2,ui->label_qzsim3);
        m_qzLabelsSim.insert(3,ui->label_qzsim4);
        m_qzLabelsSim.insert(4,ui->label_qzsim5);
        m_qzLabelsSim.insert(5,ui->label_qzsim6);
        m_qzLabelsSim.insert(6,ui->label_qzsim7);
        m_qzLabelsSim.insert(7,ui->label_qzsim8);
        m_qzLabelsSim.insert(8,ui->label_qzsim9);
        m_qzLabelsSim.insert(9,ui->label_qzsim10);

        m_qzLabelsSim.insert(10,ui->label_qzsim11);
        m_qzLabelsSim.insert(11,ui->label_qzsim12);
        m_qzLabelsSim.insert(12,ui->label_qzsim13);
        m_qzLabelsSim.insert(13,ui->label_qzsim14);
        m_qzLabelsSim.insert(14,ui->label_qzsim15);
        m_qzLabelsSim.insert(15,ui->label_qzsim16);
        m_qzLabelsSim.insert(16,ui->label_qzsim17);
        m_qzLabelsSim.insert(17,ui->label_qzsim18);
        m_qzLabelsSim.insert(18,ui->label_qzsim19);
        m_qzLabelsSim.insert(19,ui->label_qzsim20);

        m_qzLabelsCurrentSim.insert(0,ui->label_qzcurrent1);
        m_qzLabelsCurrentSim.insert(1,ui->label_qzcurrent2);
        m_qzLabelsCurrentSim.insert(2,ui->label_qzcurrent3);
        m_qzLabelsCurrentSim.insert(3,ui->label_qzcurrent4);
        m_qzLabelsCurrentSim.insert(4,ui->label_qzcurrent5);
        m_qzLabelsCurrentSim.insert(5,ui->label_qzcurrent6);
        m_qzLabelsCurrentSim.insert(6,ui->label_qzcurrent7);
        m_qzLabelsCurrentSim.insert(7,ui->label_qzcurrent8);
        m_qzLabelsCurrentSim.insert(8,ui->label_qzcurrent9);
        m_qzLabelsCurrentSim.insert(9,ui->label_qzcurrent10);
        m_qzLabelsCurrentSim.insert(10,ui->label_qzcurrent11);
        m_qzLabelsCurrentSim.insert(11,ui->label_qzcurrent12);
        m_qzLabelsCurrentSim.insert(12,ui->label_qzcurrent13);
        m_qzLabelsCurrentSim.insert(13,ui->label_qzcurrent14);
        m_qzLabelsCurrentSim.insert(14,ui->label_qzcurrent15);
        m_qzLabelsCurrentSim.insert(15,ui->label_qzcurrent16);
        m_qzLabelsCurrentSim.insert(16,ui->label_qzcurrent17);
        m_qzLabelsCurrentSim.insert(17,ui->label_qzcurrent18);
        m_qzLabelsCurrentSim.insert(18,ui->label_qzcurrent19);
        m_qzLabelsCurrentSim.insert(19,ui->label_qzcurrent20);

        m_qzLabelsGainSim.insert(0,ui->label_qzgain1);
        m_qzLabelsGainSim.insert(1,ui->label_qzgain2);
        m_qzLabelsGainSim.insert(2,ui->label_qzgain3);
        m_qzLabelsGainSim.insert(3,ui->label_qzgain4);
        m_qzLabelsGainSim.insert(4,ui->label_qzgain5);
        m_qzLabelsGainSim.insert(5,ui->label_qzgain6);
        m_qzLabelsGainSim.insert(6,ui->label_qzgain7);
        m_qzLabelsGainSim.insert(7,ui->label_qzgain8);
        m_qzLabelsGainSim.insert(8,ui->label_qzgain9);
        m_qzLabelsGainSim.insert(9,ui->label_qzgain10);
        m_qzLabelsGainSim.insert(10,ui->label_qzgain11);
        m_qzLabelsGainSim.insert(11,ui->label_qzgain12);
        m_qzLabelsGainSim.insert(12,ui->label_qzgain13);
        m_qzLabelsGainSim.insert(13,ui->label_qzgain14);
        m_qzLabelsGainSim.insert(14,ui->label_qzgain15);
        m_qzLabelsGainSim.insert(15,ui->label_qzgain16);
        m_qzLabelsGainSim.insert(16,ui->label_qzgain17);
        m_qzLabelsGainSim.insert(17,ui->label_qzgain18);
        m_qzLabelsGainSim.insert(18,ui->label_qzgain19);
        m_qzLabelsGainSim.insert(19,ui->label_qzgain20);

    }

    m_ktLabelsSim.clear();
    {
        m_ktLabelsSim.insert(0,ui->label_ktsim1);
        m_ktLabelsSim.insert(1,ui->label_ktsim2);
        m_ktLabelsSim.insert(2,ui->label_ktsim3);
        m_ktLabelsSim.insert(3,ui->label_ktsim4);
        m_ktLabelsSim.insert(4,ui->label_ktsim5);
        m_ktLabelsSim.insert(5,ui->label_ktsim6);
        m_ktLabelsSim.insert(6,ui->label_ktsim7);
        m_ktLabelsSim.insert(7,ui->label_ktsim8);
        m_ktLabelsSim.insert(8,ui->label_ktsim9);
        m_ktLabelsSim.insert(9,ui->label_ktsim10);

        m_ktLabelsSim.insert(10,ui->label_ktsim11);
        m_ktLabelsSim.insert(11,ui->label_ktsim12);
        m_ktLabelsSim.insert(12,ui->label_ktsim13);
        m_ktLabelsSim.insert(13,ui->label_ktsim14);
        m_ktLabelsSim.insert(14,ui->label_ktsim15);
        m_ktLabelsSim.insert(15,ui->label_ktsim16);
        m_ktLabelsSim.insert(16,ui->label_ktsim17);
        m_ktLabelsSim.insert(17,ui->label_ktsim18);
        m_ktLabelsSim.insert(18,ui->label_ktsim19);
        m_ktLabelsSim.insert(19,ui->label_ktsim20);


        m_ktLabelsCurrentSim.insert(0,ui->label_ktcurrent1);
        m_ktLabelsCurrentSim.insert(1,ui->label_ktcurrent2);
        m_ktLabelsCurrentSim.insert(2,ui->label_ktcurrent3);
        m_ktLabelsCurrentSim.insert(3,ui->label_ktcurrent4);
        m_ktLabelsCurrentSim.insert(4,ui->label_ktcurrent5);
        m_ktLabelsCurrentSim.insert(5,ui->label_ktcurrent6);
        m_ktLabelsCurrentSim.insert(6,ui->label_ktcurrent7);
        m_ktLabelsCurrentSim.insert(7,ui->label_ktcurrent8);
        m_ktLabelsCurrentSim.insert(8,ui->label_ktcurrent9);
        m_ktLabelsCurrentSim.insert(9,ui->label_ktcurrent10);
        m_ktLabelsCurrentSim.insert(10,ui->label_ktcurrent11);
        m_ktLabelsCurrentSim.insert(11,ui->label_ktcurrent12);
        m_ktLabelsCurrentSim.insert(12,ui->label_ktcurrent13);
        m_ktLabelsCurrentSim.insert(13,ui->label_ktcurrent14);
        m_ktLabelsCurrentSim.insert(14,ui->label_ktcurrent15);
        m_ktLabelsCurrentSim.insert(15,ui->label_ktcurrent16);
        m_ktLabelsCurrentSim.insert(16,ui->label_ktcurrent17);
        m_ktLabelsCurrentSim.insert(17,ui->label_ktcurrent18);
        m_ktLabelsCurrentSim.insert(18,ui->label_ktcurrent19);
        m_ktLabelsCurrentSim.insert(19,ui->label_ktcurrent20);

        m_ktLabelsGainSim.insert(0,ui->label_ktgain1);
        m_ktLabelsGainSim.insert(1,ui->label_ktgain2);
        m_ktLabelsGainSim.insert(2,ui->label_ktgain3);
        m_ktLabelsGainSim.insert(3,ui->label_ktgain4);
        m_ktLabelsGainSim.insert(4,ui->label_ktgain5);
        m_ktLabelsGainSim.insert(5,ui->label_ktgain6);
        m_ktLabelsGainSim.insert(6,ui->label_ktgain7);
        m_ktLabelsGainSim.insert(7,ui->label_ktgain8);
        m_ktLabelsGainSim.insert(8,ui->label_ktgain9);
        m_ktLabelsGainSim.insert(9,ui->label_ktgain10);
        m_ktLabelsGainSim.insert(10,ui->label_ktgain11);
        m_ktLabelsGainSim.insert(11,ui->label_ktgain12);
        m_ktLabelsGainSim.insert(12,ui->label_ktgain13);
        m_ktLabelsGainSim.insert(13,ui->label_ktgain14);
        m_ktLabelsGainSim.insert(14,ui->label_ktgain15);
        m_ktLabelsGainSim.insert(15,ui->label_ktgain16);
        m_ktLabelsGainSim.insert(16,ui->label_ktgain17);
        m_ktLabelsGainSim.insert(17,ui->label_ktgain18);
        m_ktLabelsGainSim.insert(18,ui->label_ktgain19);
        m_ktLabelsGainSim.insert(19,ui->label_ktgain20);
    }

    ui->lineEdit_qzcurrent->setText(QString::number(pheConfig->getProCurrentSim(1,0)));
    ui->lineEdit_qzgain->setText(QString::number(pheConfig->getProGainSim(1,0)));

    ratio=pheConfig->getRatioSim(1);
    ui->lineEdit_qzprobSim->setText(QString::number(int(pheConfig->getProThresholdSim(1,0)*ratio)));
    ui->lineEdit_qzprobRatioSim->setText(QString::number(pheConfig->getRatioSim(1),'f',2));


    ui->lineEdit_ktcurrent->setText(QString::number(pheConfig->getProCurrentSim(2,0)));
    ui->lineEdit_ktgain->setText(QString::number(pheConfig->getProGainSim(2,0)));

    ratio=pheConfig->getRatioSim(2);
    ui->lineEdit_ktprobSim->setText(QString::number(int(pheConfig->getProThresholdSim(2,0)*ratio)));
    ui->lineEdit_ktprobRatioSim->setText(QString::number(pheConfig->getRatioSim(2),'f',2));


    ui->lineEdit_ktcurrentCodeVal->setText(QString::number(pheConfig->getProCurrentCodeValSim(2,m_mapProbVal[0+1]-1)));

    ui->lineEdit_qzcurrentCodeVal->setText(QString::number(pheConfig->getProCurrentCodeValSim(1,m_mapProbVal[1]-1)));


    if(PackageChecker::getInstance()->Options->getProbNum()==ENUMPROBNUM_3)
    {


        ui->comboBox_qzcurrent->clear();

        ui-> comboBox_qzcurrent->addItem(QString());
        ui->comboBox_qzcurrent->addItem(QString());
        ui->comboBox_qzcurrent->addItem(QString());

        ui->comboBox_qzcurrent->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg","\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2601", nullptr));
        ui->comboBox_qzcurrent->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2602", nullptr));
        ui-> comboBox_qzcurrent->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2603", nullptr));



        ui->comboBox_qzgain->clear();
        ui-> comboBox_qzgain->addItem(QString());
        ui->comboBox_qzgain->addItem(QString());
        ui->comboBox_qzgain->addItem(QString());
        ui->comboBox_qzgain->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2121", nullptr));
        ui->comboBox_qzgain->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2122", nullptr));
        ui-> comboBox_qzgain->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2123", nullptr));

        ui->comboBox_qzcurrentRatio->clear();
        ui-> comboBox_qzcurrentRatio->addItem(QString());
        ui->comboBox_qzcurrentRatio->addItem(QString());
        ui->comboBox_qzcurrentRatio->addItem(QString());
        ui->comboBox_qzcurrentRatio->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg","\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2601", nullptr));
        ui->comboBox_qzcurrentRatio->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2602", nullptr));
        ui-> comboBox_qzcurrentRatio->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2603", nullptr));





        ui->comboBox_ktcurrent->clear();
        ui-> comboBox_ktcurrent->addItem(QString());
        ui->comboBox_ktcurrent->addItem(QString());
        ui->comboBox_ktcurrent->addItem(QString());

        ui->comboBox_ktcurrent->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg",  "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2601", nullptr));
        ui->comboBox_ktcurrent->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2602", nullptr));
        ui-> comboBox_ktcurrent->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2603", nullptr));



        ui->comboBox_ktgain->clear();
        ui-> comboBox_ktgain->addItem(QString());
        ui->comboBox_ktgain->addItem(QString());
        ui->comboBox_ktgain->addItem(QString());


        ui->comboBox_ktgain->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2121", nullptr));
        ui->comboBox_ktgain->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2122", nullptr));
        ui-> comboBox_ktgain->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\345\242\236\347\233\2123", nullptr));


        ui->comboBox_ktcurrentRatio->clear();
        ui->comboBox_ktcurrentRatio->addItem(QString());
        ui->comboBox_ktcurrentRatio->addItem(QString());
        ui->comboBox_ktcurrentRatio->addItem(QString());


        ui->comboBox_ktcurrentRatio->setItemText(0, QCoreApplication::translate("dlgphotoelectricitycfg",  "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2601", nullptr));
        ui->comboBox_ktcurrentRatio->setItemText(1, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2602", nullptr));
        ui-> comboBox_ktcurrentRatio->setItemText(2, QCoreApplication::translate("dlgphotoelectricitycfg", "\346\216\242\345\244\264\347\224\265\346\265\201\347\263\273\346\225\2603", nullptr));

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
        txtTip=settingName+",Set Finish,"+"send:"+QString(text)+",receive:"+QString(Receivetext);
    }
    else
    {
        txtTip=settingName+",Set Finish,"+"send:"+QString(text)+",receive:"+QString(Receivetext);
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

QByteArray dlgphotoelectricitycfg::genSendByArraySim(int OperaCode, int v1, int v2)
{
    QByteArray sendText=QByteArray::fromHex("5501");

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
        QThread::msleep(400);

        emit updataQZADCValueSig();
    }
}

void dlgphotoelectricitycfg::startKTTestRunModel()
{

    while (m_bKTisRuning)
    {

        //调试模式，读取当前角度，ADC采样值
        QThread::msleep(400);

        emit updataKTADCValueSig();
    }
}

void dlgphotoelectricitycfg::startQZTestRunModelSim()
{
    while (m_bQZisRuningSim)
    {
        QThread::msleep(400);

        emit updataQZADCValueSigSim();
    }
}

void dlgphotoelectricitycfg::startKTTestRunModelSim()
{
    while (m_bKTisRuningSim)
    {
        QThread::msleep(400);

        emit updataKTADCValueSigSim();
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
    //    PackageChecker::getInstance()->m_isUpdateEPH=false;

    if(m_bQZisRuning)
        on_pushButton_qzStopApply_clicked();
    if(m_bKTisRuning)
        on_pushButton_ktStopApply_clicked();
    if(m_bQZisRuningSim)
        on_pushButton_qzStopApplySim_clicked();
    if(m_bKTisRuningSim)
        on_pushButton_ktStopApplySim_clicked();


    ui->stackedWidget_QZ->setCurrentIndex(1);
    ui->stackedWidget_KT->setCurrentIndex(1);

    ui->radioButton_runMode->setChecked(true);
    PackageChecker::getInstance()->m_pBaseCom->togleConnect();

    ui->groupBox_6->setEnabled(false);
    ui->groupBox_20->setEnabled(false);

    ui->groupBox_13->setEnabled(false);
    ui->groupBox_30->setEnabled(false);

    PackageChecker::getInstance()->m_pBaseCom->clearQue();
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


    //获取参数指令
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

void dlgphotoelectricitycfg::showQZAngleAndADCValueSim()
{

    //调试模式，读取当前角度，ADC采样值
    //  QByteArray sendText=QByteArray::fromHex("5502000100000000");
    QByteArray sendText=QByteArray::fromHex("AA02010100000000");
    sendText=sendText.toHex().toUpper();

    QByteArray receivedByArray;
    QVector<int> retVecTmp;
    QVector<int> retVec(22);

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,retVecTmp,2);

    if(ret)
    {
        if(receivedByArray.size()==90)
        {
            bool ok;
            QByteArray tmpReadData=receivedByArray;

            QByteArray mid= tmpReadData.mid(2,2);
            int  protime=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            retVecTmp.push_back(protime);

            mid= tmpReadData.mid(4,2);
            int chekcret=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            retVecTmp.push_back(chekcret);

            for(int i=0;i<20;i++)
            {
                mid= tmpReadData.mid(6+i*4,4);
                int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
                retVecTmp.push_back(value);
            }
        }

        //改变次序
        retVec[0]=retVecTmp[0];
        retVec[1]=retVecTmp[1];
        retVec[2]=retVecTmp[4];
        retVec[3]=retVecTmp[7];
        retVec[4]=retVecTmp[10];
        retVec[5]=retVecTmp[13];
        retVec[6]=retVecTmp[16];
        retVec[7]=retVecTmp[19];
        retVec[8]=retVecTmp[3];
        retVec[9]=retVecTmp[6];
        retVec[10]=retVecTmp[9];
        retVec[11]=retVecTmp[12];
        retVec[12]=retVecTmp[15];
        retVec[13]=retVecTmp[18];
        retVec[14]=retVecTmp[21];
        retVec[15]=retVecTmp[2];
        retVec[16]=retVecTmp[5];
        retVec[17]=retVecTmp[8];
        retVec[18]=retVecTmp[11];
        retVec[19]=retVecTmp[14];
        retVec[20]=retVecTmp[17];
        retVec[21]=retVecTmp[20];

        for(int i=0;i<20;i++)
        {
            m_qzLabelsSim[i]->setText(QString("%1").arg(retVec[i+2],4,10,QLatin1Char('0')));
        }
    }
}

void dlgphotoelectricitycfg::showKTAngleAndADCValueSim()
{
    //调试模式，读取当前角度，ADC采样值

    QByteArray sendText=QByteArray::fromHex("AA02010100000000");
    sendText=sendText.toHex().toUpper();

    QByteArray receivedByArray;
    QVector<int> retVecTmp;
    QVector<int> retVec(22);

    QElapsedTimer elaps;
    elaps.start();

    //    tmp++;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,retVecTmp,2);

    if(ret)
    {
        int sz=receivedByArray.size();

        if(receivedByArray.size()==90)
        {
            bool ok;
            QByteArray tmpReadData=receivedByArray;

            QByteArray mid= tmpReadData.mid(2,2);
            int  protime=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            retVecTmp.push_back(protime);

            mid= tmpReadData.mid(4,2);
            int chekcret=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            retVecTmp.push_back(chekcret);

            for(int i=0;i<20;i++)
            {
                mid= tmpReadData.mid(6+i*4,4);
                int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
                //                if(i==0)
                //                    value=tmp;
                retVecTmp.push_back(value);
            }
        }


        //改变次序
        retVec[0]=retVecTmp[0];
        retVec[1]=retVecTmp[1];
        retVec[2]=retVecTmp[4];
        retVec[3]=retVecTmp[7];
        retVec[4]=retVecTmp[10];
        retVec[5]=retVecTmp[13];
        retVec[6]=retVecTmp[16];
        retVec[7]=retVecTmp[19];
        retVec[8]=retVecTmp[3];
        retVec[9]=retVecTmp[6];
        retVec[10]=retVecTmp[9];
        retVec[11]=retVecTmp[12];
        retVec[12]=retVecTmp[15];
        retVec[13]=retVecTmp[18];
        retVec[14]=retVecTmp[21];
        retVec[15]=retVecTmp[2];
        retVec[16]=retVecTmp[5];
        retVec[17]=retVecTmp[8];
        retVec[18]=retVecTmp[11];
        retVec[19]=retVecTmp[14];
        retVec[20]=retVecTmp[17];
        retVec[21]=retVecTmp[20];


        for(int i=0;i<20;i++)
        {
            m_ktLabelsSim[i]->setText(QString("%1").arg(retVec[i+2],4,10,QLatin1Char('0')));
        }
    }
    DSDEBUG_<<"elaps:" <<elaps.elapsed()<<endl;
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
    on_pushButtonQZRefresh_clicked();
}

void dlgphotoelectricitycfg::on_pushButtonQZResetNum_clicked()
{
    m_mutexMap.lock();
    m_lqzTotalNum=0;
    m_lqzNGNum=0;
    ui->label_qzTotalNum->setText("0");
    ui->label_qzNGNum->setText("0");
    ui->label_qzNGRatio->setText("0");

    m_cntQZ=0;
    m_vecValQZ.fill(0,20);
    m_vecKickValQZ.fill(0,20);
    m_mutexMap.unlock();
}

void dlgphotoelectricitycfg::on_pushButtonKTRun_clicked()
{
    ui->stackedWidget_KT2->setCurrentIndex(0);
}

void dlgphotoelectricitycfg::on_pushButtonKTRecord_clicked()
{

    ui->stackedWidget_KT2->setCurrentIndex(1);

    on_pushButtonKTRefresh_clicked();
}

void dlgphotoelectricitycfg::on_pushButtonKTResetNum_clicked()
{
    m_mutexMapKT.lock();

    m_lktTotalNum=0;
    m_lktNGNum=0;
    ui->label_ktTotalNum->setText("0");
    ui->label_ktNGNum->setText("0");
    ui->label_ktNGRatio->setText("0");

    m_cntKT=0;
    m_vecValKT.fill(0,20);
    m_vecKickValKT.fill(0,20);
    m_mutexMapKT.unlock();
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

void dlgphotoelectricitycfg::buttonJudge(int)
{
    if(ui->radioButton_runMode->isChecked())
    {
        if(m_bRuning==true)
            return;

        on_pushButton_qzStopApply_clicked();
        on_pushButton_ktStopApply_clicked();

        on_pushButton_qzStopApplySim_clicked();
        on_pushButton_ktStopApplySim_clicked();

        ui->stackedWidget_QZ->setCurrentIndex(1);
        ui->stackedWidget_KT->setCurrentIndex(1);

        PackageChecker::getInstance()->m_pBaseCom->togleConnect();
        ui->groupBox_6->setEnabled(false);
        ui->groupBox_20->setEnabled(false);

        ui->groupBox_13->setEnabled(false);
        ui->groupBox_30->setEnabled(false);


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

        if(ui->radioButton_testModeSim->isChecked())
        {
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);

            ui->stackedWidget_QZ->setCurrentIndex(2);
            ui->stackedWidget_KT->setCurrentIndex(2);

            on_pushButton_qzStopApply_clicked();
            on_pushButton_ktStopApply_clicked();


        }
        else
        {

            ui->stackedWidget->setCurrentIndex(0);
            ui->stackedWidget_2->setCurrentIndex(0);

            ui->stackedWidget_QZ->setCurrentIndex(0);
            ui->stackedWidget_KT->setCurrentIndex(0);

            on_pushButton_qzStopApplySim_clicked();
            on_pushButton_ktStopApplySim_clicked();
        }

        ui->groupBox_6->setEnabled(true);
        ui->groupBox_20->setEnabled(true);

        ui->groupBox_13->setEnabled(true);
        ui->groupBox_30->setEnabled(true);
        m_bRuning=false;
    }


}

void dlgphotoelectricitycfg::buttonJudge2(int index)
{
    if(index==1)
    {
        on_pushButtonQZRefresh_clicked();
    }
    if(index==2)
    {
        for(int i=2;i<22;i++)
        {
            m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecValQZ[i-2]/m_cntQZ,4,10,QLatin1Char('0')));
        }
    }
    if(index==3)
    {
        for(int i=2;i<22;i++)
        {
            m_qzLabels2[i-2]->setText(QString("%1").arg(m_vecKickValQZ[i-2],4,10,QLatin1Char('0')));
        }
    }

}

void dlgphotoelectricitycfg::buttonJudge3(int index)
{
    if(index==1)
    {
        on_pushButtonKTRefresh_clicked();
    }
    if(index==2)
    {
        for(int i=2;i<22;i++)
        {
            m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecValKT[i-2]/m_cntKT,4,10,QLatin1Char('0')));
        }
    }
    if(index==3)
    {
        for(int i=2;i<22;i++)
        {
            m_ktLabels2[i-2]->setText(QString("%1").arg(m_vecKickValKT[i-2],4,10,QLatin1Char('0')));
        }
    }
}



//模拟板
void dlgphotoelectricitycfg::on_pushButton_qzcurrent_clicked()
{

    int ProbIndex=ui->comboBox_qzcurrent->currentIndex()+1;
    double ProbThresh=ui->lineEdit_qzcurrent->text().toDouble();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProCurrentSim(1,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(4,ProbIndex,int(ProbThresh*100));

    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,vecTmp,1);

    appendInfoText( "QZSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;

}

void dlgphotoelectricitycfg::on_pushButton_qzgain_clicked()
{
    int ProbIndex=ui->comboBox_qzgain->currentIndex()+1;
    int ProbThresh=ui->lineEdit_qzgain->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProGainSim(1,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(5,ProbIndex,ProbThresh);

    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "QZSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qzprobSim_clicked()
{

    int ProbIndex=ui->comboBox_qzprobSim->currentIndex()+1;
    int ProbThresh=ui->lineEdit_qzprobSim->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProThresholdSim(1,m_mapProbVal[ProbIndex]-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(6,m_mapProbVal[ProbIndex],ProbThresh);

    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "QZSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;

}

void dlgphotoelectricitycfg::on_pushButton_qzprobRatioSim_clicked()
{

    QString strCommand="55010700";
    QByteArray byCommand=strCommand.toLatin1();
    QString linestr=ui->lineEdit_qzprobRatioSim->text();
    double ratio=linestr.toDouble();

    QByteArray value=  QString("%1").arg(int(ratio*100),4,16,QChar('0')).toLatin1();
    byCommand=byCommand.append(value);

    byCommand=byCommand.append(QString("0000").toLatin1()).toUpper();


    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(byCommand,receivedByArray,vecTmp,1);
    appendInfoText( "QZSim", ret, byCommand,receivedByArray);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    //保存参数
    pheConfig->setRatioSim(1,ratio);


    //调整显示
    int index=ui->comboBox_qzprobSim->currentIndex();

    int ProbThresh= pheConfig->getProThresholdSim(1,index)*ratio;

    ui->lineEdit_qzprobSim->setText(QString::number(ProbThresh));
}


void dlgphotoelectricitycfg::on_pushButton_qzgetgc_clicked()
{

    QString strCommand="550203000000";
    QByteArray byCommand=strCommand.toLatin1();


    byCommand=byCommand.append(QString("0000").toLatin1()).toUpper();


    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(byCommand,receivedByArray,vecTmp,3);

    appendInfoText( "QZSim", ret, byCommand,receivedByArray);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    for(int i=0;i<20;i++)
    {
        m_qzLabelsCurrentSim[i]->setText(QString("%1").arg(vecTmp[i*2],3,10,QLatin1Char('0')));
        m_qzLabelsGainSim[i]->setText(QString("%1").arg(vecTmp[i*2+1],3,10,QLatin1Char('0')));
    }

    //调整显示
}

void dlgphotoelectricitycfg::on_pushButton_ktcurrent_clicked()
{

    int ProbIndex=ui->comboBox_ktcurrent->currentIndex()+1;
    double ProbThresh=ui->lineEdit_ktcurrent->text().toDouble();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    //   pheConfig->setProCurrentSim(2,ProbIndex-1,ProbThresh);

    pheConfig->setProCurrentSim(2,m_mapProbVal[ProbIndex]-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(4,m_mapProbVal[ProbIndex],int(ProbThresh*100));

    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktgain_clicked()
{

    int ProbIndex=ui->comboBox_ktgain->currentIndex()+1;
    int ProbThresh=ui->lineEdit_ktgain->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProGainSim(2,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(5,ProbIndex,ProbThresh);

    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktprobSim_clicked()
{
    //空头
    //探头编号，探头设定值
    int ProbIndex=ui->comboBox_ktprobSim->currentIndex()+1;
    int ProbThresh=ui->lineEdit_ktprobSim->text().toInt();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProThresholdSim(2,m_mapProbVal[ProbIndex]-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(6,m_mapProbVal[ProbIndex],ProbThresh);

    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;


    //    pheConfig->setProCurrentSim(2,m_mapProbVal[ProbIndex]-1,ProbThresh);

    //    QByteArray sendText=genSendByArraySim(4,m_mapProbVal[ProbIndex],int(ProbThresh*100));

    //    QByteArray receivedByArray;
    //    QVector<int> vecTmp;

    //    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    //    appendInfoText( "KTSim", ret, sendText,receivedByArray);
}



void dlgphotoelectricitycfg::on_pushButton_ktprobRatioSim_clicked()
{

    QString strCommand="55010700";
    QByteArray byCommand=strCommand.toLatin1();
    QString linestr=ui->lineEdit_ktprobRatioSim->text();
    double ratio=linestr.toDouble();

    QByteArray value=  QString("%1").arg(int(ratio*100),4,16,QChar('0')).toLatin1();
    byCommand=byCommand.append(value);

    byCommand=byCommand.append(QString("0000").toLatin1()).toUpper();


    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(byCommand,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, byCommand,receivedByArray);

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    //保存参数
    pheConfig->setRatioSim(2,ratio);


    //调整显示
    int index=ui->comboBox_qzprobSim->currentIndex();
    int ProbThresh= pheConfig->getProThresholdSim(2,index)*ratio;

    ui->lineEdit_ktprobSim->setText(QString::number(ProbThresh));

}


void dlgphotoelectricitycfg::on_pushButton_ktgetgc_clicked()
{
    QString strCommand="5502030000";
    QByteArray byCommand=strCommand.toLatin1();


    byCommand=byCommand.append(QString("0000").toLatin1()).toUpper();


    QByteArray receivedByArray;

    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(byCommand,receivedByArray,vecTmp,3);
    appendInfoText( "KTSim", ret, byCommand,receivedByArray);


    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;


    //调整显示
    for(int i=0;i<20;i++)
    {
        m_ktLabelsCurrentSim[i]->setText(QString("%1").arg(vecTmp[i*2],3,10,QLatin1Char('0')));
        m_ktLabelsGainSim[i]->setText(QString("%1").arg(vecTmp[i*2+1],3,10,QLatin1Char('0')));
    }

}


void dlgphotoelectricitycfg::on_comboBox_qzcurrent_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_qzcurrent->setText(QString::number(pheConfig->getProCurrentSim(1,m_mapProbVal[index+1]-1)));
}

void dlgphotoelectricitycfg::on_comboBox_qzgain_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_qzgain->setText(QString::number(pheConfig->getProGainSim(1,index)));
}

void dlgphotoelectricitycfg::on_comboBox_qzprobSim_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    double ratio=pheConfig->getRatioSim(1);

    ui->lineEdit_qzprobSim->setText(QString::number(int(pheConfig->getProThresholdSim(1,m_mapProbVal[index+1]-1)*ratio)));
}

void dlgphotoelectricitycfg::on_comboBox_ktcurrent_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_ktcurrent->setText(QString::number(pheConfig->getProCurrentSim(2,m_mapProbVal[index+1]-1)));
}

void dlgphotoelectricitycfg::on_comboBox_ktgain_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_ktgain->setText(QString::number(pheConfig->getProGainSim(2,index)));
}

void dlgphotoelectricitycfg::on_comboBox_ktprobSim_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    double ratio=pheConfig->getRatioSim(2);

    ui->lineEdit_ktprobSim->setText(QString::number(int(pheConfig->getProThresholdSim(2,m_mapProbVal[index+1]-1)*ratio)));
}


void dlgphotoelectricitycfg::on_pushButton_qzApplySim_clicked()
{
    //模拟板进入调试状态，通过发送指令到控制板，控制板触发模拟板进入调试状态，模拟板返回数据
    ui->groupBox_13->setEnabled(false);


    m_bQZisRuningSim=true;
    if(m_QZFutureSim.isRunning())
    {
        return;
    }


    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值
    QByteArray receivedByArray;
    int ModelIndex=1;
    int ModelVal=0;
    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);
    //向控制板发送指令
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZSim", ret, sendText,receivedByArray);

    m_QZFutureSim=QtConcurrent::run(this,&dlgphotoelectricitycfg::startQZTestRunModelSim);


}

void dlgphotoelectricitycfg::on_pushButton_qzStopApplySim_clicked()
{
    //终止线程的读取值
    m_bQZisRuningSim=false;
    m_QZFutureSim.waitForFinished();


    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值
    QByteArray receivedByArray;
    int ModelIndex=0;
    int ModelVal=0;
    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);


    //向控制板发送指令
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommand(sendText,receivedByArray);
    appendInfoText( "QZSim", ret, sendText,receivedByArray);

    ui->groupBox_13->setEnabled(true);

}


void dlgphotoelectricitycfg::on_pushButton_ktApplySim_clicked()
{
    //模拟板进入调试状态，通过发送指令到控制板，控制板触发模拟板进入调试状态，模拟板返回数据

    ui->groupBox_30->setEnabled(false);
    m_bKTisRuningSim=true;
    if(m_KTFutureSim.isRunning())
    {
        return;
    }

    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值
    QByteArray receivedByArray;

    int ModelIndex=1;
    int ModelVal=0;
    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);

    //向控制板发送指令
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    m_KTFutureSim=QtConcurrent::run(this,&dlgphotoelectricitycfg::startKTTestRunModelSim);




}

void dlgphotoelectricitycfg::on_pushButton_ktStopApplySim_clicked()
{
    //终止线程的读取值
    m_bKTisRuningSim=false;
    m_KTFutureSim.waitForFinished();


    //缺支
    //模式选择(调试ModelIndex=1，正常ModelIndex=0)，值
    QByteArray receivedByArray;
    int ModelIndex=0;
    int ModelVal=0;
    QByteArray sendText=genSendByArray(6,ModelIndex,ModelVal);

    //向控制板发送指令
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommand(sendText,receivedByArray);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    ui->groupBox_30->setEnabled(true);
}

void dlgphotoelectricitycfg::updateTipText(QString txtTip)
{
    m_txtCnt++;
    if(m_txtCnt==100)
    {
        ui->plainTextEdit->clear();
        m_txtCnt=0;
    }
    //    QString txtTip="";
    //    if(ret)
    //    {
    //        txtTip=settingName+",Set Success,"+"send,"+QString(text)+",receive,"+QString(Receivetext);
    //    }
    //    else
    //    {
    //        txtTip=settingName+",Set Failed,"+"send,"+QString(text)+",receive,"+QString(Receivetext);
    //    }
    ui->plainTextEdit->appendPlainText(txtTip);
}

void dlgphotoelectricitycfg::on_pushButton_qzcurrentRatio_clicked()
{
    int ProbIndex=ui->comboBox_qzcurrent->currentIndex()+1;
    double ProbThresh=ui->lineEdit_qzcurrent->text().toDouble();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProCurrentSim(1,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(4,ProbIndex,int(ProbThresh*100));

    QByteArray receivedByArray;
    QVector<int> vecTmp;
    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,vecTmp,1);

    appendInfoText( "QZSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktcurrentRatio_clicked()
{
    int ProbIndex=ui->comboBox_ktcurrent->currentIndex()+1;
    double ProbThresh=ui->lineEdit_ktcurrent->text().toDouble();

    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;
    pheConfig->setProCurrentSim(2,ProbIndex-1,ProbThresh);

    QByteArray sendText=genSendByArraySim(4,ProbIndex,int(ProbThresh*100));

    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_qzcurrentCodeVal_clicked()
{


    int ProbIndex=ui->comboBox_qzcurrentCodeVal->currentIndex()+1;
    int ProbThresh=ui->lineEdit_qzcurrentCodeVal->text().toInt();
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    pheConfig->setProCurrentCodeValSim(1,m_mapProbVal[ProbIndex]-1,ProbThresh);

    int OperaCode=14;
    int v1=ProbIndex;
    int v2=ui->lineEdit_ktcurrentCodeVal->text().toInt();

    QByteArray sendText=QByteArray::fromHex("5501");

    QString strOperaCode=QString("%1").arg(OperaCode,2,16,QLatin1Char('0')).toUtf8();
    QByteArray byOperaCode=QByteArray::fromHex(QByteArray(strOperaCode.toLatin1()));
    sendText.append(byOperaCode);

    QString strHexV1=QString("%1").arg(v1,2,10,QLatin1Char('0')).toUtf8();
    QByteArray byHexV1=QByteArray::fromHex(QByteArray(strHexV1.toLatin1()));
    sendText.append(byHexV1);

    QString strHexV2=QString("%1").arg(v2,8,16,QLatin1Char('0')).toUtf8();
    QByteArray byHexV2=QByteArray::fromHex(QByteArray(strHexV2.toLatin1()));
    sendText.append(byHexV2);
    sendText =sendText.toHex().toUpper();


    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendQZCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "QZSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_pushButton_ktcurrentCodeVal_clicked()
{

    int ProbIndex=ui->comboBox_ktcurrentCodeVal->currentIndex()+1;
    int ProbThresh=ui->lineEdit_ktcurrentCodeVal->text().toInt();
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    pheConfig->setProCurrentCodeValSim(2,m_mapProbVal[ProbIndex]-1,ProbThresh);

    int OperaCode=14;
    int v1=ProbIndex;
    int v2=ui->lineEdit_ktcurrentCodeVal->text().toInt();

    QByteArray sendText=QByteArray::fromHex("5501");

    QString strOperaCode=QString("%1").arg(OperaCode,2,16,QLatin1Char('0')).toUtf8();
    QByteArray byOperaCode=QByteArray::fromHex(QByteArray(strOperaCode.toLatin1()));
    sendText.append(byOperaCode);

    QString strHexV1=QString("%1").arg(v1,2,10,QLatin1Char('0')).toUtf8();
    QByteArray byHexV1=QByteArray::fromHex(QByteArray(strHexV1.toLatin1()));
    sendText.append(byHexV1);

    QString strHexV2=QString("%1").arg(v2,8,16,QLatin1Char('0')).toUtf8();
    QByteArray byHexV2=QByteArray::fromHex(QByteArray(strHexV2.toLatin1()));
    sendText.append(byHexV2);
    sendText =sendText.toHex().toUpper();


    QByteArray receivedByArray;
    QVector<int> vecTmp;

    bool ret= PackageChecker::getInstance()->m_pBaseCom->sendKTCommandSim(sendText,receivedByArray,vecTmp,1);
    appendInfoText( "KTSim", ret, sendText,receivedByArray);
    qDebug()<<"ret,"<<ret<<endl;
}

void dlgphotoelectricitycfg::on_comboBox_qzcurrentCodeVal_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_qzcurrentCodeVal->setText(QString::number(pheConfig->getProCurrentCodeValSim(1,m_mapProbVal[index+1]-1)));

}

void dlgphotoelectricitycfg::on_comboBox_ktcurrentCodeVal_currentIndexChanged(int index)
{
    ClsPhototElecConfig *pheConfig=PackageChecker::getInstance()->PhotoElecConfig;

    ui->lineEdit_ktcurrentCodeVal->setText(QString::number(pheConfig->getProCurrentCodeValSim(2,m_mapProbVal[index+1]-1)));
}
