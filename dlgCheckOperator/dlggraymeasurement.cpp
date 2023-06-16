#include "dlggraymeasurement.h"
#include "ui_dlggraymeasurement.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"

#include"kcontrols/dlgkeyboard.h"
DlgGrayMeasurement::DlgGrayMeasurement(CheckOperatorGrayMeasurement* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x,int basis_y,int mode,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgGrayMeasurement)
{
    ui->setupUi(this);
    this->setWindowTitle("灰度测量");

    InitStyle();
    ui->lab_Title->setText(QString("灰度测量C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    m_ModelHPos=nullptr;
    m_ModelVPos=nullptr;
    m_Res=-1;

    OpencvImage thisImage;
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

    thisImage.Format=ENUMDSIMAGEFORMAT_GRAY;

    switch (image.Format)
    {
    case ENUMDSIMAGEFORMAT_GRAY:
        thisImage.ImageGray=cv::Mat(image.ImageGray,rect);
        thisImage.Width=rect.width;
        thisImage.Height=rect.height;
        break;
    case ENUMDSIMAGEFORMAT_RGB:
        thisImage.ImageRGB=cv::Mat(image.ImageRGB,rect);
        thisImage.Width=rect.width;
        thisImage.Height=rect.height;
        break;
    case ENUMDSIMAGEFORMAT_R:
        break;
    case ENUMDSIMAGEFORMAT_G:
        break;
    case ENUMDSIMAGEFORMAT_B:
        break;
    }
    cv::cvtColor(thisImage.ImageRGB,thisImage.ImageGray,cv::COLOR_BGR2GRAY);
    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);

    this->m_Scene=new GraphicsSceneCheckOperatorMain;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);
    this->ui->graphicsView->setScene(m_Scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    ui->graphicsView->setSceneRect(QRect(0,0,thisImage.Width,thisImage.Height));
    initControl();
    loadHVPos();
}

void DlgGrayMeasurement::showKeyBoard(KControlsBase* pLineEdit)
{

    switch (pLineEdit->type())
    {
    case ENUMKCONTROLTYPE_KSPINBOX:
    {
        KSpinBoxKb* pWidget=static_cast<KSpinBoxKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x()-40,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    case ENUMKCONTROLTYPE_KLINEEDIT:
    {
        KLineEditKb* pWidget=static_cast<KLineEditKb*>(pLineEdit);

        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        dlg.setGeometry(pt.x()-pWidget->geometry().x()-40,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2,dlg.geometry().width(),dlg.geometry().height());
        dlg.exec();
    }
        break;
    }

}


void DlgGrayMeasurement::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}




bool DlgGrayMeasurement::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgGrayMeasurement::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgGrayMeasurement::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgGrayMeasurement::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgGrayMeasurement::on_btnMenu_Close_clicked()
{
    this->close();
}


void DlgGrayMeasurement::updateCheckRes()
{
    qDebug()<<"update!"<<endl;
    ui->labelMeanDown->setText(QString::number(ui->lineEditSetMean->text().toDouble()-ui->spinBoxMeanBasis->value(),'f',2));
    ui->labelMeanUp->setText(QString::number(ui->lineEditSetMean->text().toDouble()+ui->spinBoxMeanBasis->value(),'f',2));
    ui->labelStddevDown->setText(QString::number(ui->lineEditSetStddev->text().toDouble()-ui->spinBoxStddevBasis->value(),'f',2));
    ui->labelStddevUp->setText(QString::number(ui->lineEditSetStddev->text().toDouble()+ui->spinBoxStddevBasis->value(),'f',2));


    int hei=m_ItemImage->getHeight();
    int wid=m_ItemImage->getWidth();
    CheckOperatorGrayMeasurementControl * pCheckOperatorGrayMeasurementControl=new CheckOperatorGrayMeasurementControl(this->m_CheckOperator);
    double resMean=0,resStddev=0;


    OpencvImage imgRes(ENUMDSIMAGEFORMAT_GRAY,hei,wid);



    int res=pCheckOperatorGrayMeasurementControl->debugCalculator(this->m_ItemImage->getImage().ImageGray,this->m_ItemImage->getImage().ImageGray,imgRes.ImageGray,cv::Rect(0,0,wid,hei),
                                                                  ui->lineEditSetMean->text().toDouble(),ui->spinBoxMeanBasis->value(),ui->lineEditSetStddev->text().toDouble(),
                                                                  ui->spinBoxStddevBasis->value(),&resMean,&resStddev
                                                                  );


    this->m_ItemImage->reLoadImage(imgRes,imgRes.ImageGray.cols,imgRes.ImageGray.rows);
    ui->lineEditCalcMean->setText(QString::number(resMean,'f',2));
    ui->lineEditCalcStddev->setText(QString::number(resStddev,'f',2));
    if(res==0)
    {
        m_Res=0;
        ui->labelResult->setText("OK");
            this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 11pt \"微软雅黑\";");
    }
    else
    {
        m_Res=-1;
        ui->labelResult->setText("NG");
            this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 11pt \"微软雅黑\";");
    }
    delete  pCheckOperatorGrayMeasurementControl;
}



