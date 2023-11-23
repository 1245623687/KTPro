#include "dlgcheckoperator.h"
#include "ui_dlgcheckoperator.h"
#include"qtCommon/iconhelper.h"
#include<QMouseEvent>
#include<QDesktopWidget>
#include"qtCommon/frmmessagebox.h"

#include"CheckOperator/imgtobaccocontrol.h"
#include"CheckOperator/imgprocontrol.h"
#include"qtCommon/frmmessagebox.h"
#include"PackageChecker.h"
#include"dlgCheckOperator/dlgassociatedinspect.h"
#include"dlgCheckOperator/dlgbrokenlineinspect.h"
#include"dlgCheckOperator/dlgdefectinspect.h"
#include"dlgCheckOperator/dlgdefectinspect2.h"
#include"dlgCheckOperator/dlgedgepositioning.h"
#include"dlgCheckOperator/dlggraymeasurement.h"
#include"dlgCheckOperator/dlgmatchtemplate.h"
#include"dlgCheckOperator/dlgstayinspect.h"
#include"dlgCheckOperator/dlgdoublestayinspect.h"
#include"dlgCheckOperator/dlgcircleinspect.h"
#include"dlgCheckOperator/dlgDLobjectdetect.h"
#include"dlgCheckOperator/dlgassociatedinspect2.h"
#include<QScrollBar>
#include"glog/logging.h"

#define DLGCHECKOPERATOR_IMGHEI 485
#define DLGCHECKOPERATOR_IMGWID 596


unsigned char DlgCheckOperator::AnalysisCalcResult[256]={0};
DlgCheckOperator::DlgCheckOperator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCheckOperator)
{
    ui->setupUi(this);
    this->setWindowTitle("算子配置");
    m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/NG/"+QDate::currentDate().toString("yyyy-MM-dd");


    m_ThreadGrab=new KThreadGrab();
    connect(m_ThreadGrab,&KThreadGrab::inforUpdate,this,&DlgCheckOperator::loadSceneRectAll);


    InitStyle();
    InitControl();
    loadCheckOperatorSetModel(1);
    InitScene();


    loadAnaImageInfo(0);
    updateImgLstModel();

    if(m_VImageInfo.size()>0)
    {
        loadAnalysisLstModel(0);
        ui->radioBtnCur->click();
        if(m_ModelImgLst->rowCount()>0)
        {
            //QModelIndex modelIdxTmp=m_ModelCheckOperatorAna->index(0,0);
            on_tableViewImgLst_clicked(ui->tableViewImgLst->model()->index(0,0));
            ui->tableViewImgLst->selectRow(0);
            m_AnalysisImgInfoIdx=0;
        }
        m_Mode=ENUMDLGCHECKOPERATORMODE_ANA;
    }
    else
    {
        this->ui->radioBtnRef->click();
        this->btnGroupCamera->button(1)->click();

        on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(0,0));
        ui->tableViewCheckOperatorSet->selectRow(0);
        m_Mode=ENUMDLGCHECKOPERATORMODE_SET;
    }
    location = this->geometry();

}


void DlgCheckOperator::loadAnaImageInfo(int channel)
{
    m_VImageInfo.clear();
    QVector<QString> Imgfiles;
    FileHelper::getAllImgFilesInFolder(m_DirAnalysis,Imgfiles);

    DSDEBUG<<m_DirAnalysis;
    //    QFileInfoList fileInfoList = dir.entryInfoList(strLst,QDir::Files);

    for (int idx=0;idx<Imgfiles.size();idx++)
    {
        QFileInfo fInfo=Imgfiles[idx];
        if(fInfo.suffix()!="bmp"&&fInfo.suffix()!="jpg") continue;
        QString fileBaseName=fInfo.baseName();
        int len=fileBaseName.length();
        if(fileBaseName.length()!=25)continue;

        ImageInfo imageInfo(fileBaseName);

        switch (channel)
        {
        case 0:
        {
            m_VImageInfo.push_back(imageInfo);
        }
            break;
        default:
        {
            if(imageInfo.m_cameraNo==channel)  m_VImageInfo.push_back(imageInfo);
        }
            break;
        }
    }

    qSort(m_VImageInfo.begin(),m_VImageInfo.end(),ImageInfo::compare);


}


void DlgCheckOperator::updateImgLstModel()
{

    InitImgLstModel();

    for (int idx=0;idx<m_VImageInfo.size();idx++)
    {
        QStandardItem * itemNo=new QStandardItem(QString::number(idx));
        itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QStandardItem* itemDate=new QStandardItem(QString(m_VImageInfo[idx].m_datetime.time().toString("hh:mm:ss.zzz")));
        itemDate->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QStandardItem * itemCameraNo=new QStandardItem(QString::number(m_VImageInfo[idx].m_cameraNo));
        itemCameraNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_ModelImgLst->setItem(idx,0,itemNo);
        m_ModelImgLst->setItem(idx,1,itemDate);
        m_ModelImgLst->setItem(idx,2,itemCameraNo);
    }
}

void DlgCheckOperator::InitControl()
{
    //ui->pushButtonCO_CircleInspect->setVisible(false);
    ui->PushBtnCO_StayInspect->setVisible(false);
    ui->PushBtnCO_EdgePositioning->setVisible(false);
    ui->PushBtnCO_AssociatedInspect->setVisible(true);
    ui->PushBtnCO_BrokenLineInspect->setVisible(false);
    ui->PushBtnCO_DoubleStayInspect->setVisible(false);
    ui->PushBtnCO_Template->setEnabled(false);



    ui->checkBox->setEnabled(false);
    ui->pushButtonAnaDel->setVisible(false);
    //
    ui->dateEditChose->setDate(QDate::currentDate());

    ui->PushBtnCO_GrayMeasurement->setEnabled(false);
    ui->PushBtnCO_AssociatedInspect->setEnabled(false);

//    ui->pushButtonCO_CircleInspect->setEnabled(false);
//    ui->PushBtnCO_DLObjectDetect->setEnabled(false);


    //模式切换
    QButtonGroup * btnGroupMode=new QButtonGroup(this);
    btnGroupMode->addButton(ui->radioBtnRef,ENUMDLGCHECKOPERATORMODE_SET);
    btnGroupMode->addButton(ui->radioBtnCur,ENUMDLGCHECKOPERATORMODE_ANA);
    connect(btnGroupMode,SIGNAL(buttonClicked(int)),this,SLOT(buttonModeJudge(int)));


    //相机
    btnGroupCamera=new QButtonGroup(this);
    btnGroupCamera->addButton(ui->radioButton_1,1);
    btnGroupCamera->addButton(ui->radioButton_2,2);
    btnGroupCamera->addButton(ui->radioButton_3,3);
    btnGroupCamera->addButton(ui->radioButton_4,4);
    btnGroupCamera->addButton(ui->radioButton_5,5);
    btnGroupCamera->addButton(ui->radioButton_6,6);
    btnGroupCamera->addButton(ui->radioButton_7,7);
    btnGroupCamera->addButton(ui->radioButton_8,8);
    connect(btnGroupCamera,SIGNAL(buttonClicked(int)),this,SLOT(buttonCameraJudge(int)));


    int cameraNumTmp=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.size();
    for (int i=1;i<=8;++i)
    {
        if(i<=cameraNumTmp)
            btnGroupCamera->button(i)->setVisible(true);
        else
            btnGroupCamera->button(i)->setVisible(false);
    }

    for (int i=1;i<=cameraNumTmp;++i)
    {
        ui->comboBoxCamera->addItem(QString(tr("相机%1")).arg(i),i);
    }


    //显示模式
    QButtonGroup * btnGroupShowMode=new QButtonGroup(this);
    btnGroupShowMode->addButton(ui->radioButtonShowAll,0);
    btnGroupShowMode->addButton(ui->radioButtonShowCur,1);
    connect(btnGroupShowMode,SIGNAL(buttonClicked(int)),this,SLOT(buttonShowModeJudge(int)));
    ui->radioButtonShowAll->setChecked(true);

    //切换相机使能
    connect(ui->sliderButton,SIGNAL(enable(bool)),this,SLOT(changeCameraEnable(bool)));

    //初始化算子列表
    //    m_ModelCheckOperatorAna=new QStandardItemModel;
    //    ui->tableViewCheckOperatorAna->setModel(m_ModelCheckOperatorAna);
    //    InitCheckOperatorAnaModel();

    //初始化算子tableview
    ui->tableViewCheckOperatorAna->setSelectionMode ( QAbstractItemView::SingleSelection);
    ui->tableViewCheckOperatorAna->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewCheckOperatorAna->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCheckOperatorAna->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableViewCheckOperatorAna->verticalHeader()->hide();
    ui->tableViewCheckOperatorAna->verticalHeader()->setDefaultSectionSize(36);


    ui->tableViewCheckOperatorSet->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewCheckOperatorSet->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCheckOperatorSet->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableViewCheckOperatorSet->verticalHeader()->hide();
    ui->tableViewCheckOperatorSet->verticalHeader()->setDefaultSectionSize(36);
    ui->tableViewCheckOperatorSet->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_ModelCheckOperatorAna=new QStandardItemModel;
    m_ModelCheckOperatorSet=new QStandardItemModel;
    ui->tableViewCheckOperatorSet->setModel(m_ModelCheckOperatorSet);
    ui->tableViewCheckOperatorAna->setModel(m_ModelCheckOperatorAna);
    InitCheckOperatorSetModel();
    InitCheckOperatorAnaModel();

    //初始化图像列表
    ui->tableViewImgLst->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewImgLst->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewImgLst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableViewImgLst->verticalHeader()->hide();
    ui->tableViewImgLst->verticalHeader()->setDefaultSectionSize(36);
    ui->tableViewImgLst->setSelectionMode ( QAbstractItemView::SingleSelection);

    m_ModelImgLst=new QStandardItemModel;
    ui->tableViewImgLst->setModel(m_ModelImgLst);
    InitImgLstModel();

}

void DlgCheckOperator::InitCheckOperatorAnaModel()
{
    m_ModelCheckOperatorAna->clear();
    QStringList listmodelMain;
    listmodelMain<<tr("编号")<<tr("名称")<<tr("是否有效")<<tr("状态")<<tr("删除");
    m_ModelCheckOperatorAna->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewCheckOperatorAna->setColumnWidth(0,50);
    ui->tableViewCheckOperatorAna->setColumnWidth(1,90);
    ui->tableViewCheckOperatorAna->setColumnWidth(2,90);
    ui->tableViewCheckOperatorAna->setColumnWidth(3,60);
    ui->tableViewCheckOperatorAna->setColumnWidth(4,70);
}

void DlgCheckOperator::InitCheckOperatorSetModel()
{
    m_ModelCheckOperatorSet->clear();
    QStringList listmodelMain;
    listmodelMain<<tr("编号")<<tr("名称")<<tr("是否有效")<<tr("状态")<<tr("删除");
    m_ModelCheckOperatorSet->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewCheckOperatorSet->setColumnWidth(0,50);
    ui->tableViewCheckOperatorSet->setColumnWidth(1,90);
    ui->tableViewCheckOperatorSet->setColumnWidth(2,90);
    ui->tableViewCheckOperatorSet->setColumnWidth(3,60);
    ui->tableViewCheckOperatorSet->setColumnWidth(4,70);
}


