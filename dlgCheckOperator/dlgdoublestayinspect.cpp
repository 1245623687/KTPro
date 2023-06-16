#include "dlgdoublestayinspect.h"
#include "ui_dlgdoublestayinspect.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"kcontrols/dlgkeyboard.h"

DlgDoubleStayInspect::DlgDoubleStayInspect(CheckOperatorDoubleStayInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x,int basis_y,int mode,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgDoubleStayInspect)
{
    ui->setupUi(this);
    this->setWindowTitle("双线检查");
    InitStyle();


    ui->lab_Title->setText(QString("双线检测--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    m_LineItem2=nullptr;
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
    connect(m_Scene,SIGNAL(infoCoordinate(QPointF)),this,SLOT(showCoordinate(QPointF)));

    QPen pen;
    pen.setColor(QColor(0,255,0));
    m_LineItem1=new QGraphicsLineItem;
    m_LineItem1->setPen(pen);
    m_LineItem2=new QGraphicsLineItem;
    m_LineItem2->setPen(pen);
    this->m_Scene->addItem(this->m_LineItem1);
    this->m_Scene->addItem(this->m_LineItem2);
    this->ui->graphicsView->setScene(m_Scene);

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    initControl();
    loadHVPos();
    updateCheckRes();
}


void DlgDoubleStayInspect::updateCheckRes()
{
    CheckOperatorDoubleStayInspectControl control(this->m_CheckOperator);

    int p1x1=0,p1y1=0,p1x2=0,p1y2=0;
    int p2x1=0,p2y1=0,p2x2=0,p2y2=0;
    int score1=0,score2=0;
    int setScore=ui->spinBoxSetScore->value();
    int binThresh=this->ui->spinBoxBinThreshold->value();
    double angle1=0,angle2=0;

    int ret=control.debugCalculate(this->m_ItemImage->getImage().ImageGray,
                                   this->m_ItemImage->getImage().ImageGray,
                                   static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(ui->comboBoxLineDir->currentIndex()),
                                   this->ui->spinBoxAngleShift->value(),this->ui->spinBoxSetScore->value(),ui->spinBoxBinThreshold->value(),
                                   ui->spinBoxDisMin->value(),ui->spinBoxDisMax->value(),&p1x1,&p1y1,&p1x2,&p1y2,&p2x1,&p2y1,&p2x2,&p2y2,&score1,&score2,&angle1,&angle2);

    this->m_LineItem1->setLine(QLine(p1x1,p1y1,p1x2,p1y2));
    this->m_LineItem2->setLine(QLine(p2x1,p2y1,p2x2,p2y2));
    this->ui->lineEditCurAngle1->setText(QString::number(qAbs(angle1),'f',2));
    this->ui->lineEditCurAngle2->setText(QString::number(qAbs(angle2),'f',2));
    this->ui->lineEditCalcScore1->setText(std::to_string(score1).c_str());
    this->ui->lineEditCalcScore2->setText(std::to_string(score2).c_str());

    if(ret!=0)
    {
        this->ui->labelResult->setText("NG");
        this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");

        m_Res=-1;

    }
    else
    {
        this->ui->labelResult->setText("OK");
        this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
        m_Res=0;
    }
}

void DlgDoubleStayInspect::showCoordinate(QPointF pt)
{
    this->ui->labelx->setText(QString::number((int)pt.x()));
    this->ui->labely->setText(QString::number((int)pt.y()));

}

void DlgDoubleStayInspect::initControl()
{


    connect(ui->spinBoxSetScore,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxAngleShift,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxBinThreshold,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxDisMin,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxDisMax,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


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

    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);


    ui->spinBoxSetScore->setValue((int)(this->m_CheckOperator->Score));
    ui->spinBoxAngleShift->setValue((int)this->m_CheckOperator->AngleShift);
    ui->spinBoxBinThreshold->setValue((int)this->m_CheckOperator->BinThreshold);
    ui->spinBoxDisMin->setValue(m_CheckOperator->DisMin);
    ui->spinBoxDisMax->setValue(m_CheckOperator->DisMax);

    switch (this->m_CheckOperator->LineDir)
    {
    case CheckOperatorDoubleStayInspect::ENUMLINEDIR_HORIZONTAL:
        ui->comboBoxLineDir->setCurrentIndex(0);
        break;
    case CheckOperatorDoubleStayInspect::ENUMLINEDIR_VERTICAL:
        ui->comboBoxLineDir->setCurrentIndex(1);
        break;
    }


    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }

}


void DlgDoubleStayInspect::saveParam()
{
    //设定得分
    this->m_CheckOperator->Score=this->ui->spinBoxSetScore->value();
    //拉线的方向
    //this->m_CheckOperator->LineDir=static_cast<CheckOperatorStayInspect::ENUMLINEDIR>(this->ui->comboBoxLineDir->currentIndex());
    this->m_CheckOperator->LineDir=static_cast<CheckOperatorDoubleStayInspect::ENUMLINEDIR>(ui->comboBoxLineDir->currentIndex());
    //角度偏差
    this->m_CheckOperator->AngleShift=this->ui->spinBoxAngleShift->value();
    //二值阈值
    this->m_CheckOperator->BinThreshold=this->ui->spinBoxBinThreshold->value();
    //拉线间隔最小
    this->m_CheckOperator->DisMin=this->ui->spinBoxDisMin->value();
    //拉线间隔最大
    this->m_CheckOperator->DisMax=this->ui->spinBoxDisMax->value();
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;

}


void DlgDoubleStayInspect::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}

void DlgDoubleStayInspect::loadHVPos()
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

void DlgDoubleStayInspect::showKeyBoard(KControlsBase* pLineEdit)
{
    DSDEBUG<<"SHOW KEYBOARD!";
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


QString DlgDoubleStayInspect::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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
    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        ret="双线检查";
        break;
    }
    return ret;

}


bool DlgDoubleStayInspect::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        //this->on_btnMenu_Max_clicked();
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void DlgDoubleStayInspect::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgDoubleStayInspect::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgDoubleStayInspect::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgDoubleStayInspect::on_btnMenu_Close_clicked()
{
    this->close();
}


DlgDoubleStayInspect::~DlgDoubleStayInspect()
{
    delete ui;
}

void DlgDoubleStayInspect::on_pushBtnSaveReturn_clicked()
{
    saveParam();
    this->close();
}

void DlgDoubleStayInspect::on_tableViewH_doubleClicked(const QModelIndex &index)
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

void DlgDoubleStayInspect::on_tableViewV_doubleClicked(const QModelIndex &index)
{
    DSDEBUG<<"on_tableViewV_doubleClicked!";
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



void DlgDoubleStayInspect::on_pushBtnCancel_clicked()
{
    this->close();
}

void DlgDoubleStayInspect::on_spinBoxBinThreshold_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_spinBoxSetScore_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_comboBoxLineDir_currentIndexChanged(int index)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_spinBoxAngleShift_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_spinBoxDisMin_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_spinBoxDisMax_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgDoubleStayInspect::on_pushBtnAutoCalc_clicked()
{
    updateCheckRes();
}
