#include "dlgdefectinspect2.h"
#include "ui_dlgdefectinspect2.h"
#include"CheckOperator/DSIppImage.h"
#include<vector>
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"

#include"kcontrols/dlgkeyboard.h"
DlgDefectInspect2::DlgDefectInspect2(CheckOperatorDefectInspect2* pCheckOperator,ImgPro* imgPro,OpencvImage&  image,int basis_x,int basis_y,int mode,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgDefectInspect2)
{
    ui->setupUi(this);
    this->setWindowTitle("异形检查");
    InitStyle();
    ui->lab_Title->setText(QString("异型检查--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));

    m_ModelHPos=nullptr;
    m_ModelVPos=nullptr;
    m_Res=-1;

#ifdef IMG_TYPE_GRAY
    cv::Rect rect( qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs+basis_x,0),
                   qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys+basis_y,0),
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x,image.ImageGray.cols-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs-basis_x,
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye+basis_y,image.ImageGray.rows-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys-basis_y
                   );
#endif
#ifdef IMG_TYPE_RGB
    cv::Rect rect( qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs+basis_x,0),
                   qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys+basis_y,0),
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x,image.ImageRGB.cols-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs-basis_x,
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye+basis_y,image.ImageRGB.rows-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys-basis_y
                   );