void DlgCheckOperator::loadAnalysisLstModel(int imgInfoNo)
{
    m_AnalysisImgInfoIdx= imgInfoNo;

    if(m_VImageInfo.size()==0) return;
    if( m_VImageInfo[imgInfoNo].m_cameraNo>PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.size()) return;
    //先加载算子列表
    int curCameraNo=m_VImageInfo[imgInfoNo].m_cameraNo;
    loadCheckOperatorAnaModel(curCameraNo);

    loadSceneAnaRect(curCameraNo);

}

void DlgCheckOperator::InitScene()
{
    //检查参数调试的场景
//    if( this->m_SceneAna!=nullptr)
//    {
//        delete   this->m_SceneAna;
//    }
//    this->m_SceneAna=new GraphicsSceneConfig;
//    connect(m_SceneAna, &GraphicsSceneConfig::sceneDoubleClick, this, &DlgCheckOperator::showDlgCheckOperator);
//    connect(m_SceneAna, &GraphicsSceneConfig::sceneClick,this->ui->tableViewCheckOperatorAna,&QTableView::selectRow);
//    connect(m_SceneAna, &GraphicsSceneConfig::sceneClick,this,&DlgCheckOperator::changeSelectedRow);
//    connect(m_SceneAna, &GraphicsSceneConfig::sceneMouseRelease, this, &DlgCheckOperator::updateCheckoperatorOption);


    if(this->m_SceneAnaArr.size()>0)
    {
        qDeleteAll(this->m_SceneAnaArr);
        this->m_SceneAnaArr.clear();
    }

    if(this->m_SceneArr.size()>0)
    {
        qDeleteAll(this->m_SceneArr);
        this->m_SceneArr.clear();
    }
    list<ImgPro*>::iterator it1=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.begin();
    //场景的加载
    it1=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.begin();
    for(;it1!=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.end();it1++)
    {
        int CameraNo=(*it1)->ImgIndex;

        GraphicsSceneConfig * sceneAna=new GraphicsSceneConfig(CameraNo);
        connect(sceneAna, &GraphicsSceneConfig::sceneDoubleClick, this, &DlgCheckOperator::showDlgCheckOperator);
        connect(sceneAna, &GraphicsSceneConfig::sceneClick,this->ui->tableViewCheckOperatorAna,&QTableView::selectRow);
        connect(sceneAna, &GraphicsSceneConfig::sceneClick,this,&DlgCheckOperator::changeSelectedRow);
        connect(sceneAna, &GraphicsSceneConfig::sceneMouseRelease, this, &DlgCheckOperator::updateCheckoperatorOption);
        this->m_SceneAnaArr[CameraNo]=sceneAna;



        GraphicsSceneConfig * scene=new GraphicsSceneConfig(CameraNo);
        connect(scene, &GraphicsSceneConfig::sceneDoubleClick, this, &DlgCheckOperator::showDlgCheckOperator);
        connect(scene, &GraphicsSceneConfig::sceneClick,this->ui->tableViewCheckOperatorSet,&QTableView::selectRow);
        connect(scene, &GraphicsSceneConfig::sceneClick,this,&DlgCheckOperator::changeSelectedRow);
        connect(scene, &GraphicsSceneConfig::sceneMouseRelease, this, &DlgCheckOperator::updateCheckoperatorOption);

        QPixmap pixMap;
        ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro* imgPro=  control.getImgPro(CameraNo);
        imgPro->RefOpencvImage.toQPixmap(pixMap);
        scene->SetBackGroundImage(pixMap,pixMap.width(),pixMap.height());
        int wid=pixMap.width();
        int hei=pixMap.height();

        this->m_SceneArr[CameraNo]=scene;


        m_ViewArrMain[CameraNo]->setScene(scene);
    }
}

void DlgCheckOperator::loadSceneRectAll()
{
    DSDEBUG<<"enter:loadSceneRectAll"<<endl;

    list<ImgPro*>::iterator itor=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.begin();
    for (;itor!=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.end();itor++)
    {
        loadSceneSetRect((*itor)->ImgIndex);
    }
}

void DlgCheckOperator::loadSceneAnaRect(int cameraNo)
{

    this->ui->stackedWidget_2->setCurrentIndex(cameraNo-1);
    m_ViewArrMain[cameraNo]->setScene(m_SceneAnaArr[cameraNo]);

    m_SceneAnaArr[cameraNo]->clear();
    QString imagePath=m_DirAnalysis+'/'+m_VImageInfo[m_AnalysisImgInfoIdx].m_baseFileName+".bmp";
    QImage tmpQImage(imagePath);
    QPixmap pixmap= QPixmap::fromImage(tmpQImage.rgbSwapped());

    // QPixmap pixmap(imagePath);
    m_SceneAnaArr[cameraNo]->SetBackGroundImage(pixmap,pixmap.width(),pixmap.height());


#ifdef IMG_TYPE_GRAY
    m_AnalysisImage.Format=ENUMDSIMAGEFORMAT_GRAY;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(imagePath).data();
    m_AnalysisImage.ImageGray=cv::imread(name,0);
    m_AnalysisImage.Width=m_AnalysisImage.ImageGray.cols;
    m_AnalysisImage.Height=m_AnalysisImage.ImageGray.rows;
#endif
#ifdef IMG_TYPE_RGB
    m_AnalysisImage.Format=ENUMDSIMAGEFORMAT_RGB;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(imagePath).data();
    m_AnalysisImage.ImageRGB=cv::imread(name);
    m_AnalysisImage.Width=m_AnalysisImage.ImageRGB.cols;
    m_AnalysisImage.Height=m_AnalysisImage.ImageRGB.rows;
#endif


    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro* imgPro=  control.getImgPro(cameraNo);

    list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
    for(;itor!=imgPro->LstCheckOperator.end();itor++)
    {
        m_SceneAnaArr[cameraNo]->addCheckOperator((*itor));
    }

    //    memset(AnalysisCalcResult,0,256);
    itor=imgPro->LstCheckOperator.begin();
    for(;itor!=imgPro->LstCheckOperator.end();itor++)
    {
        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
        {
            AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index]=2;
        }
        else
        {
            AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index]= AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index]==2?0:AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index];
        }
    }
    this->m_SceneAnaArr[cameraNo]->setCalcResState(AnalysisCalcResult);


}

void DlgCheckOperator::loadSceneSetRect(int cameraNo)
{

    this->m_SceneArr[cameraNo]->clear();
    //加载场景的背景
    QPixmap pixmap;
    PackageChecker* pc=PackageChecker::getInstance();
    //PackageChecker::getInstance()->ImgTobaccoAnalysis->RefImage[cameraNo].toQPixmap(pixmap);
    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro* imgPro=  control.getImgPro(cameraNo);
    imgPro->RefOpencvImage.toQPixmap(pixmap);
    //m_MapImage[cameraNo].toQPixmap(pixmap);
    this->m_SceneArr[cameraNo]->SetBackGroundImage(pixmap,pixmap.width(),pixmap.height());

    list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
    for(;itor!=imgPro->LstCheckOperator.end();itor++)
    {
        m_SceneArr[cameraNo]->addCheckOperator((*itor));
    }

    memset(AnalysisCalcResult,0,256);
    itor=imgPro->LstCheckOperator.begin();
    for(;itor!=imgPro->LstCheckOperator.end();itor++)
    {
        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
        {
            AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index]=2;
        }
    }
    this->m_SceneArr[cameraNo]->setCalcResState(AnalysisCalcResult);
}

