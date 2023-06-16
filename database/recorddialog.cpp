#include "recorddialog.h"
#include "ui_recorddialog.h"

#include"dsfilerecord.h"
#include"params/dssystemparam.h"
#include"packagechecker.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include<QScrollBar>

bool compareMainRecord(const MainRecord& c1,const MainRecord& c2)//要用常数，不然编译错误
{
    return c1.m_No.toInt()>c2.m_No.toInt();
}

bool compare3(const std::pair<std::pair<QString,QString>,int>& p1,const std::pair<std::pair<QString,QString>,int>& p2)
{
    return p1.second>p2.second;
}

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("记录查询");
    InitStyle();

    initModel();


    //设置视图的属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可修改

    QVector<int> vArr;
    for (int i=0;i<4+DSSystemParam::CameraNum+1;i++)
    {
        vArr.push_back(0);
    }
    //    vArr.push_back(0);
    //    vArr.push_back(0);
    //    vArr.push_back(0);
    //    vArr.push_back(0);
    //    vArr.push_back(0);
    ui->tableView->setColumnFixWidth(vArr);




    //初始化视图
    //ui->tableView->setColumnHidden(5,true);
    ui->tableView->selectRow(0);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    init();
    m_PageIdx=0;
    loadPage();
    ui->tableViewDefect->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
    ui->tableViewDefect->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可修改
    ui->tableViewDefect->verticalHeader()->hide();
    ui->tableViewDefect->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableViewDefect->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->tableViewDetail->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
    ui->tableViewDetail->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可修改
    ui->tableViewDetail->verticalHeader()->hide();
    ui->tableViewDetail->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableViewDetail->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    ui->widget_5->setVisible(false);
    ui->widget_4->setVisible(false);
}


void RecordDialog::loadPage()
{


    QStringList listmodelMain;
    //listmodelMain<<"日期"<<"班次"<<"牌号"<<"检测总数"<<"剔除总数"<<"No";
    listmodelMain<<"日期"<<"班次"<<"牌号"<<"检测总数";
    for(int i=0;i<DSSystemParam::CameraNum;i++)
    {
        listmodelMain<<QString("相机%1剔除数").arg(i+1);
    }
    listmodelMain<<"No";





    this->modelMain->clear();
    modelMain->setHorizontalHeaderLabels(listmodelMain);
    int startIdx=m_PageIdx*m_PageRowNum;
    int endIdx=m_PageIdx*m_PageRowNum+m_PageRowNum-1;
    if(endIdx>m_VRec.size()-1) endIdx=m_VRec.size()-1;

    int rowIdx=0;
    for(int idx=startIdx;idx<=endIdx;idx++,++rowIdx)
    {
        QStandardItem * itemDate=new QStandardItem(m_VRec[idx].m_Date);
        QStandardItem * itemShift=new QStandardItem(m_VRec[idx].m_StrShift);
        QStandardItem * itemBrand=new QStandardItem(m_VRec[idx].m_Brand);
        QStandardItem * itemNumall=new QStandardItem(QString::number(m_VRec[idx].m_NumAll));
        //QStandardItem * itemNumng=new QStandardItem(QString::number(m_VRec[idx].m_NumNg));

        QStandardItem * itemNo=new QStandardItem(QString(m_VRec[idx].m_No));
        itemDate->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemShift->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemBrand->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemNumall->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //itemNumng->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        this->modelMain->setItem(rowIdx,0,itemDate);
        this->modelMain->setItem(rowIdx,1,itemShift);
        this->modelMain->setItem(rowIdx,2,itemBrand);
        this->modelMain->setItem(rowIdx,3,itemNumall);

        for(int i=0;i<DSSystemParam::CameraNum;i++)
        {

            QStandardItem * itemNumng=new QStandardItem(QString::number(m_VRec[idx].m_NumNg[i]));
            itemNumng->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            this->modelMain->setItem(rowIdx,4+i,itemNumng);
            //listmodelMain<<QString("相机%1剔除数").arg(i+1);
        }
        //this->modelMain->setItem(rowIdx,4,itemNumng);

        this->modelMain->setItem(rowIdx,4+DSSystemParam::CameraNum,itemNo);
    }

    ui->tableView->scrollToTop();
    ui->tableView->selectRow(0);
    on_tableView_clicked(this->ui->tableView->model()->index(0,0));

}

RecordDialog::~RecordDialog()
{
    delete ui;
}


void RecordDialog::InitStyle()
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
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}

bool RecordDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void RecordDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void RecordDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void RecordDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void RecordDialog::on_btnMenu_Max_clicked()
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

void RecordDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void RecordDialog::on_btnMenu_Close_clicked()
{
    this->close();
}




void RecordDialog::init()
{
    m_VRec.clear();
    DsFileRecord::loadMainRecord(QString(DSSystemParam::AppPath+"/record"),m_VRec);

    MainRecord tmpRec;
    DsFileRecord::getCurRecord(tmpRec);
    m_VRec.push_back(tmpRec);

    std::sort(m_VRec.begin(),m_VRec.end(),compareMainRecord);




}


void RecordDialog::updateDefectView(QString mainRecordId)
{
    this->setCursor(Qt::WaitCursor);

    //初始化modelDefect模型
    modelDefect->clear();
    QStringList listDefect;
    listDefect<<"相机"<<"数量"<<"比例(%)";
    modelDefect->setHorizontalHeaderLabels(listDefect);

    //初始化modelDefect模型
    modelDetail->clear();
    QStringList listDetail;
    listDetail<<"相机"<<"序号"<<"数量";
    modelDetail->setHorizontalHeaderLabels(listDetail);

    //设置尺寸
    this->ui->tableViewDefect->setColumnWidth(0,80);
    this->ui->tableViewDefect->setColumnWidth(1,95);
    this->ui->tableViewDefect->setColumnWidth(2,95);

    this->ui->tableViewDetail->setColumnWidth(0,80);
    this->ui->tableViewDetail->setColumnWidth(1,95);
    this->ui->tableViewDetail->setColumnWidth(2,95);

    //获取班次对应的所有NG信息
    std::list<DefectRecord> vdr;


    int matrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]={0};
    int cameraNum=0,allNum=0,ngNum=0;
    int ngAllNum=0;

    if(mainRecordId==DsFileRecord::getCurMainRecordNo())
    {
        cameraNum=PackageChecker::getInstance()->Cameras.size();
        allNum=PackageChecker::getInstance()->RunParam_CalcNumAll;
        ngNum=PackageChecker::getInstance()->RunParam_CalcNumNg;
        memcpy(matrix,PackageChecker::getInstance()->ErrRecord,CAMERANUM_MAX*CHECKOPERATORNUM_MAX*sizeof(int));

    }
    else
    {
        DsFileRecord::loadRecord(QString(DSSystemParam::AppPath+"/record/"+ mainRecordId+".rec"),cameraNum,allNum,ngNum,matrix);
    }


    //进行modelDefrct的显示处理
    {

        int  cameraErrNum[CAMERANUM_MAX]={0};
        for (int idx=0;idx<cameraNum;++idx)
        {

            for (int idx_i=0;idx_i<128;++idx_i)
            {
                cameraErrNum[idx]+=matrix[idx][idx_i];
                ngAllNum+=matrix[idx][idx_i];
            }
        }

        int  idxSort[CAMERANUM_MAX]={0};
        for (int idx=0;idx<cameraNum;++idx)
        {
            idxSort[idx]=idx+1;
        }

        for (int i = 0; i < cameraNum; i++)
        {
            for (int j = 0; j < cameraNum -  i - 1; j++)
            {
                if (cameraErrNum[j] < cameraErrNum[j + 1])
                {
                    int temp;
                    temp = cameraErrNum[j + 1];
                    cameraErrNum[j + 1] = cameraErrNum[j];
                    cameraErrNum[j] = temp;

                    temp=idxSort[j+1];
                    idxSort[j + 1] = idxSort[j];
                    idxSort[j] = temp;

                }
            }
        }




        //数据可视化为到tableview
        for(int i=0;i<cameraNum;i++)
        {
            QStandardItem * itemCamera=new QStandardItem(QString("相机%1").arg(idxSort[i]));
            QStandardItem * itemErrNum=new QStandardItem(QString::number(cameraErrNum[i]));
            QStandardItem * itemPercentage=new QStandardItem(QString("%1%").arg(QString::number(ngAllNum==0?0:(double)cameraErrNum[i]/ngAllNum*100,'f',2)));

            itemCamera->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemErrNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemPercentage->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            this->modelDefect->setItem(i,0,itemCamera);
            this->modelDefect->setItem(i,1,itemErrNum);
            this->modelDefect->setItem(i,2,itemPercentage);
        }


    }



    //modelDetail的显示处理
    {

        std::vector<std::pair<std::pair<QString,QString>,int>> vDetail;
        for(int idx_r=0;idx_r<CAMERANUM_MAX;++idx_r)
        {
            for(int idx_c=0;idx_c<CHECKOPERATORNUM_MAX;++idx_c)
            {
                if(matrix[idx_r][idx_c]!=0)
                {

                    QString cameraNo="相机"+QString::number(idx_r+1);
                    QString channelNo="C"+QString::number(idx_c+1);
                    std::pair<QString,QString> pairKey=std::make_pair(cameraNo,channelNo);
                    std::pair< std::pair<QString,QString>,int> pairKy=std::make_pair(pairKey,matrix[idx_r][idx_c]);
                    vDetail.push_back(pairKy);
                }
            }
        }

        std::sort(vDetail.begin(),vDetail.end(),compare3);
        std::vector<std::pair<std::pair<QString,QString>,int>>::iterator itor=vDetail.begin();
        int row=0;
        for(;itor!=vDetail.end();++itor,++row)
        {
            QStandardItem * itemCamera=new QStandardItem(itor->first.first);
            QStandardItem * itemChannel=new QStandardItem(itor->first.second);
            QStandardItem * itemNum=new QStandardItem(QString::number(itor->second));

            itemCamera->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemChannel->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            this->modelDetail->setItem(row,0,itemCamera);
            this->modelDetail->setItem(row,1,itemChannel);
            this->modelDetail->setItem(row,2,itemNum);

        }

    }




    this->setCursor(Qt::ArrowCursor);
}



