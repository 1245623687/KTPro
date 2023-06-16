#include "dlgassociatedinspect.h"
#include "ui_dlgassociatedinspect.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"kcontrols/dlgkeyboard.h"
DlgAssociatedInspect::DlgAssociatedInspect(CheckOperatorAssociatedInspect* const pCheckOperator,ImgPro* imgPro,int mode,QWidget *parent) :
    QDialog(parent),
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    ui(new Ui::DlgAssociatedInspect)
{
    ui->setupUi(this);
    this->setWindowTitle("关联检查");
    InitStyle();
    this->ui->lab_Title->setText(QString("关联检查--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    initControl();
    loadHVPos();
    loadAss();
    updateCheckRes();

    //    this->ui->labelRes->setText("OK");
    //    this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");

}
void DlgAssociatedInspect::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}




bool DlgAssociatedInspect::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgAssociatedInspect::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgAssociatedInspect::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgAssociatedInspect::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgAssociatedInspect::on_btnMenu_Close_clicked()
{
    this->close();
}

void DlgAssociatedInspect::on_comboBox_currentIndexChanged(int index)
{
    this->m_CheckOperator->LineType=static_cast<CheckOperatorAssociatedInspect::ENUMLINETYPE>(index);
    switch (this->m_CheckOperator->LineType)
    {
    case CheckOperatorAssociatedInspect::ENUMLINETYPE_SINGLE:
    {
        ImgProControl control(this->m_ImgPro);
        if(this->m_CheckOperator->Relaty1!=0)
            if(control.getCheckNo(this->m_CheckOperator->Relaty1)->CheckType!=ENUMCHECKOPERATORTYPE_STAYINSPECT)
                this->m_CheckOperator->Relaty1=0;
        if(this->m_CheckOperator->Relaty2!=0)
            if(control.getCheckNo(this->m_CheckOperator->Relaty2)->CheckType!=ENUMCHECKOPERATORTYPE_STAYINSPECT)
                this->m_CheckOperator->Relaty2=0;

    }
        break;
    case CheckOperatorAssociatedInspect::ENUMLINETYPE_DOUBLE:
    {
        ImgProControl control(this->m_ImgPro);
        if(this->m_CheckOperator->Relaty1!=0)
            if(control.getCheckNo(this->m_CheckOperator->Relaty1)->CheckType!=ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT)
                this->m_CheckOperator->Relaty1=0;
        if(this->m_CheckOperator->Relaty2!=0)
            if(control.getCheckNo(this->m_CheckOperator->Relaty2)->CheckType!=ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT)
                this->m_CheckOperator->Relaty2=0;
    }
        break;
    }
    ui->lineEditCurDis->setText(0);
    loadAss();
    updateCheckRes();
}
void DlgAssociatedInspect::updateCheckRes()
{
    if(this->m_CheckOperator->Relaty1==0||this->m_CheckOperator->Relaty2==0)
    {
        this->ui->labelRes->setText("OK");
        this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
        this->ui->lineEditCurDis->setText(0);
        return ;
    }

//    ImgProControl tmpcontrol(m_ImgPro);
//    unsigned char restmp[128]={0};
//    tmpcontrol.calculateCV(restmp);
    switch (this->m_CheckOperator->LineType)
    {
    case CheckOperatorAssociatedInspect::ENUMLINETYPE_SINGLE:
    {

        ImgProControl control(this->m_ImgPro);
        int res1=static_cast<CheckOperatorStayInspect*>(
                    control.getCheckNo(this->m_CheckOperator->Relaty1))->center;
        DSDEBUG<<"res1:"<<res1<<endl;

        int res2=static_cast<CheckOperatorStayInspect*>(
                    control.getCheckNo(this->m_CheckOperator->Relaty2))->center;
        DSDEBUG<<"res2:"<<res2<<endl;

        if(std::abs(res2-res1)>this->ui->spinBoxMaxShift->value())
        {
            this->ui->labelRes->setText("NG");
            this->ui->labelRes->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");

            this->ui->lineEditCurDis->setText(QString::number(std::abs(res2-res1)));
        }
        else
        {
            this->ui->labelRes->setText("OK");
            this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
            this->ui->lineEditCurDis->setText(QString::number(std::abs(res2-res1)));
        }
    }
        break;

    case CheckOperatorAssociatedInspect::ENUMLINETYPE_DOUBLE:
    {
        ImgProControl control(this->m_ImgPro);
        int res11=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(this->m_CheckOperator->Relaty1))->center1;
        int res12=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(this->m_CheckOperator->Relaty1))->center2;

        int res21=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(this->m_CheckOperator->Relaty2))->center1;
        int res22=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(this->m_CheckOperator->Relaty2))->center2;
        int dis1=std::abs(qMin(res11,res12)-qMin(res21,res22));
        int dis2=std::abs(qMax(res11,res12)-qMax(res21,res22));
        this->ui->lineEditCurDis->setText(QString::number(qMax(dis1,dis2)));
        if(dis1>this->ui->spinBoxMaxShift->value()||dis2>this->ui->spinBoxMaxShift->value())
        {
            this->ui->labelRes->setText("NG");
            this->ui->labelRes->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");
        }
        else
        {
            this->ui->labelRes->setText("OK");
            this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
        }

    }
        break;
    }





}