void DlgCheckOperator::updateCheckoperatorOption()
{


    switch (this->m_Mode)
    {
    case ENUMDLGCHECKOPERATORMODE_SET:
    {
        ImgTobaccoControl  imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro * imgPro=imgTabaccoControl.getImgPro(this->m_CurCameraNo);

        list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
        for (;itor!=imgPro->LstCheckOperator.end();++itor)
        {
            switch ((*itor)->CheckType)
            {
            case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            {
                CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch=static_cast<CheckOperatorTemplateMatch*>((*itor));

                if((pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xe- pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs>40)&&
                        (pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ye- pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys)>40
                        )
                {
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs=15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys=15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xe=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xe-pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs-15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ye=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ye-pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys-15;  ;

                    QString dirBrand=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName+"/";

                    for (int i_tp=0;i_tp<pCheckOperatorTemplateMatch->SearchNum;i_tp++)
                    {
                        QString fnAll=QString("%1ImgPro%2/%3/template%4_All.bmp").arg(dirBrand).arg(imgPro->ImgIndex).arg(QString::fromStdString(pCheckOperatorTemplateMatch->Code)).arg(i_tp);
                        //                            QString fn=QString("%1ImgPro%2/CheckOperator%3/template%4.bmp").arg(dirBrand).arg(imgPro->ImgIndex).arg(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index).arg(i_tp);

                        QFileInfo fi(fnAll);
                        if(fi.exists())
                        {

                            QTextCodec *code = QTextCodec::codecForName("GB2312");
                            std::string name = code->fromUnicode(fnAll).data();

                            cv::Mat allImage=cv::imread(name,0);
                            cv::Rect rectSave(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs+pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
                                              pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys+pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys,
                                              pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xe-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
                                              pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ye-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys);
                            cv::Mat tmImage=cv::Mat(allImage,rectSave);


                            //                                name = code->fromUnicode(fn).data();
                            //                                cv::imwrite(name,tmImage);

                            OpencvImage templateImage;
                            templateImage.Width=tmImage.cols;
                            templateImage.Height=tmImage.rows;
                            templateImage.ImageGray=tmImage;
                            //cv::imwrite("d:/111.bmp",tmImage);
                            templateImage.Format=ENUMDSIMAGEFORMAT_GRAY;
                            imgPro->TemplateImage[QString::fromStdString(pCheckOperatorTemplateMatch->Code)][i_tp]=templateImage;
                            // cv::imwrite("d:/222.bmp",imgPro->TemplateImage[QString::fromStdString(pCheckOperatorTemplateMatch->Code)][i_tp].ImageGray);
                        }
                        else
                        {

                            pCheckOperatorTemplateMatch->SearchNum=0;
                            pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_IsCheck=false;
                            pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_Vaild=false;
                        }
                    }
                }
                else
                {
                    pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_IsCheck=false;
                    pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_Vaild=false;

                }


            }
                break;
            case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
            {
                CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>((*itor));
                CheckOperatorEdgePositioningControl control(pCheckOperatorEdgePositioning);
                cv::Rect rectTmp(pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xe-pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ye-pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys
                                 );
                cv::Mat image=cv::Mat(imgPro->RefOpencvImage.ImageGray,rectTmp);
                int ptx=0,pty=0,score=0;
                control.debugCalculator(image,image,(int)pCheckOperatorEdgePositioning->EdgePos,pCheckOperatorEdgePositioning->BinThresh,&ptx,&pty,&score);
                if(ptx)
                {
                    pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX=ptx+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs;
                }
                if(pty)
                {
                    pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY=pty+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys;
                }
                //                pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX=ptx;
                //                pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY=pty;
                DSDEBUG<<QString("更新边缘位置:(%1,%2)").arg(ptx).arg(pty);

            }

                break;
            default:
                break;

            }
        }
    }
        break;
    case ENUMDLGCHECKOPERATORMODE_ANA:
    {
        if(this->m_VImageInfo.size()==0) return;
        ImgTobaccoControl  imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro * imgPro=imgTabaccoControl.getImgPro(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        //QMap<int,STUCHCEKOPERATOR>::iterator itor2= m_mapImgTabacco[i].begin();
        list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();

        for (;itor!=imgPro->LstCheckOperator.end();++itor)
        {
            switch ((*itor)->CheckType)
            {
            case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            {

                CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch=static_cast<CheckOperatorTemplateMatch*>((*itor));

                if((pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xe- pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs>40)&&
                        (pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ye- pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys)>40
                        )
                {
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs=15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys=15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xe=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xe-pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs-15;
                    pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ye=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ye-pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys-15;  ;


                    QString dirBrand=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName+"/";

                    for (int i_tp=0;i_tp<pCheckOperatorTemplateMatch->SearchNum;i_tp++)
                    {
                        QString fnAll=QString("%1ImgPro%2/%3/template%4_All.bmp").arg(dirBrand).arg(imgPro->ImgIndex).arg(QString::fromStdString(pCheckOperatorTemplateMatch->Code)).arg(i_tp);
                        //                            QString fn=QString("%1ImgPro%2/CheckOperator%3/template%4.bmp").arg(dirBrand).arg(imgPro->ImgIndex).arg(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index).arg(i_tp);
                        QTextCodec *code = QTextCodec::codecForName("GB2312");
                        std::string name = code->fromUnicode(fnAll).data();
                        QFileInfo fi(fnAll);
                        if(fi.exists())
                        {
                            cv::Mat allImage=cv::imread(name,0);
                            cv::Rect rectSave(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Xs+pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
                                              pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_RectValid_Ys+pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys,
                                              pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xe-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
                                              pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ye-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys);
                            cv::Mat tmImage=cv::Mat(allImage,rectSave);

                            OpencvImage templateImage;
                            templateImage.Width=tmImage.cols;
                            templateImage.Height=tmImage.rows;
                            templateImage.ImageGray=tmImage;
                            templateImage.Format=ENUMDSIMAGEFORMAT_GRAY;
                            imgPro->TemplateImage[QString::fromStdString(pCheckOperatorTemplateMatch->Code)][i_tp]=templateImage;
                        }
                        else
                        {
                            pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_IsCheck=false;
                            pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_Vaild=false;
                            pCheckOperatorTemplateMatch->SearchNum=0;
                        }
                    }

                }
                else
                {
                    pCheckOperatorTemplateMatch->SearchNum=0;
                    pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_IsCheck=false;
                    pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Basic_Vaild=false;

                }

            }
                break;
            case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
            {
                CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>((*itor));
                CheckOperatorEdgePositioningControl control(pCheckOperatorEdgePositioning);
                cv::Rect rectTmp(pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xe-pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                 pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ye-pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys
                                 );
                cv::Mat image=cv::Mat(imgPro->CalcOpencvImage.ImageGray,rectTmp);
                int ptx=0,pty=0,score=0;

                control.debugCalculator(image,image,(int)pCheckOperatorEdgePositioning->EdgePos,pCheckOperatorEdgePositioning->BinThresh,&ptx,&pty,&score);
                if(ptx)
                {
                    pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX=ptx+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs;
                }
                if(pty)
                {
                    pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY=pty+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys;
                }
                DSDEBUG<<QString("更新边缘位置:(%1,%2)").arg(ptx).arg(pty);
            }
                break;
            default:
                break;
            }
        }
        //更新位置
    }
        break;

    }

}


void DlgCheckOperator::changeSelectedRow(int row)
{
    SelectCheckOperatorNo=row+1;
}

void DlgCheckOperator::showDlgCheckOperator(int itemIndex)
{
    DSDEBUG<<"start checkoperator dlg"<<endl;

    //        QScrollBar *verticalScroll = ui->tableView->verticalScrollBar();
    //        int valueV = verticalScroll->value();

    adjustCheckOperator(itemIndex);

    //    switch (this->m_Mode)
    //    {
    //    case ENUMMODE_SETTING:
    //        loadSceneRect(m_CurCameraNo);
    //        loadTableModel(m_CurCameraNo);
    //        this->m_VScene[m_CurCameraNo]->selectRectItem(itemIndex);
    //        break;
    //    case ENUMMODE_ANALYSIS:
    //        resetAnalysisScene();
    //        updateAnalysisScene(this->m_AnalysisIndex);
    //        doCaculate();
    //        loadAnalysisSceneRect(m_AnalysisCameraNo);
    //        loadCheckOperatorAnalysisModel();
    //        this->m_SceneAnalysis->selectRectItem(itemIndex);
    //        break;
    //    }
    //    emit changeCheckOperatorRow(itemIndex-1);
    //    QModelIndex nextIndex = this->ui->tableView->model()->index(valueV, 0);
    //    this->ui->tableView->scrollTo(nextIndex, QAbstractItemView::PositionAtTop);
}




void DlgCheckOperator::InitImgLstModel()
{
    m_ModelImgLst->clear();
    QStringList listmodelMain;
    listmodelMain<<tr("编号")<<tr("日期")<<tr("相机");
    m_ModelImgLst->setHorizontalHeaderLabels(listmodelMain);
    ui->tableViewImgLst->setColumnWidth(0,90);
    ui->tableViewImgLst->setColumnWidth(1,160);
    ui->tableViewImgLst->setColumnWidth(2,110);
    this->ui->tableViewImgLst->update();
}

void DlgCheckOperator::changeCameraEnable(bool b)
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro *imgPro=imgTabaccoControl.getImgPro(m_CurCameraNo);
    imgPro->Valid=b;
}

void DlgCheckOperator::buttonModeJudge(int mode)
{
    //if(this->m_Mode==static_cast<ENUMDLGCHECKOPERATORMODE>(mode)) return;

    this->m_Mode=static_cast<ENUMDLGCHECKOPERATORMODE>(mode);
    switch (static_cast<ENUMDLGCHECKOPERATORMODE>(mode))
    {
    case ENUMDLGCHECKOPERATORMODE_SET:
    {
        if(ui->checkBox->checkState()==Qt::Checked)
        {
            frmMessageBox *msg = new frmMessageBox;
            msg->SetMessage(QString(tr("请先停止动态采集")), 0);
            msg->exec();
            return ;
        }


        this->ui->stackedWidget->setCurrentIndex(0);
        this->ui->widgetCameras->setVisible(true);
        this->ui->label->setVisible(true);
        this->ui->sliderButton->setVisible(true);

        int tmpCameraNo=1;
        if(m_VImageInfo.size()>0)
        {
            tmpCameraNo=m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo;
            m_CurCameraNo=tmpCameraNo;
        }
        else
        {
            m_CurCameraNo=tmpCameraNo;
        }

        ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro *imgPro=imgTabaccoControl.getImgPro(m_CurCameraNo);
        this->ui->sliderButton->setSilderBtnState(imgPro->Valid);

        loadSceneSetRect(tmpCameraNo);

        this->ui->stackedWidget_2->setCurrentIndex(m_CurCameraNo-1);
        this->m_ViewArrMain[m_CurCameraNo]->setScene(m_SceneArr[tmpCameraNo]);

        btnGroupCamera->button(tmpCameraNo)->click();
        ui->tableViewCheckOperatorSet->scrollTo(ui->tableViewCheckOperatorSet->model()->index(SelectCheckOperatorNo-1,0));

        on_tableViewCheckOperatorSet_clicked(ui->tableViewCheckOperatorSet->model()->index(SelectCheckOperatorNo-1,0));
        ui->tableViewCheckOperatorSet->selectRow(SelectCheckOperatorNo-1);


    }
        break;
    case ENUMDLGCHECKOPERATORMODE_ANA:
    {

        this->ui->stackedWidget->setCurrentIndex(1);
        this->ui->widgetCameras->setVisible(false);
        this->ui->label->setVisible(false);
        this->ui->sliderButton->setVisible(false);

//        this->ui->graphicsView->setScene(m_SceneAna);

        this->ui->stackedWidget_2->setCurrentIndex(m_CurCameraNo-1);

        this->m_ViewArrMain[m_CurCameraNo]->setScene(m_SceneAnaArr[m_CurCameraNo]);



        //InitCheckOperatorAnaModel();
        if(m_AnalysisImgInfoIdx>=m_VImageInfo.size()) return;

        int curselectCheckOperator=SelectCheckOperatorNo;
        on_tableViewImgLst_clicked(ui->tableViewImgLst->model()->index(m_AnalysisImgInfoIdx,0));
        ui->tableViewImgLst->scrollTo(ui->tableViewImgLst->model()->index(m_AnalysisImgInfoIdx,0));
        ui->tableViewImgLst->selectRow(m_AnalysisImgInfoIdx);

        SelectCheckOperatorNo=curselectCheckOperator;
        on_tableViewCheckOperatorAna_clicked(ui->tableViewCheckOperatorAna->model()->index(SelectCheckOperatorNo-1,0));
        ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(SelectCheckOperatorNo-1,0));
        ui->tableViewCheckOperatorAna->selectRow(SelectCheckOperatorNo-1);

        //        loadSceneAnaRect(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        //        loadAnalysisLstModel(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        //        ui->tableViewCheckOperatorAna->selectRow(SelectCheckOperatorNo-1);
        //        ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(SelectCheckOperatorNo-1,0));


    }
        break;

    }

}

void DlgCheckOperator::buttonCameraJudge(int idx)
{
    m_CurCameraNo=idx;
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro *imgPro=imgTabaccoControl.getImgPro(m_CurCameraNo);
    this->ui->sliderButton->setSilderBtnState(imgPro->Valid);

    loadSceneSetRect(m_CurCameraNo);

    ui->stackedWidget_2->setCurrentIndex(idx-1);
    m_ViewArrMain[idx]->setScene(this->m_SceneArr[idx]);

    loadCheckOperatorSetModel(m_CurCameraNo);
}

void DlgCheckOperator::buttonShowModeJudge(int mode)
{

    m_ShowMode=static_cast<ENUMSHOWMODE>(mode);
    switch (static_cast<ENUMSHOWMODE>(mode))
    {
    case ENUMSHOWMODE_ALL:
    {
        QMap<int,GraphicsSceneConfig*>::iterator itor= m_SceneArr.begin();
        for(;itor!=m_SceneArr.end();itor++)
        {
            itor.value()->setRectMode(0);

        }

        m_SceneAnaArr[m_CurCameraNo]->setRectMode(0);

    }
        break;

    case ENUMSHOWMODE_CUR:
    {
        QMap<int,GraphicsSceneConfig*>::iterator itor= m_SceneArr.begin();
        for(;itor!=m_SceneArr.end();itor++)
        {
            itor.value()->setRectMode(1);

        }

        m_SceneAnaArr[m_CurCameraNo]->setRectMode(1);
    }
        break;

    }

    switch (m_Mode)
    {
    case  ENUMDLGCHECKOPERATORMODE_SET:
        this->m_SceneArr[m_CurCameraNo]->selectRectItem(this->SelectCheckOperatorNo);

        break;
    case  ENUMDLGCHECKOPERATORMODE_ANA:
        this->m_SceneAnaArr[m_CurCameraNo]->selectRectItem(this->SelectCheckOperatorNo);
        break;
    }
}

