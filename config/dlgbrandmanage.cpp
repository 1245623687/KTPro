#include "dlgbrandmanage.h"
#include "ui_dlgbrandmanage.h"
#include<QTextCodec>

#include"dsdebug.h"
#include"common/XmlHelper.h"
#include"dlgbrandadd.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"


#define DLGBRANDMANAGE_IMGHEI 442
#define DLGBRANDMANAGE_IMGWID 644

DlgBrandManage::DlgBrandManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgBrandManage)
{
    ui->setupUi(this);
    this->setWindowTitle("品牌管理");
    InitStyle();

    m_Scene=nullptr;
    m_Model=nullptr;


    this->setWindowTitle("烟包品种管理");

    m_Model=new QStandardItemModel;
    this->ui->tableViewBrand->setModel(m_Model);

    loadTableModel();

    m_Scene=new GraphicsScene;
    this->ui->graphicsView->setScene(m_Scene);


    m_GroupButton=new QButtonGroup;
    m_GroupButton->addButton(ui->radioButton_1,1);
    m_GroupButton->addButton(ui->radioButton_2,2);
    m_GroupButton->addButton(ui->radioButton_3,3);
    m_GroupButton->addButton(ui->radioButton_4,4);
    m_GroupButton->addButton(ui->radioButton_5,5);
    m_GroupButton->addButton(ui->radioButton_6,6);
    m_GroupButton->addButton(ui->radioButton_7,7);
    m_GroupButton->addButton(ui->radioButton_8,8);
    connect(m_GroupButton,SIGNAL(buttonClicked(int)),this,SLOT(changeCameraNo(int)));

    this->m_GroupButton->button(1)->setChecked(true);
    showImg(0);
}




void DlgBrandManage::InitStyle()
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


bool DlgBrandManage::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgBrandManage::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgBrandManage::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgBrandManage::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgBrandManage::on_btnMenu_Max_clicked()
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

void DlgBrandManage::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void DlgBrandManage::on_btnMenu_Close_clicked()
{
    this->close();
}



void DlgBrandManage::showImg(int row)
{
    if(m_Model->rowCount()==0) return;

    m_CurRow=row;

    this->m_Scene->clear();
    QString brand =this->m_Model->data(m_Model->index(row,1)).toString();
    QString imagePath= DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+brand+"/"+QString("ImgPro%1.bmp").arg(m_ShowCameraNo);
    QFileInfo fi(imagePath);
    if(fi.exists())
    {
        QPixmap pixmap(imagePath);
        this->m_Scene->SetBackGroundImage(pixmap,pixmap.width(),pixmap.height());

        if((double)pixmap.height()/pixmap.width()>(double)DLGBRANDMANAGE_IMGHEI/DLGBRANDMANAGE_IMGWID)
        {
            ui->graphicsView->scaleto((double)DLGBRANDMANAGE_IMGHEI/(double)pixmap.height(),
                                      (double)DLGBRANDMANAGE_IMGHEI/(double)pixmap.height());

            ui->graphicsView->setSceneRect(QRectF(0,0,(double)pixmap.width(),(double)pixmap.height()));

        }
        else
        {
            ui->graphicsView->scaleto((double)DLGBRANDMANAGE_IMGWID/pixmap.width(),
                                      (double)DLGBRANDMANAGE_IMGWID/pixmap.width());


             ui->graphicsView->setSceneRect(QRectF(0,0,(double)pixmap.width(),(double)pixmap.height()));
        }

    }

    int camereNo=this->m_Model->data(m_Model->index(row,2)).toString().toInt();
    for (int i=1;i<=m_GroupButton->buttons().size();i++)
    {
        m_GroupButton->button(i)->setVisible(i<=camereNo?true:false);
    }

}

