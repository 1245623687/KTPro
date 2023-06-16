#include "dlgmatchtemplate.h"
#include "ui_dlgmatchtemplate.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"
#include"ImageHandle/opencvimage.h"
#include"kcontrols/dlgkeyboard.h"
DlgMatchTemplate::DlgMatchTemplate(int cameraNo,int checkOperatorNo,CheckOperatorTemplateMatch* pCheckOperator,ImgTobacco* Tabacco,OpencvImage & image,int mode,QWidget *parent):
    QDialog(parent),
    ui(new Ui::DlgMatchTemplate),
    m_CheckOperator(pCheckOperator),
    m_Tabacco(Tabacco),
    m_CameraNo(cameraNo),
    m_Mode(mode),
    m_CheckOperatorNo(checkOperatorNo)
{
    ui->setupUi(this);
    this->setWindowTitle("对象搜索");
    m_Res=-1;
    m_TemplateNum=0;
    m_SceneSearch=nullptr;
    m_SceneTemplateArr[0]=nullptr;
    m_SceneTemplateArr[1]=nullptr;
    m_SceneTemplateArr[2]=nullptr;
    m_SceneTemplateArr[3]=nullptr;
    m_ItemImage=nullptr;

    InitStyle();
    ui->lab_Title->setText(QString("对象搜索--R%1").arg(pCheckOperator->stu_CheckOperatorBasic->Index));
    m_FullImage=image;

    OpencvImage thisImage;
    thisImage.Format=ENUMDSIMAGEFORMAT_GRAY;
    cv::cvtColor(image.ImageRGB,image.ImageGray,cv::COLOR_BGR2GRAY);
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

    this->m_SceneSearch=new GraphicsSceneCheckOperatorMain;

    connect(m_SceneSearch, &GraphicsSceneCheckOperatorMain::sceneMouseRelease, this, &DlgMatchTemplate::updateCheckoperatorOption);

    this->m_SceneSearch->setBackgroundBrush(QBrush(QColor(100,100,100)));
    this->m_SceneSearch->addItem(this->m_ItemImage);
    //this->m_Scene->setBackGroundImage(this->m_ItemImage);
    this->ui->graphicsView->setScene(m_SceneSearch);

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(100,100,100)));

    for(int sceneNum=0;sceneNum<4;sceneNum++)
    {
        this->m_SceneTemplateArr[sceneNum]=new GraphicsSceneCheckOperatorMain;
        this->m_SceneTemplateArr[sceneNum]->setBackgroundBrush(QBrush(QColor(100,100,100)));
    }
    this->ui->graphicsViewTarget1->setScene(this->m_SceneTemplateArr[0]);
    this->ui->graphicsViewTarget2->setScene(this->m_SceneTemplateArr[1]);
    this->ui->graphicsViewTarget3->setScene(this->m_SceneTemplateArr[2]);
    this->ui->graphicsViewTarget4->setScene(this->m_SceneTemplateArr[3]);
    ui->graphicsViewTarget1->setBackgroundBrush(QBrush(QColor(100,100,100)));
    ui->graphicsViewTarget2->setBackgroundBrush(QBrush(QColor(100,100,100)));
    ui->graphicsViewTarget3->setBackgroundBrush(QBrush(QColor(100,100,100)));
    ui->graphicsViewTarget4->setBackgroundBrush(QBrush(QColor(100,100,100)));


    initControl();
    loadTemplate();
    updateCheckRes();
}

void DlgMatchTemplate::showKeyBoard(KControlsBase* pLineEdit)
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

void DlgMatchTemplate::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

}


bool DlgMatchTemplate::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgMatchTemplate::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgMatchTemplate::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgMatchTemplate::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgMatchTemplate::on_btnMenu_Close_clicked()
{
    this->close();
}

