#include "dlgstayinspect.h"
#include "ui_dlgstayinspect.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"

#include"kcontrols/dlgkeyboard.h"
DlgStayInspect::DlgStayInspect(CheckOperatorStayInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x,int basis_y,int mode,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgStayInspect)
{
    ui->setupUi(this);
    this->setWindowTitle("拉线检查");
    InitStyle();
    ui->lab_Title->setText(QString("拉线检测--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    m_LineItem=nullptr;
    m_Res=-1;
    m_ModelHPos=nullptr;
    m_ModelVPos=nullptr;
    m_ItemImage=nullptr;
    m_Scene=nullptr;
    OpencvImage thisImage;
    thisImage.Format=image.Format;

    cv::Rect rect( qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs+basis_x,0),
                   qMax((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys+basis_y,0),
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x,image.ImageGray.cols-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs-basis_x,
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye+basis_y,image.ImageGray.rows-1)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys-basis_y
                   );

    thisImage.Format=image.Format;
    switch (thisImage.Format)
    {
    case ENUMDSIMAGEFORMAT_GRAY:
        thisImage.ImageGray=cv::Mat(image.ImageGray,rect);
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


    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);

    this->m_Scene=new GraphicsSceneCheckOperatorMain;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);
    m_LineItem=new QGraphicsLineItem;
    QPen pen;
    pen.setColor(QColor(0,255,0));
    m_LineItem->setPen(pen);
    this->m_Scene->addItem(this->m_LineItem);
    this->ui->graphicsView->setScene(m_Scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    initControl();
    loadHVPos();

    updateCheckRes();
}


void DlgStayInspect::showKeyBoard(KControlsBase* pLineEdit)
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



void DlgStayInspect::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgStayInspect::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgStayInspect::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgStayInspect::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgStayInspect::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgStayInspect::on_btnMenu_Close_clicked()
{
    this->close();
}



void DlgStayInspect::initControl()
{

    connect(ui->spinBoxAveShift,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxSetScore,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxPackWidth,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxAngleShift,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxBinThreshold,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    ui->lineEditCurAngle->setEnabled(false);
    ui->lineEditCalcScore->setEnabled(false);

    this->ui->labelRecommentThresh->hide();
    this->m_ModelHPos=new QStandardItemModel;
    this->m_ModelVPos=new QStandardItemModel;
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
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

    //拉线方向
    ui->comboBoxLineDir->addItem(QString("水平"));
    ui->comboBoxLineDir->addItem(QString("垂直"));


    //拉线类型
    ui->comboBoxType->addItem(QString("单侧"));
    ui->comboBoxType->addItem(QString("双侧"));
    ui->comboBoxType->addItem(QString("不限制"));
    ui->comboBoxType->setEnabled(false);

    //线颜色，亮色和暗色
    ui->comboBoxLineColor->addItem(QString("亮色"));
    ui->comboBoxLineColor->addItem(QString("暗色"));
    ui->comboBoxLineColor->setEnabled(false);

    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    ui->spinBoxSetScore->setValue((int)(this->m_CheckOperator->Score));
    ui->spinBoxAngleShift->setValue((int)this->m_CheckOperator->AngleShift);
    ui->spinBoxPackWidth->setValue((int)this->m_CheckOperator->PackWidth);
    ui->spinBoxAveShift->setValue((int)this->m_CheckOperator->AveShift);
    ui->spinBoxBinThreshold->setValue((int)this->m_CheckOperator->BinThreshold);

    switch (this->m_CheckOperator->LineDir)
    {
    case CheckOperatorStayInspect::ENUMLINEDIR_HORIZONTAL:
        ui->comboBoxLineDir->setCurrentIndex(0);
        break;
    case CheckOperatorStayInspect::ENUMLINEDIR_VERTICAL:
        ui->comboBoxLineDir->setCurrentIndex(1);
        break;
    }

    switch (this->m_CheckOperator->LineType)
    {
    case CheckOperatorStayInspect::ENUMLINTYPE_SINGLE:
        this->ui->comboBoxType->setCurrentIndex(0);
        break;
    case CheckOperatorStayInspect::ENUMLINTYPE_DOUBLE:
        this->ui->comboBoxType->setCurrentIndex(1);
        break;
    case CheckOperatorStayInspect::ENUMLINTYPE_DEFAULT:
        this->ui->comboBoxType->setCurrentIndex(2);
        break;
    }

    switch (this->m_CheckOperator->LineColor)
    {
    case CheckOperatorStayInspect::ENUMLINECOLOR_BRIGHT:
        this->ui->comboBoxLineColor->setCurrentIndex(0);
        break;
    case CheckOperatorStayInspect::ENUMLINECOLOR_DARK:
        this->ui->comboBoxLineColor->setCurrentIndex(1);
        break;
    }

    ui->spinBoxSetScore->setValue(this->m_CheckOperator->Score);
    ui->spinBoxAngleShift->setValue(this->m_CheckOperator->AngleShift);
    ui->spinBoxPackWidth->setValue(this->m_CheckOperator->PackWidth);
    ui->spinBoxAveShift->setValue(this->m_CheckOperator->GrayAve);
    ui->spinBoxBinThreshold->setValue(this->m_CheckOperator->BinThreshold);

    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }

}


void DlgStayInspect::updateRecommentThresh()
{
    int threshRecomment=0;
    CheckOperatorStayInspectControl control(this->m_CheckOperator);
    control.getRecommentThresh(this->m_ItemImage->getImage().ImageGray,static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(ui->comboBoxLineDir->currentIndex()),&threshRecomment);
    this->ui->labelRecommentThresh->setText(std::to_string(threshRecomment).c_str());
}


void DlgStayInspect::updateCheckRes()
{
    CheckOperatorStayInspectControl control(this->m_CheckOperator);

    int x1=0,y1=0,x2=0,y2=0;
    int score=0;
    int setScore=ui->spinBoxSetScore->value();
    int binThresh=this->ui->spinBoxBinThreshold->value();
    double angle=0;


    int ret=control.debugCalculate(this->m_ItemImage->getImage().ImageGray,
                                   this->m_ItemImage->getImage().ImageGray,
                                   static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(ui->comboBoxLineDir->currentIndex()),
                                   this->ui->spinBoxAngleShift->value(),
                                   setScore,
                                   binThresh,
                                   &x1,&y1,&x2,&y2,
                                   &score,
                                   &angle);

    if(ret!=0)
    {
        this->ui->labelResult->setText("NG");

        this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");
        this->ui->lineEditCalcScore->setText(std::to_string(score).c_str());
        this->ui->lineEditCurAngle->setText(QString::number(qAbs(angle),'f',2));
        this->m_LineItem->setLine(QLine(0,0,0,0));
        m_Res=-1;
        return;
    }
    this->ui->lineEditCalcScore->setText(std::to_string(score).c_str());
    this->ui->lineEditCurAngle->setText(QString::number(qAbs(angle),'f',2));
    this->ui->labelResult->setText("OK");
    this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
    this->m_LineItem->setLine(QLine(x1,y1,x2,y2));
    m_Res=0;
}

QString DlgStayInspect::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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

void DlgStayInspect::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,50);
    ui->tableViewH->setColumnWidth(1,100);
    ui->tableViewH->setColumnWidth(2,79);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,50);
    ui->tableViewV->setColumnWidth(1,100);
    ui->tableViewV->setColumnWidth(2,79);
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
            //横向
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

            //纵向
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


