#include "dlgcircleinspect.h"
#include "ui_dlgcircleinspect.h"
#include<QMessageBox>
#include<QButtonGroup>
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"kcontrols/dlgkeyboard.h"

DlgCircleInspect::DlgCircleInspect(CheckOperatorCircleInspect* const pCheckOperator,ImgPro* imgPro,OpencvImage & image,int mode,int basis_x,int basis_y,QWidget *parent):
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgCircleInspect)
{
    ui->setupUi(this);
    this->setWindowTitle("缺陷检查");
    InitStyle();
    ui->lab_Title->setText(QString("嘴棒检查--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
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
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x,image.ImageRGB.cols)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs-basis_x,
                   std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye+basis_y,image.ImageRGB.rows)-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys-basis_y
                   );

#endif
    int w0=(int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x;
    int w1= std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe+basis_x,image.ImageRGB.cols);
    int w2= std::min((int)pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye+basis_y,image.ImageRGB.rows);
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

    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);

    this->m_Scene=new GraphicsScene;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);



    m_ItemImageInit=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);


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

    ui->widget_16->setVisible(false);

    //cv::imwrite("d:/cur.bmp",thisImage.ImageGray);

}

void DlgCircleInspect::updateCheckRes()
{
    //this->m_Scene->clear();
    int hei=m_ItemImage->getHeight();
    int wid=m_ItemImage->getWidth();
    CheckOperatorCircleInspectControl checkOperatorCircleInspectControl(this->m_CheckOperator);
    OpencvImage imgRes(ENUMDSIMAGEFORMAT_GRAY,hei,wid);
    int calcArea=0;



#ifdef IMG_TYPE_GRAY
    bool res=pCheckOperatorDefectInspectControl->debugCalculator(this->m_ItemImage->getImage().ImageGray,this->m_ItemImage->getImage().ImageGray,imgRes.ImageGray,
                                                                 cv::Rect(0,0,this->m_ItemImage->getWidth(),this->m_ItemImage->getHeight()),
                                                                 ui->spinBoxBinThresh->value(),ui->radioBtnIsBlackBlob->isChecked(),
                                                                 ui->spinBoxMinArea->value(),
                                                                 &calcArea
                                                                 );
#endif
#ifdef IMG_TYPE_RGB
    cv::Mat imgGray=this->m_ItemImageInit->getImage().ImageGray;
    //     cv::imwrite("d:/2.bmp",imgGray);
    int scaleScore=0, outerArea=0,inerArea=0;
    bool bPresCircle=false, bOuterArea=false, bInerArea=false;


    int p1=ui->spinBoxPrecOut->value();

    int p2=ui->spinBoxPrecIn->value();
    int p3=ui->spinBoxPrecCircle->value();
    int res=checkOperatorCircleInspectControl.debugCalculator(imgGray,imgGray,imgRes.ImageGray, cv::Rect(0,0,this->m_ItemImage->getWidth(),this->m_ItemImage->getHeight()),
                                                              ui->spinBoxPrecOut_2->value(),ui->spinBoxPrecOut_4->value(), ui->spinBoxPrecIn_2->value(),0,
                                                              ui->spinBoxPrecOut->value(),ui->spinBoxPrecIn->value(),ui->spinBoxPrecCircle->value(), &scaleScore,&outerArea,&inerArea,bPresCircle,bOuterArea,bInerArea);


    int res2=checkOperatorCircleInspectControl. calculate( imgGray, imgGray);

    //   cv::imwrite("./refTemp2.bmp",imgGray);
    //   cv::imwrite("./curTemp2.bmp",imgGray);
#endif
    ui->labelScore->setText(QString::number(scaleScore));
    ui->labelScore_2->setText(QString::number(inerArea));
    ui->labelScore_3->setText(QString::number(outerArea));

    //    cv::imwrite("d:/1.bmp",imgRes.ImageGray);
    this->m_ItemImage->reLoadImage(imgRes,imgRes.ImageGray.cols,imgRes.ImageGray.rows);


    {
        QString resStr="";
        DSDEBUG<<"res:"<<res;
        if(bOuterArea||(res==1&&((bOuterArea||bInerArea||bPresCircle)==false)))
        {
            resStr="烟丝异常";
            ui->labelResult_3->setText(resStr);
            this->ui->labelResult_3->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");

        }
        else
        {
            ui->labelResult_3->setText("OK");
            this->ui->labelResult_3->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
            m_Res=0;
        }
        if(bInerArea||(res==1&&((bOuterArea||bInerArea||bPresCircle)==false)))
        {
            resStr="内径异常";
            ui->labelResult_2->setText(resStr);
            this->ui->labelResult_2->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");

        }
        else
        {
            ui->labelResult_2->setText("OK");
            this->ui->labelResult_2->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
            m_Res=0;
        }

        if(bPresCircle||(res==1&&((bOuterArea||bInerArea||bPresCircle)==false)))
        {
            resStr="烟支变型";
            ui->labelResult->setText(resStr);
            this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");

        }
        else
        {
            ui->labelResult->setText("OK");
            this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
            m_Res=0;
        }

        m_Res=-1;
    }

}