void DlgCheckOperator::loadCheckOperatorSetModel(int cameraNo)
{
    InitCheckOperatorSetModel();
    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro* imgPro=control.getImgPro(cameraNo);
    list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
    int row=0;
    for(;itor!=imgPro->LstCheckOperator.end();itor++,row++)
    {
        //  this->m_TabelModel[cameraNo]
        QStandardItem * itemNo;
        switch ((*itor)->CheckType)
        {

        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:

            itemNo =new QStandardItem(QString("R%1").arg((*itor)->stu_CheckOperatorBasic->Index));

            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
            itemNo=new QStandardItem(QString("C%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            break;
        }

        QStandardItem * itemName=new QStandardItem(getCheckOperatorName((*itor)->CheckType));
        QStandardItem * itemValid=new QStandardItem((*itor)->stu_CheckOperatorBasic->Basic_IsCheck?tr("有效"):tr("无效"));
        //qDebug()<<"itemValid:"<<itemValid->text()<<endl;;
        QStandardItem* itemState=new QStandardItem((*itor)->stu_CheckOperatorBasic->Basic_Vaild?tr("完成"):tr("未设置"));
        QStandardItem* itemDel=new QStandardItem(tr("删除"));

        itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemValid->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemState->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemDel->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QFont font;
        font.setUnderline(true);
        itemDel->setFont(font);
        itemDel->setForeground(QBrush(QColor(255, 0, 0)));

        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
        {
            itemNo->setBackground(QBrush(QColor(116,135,255)));
            itemName->setBackground(QBrush(QColor(116,135,255)));
            itemValid->setBackground(QBrush(QColor(116,135,255)));
            itemState->setBackground(QBrush(QColor(116,135,255)));
            itemDel->setBackground(QBrush(QColor(116,135,255)));
        }

        this->m_ModelCheckOperatorSet->setItem(row,0,itemNo);
        this->m_ModelCheckOperatorSet->setItem(row,1,itemName);
        this->m_ModelCheckOperatorSet->setItem(row,2,itemValid);
        this->m_ModelCheckOperatorSet->setItem(row,3,itemState);
        this->m_ModelCheckOperatorSet->setItem(row,4,itemDel);
    }
}




void DlgCheckOperator::loadCheckOperatorAnaModel(int cameraNo)
{
    InitCheckOperatorAnaModel();
    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro* imgPro=control.getImgPro(cameraNo);
    list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
    int row=0;
    for(;itor!=imgPro->LstCheckOperator.end();itor++,row++)
    {
        //  this->m_TabelModel[cameraNo]
        QStandardItem * itemNo;
        switch ((*itor)->CheckType)
        {

        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:

            itemNo =new QStandardItem(QString("R%1").arg((*itor)->stu_CheckOperatorBasic->Index));

            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
            itemNo=new QStandardItem(QString("C%1").arg((*itor)->stu_CheckOperatorBasic->Index));
            break;
        }

        QStandardItem * itemName=new QStandardItem(getCheckOperatorName((*itor)->CheckType));
        QStandardItem * itemValid=new QStandardItem((*itor)->stu_CheckOperatorBasic->Basic_IsCheck?tr("有效"):tr("无效"));
        //qDebug()<<"itemValid:"<<itemValid->text()<<endl;;
        QStandardItem* itemState=new QStandardItem(AnalysisCalcResult[(*itor)->stu_CheckOperatorBasic->Index]==1?tr("失败"):tr("完成"));
        QStandardItem* itemDel=new QStandardItem(tr("删除"));

        itemNo->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemValid->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemState->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemDel->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QFont font;
        font.setUnderline(true);
        itemDel->setFont(font);
        itemDel->setForeground(QBrush(QColor(255, 0, 0)));

        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
        {
            itemNo->setBackground(QBrush(QColor(116,135,255)));
            itemName->setBackground(QBrush(QColor(116,135,255)));
            itemValid->setBackground(QBrush(QColor(116,135,255)));
            itemState->setBackground(QBrush(QColor(116,135,255)));
            itemDel->setBackground(QBrush(QColor(116,135,255)));
        }



        this->m_ModelCheckOperatorAna->setItem(row,0,itemNo);
        this->m_ModelCheckOperatorAna->setItem(row,1,itemName);
        this->m_ModelCheckOperatorAna->setItem(row,2,itemValid);
        this->m_ModelCheckOperatorAna->setItem(row,3,itemState);
        this->m_ModelCheckOperatorAna->setItem(row,4,itemDel);
    }


}


DlgCheckOperator::~DlgCheckOperator()
{
    QString imgPathAna=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+tr("/分析图像/");
    QVector<ImageInfo> m_VImageInfoDel;
    QVector<QString> Imgfiles;
    FileHelper::getAllImgFilesInFolder(imgPathAna,Imgfiles);

    //    QFileInfoList fileInfoList = dir.entryInfoList(strLst,QDir::Files);

    for (int idx=0;idx<Imgfiles.size();idx++)
    {
        QFileInfo fInfo=Imgfiles[idx];
        if(fInfo.suffix()!="bmp"&&fInfo.suffix()!="jpg") continue;
        QString fileBaseName=fInfo.baseName();
        ImageInfo imageInfo(fileBaseName);
        m_VImageInfoDel.push_back(imageInfo);

    }
    qSort(m_VImageInfoDel.begin(),m_VImageInfoDel.end(),ImageInfo::compare);
    for (int i=0;i<m_VImageInfoDel.size();++i)
    {
        if(i<1*DSSystemParam::CameraNum) continue;
        QFile::remove(imgPathAna+ m_VImageInfoDel[i].m_baseFileName+".bmp");

    }



    PackageChecker::DynamicGrab=false;

    if(this->m_SceneAnaArr.size()>0)
    {
        qDeleteAll(this->m_SceneAnaArr);
        this->m_SceneAnaArr.clear();
    }

    if(m_SceneAna!=nullptr)
    {
        this->m_SceneAna->clear();
        delete  m_SceneAna;
        m_SceneAna=nullptr;
    }


    if(m_SceneArr.size()>0)
    {
        QMap<int,GraphicsSceneConfig*>::iterator it=m_SceneArr.begin();
        for(;it!=m_SceneArr.end();it++)
        {
            it.value()->clear();
            delete it.value();
        }
    }
    m_SceneArr.clear();

    if(m_VImageInfo.size()>0)
    {
        m_VImageInfo.clear();
    }

    if(m_SceneAna!=nullptr)
    {
        m_SceneAna->clear();
        delete  m_SceneAna;
    }
    if(m_ModelCheckOperatorSet!=nullptr)
    {
        m_ModelCheckOperatorSet->clear();
        delete  m_ModelCheckOperatorSet;
    }

    if(m_ModelCheckOperatorAna!=nullptr)
    {
        m_ModelCheckOperatorAna->clear();
        delete  m_ModelCheckOperatorAna;
    }
    //    if(m_ThreadGrab!=nullptr)
    //    {
    //        delete m_ThreadGrab;
    //    }


    delete ui;
}

void DlgCheckOperator::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;


    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    //    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf02c), 12);

    m_ViewArrMain[1]=ui->graphicsView;
    m_ViewArrMain[2]=ui->graphicsView_2;
    m_ViewArrMain[3]=ui->graphicsView_3;
    m_ViewArrMain[4]=ui->graphicsView_4;



    int tmpCameraNum=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.size();
    for (int idx=1;idx<=tmpCameraNum;++idx)
    {

        if((double)PackageChecker::getInstance()->ImgHeis[idx-1]/PackageChecker::getInstance()->ImgWids[idx-1]>(double)DLGCHECKOPERATOR_IMGHEI/DLGCHECKOPERATOR_IMGWID)
        {

             m_ViewArrMain[idx]->scaleto((double)DLGCHECKOPERATOR_IMGHEI/(double)PackageChecker::getInstance()->ImgHeis[idx-1],
                                            (double)DLGCHECKOPERATOR_IMGHEI/(double)PackageChecker::getInstance()->ImgHeis[idx-1]);

             m_ViewArrMain[idx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[idx-1],(double)PackageChecker::getInstance()->ImgHeis[idx-1]));

        }
        else
        {
             m_ViewArrMain[idx]->scaleto((double)DLGCHECKOPERATOR_IMGWID/PackageChecker::getInstance()->ImgWids[idx-1],
                                            (double)DLGCHECKOPERATOR_IMGWID/PackageChecker::getInstance()->ImgWids[idx-1]);

             m_ViewArrMain[idx]->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWids[idx-1],(double)PackageChecker::getInstance()->ImgHeis[idx-1]));

        }

    }


}

bool DlgCheckOperator::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgCheckOperator::mouseMoveEvent(QMouseEvent *e)
{
    //    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
    //        this->move(e->globalPos() - mousePoint);
    //        e->accept();
    //    }
}

void DlgCheckOperator::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgCheckOperator::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

//void DlgCheckOperator::on_btnMenu_Max_clicked()
//{
//    if (max) {
//        this->setGeometry(location);
//        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
//        ui->btnMenu_Max->setToolTip("最大化");
//    } else {
//        location = this->geometry();
//        this->setGeometry(qApp->desktop()->availableGeometry());
//        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
//        ui->btnMenu_Max->setToolTip("还原");
//    }
//    max = !max;
//}

//void DlgCheckOperator::on_btnMenu_Min_clicked()
//{
//    this->showMinimized();
//}

void DlgCheckOperator::on_btnMenu_Close_clicked()
{

    if(m_Capturing)
    {
        m_ThreadGrab->stop();
        this->ui->toolButtonCapture->setText(tr("取图"));
        m_Capturing=!m_Capturing;
    }

    PackageChecker::getInstance()->ImgTobaccoRun->copyto(PackageChecker::getInstance()->ImgTobaccoAnalysis);

    this->close();
}

void DlgCheckOperator::on_toolButtonAnaImg_clicked()
{
    if(ui->widgetAnaImg->isVisible())
    {
        ui->widgetAnaImg->setVisible(false);
        ui->toolButtonAnaImg->setStyleSheet("QToolButton#toolButtonAnaImg{border-image:url(:/更多normal.png);  background:transparent;}"
                                            "QToolButton#toolButtonAnaImg:hover{border-image:url(:/更多hover.png); }"
                                            "QToolButton#toolButtonAnaImg:pressed{border-image:url(:/更多press.png); }");

    }
    else
    {
        ui->widgetAnaImg->setVisible(true);
        ui->toolButtonAnaImg->setStyleSheet("QToolButton#toolButtonAnaImg{border-image:url(:/下箭头normal.png);  background:transparent;}"
                                            "QToolButton#toolButtonAnaImg:hover{border-image:url(:/下箭头hover.png); }"
                                            "QToolButton#toolButtonAnaImg:pressed{border-image:url(:/下箭头press.png); }");
    }
}

void DlgCheckOperator::on_toolButtonCoSet_clicked()
{
    if(ui->widgetCoSet->isVisible())
    {
        ui->widgetCoSet->setVisible(false);
        ui->toolButtonCoSet->setStyleSheet("QToolButton#toolButtonCoSet{border-image:url(:/更多normal.png);  background:transparent;}"
                                           "QToolButton#toolButtonCoSet:hover{border-image:url(:/更多hover.png); }"
                                           "QToolButton#toolButtonCoSet:pressed{border-image:url(:/更多press.png); }");
    }
    else
    {
        ui->widgetCoSet->setVisible(true);
        ui->toolButtonCoSet->setStyleSheet("QToolButton#toolButtonCoSet{border-image:url(:/下箭头normal.png);  background:transparent;}"
                                           "QToolButton#toolButtonCoSet:hover{border-image:url(:/下箭头hover.png); }"
                                           "QToolButton#toolButtonCoSet:pressed{border-image:url(:/下箭头press.png); }");
    }
}

