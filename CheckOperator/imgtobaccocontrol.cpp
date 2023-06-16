#include "imgtobaccocontrol.h"
#include<QDateTime>
#include<QFileInfo>
#include"packagechecker.h"
#include "glog/logging.h"

ImgTobaccoControl::ImgTobaccoControl(ImgTobacco* clsImgTabacco) :ClsImgTabacco(clsImgTabacco)
{
}

//************************************
// Method:    从xmlElement中加载数据
// FullName:  ImgTabaccoControl::LoadXml
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: XMLElement * pEleParent
//************************************
int ImgTobaccoControl::LoadXml(XMLElement* pEleParent)
{
    //QString sss=pEleParent->Name();
    string temp = "";
    XmlHelper::GetElementTextFromParent("ProductName", pEleParent, temp);
    //        DSDEBUG<<QString::fromStdString(temp)<<endl;
    this->ClsImgTabacco->ProductName = temp;

    XmlHelper::GetElementTextFromParent("Code", pEleParent, temp);
    this->ClsImgTabacco->Code = temp;
    XmlHelper::GetElementTextFromParent("ImgProObjNum", pEleParent, temp);
    this->ClsImgTabacco->ImgProObjNum = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("RevertEnable", pEleParent, temp);
    this->ClsImgTabacco->RevertEnable = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("PatternsSets", pEleParent, temp);
    this->ClsImgTabacco->PatternsSets = atoi(temp.c_str());

//    XmlHelper::GetElementTextFromParent("ImgWid", pEleParent, temp);
//    this->ClsImgTabacco->ImgWid = atoi(temp.c_str());

//    XmlHelper::GetElementTextFromParent("ImgHei", pEleParent, temp);
//    this->ClsImgTabacco->ImgHei = atoi(temp.c_str());


    for(int i=0;i<this->ClsImgTabacco->ImgProObjNum;i++)
    {
        QString nodeName=QString("ImgWid%1").arg(i+1);
        XmlHelper::GetElementTextFromParent(nodeName.toStdString(), pEleParent, temp);
        this->ClsImgTabacco->ImgWids[i] = atoi(temp.c_str());

        nodeName=QString("ImgHei%1").arg(i+1);
        XmlHelper::GetElementTextFromParent(nodeName.toStdString(), pEleParent, temp);
        this->ClsImgTabacco->ImgHeis[i] = atoi(temp.c_str());

    }


    vector<XMLElement*> vEle;
    XMLElement* eleTemp = pEleParent->FirstChildElement();
    while (eleTemp != NULL)
    {
        string s = eleTemp->Name();
        if (string(eleTemp->Name()) == "ImgPro")
        {
            vEle.push_back(eleTemp);
        }
        eleTemp = eleTemp->NextSiblingElement();
    }

    this->ClsImgTabacco->LstImgPro.clear();
    for (int idx = 0; idx < vEle.size(); idx++)
    {
        ImgPro* imgPro = new ImgPro;
        ImgProControl imgProControl(imgPro);
        imgProControl.LoadXml(vEle[idx]);
        this->ClsImgTabacco->LstImgPro.push_back(imgPro);
    }

    return 0;
}


void ImgTobaccoControl::saveTemplatestoFile(QString dirBrand)
{


    for(std::list<ImgPro*>::iterator itor1= this->ClsImgTabacco->LstImgPro.begin();itor1!=this->ClsImgTabacco->LstImgPro.end();itor1++)
    {
        for(std::map<QString,std::map<int,OpencvImage>>::iterator itor2=(*itor1)->TemplateImage.begin();itor2!=(*itor1)->TemplateImage.end();itor2++)
        {
            std::map<int,OpencvImage>::iterator itor3=itor2->second.begin();
            for(;itor3!=itor2->second.end();++itor3)
            {
                QString savePath=dirBrand+"/ImgPro"+
                        QString::number((*itor1)->ImgIndex)+"/"+itor2->first+"/template"+QString::number(itor3->first)+".bmp";
                FileHelper::makeDir(dirBrand+"/ImgPro"+
                                    QString::number((*itor1)->ImgIndex)+"/"+itor2->first);
                //DSDEBUG<<savePath<<endl;
                itor3->second.save(savePath);
            }
        }
    }
}