void DlgCircleInspect::showKeyBoard(KControlsBase* pLineEdit)
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
void DlgCircleInspect::saveParams()
{
    m_CheckOperator->ThreshLowOut=ui->spinBoxPrecOut_2->value();
    m_CheckOperator->ThreshHighOut=ui->spinBoxPrecOut_4->value();

    //    m_CheckOperator->ThreshLowIn=ui->spinBoxBinThreshInLow->value();
    //    m_CheckOperator->ThreshHighIn=ui->spinBoxBinThreshInHigh->value();

    m_CheckOperator->PresOut=ui->spinBoxPrecOut->value();
    m_CheckOperator->PresIn=ui->spinBoxPrecIn->value();


    m_CheckOperator->ThreshOutCirlVal=0;
    m_CheckOperator->ThreshInCirlVal=ui->spinBoxPrecIn_2->value();

    m_CheckOperator->PresCircle=ui->spinBoxPrecCircle->value();


}

void DlgCircleInspect::initControl()
{
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
    ui->tableViewV->setEditTriggers(QAbstractItemView::NoEditTriggers);    //ui->tableViewV->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewV->verticalHeader()->hide();

    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }



    //        ui->spinBoxBinThreshOutLow->setValue(this->m_CheckOperator->ThreshLowOut);
    //        ui->spinBoxBinThreshOutHigh->setValue(this->m_CheckOperator->ThreshHighOut);
    //    ui->spinBoxBinThreshInLow->setValue(this->m_CheckOperator->ThreshLowIn);
    //    ui->spinBoxBinThreshInHigh->setValue(this->m_CheckOperator->ThreshHighIn);

    ui->spinBoxPrecOut->setValue(m_CheckOperator->PresOut);
    ui->spinBoxPrecIn->setValue(m_CheckOperator->PresIn);

    ui->spinBoxPrecOut_2->setValue(m_CheckOperator->ThreshLowOut);
    ui->spinBoxPrecOut_4->setValue(m_CheckOperator->ThreshHighOut);

    ui->spinBoxPrecIn_2->setValue(m_CheckOperator->ThreshInCirlVal);

    ui->spinBoxPrecCircle->setValue(m_CheckOperator->PresCircle);

    //    connect(ui->spinBoxBinThreshOutLow,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    //    connect(ui->spinBoxBinThreshOutHigh,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    //    connect(ui->spinBoxBinThreshInLow,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    //    connect(ui->spinBoxBinThreshInHigh,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    connect(ui->spinBoxPrecIn,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxPrecIn_2,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    connect(ui->spinBoxPrecCircle,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    connect(ui->spinBoxPrecOut,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxPrecOut_2,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxPrecOut_4,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


    connect(ui->graphicsView,SIGNAL(infoCoordinate(int ,int )),this,SLOT(showPixelInfo(int ,int )));
    connect(ui->graphicsView_2,SIGNAL(infoCoordinate(int ,int )),this,SLOT(showPixelInfo(int ,int )));

    updateCheckRes();
}

void DlgCircleInspect::showPixelInfo(int x,int y)
{
    // DSDEBUG<<x<<"  "<<y;
    if(x<0||x>=thisImage.ImageGray.cols||y<0||y>=thisImage.ImageGray.rows){

        this->ui->labelPixInfo->setText("(000,000):000");
        return;
    }

    QString pixInfoStr=QString("(%1,%2):%3").arg(x, 3, 10, QChar('0')).arg(y, 3, 10, QChar('0')).arg(this->thisImage.ImageGray.at<uchar>(y,x), 3, 10, QChar('0'));
    this->ui->labelPixInfo->setText(pixInfoStr);
}