void DlgAssociatedInspect::loadAss()
{
    this->m_Model->clear();
    QStringList listmodelMain;
    listmodelMain<<"编号"<<"类型"<<"方向"<<"状态";
    this->m_Model->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewAss->setModel(this->m_Model);
    ui->tableViewAss->setColumnWidth(0,56);
    ui->tableViewAss->setColumnWidth(1,100);
    ui->tableViewAss->setColumnWidth(2,70);
    ui->tableViewAss->setColumnWidth(3,70);
    ui->tableViewAss->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list<CheckOperatorBasic*>::iterator itor= m_ImgPro->LstCheckOperator.begin();
    int row_idx=0;
    for (;itor!=m_ImgPro->LstCheckOperator.end();++itor)
    {

        if((*itor)->CheckType==ENUMCHECKOPERATORTYPE_STAYINSPECT)
        {
            CheckOperatorStayInspect* pTmp=static_cast<CheckOperatorStayInspect*>((*itor));
            QStandardItem* itemNo=new QStandardItem(QString("C%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            QStandardItem* itemType=new QStandardItem(QString("拉线检查"));
            QStandardItem* itemDir=new QStandardItem(pTmp->LineDir?QString("垂直"):QString("水平"));
            QStandardItem* itemEnable;
            if(m_CheckOperator->Relaty1==pTmp->stu_CheckOperatorBasic->Index)
            {
                itemEnable=new QStandardItem("Y");
            }
            else if(m_CheckOperator->Relaty2==pTmp->stu_CheckOperatorBasic->Index)
            {
                itemEnable=new QStandardItem("Y");
            }
            else
            {
                itemEnable=new QStandardItem();
            }

            itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemType->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemDir->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemEnable->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            this->m_Model->setItem(row_idx,0,itemNo);
            this->m_Model->setItem(row_idx,1,itemType);
            this->m_Model->setItem(row_idx,2,itemDir);
            this->m_Model->setItem(row_idx,3,itemEnable);
            row_idx++;
        }


        if((*itor)->CheckType==ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT)
        {
            CheckOperatorStayInspect* pTmp=static_cast<CheckOperatorStayInspect*>((*itor));
            QStandardItem* itemNo=new QStandardItem(QString("C%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            QStandardItem* itemType=new QStandardItem(QString("双线检查"));
            QStandardItem* itemDir=new QStandardItem(pTmp->LineDir?QString("垂直"):QString("水平"));
            QStandardItem* itemEnable;
            if(m_CheckOperator->Relaty1==pTmp->stu_CheckOperatorBasic->Index)
            {
                itemEnable=new QStandardItem("Y");
            }
            else if(m_CheckOperator->Relaty2==pTmp->stu_CheckOperatorBasic->Index)
            {
                itemEnable=new QStandardItem("Y");
            }
            else
            {
                itemEnable=new QStandardItem();
            }

            itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemType->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemDir->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemEnable->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            this->m_Model->setItem(row_idx,0,itemNo);
            this->m_Model->setItem(row_idx,1,itemType);
            this->m_Model->setItem(row_idx,2,itemDir);
            this->m_Model->setItem(row_idx,3,itemEnable);
            row_idx++;
        }
    }

}


void DlgAssociatedInspect::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,76);
    ui->tableViewH->setColumnWidth(1,130);
    ui->tableViewH->setColumnWidth(2,90);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,76);
    ui->tableViewV->setColumnWidth(1,130);
    ui->tableViewV->setColumnWidth(2,90);
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

DlgAssociatedInspect::~DlgAssociatedInspect()
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
    if(m_Model!=nullptr)
    {
        m_Model->clear();
        delete  m_Model;
    }


    delete ui;
}

QString DlgAssociatedInspect::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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
    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        ret="双线检查";
        break;
    }
    return ret;
}
void DlgAssociatedInspect::on_tableViewH_doubleClicked(const QModelIndex &index)
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

