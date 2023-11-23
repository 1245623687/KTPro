#include "dlgimagerecord.h"
#include "ui_dlgimagerecord.h"

#include"dsdebug.h"

#include"qtCommon/iconhelper.h"
#include<QDesktopWidget>
#include<QMouseEvent>
#include<QIcon>
#include"PackageChecker.h"
#include"kcontrols/kprogressbar.h"
#include"checkoperator/ImgTobaccoControl.h"
#include"checkoperator/ImgProControl.h"
#include"params/dssystemparam.h"
#include"qtCommon/frmmessagebox.h"
#include"common/filehelper.h"

#define DLGMAINRECORD_IMGWID 637
#define DLGMAINRECORD_IMGHEI 463

DlgImageRecord::DlgImageRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImageRecord)
{
    ui->setupUi(this);

    this->setWindowTitle("图像查询");
    this->m_ImagePath=PackageChecker::getInstance()->Options->ImgSavePath()+QString(tr("/图像保存/%1/NG")).arg(DSSystemParam::BrandName);

    InitStyle();
    InitControl();
}

void DlgImageRecord::on_changeShowResMode(bool b)
{
    m_showRes=b;
}

void DlgImageRecord::InitControl()
{
    ui->toolButtonDelImg->setVisible(false);
    ui->toolButtonDelFloder->setVisible(false);

    //combox
    for (int i=1;i<=PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.size();++i)
    {
        ui->comboBox->addItem(QString("相机%1").arg(i));
    }


    //显示结果
    connect(ui->slideButton,SIGNAL(enable(bool)),this,SLOT(on_changeShowResMode(bool)));

    //treeview
    //    ui->treeView->setIndentation(28);
    //    m_modelImage = new QStandardItemModel;
    //    ui->treeView->setHeaderHidden(true);	//设置隐藏表头
    //    ui->treeView->setModel(m_modelImage);
    //    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),this, SLOT(on_changeSelectedImage(QModelIndex)));
    //    //loadImageInfo(QDate::currentDate());

    //tableview
    m_modelImage=new QStandardItemModel;
    ui->tableView->setModel(m_modelImage);
    //  ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

#ifdef IMG_TYPE_GRAY
    m_pCalcImg=new unsigned char[PackageChecker::getInstance()->ImgHei*PackageChecker::getInstance()->ImgWid];
    memset(m_pCalcImg,0,PackageChecker::getInstance()->ImgHei*PackageChecker::getInstance()->ImgWid);
    m_Scene=new GraphicsSceneRec(PackageChecker::getInstance()->ImgWid,PackageChecker::getInstance()->ImgHei,m_pCalcImg);
#endif
#ifdef IMG_TYPE_RGB
    m_pCalcImg=new unsigned char[PackageChecker::getInstance()->ImgHei*PackageChecker::getInstance()->ImgWid*3];
    memset(m_pCalcImg,0,PackageChecker::getInstance()->ImgHei*PackageChecker::getInstance()->ImgWid*3);
    m_Scene=new GraphicsSceneRec(PackageChecker::getInstance()->ImgWid,PackageChecker::getInstance()->ImgHei,m_pCalcImg);
#endif

    this->ui->graphicsView->setScene(m_Scene);

    if((double)PackageChecker::getInstance()->ImgHei/PackageChecker::getInstance()->ImgWid>(double)DLGMAINRECORD_IMGHEI/DLGMAINRECORD_IMGWID)
    {

        this->ui->graphicsView->scaleto((double)DLGMAINRECORD_IMGHEI/(double)PackageChecker::getInstance()->ImgHei,
                                        (double)DLGMAINRECORD_IMGHEI/(double)PackageChecker::getInstance()->ImgHei);
        //        this->ui->graphicsView->setSceneRect(QRectF( -((double)DLGMAINRECORD_IMGWID-(double)PackageChecker::getInstance()->ImgWid)/2,
        //                                                    0,
        //                                                    DLGMAINRECORD_IMGWID,
        //                                                    PackageChecker::getInstance()->ImgHei));

        this->ui->graphicsView->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWid,(double)PackageChecker::getInstance()->ImgHei));

    }
    else
    {
        this->ui->graphicsView->scaleto((double)DLGMAINRECORD_IMGWID/PackageChecker::getInstance()->ImgWid,
                                        (double)DLGMAINRECORD_IMGWID/PackageChecker::getInstance()->ImgWid);

        //        this->ui->graphicsView->setSceneRect(QRectF(0,
        //                                                   -(DLGMAINRECORD_IMGHEI-PackageChecker::getInstance()->ImgHei)/2,
        //                                                   PackageChecker::getInstance()->ImgWid,
        //                                                   DLGMAINRECORD_IMGHEI));
        this->ui->graphicsView->setSceneRect(QRectF(0,0,(double)PackageChecker::getInstance()->ImgWid,(double)PackageChecker::getInstance()->ImgHei));

    }



    //    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    m_HandleInfo=new QStandardItemModel;
    //    //    QStandardItem* itemDetail1=new QStandardItem("处理信息1");
    //    //    QStandardItem* itemDetail2=new QStandardItem("处理信息2");
    //    //    QStandardItem* itemDetail3=new QStandardItem("处理信息3");
    //    //    QStandardItem* itemDetail4=new QStandardItem("处理信息4");
    //    //    m_HandleInfo->appendRow(itemDetail1);
    //    //    m_HandleInfo->appendRow(itemDetail2);
    //    //    m_HandleInfo->appendRow(itemDetail3);
    //    //    m_HandleInfo->appendRow(itemDetail4);
    //    this->ui->listView->setModel(m_HandleInfo);


    this->ui->dateEditChose->setDate(QDate::currentDate());


    m_ImagePath=QString(tr("%1/图像保存/%2/%3/%4")).arg(PackageChecker::getInstance()->Options->ImgSavePath()).arg(DSSystemParam::BrandName).arg("NG").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    loadImageInfo();
    // connect(ui->slideButton,SIGNAL(enable(bool)),m_Scene,SLOT(setResultImgEnable(bool)));

}