#endif

    cv::cvtColor(image.ImageRGB,image.ImageGray,cv::COLOR_BGR2GRAY);
    thisImage.Format=ENUMDSIMAGEFORMAT_GRAY;
    switch (thisImage.Format)
    {
    case ENUMDSIMAGEFORMAT_GRAY:
        thisImage.ImageGray=cv::Mat(image.ImageGray,rect).clone();
        thisImage.Width=rect.width;
        thisImage.Height=rect.height;
        break;
    case ENUMDSIMAGEFORMAT_RGB:
        break;
    case ENUMDSIMAGEFORMAT_R:
        break;
    case ENUMDSIMAGEFORMAT_G:
        break;
    case ENUMDSIMAGEFORMAT_B:
        break;
    }

    vector<cv::Point> tmpPts;
    tmpPts.push_back(cv::Point(pCheckOperator->X1,pCheckOperator->Y1));
    tmpPts.push_back(cv::Point(pCheckOperator->X2,pCheckOperator->Y2));
    tmpPts.push_back(cv::Point(pCheckOperator->X3,pCheckOperator->Y3));
    tmpPts.push_back(cv::Point(pCheckOperator->X4,pCheckOperator->Y4));
    tmpPts.push_back(cv::Point(pCheckOperator->X5,pCheckOperator->Y5));
    tmpPts.push_back(cv::Point(pCheckOperator->X6,pCheckOperator->Y6));
    tmpPts.push_back(cv::Point(pCheckOperator->X7,pCheckOperator->Y7));
    tmpPts.push_back(cv::Point(pCheckOperator->X8,pCheckOperator->Y8));




    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);
    QVector<QPoint> vPts;
    vPts.push_back(QPoint(pCheckOperator->X1,pCheckOperator->Y1));
    vPts.push_back(QPoint(pCheckOperator->X2,pCheckOperator->Y2));
    vPts.push_back(QPoint(pCheckOperator->X3,pCheckOperator->Y3));
    vPts.push_back(QPoint(pCheckOperator->X4,pCheckOperator->Y4));
    vPts.push_back(QPoint(pCheckOperator->X5,pCheckOperator->Y5));
    vPts.push_back(QPoint(pCheckOperator->X6,pCheckOperator->Y6));
    vPts.push_back(QPoint(pCheckOperator->X7,pCheckOperator->Y7));
    vPts.push_back(QPoint(pCheckOperator->X8,pCheckOperator->Y8));
    m_ItemImage->setEdgeLine(vPts);






    this->m_Scene=new GraphicsScene;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);


    DSIppImage::getMaskedImage(thisImage.ImageGray,tmpPts,100);



    m_ItemImageInit=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);
    m_ItemImageInit->setEdgeLine(vPts);
    vPts.clear();

    this->m_SceneInit=new GraphicsScene;
    this->m_SceneInit->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_SceneInit->addItem(this->m_ItemImageInit);

    this->ui->graphicsView->setScene(m_Scene);
    this->ui->graphicsView_2->setScene(m_SceneInit);
    ui->graphicsView->setSceneRect(QRect(0,0,thisImage.Width,thisImage.Height));
    ui->graphicsView_2->setSceneRect(QRect(0,0,thisImage.Width,thisImage.Height));

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));
    ui->graphicsView_2->setBackgroundBrush(QBrush(QColor(100,100,100)));
    initControl();
    loadHVPos();
    updateCheckRes();


    connect(ui->spinBoxMinArea,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxBinThresh,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    connect(ui->graphicsView,SIGNAL(infoCoordinate(int ,int )),this,SLOT(showPixelInfo(int ,int )));
    connect(ui->graphicsView_2,SIGNAL(infoCoordinate(int ,int )),this,SLOT(showPixelInfo(int ,int )));



}
void DlgDefectInspect2::showKeyBoard(KControlsBase* pLineEdit)
{

    switch (pLineEdit->type())
    {
    case ENUMKCONTROLTYPE_KSPINBOX:
    {
        KSpinBoxKb* pWidget=static_cast<KSpinBoxKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x(),pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    case ENUMKCONTROLTYPE_KLINEEDIT:
    {
        KLineEditKb* pWidget=static_cast<KLineEditKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x(),pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    }

}

void DlgDefectInspect2::showPixelInfo(int x, int y)
{
    if(x<0||x>=thisImage.ImageGray.cols||y<0||y>=thisImage.ImageGray.rows){

        this->ui->labelPixInfo->setText("(000,000):000");
        return;
    }
    QString pixInfoStr=QString("(%1,%2):%3").arg(x, 3, 10, QChar('0')).arg(y, 3, 10, QChar('0')).arg(this->thisImage.ImageGray.at<uchar>(y,x), 3, 10, QChar('0'));
    this->ui->labelPixInfo->setText(pixInfoStr);

}


void DlgDefectInspect2::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgDefectInspect2::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgDefectInspect2::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgDefectInspect2::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgDefectInspect2::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgDefectInspect2::on_btnMenu_Close_clicked()
{
    this->close();
}

void DlgDefectInspect2::updateCheckRes()
{
    //this->m_Scene->clear();
    int hei=m_ItemImage->getHeight();
    int wid=m_ItemImage->getWidth();
    CheckOperatorDefectInspect2Control * pCheckOperatorDefectInspectControl=new CheckOperatorDefectInspect2Control(this->m_CheckOperator);
    OpencvImage imgRes(ENUMDSIMAGEFORMAT_GRAY,hei,wid);
    int calcArea=0;
    int sumArea=0;
  //  cv::imwrite("./dst1.bmp",this->m_ItemImage->getImage().ImageGray );

    bool defectAorS=ui->radioBtnDefectAll->isChecked();
    bool res=pCheckOperatorDefectInspectControl->debugCalculator(this->m_ItemImage->getImage().ImageGray,this->m_ItemImage->getImage().ImageGray,imgRes.ImageGray,
                                                                 cv::Rect(0,0,this->m_ItemImage->getWidth(),this->m_ItemImage->getHeight()),
                                                                 ui->spinBoxBinThresh->value(),ui->radioBtnIsBlackBlob->isChecked(),ui->radioBtnDefectAll->isChecked(),
                                                                 ui->spinBoxMinArea->value(),&calcArea,&sumArea);
    vector<cv::Point> tmpPts;
    tmpPts.push_back(cv::Point(m_CheckOperator->X1,m_CheckOperator->Y1));
    tmpPts.push_back(cv::Point(m_CheckOperator->X2,m_CheckOperator->Y2));
    tmpPts.push_back(cv::Point(m_CheckOperator->X3,m_CheckOperator->Y3));
    tmpPts.push_back(cv::Point(m_CheckOperator->X4,m_CheckOperator->Y4));
    tmpPts.push_back(cv::Point(m_CheckOperator->X5,m_CheckOperator->Y5));
    tmpPts.push_back(cv::Point(m_CheckOperator->X6,m_CheckOperator->Y6));
    tmpPts.push_back(cv::Point(m_CheckOperator->X7,m_CheckOperator->Y7));
    tmpPts.push_back(cv::Point(m_CheckOperator->X8,m_CheckOperator->Y8));
    DSIppImage::getMaskedImage(imgRes.ImageGray,tmpPts,100);




    this->m_ItemImage->reLoadImage(imgRes,imgRes.ImageGray.cols,imgRes.ImageGray.rows);
   // cv::imwrite("./dst2.bmp",imgRes.ImageGray);

    ui->lineEditImageArea->setText(QString::number(sumArea));
    ui->labelCalArea->setText(QString::number(calcArea));
    if(!res) {
        ui->labelResult->setText("OK");
        this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
        m_Res=0;
    }
    else
    {
        ui->labelResult->setText("NG");
        this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");
        m_Res=-1;
    }
    delete  pCheckOperatorDefectInspectControl;
}


QString DlgDefectInspect2::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
{
    QString ret="";
    switch (t)
    {
    case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        ret="对象搜索";
        break;
    case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        ret="边缘定位";
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        ret="缺陷检查";
        break;
    case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        ret="折线检查";
        break;
    case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        ret="灰度测量";
        break;
    case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        ret="直线查找";
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        ret="关联检查";
        break;
    case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        ret="拉线检查";
        break;
    }
    return ret;
}

void DlgDefectInspect2::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<tr("类型")<<tr("顺序");
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,63);
    ui->tableViewH->setColumnWidth(1,105);
    ui->tableViewH->setColumnWidth(2,95);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,62);
    ui->tableViewV->setColumnWidth(1,105);
    ui->tableViewV->setColumnWidth(2,95);
    ui->tableViewV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewV->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewV->verticalHeader()->hide();


    int v_Num=0,h_Num=0;
    std::vector<std::string> refV,refH;
    StuCheckOperatorBasicControl control(this->m_CheckOperator->stu_CheckOperatorBasic);
    control.getRefY(refH);
    control.getRefX(refV);


    list<CheckOperatorBasic*>::iterator itor=this->m_ImgPro->LstCheckOperator.begin();
    for (;itor!=this->m_ImgPro->LstCheckOperator.end();itor++)
    {
        switch ((*itor)->CheckType) {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {
            //横向x
            QStandardItem * itemNo;
            itemNo =new QStandardItem(QString("R%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            QStandardItem * itemName=new QStandardItem(getCheckOperatorName((*itor)->CheckType));
            bool exist=false;
            int orderNo=0;
            for (int idx=0;idx<refV.size();++idx)
            {
                if((*itor)->stu_CheckOperatorBasic->Index==std::atoi(refV[idx].c_str()))
                {
                    exist=true;
                    orderNo=idx+1;
                    break;
                }
            }
            QStandardItem * itemOrder;
            if(exist) itemOrder=new QStandardItem(std::to_string(orderNo).c_str());
            else itemOrder=new QStandardItem(" ");
            itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemOrder->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            this->m_ModelHPos->setItem(h_Num,0,itemNo);
            this->m_ModelHPos->setItem(h_Num,1,itemName);
            this->m_ModelHPos->setItem(h_Num,2,itemOrder);

            //纵向y
            QStandardItem * itemNo2;
            itemNo2 =new QStandardItem(QString("R%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            QStandardItem * itemName2=new QStandardItem(getCheckOperatorName((*itor)->CheckType));
            exist=false;
            orderNo=0;
            for (int idx=0;idx<refH.size();++idx)
            {
                if((*itor)->stu_CheckOperatorBasic->Index==std::atoi(refH[idx].c_str()))
                {
                    exist=true;
                    orderNo=idx+1;
                    break;
                }
            }
            QStandardItem * itemOrder2;
            if(exist) itemOrder2=new QStandardItem(std::to_string(orderNo).c_str());
            else itemOrder2=new QStandardItem(" ");
            itemNo2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemName2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemOrder2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            this->m_ModelVPos->setItem(v_Num,0,itemNo2);
            this->m_ModelVPos->setItem(v_Num,1,itemName2);
            this->m_ModelVPos->setItem(v_Num,2,itemOrder2);
            v_Num++;h_Num++;
        }

            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            QStandardItem * itemNo;
            itemNo =new QStandardItem(QString("R%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            QStandardItem * itemName=new QStandardItem(getCheckOperatorName((*itor)->CheckType));


            itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            ImgProControl control(this->m_ImgPro);
            CheckOperatorBasic* pCheckOperator= control.getCheckNo((*itor)->stu_CheckOperatorBasic->Index);
            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>(pCheckOperator);

            switch (pCheckOperatorEdgePositioning->EdgePos)
            {
            case CheckOperatorEdgePositioning::ENUMEDGEPOS_LEFT:
            case CheckOperatorEdgePositioning::ENUMEDGEPOS_RIGHT:
            {
                bool  exist=false;
                int orderNo=0;
                for (int idx=0;idx<refV.size();++idx)
                {
                    if((*itor)->stu_CheckOperatorBasic->Index==std::atoi(refV[idx].c_str()))
                    {
                        exist=true;
                        orderNo=idx+1;
                    }
                }
                QStandardItem * itemOrder;
                if(exist){
                    itemOrder=new QStandardItem(std::to_string(orderNo).c_str());
                }
                else
                {
                    itemOrder=new QStandardItem("");
                }
                itemOrder->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                this->m_ModelHPos->setItem(h_Num,0,itemNo);
                this->m_ModelHPos->setItem(h_Num,1,itemName);
                this->m_ModelHPos->setItem(h_Num,2,itemOrder);
                DSDEBUG<<itemOrder->data().toInt()<<endl;
                h_Num++;
            }
                break;

            case CheckOperatorEdgePositioning::ENUMEDGEPOS_UP:
            case CheckOperatorEdgePositioning::ENUMEDGEPOS_DOWN:
            {

                bool  exist=false;
                int orderNo=0;
                for (int idx=0;idx<refH.size();++idx)
                {
                    if((*itor)->stu_CheckOperatorBasic->Index==std::atoi(refH[idx].c_str()))
                    {
                        exist=true;
                        orderNo=idx+1;
                    }
                }

                QStandardItem * itemOrder;
                if(exist){
                    itemOrder=new QStandardItem(std::to_string(orderNo).c_str());
                }
                else
                {
                    itemOrder=new QStandardItem("");
                }
                itemOrder->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                this->m_ModelVPos->setItem(v_Num,0,itemNo);
                this->m_ModelVPos->setItem(v_Num,1,itemName);
                this->m_ModelVPos->setItem(v_Num,2,itemOrder);
                DSDEBUG<<itemOrder->data().toInt()<<endl;
                v_Num++;
            }
                break;
            }
        }
            break;
        default:
            break;
        }
    }

}

void DlgDefectInspect2::initControl()
{
    ui->lineEditImageArea->setEnabled(false);
    if(this->m_ItemImage!=nullptr)
    {
        int wid=this->m_ItemImage->getWidth();
        int hei=this->m_ItemImage->getHeight();
        ui->lineEditImageArea->setText(QString::number(this->m_ItemImage->getHeight()*this->m_ItemImage->getWidth()));
    }

    this->m_ModelHPos=new QStandardItemModel;
    this->m_ModelVPos=new QStandardItemModel;
    QStringList listmodelMain;
    listmodelMain<<" "<<tr("类型")<<tr("顺序");
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,50);
    ui->tableViewH->setColumnWidth(1,100);
    ui->tableViewH->setColumnWidth(2,80);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,50);
    ui->tableViewV->setColumnWidth(1,100);
    ui->tableViewV->setColumnWidth(2,80);
    ui->tableViewV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewV->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewV->verticalHeader()->hide();

    //    ui->groupBoxHPos->setEnabled(false);
    //    ui->groupBoxVPos->setEnabled(false);

    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    ui->radioBtnBin->setChecked(true);
    ui->radioBtnBin->setEnabled(false);
    ui->radioBtnGray->setEnabled(false);




    if(this->m_CheckOperator->IsBlackBlob) ui->radioBtnIsBlackBlob->setChecked(true);
    else ui->radioBtnIsWhiteBlob->setChecked(true);

    ui->spinBoxBinThresh->setValue(this->m_CheckOperator->BinThresh);
    ui->spinBoxMinArea->setValue(this->m_CheckOperator->MaxArea);

    if(this->m_CheckOperator->IsDefectAll) ui->radioBtnDefectAll->setChecked(true);
    else ui->radioBtnDefectSingle->setChecked(true);

    //    ui->labelCalArea->setEnabled(false);
    ui->lineEditImageArea->setEnabled(true);


    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }

    updateCheckRes();
}

DlgDefectInspect2::~DlgDefectInspect2()
{

    if(m_ModelHPos!=nullptr)
    {
        m_ModelHPos->clear();
        delete  m_ModelHPos;
    }
    if(m_ModelVPos!=nullptr)
    {
        m_ModelVPos->clear();
        delete  m_ModelVPos;
    }
    if(this->m_ItemImage!=nullptr)
    {
        delete this->m_ItemImage;
    }
    if(this->m_Scene!=nullptr)
    {
        delete this->m_Scene;
    }
    if(this->m_ItemImageInit!=nullptr)
    {
        delete this->m_ItemImageInit;
    }
    if(this->m_SceneInit!=nullptr)
    {
        delete this->m_SceneInit;
    }



    delete ui;
}
void DlgDefectInspect2::on_pushBtnAutoCalc_clicked()
{
    if(m_Res==-1)
    {
        this->ui->spinBoxMinArea->setValue(this->ui->labelCalArea->text().toInt()+5);
    }
    updateCheckRes();
}

void DlgDefectInspect2::saveParam()
{

    this->m_CheckOperator->MaxArea=this->ui->spinBoxMinArea->value();
    this->m_CheckOperator->BinThresh=this->ui->spinBoxBinThresh->value();
    this->m_CheckOperator->IsBlackBlob=this->ui->radioBtnIsBlackBlob->isChecked();

   this->m_CheckOperator->IsDefectAll=this->ui->radioBtnDefectAll->isChecked();

    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;

}
void DlgDefectInspect2::on_pushBtnSaveReturn_clicked()
{
    //updateCheckRes();
    saveParam();
    this->close();
    //    if(m_Res==0)
    //    {
    //        saveParam();
    //        this->close();
    //    }
    //    else
    //    {
    //        int res=QMessageBox::question(NULL,  "是否保存",  "\n当前参数设置错误,是否继续保存                                  ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    //        if(res==QMessageBox::Yes)
    //        {
    //            saveParam();
    //        }
    //        else
    //        {
    //            return;
    //        }

    //        this->close();
    //    }
}

void DlgDefectInspect2::on_pushBtnCancel_clicked()
{
    this->close();
}

void DlgDefectInspect2::on_tableViewH_doubleClicked(const QModelIndex &index)
{
    if(index.row()>this->m_ModelHPos->rowCount()) return;
    QString strOrder=this->m_ModelHPos->item(index.row(),index.column())->text();
    DSDEBUG<< strOrder.toInt()<<endl;
    if(strOrder.toInt()==0)
    {
        QString strTmp=this->m_ModelHPos->item(index.row(),0)->text();
        int checkOperatorNo= strTmp.mid(1,-1).toUInt();
        switch(index.column())
        {
        case 2:
        {
            StuCheckOperatorBasicControl control(this->m_CheckOperator->stu_CheckOperatorBasic);
            control.addRefX(checkOperatorNo);
            // std::vector<std::string> vstr;
            // this->m_ModelHPos->setData(index,control.getRefX(vstr));
        }
            break;
        default:
            break;
        }
    }
    else
    {
        StuCheckOperatorBasicControl control(this->m_CheckOperator->stu_CheckOperatorBasic);
        int kk=this->m_ModelHPos->item(index.row(),0)->text().mid(1,-1).toUInt();
        control.delRefX(this->m_ModelHPos->item(index.row(),0)->text().mid(1,-1).toUInt());
    }
    loadHVPos();
}

void DlgDefectInspect2::on_tableViewV_doubleClicked(const QModelIndex &index)
{

    if(index.row()>this->m_ModelVPos->rowCount()) return;
    QString strOrder=this->m_ModelVPos->item(index.row(),index.column())->text();
    DSDEBUG<< strOrder.toInt()<<endl;
    if(strOrder.toInt()==0)
    {

        QString strTmp=this->m_ModelVPos->item(index.row(),0)->text();
        int checkOperatorNo= strTmp.mid(1,-1).toUInt();
        switch(index.column())
        {
        case 2:
        {
            StuCheckOperatorBasicControl control(this->m_CheckOperator->stu_CheckOperatorBasic);
            control.addRefY(checkOperatorNo);
            // std::vector<std::string> vstr;
            //this->m_ModelVPos->setData(index,control.getRefY(vstr));
        }
            break;
        default:
            break;
        }
    }
    else
    {
        StuCheckOperatorBasicControl control(this->m_CheckOperator->stu_CheckOperatorBasic);
        control.delRefY(this->m_ModelVPos->item(index.row(),0)->text().mid(1,-1).toUInt());

    }
    loadHVPos();
}

void DlgDefectInspect2::on_toolButtonMin_clicked()
{
    this->ui->graphicsView->scaleMin();
    this->ui->graphicsView_2->scaleMin();
}

void DlgDefectInspect2::on_toolButtonMax_clicked()
{

    this->ui->graphicsView->scaleMax();
    this->ui->graphicsView_2->scaleMax();
}

void DlgDefectInspect2::on_toolButtonInit_clicked()
{

    this->ui->graphicsView->scaleInit();
    this->ui->graphicsView_2->scaleInit();
}

void DlgDefectInspect2::on_spinBoxBinThresh_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDefectInspect2::on_spinBoxMinArea_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDefectInspect2::on_radioBtnIsBlackBlob_clicked()
{
    updateCheckRes();
}

void DlgDefectInspect2::on_radioBtnIsWhiteBlob_clicked()
{
    updateCheckRes();
}

void DlgDefectInspect2::on_radioBtnDefectAll_toggled(bool checked)
{
    updateCheckRes();
}