//************************************
// Method:    生成xmlElement块
// FullName:  ImgTabaccoControl::generateXmlElementBlock
// Access:    virtual public
// Returns:   tinyxml2::XMLElement *
// Qualifier:
// Parameter: XMLDocument & doc
//************************************
XMLElement * ImgTobaccoControl::generateXmlElementBlock(TinyXmlDocument & doc)
{
    XMLElement* eleTabacco = doc.NewElement("Tobacco");

    XMLElement* eleProductName = doc.NewElement("ProductName");
    eleProductName->SetText(this->ClsImgTabacco->ProductName.c_str());

    XMLElement* eleCode = doc.NewElement("Code");
    eleCode->SetText(this->ClsImgTabacco->Code.c_str());

    XMLElement* eleImgProObjNum = doc.NewElement("ImgProObjNum");
    eleImgProObjNum->SetText(this->ClsImgTabacco->ImgProObjNum);

    XMLElement* eleRevertEnable = doc.NewElement("RevertEnable");
    eleRevertEnable->SetText(this->ClsImgTabacco->RevertEnable?1:0);

    XMLElement* elePatternsSets = doc.NewElement("PatternsSets");
    elePatternsSets->SetText(this->ClsImgTabacco->PatternsSets);

//    XMLElement* eleImgWid = doc.NewElement("ImgWid");
//    eleImgWid->SetText(this->ClsImgTabacco->ImgWid);

//    XMLElement* eleImgHei = doc.NewElement("ImgHei");
//    eleImgHei->SetText(this->ClsImgTabacco->ImgHei);



    eleTabacco->InsertEndChild(eleProductName);
    eleTabacco->InsertEndChild(eleCode);
    eleTabacco->InsertEndChild(eleImgProObjNum);
    eleTabacco->InsertEndChild(eleRevertEnable);
    eleTabacco->InsertEndChild(elePatternsSets);
//    eleTabacco->InsertEndChild(eleImgWid);
//    eleTabacco->InsertEndChild(eleImgHei);

    for(int i=0;i<this->ClsImgTabacco->ImgProObjNum;i++)
    {
         QString nodeName=QString("ImgWid%1").arg(i+1);
        XMLElement* eleImgWid = doc.NewElement(nodeName.toStdString().c_str());
        eleImgWid->SetText(this->ClsImgTabacco->ImgWids[i]);

        nodeName=QString("ImgHei%1").arg(i+1);
        XMLElement* eleImgHei = doc.NewElement(nodeName.toStdString().c_str());
        eleImgHei->SetText(this->ClsImgTabacco->ImgHeis[i]);

        eleTabacco->InsertEndChild(eleImgWid);
        eleTabacco->InsertEndChild(eleImgHei);
    }


    list<ImgPro*>::iterator it=ClsImgTabacco->LstImgPro.begin();
    for (; it!= ClsImgTabacco->LstImgPro.end(); it++)
    {
        ImgProControl * imgProControl = new ImgProControl((*it));
        XMLElement* eleImgProAdd = imgProControl->generateXmlElementBlock(doc);
        int iidex=eleImgProAdd->IntAttribute("Index");

        delete imgProControl;
        eleTabacco->InsertEndChild(eleImgProAdd);
    }
    doc.InsertFirstChild(eleTabacco);
    return eleTabacco;
}

