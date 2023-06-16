#include "dlgbrokenlineinspect.h"
#include "ui_dlgbrokenlineinspect.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"kcontrols/dlgkeyboard.h"
DlgBrokenLineInspect::DlgBrokenLineInspect(CheckOperatorBrokenlineInspect* pCheckOperator,ImgPro* imgPro,OpencvImage & image,int basis_x,int basis_y,int mode,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgBrokenLineInspect)
{
    ui->setupUi(this);
    this->setWindowTitle("折线检查");
    InitStyle();
    ui->lab_Title->setText(QString("折线检查--C%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    m_Res=-1;
    m_ModelHPos=nullptr;
    m_ModelVPos=nullptr;
    m_LineItem=nullptr;
    m_LineItem2=nullptr;
    m_LineRes=nullptr;
    m_ItemImage=nullptr;
    m_Scene=nullptr;
    OpencvImage thisImage;
    thisImage.Format=image.Format;

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
    thisImage.Format=image.Format;
    switch (thisImage.Format)
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


    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);

    this->m_Scene=new GraphicsItemScene;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);
    this->ui->graphicsView->setScene(m_Scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    if(this->m_CheckOperator->Line1Seg_xs==0&&this->m_CheckOperator->Line1Seg_ys==0&&
            this->m_CheckOperator->Line1Seg_ys==0&&this->m_CheckOperator->Line1Seg_ye==0&&
            this->m_CheckOperator->Line2Seg_xs==0&&this->m_CheckOperator->Line2Seg_ys==0&&
            this->m_CheckOperator->Line2Seg_ys==0&&this->m_CheckOperator->Line2Seg_ye==0)
    {
        int roiWidth=this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs;
        int roiHeight=this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys;
        m_LineItem=new GraphicsSingleLineItem(std::min(roiWidth,5),std::min(roiHeight,5),
                                              std::min(roiWidth,5),qMax(0,roiHeight-5));
        m_LineItem2=new GraphicsSingleLineItem(qMax(roiWidth-5,0),std::min(roiHeight,5),
                                               qMax(roiWidth-5,0),qMax(0,roiHeight-5));
    }
    else
    {

        m_LineItem=new GraphicsSingleLineItem(this->m_CheckOperator->Line1Seg_xs,this->m_CheckOperator->Line1Seg_ys,
                                              this->m_CheckOperator->Line1Seg_xe,this->m_CheckOperator->Line1Seg_ye);
        m_LineItem2=new GraphicsSingleLineItem(this->m_CheckOperator->Line2Seg_xs,this->m_CheckOperator->Line2Seg_ys,
                                               this->m_CheckOperator->Line2Seg_xe,this->m_CheckOperator->Line2Seg_ye);
    }

    m_LineRes=new QGraphicsLineItem;
    m_LineRes->setLine(0,0,0,0);
    QPen pen;
    pen.setColor(QColor(255,255,0));
    m_LineRes->setPen(pen);

    this->m_Scene->addItem(m_LineRes);
    this->m_Scene->addItem(m_LineItem);
    this->m_Scene->addItem(m_LineItem2);


    initControl();
    loadHVPos();

}

void DlgBrokenLineInspect::showKeyBoard(KControlsBase* pLineEdit)
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

void DlgBrokenLineInspect::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgBrokenLineInspect::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgBrokenLineInspect::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgBrokenLineInspect::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgBrokenLineInspect::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgBrokenLineInspect::on_btnMenu_Close_clicked()
{
    this->close();
}

DlgBrokenLineInspect::~DlgBrokenLineInspect()
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

    if(m_LineItem2!=nullptr)
    {
        delete  m_LineItem2;
        m_LineItem2=nullptr;
    }
    if(m_LineRes!=nullptr)
    {
        delete  m_LineRes;
        m_LineRes=nullptr;
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




void DlgBrokenLineInspect::initControl()
{

    connect(ui->spinBoxSetScore,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxSetLength,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxAngleShift,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxThreshBasis,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxBinThreshold,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));
    connect(ui->spinBoxLengthThresh,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));


    ui->label_15->setVisible(false);

    ui->lineEditCurLength->setEnabled(false);
    this->ui->spinBoxLengthThresh->setValue(this->m_CheckOperator->StuStuLineLenLineLen->LineLength);
    this->m_ModelHPos=new QStandardItemModel;
    this->m_ModelVPos=new QStandardItemModel;
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,51);
    ui->tableViewH->setColumnWidth(1,125);
    ui->tableViewH->setColumnWidth(2,85);
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

    ui->spinBoxSetScore->setValue(this->m_CheckOperator->Score);
    ui->spinBoxSetLength->setValue(this->m_CheckOperator->LineLength);
    ui->spinBoxBinThreshold->setValue(this->m_CheckOperator->BinThreshold);
    this->ui->spinBoxAngleShift->setValue(this->m_CheckOperator->AngleShift);

    //直线查找的最小线段像素数量
    this->ui->spinBoxLengthThresh->setValue(10);

    //    this->ui->spinBoxSetLength->setVisible(0);
    //    this->ui->lineEditCurLength->setVisible(0);
    //    this->ui->label_11->setVisible(0);
    //    this->ui->label_13->setVisible(0);

    this->ui->spinBoxThreshBasis->setVisible(false);
    if(this->m_Mode==0)//设置模式
    {
        this->ui->lineEditCalcScore->setText(QString::number(100));
        this->ui->lineEditCalcScore->setEnabled(false);
        this->ui->lineEditCurAngle->setText(QString::number(0));
        this->ui->lineEditCurAngle->setEnabled(false);

    }
    else
    {
        this->ui->lineEditCalcScore->setText(QString::number(100));


    }


    //this->ui->label_9->hide();
    //this->ui->spinBoxThreshBasis->hide();
    //  this->ui->spinBoxAngleShift->hide();

    if(m_Mode)
    {
        this->ui->tableViewH->setEnabled(false);
        this->ui->tableViewV->setEnabled(false);
    }

    updateCheckRes();

}