void DlgMatchTemplate::updateCheckoperatorOption()
{
    //DSDEBUG<<"RELE:"<<this->m_Scene->m_mapRectItem[1]->scenePos();
    if(this->m_SceneSearch->m_mapRectItem[1]->scenePos().x()<0)
    {
        this->m_SceneSearch->m_mapRectItem[1]->setPos(0,this->m_SceneSearch->m_mapRectItem[1]->scenePos().y());
    }
    if(this->m_SceneSearch->m_mapRectItem[1]->scenePos().y()<0)
    {
        this->m_SceneSearch->m_mapRectItem[1]->setPos(this->m_SceneSearch->m_mapRectItem[1]->scenePos().x(),0);
    }
    if(this->m_SceneSearch->m_mapRectItem[1]->scenePos().x()+this->m_SceneSearch->m_mapRectItem[1]->rect().width()>m_ItemImage->getWidth())
    {

        this->m_SceneSearch->m_mapRectItem[1]->setPos(m_ItemImage->getWidth()-this->m_SceneSearch->m_mapRectItem[1]->rect().width(),this->m_SceneSearch->m_mapRectItem[1]->scenePos().y());
    }
    if(this->m_SceneSearch->m_mapRectItem[1]->scenePos().y()+this->m_SceneSearch->m_mapRectItem[1]->rect().height()>m_ItemImage->getHeight())
    {

        this->m_SceneSearch->m_mapRectItem[1]->setPos(this->m_SceneSearch->m_mapRectItem[1]->scenePos().x(),m_ItemImage->getHeight()-this->m_SceneSearch->m_mapRectItem[1]->rect().height());
    }

    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xs=m_SceneSearch->m_mapRectItem[1]->scenePos().x();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ys=m_SceneSearch->m_mapRectItem[1]->scenePos().y();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xe=m_SceneSearch->m_mapRectItem[1]->scenePos().x()+m_SceneSearch->m_mapRectItem[1]->rect().width();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ye=m_SceneSearch->m_mapRectItem[1]->scenePos().y()+m_SceneSearch->m_mapRectItem[1]->rect().height();

}


void DlgMatchTemplate::loadTemplate()
{

    this->m_TemplateNum=this->m_CheckOperator->SearchNum;
    ImgTobaccoControl control(m_Tabacco);
    for(int idx=0;idx<this->m_TemplateNum;idx++)
    {
        //        QString imgPath=QString(PackageCheckerSystemParam::DirPath+"/"+PackageCheckerSystemParam::brandParam->BrandDirPath+"/"+
        //                                PackageCheckerSystemParam::brandParam->BrandName+"/ImgPro%1/"+"CheckOperator%2/"+"template%3.bmp").arg(m_CameraNo).arg(this->m_CheckOperator->stu_CheckOperatorBasic->Index).arg(idx);

        //        this->m_SceneArr[idx]->clear();
        //        OpenCvImage image(this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ye-this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ys,
        //                          this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xe-this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xs,
        //                          ENUMDSIMAGEFORMAT_GRAY
        //                          );
        //        image.loadImage(imgPath);
        //        DSGraphicsItemImage<OpenCvImage>* ItemImg=new DSGraphicsItemImage<OpenCvImage>(image,image.Width,image.Height);
        //        this->m_SceneArr[idx]->addItem(ItemImg);

        DSGraphicsItemImage<OpencvImage>* ItemImg=new DSGraphicsItemImage<OpencvImage>(
                    control.getImgPro(this->m_CameraNo)->TemplateImage[QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code)][idx],
                    control.getImgPro(this->m_CameraNo)->TemplateImage[QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code)][idx].Width,
                    control.getImgPro(this->m_CameraNo)->TemplateImage[QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code)][idx].Height
                );
        this->m_SceneTemplateArr[idx]->addItem(ItemImg);
    }



    if(this->m_TemplateNum==0)
    {
        this->m_SceneSearch->createRect(1,QRect(15,
                                              15,
                                              m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs-30,
                                              m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys-30),
                                        GraphicsCheckOperatorItem::ENUMRECTTYPE_RECTBLANCK);

        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
    else
    {
        this->m_SceneSearch->createRect(1,QRect(m_CheckOperator->StuStuGrayMatch->PatternRect_Xs,
                                              m_CheckOperator->StuStuGrayMatch->PatternRect_Ys,
                                              m_CheckOperator->StuStuGrayMatch->PatternRect_Xe-m_CheckOperator->StuStuGrayMatch->PatternRect_Xs,
                                              m_CheckOperator->StuStuGrayMatch->PatternRect_Ye-m_CheckOperator->StuStuGrayMatch->PatternRect_Ys
                                              ),GraphicsCheckOperatorItem::ENUMRECTTYPE_RECTBLANCK);

        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }
    this->m_SceneSearch->m_mapRectItem[1]->setMaxHeight(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye-this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys);
    this->m_SceneSearch->m_mapRectItem[1]->setMaxWidth(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe-this->m_CheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs);
}