void DlgCheckOperator::on_toolButtonCoSetLstInfo_clicked()
{

    if(ui->widgetCoSetLstInfo->isVisible())
    {
        ui->widgetCoSetLstInfo->setVisible(false);
        ui->toolButtonCoSetLstInfo->setStyleSheet("QToolButton#toolButtonCoSetLstInfo{border-image:url(:/更多normal.png);  background:transparent;}"
                                                  "QToolButton#toolButtonCoSetLstInfo:hover{border-image:url(:/更多hover.png); }"
                                                  "QToolButton#toolButtonCoSetLstInfo:pressed{border-image:url(:/更多press.png); }");
    }
    else
    {
        ui->widgetCoSetLstInfo->setVisible(true);
        ui->toolButtonCoSetLstInfo->setStyleSheet("QToolButton#toolButtonCoSetLstInfo{border-image:url(:/下箭头normal.png);  background:transparent;}"
                                                  "QToolButton#toolButtonCoSetLstInfo:hover{border-image:url(:/下箭头hover.png); }"
                                                  "QToolButton#toolButtonCoSetLstInfo:pressed{border-image:url(:/下箭头press.png); }");
    }
}

void DlgCheckOperator::on_toolButtonCoAnaLstInfo_clicked()
{
    if(ui->widgetAna->isVisible())
    {
        ui->widgetAna->setVisible(false);

        ui->toolButtonCoAnaLstInfo->setStyleSheet("QToolButton#toolButtonCoAnaLstInfo{border-image:url(:/更多normal.png);  background:transparent;}"
                                                  "QToolButton#toolButtonCoAnaLstInfo:hover{border-image:url(:/更多hover.png); }"
                                                  "QToolButton#toolButtonCoAnaLstInfo:pressed{border-image:url(:/更多press.png); }");

    }
    else
    {
        ui->widgetAna->setVisible(true);
        ui->toolButtonCoAnaLstInfo->setStyleSheet("QToolButton#toolButtonCoAnaLstInfo{border-image:url(:/下箭头normal.png);  background:transparent;}"
                                                  "QToolButton#toolButtonCoAnaLstInfo:hover{border-image:url(:/下箭头hover.png); }"
                                                  "QToolButton#toolButtonCoAnaLstInfo:pressed{border-image:url(:/下箭头press.png); }");
    }
}

QString DlgCheckOperator::getCheckOperatorName(ENUMCHECKOPERATORTYPE t)
{
    QString ret="";
    switch (t)
    {
    case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        ret=tr("对象搜索");
        break;
    case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        ret=tr("边缘定位");
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        ret=tr("缺陷检查");
        break;
    case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        ret=tr("嘴棒检查");
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        ret=tr("异型检查");
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        ret=tr("关联检查");
        break;
    case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        ret=tr("折线检查");
        break;
    case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        ret=tr("灰度测量");
        break;
    case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        ret=tr("直线查找");
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        ret=tr("关联检查");
        break;
    case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        ret=tr("拉线检查");
        break;
    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        ret=tr("双线检查");
        break;
    case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        ret=tr("空头检测");
        break;
    }
    return ret;
}

void DlgCheckOperator::on_tableViewCheckOperatorSet_clicked(const QModelIndex &index)
{

    if(this->m_ModelCheckOperatorSet->rowCount()==0) return;
    if(index.row()>this->m_ModelCheckOperatorSet->rowCount()) return;

    int selectNum=m_ModelCheckOperatorSet->item(index.row(),0)->text().mid(1).toInt();
    this->SelectCheckOperatorNo=selectNum;
    //int selectNum=index.row()+1;
    switch(index.column())
    {
    case 4:
    {

        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("确认删除当前算子吗？")), 1);
        int res= msg->exec();
        if(!res) return;


        ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        control.deleteImgProCheckOperator(this->m_CurCameraNo,selectNum);
        this->m_SceneArr[this->m_CurCameraNo]->deleteItem(selectNum);
        loadCheckOperatorSetModel(m_CurCameraNo);
        loadSceneSetRect(m_CurCameraNo);

        SelectCheckOperatorNo=selectNum;

        if(SelectCheckOperatorNo<=ui->tableViewCheckOperatorSet->model()->rowCount())
        {
            on_tableViewCheckOperatorSet_clicked(ui->tableViewCheckOperatorSet->model()->index(SelectCheckOperatorNo-1,0));
            ui->tableViewCheckOperatorSet->scrollTo(ui->tableViewCheckOperatorSet->model()->index(SelectCheckOperatorNo-1,0));

            ui->tableViewCheckOperatorSet->selectRow(SelectCheckOperatorNo-1);
        }
        else
        {
            ui->tableViewCheckOperatorSet->scrollToTop();
            on_tableViewCheckOperatorSet_clicked(ui->tableViewCheckOperatorSet->model()->index(0,0));
            ui->tableViewCheckOperatorSet->selectRow(0);
            SelectCheckOperatorNo=1;

        }

    }
        break;
    default:
        this->m_SceneArr[m_CurCameraNo]->selectRectItem(selectNum);
        // SelectCheckOperatorNo=selectNum;
    }
    SelectCheckOperatorNo=selectNum;
}

void DlgCheckOperator::on_tableViewImgLst_clicked(const QModelIndex &index)
{
    if(index.row()>this->m_ModelImgLst->rowCount()) return;
    this->m_AnalysisImgInfoIdx=index.row();

    this->m_CurCameraNo=m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo;
    QString imagePath=m_DirAnalysis+'/'+m_VImageInfo[m_AnalysisImgInfoIdx].m_baseFileName+".bmp";
    QImage tmpQImage(imagePath);
    QPixmap pixmap= QPixmap::fromImage(tmpQImage.rgbSwapped());

#ifdef IMG_TYPE_GRAY
    m_AnalysisImage.Format=ENUMDSIMAGEFORMAT_GRAY;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(imagePath).data();
    m_AnalysisImage.ImageGray=cv::imread(name,0);
    m_AnalysisImage.Width=m_AnalysisImage.ImageGray.cols;
    m_AnalysisImage.Height=m_AnalysisImage.ImageGray.rows;
#endif
#ifdef IMG_TYPE_RGB
    m_AnalysisImage.Format=ENUMDSIMAGEFORMAT_RGB;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(imagePath).data();
    m_AnalysisImage.ImageRGB=cv::imread(name);
    m_AnalysisImage.Width=m_AnalysisImage.ImageRGB.cols;
    m_AnalysisImage.Height=m_AnalysisImage.ImageRGB.rows;
#endif


    doCaculate();

//    ui->stackedWidget_2->setCurrentIndex(m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo-1);
//    m_ViewArrMain[m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo]->setScene(m_SceneAnaArr[m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo] );

    loadAnalysisLstModel( this->m_AnalysisImgInfoIdx);
    loadSceneAnaRect(m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo);
    m_SceneAnaArr[m_VImageInfo[this->m_AnalysisImgInfoIdx].m_cameraNo]->setCalcResState(AnalysisCalcResult);



    QString text="";
    if(m_VImageInfo[m_AnalysisImgInfoIdx].m_iNgType==1)
    {
        text=tr("图像");
    }
    if(m_VImageInfo[m_AnalysisImgInfoIdx].m_iNgType==2)
    {
        text=tr("光电");
    }
    if(m_VImageInfo[m_AnalysisImgInfoIdx].m_iNgType==3)
    {
        text=tr("图像+光电");
    }

    m_SceneAnaArr[m_CurCameraNo]->setText(text);

    for (int row=0;row<m_ModelCheckOperatorAna->rowCount();++row)
    {
        switch (AnalysisCalcResult[row+1])
        {
        case 1:
            m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(255,100,100)));
            m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(255,100,100)));
            m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(255,100,100)));
            m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(255,100,100)));
            m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(255,100,100)));

            break;
        case 2:
            m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(116,135,255)));
            m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(116,135,255)));
            m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(116,135,255)));
            m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(116,135,255)));
            m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(116,135,255)));
            break;
        }
    }



    bool isNg=false;
    for (int row=0;row<m_ModelCheckOperatorAna->rowCount();++row)
    {
        if(AnalysisCalcResult[row+1]==1)
        {
            QModelIndex modelIdx=m_ModelCheckOperatorAna->index(row,0);
            on_tableViewCheckOperatorAna_clicked(modelIdx);
            ui->tableViewCheckOperatorAna->selectRow(row);

            isNg=true;
            break;
        }
    }
    if(!isNg)
    {
        if(this->ui->tableViewCheckOperatorAna->model()->rowCount()!=0)
        {
            on_tableViewCheckOperatorAna_clicked(this->ui->tableViewCheckOperatorAna->model()->index(0,0));
            ui->tableViewCheckOperatorAna->selectRow(0);
        }
    }




}

void DlgCheckOperator::doCaculate()
{
    if(this->m_Mode!=ENUMDLGCHECKOPERATORMODE_ANA) return;
    //set calc image
    PackageChecker * pc=PackageChecker::getInstance();
    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    control.setCalcImage(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo,m_AnalysisImage);

    //获取当前相机的算子集合
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    ImgPro *imgPro=imgTabaccoControl.getImgPro(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);

    ImgProControl imgProControl(imgPro);

    QString resStr="";
    memset(AnalysisCalcResult,0,256);

    clock_t ssss=clock();
    DSDEBUG<<"clock:"<<clock()<<endl;



    int errNum=imgProControl.calculateCV(AnalysisCalcResult);

    //    for (int i=1;i<128;++i)
    //    {
    //        if(QString::number(AnalysisCalcResult[i])=="1"){
    //          resStr+=QString::number(i)+":"+QString::number(AnalysisCalcResult[i])+", ";
    //        }
    //    }
    DSDEBUG<<"clock:"<<clock()<<"   time:"<< clock()-ssss<< "     errNum:"<<errNum<<endl;
    // LOG(INFO)<<"time:"<< clock()-ssss<< "     errNum:"<<errNum<<endl;
    //  DSDEBUG<<resStr<<endl;

}

void DlgCheckOperator::on_dateEditChose_dateChanged(const QDate &date)
{
    QStringList strlst=m_DirAnalysis.split("/");
    strlst.removeLast();
    strlst.append(date.toString("yyyy-MM-dd"));
    QString newImagePath=strlst.join("/");


    if( this->m_DirAnalysis==newImagePath) return;
    this->m_DirAnalysis=newImagePath;
    loadAnaImageInfo(this->ui->comboBoxCamera->currentIndex());
    updateImgLstModel();
    m_AnalysisImgInfoIdx=0;
    if(m_VImageInfo.size()==0) return;
    on_tableViewImgLst_clicked(this->ui->tableViewImgLst->model()->index(0,0));

    ui->tableViewImgLst->selectRow(0);
}



