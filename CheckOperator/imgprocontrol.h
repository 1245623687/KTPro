#ifndef IMGPROCONTROL_H
#define IMGPROCONTROL_H

#include"imgpro.h"
#include "CheckOperatorControl.hpp"
#include"common/XmlHelper.h"
class ImgProControl
{

private:
    int m_InitHeight=132;
    int m_InitWidth=132;
public:
    ImgProControl(ImgPro* imgPro);

    //************************************
    // Method:    从XmlElement中加载类数据
    // FullName:  ImgProControl::LoadXml
    // Access:    public
    // Returns:   int
    // Qualifier:
    // Parameter: XMLElement * pEleParent
    //************************************
    int LoadXml(XMLElement* pEleParent);
    //int LoadXml2( QDomElement pEleParent);



    static XMLElement* generateInitXmlElementBlock(TinyXmlDocument& doc,int index);

    static QDomElement* generateInitXmlElementBlock2(QDomDocument& doc,int index);

    //************************************
    // Method:    根据数据类生成xmlElement结构
    // FullName:  ImgProControl::generateXmlElementBlock
    // Access:    virtual public
    // Returns:   tinyxml2::XMLElement *
    // Qualifier:
    // Parameter: XMLDocument & doc
    //************************************
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc);
//   virtual  QDomElement * generateXmlElementBlock2(QDomDocument & doc);



    //************************************
    // Method:    获取算子数量
    // FullName:  ImgProControl::getCheckOperatorNum
    // Access:    public
    // Returns:   int
    // Qualifier:
    //************************************
    int getCheckOperatorNum();


    //修改子节点内容
    void updateImgProNode(XMLElement * eleParent,string eleName,string text);
    void updateImgProNode(XMLElement * eleParent,string eleName, int inttext);
    void updateImgProNode(XMLElement * eleParent, string eleName, bool btext);
    void updateImgProNode(XMLElement * eleParent, string eleName, double dtext);
    void updateImgProNode(XMLElement * eleParent, string eleName, float ftext);


    //************************************
    // Method:    获取第index个算子
    // FullName:  ImgProControl::getCheckNo
    // Access:    public
    // Returns:   CheckOperatorBasic*
    // Qualifier:
    // Parameter: int index
    //************************************
    CheckOperatorBasic* getCheckNo(int index);


    //************************************
    // Method:    添加算子到文件
    // FullName:  ImgProControl::insertCheckOperator
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: XMLElement * EleParent:父节点
    // Parameter: CheckOperatorBasic * checkOperator：要添加的算子
    // Parameter: ENUMCHECKOPERATORTYPE type：算子类型
    // Parameter: TinyXmlDocument & doc：xmldocument
    //************************************
    void insertXMLCheckOperator(XMLElement * EleParent,CheckOperatorBasic* checkOperator, ENUMCHECKOPERATORTYPE type, TinyXmlDocument& doc);
    CheckOperatorBasic*  insertCheckOperator(ENUMCHECKOPERATORTYPE type);


    void insertCheckOperator(CheckOperatorBasic* pCheckOperatorBasic);

    //************************************
    // Method:    删除对应编号的算子
    // FullName:  ImgProControl::deleteCheckOperator
    // Access:    public
    // Returns:   int
    // Qualifier:
    // Parameter: int index
    //************************************
    int deleteCheckOperator(int index);

    void setRefImage(OpencvImage& openCvImage);

    void setCalcImage(OpencvImage& openCvImage);

    void getCalcImage(OpencvImage& openCvImage);
    void getCurBadImage(OpencvImage& openCvImage);



    int calculateCV(unsigned char errArr[]);


    //int updateRefPostion();
//    int updateRefPos();


public:
    ImgPro* ClsImgPro;
    cv::Mat tempCloneMat;

};

#endif // IMGPROCONTROL_H