void DlgMatchTemplate::updateCheckRes()
{

    CheckOperatorTemplateMatchControl checkOperatorTemplateMatchControl(this->m_CheckOperator);
    int best_x=0,best_y=0,best_score=0;

    for(int idx=0;idx<m_TemplateNum;idx++)
    {
        //QString imgPath=QString(PackageCheckerSystemParam::DirPath+"/"+PackageCheckerSystemParam::brandParam->BrandDirPath+"/"+
        //                        PackageCheckerSystemParam::brandParam->BrandName+"/ImgPro%1/"+"CheckOperator%2/"+"template%3.bmp").arg(m_CameraNo).arg(this->m_CheckOperator->stu_CheckOperatorBasic->Index).arg(idx);
        //QTextCodec *code = QTextCodec::codecForName("GB2312");
        //std::string name = code->fromUnicode(imgPath).data();
        //cv::Mat target=cv::imread( name ,0);

       // cv::Mat target= this->m_Tabacco->m_Template[this->m_CameraNo][this->m_CheckOperatorNo][idx].ImageGray;
        cv::Mat target= ImgTobaccoControl(this->m_Tabacco).getImgPro(m_CameraNo)->TemplateImage[QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(m_CheckOperator)->Code)][idx].ImageGray;

        int score=0;
        int x_pos=0,y_pos=0;
        if(-1==checkOperatorTemplateMatchControl.debugCalculator(this->m_ItemImage->getImage().ImageGray,this->m_ItemImage->getImage().ImageGray,target,ui->spinBoxSetScore->value(),&score,&x_pos,&y_pos))
        {
            DSDEBUG<<"调试搜索得分:"<<score<<endl;
            this->ui->lineEditCalcScore->setText("0");
            this->ui->labelRes->setText("NG");
        }
        else
        {
            //this->ui->lineEditCalcScore->setText(QString::number(score));
            //if(score>=this->ui->spinBoxSetScore->value()) this->ui->labelRes->setText("OK");
            //else this->ui->labelRes->setText("NG");
             DSDEBUG<<"调试搜索得分:"<<score<<endl;
            if(score>best_score)
            {
                best_score=score;
                best_x=x_pos;
                best_y=y_pos;
            }

        }
    }

    this->ui->lineEditCalcScore->setText(QString::number(best_score));
    if(best_score>=this->ui->spinBoxSetScore->value())
    {
        m_Res=0;
        this->ui->labelRes->setText("OK");
        this->ui->labelRes->setStyleSheet("background-color: rgb(0, 255, 0);");
    }
    else
    {
        m_Res=-1;
        this->ui->labelRes->setStyleSheet("background-color: rgb(255, 0, 0);");
        this->ui->labelRes->setText("NG");
    }
    this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->m_SceneSearch->m_mapRectItem[1]->setPos(best_x,best_y);
    DSDEBUG<<QString("bestxy(%1,%2)").arg(best_x).arg(best_y);
    this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, false);

}

void DlgMatchTemplate::initControl()
{
    connect(ui->spinBoxSetScore,SIGNAL(clicked(KControlsBase* )),this,SLOT(showKeyBoard(KControlsBase* )));

    ui->lineEditCalcScore->setEnabled(false);
    ui->spinBoxR->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->RedRate*1000));
    ui->spinBoxG->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->GreenRate*1000));
    ui->spinBoxB->setValue((int)(this->m_CheckOperator->stu_CheckOperatorBasic->Basic_GrayConversion->BlueRate*1000));
    ui->spinBoxR->setEnabled(false);
    ui->spinBoxG->setEnabled(false);
    ui->spinBoxB->setEnabled(false);

    ui->spinBoxSetScore->setValue((int)(this->m_CheckOperator->MatchThresh));
}


DlgMatchTemplate::~DlgMatchTemplate()
{

    if(m_SceneSearch!=nullptr)
    {
        m_SceneSearch->clear();
        delete m_SceneSearch;
        m_SceneSearch=nullptr;
    }
    if(m_SceneTemplateArr[0]!=nullptr)
    {
        m_SceneTemplateArr[0]->clear();
        delete m_SceneTemplateArr[0];
        m_SceneTemplateArr[0]=nullptr;
    }
    if(m_SceneTemplateArr[1]!=nullptr)
    {
        m_SceneTemplateArr[1]->clear();
        delete m_SceneTemplateArr[1];
        m_SceneTemplateArr[1]=nullptr;
    }
    if( m_SceneTemplateArr[2]!=nullptr)
    {
         m_SceneTemplateArr[2]->clear();
        delete  m_SceneTemplateArr[2];
         m_SceneTemplateArr[2]=nullptr;
    }
    if(m_SceneTemplateArr[3]!=nullptr)
    {
        m_SceneTemplateArr[3]->clear();
        delete m_SceneTemplateArr[3];
        m_SceneTemplateArr[3]=nullptr;
    }


    delete ui;
}