void DlgImageRecord::loadImageInfo()
{
    KProgressBar::start("正在加载图片...");
    KProgressBar::setValue(10);

    this->m_modelImage->clear();

    QStringList listmodelMain;
    listmodelMain<<"时间"<<"班次"<<"相机";
    m_modelImage->setHorizontalHeaderLabels(listmodelMain);
    ui->tableView->setColumnWidth(0,170);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(2,80);


    QMap<QString,QVector<ImageInfo>>::iterator itdel= m_ImageArray.begin();
    for (;itdel!= m_ImageArray.end();++itdel)
    {
        QVector<ImageInfo> pNullVector;
        itdel.value().swap(pNullVector);
    }

    m_ImageArray.clear();
    KProgressBar::setValue(50);

    QVector<QString> arrFiles;
    FileHelper::getAllImgFilesInFolder(m_ImagePath,arrFiles);

    QVector<ImageInfo> vImgInfo;
    for (int idx=0;idx<arrFiles.size();++idx)
    {
        QFileInfo fi(arrFiles[idx]);
        QString fileBaseName=fi.baseName();
        ImageInfo imageInfo(fileBaseName);
        if(m_CameraNo==0)
            vImgInfo.push_back(imageInfo);
        else
            if(imageInfo.m_camerano==m_CameraNo)
                vImgInfo.push_back(imageInfo);
    }

    for(int idx=0;idx<vImgInfo.size();++idx)
    {
        ImageInfo imageInfo=vImgInfo[idx];
        QStandardItem* itemDatetime=new QStandardItem(imageInfo.m_datetime.toString("hh:mm:ss.zzz"));
        itemDatetime->setTextAlignment(Qt::AlignCenter);
        QStandardItem* itemShift=new QStandardItem(getShiftName(imageInfo.m_shift));
        itemShift->setTextAlignment(Qt::AlignCenter);
        QStandardItem* itemCamera=new QStandardItem(QString::number(imageInfo.m_camerano));
        itemCamera->setTextAlignment(Qt::AlignCenter);
        QStandardItem* itemBaseName=new QStandardItem(imageInfo.m_baseFileName);

        m_modelImage->setItem(idx,0,itemDatetime);
        m_modelImage->setItem(idx,1,itemShift);
        m_modelImage->setItem(idx,2,itemCamera);
        m_modelImage->setItem(idx,3,itemBaseName);
    }

    ui->tableView->setColumnHidden(3,true);
    KProgressBar::setValue(100);


    //    QIcon ImgIcon(":/img.png");
    //    QVector<QString> folders;
    //    FileHelper::getAllFileFolderRegex(m_ImagePath,date.toString("yyyy-MM-dd"),folders);



    //qSort(folders.begin(),folders.end(),DlgImageRecord::compareFolder);


    //    for (int idx=0;idx<folders.size();++idx)
    //    {
    //        QVector<ImageInfo> vFolderImgs;
    //        QString folderPath=QString("%1/%2/").arg(m_ImagePath).arg(folders[idx]);

    //        QVector<QString> vImgPath;
    //        FileHelper::getAllImgFilesInFolder(folderPath,vImgPath);

    //        for (int idx=0;idx<vImgPath.size();idx++)
    //        {
    //            QFileInfo fInfo=vImgPath[idx];
    //            if(fInfo.suffix()!="bmp"&&fInfo.suffix()!="jpg") continue;
    //            QString fileBaseName=fInfo.baseName();
    //            ImageInfo imageInfo(fileBaseName);
    //            vFolderImgs.push_back(imageInfo);


    //        }
    //        qSort(vFolderImgs.begin(),vFolderImgs.end(),DlgImageRecord::ImageInfo::compare);
    //        m_ImageArray.insert(folders[idx],vFolderImgs);
    //    }

    //     KProgressBar::setValue(60);

    //    QMap<QString,QVector<ImageInfo>>::iterator itor= m_ImageArray.end();
    //    itor--;
    //    for (;itor!=-- m_ImageArray.begin();--itor)
    //    {
    //        QString tmpDate=itor.key().split('_')[0]+"_"+getShiftName(itor.key().split('_')[1].toInt());
    //        QStandardItem* itemFolder=new QStandardItem(tmpDate);
    //        itemFolder->setFont(QFont("黑体",14,QFont::Bold));

    //        QVector<ImageInfo>::iterator it=itor.value().begin();
    //        for (;it!=itor.value().end();++it)
    //        {

    //            QString tmpFileName=it->m_baseFileName.mid(9,2)+":"+
    //                    it->m_baseFileName.mid(11,2)+":"+
    //                    it->m_baseFileName.mid(13,2)+"."+
    //                    it->m_baseFileName.mid(16,3);
    //            QStandardItem* itemFile = new QStandardItem(ImgIcon,tmpFileName);
    //            itemFolder->appendRow(itemFile);
    //        }
    //        m_modelImage->appendRow(itemFolder);
    //    }

    //    KProgressBar::setValue(100);
}