int ImgTobaccoControl::generateInitXmlFile(QString xmlPath,QString brandName,int* imgHei,int* imgWid,int cameraNum)
{
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string tmpName = code->fromUnicode(xmlPath).data();

    //const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    TinyXmlDocument doc;
    doc.Parse(declaration.c_str());

    XMLElement* eleTabacco=doc.NewElement("Tobacco");

    XMLElement* eleProductName = doc.NewElement("ProductName");
    eleProductName->SetText(brandName.toStdString().c_str());

    XMLElement* eleCode = doc.NewElement("Code");
    eleCode->SetText(QDateTime::currentDateTime().toString("yyMMddhhmmss").toStdString().c_str());

    XMLElement* eleImgProObjNum = doc.NewElement("ImgProObjNum");
    eleImgProObjNum->SetText(cameraNum);
    XMLElement* eleRevertEnable = doc.NewElement("RevertEnable");
    eleRevertEnable->SetText(0);
    XMLElement* elePatternsSets = doc.NewElement("PatternsSets");
    elePatternsSets->SetText(0);

//    XMLElement* eleImgHei = doc.NewElement("ImgHei");
//    eleImgHei->SetText(imgHei);
//    XMLElement* eleImgWid = doc.NewElement("ImgWid");
//    eleImgWid->SetText(imgWid);

    eleTabacco->InsertEndChild(eleProductName);
    eleTabacco->InsertEndChild(eleCode);
    eleTabacco->InsertEndChild(eleImgProObjNum);
    eleTabacco->InsertEndChild(eleRevertEnable);
    eleTabacco->InsertEndChild(elePatternsSets);


//    eleTabacco->InsertEndChild(eleImgHei);
//    eleTabacco->InsertEndChild(eleImgWid);

    for(int i=1;i<=cameraNum;i++)
    {
        QString nodeName=QString("ImgHei%1").arg(i);
        XMLElement* eleImgHei = doc.NewElement(nodeName.toStdString().c_str());
        eleImgHei->SetText(imgHei[i-1]);

        nodeName=QString("ImgWid%1").arg(i);
        XMLElement* eleImgWid = doc.NewElement(nodeName.toStdString().c_str());
        eleImgWid->SetText(imgWid[i-1]);

        eleTabacco->InsertEndChild(eleImgHei);
        eleTabacco->InsertEndChild(eleImgWid);
    }


    for(int i=1;i<=cameraNum;i++)
    {
        XMLElement *ele= ImgProControl::generateInitXmlElementBlock(doc,i);
        eleTabacco->InsertEndChild(ele);
    }

    doc.InsertEndChild(eleTabacco);

    XMLError xe= doc.SaveFile(tmpName.c_str());
    if(xe!=XML_SUCCESS) return -1;
    return  0;
}

//************************************
// Method:    获取当前相机数量
// FullName:  ImgTabaccoControl::getImgProNum
// Access:    public
// Returns:   int
// Qualifier:
//************************************
int ImgTobaccoControl::getImgProNum()
{
    return this->ClsImgTabacco->ImgProObjNum;
}

//修改子节点内容
void ImgTobaccoControl::updateTabaccoNode(XMLElement* eleParent,string eleName, string text)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(text.c_str());
}
void ImgTobaccoControl::updateTabaccoNode(XMLElement* eleParent, string eleName, int inttext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(inttext).c_str());
}
void ImgTobaccoControl::updateTabaccoNode(XMLElement* eleParent, string eleName, bool btext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(btext ? "1" : "0");
}
void ImgTobaccoControl::updateTabaccoNode(XMLElement* eleParent, string eleName, double dtext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(dtext).c_str());
}
void ImgTobaccoControl::updateTabaccoNode(XMLElement* eleParent, string eleName, float ftext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(ftext).c_str());
}


//************************************
// Method:    添加相机
// FullName:  ImgTabaccoControl::insertImgPro
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: ImgPro * imgPro：要添加的相机
// Parameter: XMLDocument & doc
//************************************
void ImgTobaccoControl::addImgPro(ImgPro * imgPro,XMLElement* EleParent,TinyXmlDocument& doc)
{
    this->ClsImgTabacco->ImgProObjNum++;
    EleParent->FirstChildElement("ImgProObjNum")->SetText(this->ClsImgTabacco->ImgProObjNum);

    imgPro->ImgIndex = this->ClsImgTabacco->ImgProObjNum;
    ImgProControl imgProControl(imgPro);
    XMLElement * eleImgPro=imgProControl.generateXmlElementBlock(doc);

    this->ClsImgTabacco->LstImgPro.push_back(imgPro);
    EleParent->InsertEndChild(eleImgPro);
}


//************************************
// Method:    根据编号获取相机
// FullName:  ImgTabaccoControl::getImgPro
// Access:    public
// Returns:   ImgPro*
// Qualifier:
// Parameter: int index：相机编号
//************************************
ImgPro* ImgTobaccoControl::getImgPro(int index)
{
    if (index>this->ClsImgTabacco->ImgProObjNum)
    {
        return NULL;
    }
    list<ImgPro*>::iterator reit;
    int idx = 1;
    for (list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin(); it != this->ClsImgTabacco->LstImgPro.end(); it++, idx++)
    {
        if (idx == index)
        {
            reit = it;
        }
    }
    return *reit;
}