void DlgCheckOperator::on_tableViewCheckOperatorAna_clicked(const QModelIndex &index)
{
    LOG(INFO)<<"算子列表窗口被单击,点击行号:"<<index.row();

    if(this->m_ModelCheckOperatorAna->rowCount()<=0) return;
    if(index.row()>this->m_ModelCheckOperatorAna->rowCount()) return;
    int selectNum=m_ModelCheckOperatorAna->item(index.row(),0)->text().mid(1).toInt();
    DSDEBUG<<selectNum;
    //int selectNum=index.row()+1;
    switch(index.column())
    {
    case 4:
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("确认删除当前算子吗？")), 1);
        int res= msg->exec();
        if(!res) return;

        ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        //        control.deleteImgProCheckOperator(this->m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo,selectNum);
        control.deleteImgProCheckOperator(this->m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo,selectNum);

        doCaculate();
        this->m_SceneAnaArr[m_CurCameraNo]->deleteItem(selectNum);
        loadCheckOperatorAnaModel(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        loadSceneAnaRect(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        buttonShowModeJudge(m_ShowMode);

        for (int row=0;row<m_ModelCheckOperatorAna->rowCount();++row)
        {
            switch (AnalysisCalcResult[row+1])
            {
            case 1:
                m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(255,100,100)));
                m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(255,100,100)));
                m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(255,100,100)));
                m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(255,100,100)));
                m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(255,100,100)));

                break;
            case 2:
                m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(116,135,255)));
                m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(116,135,255)));
                m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(116,135,255)));
                m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(116,135,255)));
                m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(116,135,255)));
                break;
            }
        }

        SelectCheckOperatorNo=selectNum;
        //        this->ui->tableViewCheckOperatorAna->selectRow(selectNum-1);

        if(selectNum>m_ModelCheckOperatorAna->rowCount())
        {
            selectNum--;
        }
        if(selectNum==0) return;

        QModelIndex modelIdx=m_ModelCheckOperatorAna->index(selectNum-1,0);
        //        on_tableViewCheckOperatorAna_clicked(modelIdx);
        ui->tableViewCheckOperatorAna->selectRow(selectNum-1);
        ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(selectNum-1,0));
        this->m_SceneAnaArr[m_CurCameraNo]->selectRectItem(selectNum);
        SelectCheckOperatorNo=selectNum;
    }
        break;
    default:
        this->m_SceneAnaArr[m_CurCameraNo]->selectRectItem(selectNum);
        SelectCheckOperatorNo=selectNum;
    }


}

void DlgCheckOperator::on_pushButtonImgPre_clicked()
{
    if(this->ui->tableViewImgLst->model()->rowCount()==0) return;
    if(m_AnalysisImgInfoIdx==0) return;

    QModelIndex modelIdxTmp=ui->tableViewImgLst->model()->index(--m_AnalysisImgInfoIdx,0);

    on_tableViewImgLst_clicked(modelIdxTmp);
    ui->tableViewImgLst->selectRow(m_AnalysisImgInfoIdx);

}

void DlgCheckOperator::on_pushButtonImgNext_clicked()
{
    if(this->ui->tableViewImgLst->model()->rowCount()==0) return;
    if(m_AnalysisImgInfoIdx==m_ModelImgLst->rowCount()-1) return;

    QModelIndex modelIdxTmp=ui->tableViewImgLst->model()->index(++m_AnalysisImgInfoIdx,0);

    on_tableViewImgLst_clicked(modelIdxTmp);
    ui->tableViewImgLst->selectRow(m_AnalysisImgInfoIdx);

}

void DlgCheckOperator::on_toolButtonImgMax_clicked()
{
    this->ui->graphicsView->scaleMax();
}

void DlgCheckOperator::on_toolButtonImgMin_clicked()
{
    this->ui->graphicsView->scaleMin();
}

void DlgCheckOperator::on_toolButtonImgInitial_clicked()
{
    this->ui->graphicsView->scaleInit();
}