void DlgBrokenLineInspect::updateCheckRes()
{
    //this->m_Mode=0;
    if(this->m_Mode==0)//设置模式
    {
        int x1=0,y1=0,x2=0,y2=0;


        double resLineLen=0;
        int score=0;

        CheckOperatorBrokenlineInspectControl control(this->m_CheckOperator);

#ifdef IMG_TYPE_GRAY
        int res= control.debugCalculator(this->m_ItemImage->getImage().ImageGray,
                                         this->m_LineItem->X1(),this->m_LineItem->Y1(),this->m_LineItem->X2(),this->m_LineItem->Y2(),
                                         this->m_LineItem2->X1(),this->m_LineItem2->Y1(),this->m_LineItem2->X2(),this->m_LineItem2->Y2(),
                                         ui->spinBoxBinThreshold->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value()/2,
                                         this->m_CheckOperator->AngleMin,this->m_CheckOperator->AngleMax,this->ui->spinBoxSetLength->value(),false,
                                         &x1,&y1,&x2,&y2,resLineLen,score);

#endif
#ifdef IMG_TYPE_RGB
        cv::Mat imgGray;
        cv::cvtColor(this->m_ItemImage->getImage().ImageRGB,imgGray,cv::COLOR_BGR2GRAY);
        int res= control.debugCalculator(imgGray,
                                         this->m_LineItem->X1(),this->m_LineItem->Y1(),this->m_LineItem->X2(),this->m_LineItem->Y2(),
                                         this->m_LineItem2->X1(),this->m_LineItem2->Y1(),this->m_LineItem2->X2(),this->m_LineItem2->Y2(),
                                         ui->spinBoxBinThreshold->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value()/2,
                                         this->m_CheckOperator->AngleMin,this->m_CheckOperator->AngleMax,this->ui->spinBoxSetLength->value(),false,
                                         &x1,&y1,&x2,&y2,resLineLen,score);

#endif
        //m_Res=res==0?0:-1;
        this->m_LineRes->setLine(x1,y1,x2,y2);

        this->ui->lineEditCurLength->setText(QString::number(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),'f', 1));



    }
    else
    {

        int x1=0,y1=0,x2=0,y2=0;


        double resLineLen=0;
        int score=0;

        CheckOperatorBrokenlineInspectControl control(this->m_CheckOperator);
#ifdef IMG_TYPE_GRAY
        int res= control.debugCalculator(this->m_ItemImage->getImage().ImageGray,
                                         this->m_LineItem->X1(),this->m_LineItem->Y1(),this->m_LineItem->X2(),this->m_LineItem->Y2(),
                                         this->m_LineItem2->X1(),this->m_LineItem2->Y1(),this->m_LineItem2->X2(),this->m_LineItem2->Y2(),
                                         ui->spinBoxBinThreshold->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value()/2,
                                         this->m_CheckOperator->AngleMin,this->m_CheckOperator->AngleMax,this->ui->spinBoxSetLength->value(),true,
                                         &x1,&y1,&x2,&y2,resLineLen,score);
#endif
#ifdef IMG_TYPE_RGB
        cv::Mat imgGray;
        cv::cvtColor(this->m_ItemImage->getImage().ImageRGB,imgGray,cv::COLOR_BGR2GRAY);
        int res= control.debugCalculator(imgGray,
                                         this->m_LineItem->X1(),this->m_LineItem->Y1(),this->m_LineItem->X2(),this->m_LineItem->Y2(),
                                         this->m_LineItem2->X1(),this->m_LineItem2->Y1(),this->m_LineItem2->X2(),this->m_LineItem2->Y2(),
                                         ui->spinBoxBinThreshold->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value(),this->ui->spinBoxLengthThresh->value()/2,
                                         this->m_CheckOperator->AngleMin,this->m_CheckOperator->AngleMax,this->ui->spinBoxSetLength->value(),true,
                                         &x1,&y1,&x2,&y2,resLineLen,score);
#endif
        m_Res=res==0?0:-1;
        this->m_LineRes->setLine(x1,y1,x2,y2);
        ui->lineEditCurLength->setText(QString::number(resLineLen));
        ui->lineEditCalcScore->setText(QString::number(score));
        this->ui->lineEditCurLength->setText(QString::number(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),'f',1));

        double stdAngle=atan(double(m_CheckOperator->StdLine_ye-m_CheckOperator->StdLine_ys)/double(m_CheckOperator->StdLine_xe-m_CheckOperator->StdLine_xs))*180/3.1415926;
        //double stdLineLen=sqrt((m_CheckOperator->StdLine_xe-m_CheckOperator->StdLine_xs)*(m_CheckOperator->StdLine_xe-m_CheckOperator->StdLine_xs)+(m_CheckOperator->StdLine_ys-m_CheckOperator->StdLine_ye)*(m_CheckOperator->StdLine_ys-m_CheckOperator->StdLine_ye));

        double curAngle=atan(double(y1-y2)/double(x1-x2))*180/3.1415926;
        //double curLineLen=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

        this->ui->lineEditCurAngle->setText(QString::number(abs(curAngle-stdAngle),'f', 1));
        this->ui->lineEditCalcScore->setText(QString::number(score));


        if((score<this->ui->spinBoxSetScore->value()||abs(stdAngle-curAngle)>(double)ui->spinBoxAngleShift->value()))
        {

            this->ui->labelResult->setText("NG");
            this->ui->labelResult->setStyleSheet("background-color: rgb(255, 0, 0);font: 12pt \"微软雅黑\";");
        }
        else
        {
            this->ui->labelResult->setText("OK");
            this->ui->labelResult->setStyleSheet("background-color: rgb(0, 255, 0);font: 12pt \"微软雅黑\";");
        }


    }






}