//************************************
// Method:    获取指定相机的指定算子
// FullName:  ImgTabaccoControl::getImgProCheckOperator
// Access:    public
// Returns:   CheckOperatorBasic*
// Qualifier:
// Parameter: int cameraNo:相机编号
// Parameter: int CheckNo:算子编号
//************************************
CheckOperatorBasic* ImgTobaccoControl::getImgProCheckOperator(int cameraNo,int CheckNo)
{
    ImgPro * imgPro = getImgPro(cameraNo);
    if (imgPro==NULL)
    {
        return NULL;
    }
    ImgProControl* imgProControl = new ImgProControl(imgPro);
    CheckOperatorBasic* ret= imgProControl->getCheckNo(CheckNo);
    delete  imgProControl;
    return ret;
}


int ImgTobaccoControl::addImgProCheckOperator(int cameraNo,CheckOperatorBasic* pCheckOperator)
{
    ImgPro * imgPro=getImgPro(cameraNo);
    if (imgPro==NULL)
    {
        return -1;
    }
    ImgProControl imgProControl(imgPro);
    imgProControl.insertCheckOperator(pCheckOperator);
    return 0;
}

CheckOperatorBasic* ImgTobaccoControl::addImgProCheckOperator(int cameraNo,ENUMCHECKOPERATORTYPE type)
{
    ImgPro * imgPro=getImgPro(cameraNo);
    if (imgPro==NULL)
    {
        return NULL;
    }
    ImgProControl imgProControl(imgPro);
    return  imgProControl.insertCheckOperator(type);

}




//************************************
// Method:    删除CameraNo下面的对应算子
// FullName:  ImgTabaccoControl::deleteImgProCheckOperator
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: int cameraNo
// Parameter: int delCheckNo
//************************************
int ImgTobaccoControl::deleteImgProCheckOperator(int cameraNo, int delCheckNo)
{
    ImgPro * imgPro=getImgPro(cameraNo);
    if (imgPro==NULL)
    {
        return -1;
    }
    ImgProControl * imgProControl = new ImgProControl(imgPro);


    int ret= imgProControl->deleteCheckOperator(delCheckNo);
    delete  imgProControl;
    return ret;
}


//************************************
// Method:    修改,先删除,再添加
// FullName:  ImgTabaccoControl::updataXmlDocument
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: XMLElement * eleTabcco
// Parameter: XMLDocument & doc
//************************************
int ImgTobaccoControl::updataXmlDocument(XMLElement* eleTabcco,TinyXmlDocument & doc)
{
    doc.DeleteNode(eleTabcco);
    XMLElement* eleTabccoNew = generateXmlElementBlock(doc);
    if (eleTabccoNew==NULL)
    {
        return -1;
    }
    doc.InsertEndChild(eleTabccoNew);
    return 0;
}



void ImgTobaccoControl::setRefImage(vector<OpencvImage> & vOpenCvImage)
{

    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    int idx = 0;
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++, idx++)
    {
        ImgProControl control(*it);
        control.setRefImage(vOpenCvImage[idx]);

    }
}


void ImgTobaccoControl::setCalcImage(int idx,OpencvImage &openCvImage)
{
    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++)
    {
        if((*it)->ImgIndex==idx)
        {
            ImgProControl control(*it);
            control.setCalcImage(openCvImage);
        }
    }
}


void ImgTobaccoControl::setCalcImage(vector<OpencvImage> & vOpenCvImage)
{

    //可以放在线程中给计算图赋值
    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    int idx = 0;
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++, idx++)
    {
        ImgProControl control(*it);
        control.setCalcImage(vOpenCvImage[idx]);
    }
}

void ImgTobaccoControl::getCalcImagge(int idx, OpencvImage &openCvImage)
{
    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++)
    {
        if((*it)->ImgIndex==idx)
        {
            ImgProControl control(*it);
            control.getCalcImage(openCvImage);
        }
    }
}

void ImgTobaccoControl::getCurBadImage(int idx, OpencvImage &openCvImage)
{
    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++)
    {
        if((*it)->ImgIndex==idx)
        {
            ImgProControl control(*it);
            control.getCurBadImage(openCvImage);
        }
    }
}