void DlgMatchTemplate::on_pushButtonAdd_clicked()
{
    if(m_TemplateNum==4)
    {

        return;
    }
    //使能矩形框:FALSE
    {
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    QMap<int,GraphicsCheckOperatorItem*>::iterator it=this->m_SceneSearch->m_mapRectItem.begin();
    QRect rect(m_SceneSearch->m_mapRectItem[1]->scenePos().x(),
            m_SceneSearch->m_mapRectItem[1]->scenePos().y(),
            it.value()->rect().width(),
            it.value()->rect().height()
            );

    OpencvImage  addImg=  this->m_ItemImage->getImage().getRectImage(rect);



    QString savePath=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+
                             DSSystemParam::BrandName+"/ImgPro%1/"+"%2/"+"template%3.bmp").arg(m_CameraNo).arg(QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code)).arg(this->m_TemplateNum);
    QString savePathAll=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+
                                DSSystemParam::BrandName+"/ImgPro%1/"+"%2/"+"template%3_All.bmp").arg(m_CameraNo).arg(QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code)).arg(this->m_TemplateNum);

    FileHelper::makeDir(savePath);
    addImg.save(savePath);
    m_FullImage.save(savePathAll);

    ImgTobaccoControl control(this->m_Tabacco);
    control.addTemplate(this->m_CameraNo,QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code),this->m_CheckOperator->SearchNum,addImg);

    DSGraphicsItemImage<OpencvImage>* addItemImg=new DSGraphicsItemImage<OpencvImage>(addImg,addImg.Width,addImg.Height);
    this->m_SceneTemplateArr[this->m_TemplateNum++]->addItem(addItemImg);
    this->m_CheckOperator->SearchNum++;



    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xs=m_SceneSearch->m_mapRectItem[1]->scenePos().x();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ys=m_SceneSearch->m_mapRectItem[1]->scenePos().y();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Xe=m_SceneSearch->m_mapRectItem[1]->scenePos().x()+m_SceneSearch->m_mapRectItem[1]->rect().width();
    this->m_CheckOperator->StuStuGrayMatch->PatternRect_Ye=m_SceneSearch->m_mapRectItem[1]->scenePos().y()+m_SceneSearch->m_mapRectItem[1]->rect().height();


}


void DlgMatchTemplate::on_pushButtonDel_clicked()
{
    if(m_TemplateNum==0) return;
    this->m_SceneTemplateArr[m_TemplateNum-1]->clear();

    ImgTobaccoControl control(this->m_Tabacco);
    control.delTemplate(this->m_CameraNo,QString::fromStdString(static_cast<CheckOperatorTemplateMatch*>(this->m_CheckOperator)->Code),this->m_CheckOperator->SearchNum-1);

    m_TemplateNum--;
    this->m_CheckOperator->SearchNum--;

    if(m_TemplateNum==0)
    {
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->m_SceneSearch->m_mapRectItem[1]->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        this->m_SceneSearch->selectRectItem(1);
    }

    QString delPath=QString(DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+
                            DSSystemParam::BrandName+"/ImgPro%1/"+"CheckOperator%2/"+"template%3.bmp").arg(m_CameraNo).arg(this->m_CheckOperator->stu_CheckOperatorBasic->Index).arg(m_TemplateNum);


    QFile fileTemp(delPath);
    fileTemp.remove();
    if(this->m_TemplateNum==0)
    {
        QString delDir=QString(DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+
                               DSSystemParam::BrandName+"/ImgPro%1/").arg(m_CameraNo);
        QDir dir;
        dir.setPath(delDir);
        dir.removeRecursively();
    }
}

void DlgMatchTemplate::on_pushBtnCancel_clicked()
{
     this->close();
}

void DlgMatchTemplate::saveParam()
{

    this->m_CheckOperator->MatchThresh=this->ui->spinBoxSetScore->value();
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_Vaild=true;
    this->m_CheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=true;
}

void DlgMatchTemplate::on_pushBtnAutoCalc_clicked()
{
      updateCheckRes();
}

void DlgMatchTemplate::on_pushBtnSaveReturn_clicked()
{
    saveParam();
    this->close();
}