void DlgGrayMeasurement::initControl()
{




    connect(ui->lineEditSetMean,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->lineEditSetStddev,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


    connect(ui->spinBoxMeanBasis,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxStddevBasis,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

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



    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    ui->lineEditSetMean->setText(QString::number(this->m_CheckOperator->AverageStd));
    ui->lineEditSetMean->setValidator(new QDoubleValidator(0,255,2,this));
    ui->lineEditSetStddev->setText(QString::number(this->m_CheckOperator->DevStd));
    ui->lineEditSetStddev->setValidator(new QDoubleValidator(0,1000,2,this));
    ui->spinBoxMeanBasis->setValue((int)(this->m_CheckOperator->AverageLimited));
    ui->spinBoxStddevBasis->setValue((int)(this->m_CheckOperator->DevLimited));

    ui->labelMeanDown->setNum(this->m_CheckOperator->AverageStd-this->m_CheckOperator->AverageLimited);
    ui->labelMeanUp->setNum(this->m_CheckOperator->AverageStd+this->m_CheckOperator->AverageLimited);
    ui->labelStddevDown->setNum(this->m_CheckOperator->DevStd-this->m_CheckOperator->DevLimited);
    ui->labelStddevUp->setNum(this->m_CheckOperator->DevStd+this->m_CheckOperator->DevLimited);

    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
        this->ui->lineEditSetMean->setReadOnly(true);
        this->ui->lineEditSetStddev->setReadOnly(true);
        this->ui->lineEditCalcMean->setReadOnly(true);
        this->ui->lineEditCalcStddev->setReadOnly(true);

    }
    else
    {
        this->ui->lineEditSetMean->setReadOnly(false);
        this->ui->lineEditSetStddev->setReadOnly(false);
        this->ui->lineEditCalcMean->setReadOnly(true);
        this->ui->lineEditCalcStddev->setReadOnly(true);
    }


    updateCheckRes();
}

DlgGrayMeasurement::~DlgGrayMeasurement()
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
    if(m_ItemImage!=nullptr)
    {
        delete  m_ItemImage;

    }
    if(this->m_Scene!=nullptr)
    {
        this->m_Scene->clear();
        delete  this->m_Scene;
    }


    delete ui;
}

void DlgGrayMeasurement::on_pushBtnAutoCalc_clicked()
{
    if(this->m_Mode)//tiaoshi
    {
        if( std::abs(this->ui->lineEditCalcMean->text().toDouble()-this->ui->lineEditSetMean->text().toDouble())>(double)this->ui->spinBoxMeanBasis->value())
        {
            this->ui->spinBoxMeanBasis->setValue(
                        (int)std::abs(this->ui->lineEditCalcMean->text().toDouble()-this->ui->lineEditSetMean->text().toDouble())+2
                        );

        }

        if( std::abs(this->ui->lineEditSetStddev->text().toDouble()-this->ui->lineEditCalcStddev->text().toDouble())>(double)this->ui->spinBoxStddevBasis->value())
        {
            this->ui->spinBoxStddevBasis->setValue(
                        (int)std::abs(this->ui->lineEditSetStddev->text().toDouble()-this->ui->lineEditSetStddev->text().toDouble())+2
                        );

        }

    }
    else
    {

        this->ui->lineEditSetMean->setText(this->ui->lineEditCalcMean->text());
        this->ui->lineEditSetStddev->setText(this->ui->lineEditCalcStddev->text());
        this->ui->spinBoxMeanBasis->setValue(10);
        this->ui->spinBoxStddevBasis->setValue(10);
    }
    updateCheckRes();
}

void DlgGrayMeasurement::on_spinBoxMeanBasis_valueChanged(int arg1)
{
      updateCheckRes();
}

void DlgGrayMeasurement::on_spinBoxStddevBasis_valueChanged(int arg1)
{
       updateCheckRes();
}

void DlgGrayMeasurement::on_lineEditSetStddev_textChanged(const QString &arg1)
{
     updateCheckRes();
}

void DlgGrayMeasurement::on_lineEditSetMean_textChanged(const QString &arg1)
{
     updateCheckRes();
}

void DlgGrayMeasurement::saveParam()
{
    this->m_CheckOperator->AverageStd=this->ui->lineEditSetMean->text().toFloat();
    this->m_CheckOperator->AverageLimited=this->ui->spinBoxMeanBasis->value();
    this->m_CheckOperator->DevStd=this->ui->lineEditSetStddev->text().toFloat();
    this->m_CheckOperator->DevLimited=this->ui->spinBoxStddevBasis->text().toInt();

    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
}

void DlgGrayMeasurement::on_pushBtnSaveReturn_clicked()
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
    //        int res=QMessageBox::question(NULL,  "是否保存",  "\n当前参数设置错误,是否继续保存                                         ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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

void DlgGrayMeasurement::on_pushBtnCancel_clicked()
{
    //    QList<QGraphicsItem *>  lstItem=this->m_Scene->items();
    //    DSDEBUG<<lstItem.size();

    //    return;
        this->close();
}

QString DlgGrayMeasurement::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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

void DlgGrayMeasurement::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,52);
    ui->tableViewH->setColumnWidth(1,100);
    ui->tableViewH->setColumnWidth(2,90);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,51);
    ui->tableViewV->setColumnWidth(1,100);
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
void DlgGrayMeasurement::on_tableViewH_doubleClicked(const QModelIndex &index)
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

void DlgGrayMeasurement::on_tableViewV_doubleClicked(const QModelIndex &index)
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