void DlgImageRecord::on_changeSelectedImage(const QModelIndex &index)
{
    //    QModelIndex curItem = ui->treeView->currentIndex();
    //    curItem = curItem.sibling(curItem.row(),0);
    //    QString itemStr=m_modelImage->itemData(curItem).values()[0].toString();
    //    if(!itemStr.contains(':')) return;

    //    QStringList strLst=m_modelImage->itemData(curItem.parent()).values()[0].toString().split('_');
    //    QString folderName=strLst[0]+"_"+QString::number(getShiftId(strLst[1]));
    //    QString imgPath=m_ImagePath+"/"+folderName+"/"+m_ImageArray[folderName][curItem.row()].m_baseFileName+".bmp";



    //    ImgProControl control((*PackageChecker::getInstance()->ImgTobaccoAnalysis->LstImgPro.begin()));
    //    OpencvImage calcImg;
    //    calcImg.Format=ENUMDSIMAGEFORMAT_GRAY;
    //    QTextCodec *code = QTextCodec::codecForName("GB2312");
    //    std::string name = code->fromUnicode(imgPath.toStdString().c_str()).data();
    //    calcImg.ImageGray=cv::imread(name,0);





    //    for (int idxr=0;idxr<calcImg.ImageGray.rows;++idxr)
    //    {
    //        memcpy(m_pCalcImg+idxr*calcImg.ImageGray.cols,calcImg.ImageGray.ptr<uchar>(idxr),calcImg.ImageGray.cols);
    //    }


    //    control.setCalcImage(calcImg);
    //    QStringList debugInfo;
    //    cv::Mat defectImg;
    //    int res=control.debugCV(debugInfo,defectImg);
    //    int tmp_wid=calcImg.ImageGray.cols;
    //    int tmp_hei=calcImg.ImageGray.rows;
    //    //this->m_Scene->setResultImg(defectImg.ptr<uchar>(0));
    //    for (int idxr=0;idxr<tmp_hei;++idxr)
    //    {
    //        memset(this->m_pResImg+idxr*tmp_wid,0,tmp_wid);
    //    }



    //    for (int idxr=0;idxr<defectImg.rows;++idxr)
    //    {
    //        memcpy(this->m_pResImg+idxr*tmp_wid,defectImg.ptr<uchar>(idxr),tmp_wid);
    //    }
    //    //cv::imwrite("d:/defect.bmp",defectImg);
    //    this->m_Scene->updatethis();
    //    this->ui->graphicsView->update();
    //    QStringList  errlist;
    ////    errlist.append();


    //    updateErrInfo(errlist);
}