void RecordDialog::initModel()
{
    //mainrecord的model
    modelMain = new QStandardItemModel(1,1,this);
    QStringList listmodelMain;
    listmodelMain<<"日期"<<"班次"<<"牌号"<<"检测总数"<<"剔除总数"<<"No";
    modelMain->setHorizontalHeaderLabels(listmodelMain);
    //    modelMain->setHorizontalHeaderItem(0,new QStandardItem("日期"));
    //    modelMain->setHorizontalHeaderItem(1,new QStandardItem("班次"));
    //    modelMain->setHorizontalHeaderItem(2,new QStandardItem("牌号"));
    //    modelMain->setHorizontalHeaderItem(3,new QStandardItem("检测总数"));
    //    modelMain->setHorizontalHeaderItem(4,new QStandardItem("剔除总数"));
    this->ui->tableView->setModel(modelMain);

    //该班次错误类型的统计
    modelDefect= new QStandardItemModel(1,1,this);
    this->ui->tableViewDefect->setModel(modelDefect);
    modelDetail=new QStandardItemModel(1,1,this);
    this->ui->tableViewDetail->setModel(modelDetail);
}

void RecordDialog::on_tableView_clicked(const QModelIndex &index)
{ 
    QString No=this->modelMain->index(index.row(),5,QModelIndex()).data().toString();
    updateDefectView(No);
}

void RecordDialog::on_btnPre_clicked()
{

    if(this->m_PageIdx==0) return;
    --m_PageIdx;
    loadPage();

    ui->tableView->scrollToTop();
    ui->tableView->selectRow(0);
    on_tableView_clicked(this->ui->tableView->model()->index(0,0));
    //    this->ui->tableView->selectRow(this->curSelectRow);
    //    QString No=this->modelMain->index(this->curSelectRow,5,QModelIndex()).data().toString();
    //    updateDefectView(No);
}

void RecordDialog::on_btnNext_clicked()
{
    if(this->m_PageIdx==(m_VRec.size()+m_PageRowNum-1)/m_PageRowNum-1) return;
    ++m_PageIdx;
    loadPage();

    ui->tableView->scrollToTop();
    ui->tableView->selectRow(0);
    on_tableView_clicked(this->ui->tableView->model()->index(0,0));
}

void RecordDialog::on_toolButtonCoSet_clicked()
{
    if(ui->tableViewDefect->isVisible())
    {
        ui->tableViewDefect->setVisible(false);
        ui->toolButtonCoSet->setStyleSheet("QToolButton#toolButtonCoSet{border-image:url(:/更多normal.png);  background:transparent;}"
                                           "QToolButton#toolButtonCoSet:hover{border-image:url(:/更多hover.png); }"
                                           "QToolButton#toolButtonCoSet:pressed{border-image:url(:/更多press.png); }");
    }
    else
    {
        ui->tableViewDefect->setVisible(true);
        ui->toolButtonCoSet->setStyleSheet("QToolButton#toolButtonCoSet{border-image:url(:/下箭头normal.png);  background:transparent;}"
                                           "QToolButton#toolButtonCoSet:hover{border-image:url(:/下箭头hover.png); }"
                                           "QToolButton#toolButtonCoSet:pressed{border-image:url(:/下箭头press.png); }");
    }
}
