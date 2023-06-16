#ifndef IMGTOBACCOCONTROL_H
#define IMGTOBACCOCONTROL_H
#include<QDateTime>
#include"imgtobacco.h"
#include"CheckOperator.hpp"
#include"common/XmlHelper.h"
#include"imgprocontrol.h"
#include"params/dssystemparam.h"
#include"common/filehelper.h"

class ImgTobaccoControl
{
public:
    ImgTobaccoControl(ImgTobacco* clsImgTabacco) ;

    //************************************
    // Method:    从xmlElement中加载数据
    // FullName:  ImgTabaccoControl::LoadXml
    // Access:    public
    // Returns:   int
    // Qualifier:
    // Parameter: XMLElement * pEleParent
    //************************************
    int LoadXml(XMLElement* pEleParent);

    //************************************
    // Method:    生成xmlElement块
    // FullName:  ImgTabaccoControl::generateXmlElementBlock
    // Access:    virtual public
    // Returns:   tinyxml2::XMLElement *
    // Qualifier:
    // Parameter: XMLDocument & doc
    //************************************
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc);


    static int generateInitXmlFile(QString xmlPath,QString brandName,int* imgHei,int* imgWid,int cameraNum);




    //************************************
    // Method:    获取当前相机数量
    // FullName:  ImgTabaccoControl::getImgProNum
    // Access:    public
    // Returns:   int
    // Qualifier:
    //************************************
    int getImgProNum();

    int getImgWid()
    {
        return  this->ClsImgTabacco->ImgWid;
    }

    int getImgHei()
    {
        return  this->ClsImgTabacco->ImgHei;
    }

    int getImgWid(int camIdx)
    {
        return  this->ClsImgTabacco->ImgWids[camIdx];
    }

    int getImgHei(int camIdx)
    {
        return  this->ClsImgTabacco->ImgHeis[camIdx];
    }


    //修改子节点内容
    void updateTabaccoNode(XMLElement* eleParent,string eleName, string text);
    void updateTabaccoNode(XMLElement* eleParent, string eleName, int inttext);
    void updateTabaccoNode(XMLElement* eleParent, string eleName, bool btext);
    void updateTabaccoNode(XMLElement* eleParent, string eleName, double dtext);
    void updateTabaccoNode(XMLElement* eleParent, string eleName, float ftext);


    //************************************
    // Method:    添加相机
    // FullName:  ImgTabaccoControl::insertImgPro
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: ImgPro * imgPro：要添加的相机
    // Parameter: XMLDocument & doc
    //************************************
    void addImgPro(ImgPro * imgPro,XMLElement* EleParent,TinyXmlDocument& doc);


    //************************************
    // Method:    根据编号获取相机
    // FullName:  ImgTabaccoControl::getImgPro
    // Access:    public
    // Returns:   ImgPro*
    // Qualifier:
    // Parameter: int index：相机编号
    //************************************
    ImgPro* getImgPro(int index);



    void resetforNewBrand();

    //************************************
    // Method:    获取指定相机的指定算子
    // FullName:  ImgTabaccoControl::getImgProCheckOperator
    // Access:    public
    // Returns:   CheckOperatorBasic*
    // Qualifier:
    // Parameter: int cameraNo:相机编号
    // Parameter: int CheckNo:算子编号
    //************************************
    CheckOperatorBasic* getImgProCheckOperator(int cameraNo,int CheckNo);

    int addImgProCheckOperator(int cameraNo,CheckOperatorBasic* pCheckOperator);

    CheckOperatorBasic* addImgProCheckOperator(int cameraNo,ENUMCHECKOPERATORTYPE type);




    //************************************
    // Method:    删除CameraNo下面的对应算子
    // FullName:  ImgTabaccoControl::deleteImgProCheckOperator
    // Access:    public
    // Returns:   int
    // Qualifier:
    // Parameter: int cameraNo
    // Parameter: int delCheckNo
    //************************************
    int deleteImgProCheckOperator(int cameraNo, int delCheckNo);


    //************************************
    // Method:    修改,先删除,再添加
    // FullName:  ImgTabaccoControl::updataXmlDocument
    // Access:    public
    // Returns:   int
    // Qualifier:
    // Parameter: XMLElement * eleTabcco
    // Parameter: XMLDocument & doc
    //************************************
    int updataXmlDocument(XMLElement* eleTabcco,TinyXmlDocument & doc);

    void saveConfigtoXmlFile(QString dirBrand)
    {
        ImgTobaccoControl control(this->ClsImgTabacco);


        //xml的保存
        TinyXmlDocument doc;
        std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
        doc.Parse(declaration.c_str());//会覆盖xml所有内容
        XMLElement * root= control.generateXmlElementBlock(doc);
        doc.InsertEndChild(root);

        QString xmlConfigPath=QString(dirBrand+"/config.xml");
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(xmlConfigPath).data();
        XMLError err=doc.SaveFile(name.c_str());
        DSDEBUG<<"save config.xml :"<<QString::fromStdString(name)<<"   res:" <<err<<endl;

    }
    void saveConfigtoXmlFileBackUp(QString dirBrand)
    {
        ImgTobaccoControl control(this->ClsImgTabacco);


        //xml的保存
        TinyXmlDocument doc;
        std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
        doc.Parse(declaration.c_str());//会覆盖xml所有内容
        XMLElement * root= control.generateXmlElementBlock(doc);
        doc.InsertEndChild(root);

        QString xmlConfigPath=QString(dirBrand+"/config(backup).xml");
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(xmlConfigPath).data();
        XMLError err=doc.SaveFile(name.c_str());
        DSDEBUG<<"save config.xml :"<<QString::fromStdString(name)<<"   res:" <<err<<endl;

    }


    void setRefImage(vector<OpencvImage> & vOpenCvImage);
    void setRefImage(int idx,OpencvImage& img)
    {
        list<ImgPro*>::iterator it=this->ClsImgTabacco->LstImgPro.begin();
        for(;it!=this->ClsImgTabacco->LstImgPro.end();++it)
        {
            if((*it)->ImgIndex==idx)
            {
                ImgProControl control((*it));
                control.setRefImage(img);
            }
        }
    }


    void setCalcImage(int idx,OpencvImage &openCvImage);
    void setCalcImage(vector<OpencvImage> & vOpenCvImage);

    void getCalcImagge(int idx,OpencvImage &openCvImage);

    void getCurBadImage(int idx,OpencvImage &openCvImage);

    void saveRefImagetoFile(QString dirBrand)
    {
        std::list<ImgPro*>::iterator itor=  this->ClsImgTabacco->LstImgPro.begin();
        for(;itor!=this->ClsImgTabacco->LstImgPro.end();++itor)
        {
            (*itor)->RefOpencvImage.save(dirBrand+"/ImgPro"+QString::number((*itor)->ImgIndex)+".bmp");
        }

    }

    int getBoundingBoxall(QRect allRect[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]);


    void loadRefImage(std::string dirPath);
    int calculate(unsigned char errMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]);

    int calculate(int imgIdx,unsigned char errMatrix[CAMERANUM_MAX][CHECKOPERATORNUM_MAX]);

    void loadConfig(QString dirBrand);
    void loadRefImageFromDir(QString dirBrand);
    void loadRefImageFromMemory(std::map<int,OpencvImage>& mapImage);

    void loadTemplate(QString dirBrand);

    void addTemplate(int cameraNo,QString code,int templateNo,OpencvImage& templateImage);


    void delTemplate(int cameraNo,QString code,int templateNo);

    void saveTemplatestoFile(QString dirBrand);


public:
    ImgTobacco *ClsImgTabacco;


};

#endif // IMGTOBACCOCONTROL_H