void DlgImageRecord::updateErrInfo(QStringList& errinfo)
{
    m_HandleInfo->clear();
    for (int i=0;i<errinfo.size();++i) {
        QStandardItem* itemDetail1=new QStandardItem("->"+errinfo[i]);
        itemDetail1->setFont( QFont( "微软雅黑", 10, QFont::Bold  ) );
        itemDetail1->setBackground(QBrush(QColor(232,138,136)));
        m_HandleInfo->appendRow(itemDetail1);
    }
    errinfo.clear();
}


void DlgImageRecord::InitStyle()
{
    this->ui->widget_4->setVisible(false);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
//    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf03e), 12);

}


bool DlgImageRecord::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void DlgImageRecord::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void DlgImageRecord::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void DlgImageRecord::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void DlgImageRecord::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void DlgImageRecord::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void DlgImageRecord::on_btnMenu_Close_clicked()
{
    this->close();
}

DlgImageRecord::~DlgImageRecord()
{
    if(m_Scene!=nullptr)
    {
        m_Scene->clear();
        delete m_Scene;
    }
    if(m_modelImage!=nullptr)
    {
        m_modelImage->clear();
        delete m_modelImage;
    }
    if(m_HandleInfo!=nullptr)
    {
        m_HandleInfo->clear();
        delete m_HandleInfo;
    }

    if(m_pCalcImg!=nullptr)
    {
        delete[] m_pCalcImg;
    }
    QMap<QString,QVector<ImageInfo>>::iterator itor= m_ImageArray.begin();
    for (;itor!= m_ImageArray.end();++itor)
    {
        itor.value().clear();
    }
    m_ImageArray.clear();

    delete ui;
}

void DlgImageRecord::on_toolButtonUpdate_clicked()
{
    loadImageInfo();
    on_tableView_clicked(m_modelImage->index(0,3));
}




void DlgImageRecord::on_dateEditChose_dateChanged(const QDate &date)
{
    QStringList strlst=m_ImagePath.split("/");
    strlst.removeLast();
    strlst.append(date.toString("yyyy-MM-dd"));
    QString newImagePath=strlst.join("/");


    if( this->m_ImagePath==newImagePath) return;
    this->m_ImagePath=newImagePath;
    loadImageInfo();
}

void DlgImageRecord::on_toolButtonShowAll_clicked()
{
    QString newImagePath=PackageChecker::getInstance()->Options->ImgSavePath()+QString(tr("/图像保存/%1/所有图像/%2")).arg(DSSystemParam::BrandName).arg(ui->dateEditChose->date().toString("yyyy-MM-dd"));
    if( this->m_ImagePath==newImagePath) return;
    this->m_ImagePath=newImagePath;
    loadImageInfo();
}



void DlgImageRecord::on_toolButtonShowNg_clicked()
{
    QString newImagePath=PackageChecker::getInstance()->Options->ImgSavePath()+QString(tr("/图像保存/%1/NG/%2")).arg(DSSystemParam::BrandName).arg(ui->dateEditChose->date().toString("yyyy-MM-dd"));
    if( this->m_ImagePath==newImagePath) return;
    this->m_ImagePath=newImagePath;
    loadImageInfo();
}

void DlgImageRecord::on_toolButtonShowInfo_clicked()
{
    if(m_showInfoMore)
    {
        ui->listView->setVisible(false);
        ui->toolButtonShowInfo->setStyleSheet("QToolButton#toolButtonShowInfo{border-image:url(:/更多normal.png); background:transparent; border-style:none;}"
                                              "QToolButton#toolButtonShowInfo:hover{border-image:url(:/更多hover.png); background:transparent;border-style:none;}"
                                              "QToolButton#toolButtonShowInfo:pressed{border-image:url(:/更多pressed.png); background:transparent;border-style:none;}");
    }
    else
    {
        ui->listView->setVisible(true);
        ui->toolButtonShowInfo->setStyleSheet("QToolButton#toolButtonShowInfo{border-image:url(:/下箭头normal.png); background:transparent; border-style:none;}"
                                              "QToolButton#toolButtonShowInfo:hover{border-image:url(:/下箭头hover.png); background:transparent;border-style:none;}"
                                              "QToolButton#toolButtonShowInfo:pressed{border-image:url(:/下箭头pressed.png); background:transparent;border-style:none;}");
    }
    m_showInfoMore=!m_showInfoMore;
}