int ImgTobaccoControl::getBoundingBoxall(QRect allRect[CAMERANUM_MAX][CHECKOPERATORNUM_MAX])
{
    ImgTobaccoControl imgTabaccoControl(this->ClsImgTabacco);
    list<ImgPro*>::iterator itor=imgTabaccoControl.ClsImgTabacco->LstImgPro.begin();
    for(;itor!=imgTabaccoControl.ClsImgTabacco->LstImgPro.end();itor++)
    {
        int cameraNo=(*itor)->ImgIndex;

        list<CheckOperatorBasic*>::iterator _it=(*itor)->LstCheckOperator.begin();
        for(;_it!=(*itor)->LstCheckOperator.end();_it++)
        {
            int checkNo=(*_it)->stu_CheckOperatorBasic->Index;
            allRect[cameraNo][checkNo].setX((*_it)->stu_CheckOperatorBasic->Basic_RectValid_Xs);
            allRect[cameraNo][checkNo].setY((*_it)->stu_CheckOperatorBasic->Basic_RectValid_Ys);
            allRect[cameraNo][checkNo].setWidth((*_it)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*_it)->stu_CheckOperatorBasic->Basic_RectValid_Xs);
            allRect[cameraNo][checkNo].setHeight((*_it)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*_it)->stu_CheckOperatorBasic->Basic_RectValid_Ys);
        }
    }
    return  0;
}


int ImgTobaccoControl::calculate(int imgIdx,unsigned char errMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX])
{


    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    int ret=0;
    //每个相机分别计算，放在线程中
    for ( ;it!= this->ClsImgTabacco->LstImgPro.end();it++)
    {
        if((*it)->ImgIndex!=imgIdx) continue;

        //LOG(INFO)<<"calculate开始执行计算  camera:"<<imgIdx;

        DSDEBUG<<"calculate开始执行计算  camera:"<<imgIdx<<endl;
        ImgProControl* imgProControl = new ImgProControl(*it);
        ret+= (imgProControl->calculateCV(errMatrix[(*it)->ImgIndex]));

       // LOG(INFO)<<"相机:"<<imgIdx<<"结束执行calculate,累计错误数量:"<<ret;
        DSDEBUG<<"相机:"<<imgIdx<<"结束执行calculate,累计错误数量:"<<ret;
        delete imgProControl;
    }
    return ret;
}

int ImgTobaccoControl::calculate(unsigned char errMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX])
{
    //LOG(INFO)<<"calculate开始执行计算";
    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    int ret=0;
    //每个相机分别计算，放在线程中
    for ( ;it!= this->ClsImgTabacco->LstImgPro.end();it++)
    {
       // LOG(INFO)<<"相机:"<<(*it)->ImgIndex<<"开始执行calculate";

        ImgProControl* imgProControl = new ImgProControl(*it);
        ret+= (imgProControl->calculateCV(errMatrix[(*it)->ImgIndex]));

        //LOG(INFO)<<"相机:"<<(*it)->ImgIndex<<"结束执行calculate,累计错误数量:"<<ret;
        delete imgProControl;
    }
    return ret;
}

void ImgTobaccoControl::loadRefImage(std::string dirPath)
{

    std::list<ImgPro*>::iterator itor= this->ClsImgTabacco->LstImgPro.begin();
    for(;itor!=this->ClsImgTabacco->LstImgPro.end();++itor)
    {
        std::string imgPath=dirPath+"/ImgPro"+to_string((*itor)->ImgIndex)+".bmp";

        // bool isExist=true;
        //        this->ClsImgTabacco->ExistRefImage=1;

        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(imgPath.c_str()).data();


#ifdef IMG_TYPE_GRAY
        (*itor)->RefOpencvImage.Format=ENUMDSIMAGEFORMAT_GRAY;
        (*itor)->RefOpencvImage.ImageGray=cv::imread(name,0);

        (*itor)->RefOpencvImage.Height= (*itor)->RefOpencvImage.ImageGray.rows;
        (*itor)->RefOpencvImage.Width= (*itor)->RefOpencvImage.ImageGray.cols;
#endif
#ifdef IMG_TYPE_RGB
        (*itor)->RefOpencvImage.Format=ENUMDSIMAGEFORMAT_RGB;
        (*itor)->RefOpencvImage.ImageRGB=cv::imread(name);

        (*itor)->RefOpencvImage.Height= (*itor)->RefOpencvImage.ImageRGB.rows;
        (*itor)->RefOpencvImage.Width= (*itor)->RefOpencvImage.ImageRGB.cols;
#endif

    }
}