DlgStayInspect::~DlgStayInspect()
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
    if(m_LineItem!=nullptr)
    {
        delete  m_LineItem;
        m_LineItem=nullptr;
    }
    if(m_ItemImage!=nullptr)
    {
        delete  m_ItemImage;
        m_ItemImage=nullptr;
    }
    if(m_Scene!=nullptr)
    {
        m_Scene->clear();
        delete  m_Scene;
        m_Scene=nullptr;
    }

    delete ui;
}


void DlgStayInspect::saveParam()
{
    //设定得分
    this->m_CheckOperator->Score=this->ui->spinBoxSetScore->value();
    //拉线的方向
    //this->m_CheckOperator->LineDir=static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(this->ui->comboBoxLineDir->currentIndex());
    this->m_CheckOperator->LineDir=static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(1);
    //拉线类型
    this->m_CheckOperator->LineType=static_cast<CheckOperatorStayInspect::ENUMLINTYPE>(this->ui->comboBoxType->currentIndex());
    //拉线颜色
    this->m_CheckOperator->LineColor=static_cast<CheckOperatorStayInspect::ENUMLINECOLOR>(this->ui->comboBoxLineColor->currentIndex());
    //角度偏差
    this->m_CheckOperator->AngleShift=this->ui->spinBoxAngleShift->value();
    //拉线宽度
    this->m_CheckOperator->PackWidth=this->ui->spinBoxPackWidth->value();
    //灰度补正
    this->m_CheckOperator->GrayAve=this->ui->spinBoxAveShift->value();
    //二值阈值
    this->m_CheckOperator->BinThreshold=this->ui->spinBoxBinThreshold->value();
    //    switch (this->m_CheckOperator->LineDir) {
    //  case  CheckOperatorStayInspect::ENUMLINEDIR_HORIZONTAL:
    //        this->m_CheckOperator->AngleMin=-this->ui->spinBoxAngleShift->value();
    //         this->m_CheckOperator->AngleMin=this->ui->spinBoxAngleShift->value();
    //        break;
    //   case CheckOperatorStayInspect::ENUMLINEDIR_VERTICAL:

    //        break;

    //    }
    this->m_CheckOperator->AveShift=this->ui->spinBoxAngleShift->value();
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;

}

void DlgStayInspect::on_pushBtnSaveReturn_clicked()
{
    saveParam();
    this->close();

    //    updateCheckRes();
    //    if(m_Res==0)
    //    {
    //        saveParam();
    //        this->close();
    //    }
    //    else
    //    {
    //        int res=QMessageBox::question(NULL,  "是否保存",  "\n当前参数设置错误,是否继续保存                                    ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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

void DlgStayInspect::on_pushBtnAutoCalc_clicked()
{
    updateCheckRes();
}

void DlgStayInspect::on_tableViewH_doubleClicked(const QModelIndex &index)
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
        //int kk=this->m_ModelHPos->item(index.row(),0)->text().mid(1,-1).toUInt();
        control.delRefX(this->m_ModelHPos->item(index.row(),0)->text().mid(1,-1).toUInt());
    }
    loadHVPos();
}

void DlgStayInspect::on_tableViewV_doubleClicked(const QModelIndex &index)
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

void DlgStayInspect::on_comboBoxLineDir_currentIndexChanged(int index)
{
    updateRecommentThresh();
    updateCheckRes();
}

void DlgStayInspect::on_pushBtnCancel_clicked()
{

    close();
}

void DlgStayInspect::on_spinBoxBinThreshold_valueChanged(int arg1)
{
      updateCheckRes();
}