void DlgBrandManage::loadTableModel()
{
    m_Model->clear();
    QStringList listmodelMain;
    listmodelMain<<"编号"<<"品牌"<<"相机数";
    m_Model->setHorizontalHeaderLabels(listmodelMain);

    QVector<QString> floders;
    FileHelper::getAllFileFolder(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath,floders);
    QVector<StuBrand> vBrands;
    for(int i=0;i<floders.size();i++)
    {
        StuBrand tmpBrand;
        tmpBrand.No=i;
        tmpBrand.BrandName=floders[i];
        QString configPath=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+floders[i]+"/config.xml";
        DSDEBUG<<"configPath："<<configPath<<endl;
        std::string cameraNumStr;
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string tmpName = code->fromUnicode(configPath).data();
        XmlHelper::GetElementTextByPath("Tobacco/ImgProObjNum",tmpName.c_str(),cameraNumStr);
        int cameraNum=std::atoi(cameraNumStr.c_str());
        tmpBrand.CameraNo=cameraNum;
        vBrands.push_back(tmpBrand);
    }
    floders.clear();

    //加载tableview
    for (int idx=0;idx<vBrands.size();idx++)
    {
        QStandardItem * itemNo=new QStandardItem(QString::number(vBrands[idx].No));
        itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QStandardItem* itemBrandName=new QStandardItem(vBrands[idx].BrandName);
        itemBrandName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QStandardItem * itemCameraNum=new QStandardItem(QString::number(vBrands[idx].CameraNo));
        itemCameraNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_Model->setItem(idx,0,itemNo);
        m_Model->setItem(idx,1,itemBrandName);
        m_Model->setItem(idx,2,itemCameraNum);
    }

    ui->tableViewBrand->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewBrand->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewBrand->setColumnWidth(0,60);
    ui->tableViewBrand->setColumnWidth(1,150);
    ui->tableViewBrand->setColumnWidth(2,60);
    ui->tableViewBrand->verticalHeader()->hide();
    ui->tableViewBrand->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableViewBrand->setStyleSheet("selection-background-color: rgb(152,202,104)");
}

DlgBrandManage::~DlgBrandManage()
{
    if(m_Scene!=nullptr)
    {
        m_Scene->clear();
        delete  m_Scene;
        m_Scene=nullptr;
    }    if(m_Model!=nullptr)
    {
        m_Model->clear();
        delete  m_Model;
        m_Model=nullptr;
    }
    delete ui;
}

void DlgBrandManage::on_pushButtonAddBrand_clicked()
{
    int curRow= ui->tableViewBrand->selectionModel()->currentIndex().row();
    QString brandname="Default";
    if(curRow>=0)
    {
        brandname=ui->tableViewBrand->model()->data(ui->tableViewBrand->model()->index(curRow,1)).toString();
    }

    DlgBrandAdd w(brandname);
    w.exec();

    loadTableModel();
}

void DlgBrandManage::on_pushButtonDelBrand_clicked()
{


    int row=this->ui->tableViewBrand->currentIndex().row();
    if(row<0)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("请选择要删除的产品!", 0);
        msg->exec();
        return;
    }
    QString delBrand =this->m_Model->data(m_Model->index(row,1)).toString();

    QDir dir(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+delBrand);

    dir.removeRecursively();
    loadTableModel();
}

void DlgBrandManage::on_pushButtonChange_clicked()
{

    if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
    {


        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("系统正在运行，无法切换品牌，请先停止运行！", 0);
        msg->exec();

        return;
    }

    int row=this->ui->tableViewBrand->currentIndex().row();

    DSDEBUG<<"ROW:"<<row<<endl;
    if(row<0)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("请选择要导入的产品！", 0);
        msg->exec();

        return;
    }

    QString newBrand=this->m_Model->data(m_Model->index(row,1)).toString();
    if(newBrand==DSSystemParam::BrandName)
    {

        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString("当前牌号为: "+DSSystemParam::BrandName+"，无需切换!"), 0);
        msg->exec();

        return;
    }

    // int res=QMessageBox::question(NULL,"确认","\n是否切换为:    "+this->m_Model->data(m_Model->index(row,1)).toString()+"    ?         ",QMessageBox::Yes,QMessageBox::No);

    frmMessageBox *msg = new frmMessageBox;
    msg->SetMessage(QString("是否切换为:    "+this->m_Model->data(m_Model->index(row,1)).toString()+"    ?         "), 1);
    int res= msg->exec();

    //DSDEBUG<<"res:"<<res;
    if(!res) return;


    DSSystemParam::BrandName=newBrand;
    DSSystemParam::CameraNum=this->m_Model->data(m_Model->index(row,2)).toInt();





    this->close();
}

void DlgBrandManage::on_pushButtonCancel_clicked()
{
    this->close();
}

void DlgBrandManage::on_tableViewBrand_clicked(const QModelIndex &index)
{

    showImg(index.row());

}

void DlgBrandManage::on_toolButtonClose_clicked()
{
    close();
}