void DlgImageRecord::on_comboBox_currentIndexChanged(int index)
{
    m_CameraNo=index;
    loadImageInfo();
}

void DlgImageRecord::on_toolButtonDelImg_clicked()
{
    int  curRow=ui->tableView->currentIndex().row();
    if(curRow==-1)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("请选择需要删除的图片!", 0);
        msg->exec();
        return;
    }
    //    DSDEBUG<<"row:"<<curRow<<"   FILENAME:"<<m_modelImage->data(m_modelImage->index(curRow,3));
    QString delName=m_modelImage->data(m_modelImage->index(curRow,3)).toString();
    QString delBmpPath=QString("%1/%2.bmp").arg(m_ImagePath).arg(delName);
    DSDEBUG<<delBmpPath;

    //删除tableview
    if((curRow+1)==m_modelImage->rowCount())
    {
        m_modelImage->removeRows(curRow,1);
        ui->tableView->selectRow(curRow-1);
        on_tableView_clicked(m_modelImage->index(curRow-1,3));
    }
    else
    {
        m_modelImage->removeRows(curRow,1);
        ui->tableView->selectRow(curRow);
        on_tableView_clicked(m_modelImage->index(curRow,3));
    }
    //删除图片
    QFile fileTemp(delBmpPath);
    fileTemp.remove();




}

void DlgImageRecord::on_toolButtonDelFloder_clicked()
{

    QDir dir;
    dir.setPath(m_ImagePath );
    dir.removeRecursively();
    loadImageInfo();
}

void DlgImageRecord::on_tableView_clicked(const QModelIndex &index)
{
    int  curRow=ui->tableView->currentIndex().row();
    if(curRow==-1)
    {
        return;
    }
    //    DSDEBUG<<"row:"<<curRow<<"   FILENAME:"<<m_modelImage->data(m_modelImage->index(curRow,3));

    QString bmpPath=QString("%1/%2.bmp").arg(m_ImagePath).arg(m_modelImage->data(m_modelImage->index(curRow,3)).toString());
    OpencvImage calcImg;
    calcImg.Format=ENUMDSIMAGEFORMAT_GRAY;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(bmpPath.toStdString().c_str()).data();

#ifdef IMG_TYPE_GRAY
    calcImg.ImageGray=cv::imread(name,0);
    calcImg.Width=calcImg.ImageGray.cols;
    calcImg.Height=calcImg.ImageGray.rows;
    calcImg.Format=ENUMDSIMAGEFORMAT_GRAY;
    for (int idxr=0;idxr<calcImg.ImageGray.rows;++idxr)
    {
        memcpy(m_pCalcImg+idxr*calcImg.ImageGray.cols,calcImg.ImageGray.ptr<uchar>(idxr),calcImg.ImageGray.cols);
    }

#endif
#ifdef IMG_TYPE_RGB
    calcImg.ImageRGB=cv::imread(name);
    cvtColor(calcImg.ImageRGB, calcImg.ImageRGB, cv::COLOR_RGB2BGR);
    calcImg.Width=calcImg.ImageRGB.cols;
    calcImg.Height=calcImg.ImageRGB.rows;
    calcImg.Format=ENUMDSIMAGEFORMAT_RGB;
    for (int idxr=0;idxr<calcImg.ImageRGB.rows;++idxr)
    {
        memcpy(m_pCalcImg+idxr*calcImg.ImageRGB.cols*3,calcImg.ImageRGB.ptr<uchar>(idxr),calcImg.ImageRGB.cols*3);
    }

#endif





    this->m_Scene->updatethis();
    this->ui->graphicsView->update();

}

void DlgImageRecord::on_toolButtonRecMin_clicked()
{
    this->ui->graphicsView->scaleMin();
}

void DlgImageRecord::on_toolButtonRecMax_clicked()
{
    this->ui->graphicsView->scaleMax();
}

void DlgImageRecord::on_toolButtonRecInit_clicked()
{
    this->ui->graphicsView->scaleInit();
}