void ImgTobaccoControl::loadConfig(QString dirBrand)
{
    TinyXmlDocument doc;
    QString  xmlConfigPath=dirBrand+"/config.xml";

    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string tmpName = code->fromUnicode(xmlConfigPath).data();

    XMLError ret= doc.LoadFile(tmpName.c_str());

    //DSDEBUG<<"xmlConfigPath:"<<xmlConfigPath<<endl;

    XMLElement * eleTabacco = doc.RootElement();
    //string str = eleTabacco->Name();
    this->LoadXml(eleTabacco);
}



void ImgTobaccoControl::loadRefImageFromDir(QString dirBrand)
{

    for(int idx=1;idx<=this->ClsImgTabacco->ImgProObjNum;idx++)
    {
        OpencvImage img;
#ifdef IMG_TYPE_GRAY
        img.Format=ENUMDSIMAGEFORMAT_GRAY;
#endif
#ifdef IMG_TYPE_RGB
        img.Format=ENUMDSIMAGEFORMAT_RGB;

#endif

        QString imgPath=dirBrand+"/ImgPro"+QString::number(idx)+".bmp";
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(imgPath).data();
        QFileInfo fi(imgPath);

//        img.Width=PackageChecker::getInstance()->ImgWid;
//        img.Height=PackageChecker::getInstance()->ImgHei;

        img.Width=PackageChecker::getInstance()->ImgWids[idx-1];
        img.Height=PackageChecker::getInstance()->ImgHeis[idx-1];

        this->ClsImgTabacco->PatternsSets=1;
        if(fi.exists())
        {
#ifdef IMG_TYPE_GRAY
            img.ImageGray=cv::imread(name,0);

#endif
#ifdef IMG_TYPE_RGB
            img.ImageRGB=cv::imread(name);

#endif

        }
        else
        {
#ifdef IMG_TYPE_GRAY
            img.ImageGray= cv::Mat::zeros(img.Height,img.Width,CV_8UC1);
#endif
#ifdef IMG_TYPE_RGB
            img.ImageRGB= cv::Mat::zeros(img.Height,img.Width,CV_8UC3);
#endif

            this->ClsImgTabacco->PatternsSets=0;//当作没有图片使用
        }
        //        img.Width=img.ImageGray.cols;
        //        img.Height=img.ImageGray.rows;

        ImgTobaccoControl control(this->ClsImgTabacco);
        ImgPro* imgPro=control.getImgPro(idx);
        ImgProControl imgProControl(imgPro);
        imgProControl.setRefImage(img);
        //this->ClsImgTabacco->RefImage.insert(std::pair<int,OpenCvImage>(idx,img));
    }
}

void ImgTobaccoControl::loadRefImageFromMemory(std::map<int,OpencvImage>& mapImage)
{
    //    this->ClsImgTabacco->RefImage.clear();
    //    std::map<int,OpenCvImage>::iterator itor=mapImage.begin();
    //    for(;itor!=mapImage.end();++itor)
    //    {
    //        this->ClsImgTabacco->RefImage[itor->first]=itor->second;
    //    }


    list<ImgPro*>::iterator it = this->ClsImgTabacco->LstImgPro.begin();
    for (; it != this->ClsImgTabacco->LstImgPro.end(); it++)
    {
        ImgProControl control(*it);
        control.setRefImage(mapImage[(*it)->ImgIndex]);
    }
}



//void ImgTobaccoControl::loadTemplate(QString dirBrand)
//{
//    //先清除所有
//    for(std::map<int,std::map<int,std::map<int,OpenCvImage>>>::iterator itor1=this->ClsImgTabacco->m_Template.begin();
//        itor1!=this->ClsImgTabacco->m_Template.end();itor1++)
//    {
//        for(std::map<int,std::map<int,OpenCvImage>>::iterator itor2=itor1->second.begin();itor2!=itor1->second.end();itor2++)
//        {
//            this->ClsImgTabacco->m_Template[itor1->first][itor2->first].clear();
//        }
//        this->ClsImgTabacco->m_Template[itor1->first].clear();
//    }
//    this->ClsImgTabacco->m_Template.clear();