void DlgCheckOperator::on_tableViewCheckOperatorSet_doubleClicked(const QModelIndex &index)
{
    if(index.row()>this->m_ModelCheckOperatorSet->rowCount()) return;
    int selectNum=m_ModelCheckOperatorSet->item(index.row(),0)->text().mid(1).toInt();
    switch(index.column())
    {
    case 4:
        break;
    case 2:
    {
        ImgTobaccoControl  imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro * imgPro=imgTabaccoControl.getImgPro(this->m_CurCameraNo);
        list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
        for(;itor!=imgPro->LstCheckOperator.end();++itor)
        {
            if((*itor)->stu_CheckOperatorBasic->Index==selectNum)
            {

                int res;
                if((*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
                {
                    frmMessageBox *msg = new frmMessageBox;
                    msg->SetMessage(QString(tr("是否禁用当前算子?")), 1);
                    res= msg->exec();
                }
                else
                {

                    frmMessageBox *msg = new frmMessageBox;
                    msg->SetMessage(QString(tr("是否启用当前算子?")), 1);
                    res= msg->exec();
                }
                if(res)
                {

                    (*itor)->stu_CheckOperatorBasic->Basic_IsCheck=!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck;
                    loadSceneSetRect(m_CurCameraNo);
                    loadCheckOperatorSetModel(m_CurCameraNo);

                    QModelIndex lastModelIdx=ui->tableViewCheckOperatorSet->model()->index(selectNum-1,0);
                    on_tableViewCheckOperatorSet_clicked(lastModelIdx);
                    ui->tableViewCheckOperatorSet->selectRow(selectNum-1);

                }
                else
                {
                    return;
                }
            }
        }
    }
        break;

    default:
        this->m_SceneArr[m_CurCameraNo]->selectRectItem(selectNum);
        adjustCheckOperator(selectNum);

        break;
    }

    ui->tableViewCheckOperatorSet->selectRow(selectNum-1);
    on_tableViewCheckOperatorSet_clicked(ui->tableViewCheckOperatorSet->model()->index(selectNum-1,0));
    ui->tableViewCheckOperatorSet->scrollTo(ui->tableViewCheckOperatorSet->model()->index(selectNum-1,0));
    this->SelectCheckOperatorNo=selectNum;
}

void DlgCheckOperator::adjustCheckOperator(int itemIndex)
{

    int lastIdx=itemIndex;
    //    DSDEBUG<<lastIdx;
    switch (this->m_Mode)
    {
    case  ENUMDLGCHECKOPERATORMODE_SET:
    {
        ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro * imgPro=imgTabaccoControl.getImgPro(this->m_CurCameraNo);
        CheckOperatorBasic * pCheckOperator=imgTabaccoControl.getImgProCheckOperator(this->m_CurCameraNo,itemIndex);
        switch (pCheckOperator->CheckType)
        {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {
            CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch=static_cast<CheckOperatorTemplateMatch*>(pCheckOperator) ;
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            DlgMatchTemplate w(this->m_CurCameraNo,pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,pCheckOperatorTemplateMatch,PackageChecker::getInstance()->ImgTobaccoAnalysis,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>(pCheckOperator) ;
            DlgEdgePositioning w(pCheckOperatorEdgePositioning,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorDefectInspect* pCheckOperatorDefectInspect=static_cast<CheckOperatorDefectInspect*>(pCheckOperator) ;
            DlgDefectInspect w(pCheckOperatorDefectInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorCircleInspect* pCheckOperatorCircleInspect=static_cast<CheckOperatorCircleInspect*>(pCheckOperator) ;
            DlgCircleInspect w(pCheckOperatorCircleInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorDefectInspect2* pCheckOperatorDefectInspect=static_cast<CheckOperatorDefectInspect2*>(pCheckOperator) ;
            DlgDefectInspect2 w(pCheckOperatorDefectInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        {

            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2=static_cast<CheckOperatorAssociatedInspect2*>(pCheckOperator) ;
            DlgAssociatedInspect2  w(pCheckOperatorAssociatedInspect2,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;

        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        {

            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect=static_cast<CheckOperatorDLObjectDetect*>(pCheckOperator) ;
            DlgDLObjectDetect w(pCheckOperatorDLObjectDetect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();

        }
            break;
        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect=static_cast<CheckOperatorBrokenlineInspect*>(pCheckOperator) ;
            DlgBrokenLineInspect w(pCheckOperatorBrokenlineInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement=static_cast<CheckOperatorGrayMeasurement*>(pCheckOperator) ;
            DlgGrayMeasurement w(pCheckOperatorGrayMeasurement,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect=static_cast<CheckOperatorAssociatedInspect*>(pCheckOperator) ;
            DlgAssociatedInspect *w=new DlgAssociatedInspect(pCheckOperatorAssociatedInspect,imgPro,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w->exec();
            delete  w;


        }

            break;
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorStayInspect* pCheckOperatorStayInspect=static_cast<CheckOperatorStayInspect*>(pCheckOperator) ;
            DlgStayInspect *w=new DlgStayInspect(pCheckOperatorStayInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w->exec();
            delete  w;
        }
            break;
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect=static_cast<CheckOperatorDoubleStayInspect*>(pCheckOperator) ;
            DlgDoubleStayInspect *w=new DlgDoubleStayInspect(pCheckOperatorDoubleStayInspect,imgPro,imgPro->RefOpencvImage,(int)ENUMDLGCHECKOPERATORMODE_SET);
            w->exec();
            delete  w;
        }
            break;
        }

    }
        break;
    case  ENUMDLGCHECKOPERATORMODE_ANA:
    {
        ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        int tmpCameraNo=m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo;
        ImgPro * imgPro=imgTabaccoControl.getImgPro(tmpCameraNo);
        CheckOperatorBasic * pCheckOperator=imgTabaccoControl.getImgProCheckOperator(tmpCameraNo,itemIndex);
        switch (pCheckOperator->CheckType)
        {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {

            CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch=static_cast<CheckOperatorTemplateMatch*>(pCheckOperator) ;
            DlgMatchTemplate w(tmpCameraNo,pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,pCheckOperatorTemplateMatch,PackageChecker::getInstance()->ImgTobaccoAnalysis,m_AnalysisImage,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning=static_cast<CheckOperatorEdgePositioning*>(pCheckOperator) ;
            DlgEdgePositioning w(pCheckOperatorEdgePositioning,imgPro,m_AnalysisImage,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        {
            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }


            CheckOperatorDefectInspect* pCheckOperatorDefectInspect=static_cast<CheckOperatorDefectInspect*>(pCheckOperator) ;
            DlgDefectInspect w(pCheckOperatorDefectInspect,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        {

            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorCircleInspect* pCheckOperatorCircleInspect=static_cast<CheckOperatorCircleInspect*>(pCheckOperator) ;
            DlgCircleInspect w(pCheckOperatorCircleInspect,imgPro,m_AnalysisImage,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();

        }
            break;

        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect=static_cast<CheckOperatorDLObjectDetect*>(pCheckOperator) ;
            DlgDLObjectDetect w(pCheckOperatorDLObjectDetect,imgPro,m_AnalysisImage,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        {
            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }

            CheckOperatorDefectInspect2* pCheckOperatorDefectInspect=static_cast<CheckOperatorDefectInspect2*>(pCheckOperator) ;
            DlgDefectInspect2 w(pCheckOperatorDefectInspect,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;


        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        {
            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }


            CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2=static_cast<CheckOperatorAssociatedInspect2*>(pCheckOperator) ;
            DlgAssociatedInspect2 w(pCheckOperatorAssociatedInspect2,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;

        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        {
            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }

            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect=static_cast<CheckOperatorBrokenlineInspect*>(pCheckOperator) ;
            DlgBrokenLineInspect w(pCheckOperatorBrokenlineInspect,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        {
            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }


            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement=static_cast<CheckOperatorGrayMeasurement*>(pCheckOperator) ;
            DlgGrayMeasurement w(pCheckOperatorGrayMeasurement,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w.exec();
        }
            break;
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        {
            ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
            //ImgPro* imgPro=  control.getImgPro(m_CurCameraNo);
            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect=static_cast<CheckOperatorAssociatedInspect*>(pCheckOperator) ;

            DlgAssociatedInspect *w=new DlgAssociatedInspect(pCheckOperatorAssociatedInspect,imgPro,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w->exec();
            delete  w;
        }
            break;
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        {

            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }

            CheckOperatorStayInspect* pCheckOperatorStayInspect=static_cast<CheckOperatorStayInspect*>(pCheckOperator) ;
            DlgStayInspect *w=new DlgStayInspect(pCheckOperatorStayInspect,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w->exec();
            delete  w;
        }
            break;
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        {

            StuCheckOperatorBasicControl control(pCheckOperator->stu_CheckOperatorBasic);
            std::vector<int> vRefx;
            std::vector<int> vRefy;
            control.getRefX(vRefx);
            control.getRefY(vRefy);
            cv::Point bestPt(0,0);
            int bestScore=0;
            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
            {
                if(imgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefx[idx_x]].Score;
                    bestPt.x=imgPro->MapPostion[vRefx[idx_x]].Pt.x;
                }
            }
            bestScore=0;
            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
            {
                if(imgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                {
                    bestScore=imgPro->MapPostion[vRefy[idx_y]].Score;
                    bestPt.y=imgPro->MapPostion[vRefy[idx_y]].Pt.y;
                }
            }

            CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect=static_cast<CheckOperatorDoubleStayInspect*>(pCheckOperator) ;
            DlgDoubleStayInspect *w=new DlgDoubleStayInspect(pCheckOperatorDoubleStayInspect,imgPro,m_AnalysisImage,bestPt.x,bestPt.y,(int)ENUMDLGCHECKOPERATORMODE_ANA);
            w->exec();
            delete  w;
        }
            break;
        }

        on_tableViewImgLst_clicked( ui->tableViewImgLst->model()->index(m_AnalysisImgInfoIdx,0));
        ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(lastIdx-1,0));
        ui->tableViewCheckOperatorAna->selectRow(lastIdx-1);
        m_SceneArr[m_CurCameraNo]->selectRectItem(lastIdx);
        on_tableViewCheckOperatorAna_clicked(ui->tableViewCheckOperatorAna->model()->index(lastIdx-1,0));
        SelectCheckOperatorNo=lastIdx;
    }
        break;
    }

}


void DlgCheckOperator::on_tableViewCheckOperatorAna_doubleClicked(const QModelIndex &index)
{

    if(index.row()>this->m_ModelCheckOperatorAna->rowCount()) return;
    int selectNum=m_ModelCheckOperatorAna->item(index.row(),0)->text().mid(1).toInt();

    DSDEBUG<<selectNum;
    switch(index.column())
    {
    case 4:
        break;
    case 2:
    {
        ImgTobaccoControl  imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
        ImgPro * imgPro=imgTabaccoControl.getImgPro(m_VImageInfo[m_AnalysisImgInfoIdx].m_cameraNo);
        list<CheckOperatorBasic*>::iterator itor=imgPro->LstCheckOperator.begin();
        for(;itor!=imgPro->LstCheckOperator.end();++itor)
        {
            if((*itor)->stu_CheckOperatorBasic->Index==selectNum)
            {

                int res;
                if((*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
                {
                    frmMessageBox *msg = new frmMessageBox;
                    msg->SetMessage(QString(tr("是否禁用当前算子?")), 1);
                    res= msg->exec();
                }
                else
                {
                    frmMessageBox *msg = new frmMessageBox;
                    msg->SetMessage(QString(tr("是否启用当前算子?")), 1);
                    res= msg->exec();
                }
                DSDEBUG<<res;
                if(res)
                {
                    (*itor)->stu_CheckOperatorBasic->Basic_IsCheck=!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck;

                    loadSceneAnaRect(m_CurCameraNo);
                    loadCheckOperatorAnaModel(m_CurCameraNo);

                    for (int row=0;row<m_ModelCheckOperatorAna->rowCount();++row)
                    {
                        switch (AnalysisCalcResult[row+1])
                        {
                        case 1:
                            m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(255,100,100)));
                            m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(255,100,100)));
                            m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(255,100,100)));
                            m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(255,100,100)));
                            m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(255,100,100)));
                            break;
                        case 2:
                            m_ModelCheckOperatorAna->item(row,0)->setBackground(QBrush(QColor(116,135,255)));
                            m_ModelCheckOperatorAna->item(row,1)->setBackground(QBrush(QColor(116,135,255)));
                            m_ModelCheckOperatorAna->item(row,2)->setBackground(QBrush(QColor(116,135,255)));
                            m_ModelCheckOperatorAna->item(row,3)->setBackground(QBrush(QColor(116,135,255)));
                            m_ModelCheckOperatorAna->item(row,4)->setBackground(QBrush(QColor(116,135,255)));
                            break;
                        }
                    }

                    QModelIndex lastModelIdx=ui->tableViewCheckOperatorAna->model()->index(selectNum-1,0);
                    on_tableViewCheckOperatorAna_clicked(lastModelIdx);
                    ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(selectNum-1,0));
                    ui->tableViewCheckOperatorAna->selectRow(selectNum-1);

                }
                else
                {
                    return;
                }
            }
        }
    }
        break;
    default:
        this->m_SceneAnaArr[m_CurCameraNo]->selectRectItem(selectNum);
        adjustCheckOperator(selectNum);
        //        on_tableViewImgLst_clicked( ui->tableViewImgLst->model()->index(m_AnalysisImgInfoIdx,0));
        break;
    }



    //    ui->tableViewCheckOperatorAna->selectRow(selectNum-1);
    //    on_tableViewCheckOperatorAna_clicked(ui->tableViewCheckOperatorAna->model()->index(selectNum-1,0));
    //    ui->tableViewCheckOperatorAna->scrollTo(ui->tableViewCheckOperatorAna->model()->index(selectNum-1,0));
    //    this->SelectCheckOperatorNo=selectNum;

}

void DlgCheckOperator::on_toolButtonUpdate_clicked()
{
    //    QStringList strlst=m_DirAnalysis.split("/");
    //    strlst.removeLast();
    //    strlst.append(ui->dateEditChose->date().toString("yyyy-MM-dd"));
    //    QString newImagePath=strlst.join("/");


    if(ui->checkBox->checkState()==Qt::Checked)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请先停止动态采集")), 0);
        msg->exec();
        return ;

    }
    ui->checkBox->setEnabled(false);
    ui->pushButtonAnaDel->setVisible(false);
    ui->dateEditChose->setEnabled(true);
    ui->pushButtonShowAna->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929); }");
    QString newImagePath=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/NG/"+ui->dateEditChose->date().toString("yyyy-MM-dd");
    this->m_DirAnalysis=newImagePath;
    loadAnaImageInfo(this->ui->comboBoxCamera->currentIndex());
    updateImgLstModel();
    m_AnalysisImgInfoIdx=0;
    if(m_VImageInfo.size()==0) return;
    on_tableViewImgLst_clicked(this->ui->tableViewImgLst->model()->index(0,0));

    ui->tableViewImgLst->selectRow(0);
}

void DlgCheckOperator::on_toolButtonShowAll_clicked()
{
    //    QStringList strlst=m_DirAnalysis.split("/");
    //    if(strlst.size()<3) return;
    //    strlst.removeLast();
    //    strlst.removeLast();
    //    strlst.append("所有图像");
    //    strlst.append(ui->dateEditChose->date().toString("yyyy-MM-dd"));
    //    QString newImagePath=strlst.join("/");

    if(ui->checkBox->checkState()==Qt::Checked)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请先停止动态采集")), 0);
        msg->exec();
        return ;

    }
    ui->checkBox->setEnabled(false);
    ui->pushButtonAnaDel->setVisible(false);
    ui->dateEditChose->setEnabled(true);
    ui->pushButtonShowAna->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929); }");

    QString newImagePath= m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+tr("/所有图像/")+ui->dateEditChose->date().toString("yyyy-MM-dd");
    this->m_DirAnalysis=newImagePath;
    loadAnaImageInfo(this->ui->comboBoxCamera->currentIndex());
    updateImgLstModel();
    m_AnalysisImgInfoIdx=0;
    if(m_VImageInfo.size()==0) return;
    on_tableViewImgLst_clicked(this->ui->tableViewImgLst->model()->index(0,0));
    ui->tableViewImgLst->selectRow(0);
}

void DlgCheckOperator::on_toolButtonShowNg_clicked()
{
    //    QStringList strlst=m_DirAnalysis.split("/");
    //    if(strlst.size()<3) return;
    //    strlst.removeLast();
    //    strlst.removeLast();
    //    strlst.append("NG");
    //    strlst.append(ui->dateEditChose->date().toString("yyyy-MM-dd"));
    //    QString newImagePath=strlst.join("/");

    if(ui->checkBox->checkState()==Qt::Checked)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请先停止动态采集")), 0);
        msg->exec();
        return ;

    }
    ui->checkBox->setEnabled(false);
    ui->pushButtonAnaDel->setVisible(false);
    ui->dateEditChose->setEnabled(true);
    ui->pushButtonShowAna->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929); }");

    QString newImagePath= m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/NG/"+ui->dateEditChose->date().toString("yyyy-MM-dd");
    this->m_DirAnalysis=newImagePath;
    loadAnaImageInfo(this->ui->comboBoxCamera->currentIndex());
    updateImgLstModel();
    m_AnalysisImgInfoIdx=0;
    if(m_VImageInfo.size()==0) return;
    on_tableViewImgLst_clicked(this->ui->tableViewImgLst->model()->index(0,0));
    ui->tableViewImgLst->selectRow(0);
}

void DlgCheckOperator::on_comboBoxCamera_currentIndexChanged(int index)
{
    loadAnaImageInfo(this->ui->comboBoxCamera->currentIndex());
    updateImgLstModel();
    m_AnalysisImgInfoIdx=0;
    if(m_VImageInfo.size()==0) return;
    on_tableViewImgLst_clicked(this->ui->tableViewImgLst->model()->index(0,0));
    ui->tableViewImgLst->selectRow(0);
}

void DlgCheckOperator::on_PushBtnCO_Template_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_TEMPLATEMATCH);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToTop();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_EdgePositioning_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_EDGEPOSITIONING);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToTop();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_DefectInspect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_DEFECTINSPECT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}


void DlgCheckOperator::on_pushButtonCO_CircleInspect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_CIRCLEINSPECT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));

}


void DlgCheckOperator::on_PushBtnCO_GrayMeasurement_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_BrokenLineInspect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_AssociatedInspect_clicked()
{

    //    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    //    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT);
    //    loadSceneSetRect(this->m_CurCameraNo);
    //    loadCheckOperatorSetModel(m_CurCameraNo);
    //    this->ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    //    ui->tableViewCheckOperatorSet->scrollToBottom();
    //    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));



    //连包检查



    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);

    //只有一个关联检查框
    list<CheckOperatorBasic*>::iterator itor= imgTabaccoControl.getImgPro(m_CurCameraNo)->LstCheckOperator.begin();
    for (;itor!=imgTabaccoControl.getImgPro(m_CurCameraNo)->LstCheckOperator.end();itor++)
    {
        if(((*itor)->CheckType)==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2)
            return;
    }


    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));



}