void DlgAssociatedInspect::showKeyBoard(KControlsBase* pLineEdit)
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

void DlgAssociatedInspect::initControl()
{
    connect(ui->spinBoxMaxShift,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

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
    ui->tableViewV->verticalHeader()->hide();

    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }

    this->m_Model=new QStandardItemModel;
    this->ui->tableViewAss->setModel(m_Model);
    this->ui->tableViewAss->verticalHeader()->hide();

    this->ui->lineEditStdDis->setText(QString::number(this->m_CheckOperator->StdDis));
    this->ui->spinBoxMaxShift->setRange(0,100);
    this->ui->spinBoxMaxShift->setValue(this->m_CheckOperator->MaxShift);
    this->ui->lineEditStdDis->setEnabled(false);
    this->ui->lineEditCurDis->setEnabled(false);
    this->ui->comboBox->setCurrentIndex(m_CheckOperator->LineType);

}

void DlgAssociatedInspect::saveParam()
{
    m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
    m_CheckOperator->StdDis=this->ui->lineEditStdDis->text().toInt();
    m_CheckOperator->MaxShift=this->ui->spinBoxMaxShift->value();
    m_CheckOperator->LineType=static_cast<CheckOperatorAssociatedInspect::ENUMLINETYPE>(this->ui->comboBox->currentIndex());
}



void DlgAssociatedInspect::on_tableViewV_doubleClicked(const QModelIndex &index)
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

void DlgAssociatedInspect::on_tableViewAss_doubleClicked(const QModelIndex &index)
{
    if(index.row()>this->m_Model->rowCount()) return;
    QString checkName=this->m_Model->item(index.row(),1)->text().trimmed();
    switch (static_cast<CheckOperatorAssociatedInspect::ENUMLINETYPE>(ui->comboBox->currentIndex()))
    {
    case CheckOperatorAssociatedInspect::ENUMLINETYPE_SINGLE:
    {
        if(checkName!="拉线检查")
            return;
    }
        break;
    case CheckOperatorAssociatedInspect::ENUMLINETYPE_DOUBLE:
    {
        if(checkName!="双线检查")
            return;
     }
        break;

    }

    int Order=this->m_Model->item(index.row(),0)->text().mid(1).toInt();
    if(m_CheckOperator->Relaty1==Order)
    {m_CheckOperator->Relaty1=0; loadAss();
        updateCheckRes();
        return;}
    if(m_CheckOperator->Relaty2==Order)
    {m_CheckOperator->Relaty2=0; loadAss();
        updateCheckRes();
        return;}

    if(m_CheckOperator->Relaty1==0)
    {
        m_CheckOperator->Relaty1=Order;
        loadAss();
        updateCheckRes();
        return;
    }

    if(m_CheckOperator->Relaty2==0)
    {
        m_CheckOperator->Relaty2=Order;
        loadAss();
        updateCheckRes();
        return;
    }

    updateCheckRes();
}

void DlgAssociatedInspect::on_pushBtnSaveReturn_clicked()
{
    saveParam();
    this->close();
}

void DlgAssociatedInspect::on_pushBtnCancel_clicked()
{
    this->close();
}

void DlgAssociatedInspect::on_spinBoxMaxShift_valueChanged(int arg1)
{
    updateCheckRes();

}

void DlgAssociatedInspect::on_pushButton_clicked()
{
    if(this->ui->spinBoxMaxShift->value()<this->ui->lineEditCurDis->text().toInt())
    {
        this->ui->spinBoxMaxShift->setValue(this->ui->lineEditCurDis->text().toInt());
    }
}