//    for(list<ImgPro*>::iterator itor1=this->ClsImgTabacco->LstImgPro.begin();itor1!=this->ClsImgTabacco->LstImgPro.end();itor1++)
//    {

//        for(list<CheckOperatorBasic*>::iterator itor2=(*itor1)->LstCheckOperator.begin();itor2!=(*itor1)->LstCheckOperator.end();itor2++)
//        {
//            if((*itor2)->CheckType==ENUMCHECKOPERATORTYPE_TEMPLATEMATCH)
//            {
//                CheckOperatorTemplateMatch * checkOperator=static_cast<CheckOperatorTemplateMatch*>((*itor2));
//                for(int idx=0;idx<checkOperator->SearchNum;idx++)
//                {
//                    OpenCvImage img;
//                    img.Format=ENUMDSIMAGEFORMAT_GRAY;
//                    QString imgPath=dirBrand+"/ImgPro"+QString::number((*itor1)->ImgIndex)+
//                            "/CheckOperator"+QString::number(checkOperator->stu_CheckOperatorBasic->Index)+"/template"+QString::number(idx)+".bmp";
//                    QTextCodec *code = QTextCodec::codecForName("GB2312");
//                    std::string name = code->fromUnicode(imgPath).data();

//                    img.ImageGray=cv::imread(name,0);
//                    img.Width=img.ImageGray.cols;
//                    img.Height=img.ImageGray.rows;
//                    this->ClsImgTabacco->m_Template[(*itor1)->ImgIndex][(*itor2)->stu_CheckOperatorBasic->Index].insert(std::pair<int,OpenCvImage>(idx,img));
//                }

//            }
//        }
//    }
//}