void DlgCheckOperator::on_PushBtnCO_DoubleStayInspect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_StayInspect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_STAYINSPECT);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

void DlgCheckOperator::on_PushBtnCO_DefectInspect2_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_DEFECTINSPECT2);
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
}

//void DlgCheckOperator::on_PushBtnCO_DefectInspect2_clicked()
//{
//    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);
//    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2);
//    loadSceneSetRect(this->m_CurCameraNo);
//    loadCheckOperatorSetModel(m_CurCameraNo);
//    ui->tableViewCheckOperatorSet->scrollToBottom();
//    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
//    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));
//}



void DlgCheckOperator::on_checkBox_stateChanged(int arg1)
{
    PackageChecker::DynamicGrab=arg1;
    DSDEBUG<<"动态采集:"<<PackageChecker::DynamicGrab;
    if( PackageChecker::DynamicGrab)
    {
        ui->pushButtonAnaDel->setEnabled(false);
        ui->pushButtonAnaDel->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #AAAAAA); }");
        if(m_Timer==nullptr)
        {
            m_Timer= new QTimer(this);
        }

        //        this->ui->toolButtonShowNg->setEnabled(false);
        //        this->ui->toolButtonShowAll->setEnabled(false);
        //        this->ui->toolButtonUpdate->setEnabled(false);
        this->ui->dateEditChose->setEnabled(false);

        // this->ui->toolButtonShowNg->setStyleSheet("border-image: url(:/查看错误图片normal.png);background-color:rgb(78,78,78); ");

        InitImgLstModel();
        this->m_SceneAnaArr[m_CurCameraNo]->clear();
        InitCheckOperatorAnaModel();

        m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+tr("/分析图像/");


        connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
        m_Timer->start(100);
    }
    else
    {
        ui->pushButtonAnaDel->setEnabled(true);
        ui->pushButtonAnaDel->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929); }");
        if(m_Timer!=nullptr)
        {
            m_Timer->stop();
            delete  m_Timer;
            m_Timer=nullptr;
            QThread::msleep(100);
            loadAnaImageInfo(0);
            updateImgLstModel();
        }
        //        this->ui->toolButtonShowNg->setEnabled(true);
        //        this->ui->toolButtonShowAll->setEnabled(true);
        //        this->ui->toolButtonUpdate->setEnabled(true);
        this->ui->dateEditChose->setEnabled(true);

        //        this->ui->toolButtonShowNg->setStyleSheet("QToolButton#toolButtonShowNg {border-image: url(:/查看错误图片normal.png);background-color:rgb(0,0,0); "
        //                                                  "border-style:none;}QToolButton#toolButtonShowNg:hover{border-image: url(:/查看错误图片hover.png);border-style:none;} "
        //                                                  "QToolButton#toolButtonShowNg:pressed{border-image: url(:/查看错误图片press.png); border-style:none;}");


        this->m_TimerImageNum=0;
        //        m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+"/NG/"+ui->dateEditChose->date().toString("yyyy-MM-dd");
    }
}

void DlgCheckOperator::onTimeout()
{

    QStringList strLst;
    QDir dir(m_DirAnalysis);
    DSDEBUG<<"分析图像路径:"<<m_DirAnalysis<<endl;
    QFileInfoList fileInfoList = dir.entryInfoList(strLst,QDir::Files);
    if(fileInfoList.size()==m_TimerImageNum) return;

    DSDEBUG<<"定时器触发"<<endl;
    m_TimerImageNum=fileInfoList.size();

    loadAnaImageInfo(0);
    updateImgLstModel();
}


void DlgCheckOperator::on_toolButtonSave_clicked()
{
    //    on_tableViewImgLst_clicked( ui->tableViewImgLst->model()->index(m_AnalysisImgInfoIdx,0));


    if(m_Capturing)
    {

        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请先停止获取图像")), 0);
        msg->exec();
        return ;
    }


    //    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    //    control.saveRefImagetoFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    //    control.saveConfigtoXmlFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    //    control.saveTemplatestoFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    //    QString backupPath=DSSystemParam::AppPath+"/backup/backup1/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
    //    QString srcpath=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    //    FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName),
    //                                backupPath
    //                                );


    switch (DSSystemParam::SystemState)
    {
    case DSSystemParam::ENUMSYSTEMSTATE_RUNNING:

    case DSSystemParam::ENUMSYSTEMSTATE_STOPPING:

        break;
    }
}

void DlgCheckOperator::on_toolButtonCancel_clicked()
{
    if(m_Capturing)
    {
        m_ThreadGrab->stop();
        this->ui->toolButtonCapture->setText(tr("取图"));
        m_Capturing=!m_Capturing;
    }

    PackageChecker::getInstance()->ImgTobaccoRun->copyto(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    close();
}

void DlgCheckOperator::on_toolButtonSaveReturn_clicked()
{
    if(m_Capturing)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请先停止获取图像")), 0);
        msg->exec();
        return ;
    }


    ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoAnalysis);
    control.saveRefImagetoFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);



    control.saveConfigtoXmlFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    control.saveTemplatestoFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);



    QString backupPath=DSSystemParam::AppPath+"/backup/backup1/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName;
    QString srcpath=QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
    if(!PackageChecker::getInstance()->testIsXmlfileBroken(srcpath))
    {
        FileHelper::copyRecursively(QString(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName),
                                    backupPath);
    }


    switch (DSSystemParam::SystemState)
    {
    case DSSystemParam::ENUMSYSTEMSTATE_RUNNING:




        //        PackageChecker::getInstance()->UpdateCheckOperator2=1;
        //        QThread::msleep(200);
        //        PackageChecker::getInstance()->m_copyMutex.lock();
        //        PackageChecker::getInstance()->ImgTobaccoAnalysis->copyto( PackageChecker::getInstance()->ImgTobaccoRun);
        //        PackageChecker::getInstance()->m_copyMutex.unlock();
        //        PackageChecker::getInstance()->UpdateCheckOperator2=0;




        PackageChecker::getInstance()->UpdateCheckOperator=1;
        break;
    case DSSystemParam::ENUMSYSTEMSTATE_STOPPING:
        ImgTobacco* tmp=PackageChecker::getInstance()->ImgTobaccoRun;
        delete  tmp;
        PackageChecker::getInstance()->ImgTobaccoRun=new ImgTobacco;
        PackageChecker::getInstance()->ImgTobaccoAnalysis->copyto( PackageChecker::getInstance()->ImgTobaccoRun);

        //        ImgTobaccoControl control(PackageChecker::getInstance()->ImgTobaccoRun);
        //        control.loadConfig(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
        //        control.loadRefImageFromDir(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
        //        control.loadTemplate(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName);
        break;
    }

    //  QThread::msleep(400);



    close();
}

void DlgCheckOperator::on_toolButtonCapture_clicked()
{
    if(this->m_Mode==ENUMDLGCHECKOPERATORMODE_ANA)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("请切换至设定模式!")), 0);
        msg->exec();
        return;

    }

    if(DSSystemParam::SystemState==DSSystemParam::ENUMSYSTEMSTATE_RUNNING)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("当前系统正在运行,请停止运行系统!")), 0);
        msg->exec();
        return;
    }

    PackageChecker* pc=PackageChecker::getInstance();

    if(!pc->Cameras[m_CurCameraNo]->IsOpen())
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(QString(tr("相机%1无法打开,请检查相机%1是否连接!")).arg(m_CurCameraNo), 0);
        int res= msg->exec();
        return;
    }

    if(m_Capturing)
    {
        m_ThreadGrab->stop();
        this->ui->toolButtonCapture->setText(tr("取图"));
        m_Capturing=!m_Capturing;
    }
    else
    {
        m_ThreadGrab->setCameraIdx(m_CurCameraNo);
        m_ThreadGrab->start();
        this->ui->toolButtonCapture->setText(tr("停取"));
        m_Capturing=!m_Capturing;
    }

}

void DlgCheckOperator::on_pushButtonShowAna_clicked()
{
    ui->checkBox->setEnabled(true);
    ui->pushButtonAnaDel->setVisible(true);
    ui->pushButtonShowAna->setStyleSheet("QPushButton{border-style: none;border: 0px;color: #F0F0F0; padding: 5px;min-height: 20px;border-radius:5px;font: 11pt \"微软雅黑\";background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #AAAAAA); }");
    ui->dateEditChose->setEnabled(false);
    InitImgLstModel();
    this->m_SceneAnaArr[m_CurCameraNo]->clear();
    InitCheckOperatorAnaModel();

    m_DirAnalysis=PackageChecker::getInstance()->Options->ImgSavePath()+tr("/图像保存/")+DSSystemParam::BrandName+tr("/分析图像/");

    //    QStringList strLst;
    //    QDir dir(m_DirAnalysis);
    //    DSDEBUG<<"分析图像路径:"<<m_DirAnalysis<<endl;
    //    QFileInfoList fileInfoList = dir.entryInfoList(strLst,QDir::Files);
    //    if(fileInfoList.size()==m_TimerImageNum) return;

    //    m_TimerImageNum=fileInfoList.size();

    loadAnaImageInfo(0);
    updateImgLstModel();

}

void DlgCheckOperator::on_pushButtonAnaDel_clicked()
{
    FileHelper::deleteDir(m_DirAnalysis);
    QStringList strLst;
    QDir dir(m_DirAnalysis);
    QFileInfoList fileInfoList = dir.entryInfoList(strLst,QDir::Files);

    loadAnaImageInfo(0);
    updateImgLstModel();
}

void DlgCheckOperator::on_PushBtnCO_DLObjectDetect_clicked()
{
    ImgTobaccoControl imgTabaccoControl(PackageChecker::getInstance()->ImgTobaccoAnalysis);

    //    list<CheckOperatorBasic*>::iterator itor= imgTabaccoControl.getImgPro(m_CurCameraNo)->LstCheckOperator.begin();
    //    for (;itor!=imgTabaccoControl.getImgPro(m_CurCameraNo)->LstCheckOperator.end();itor++)
    //    {
    //        if(((*itor)->CheckType)==ENUMCHECKOPERATORTYPE_DLOBJECTDETECT)
    //            return;
    //    }

    CheckOperatorBasic* pCheckOperator= imgTabaccoControl.addImgProCheckOperator(this->m_CurCameraNo,ENUMCHECKOPERATORTYPE_DLOBJECTDETECT);

    pCheckOperator->stu_CheckOperatorBasic->CamIdx=m_CurCameraNo;
    loadSceneSetRect(this->m_CurCameraNo);
    loadCheckOperatorSetModel(m_CurCameraNo);
    ui->tableViewCheckOperatorSet->selectRow(pCheckOperator->stu_CheckOperatorBasic->Index-1);
    ui->tableViewCheckOperatorSet->scrollToBottom();
    on_tableViewCheckOperatorSet_clicked(this->ui->tableViewCheckOperatorSet->model()->index(pCheckOperator->stu_CheckOperatorBasic->Index-1,0));

}