QString DlgBrokenLineInspect::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
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

void DlgBrokenLineInspect::loadHVPos()
{
    this->m_ModelHPos->clear();
    this->m_ModelVPos->clear();
    QStringList listmodelMain;
    listmodelMain<<" "<<"类型"<<"顺序";
    this->m_ModelHPos->setHorizontalHeaderLabels(listmodelMain);
    this->m_ModelVPos->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewH->setModel(this->m_ModelHPos);
    ui->tableViewH->setColumnWidth(0,50);
    ui->tableViewH->setColumnWidth(1,110);
    ui->tableViewH->setColumnWidth(2,77);
    ui->tableViewH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewH->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableViewH->verticalHeader()->hide();
    ui->tableViewV->setModel(this->m_ModelVPos);
    ui->tableViewV->setColumnWidth(0,50);
    ui->tableViewV->setColumnWidth(1,110);
    ui->tableViewV->setColumnWidth(2,77);
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
                if(exist)
                {
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


void DlgBrokenLineInspect::on_pushBtnSaveReturn_clicked()
{
    updateCheckRes();


    saveParam();
    this->close();
}


void DlgBrokenLineInspect::saveParam()
{
    this->m_CheckOperator->Score=this->ui->spinBoxSetScore->value();
    this->m_CheckOperator->LineLength=this->ui->spinBoxSetLength->value();
    this->m_CheckOperator->BinThreshold=this->ui->spinBoxBinThreshold->value();
    this->m_CheckOperator->Line1Seg_xs=m_LineItem->X1();
    this->m_CheckOperator->Line1Seg_ys=m_LineItem->Y1();
    this->m_CheckOperator->Line1Seg_xe=m_LineItem->X2();
    this->m_CheckOperator->Line1Seg_ye=m_LineItem->Y2();
    this->m_CheckOperator->Line2Seg_xs=m_LineItem2->X1();
    this->m_CheckOperator->Line2Seg_ys=m_LineItem2->Y1();
    this->m_CheckOperator->Line2Seg_xe=m_LineItem2->X2();
    this->m_CheckOperator->Line2Seg_ye=m_LineItem2->Y2();
    this->m_CheckOperator->StuStuLineLenLineLen->LineLength= this->ui->spinBoxLengthThresh->value();
    this->m_CheckOperator->StuStuHoughLine->ThreshShift=this->ui->spinBoxLengthThresh->value();
    //    double tanMin=.0,tanMax=.0;
    //    double tan_p1 = 0, tan_p2 = 0;
    //    if (m_LineItem->X1() == m_LineItem->X2())tan_p1 = 1000;
    //    else tan_p1 = (double)(m_LineItem->Y2() - m_LineItem->Y1()) / (m_LineItem->X2() - m_LineItem->X1());
    //    if (m_LineItem2->X1() == m_LineItem2->X2()) tan_p2 = 1000;
    //    else tan_p2 = (double)(m_LineItem2->Y2() - m_LineItem2->Y1()) / (m_LineItem2->X2() - m_LineItem2->X1());

    //    if (tan_p1 >= tan_p2)
    //    {
    //        tanMin = tan_p2;
    //        tanMax = tan_p1;
    //    }
    //    else
    //    {
    //        tanMin = tan_p1;
    //        tanMax = tan_p2;
    //    }

    //    this->m_CheckOperator->AngleMax=tanMax;
    //    this->m_CheckOperator->AngleMin=tanMin;
    //    this->m_CheckOperator->AngleShift=this->ui->spinBoxAngleShift->value();


    if(this->m_Mode==0)//设置模式
    {


        this->m_CheckOperator->StdLine_xs=this->m_LineRes->line().x1();
        this->m_CheckOperator->StdLine_xe=this->m_LineRes->line().x2();
        this->m_CheckOperator->StdLine_ys=this->m_LineRes->line().y1();
        this->m_CheckOperator->StdLine_ye=this->m_LineRes->line().y2();


        double curAngle=atan(double(this->m_LineRes->line().y1()-this->m_LineRes->line().y2())/double(this->m_LineRes->line().x1()-this->m_LineRes->line().x2()))*180/3.1415926;
        this->m_CheckOperator->AngleMax=curAngle+this->ui->spinBoxAngleShift->value();
        this->m_CheckOperator->AngleMin=curAngle-this->ui->spinBoxAngleShift->value();
        this->m_CheckOperator->AngleShift=this->ui->spinBoxAngleShift->value();
        this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
        this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;

    }
    else
    {
        double curAngle=atan(double(this->m_CheckOperator->StdLine_ys-this->m_CheckOperator->StdLine_ye)/double(this->m_CheckOperator->StdLine_xs-this->m_CheckOperator->StdLine_xe))*180/3.1415926;
        this->m_CheckOperator->AngleMax=curAngle+this->ui->spinBoxAngleShift->value();
        this->m_CheckOperator->AngleMin=curAngle-this->ui->spinBoxAngleShift->value();
        this->m_CheckOperator->AngleShift=this->ui->spinBoxAngleShift->value();
    }

}


void DlgBrokenLineInspect::on_tableViewH_doubleClicked(const QModelIndex &index)
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


void DlgBrokenLineInspect::on_tableViewV_doubleClicked(const QModelIndex &index)
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

void DlgBrokenLineInspect::on_pushBtnAutoCalc_clicked()
{
    if(this->m_Mode==0)
    {
        this->ui->spinBoxSetScore->setValue(50);
        this->ui->spinBoxAngleShift->setValue(12);

        int tmp=(int)this->ui->lineEditCurLength->text().toDouble();
        DSDEBUG<<QString::number(tmp);
        this->ui->spinBoxSetLength->setValue(tmp);

    }
    else
    {
        //        this->ui->spinBoxSetScore->setValue(this->ui->lineEditCalcScore->text().toInt()-5);
        //        this->ui->spinBoxAngleShift->setValue(this->ui->lineEditCurAngle->text().toInt()+2);
        //this->ui->spinBoxSetLength->setValue(this->ui->lineEditCurLength->text().toInt());

    }
    updateCheckRes();
}

void DlgBrokenLineInspect::on_pushBtnCancel_clicked()
{
    this->close();
}

void DlgBrokenLineInspect::on_spinBoxSetScore_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgBrokenLineInspect::on_spinBoxAngleShift_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgBrokenLineInspect::on_spinBoxLengthThresh_valueChanged(int arg1)
{
    updateCheckRes();
}

void DlgBrokenLineInspect::on_spinBoxBinThreshold_valueChanged(int arg1)
{
    updateCheckRes();
}