DlgCircleInspect::~DlgCircleInspect()
{
    delete ui;
}
QString DlgCircleInspect::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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
    case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        ret="嘴棒检查";
        break;
    }
    return ret;
}

void DlgCircleInspect::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,60);
    ui->tableViewH->setColumnWidth(1,110);
    ui->tableViewH->setColumnWidth(2,95);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,60);
    ui->tableViewV->setColumnWidth(1,110);
    ui->tableViewV->setColumnWidth(2,94);
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

void DlgCircleInspect::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgCircleInspect::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgCircleInspect::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgCircleInspect::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgCircleInspect::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgCircleInspect::on_btnMenu_Close_clicked()
{
    this->close();
}

void DlgCircleInspect::on_toolButtonMin_clicked()
{
    this->ui->graphicsView->scaleMin();
    this->ui->graphicsView_2->scaleMin();
}

void DlgCircleInspect::on_toolButtonMax_clicked()
{
    this->ui->graphicsView->scaleMax();
    this->ui->graphicsView_2->scaleMax();
}

void DlgCircleInspect::on_toolButtonInit_clicked()
{
    this->ui->graphicsView->scaleInit();
    this->ui->graphicsView_2->scaleInit();
}

void DlgCircleInspect::on_spinBoxBinThresh_valueChanged(int arg1)
{
    updateCheckRes();
}


void DlgCircleInspect::on_horizontalSliderspinBoxPrecOut_valueChanged(int value)
{
    this->ui->spinBoxPrecOut->setValue(value);

}

void DlgCircleInspect::on_horizontalSliderspinBoxPrecIn_valueChanged(int value)
{
    this->ui->spinBoxPrecIn->setValue(value);
}

void DlgCircleInspect::on_horizontalSliderPrecCircle_valueChanged(int value)
{
    this->ui->spinBoxPrecCircle->setValue(value);
}

void DlgCircleInspect::on_spinBoxPrecCircle_valueChanged(int arg1)
{
    this->ui->horizontalSliderPrecCircle->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxPrecIn_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxPrecIn->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxPrecOut_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxPrecOut->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxBinThreshOutLow_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxBinThreshOutHigh_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxBinThreshInLow_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgCircleInspect::on_spinBoxBinThreshInHigh_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgCircleInspect::on_pushBtnSaveReturn_clicked()
{
    saveParams();
    this->close();
}

void DlgCircleInspect::on_pushBtnCancel_clicked()
{
    this->close();
}




void DlgCircleInspect::on_spinBoxPrecIn_2_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxPrecIn_2->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_horizontalSliderspinBoxPrecIn_2_valueChanged(int value)
{
    this->ui->spinBoxPrecIn_2->setValue(value);
}

void DlgCircleInspect::on_spinBoxPrecOut_2_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxPrecOut_2->setValue(arg1);
//    if(arg1>ui->spinBoxPrecOut_4->value())
//    {
//        ui->spinBoxPrecOut_2->setValue(ui->spinBoxPrecOut_4->value());
//        return;
//    }
    updateCheckRes();
}

void DlgCircleInspect::on_horizontalSliderspinBoxPrecOut_2_valueChanged(int value)
{
    this->ui->spinBoxPrecOut_2->setValue(value);
}

void DlgCircleInspect::on_spinBoxPrecOut_4_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxPrecOut_4->setValue(arg1);
//    if(arg1<ui->spinBoxPrecOut_2->value())
//    {
//        ui->spinBoxPrecOut_4->setValue(ui->spinBoxPrecOut_2->value());
//        return;
//    }
    updateCheckRes();
}

void DlgCircleInspect::on_horizontalSliderspinBoxPrecOut_4_valueChanged(int value)
{

    this->ui->spinBoxPrecOut_4->setValue(value);
}


void DlgCircleInspect::on_spinBoxquezhiArea_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxquezhiArea->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_horizontalSliderspinBoxquezhiArea_valueChanged(int value)
{
      this->ui->spinBoxquezhiArea->setValue(value);
}

void DlgCircleInspect::on_spinBoxquezhiGrayVal_valueChanged(int arg1)
{
    this->ui->horizontalSliderspinBoxquezhiGrayVal->setValue(arg1);
    updateCheckRes();
}

void DlgCircleInspect::on_horizontalSliderspinBoxquezhiGrayVal_valueChanged(int value)
{
     this->ui->spinBoxquezhiGrayVal->setValue(value);
}
