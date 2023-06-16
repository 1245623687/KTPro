#include "dlgedgepositioning.h"
#include "ui_dlgedgepositioning.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"


#include"kcontrols/dlgkeyboard.h"

DlgEdgePositioning::DlgEdgePositioning(CheckOperatorEdgePositioning* pCheckOperator,ImgPro* imgPro,OpencvImage&  image,int mode=0,QWidget *parent) :
    m_CheckOperator(pCheckOperator),
    m_ImgPro(imgPro),
    m_Mode(mode),
    QDialog(parent),
    ui(new Ui::DlgEdgePositioning)
{
    ui->setupUi(this);
    this->setWindowTitle("边缘检查");
    InitStyle();
    ui->lab_Title->setText(QString("边缘检查R%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    OpencvImage thisImage;
    thisImage.Format=image.Format;
    switch (thisImage.Format)
    {
    case ENUMDSIMAGEFORMAT_GRAY:

        thisImage.Width=pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe
                -pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs;
        thisImage.Height=pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye
                -pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys;
        thisImage.ImageGray=cv::Mat(image.ImageGray,
                                    cv::Rect(pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                             pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                                             pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                             pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys
                                             ));
        break;
    case ENUMDSIMAGEFORMAT_RGB:
        break;
    case ENUMDSIMAGEFORMAT_R :
        break;
    case ENUMDSIMAGEFORMAT_G :
        break;
    case ENUMDSIMAGEFORMAT_B:
        break;
    }

    m_ItemImage=new DSGraphicsItemImage<OpencvImage>(thisImage,thisImage.Width,thisImage.Height);

    this->m_Scene=new GraphicsSceneCheckOperatorMain;
    this->m_Scene->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_Scene->addItem(this->m_ItemImage);
    this->ui->graphicsView->setScene(m_Scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    m_LineItem=new QGraphicsLineItem;
    QPen pen;
    pen.setColor(QColor(255,0,0));
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    m_LineItem->setPen(pen);
    this->m_Scene->addItem(this->m_LineItem);

    initControl();

    //添加结果线
    updateCheckRes();



}



void DlgEdgePositioning::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgEdgePositioning::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgEdgePositioning::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgEdgePositioning::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgEdgePositioning::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgEdgePositioning::on_btnMenu_Close_clicked()
{
    this->close();
}



void DlgEdgePositioning::showKeyBoard(KControlsBase* pLineEdit)
{

    switch (pLineEdit->type())
    {
    case ENUMKCONTROLTYPE_KSPINBOX:
    {
        KSpinBoxKb* pWidget=static_cast<KSpinBoxKb*>(pLineEdit);
        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
        //        dlg.setGeometry(qMin(1024-dlg.geometry().width(),qMax(0,pt.x()-pWidget->geometry().x())),
        //                        qMin(768-dlg.geometry().height(),qMax(0,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2)),
        //                        dlg.geometry().width(),dlg.geometry().height());
        dlg.setGeometry(pt.x()-pWidget->geometry().x(),pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2-150-95,dlg.geometry().width(),dlg.geometry().height());


        dlg.exec();
    }
        break;
    case ENUMKCONTROLTYPE_KLINEEDIT:
    {
        KLineEditKb* pWidget=static_cast<KLineEditKb*>(pLineEdit);
        QPoint pt=pWidget->mapToGlobal(QPoint(pWidget->geometry().x(),pWidget->geometry().y()));
        DlgKeyBoard dlg(pWidget);
//        dlg.setGeometry(qMin(1024-dlg.geometry().width(),qMax(0,pt.x()-pWidget->geometry().x())),
//                        qMin(768-dlg.geometry().height(),qMax(0,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2)),
//                        dlg.geometry().width(),dlg.geometry().height());

        dlg.setGeometry(pt.x()-pWidget->geometry().x()-40,pt.y()-pWidget->geometry().y()+pWidget->geometry().height()+2-150-95,dlg.geometry().width(),dlg.geometry().height());

        dlg.exec();
    }
        break;
    }

}


void DlgEdgePositioning::initControl()
{
    connect(ui->spinBoxThresh,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));



    ui->labelRecommentThresh->hide();

    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    ui->comboBoxDir->addItem(QString("左侧"));
    ui->comboBoxDir->addItem(QString("上侧"));
    ui->comboBoxDir->addItem(QString("右侧"));
    ui->comboBoxDir->addItem(QString("下侧"));

    ui->comboBoxDir->setCurrentIndex(this->m_CheckOperator->EdgePos);


    //    switch (this->m_CheckOperator->EdgePos)
    //    {

    //    case CheckOperatorEdgePositioning::ENUMEDGEPOS_LEFT:
    //        ui->comboBoxDir->setCurrentIndex(0);
    //        break;
    //    case CheckOperatorEdgePositioning::ENUMEDGEPOS_UP:
    //        ui->comboBoxDir->setCurrentIndex(1);
    //        break;
    //    case CheckOperatorEdgePositioning::ENUMEDGEPOS_RIGHT:
    //        ui->comboBoxDir->setCurrentIndex(2);
    //        break;
    //    case CheckOperatorEdgePositioning::ENUMEDGEPOS_DOWN:
    //        ui->comboBoxDir->setCurrentIndex(3);
    //        break;
    //    }

    ui->spinBoxThresh->setValue(this->m_CheckOperator->BinThresh);



    updateRecommentThresh();
}

DlgEdgePositioning::~DlgEdgePositioning()
{
    this->m_Scene->clear();
    delete this->m_Scene;
    delete ui;
}


void DlgEdgePositioning::saveParam()
{

    this->m_CheckOperator->BinThresh=this->ui->spinBoxThresh->value();
    this->m_CheckOperator->EdgePos=(CheckOperatorEdgePositioning::ENUMEDGEPOS)this->ui->comboBoxDir->currentIndex();
    switch (this->m_CheckOperator->EdgePos)
    {
    case CheckOperatorEdgePositioning::ENUMEDGEPOS_LEFT:
    case CheckOperatorEdgePositioning::ENUMEDGEPOS_RIGHT:
        //DSDEBUG<<this->m_LineItem->line().x1()<<endl;
        //this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Ref_X=this->m_LineItem->scenePos().x();
        this->m_CheckOperator->StuStuEdgeLine->PosX=this->m_LineItem->line().x1()+this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs;
        this->m_CheckOperator->StuStuEdgeLine->PosY=0;
        break;
    case CheckOperatorEdgePositioning::ENUMEDGEPOS_UP:
    case CheckOperatorEdgePositioning::ENUMEDGEPOS_DOWN:
        //this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Ref_Y=this->m_LineItem->line().dy();
        this->m_CheckOperator->StuStuEdgeLine->PosX=0;
        this->m_CheckOperator->StuStuEdgeLine->PosY=this->m_LineItem->line().y1()+this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys;
        //DSDEBUG<<this->m_LineItem->line().y1()<<endl;
        break;
    }

    //    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=m_Res==0?true:false;
    //    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=m_Res==0?true:false;

    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
}



void DlgEdgePositioning::on_pushBtnSaveReturn_clicked()
{
    //updateCheckRes();
    saveParam();
    this->close();
    //    if(m_Res==0)
    //    {
    //        this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    //        this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
    //        saveParam();
    //        this->close();
    //    }
    //    else
    //    {
    //        int res=QMessageBox::question(NULL,  "是否保存",  "\n当前参数设置错误,是否继续保存                                  ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    //        if(res==QMessageBox::Yes)
    //        {
    //            this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    //            this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
    //            saveParam();
    //        }
    //        else
    //        {
    //            return;
    //        }

    //        this->close();
    //    }
}

void DlgEdgePositioning::on_pushBtnCancel_clicked()
{
    this->close();
}

void DlgEdgePositioning::updateCheckRes()
{
    CheckOperatorEdgePositioningControl control(this->m_CheckOperator);
    int x_pos=0,y_pos=0;
    int score=0;
    control.debugCalculator(this->m_ItemImage->getImage().ImageGray,this->m_ItemImage->getImage().ImageGray,(int)ui->comboBoxDir->currentIndex(),this->ui->spinBoxThresh->value(),&x_pos,&y_pos,&score);

    DSDEBUG<<"SCORE:"<<score<<endl;
    //得分小于10，查找肯定是失败的
    if(score<20)
    {
        this->m_LineItem->setLine(QLine(0,0,0,0));
        m_Res=-1;
        this->ui->labelRes->setText("NG");
        this->ui->labelRes->setStyleSheet("background-color: rgb(255, 0, 0);");
        return ;
    }
    switch (this->ui->comboBoxDir->currentIndex())
    {
    case 0:
    case 2:
        //DSDEBUG<<"x_pos:"<<x_pos<<endl;
        this->m_LineItem->setLine(QLine(x_pos,0,x_pos,this->m_ItemImage->getHeight()-1));

        break;
    case 1:
    case 3:
        //DSDEBUG<<"y_pos:"<<y_pos<<endl;
        this->m_LineItem->setLine(QLine(0,y_pos,this->m_ItemImage->getWidth()-1,y_pos));
        break;
    }
    this->ui->labelRes->setText("OK");
    this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);");
    m_Res=0;
}


void DlgEdgePositioning::on_pushBtnAutoCalc_clicked()
{
    updateCheckRes();
}

void DlgEdgePositioning::updateRecommentThresh()
{
    int threshRecomment=0;
    CheckOperatorEdgePositioningControl control(this->m_CheckOperator);
    control.getRecommentThresh(this->m_ItemImage->getImage().ImageGray,static_cast<CheckOperatorEdgePositioning::ENUMEDGEPOS>(ui->comboBoxDir->currentIndex()),&threshRecomment);
    this->ui->labelRecommentThresh->setText(std::to_string(threshRecomment).c_str());

}
void DlgEdgePositioning::on_comboBoxDir_currentIndexChanged(int index)
{
    updateRecommentThresh();
    updateCheckRes();
}

void DlgEdgePositioning::on_spinBoxThresh_valueChanged(int arg1)
{
    updateCheckRes();
}