void ImgTobaccoControl::resetforNewBrand()
{

    for(std::list<ImgPro*>::iterator itor1= this->ClsImgTabacco->LstImgPro.begin();itor1!=this->ClsImgTabacco->LstImgPro.end();++itor1)
    {
        list<CheckOperatorBasic*>::iterator itor2=(*itor1)->LstCheckOperator.begin();
        for(;itor2!=(*itor1)->LstCheckOperator.end();++itor2)
        {
            switch ((*itor2)->CheckType)
            {
            case  ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            {
                CheckOperatorTemplateMatch * pCheckOperator=static_cast<CheckOperatorTemplateMatch*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;
                pCheckOperator->MatchThresh=60;
                pCheckOperator->SearchNum=0;

            }
                break;
            case  ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
            {
                CheckOperatorEdgePositioning * pCheckOperator=static_cast<CheckOperatorEdgePositioning*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }

                break;
            case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
            {
                //                CheckOperatorDefectInspect * pCheckOperator=static_cast<CheckOperatorDefectInspect*>((*itor2));
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
            {
                CheckOperatorBrokenlineInspect * pCheckOperator=static_cast<CheckOperatorBrokenlineInspect*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
            {
                //                CheckOperatorGrayMeasurement * pCheckOperator=static_cast<CheckOperatorGrayMeasurement*>((*itor2));
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_LINEINSPECT:
            {
                //                CheckOperatorLineInspect * pCheckOperator=static_cast<CheckOperatorLineInspect*>((*itor2));
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                //                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
            {
                CheckOperatorAssociatedInspect * pCheckOperator=static_cast<CheckOperatorAssociatedInspect*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_STAYINSPECT:
            {
                CheckOperatorStayInspect * pCheckOperator=static_cast<CheckOperatorStayInspect*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            case  ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
            {
                CheckOperatorDoubleStayInspect * pCheckOperator=static_cast<CheckOperatorDoubleStayInspect*>((*itor2));
                pCheckOperator->stu_CheckOperatorBasic->Basic_Vaild=false;
                pCheckOperator->stu_CheckOperatorBasic->Basic_IsCheck=false;


            }
                break;
            }
        }
    }

}






//清空后重新添加
void ImgTobaccoControl::loadTemplate(QString dirBrand)
{

    for(std::list<ImgPro*>::iterator itor1= this->ClsImgTabacco->LstImgPro.begin();itor1!=this->ClsImgTabacco->LstImgPro.end();itor1++)
    {
        for(std::map<QString,std::map<int,OpencvImage>>::iterator itor2=(*itor1)->TemplateImage.begin();itor2!=(*itor1)->TemplateImage.end();itor2++)
        {
            itor2->second.clear();
        }
        (*itor1)->TemplateImage.clear();
    }


    for(std::list<ImgPro*>::iterator itor1= this->ClsImgTabacco->LstImgPro.begin();itor1!=this->ClsImgTabacco->LstImgPro.end();itor1++)
    {
        for(list<CheckOperatorBasic*>::iterator itor2=(*itor1)->LstCheckOperator.begin();itor2!=(*itor1)->LstCheckOperator.end();itor2++)
        {
            if((*itor2)->CheckType==ENUMCHECKOPERATORTYPE_TEMPLATEMATCH)
            {
                CheckOperatorTemplateMatch * checkOperator=static_cast<CheckOperatorTemplateMatch*>((*itor2));

                for(int idx=0;idx<checkOperator->SearchNum;idx++)
                {
                    OpencvImage img;

#ifdef IMG_TYPE_GRAY
                    img.Format=ENUMDSIMAGEFORMAT_GRAY;
#endif
#ifdef IMG_TYPE_RGB
                    img.Format=ENUMDSIMAGEFORMAT_RGB;
#endif
                    QString imgPath=dirBrand+"/ImgPro"+QString::number((*itor1)->ImgIndex)+
                            "/"+QString::fromStdString(
                                static_cast<CheckOperatorTemplateMatch*>((*itor2))->Code
                                )+"/template"+QString::number(idx)+".bmp";
                    QTextCodec *code = QTextCodec::codecForName("GB2312");
                    std::string name = code->fromUnicode(imgPath).data();
#ifdef IMG_TYPE_GRAY
                    img.ImageGray=cv::imread(name,0);
                    img.Width=img.ImageGray.cols;
                    img.Height=img.ImageGray.rows;
#endif
#ifdef IMG_TYPE_RGB
                    img.ImageRGB=cv::imread(name);
                    img.Width=img.ImageRGB.cols;
                    img.Height=img.ImageRGB.rows;

#endif

                    (*itor1)->TemplateImage[QString::fromStdString(
                                static_cast<CheckOperatorTemplateMatch*>((*itor2))->Code
                                )].insert(std::pair<int,OpencvImage>(idx,img));
                    //this->ClsImgTabacco->m_Template[(*itor1)->ImgIndex][(*itor2)->stu_CheckOperatorBasic->Index].insert(std::pair<int,OpenCvImage>(idx,img));
                }
            }
        }
    }
}

//void ImgTobaccoControl::addTemplate(int cameraNo,int checkOperatorNo,int templateNo,OpenCvImage& templateImage)
//{
//    this->ClsImgTabacco->m_Template[cameraNo][checkOperatorNo][templateNo]=templateImage;
//}
void ImgTobaccoControl::addTemplate(int cameraNo,QString code,int templateNo,OpencvImage& templateImage)
{
    ImgPro * imgPro=ImgTobaccoControl(this->ClsImgTabacco).getImgPro(cameraNo);
    imgPro->TemplateImage[code][templateNo]=templateImage;
}



//void ImgTobaccoControl::delTemplate(int cameraNo,int checkOperatorNo,int templateNo)
//{

//    std::map<int,OpenCvImage>::iterator it=  this->ClsImgTabacco->m_Template[cameraNo][checkOperatorNo].begin();
//    for(;it!=this->ClsImgTabacco->m_Template[cameraNo][checkOperatorNo].end();++it)
//    {

//        if(it->first==templateNo)
//        {
//            this->ClsImgTabacco->m_Template[cameraNo][checkOperatorNo].erase(it);
//            break;
//        }
//    }

//}

void ImgTobaccoControl::delTemplate(int cameraNo,QString code,int templateNo)
{

    ImgPro* imgPro=this->getImgPro(cameraNo);
    std::map<int,OpencvImage>::iterator it= imgPro->TemplateImage[code].begin();
    for(;it!=imgPro->TemplateImage[code].end();++it)
    {
        if(it->first==templateNo)
        {
            imgPro->TemplateImage[code].erase(it);
            break;
        }
    }

}
