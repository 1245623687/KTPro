#ifdef __cplusplus
#pragma once


#include<math.h>
#include "CheckOperator.hpp"
#include "opencv2/opencv.hpp"
#include "tinyxml2.h"
#include "common/XmlHelper.h"
#include "DSIppImage.h"
#include"dsdebug.h"
#include"CheckOperator/ImageBase.hpp"
#include "glog/logging.h"
#include"Onnx.h"




using namespace tinyxml2;
#ifndef PI
#define PI 3.14159265
#endif
class CheckOperatorControlBase
{
public:
    
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)=0;
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc) = 0;

    virtual QDomElement * generateXmlElementBlock2(QDomDocument & qdoc){
        return nullptr;
    };
    
    //virtual int calculate() = 0;
    
    
    
    virtual ~CheckOperatorControlBase() {}
};

//rgb->gray
class StuGrayConversionControl :public CheckOperatorControlBase
{
public:
    StuGrayConversionControl(StuGrayConversion* stuGrayConversion) :m_StuGrayConversion(stuGrayConversion){}
    
    int LoadXml( XMLElement* pEleParent)
    {
        string temp="";
        XmlHelper::GetElementTextFromParent("GrayMode", pEleParent, temp);
        this->m_StuGrayConversion->GrayMode = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("RedRate", pEleParent, temp);
        this->m_StuGrayConversion->RedRate = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("GreenRate", pEleParent, temp);
        this->m_StuGrayConversion->GreenRate = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("BlueRate", pEleParent, temp);
        this->m_StuGrayConversion->BlueRate = atof(temp.c_str());
        return 0;
    }

    int LoadXml2( QDomElement pEleParent)
    {
        QString temp="";
        XmlHelper::GetElementTextFromParent2("GrayMode", pEleParent, temp);
        this->m_StuGrayConversion->GrayMode = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RedRate", pEleParent, temp);
        this->m_StuGrayConversion->RedRate = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("GreenRate", pEleParent, temp);
        this->m_StuGrayConversion->GreenRate = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("BlueRate", pEleParent, temp);
        this->m_StuGrayConversion->BlueRate = atof(temp.toStdString().c_str());
        return 0;
    }
    
    //生成xmlElement
    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleGrayConversion = doc.NewElement("GrayConversion");
        XMLElement* eleGrayMode = doc.NewElement("GrayMode");
        eleGrayMode->SetText(this->m_StuGrayConversion->GrayMode);
        XMLElement* eleRedRate = doc.NewElement("RedRate");
        eleRedRate->SetText(this->m_StuGrayConversion->RedRate);
        XMLElement* eleGreenRate = doc.NewElement("GreenRate");
        eleGreenRate->SetText(this->m_StuGrayConversion->GreenRate);
        XMLElement* eleBlueRate = doc.NewElement("BlueRate");
        eleBlueRate->SetText(this->m_StuGrayConversion->BlueRate);
        
        eleGrayConversion->InsertEndChild(eleGrayMode);
        eleGrayConversion->InsertEndChild(eleRedRate);
        eleGrayConversion->InsertEndChild(eleGreenRate);
        eleGrayConversion->InsertEndChild(eleBlueRate);
        
        return eleGrayConversion;
    }


    //生成xmlElement
    QDomElement * generateXmlElementBlock2(QDomDocument & doc)
    {
        QDomElement tempeleGrayConversion=doc.createElement("GrayConversion");
        QDomElement *eleGrayConversion=new QDomElement(tempeleGrayConversion);


        QDomElement eleGrayMode=doc.createElement("GrayMode");
        QDomText texteleGrayMode=doc.createTextNode(QString::number(this->m_StuGrayConversion->GrayMode));
        eleGrayMode.appendChild(texteleGrayMode);


        QDomElement eleRedRate=doc.createElement("RedRate");
        QDomText texteleRedRate=doc.createTextNode(QString::number(this->m_StuGrayConversion->RedRate));
        eleRedRate.appendChild(texteleRedRate);

        QDomElement eleGreenRate=doc.createElement("GreenRate");
        QDomText texteleGreenRate=doc.createTextNode(QString::number(this->m_StuGrayConversion->GreenRate));
        eleGreenRate.appendChild(texteleGreenRate);

        QDomElement eleBlueRate=doc.createElement("BlueRate");
        QDomText texteleBlueRate=doc.createTextNode(QString::number(this->m_StuGrayConversion->BlueRate));
        eleBlueRate.appendChild(texteleBlueRate);


        eleGrayConversion->appendChild(eleGrayMode);
        eleGrayConversion->appendChild(eleRedRate);
        eleGrayConversion->appendChild(eleGreenRate);
        eleGrayConversion->appendChild(eleBlueRate);

        return eleGrayConversion;
    }
    
    
    
public:
    StuGrayConversion* m_StuGrayConversion;
};

//霍夫
class StuHoughLineControl :public CheckOperatorControlBase
{
public:
    StuHoughLineControl(StuHoughLine* stuStuHoughLine): StuStuHoughLine(stuStuHoughLine){}
    int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        XmlHelper::GetElementTextFromParent("StAngle", pEleParent, temp);
        this->StuStuHoughLine->StAngle = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("EndAngle", pEleParent, temp);
        this->StuStuHoughLine->EndAngle = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineNum", pEleParent, temp);
        this->StuStuHoughLine->LineNum = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("EdgeThresh", pEleParent, temp);
        this->StuStuHoughLine->EdgeThresh = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RgnR", pEleParent, temp);
        this->StuStuHoughLine->RgnR = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RgnQ", pEleParent, temp);
        this->StuStuHoughLine->RgnQ = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("ThreshShift", pEleParent, temp);
        this->StuStuHoughLine->ThreshShift = atof(temp.c_str());
        return 0;
    }
    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc) {
        
        XMLElement* eleHoughLine = doc.NewElement("HoughLine");
        
        XMLElement* eleStAngle = doc.NewElement("StAngle");
        eleStAngle->SetText(this->StuStuHoughLine->StAngle);
        XMLElement* eleEndAngle = doc.NewElement("EndAngle");
        eleEndAngle->SetText(this->StuStuHoughLine->EndAngle);
        XMLElement* eleLineNum = doc.NewElement("LineNum");
        eleLineNum->SetText(this->StuStuHoughLine->LineNum);
        XMLElement* eleEdgeThresh = doc.NewElement("EdgeThresh");
        eleEdgeThresh->SetText(this->StuStuHoughLine->EdgeThresh);
        XMLElement* eleRgnR = doc.NewElement("RgnR");
        eleRgnR->SetText(this->StuStuHoughLine->RgnR);
        XMLElement* eleRgnQ = doc.NewElement("RgnQ");
        eleRgnQ->SetText(this->StuStuHoughLine->RgnQ);
        XMLElement* eleThreshShift = doc.NewElement("ThreshShift");
        eleThreshShift->SetText(this->StuStuHoughLine->ThreshShift);
        
        eleHoughLine->InsertEndChild(eleStAngle);
        eleHoughLine->InsertEndChild(eleEndAngle);
        eleHoughLine->InsertEndChild(eleLineNum);
        eleHoughLine->InsertEndChild(eleEdgeThresh);
        eleHoughLine->InsertEndChild(eleRgnR);
        eleHoughLine->InsertEndChild(eleRgnQ);
        eleHoughLine->InsertEndChild(eleThreshShift);
        return eleHoughLine;
    }
    
public:
    StuHoughLine* StuStuHoughLine;
};

//StuLineLenLineLen
class StuLineLenLineLenControl :public CheckOperatorControlBase
{
public:
    StuLineLenLineLenControl(StuLineLenLineLen* stuStuLineLenLineLen) : StuStuLineLenLineLen(stuStuLineLenLineLen) {}
    int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        XmlHelper::GetElementTextFromParent("LineColor", pEleParent, temp);
        this->StuStuLineLenLineLen->LineColor = static_cast<ENUMLINECOLOR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("EdgeThreshold", pEleParent, temp);
        this->StuStuLineLenLineLen->EdgeThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->StuStuLineLenLineLen->LineDir = static_cast<StuLineLenLineLen::ENUMLINEDIR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("ScanLineDir", pEleParent, temp);
        this->StuStuLineLenLineLen->ScanLineDir = static_cast<StuLineLenLineLen::ENUMLINEDIR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("FitWeight", pEleParent, temp);
        this->StuStuLineLenLineLen->FitWeight = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineLength", pEleParent, temp);
        this->StuStuLineLenLineLen->LineLength = atoi(temp.c_str());
        
        return 0;
    }
    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleLineLen = doc.NewElement("LineLen");
        
        XMLElement* eleLineColor = doc.NewElement("LineColor");
        eleLineColor->SetText(static_cast<int>(this->StuStuLineLenLineLen->LineColor));
        XMLElement* eleEdgeThreshold = doc.NewElement("EdgeThreshold");
        eleEdgeThreshold->SetText(this->StuStuLineLenLineLen->EdgeThreshold);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText(static_cast<int>(this->StuStuLineLenLineLen->LineDir));
        XMLElement* eleScanLineDir = doc.NewElement("ScanLineDir");
        eleScanLineDir->SetText(static_cast<int>(this->StuStuLineLenLineLen->ScanLineDir));
        XMLElement* eleFitWeight = doc.NewElement("FitWeight");
        eleFitWeight->SetText(static_cast<int>(this->StuStuLineLenLineLen->FitWeight));
        XMLElement* eleLineLength = doc.NewElement("LineLength");
        eleLineLength->SetText(static_cast<int>(this->StuStuLineLenLineLen->LineLength));
        
        eleLineLen->InsertEndChild(eleLineColor);
        eleLineLen->InsertEndChild(eleEdgeThreshold);
        eleLineLen->InsertEndChild(eleLineDir);
        eleLineLen->InsertEndChild(eleScanLineDir);
        eleLineLen->InsertEndChild(eleFitWeight);
        eleLineLen->InsertEndChild(eleLineLength);
        
        return eleLineLen;
    }
    
public:
    StuLineLenLineLen * StuStuLineLenLineLen;
};


//StuEdgeLine
class StuEdgeLineControl :public CheckOperatorControlBase
{
public:
    StuEdgeLineControl(CheckOperatorEdgePositioning::StuEdgeLine* stuStuEdgeLine) : StuStuEdgeLine(stuStuEdgeLine) {}
    int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        
        XmlHelper::GetElementTextFromParent("LineColor", pEleParent, temp);
        //this->StuStuEdgeLine->LineColor = (ENUMLINECOLOR)atoi(temp.c_str());
        this->StuStuEdgeLine->LineColor = static_cast<ENUMLINECOLOR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("EdgeThreshold", pEleParent, temp);
        this->StuStuEdgeLine->EdgeThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->StuStuEdgeLine->LineDir = static_cast<CheckOperatorEdgePositioning::StuEdgeLine::ENUMLINEDIR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("ScanLineDir", pEleParent, temp);
        this->StuStuEdgeLine->ScanLineDir = static_cast<CheckOperatorEdgePositioning::StuEdgeLine::ENUMLINEDIR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("FitWeight", pEleParent, temp);
        this->StuStuEdgeLine->FitWeight = static_cast<ENUMLINECOLOR>(atoi(temp.c_str()));
        XmlHelper::GetElementTextFromParent("PosX", pEleParent, temp);
        this->StuStuEdgeLine->PosX = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PosY", pEleParent, temp);
        this->StuStuEdgeLine->PosY = atoi(temp.c_str());
        
        return 0;
    }
    QDomElement * generateXmlElementBlock2(QDomDocument& doc)
    {
        QDomElement tempeleLineLen=doc.createElement("EdgeLine");
        QDomElement* eleLineLen = new QDomElement(tempeleLineLen);

        QDomElement eleLineColor=doc.createElement("LineColor");
        QDomText texteleLineColor=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->LineColor)));
        eleLineColor.appendChild(texteleLineColor);

        QDomElement eleEdgeThreshold=doc.createElement("EdgeThreshold");
        QDomText texteleEdgeThreshold=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->EdgeThreshold)));
        eleEdgeThreshold.appendChild(texteleEdgeThreshold);

        QDomElement eleLineDir=doc.createElement("LineDir");
        QDomText texteleLineDir=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->LineDir)));
        eleLineDir.appendChild(texteleLineDir);

        QDomElement eleScanLineDir=doc.createElement("ScanLineDir");
        QDomText texteleScanLineDir=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->ScanLineDir)));
        eleScanLineDir.appendChild(texteleScanLineDir);

        QDomElement eleFitWeight=doc.createElement("FitWeight");
        QDomText texteleFitWeight=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->FitWeight)));
        eleFitWeight.appendChild(texteleFitWeight);

        QDomElement elePosX=doc.createElement("PosX");
        QDomText textelePosX=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->PosX)));
        elePosX.appendChild(textelePosX);

        QDomElement elePosY=doc.createElement("PosY");
        QDomText textelePosY=doc.createTextNode(QString::number(static_cast<int>(this->StuStuEdgeLine->PosY)));
        elePosY.appendChild(textelePosY);


        
        eleLineLen->appendChild(eleLineColor);
        eleLineLen->appendChild(eleEdgeThreshold);
        eleLineLen->appendChild(eleLineDir);
        eleLineLen->appendChild(eleScanLineDir);
        eleLineLen->appendChild(eleFitWeight);
        eleLineLen->appendChild(elePosX);
        eleLineLen->appendChild(elePosY);
        
        return eleLineLen;
    }

    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleLineLen = doc.NewElement("EdgeLine");

        XMLElement* eleLineColor = doc.NewElement("LineColor");
        eleLineColor->SetText(static_cast<int>(this->StuStuEdgeLine->LineColor));
        XMLElement* eleEdgeThreshold = doc.NewElement("EdgeThreshold");
        eleEdgeThreshold->SetText(this->StuStuEdgeLine->EdgeThreshold);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText(static_cast<int>(this->StuStuEdgeLine->LineDir));
        XMLElement* eleScanLineDir = doc.NewElement("ScanLineDir");
        eleScanLineDir->SetText(static_cast<int>(this->StuStuEdgeLine->ScanLineDir));
        XMLElement* eleFitWeight = doc.NewElement("FitWeight");
        eleFitWeight->SetText(static_cast<int>(this->StuStuEdgeLine->FitWeight));
        XMLElement* elePosX = doc.NewElement("PosX");
        elePosX->SetText(static_cast<int>(this->StuStuEdgeLine->PosX));
        XMLElement* elePosY = doc.NewElement("PosY");
        elePosY->SetText(static_cast<int>(this->StuStuEdgeLine->PosY));

        eleLineLen->InsertEndChild(eleLineColor);
        eleLineLen->InsertEndChild(eleEdgeThreshold);
        eleLineLen->InsertEndChild(eleLineDir);
        eleLineLen->InsertEndChild(eleScanLineDir);
        eleLineLen->InsertEndChild(eleFitWeight);
        eleLineLen->InsertEndChild(elePosX);
        eleLineLen->InsertEndChild(elePosY);

        return eleLineLen;
    }
    
    
    
    
public:
    CheckOperatorEdgePositioning::StuEdgeLine * StuStuEdgeLine;
};


class StuGrayMatchControl :public CheckOperatorControlBase
{
public:
    StuGrayMatchControl(CheckOperatorTemplateMatch::StuGrayMatch* stuStuGrayMatch) : StuStuGrayMatch(stuStuGrayMatch) {}
    
    
    int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        
        XmlHelper::GetElementTextFromParent("SearchCount", pEleParent, temp);
        this->StuStuGrayMatch->SearchCount = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("SearchMode", pEleParent, temp);
        this->StuStuGrayMatch->SearchMode = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Precision", pEleParent, temp);
        this->StuStuGrayMatch->Precision =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Condition", pEleParent, temp);
        this->StuStuGrayMatch->Condition =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score1", pEleParent, temp);
        this->StuStuGrayMatch->Score1 = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("Score2", pEleParent, temp);
        this->StuStuGrayMatch->Score2 =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PatternNum", pEleParent, temp);
        this->StuStuGrayMatch->PatternNum = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PatternRect_Xs", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Xs =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PatternRect_Ys", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Ys = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PatternRect_Xe", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Xe =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PatternRect_Ye", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Ye = atoi(temp.c_str());
        
        return 0;
    }
    int LoadXml2(QDomElement pEleParent)
    {
        QString temp = "";

        XmlHelper::GetElementTextFromParent2("SearchCount", pEleParent, temp);
        this->StuStuGrayMatch->SearchCount = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("SearchMode", pEleParent, temp);
        this->StuStuGrayMatch->SearchMode = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("Precision", pEleParent, temp);
        this->StuStuGrayMatch->Precision =atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("Condition", pEleParent, temp);
        this->StuStuGrayMatch->Condition =atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("Score1", pEleParent, temp);
        this->StuStuGrayMatch->Score1 = atoi(temp.toStdString().c_str());

        XmlHelper::GetElementTextFromParent2("Score2", pEleParent, temp);
        this->StuStuGrayMatch->Score2 =atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("PatternNum", pEleParent, temp);
        this->StuStuGrayMatch->PatternNum = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("PatternRect_Xs", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Xs =atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("PatternRect_Ys", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Ys = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("PatternRect_Xe", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Xe =atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("PatternRect_Ye", pEleParent, temp);
        this->StuStuGrayMatch->PatternRect_Ye = atoi(temp.toStdString().c_str());

        return 0;
    }


    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleGrayMatch = doc.NewElement("GrayMatch");
        
        XMLElement* eleSearchCount = doc.NewElement("SearchCount");
        eleSearchCount->SetText(static_cast<int>(this->StuStuGrayMatch->SearchCount));
        XMLElement* eleSearchMode = doc.NewElement("SearchMode");
        eleSearchMode->SetText(static_cast<int>(this->StuStuGrayMatch->SearchMode));
        XMLElement* elePrecision = doc.NewElement("Precision");
        elePrecision->SetText(static_cast<int>(this->StuStuGrayMatch->Precision));
        XMLElement* eleCondition = doc.NewElement("Condition");
        eleCondition->SetText(static_cast<int>(this->StuStuGrayMatch->Condition));
        XMLElement* eleScore1 = doc.NewElement("Score1");
        eleScore1->SetText(static_cast<int>(this->StuStuGrayMatch->Score1));
        XMLElement* eleScore2 = doc.NewElement("Score2");
        eleScore2->SetText(static_cast<int>(this->StuStuGrayMatch->Score2));
        XMLElement* elePatternNum = doc.NewElement("PatternNum");
        elePatternNum->SetText(static_cast<int>(this->StuStuGrayMatch->PatternNum));
        XMLElement* elePatternRect_Xs = doc.NewElement("PatternRect_Xs");
        elePatternRect_Xs->SetText(static_cast<int>(this->StuStuGrayMatch->PatternRect_Xs));
        XMLElement* elePatternRect_Ys = doc.NewElement("PatternRect_Ys");
        elePatternRect_Ys->SetText(static_cast<int>(this->StuStuGrayMatch->PatternRect_Ys));
        XMLElement* elePatternRect_Xe = doc.NewElement("PatternRect_Xe");
        elePatternRect_Xe->SetText(static_cast<int>(this->StuStuGrayMatch->PatternRect_Xe));
        XMLElement* elePatternRect_Ye = doc.NewElement("PatternRect_Ye");
        elePatternRect_Ye->SetText(static_cast<int>(this->StuStuGrayMatch->PatternRect_Ye));
        
        
        
        eleGrayMatch->InsertEndChild(eleSearchCount);
        eleGrayMatch->InsertEndChild(eleSearchMode);
        eleGrayMatch->InsertEndChild(elePrecision);
        eleGrayMatch->InsertEndChild(eleCondition);
        eleGrayMatch->InsertEndChild(eleScore1);
        eleGrayMatch->InsertEndChild(eleScore2);
        eleGrayMatch->InsertEndChild(elePatternNum);
        eleGrayMatch->InsertEndChild(elePatternRect_Xs);
        eleGrayMatch->InsertEndChild(elePatternRect_Ys);
        eleGrayMatch->InsertEndChild(elePatternRect_Xe);
        eleGrayMatch->InsertEndChild(elePatternRect_Ye);
        
        
        return eleGrayMatch;
    }

    QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleGrayMatch=doc.createElement("GrayMatch");
        QDomElement *eleGrayMatch=new QDomElement(tempeleGrayMatch);


        QDomElement eleSearchCount=doc.createElement("SearchCount");
        QDomText texteleSearchCount=doc.createTextNode(QString::number(this->StuStuGrayMatch->SearchCount));
        eleSearchCount.appendChild(texteleSearchCount);

        QDomElement eleSearchMode=doc.createElement("SearchMode");
        QDomText texteleSearchMode=doc.createTextNode(QString::number(this->StuStuGrayMatch->SearchMode));
        eleSearchMode.appendChild(texteleSearchMode);

        QDomElement elePrecision=doc.createElement("Precision");
        QDomText textelePrecision=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->Precision)));
        elePrecision.appendChild(textelePrecision);

        QDomElement eleCondition=doc.createElement("Condition");
        QDomText texteleCondition=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->Condition)));
        eleCondition.appendChild(texteleCondition);

        QDomElement eleScore1=doc.createElement("Score1");
        QDomText texteleScore1=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->Score1)));
        eleScore1.appendChild(texteleScore1);

        QDomElement eleScore2=doc.createElement("Score2");
        QDomText texteleScore2=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->Score2)));
        eleScore2.appendChild(texteleScore2);

        QDomElement elePatternNum=doc.createElement("PatternNum");
        QDomText textelePatternNum=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->PatternNum)));
        elePatternNum.appendChild(textelePatternNum);

        QDomElement elePatternRect_Xs=doc.createElement("PatternRect_Xs");
        QDomText textelePatternRect_Xs=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->PatternRect_Xs)));
        elePatternRect_Xs.appendChild(textelePatternRect_Xs);

        QDomElement elePatternRect_Ys=doc.createElement("PatternRect_Ys");
        QDomText textelePatternRect_Ys=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->PatternRect_Ys)));
        elePatternRect_Ys.appendChild(textelePatternRect_Ys);

        QDomElement elePatternRect_Xe=doc.createElement("PatternRect_Xe");
        QDomText textelePatternRect_Xe=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->PatternRect_Xe)));
        elePatternRect_Xe.appendChild(textelePatternRect_Xe);


        QDomElement elePatternRect_Ye=doc.createElement("PatternRect_Ye");
        QDomText textelePatternRect_Ye=doc.createTextNode(QString::number(static_cast<int>(this->StuStuGrayMatch->PatternRect_Ye)));
        elePatternRect_Ye.appendChild(textelePatternRect_Ye);





        eleGrayMatch->appendChild(eleSearchCount);
        eleGrayMatch->appendChild(eleSearchMode);
        eleGrayMatch->appendChild(elePrecision);
        eleGrayMatch->appendChild(eleCondition);
        eleGrayMatch->appendChild(eleScore1);
        eleGrayMatch->appendChild(eleScore2);
        eleGrayMatch->appendChild(elePatternNum);
        eleGrayMatch->appendChild(elePatternRect_Xs);
        eleGrayMatch->appendChild(elePatternRect_Ys);
        eleGrayMatch->appendChild(elePatternRect_Xe);
        eleGrayMatch->appendChild(elePatternRect_Ye);


        return eleGrayMatch;
    }
    
public:
    CheckOperatorTemplateMatch::StuGrayMatch * StuStuGrayMatch;
};




/*------------------------------------------------------------------------------------华丽的分割线------------------------------------------------------------------------------------*/



//Basic
class StuCheckOperatorBasicControl :public CheckOperatorControlBase
{
public:
    StuCheckOperatorBasicControl(StuCheckOperatorBasic* stuCheckOperatorBasic) :m_StuCheckOperatorBasic(stuCheckOperatorBasic) {}
    
    int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        
        XmlHelper::GetElementTextFromParent("RectValid_Xs", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValid_Ys", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValid_Xe", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValid_Ye", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Ye = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValidInvalid_Xs", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValidInvalid_Ys", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValidInvalid_Xe", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("RectValidInvalid_Ye", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ye = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("ExistInvalidRect", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_ExistInvalidRect = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("IsCheck", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_IsCheck = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Vaild", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_Vaild = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Ref_X", pEleParent, temp);
        if(temp!="") this->m_StuCheckOperatorBasic->Basic_Ref_X = temp.c_str();
        else this->m_StuCheckOperatorBasic->Basic_Ref_X="";
        //this->m_StuCheckOperatorBasic->Basic_Ref_X = temp.c_str();
        XmlHelper::GetElementTextFromParent("Ref_Y", pEleParent, temp);
        if(temp!="") this->m_StuCheckOperatorBasic->Basic_Ref_Y = temp.c_str();
        else this->m_StuCheckOperatorBasic->Basic_Ref_Y="";
        //this->m_StuCheckOperatorBasic->Basic_Ref_Y = temp.c_str();
        XmlHelper::GetElementTextFromParent("Ref_Angle", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_Ref_Angle = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AsRef", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsRef = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AsDir", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsDir = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("AsContinusCheck", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsContinusCheck = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("IsAssociated", pEleParent, temp);
        this->m_StuCheckOperatorBasic->IsAssociated = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("CamIdx", pEleParent, temp);
        this->m_StuCheckOperatorBasic->CamIdx = atoi(temp.c_str());
        
        XMLElement * eleGrayConversion = XmlHelper::GetElementByParent("GrayConversion", pEleParent);
        StuGrayConversionControl *stuGrayConversionControl = new StuGrayConversionControl(this->m_StuCheckOperatorBasic->Basic_GrayConversion);
        stuGrayConversionControl->LoadXml(eleGrayConversion);
        delete stuGrayConversionControl;
        
        return 0;
    }

    int LoadXml2(QDomElement pEleParent)
    {
        QString temp = "";

        XmlHelper::GetElementTextFromParent2("RectValid_Xs", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Xs = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValid_Ys", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Ys = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValid_Xe", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Xe =atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValid_Ye", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValid_Ye = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValidInvalid_Xs", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xs = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValidInvalid_Ys", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ys = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValidInvalid_Xe", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xe = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("RectValidInvalid_Ye", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ye = atof(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("ExistInvalidRect", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_ExistInvalidRect = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("IsCheck", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_IsCheck = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("Vaild", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_Vaild = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("Ref_X", pEleParent, temp);
        if(temp!="") this->m_StuCheckOperatorBasic->Basic_Ref_X = temp.toStdString().c_str();
        else this->m_StuCheckOperatorBasic->Basic_Ref_X="";
        //this->m_StuCheckOperatorBasic->Basic_Ref_X = temp.c_str();
        XmlHelper::GetElementTextFromParent2("Ref_Y", pEleParent, temp);
        if(temp!="") this->m_StuCheckOperatorBasic->Basic_Ref_Y = temp.toStdString().c_str();
        else this->m_StuCheckOperatorBasic->Basic_Ref_Y="";
        //this->m_StuCheckOperatorBasic->Basic_Ref_Y = temp.c_str();
        XmlHelper::GetElementTextFromParent2("Ref_Angle", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_Ref_Angle = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("AsRef", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsRef = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("AsDir", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsDir = atoi(temp.toStdString().c_str());
        XmlHelper::GetElementTextFromParent2("AsContinusCheck", pEleParent, temp);
        this->m_StuCheckOperatorBasic->Basic_AsContinusCheck = atoi(temp.toStdString().c_str());

        QDomElement * eleGrayConversion = XmlHelper::GetElementByParent2("GrayConversion", pEleParent);
        StuGrayConversionControl *stuGrayConversionControl = new StuGrayConversionControl(this->m_StuCheckOperatorBasic->Basic_GrayConversion);
        stuGrayConversionControl->LoadXml2(*eleGrayConversion);
        delete stuGrayConversionControl;
        delete eleGrayConversion;

        return 0;
    }


    
    XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleBasic = doc.NewElement("Basic");
        XMLElement* eleRectValid_Xs = doc.NewElement("RectValid_Xs");
        eleRectValid_Xs->SetText(this->m_StuCheckOperatorBasic->Basic_RectValid_Xs);
        XMLElement* eleRectValid_Ys = doc.NewElement("RectValid_Ys");
        eleRectValid_Ys->SetText(this->m_StuCheckOperatorBasic->Basic_RectValid_Ys);
        XMLElement* eleRectValid_Xe = doc.NewElement("RectValid_Xe");
        eleRectValid_Xe->SetText(this->m_StuCheckOperatorBasic->Basic_RectValid_Xe);
        XMLElement* eleRectValid_Ye = doc.NewElement("RectValid_Ye");
        eleRectValid_Ye->SetText(this->m_StuCheckOperatorBasic->Basic_RectValid_Ye);
        
        XMLElement* eleRectValidInvalid_Xs = doc.NewElement("RectValidInvalid_Xs");
        eleRectValidInvalid_Xs->SetText(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xs);
        XMLElement* eleRectValidInvalid_Ys = doc.NewElement("RectValidInvalid_Ys");
        eleRectValidInvalid_Ys->SetText(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ys);
        XMLElement* eleRectValidInvalid_Xe = doc.NewElement("RectValidInvalid_Xe");
        eleRectValidInvalid_Xe->SetText(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xe);
        XMLElement* eleRectValidInvalid_Ye = doc.NewElement("RectValidInvalid_Ye");
        eleRectValidInvalid_Ye->SetText(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ye);
        
        XMLElement* eleExistInvalidRect = doc.NewElement("ExistInvalidRect");
        eleExistInvalidRect->SetText(this->m_StuCheckOperatorBasic->Basic_ExistInvalidRect?1:0);
        XMLElement* eleIsCheck = doc.NewElement("IsCheck");
        eleIsCheck->SetText(this->m_StuCheckOperatorBasic->Basic_IsCheck ? 1 : 0);
        XMLElement* eleVaild = doc.NewElement("Vaild");
        eleVaild->SetText(this->m_StuCheckOperatorBasic->Basic_Vaild ? 1 : 0);
        XMLElement* eleRef_X = doc.NewElement("Ref_X");
        //DSDEBUG<<"$$$$"<<this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str()<<endl;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X!=""){
            eleRef_X->SetText(this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str());
        }
        XMLElement* eleRef_Y = doc.NewElement("Ref_Y");
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y!=""){
            eleRef_Y->SetText(this->m_StuCheckOperatorBasic->Basic_Ref_Y.c_str());
        }
        XMLElement* eleRef_Angle = doc.NewElement("Ref_Angle");
        eleRef_Angle->SetText(this->m_StuCheckOperatorBasic->Basic_Ref_Angle);
        XMLElement* eleAsRef = doc.NewElement("AsRef");
        eleAsRef->SetText(this->m_StuCheckOperatorBasic->Basic_AsRef);

        XMLElement* eleAsDir = doc.NewElement("AsDir");
        eleAsDir->SetText(this->m_StuCheckOperatorBasic->Basic_AsDir);

        XMLElement* eleAsContinusCheck = doc.NewElement("AsContinusCheck");
        eleAsContinusCheck->SetText(this->m_StuCheckOperatorBasic->Basic_AsContinusCheck);

        XMLElement* eleIsAssociated = doc.NewElement("IsAssociated");
        eleIsAssociated->SetText(this->m_StuCheckOperatorBasic->IsAssociated);

        XMLElement* eleCamIdx = doc.NewElement("CamIdx");
        eleCamIdx->SetText(this->m_StuCheckOperatorBasic->CamIdx);

        
        StuGrayConversionControl *  eleStuGrayConversionControl = new StuGrayConversionControl(this->m_StuCheckOperatorBasic->Basic_GrayConversion);
        XMLElement * eleGrayConversion = eleStuGrayConversionControl->generateXmlElementBlock(doc);
        delete eleStuGrayConversionControl;
        
        eleBasic->InsertEndChild(eleRectValid_Xs);
        eleBasic->InsertEndChild(eleRectValid_Ys);
        eleBasic->InsertEndChild(eleRectValid_Xe);
        eleBasic->InsertEndChild(eleRectValid_Ye);
        eleBasic->InsertEndChild(eleRectValidInvalid_Xs);
        eleBasic->InsertEndChild(eleRectValidInvalid_Ys);
        eleBasic->InsertEndChild(eleRectValidInvalid_Xe);
        eleBasic->InsertEndChild(eleRectValidInvalid_Ye);
        eleBasic->InsertEndChild(eleExistInvalidRect);
        eleBasic->InsertEndChild(eleIsCheck);
        eleBasic->InsertEndChild(eleVaild);
        eleBasic->InsertEndChild(eleRef_X);
        eleBasic->InsertEndChild(eleRef_Y);
        eleBasic->InsertEndChild(eleRef_Angle);
        eleBasic->InsertEndChild(eleAsRef);
        eleBasic->InsertEndChild(eleAsDir);
        eleBasic->InsertEndChild(eleAsContinusCheck);
        eleBasic->InsertEndChild(eleIsAssociated);
        eleBasic->InsertEndChild(eleCamIdx);


        eleBasic->InsertEndChild(eleGrayConversion);
        return eleBasic;
    }


    QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleBasic=doc.createElement("Basic");

        QDomElement* eleBasic = new QDomElement(tempeleBasic);


        QDomElement eleRectValid_Xs = doc.createElement("RectValid_Xs");
        QDomText texteleRectValid_Xs=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValid_Xs));
        eleRectValid_Xs.appendChild(texteleRectValid_Xs);

        QDomElement eleRectValid_Ys = doc.createElement("RectValid_Ys");
        QDomText texteleRectValid_Ys=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValid_Ys));
        eleRectValid_Ys.appendChild(texteleRectValid_Ys);

        QDomElement eleRectValid_Xe = doc.createElement("RectValid_Xe");
        QDomText texteleRectValid_Xe=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValid_Xe));
        eleRectValid_Xe.appendChild(texteleRectValid_Xe);


        QDomElement eleRectValid_Ye = doc.createElement("RectValid_Ye");
        QDomText texteleRectValid_Ye=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValid_Ye));
        eleRectValid_Ye.appendChild(texteleRectValid_Ye);


        QDomElement eleRectValidInvalid_Xs = doc.createElement("RectValidInvalid_Xs");
        QDomText texteleRectValidInvalid_Xs=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xs));
        eleRectValidInvalid_Xs.appendChild(texteleRectValidInvalid_Xs);

        QDomElement eleRectValidInvalid_Ys = doc.createElement("RectValidInvalid_Ys");
        QDomText texteleRectValidInvalid_Ys=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ys));
        eleRectValidInvalid_Ys.appendChild(texteleRectValidInvalid_Xs);

        QDomElement eleRectValidInvalid_Xe = doc.createElement("RectValidInvalid_Xe");
        QDomText texteleRectValidInvalid_Xe=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Xe));
        eleRectValidInvalid_Xe.appendChild(texteleRectValidInvalid_Xs);

        QDomElement eleRectValidInvalid_Ye = doc.createElement("RectValidInvalid_Ye");
        QDomText texteleRectValidInvalid_Ye=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_RectValidInvalid_Ye));
        eleRectValidInvalid_Ye.appendChild(texteleRectValidInvalid_Ye);

        QDomElement eleExistInvalidRect = doc.createElement("ExistInvalidRect");
        QDomText texteleExistInvalidRect=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_ExistInvalidRect?1:0));
        eleExistInvalidRect.appendChild(texteleExistInvalidRect);


        QDomElement eleIsCheck = doc.createElement("ExistInvalidRect");
        QDomText texteleIsCheck=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_IsCheck ? 1 : 0));
        eleIsCheck.appendChild(texteleIsCheck);

        QDomElement eleVaild = doc.createElement("Vaild");
        QDomText texteleVaild=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_Vaild ? 1 : 0));
        eleVaild.appendChild(texteleVaild);

        QDomElement eleRef_X = doc.createElement("Ref_X");
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X!="")
        {
            QDomText texteleRef_X=doc.createTextNode(QString(this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str()));
            eleRef_X.appendChild(texteleRef_X);
        }

        QDomElement eleRef_Y = doc.createElement("Ref_Y");
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y!="")
        {
            QDomText texteleRef_Y=doc.createTextNode(QString(this->m_StuCheckOperatorBasic->Basic_Ref_Y.c_str()));
            eleRef_Y.appendChild(texteleRef_Y);
        }

        QDomElement eleRef_Angle = doc.createElement("Ref_Angle");
        QDomText texteleRef_Angle=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_Ref_Angle));
        eleRef_Angle.appendChild(texteleRef_Angle);

        QDomElement eleAsRef = doc.createElement("AsRef");
        QDomText texteleAsRef=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_AsRef));
        eleAsRef.appendChild(texteleAsRef);


        QDomElement eleAsDir = doc.createElement("AsDir");
        QDomText texteleAsDir=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_AsDir));
        eleAsDir.appendChild(texteleAsDir);

        QDomElement eleAsContinusCheck = doc.createElement("AsContinusCheck");
        QDomText texteleAsContinusCheck=doc.createTextNode(QString::number(this->m_StuCheckOperatorBasic->Basic_AsContinusCheck));
        eleAsContinusCheck.appendChild(texteleAsContinusCheck);




        StuGrayConversionControl *  eleStuGrayConversionControl = new StuGrayConversionControl(this->m_StuCheckOperatorBasic->Basic_GrayConversion);
        QDomElement * eleGrayConversion = eleStuGrayConversionControl->generateXmlElementBlock2(doc);
        delete eleStuGrayConversionControl;

        eleBasic->appendChild(eleRectValid_Xs);
        eleBasic->appendChild(eleRectValid_Ys);
        eleBasic->appendChild(eleRectValid_Xe);
        eleBasic->appendChild(eleRectValid_Ye);
        eleBasic->appendChild(eleRectValidInvalid_Xs);
        eleBasic->appendChild(eleRectValidInvalid_Ys);
        eleBasic->appendChild(eleRectValidInvalid_Xe);
        eleBasic->appendChild(eleRectValidInvalid_Ye);
        eleBasic->appendChild(eleExistInvalidRect);
        eleBasic->appendChild(eleIsCheck);
        eleBasic->appendChild(eleVaild);
        eleBasic->appendChild(eleRef_X);
        eleBasic->appendChild(eleRef_Y);
        eleBasic->appendChild(eleRef_Angle);
        eleBasic->appendChild(eleAsRef);
        eleBasic->appendChild(eleAsDir);
        eleBasic->appendChild(eleAsContinusCheck);
        eleBasic->appendChild(*eleGrayConversion);
        return eleBasic;
    }
    
    int getRefX(std::vector<std::string>& vStr)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X=="") return 0;
        
        split(this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str(),",",vRes);
        std::vector<std::string>::iterator it=vRes.begin();
        for(;it!=vRes.end();++it)
        {
            if(*it!="0") vStr.push_back(*it);
        }
        return  vStr.size();
    }
    
    int getRefX(std::vector<int>& vStr)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X=="") return 0;
        
        split(this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str(),",",vRes);
        std::vector<std::string>::iterator it=vRes.begin();
        for(;it!=vRes.end();++it)
        {
            if(*it!="0") vStr.push_back(std::atoi((*it).c_str()));
        }
        return  vStr.size();
    }
    
    
    
    int getRefY(std::vector<std::string>& vStr)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y=="") return 0;
        split(this->m_StuCheckOperatorBasic->Basic_Ref_Y.c_str(),",",vRes);
        std::vector<std::string>::iterator it=vRes.begin();
        for(;it!=vRes.end();++it)
        {
            if(*it!="0") vStr.push_back(*it);
        }
        
        return  vStr.size();
    }
    
    int getRefY(std::vector<int>& vStr)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y=="") return 0;
        split(this->m_StuCheckOperatorBasic->Basic_Ref_Y.c_str(),",",vRes);
        std::vector<std::string>::iterator it=vRes.begin();
        for(;it!=vRes.end();++it)
        {
            if(*it!="0") vStr.push_back(std::atoi((*it).c_str()));
        }
        
        return  vStr.size();
    }
    
    
    void addRefX(int checkOperatorNo)
    {
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X=="")this->m_StuCheckOperatorBasic->Basic_Ref_X=std::to_string(checkOperatorNo);
        else this->m_StuCheckOperatorBasic->Basic_Ref_X+=","+std::to_string(checkOperatorNo);
    }
    
    void addRefY(int checkOperatorNo)
    {
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y=="")this->m_StuCheckOperatorBasic->Basic_Ref_Y=std::to_string(checkOperatorNo);
        else this->m_StuCheckOperatorBasic->Basic_Ref_Y+=","+std::to_string(checkOperatorNo);
    }
    
    void delRefX(int checkOperatorNo)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_X=="") return;
        split(this->m_StuCheckOperatorBasic->Basic_Ref_X.c_str(),",",vRes);
        std::vector<std::string> newV;
        for(int idx=0;idx<vRes.size();idx++)
        {
            if(std::atoi(vRes[idx].c_str())==checkOperatorNo) continue;
            newV.push_back(vRes[idx]);
        }
        std::string newRefX;
        if(newV.size()==0)
        {
            this->m_StuCheckOperatorBasic->Basic_Ref_X="";
            return;
        }
        for(int idx=0;idx<newV.size();idx++)
        {
            if(idx!=0) newRefX+=",";
            newRefX+=newV[idx];
        }
        this->m_StuCheckOperatorBasic->Basic_Ref_X=newRefX;
    }
    
    void delRefY(int checkOperatorNo)
    {
        std::vector<std::string> vRes;
        if(this->m_StuCheckOperatorBasic->Basic_Ref_Y=="") return;
        split(this->m_StuCheckOperatorBasic->Basic_Ref_Y.c_str(),",",vRes);
        std::vector<std::string> newV;
        for(int idx=0;idx<vRes.size();idx++)
        {
            if(std::atoi(vRes[idx].c_str())==checkOperatorNo) continue;
            newV.push_back(vRes[idx]);
        }
        std::string newRefY;
        if(newV.size()==0)
        {
            this->m_StuCheckOperatorBasic->Basic_Ref_Y="";
            return;
        }
        for(int idx=0;idx<newV.size();idx++)
        {
            if(idx!=0) newRefY+=",";
            newRefY+=newV[idx];
        }
        this->m_StuCheckOperatorBasic->Basic_Ref_Y=newRefY;
    }
    
private:
    void split(string &str, string delimit, vector<string>&result) {
        size_t pos = str.find(delimit);
        str += delimit;
        while (pos != string::npos) {
            result.push_back(str.substr(0, pos));
            str = str.substr(pos + 1);
            pos = str.find(delimit);
        }
    }
    
    void split(const char* str, const char* delim, vector<string>& ret)
    {
        char* strcopy = new char[strlen(str) + 1];
        strcpy(strcopy, str);
        char *word = strtok(strcopy, delim);
        ret.push_back(word);
        while (word = strtok(nullptr, delim))
            ret.push_back(word);
        delete[] strcopy;
    }
    
public:
    StuCheckOperatorBasic* m_StuCheckOperatorBasic;
};



//关联检查算子
class CheckOperatorAssociatedInspectControl :public CheckOperatorControlBase
{
public:
    CheckOperatorAssociatedInspectControl(CheckOperatorAssociatedInspect* clsCheckOperatorAssociatedInspect) :ClsCheckOperatorAssociatedInspect(clsCheckOperatorAssociatedInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        string temp = "";
        this->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic->Index=pEleParent->IntAttribute("Index");
        
        XmlHelper::GetElementTextFromParent("StdDis", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->StdDis = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("MaxShift", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->MaxShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Relaty1", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->Relaty1 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Relaty2", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->Relaty2 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->LineDir = (CheckOperatorAssociatedInspect::ENUMLINEDIR)atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("LineType", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect->LineType = (CheckOperatorAssociatedInspect::ENUMLINETYPE)atoi(temp.c_str());
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  eleStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic);
        eleStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete eleStuCheckOperatorBasicControl;
        
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorAssociatedInspect->CheckType);
        
        XMLElement* eleStdDis = doc.NewElement("StdDis");
        eleStdDis->SetText(this->ClsCheckOperatorAssociatedInspect->StdDis);
        XMLElement* eleMaxShift = doc.NewElement("MaxShift");
        eleMaxShift->SetText(this->ClsCheckOperatorAssociatedInspect->MaxShift);
        XMLElement* eleRelaty1 = doc.NewElement("Relaty1");
        eleRelaty1->SetText(this->ClsCheckOperatorAssociatedInspect->Relaty1);
        XMLElement* eleRelaty2 = doc.NewElement("Relaty2");
        eleRelaty2->SetText(this->ClsCheckOperatorAssociatedInspect->Relaty2);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText((int)this->ClsCheckOperatorAssociatedInspect->LineDir);
        
        XMLElement* eleLineType = doc.NewElement("LineType");
        eleLineType->SetText((int)this->ClsCheckOperatorAssociatedInspect->LineType);
        
        StuCheckOperatorBasicControl *  eleStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic = eleStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        
        delete eleStuCheckOperatorBasicControl;
        
        eleCheckNo->InsertEndChild(eleStdDis);
        eleCheckNo->InsertEndChild(eleMaxShift);
        eleCheckNo->InsertEndChild(eleRelaty1);
        eleCheckNo->InsertEndChild(eleRelaty2);
        eleCheckNo->InsertEndChild(eleLineDir);
        eleCheckNo->InsertEndChild(eleLineType);
        eleCheckNo->InsertEndChild(eleBasic);
        return eleCheckNo;
    }
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur,int c11,int c12,int c21,int c22)
    {

        return 0;
    }
    
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur,int c1,int c2)
    {

        return 0;
    }
    
public:
    CheckOperatorAssociatedInspect * ClsCheckOperatorAssociatedInspect;
};


//椭圆检查算子
class CheckOperatorCircleInspectControl:public CheckOperatorControlBase
{
public:
    CheckOperatorCircleInspectControl(CheckOperatorCircleInspect* clsCheckOperatorCircleInspect) :ClsCheckOperatorCircleInspect(clsCheckOperatorCircleInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string str = pEleParent->Name();
        int idex=pEleParent->IntAttribute("Index");
        int type = pEleParent->IntAttribute("Type");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("ThreshLowOut", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshLowOut = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("ThreshHighOut", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshHighOut = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("ThreshLowIn", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshLowIn = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("ThreshHighIn", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshHighIn = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("PresOut", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->PresOut = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PresIn", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->PresIn = atoi(temp.c_str());


        XmlHelper::GetElementTextFromParent("ThreshInCirlVal", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshInCirlVal = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("ThreshOutCirlVal", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->ThreshOutCirlVal = atoi(temp.c_str());


        XmlHelper::GetElementTextFromParent("MinCirleArea", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->MinCirleArea = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("MaxCirleArea", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->MaxCirleArea = atoi(temp.c_str());


        XmlHelper::GetElementTextFromParent("PresCircle", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->PresCircle = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("NoCheck", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->NoCheck = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("Thrick", pEleParent, temp);
        this->ClsCheckOperatorCircleInspect->Thrick = atoi(temp.c_str());


        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorCircleInspect->CheckType);
        
        XMLElement* eleThreshLowOut = doc.NewElement("ThreshLowOut");
        eleThreshLowOut->SetText(this->ClsCheckOperatorCircleInspect->ThreshLowOut);
        XMLElement* eleThreshHighOut = doc.NewElement("ThreshHighOut");
        eleThreshHighOut->SetText(this->ClsCheckOperatorCircleInspect->ThreshHighOut);
        XMLElement* eleThreshLowIn = doc.NewElement("ThreshLowIn");
        eleThreshLowIn->SetText(this->ClsCheckOperatorCircleInspect->ThreshLowIn);
        XMLElement* eleThreshHighIn = doc.NewElement("ThreshHighIn");
        eleThreshHighIn->SetText(this->ClsCheckOperatorCircleInspect->ThreshHighIn);
        XMLElement* elePresOut = doc.NewElement("PresOut");
        elePresOut->SetText(this->ClsCheckOperatorCircleInspect->PresOut);
        XMLElement* elePresIn = doc.NewElement("PresIn");
        elePresIn->SetText(this->ClsCheckOperatorCircleInspect->PresIn);


        XMLElement* elePresInGrayVal = doc.NewElement("ThreshInCirlVal");
        elePresInGrayVal->SetText(this->ClsCheckOperatorCircleInspect->ThreshInCirlVal);
        XMLElement* elePresOutGrayVal = doc.NewElement("ThreshOutCirlVal");
        elePresOutGrayVal->SetText(this->ClsCheckOperatorCircleInspect->ThreshOutCirlVal);

        XMLElement* eleMinCirleArea = doc.NewElement("MinCirleArea");
        eleMinCirleArea->SetText(this->ClsCheckOperatorCircleInspect->MinCirleArea);
        XMLElement* eleMaxCirleArea = doc.NewElement("MaxCirleArea");
        eleMaxCirleArea->SetText(this->ClsCheckOperatorCircleInspect->MaxCirleArea);



        XMLElement* elePresCircle = doc.NewElement("PresCircle");
        elePresCircle->SetText(this->ClsCheckOperatorCircleInspect->PresCircle);
        XMLElement* eleNoCheck= doc.NewElement("NoCheck");
        eleNoCheck->SetText(this->ClsCheckOperatorCircleInspect->NoCheck);

        XMLElement* eleThrick= doc.NewElement("Thrick");
        eleThrick->SetText(this->ClsCheckOperatorCircleInspect->Thrick);

        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        
        eleCheckNo->InsertEndChild(eleThreshLowOut);
        eleCheckNo->InsertEndChild(eleThreshHighOut);
        eleCheckNo->InsertEndChild(eleThreshLowIn);
        eleCheckNo->InsertEndChild(eleThreshHighIn);
        eleCheckNo->InsertEndChild(elePresOut);
        eleCheckNo->InsertEndChild(elePresIn);
        eleCheckNo->InsertEndChild(elePresInGrayVal);
        eleCheckNo->InsertEndChild(elePresOutGrayVal);
        eleCheckNo->InsertEndChild(elePresCircle);

        eleCheckNo->InsertEndChild(eleMinCirleArea);
        eleCheckNo->InsertEndChild(eleMaxCirleArea);

        eleCheckNo->InsertEndChild(eleNoCheck);
        eleCheckNo->InsertEndChild(eleThrick);
        eleCheckNo->InsertEndChild(eleBasic);

        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");
        QDomElement* eleCheckNo = new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorCircleInspect->CheckType);

        QDomElement eleThreshLowOut=doc.createElement("eleThreshLowOut");
        QDomText texteleThreshLowOut=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->ThreshLowOut));
        eleThreshLowOut.appendChild(texteleThreshLowOut);

        QDomElement eleThreshHighOut=doc.createElement("ThreshHighOut");
        QDomText texteleThreshHighOut=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->ThreshHighOut));
        eleThreshHighOut.appendChild(texteleThreshHighOut);


        QDomElement eleThreshLowIn=doc.createElement("ThreshLowIn");
        QDomText texteleThreshLowIn=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->ThreshLowIn));
        eleThreshLowIn.appendChild(texteleThreshLowIn);


        QDomElement eleThreshHighIn=doc.createElement("ThreshHighIn");
        QDomText texteleThreshHighIn=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->ThreshHighIn));
        eleThreshHighIn.appendChild(texteleThreshHighIn);


        QDomElement elePresOut=doc.createElement("PresOut");
        QDomText textelePresOut=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->PresOut));
        elePresOut.appendChild(textelePresOut);

        QDomElement elePresIn=doc.createElement("PresIn");
        QDomText textelePresIn=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->PresIn));
        elePresIn.appendChild(textelePresIn);

        QDomElement elePresCircle=doc.createElement("PresCircle");
        QDomText textelePresCircle=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->PresCircle));
        elePresCircle.appendChild(textelePresCircle);

        QDomElement eleNoCheck=doc.createElement("texteleNoCheck");
        QDomText texteleNoCheck=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->NoCheck));
        eleNoCheck.appendChild(texteleNoCheck);

        QDomElement eleThrick=doc.createElement("Thrick");
        QDomText texteleThrick=doc.createTextNode(QString::number(this->ClsCheckOperatorCircleInspect->Thrick));
        eleThrick.appendChild(texteleThrick);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorCircleInspect->stu_CheckOperatorBasic);
        QDomElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;


        eleCheckNo->appendChild(eleThreshLowOut);
        eleCheckNo->appendChild(eleThreshHighOut);
        eleCheckNo->appendChild(eleThreshLowIn);
        eleCheckNo->appendChild(eleThreshHighIn);
        eleCheckNo->appendChild(elePresOut);
        eleCheckNo->appendChild(elePresIn);
        eleCheckNo->appendChild(elePresCircle);
        eleCheckNo->appendChild(eleNoCheck);
        eleCheckNo->appendChild(eleThrick);
        eleCheckNo->appendChild(*eleBasic);

        return eleCheckNo;
    }

    void getLargestConnecttedComponent(cv::Mat srcImage,cv:: Mat &dstImage)
    {
        cv::Mat temp;
        cv::Mat labels;
        srcImage.copyTo(temp);


        int n_comps = connectedComponents(temp, labels, 4, CV_16U);
        vector<int> histogram_of_labels;
        for (int i = 0; i < n_comps; i++)
        {
            histogram_of_labels.push_back(0);
        }

        int rows = labels.rows;
        int cols = labels.cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
            }
        }
        histogram_of_labels.at(0) = 0;


        int maximum = 0;
        int max_idx = 0;
        for (int i = 0; i < n_comps; i++)
        {
            if (histogram_of_labels.at(i) > maximum)
            {
                maximum = histogram_of_labels.at(i);
                max_idx = i;
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (labels.at<unsigned short>(row, col) == max_idx)
                {
                    labels.at<unsigned short>(row, col) = 255;
                }
                else
                {
                    labels.at<unsigned short>(row, col) = 0;
                }
            }
        }


        labels.convertTo(dstImage, CV_8U);
    }



    void fill_up(cv::Mat src, cv::Mat &dst)
    {
        dst = cv::Mat(src.size(), CV_8UC1, cv::Scalar(0));
        vector<vector<cv::Point>> contours;
        findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        drawContours(dst, contours, -1, cv::Scalar(255), cv::FILLED);
    }
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        //siping
        //        {
        //            int ret=0;
        //            clock_t starttime=clock();
        //            cv::Mat areaImg=cv::Mat::zeros(cur.size(),CV_8UC1);
        //            // cv::Mat dst=cur.clone();

        //            cv::Mat imgBin;


        //            adaptiveThreshold(cur, imgBin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 0);
        //            cv::Mat structureElementOpen = getStructuringElement(cv::MORPH_RECT, cv::Size(5,5), cv::Point(-1, -1));
        //            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

        //            cv::Mat structureElementClose = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));


        //            cv::Mat imgBinApt;
        //            cv::morphologyEx(imgBin, imgBinApt, cv::MORPH_OPEN, structureElementOpen);

        //            cv::Mat unit;
        //            getLargestConnecttedComponent(imgBinApt, unit);

        //            cv::morphologyEx(unit, unit, cv::MORPH_CLOSE, structureElementOpen);//only wuzhong



        //            //先计算分割出的滤嘴边缘面积是否在合理的范围
        //            int minCirleArea = 0;
        //            int maxCirleArea=0;



        //            vector<vector<cv::Point>> contours;
        //            vector<cv::Vec4i> hierarchy;
        //            cv::Vec<int,4> vec4;
        //            cv::findContours(unit, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());

        //            //轮廓点少于50判定为异常轮廓，删除
        //            std::vector<vector<cv::Point>>::iterator itConter;
        //            for (itConter = contours.begin();itConter!=contours.end();)
        //            {
        //                if (itConter->size()<40)//siping

        //                {
        //                    itConter = contours.erase(itConter);
        //                }
        //                else
        //                {
        //                    itConter++;
        //                }
        //            }


        //            //先考虑轮廓为1的情况
        //            //计算烟丝异常的面积
        //            int yansiArea = 0;
        //            cv::Mat yansiImg = cv::Mat::zeros(cur.size(), CV_8UC1);
        //            vector<vector<cv::Point>> yansiContours1;
        //            if (contours.size() == 1)
        //            {

        //                return  1;

        //                cv::Rect rect=cv::boundingRect(contours[0]);
        //                int x = rect.x;
        //                int y = rect.y;
        //                for (int i=0;i<rect.height;i++)
        //                {
        //                    for (int j=0;j<rect.width;j++)
        //                    {
        //                        if (ClsCheckOperatorCircleInspect->ThreshLowOut <= cur.at<uchar>(y + i, x + j)&& cur.at<uchar>(y + i, x + j) <= ClsCheckOperatorCircleInspect->ThreshHighOut)
        //                        {
        //                            yansiImg.at<uchar>(y + i, x + j) = 255;
        //                        }
        //                    }
        //                }
        //                cv::morphologyEx(yansiImg, yansiImg, cv::MORPH_OPEN, structureElementOpen2);

        //                getLargestConnecttedComponent(yansiImg, yansiImg);
        //                for (int r = 0; r < yansiImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < yansiImg.cols; ++c)
        //                    {

        //                        if(unit.at<uchar>(r, c))
        //                        {
        //                            minCirleArea++;
        //                        }
        //                        //                    if(cur.at<uchar>(r,c)>210)
        //                        //                    {
        //                        //                        maxCirleArea++;
        //                        //                    }


        //                        if (yansiImg.at<uchar>(r, c))
        //                        {
        //                            yansiArea++;
        //                        }
        //                    }
        //                }

        //                //if (mincirleArea < ClsCheckOperatorCircleInspect->MinCirleArea)
        //                if (minCirleArea < 1100 || minCirleArea> 1900)// siping
        //                {
        //                    ret = 1;
        //                    return  ret;
        //                }
        //                // if (  maxCirleArea> ClsCheckOperatorCircleInspect->MaxCirleArea)

        //                if (  maxCirleArea> 2000)
        //                {
        //                    ret = 1;
        //                    return  ret;
        //                }

        //                if(yansiArea==yansiImg.cols*yansiImg.rows)
        //                {
        //                    yansiArea=0;
        //                }

        //                if (yansiArea>ClsCheckOperatorCircleInspect->PresOut)
        //                {
        //                    ret= 1;
        //                    return ret;
        //                }

        //            }

        //            //轮廓为2的情况
        //            int yansiArea2 = 0;
        //            cv::Mat yansiImg2 = cv::Mat::zeros(cur.size(), CV_8UC1);
        //            vector<vector<cv::Point>> yansiContours2;
        //            if (contours.size() == 2)
        //            {

        //                int minindex = contours[0].size() <= contours[1].size() ? 0 : 1;
        //                int maxindex = contours[0].size() <= contours[1].size() ? 1 : 0;

        //                //计算内圆烟丝

        //                cv::Rect rect = cv::boundingRect(contours[minindex]);
        //                int x = rect.x;
        //                int y = rect.y;
        //                for (int i = 0; i < rect.height; i++)
        //                {
        //                    for (int j = 0; j < rect.width; j++)
        //                    {
        //                        if (ClsCheckOperatorCircleInspect->ThreshLowOut  <= cur.at<uchar>(y + i, x + j) && cur.at<uchar>(y + i, x + j) <= ClsCheckOperatorCircleInspect->ThreshHighOut)
        //                        {
        //                            yansiImg2.at<uchar>(y + i, x + j) = 255;
        //                        }
        //                    }
        //                }
        //                cv::morphologyEx(yansiImg2, yansiImg2, cv::MORPH_OPEN, structureElementOpen2);

        //                getLargestConnecttedComponent(yansiImg2, yansiImg2);
        //                for (int r = 0; r < yansiImg2.rows; ++r)
        //                {
        //                    for (int c = 0; c < yansiImg2.cols; ++c)
        //                    {

        //                        if(unit.at<uchar>(r, c))
        //                        {
        //                            minCirleArea++;
        //                        }
        //                        //                    if(cur.at<uchar>(r,c)>190)
        //                        //                    {
        //                        //                        maxCirleArea++;
        //                        //                    }

        //                        if (yansiImg2.at<uchar>(r, c))
        //                        {
        //                            yansiArea2++;
        //                        }
        //                    }
        //                }


        //                if (minCirleArea < 1100 || minCirleArea> 1900)// siping
        //                {
        //                    ret = 1;
        //                    return ret;
        //                }


        //                if(yansiArea2==yansiImg2.cols*yansiImg2.rows)
        //                {
        //                    yansiArea2=0;
        //                }

        //                if (yansiArea2 >ClsCheckOperatorCircleInspect->PresOut)
        //                {
        //                    ret = 1;
        //                    return ret;
        //                }


        //                //计算内圆长宽比 //siping
        //                cv::RotatedRect rotatedRect = minAreaRect(contours[minindex]);
        //                int rWid = abs(rotatedRect.size.width);
        //                int rHei = abs(rotatedRect.size.height);
        //                if (rWid == 0 || rHei == 0) return 4;
        //                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

        //                //拟合内圆
        //                int sigleForegnArea = 0;

        //                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
        //                cv::Mat fitForegnImg= cv::Mat::zeros(cur.size(), CV_8UC1);

        //                cv::RotatedRect rrt = cv::fitEllipse(contours[minindex]);
        //                int fWid=rrt.size.width;
        //                int fHei=rrt.size.height;
        //                double dwhscale2 = (double)min(fWid, fHei) / (double)max(fWid, fHei);
        //                int whscale =min( round(dwhscale * 100),round(dwhscale2 * 100));

        //                if (whscale <ClsCheckOperatorCircleInspect->PresCircle)
        //                {
        //                    ret= 1;
        //                    return ret;
        //                }



        //                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);

        //                for (int r = 0; r < fitInnerImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < fitInnerImg.cols; ++c)
        //                    {
        //                        if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c))
        //                        {
        //                            fitForegnImg.at<uchar>(r,c)=255;
        //                        }
        //                    }
        //                }



        //                cv::morphologyEx(fitForegnImg, fitForegnImg, cv::MORPH_OPEN, structureElementOpen2);

        //                getLargestConnecttedComponent(fitForegnImg, fitForegnImg);

        //                for (int r = 0; r < fitForegnImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < fitForegnImg.cols; ++c)
        //                    {
        //                        if (fitForegnImg.at<uchar>(r, c))
        //                        {
        //                            sigleForegnArea++;
        //                        }
        //                    }
        //                }


        //                if(sigleForegnArea>0&&sigleForegnArea<fitForegnImg.cols*fitForegnImg.rows)
        //                {
        //                    sigleForegnArea=0;
        //                    for (int r = 0; r < fitForegnImg.rows; ++r)
        //                    {
        //                        for (int c = 0; c < fitForegnImg.cols; ++c)
        //                        {
        //                            if (fitForegnImg.at<uchar>(r, c)&&cur.at<uchar>(r,c)>=ClsCheckOperatorCircleInspect->ThreshInCirlVal)
        //                            {
        //                                sigleForegnArea++;
        //                            }
        //                        }
        //                    }
        //                }
        //                else
        //                {
        //                    sigleForegnArea=0;
        //                }



        //                //内圆异常
        //                if (sigleForegnArea >ClsCheckOperatorCircleInspect->PresIn)
        //                {
        //                    ret= 1;
        //                    return ret;
        //                }


        //            }

        //            //其他异常
        //            if(contours.size()==0||contours.size()>=3)
        //            {
        //                ret=1;
        //                return ret;

        //            }

        //            return  ret;

        //        }
        //wuzhong
        {

            ClsCheckOperatorCircleInspect->PresOut;

            int threshOutLow =ClsCheckOperatorCircleInspect->ThreshLowOut;
            int threshOutHigh = ClsCheckOperatorCircleInspect->ThreshHighOut;
            int ThreshInCirlVal = ClsCheckOperatorCircleInspect->ThreshInCirlVal;
            int ThreshOutCirlVal =  ClsCheckOperatorCircleInspect->ThreshOutCirlVal;
            int presOut = ClsCheckOperatorCircleInspect->PresOut;
            int presIn = ClsCheckOperatorCircleInspect->PresIn;
            int presCircle = ClsCheckOperatorCircleInspect->PresCircle;


            int ret=0;
            clock_t starttime=clock();
            cv::Mat areaImg=cv::Mat::zeros(cur.size(),CV_8UC1);
            //            cv::Mat tmp=cur.clone();

            cv::Mat imgBin;
            cv::Mat structureElementOpen = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

            cv::Mat structureElementClose = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(-1, -1));

            cv::Mat structureElementERODE = getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6), cv::Point(-1, -1));
            cv::Mat structureElementERODE2 = getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11), cv::Point(-1, -1));


            adaptiveThreshold(cur, imgBin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 0);
            //            cv::threshold(tmp,imgBin,ThreshInCirlVal,255,cv::THRESH_BINARY);



            cv::Mat imgBinApt, imgBinAptfillup;
            cv::Mat unit;
            //填充

            fill_up(imgBin, imgBinAptfillup);
            cv::morphologyEx(imgBinAptfillup, imgBinAptfillup, cv::MORPH_OPEN, structureElementOpen);
            cv::morphologyEx(imgBinAptfillup, imgBinAptfillup, cv::MORPH_ERODE, structureElementERODE);
            cv::Mat unitfillup;
            getLargestConnecttedComponent(imgBinAptfillup, unitfillup);
            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_DILATE, structureElementERODE);

            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_DILATE, structureElementERODE2);
            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_ERODE, structureElementERODE2);

            {
                int bobiArea = 0;
                for (int r = 0; r < imgBin.rows; ++r)
                {
                    for (int c = 0; c < imgBin.cols; ++c)
                    {
                        if (unitfillup.at<uchar>(r, c)&& cur.at<uchar>(r, c)> ThreshInCirlVal)
                        {
                            bobiArea++;

                        }
                    }
                }
                if (bobiArea > presIn  &&bobiArea<threshOutHigh)
                {

                }
                else
                {
                    return 1;
                }
            }


            vector<vector<cv::Point>> contours;
            vector<cv::Vec4i> hierarchy;
            cv::Vec<int, 4> vec4;
            cv::findContours(unitfillup, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
            if (contours.size()>=2)
            {
                fill_up(unitfillup, unitfillup);
                cv::findContours(unitfillup, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
            }

            //轮廓点少于60判定为异常轮廓，删除
//                        std::vector<vector<cv::Point>>::iterator itConter;
//                        for (itConter = contours.begin(); itConter != contours.end();)
//                        {
//                            //if (itConter->size()<40)siping
//                            if (itConter->size() < 100)//wuzhong
//                            {
//                                itConter = contours.erase(itConter);
//                            }
//                            else
//                            {
//                                itConter++;
//                            }
//                        }


            //先考虑轮廓为1的情况
            vector<vector<cv::Point>> yansiContours1;
            if (contours.size() == 1)
            {
                //计算外圆长宽比 wuzhong
                cv::RotatedRect rotatedRect = minAreaRect(contours[0]);
                int rWid = abs(rotatedRect.size.width);
                int rHei = abs(rotatedRect.size.height);
                if (rWid == 0 || rHei == 0) return 4;
                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

                int whscale = round(dwhscale * 100);
                if (whscale < presCircle)
                {
                    return  1;
                }
                else
                {

                }


                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
                int fitForegnare=0;

                cv::RotatedRect rrt = cv::fitEllipse(contours[0]);
                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);



                for (int r = 0; r < fitInnerImg.rows; ++r)
                {
                    for (int c = 0; c < fitInnerImg.cols; ++c)
                    {
                        if (fitInnerImg.at<uchar>(r, c) && cur.at<uchar>(r,c)<threshOutLow &&unitfillup.at<uchar>(r,c)==0)
                        {
                            fitForegnare++;
                        }
                    }
                }

                if (fitForegnare > presOut)
                {
                    return 1;
                }
            }

            //轮廓为2的情况
            int yansiArea2 = 0;
            cv::Mat yansiImg2 = cv::Mat::zeros(cur.size(), CV_8UC1);
            vector<vector<cv::Point>> yansiContours2;
            if (contours.size() >= 2)
            {
                return 1;

                int minindex = contours[0].size() <= contours[1].size() ? 0 : 1;
                int maxindex = contours[0].size() <= contours[1].size() ? 1 : 0;

                //计算内圆烟丝

                cv::Rect rect = cv::boundingRect(contours[minindex]);
                int x = rect.x;
                int y = rect.y;
                for (int i = 0; i < rect.height; i++)
                {
                    for (int j = 0; j < rect.width; j++)
                    {
                        if (ClsCheckOperatorCircleInspect->ThreshLowOut  <= cur.at<uchar>(y + i, x + j) && cur.at<uchar>(y + i, x + j) <= ClsCheckOperatorCircleInspect->ThreshHighOut)
                        {
                            yansiImg2.at<uchar>(y + i, x + j) = 255;
                        }
                    }
                }
                cv::morphologyEx(yansiImg2, yansiImg2, cv::MORPH_OPEN, structureElementOpen2);

                getLargestConnecttedComponent(yansiImg2, yansiImg2);
                for (int r = 0; r < yansiImg2.rows; ++r)
                {
                    for (int c = 0; c < yansiImg2.cols; ++c)
                    {

                        if(unit.at<uchar>(r, c))
                        {
                            //                            minCirleArea++;
                        }
                        //                    if(cur.at<uchar>(r,c)>190)
                        //                    {
                        //                        maxCirleArea++;
                        //                    }

                        if (yansiImg2.at<uchar>(r, c))
                        {
                            yansiArea2++;
                        }
                    }
                }


                //             if (minCirleArea < 1100 || minCirleArea> 1900)// siping
                //                if (minCirleArea < 0 || minCirleArea> 19000)//wuzhong
                {
                    ret = 1;
                    return ret;
                }


                if(yansiArea2==yansiImg2.cols*yansiImg2.rows)
                {
                    yansiArea2=0;
                }

                if (yansiArea2 >ClsCheckOperatorCircleInspect->PresOut)
                {
                    ret = 1;
                    return ret;
                }





                //计算内圆长宽比
                cv::RotatedRect rotatedRect = minAreaRect(contours[maxindex]);
                int rWid = abs(rotatedRect.size.width);
                int rHei = abs(rotatedRect.size.height);
                if (rWid == 0 || rHei == 0) return 4;
                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

                //拟合内圆
                int sigleForegnArea = 0;

                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
                cv::Mat fitForegnImg= cv::Mat::zeros(cur.size(), CV_8UC1);

                cv::RotatedRect rrt = cv::fitEllipse(contours[maxindex]);
                int fWid=rrt.size.width;
                int fHei=rrt.size.height;
                double dwhscale2 = (double)min(fWid, fHei) / (double)max(fWid, fHei);



                rrt = cv::fitEllipse(contours[minindex]);
                fWid=rrt.size.width;
                fHei=rrt.size.height;
                double  dwhscale3 = (double)min(fWid, fHei) / (double)max(fWid, fHei);
                dwhscale3=round(dwhscale3 * 100);




                int whscale =max( round(dwhscale * 100),round(dwhscale2 * 100));


                if(whscale<dwhscale3)
                    whscale=dwhscale3;





                if (whscale <ClsCheckOperatorCircleInspect->PresCircle)
                {
                    ret= 1;
                    return ret;
                }
                rrt = cv::fitEllipse(contours[minindex]);




                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);

                for (int r = 0; r < fitInnerImg.rows; ++r)
                {
                    for (int c = 0; c < fitInnerImg.cols; ++c)
                    {
                        if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c))
                        {
                            fitForegnImg.at<uchar>(r,c)=255;
                        }
                    }
                }



                cv::morphologyEx(fitForegnImg, fitForegnImg, cv::MORPH_OPEN, structureElementOpen2);

                getLargestConnecttedComponent(fitForegnImg, fitForegnImg);

                for (int r = 0; r < fitForegnImg.rows; ++r)
                {
                    for (int c = 0; c < fitForegnImg.cols; ++c)
                    {
                        if (fitForegnImg.at<uchar>(r, c))
                        {
                            sigleForegnArea++;
                        }
                    }
                }


                if(sigleForegnArea>0&&sigleForegnArea<fitForegnImg.cols*fitForegnImg.rows)
                {
                    sigleForegnArea=0;
                    for (int r = 0; r < fitForegnImg.rows; ++r)
                    {
                        for (int c = 0; c < fitForegnImg.cols; ++c)
                        {
                            if (fitForegnImg.at<uchar>(r, c)&&cur.at<uchar>(r,c)>=ClsCheckOperatorCircleInspect->ThreshInCirlVal)
                            {
                                sigleForegnArea++;
                            }
                        }
                    }
                }
                else
                {
                    sigleForegnArea=0;
                }



                //内圆异常
                if (sigleForegnArea >ClsCheckOperatorCircleInspect->PresIn)
                {
                    //                    ret= 1;
                    //                    return ret;
                }


            }

            //其他异常
            if(contours.size()==0||contours.size()>=3)
            {
                return 1;

            }
            if(ret>1)
            {
                qDebug()<<ret<<endl;
            }

            return  ret;
        }


    }
    




    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,cv::Mat& dst, cv::Rect rect, int threshOutLow,int threshOutHigh,
                                int ThreshInCirlVal,int ThreshOutCirlVal,int presOut,int presIn,int presCircle,int * score,int *outerArea,int *inerArea,
                                bool& bPresCircle,bool& bOuterArea,bool& bInerArea)
    {
        //siping
        //        {
        //            int ret=0;
        //            bPresCircle=false;
        //            bOuterArea=false;
        //            bInerArea=false;

        //            clock_t starttime=clock();
        //            cv::Mat   tmp = cur.clone();
        //            dst=cur.clone();

        //            cv::imwrite("./dst.bmp",dst);


        //            cv::Mat imgBin;
        //            adaptiveThreshold(tmp, imgBin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 0);

        //            cv::Mat structureElementOpen = getStructuringElement(cv::MORPH_RECT, cv::Size(5,5), cv::Point(-1, -1));
        //            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

        //            cv::Mat structureElementClose = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));



        //            cv::Mat imgBinApt;


        //            cv::morphologyEx(imgBin, imgBinApt, cv::MORPH_OPEN, structureElementOpen);

        //            cv::Mat unit;
        //            getLargestConnecttedComponent(imgBinApt, unit);


        //            cv::morphologyEx(unit, unit, cv::MORPH_CLOSE, structureElementOpen);//wuzhong


        //            //先计算分割出的滤嘴边缘面积是否在合理的范围
        //            int mincirleArea = 0;
        //            int maxCirleArea=0;



        //            vector<vector<cv::Point>> contours;
        //            vector<cv::Vec4i> hierarchy;
        //            cv::Vec<int,4> vec4;
        //            cv::findContours(unit, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());

        //            //轮廓点少于60判定为异常轮廓，删除
        //            std::vector<vector<cv::Point>>::iterator itConter;
        //            for (itConter = contours.begin();itConter!=contours.end();)
        //            {
        //                if (itConter->size()<40)siping

        //                {
        //                    itConter = contours.erase(itConter);
        //                }
        //                else
        //                {
        //                    itConter++;
        //                }
        //            }


        //            //先考虑轮廓为1的情况
        //            //计算烟丝异常的面积
        //            int yansiArea = 0;
        //            cv::Mat yansiImg = cv::Mat::zeros(cur.size(), CV_8UC1);
        //            vector<vector<cv::Point>> yansiContours1;
        //            if (contours.size() == 1)
        //            {

        //                cv::Rect rect=cv::boundingRect(contours[0]);
        //                int x = rect.x;
        //                int y = rect.y;
        //                for (int i=0;i<rect.height;i++)
        //                {
        //                    for (int j=0;j<rect.width;j++)
        //                    {
        //                        if (threshOutLow <= cur.at<uchar>(y + i, x + j)&& cur.at<uchar>(y + i, x + j) <= threshOutHigh)
        //                        {
        //                            yansiImg.at<uchar>(y + i, x + j) = 255;
        //                        }
        //                    }
        //                }
        //                cv::morphologyEx(yansiImg, yansiImg, cv::MORPH_OPEN, structureElementOpen2);

        //                getLargestConnecttedComponent(yansiImg, yansiImg);
        //                for (int r = 0; r < yansiImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < yansiImg.cols; ++c)
        //                    {

        //                        if(unit.at<uchar>(r, c))
        //                        {
        //                            mincirleArea++;
        //                        }

        //                        if (yansiImg.at<uchar>(r, c))
        //                        {
        //                            yansiArea++;
        //                        }
        //                    }
        //                }

        //                //if (mincirleArea < ClsCheckOperatorCircleInspect->MinCirleArea)
        //                if (mincirleArea < 1100)//四平

        //                {
        //                    ret = 1;
        //                    bPresCircle=true;
        //                    bOuterArea =true;
        //                    bInerArea=true;

        //                    *outerArea=mincirleArea;
        //                    return  ret;


        //                }
        //                // if (  maxCirleArea> ClsCheckOperatorCircleInspect->MaxCirleArea)

        //                if (  mincirleArea> 1900)//四平

        //                {
        //                    ret = 1;
        //                    bPresCircle=true;
        //                    bOuterArea =true;
        //                    bInerArea=true;

        //                    *outerArea=mincirleArea;
        //                    return  ret;
        //                }

        //                if(yansiArea==yansiImg.cols*yansiImg.rows)
        //                {
        //                    yansiArea=0;
        //                }

        //                if (yansiArea>presOut)
        //                {
        //                    cv::findContours(yansiImg, yansiContours1, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
        //                    for (int i = 0; i < yansiContours1.size(); i++)
        //                    {
        //                        cv::drawContours(dst, yansiContours1, i, cv::Scalar(64), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
        //                    }
        //                    bOuterArea=true;
        //                    *outerArea=yansiArea;
        //                    ret= 1;
        //                    return ret;
        //                }
        //                else
        //                {
        //                    bOuterArea=false;
        //                    *outerArea=yansiArea;
        //                    return 0;
        //                }
        //            }


        //            //轮廓为2的情况
        //            int yansiArea2 = 0;
        //            cv::Mat yansiImg2 = cv::Mat::zeros(cur.size(), CV_8UC1);
        //            vector<vector<cv::Point>> yansiContours2;
        //            if (contours.size() == 2)
        //            {

        //                int minindex = contours[0].size() <= contours[1].size() ? 0 : 1;
        //                int maxindex = contours[0].size() <= contours[1].size() ? 1 : 0;

        //                //计算内圆烟丝

        //                cv::Rect rect = cv::boundingRect(contours[minindex]);
        //                int x = rect.x;
        //                int y = rect.y;
        //                for (int i = 0; i < rect.height; i++)
        //                {
        //                    for (int j = 0; j < rect.width; j++)
        //                    {
        //                        if (threshOutLow <= cur.at<uchar>(y + i, x + j) && cur.at<uchar>(y + i, x + j) <= threshOutHigh)
        //                        {
        //                            yansiImg2.at<uchar>(y + i, x + j) = 255;
        //                        }
        //                    }
        //                }
        //                cv::morphologyEx(yansiImg2, yansiImg2, cv::MORPH_OPEN, structureElementOpen2);
        //                getLargestConnecttedComponent(yansiImg2, yansiImg2);
        //                for (int r = 0; r < yansiImg2.rows; ++r)
        //                {
        //                    for (int c = 0; c < yansiImg2.cols; ++c)
        //                    {
        //                        if(unit.at<uchar>(r, c))
        //                        {
        //                            mincirleArea++;
        //                        }
        //                        if (yansiImg2.at<uchar>(r, c))
        //                        {
        //                            yansiArea2++;
        //                        }
        //                    }
        //                }

        //                int min1=ClsCheckOperatorCircleInspect->MinCirleArea ;
        //                int max1=ClsCheckOperatorCircleInspect->MaxCirleArea ;

        //                if (mincirleArea < 1100)//siping

        //                {
        //                    ret = 1;
        //                    bPresCircle=true;
        //                    bOuterArea =true;
        //                    bInerArea=true;

        //                    *outerArea=mincirleArea;



        //                }
        //                if (  mincirleArea> 1900)//siping

        //                {
        //                    ret = 1;
        //                    bPresCircle=true;
        //                    bOuterArea =true;
        //                    bInerArea=true;

        //                    *outerArea=mincirleArea;
        //                }


        //                if(yansiArea2==yansiImg2.cols*yansiImg2.rows)
        //                {
        //                    yansiArea2=0;
        //                }

        //                if (yansiArea2 > presOut)
        //                {
        //                    cv::findContours(yansiImg2, yansiContours2, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
        //                    for (int i = 0; i < yansiContours2.size(); i++)
        //                    {
        //                        cv::drawContours(dst, yansiContours2, i, cv::Scalar(255), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
        //                    }
        //                    ret = 1;
        //                }


        //                //计算内圆长宽比 siping
        //                cv::RotatedRect rotatedRect = minAreaRect(contours[minindex]);
        //                int rWid = abs(rotatedRect.size.width);
        //                int rHei = abs(rotatedRect.size.height);
        //                if (rWid == 0 || rHei == 0) return 4;
        //                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

        //                //            int whscale = round(dwhscale * 100);
        //                //            *score=whscale;


        //                //拟合内圆
        //                int sigleForegnArea = 0;
        //                int tmpfitForegnArea = 0;
        //                cv::Mat matSigleForegn;
        //                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
        //                cv::Mat fitForegnImg= cv::Mat::zeros(cur.size(), CV_8UC1);

        //                cv::RotatedRect rrt = cv::fitEllipse(contours[minindex]);
        //                int fWid=rrt.size.width;
        //                int fHei=rrt.size.height;
        //                double dwhscale2 = (double)min(fWid, fHei) / (double)max(fWid, fHei);


        //                int whscale =min( round(dwhscale * 100),round(dwhscale2 * 100));
        //                *score=whscale;





        //                cv::ellipse(dst, rrt, cv::Scalar(64), 1);
        //                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);

        //                for (int r = 0; r < fitInnerImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < fitInnerImg.cols; ++c)
        //                    {
        //                        //if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c)&& cur.at<uchar>(r,c)>=ThreshInCirlVal)
        //                        if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c))
        //                        {
        //                            fitForegnImg.at<uchar>(r,c)=255;
        //                        }
        //                    }
        //                }


        //                cv::morphologyEx(fitForegnImg, fitForegnImg, cv::MORPH_OPEN, structureElementOpen2);
        //                getLargestConnecttedComponent(fitForegnImg, matSigleForegn);

        //                for (int r = 0; r < fitForegnImg.rows; ++r)
        //                {
        //                    for (int c = 0; c < fitForegnImg.cols; ++c)
        //                    {
        //                        if (matSigleForegn.at<uchar>(r, c))
        //                        {
        //                            sigleForegnArea++;
        //                        }
        //                    }
        //                }
        //                if(sigleForegnArea>0&&sigleForegnArea<matSigleForegn.cols*matSigleForegn.rows)
        //                {
        //                    sigleForegnArea=0;
        //                    for (int r = 0; r < matSigleForegn.rows; ++r)
        //                    {
        //                        for (int c = 0; c < matSigleForegn.cols; ++c)
        //                        {
        //                            if (matSigleForegn.at<uchar>(r, c)&&cur.at<uchar>(r,c)>=ThreshInCirlVal)
        //                            {
        //                                sigleForegnArea++;
        //                            }
        //                        }
        //                    }
        //                }
        //                else
        //                {
        //                    sigleForegnArea=0;
        //                }


        //                *inerArea=sigleForegnArea;

        //                *outerArea=yansiArea2;



        //                if (whscale < presCircle)
        //                {
        //                    ret= 1;
        //                    bPresCircle=true;
        //                }
        //                else
        //                {
        //                    bPresCircle=false;
        //                }

        //                //烟丝异常
        //                if (yansiArea2>presOut)
        //                {
        //                    bOuterArea=true;
        //                    ret= 1;
        //                }
        //                else
        //                {
        //                    bOuterArea=false;
        //                }

        //                //内圆异常
        //                if (sigleForegnArea >presIn)
        //                {

        //                    cv::findContours(matSigleForegn, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());
        //                    drawContours(dst, contours, 0, cv::Scalar(32), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
        //                    ret= 1;
        //                    bInerArea=true;
        //                }
        //                else
        //                {
        //                    bInerArea=false;
        //                }
        //            }

        //            //其他异常
        //            if(contours.size()==0||contours.size()>=3)
        //            {
        //                ret=1;
        //                bPresCircle=true;
        //                bOuterArea =true;
        //                bInerArea=true;
        //            }

        //            return  ret;
        //        }


        //wuzhong
        {
            int ret=0;
            bPresCircle=false;
            bOuterArea=false;
            bInerArea=false;

            clock_t starttime=clock();
            cv::Mat   tmp = cur.clone();
            dst=cur.clone();

            cv::imwrite("./dst.bmp",dst);

            cv::Mat structureElementOpen = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

            cv::Mat structureElementClose = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(-1, -1));

            cv::Mat structureElementERODE = getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6), cv::Point(-1, -1));

            cv::Mat structureElementERODE2 = getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11), cv::Point(-1, -1));


            cv::Mat imgBin;
            adaptiveThreshold(tmp, imgBin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 0);
            //            cv::threshold(tmp,imgBin,ThreshInCirlVal,255,cv::THRESH_BINARY);



            cv::Mat imgBinApt, imgBinAptfillup;
            cv::morphologyEx(imgBin, imgBinApt, cv::MORPH_OPEN, structureElementOpen);
            cv::morphologyEx(imgBinApt, imgBinApt, cv::MORPH_ERODE, structureElementERODE);
            cv::Mat unit;
            getLargestConnecttedComponent(imgBinApt, unit);
            cv::morphologyEx(unit, unit, cv::MORPH_CLOSE, structureElementOpen);//wuzhong
            cv::morphologyEx(unit, unit, cv::MORPH_DILATE, structureElementERODE);


            //填充
            fill_up(imgBin, imgBinAptfillup);
            cv::morphologyEx(imgBinAptfillup, imgBinAptfillup, cv::MORPH_OPEN, structureElementOpen);
            cv::morphologyEx(imgBinAptfillup, imgBinAptfillup, cv::MORPH_ERODE, structureElementERODE);
            cv::Mat unitfillup;
            getLargestConnecttedComponent(imgBinAptfillup, unitfillup);
            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_DILATE, structureElementERODE);

            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_DILATE, structureElementERODE2);
            cv::morphologyEx(unitfillup, unitfillup, cv::MORPH_ERODE, structureElementERODE2);




            {
                int bobiArea = 0;
                for (int r = 0; r < unit.rows; ++r)
                {
                    for (int c = 0; c < unit.cols; ++c)
                    {
                        if (unitfillup.at<uchar>(r, c)&& cur.at<uchar>(r, c)> ThreshInCirlVal)
                        {
                            dst.at<uchar>(r, c) = 250;
                            bobiArea++;

                        }
                    }
                }
                *inerArea = bobiArea;



                if (bobiArea > presIn  &&bobiArea<threshOutHigh)
                {
                    bInerArea = false;
                }
                else
                {
                    ret = 1;
                    bInerArea = true;
                }

            }


            //先计算分割出的滤嘴边缘面积是否在合理的范围
            int mincirleArea = 0;
            int maxCirleArea = 0;



            vector<vector<cv::Point>> contours;
            vector<cv::Vec4i> hierarchy;
            cv::Vec<int, 4> vec4;
            cv::findContours(unitfillup, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());

            if (contours.size()>=2)
            {
                fill_up(unitfillup, unitfillup);
                cv::findContours(unitfillup, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
            }

            //轮廓点少于60判定为异常轮廓，删除
//            std::vector<vector<cv::Point>>::iterator itConter;
//            for (itConter = contours.begin(); itConter != contours.end();)
//            {
//                //if (itConter->size()<40)siping
//                if (itConter->size() < 100)//wuzhong
//                {
//                    itConter = contours.erase(itConter);
//                }
//                else
//                {
//                    itConter++;
//                }
//            }


            //先考虑轮廓为1的情况
            //计算烟丝异常的面积
            int yansiArea = 0;
            cv::Mat yansiImg = cv::Mat::zeros(cur.size(), CV_8UC1);
            vector<vector<cv::Point>> yansiContours1;
            if (contours.size() == 1)
            {
                //                cv::drawContours(dst, contours, 0, cv::Scalar(255), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
                //                cv::drawContours(unitfillup, contours, 0, cv::Scalar(255), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());


                //计算外圆长宽比 wuzhong
                cv::RotatedRect rotatedRect = minAreaRect(contours[0]);
                int rWid = abs(rotatedRect.size.width);
                int rHei = abs(rotatedRect.size.height);
                if (rWid == 0 || rHei == 0) return 4;
                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

                int whscale = round(dwhscale * 100);
                *score = whscale;
                if (whscale < presCircle)
                {
                    ret = 1;
                    bPresCircle = true;
                }
                else
                {
                    bPresCircle = false;
                }


                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
                int fitForegnare=0;

                cv::RotatedRect rrt = cv::fitEllipse(contours[0]);
                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);

                vector<vector<cv::Point>> contours;
                vector<cv::Vec4i> hierarchy;
                cv::Vec<int, 4> vec4;
                cv::findContours(fitInnerImg, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
                cv::drawContours(dst, contours, 0, cv::Scalar(255), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());


                for (int r = 0; r < fitInnerImg.rows; ++r)
                {
                    for (int c = 0; c < fitInnerImg.cols; ++c)
                    {
                        if (fitInnerImg.at<uchar>(r, c) && cur.at<uchar>(r,c)<threshOutLow &&unitfillup.at<uchar>(r,c)==0)


                        {
                            dst.at<uchar>(r,c)=200;
                            fitForegnare++;
                        }
                    }
                }

                *outerArea = fitForegnare;
                if (fitForegnare > presOut)
                {
                    bOuterArea = true;
                    return 1;
                }
            }


            //轮廓为2的情况
            int yansiArea2 = 0;
            cv::Mat yansiImg2 = cv::Mat::zeros(cur.size(), CV_8UC1);
            vector<vector<cv::Point>> yansiContours2;
            if (contours.size() >= 2)
            {
                bPresCircle = true;
                bOuterArea = true;
                bInerArea = true;

                return 1;


                int minindex = contours[0].size() <= contours[1].size() ? 0 : 1;
                int maxindex = contours[0].size() <= contours[1].size() ? 1 : 0;

                //计算内圆烟丝

                cv::Rect rect = cv::boundingRect(contours[minindex]);
                int x = rect.x;
                int y = rect.y;
                for (int i = 0; i < rect.height; i++)
                {
                    for (int j = 0; j < rect.width; j++)
                    {
                        if (threshOutLow <= cur.at<uchar>(y + i, x + j) && cur.at<uchar>(y + i, x + j) <= threshOutHigh)
                        {
                            yansiImg2.at<uchar>(y + i, x + j) = 255;
                        }
                    }
                }
                cv::morphologyEx(yansiImg2, yansiImg2, cv::MORPH_OPEN, structureElementOpen2);
                getLargestConnecttedComponent(yansiImg2, yansiImg2);
                for (int r = 0; r < yansiImg2.rows; ++r)
                {
                    for (int c = 0; c < yansiImg2.cols; ++c)
                    {
                        if(unit.at<uchar>(r, c))
                        {
                            mincirleArea++;
                        }
                        if (yansiImg2.at<uchar>(r, c))
                        {
                            yansiArea2++;
                        }
                    }
                }

                int min1=ClsCheckOperatorCircleInspect->MinCirleArea ;
                int max1=ClsCheckOperatorCircleInspect->MaxCirleArea ;

                // if (mincirleArea < 1100)//siping
                if (mincirleArea < 0)//wuzhong
                {
                    ret = 1;
                    bPresCircle=true;
                    bOuterArea =true;
                    bInerArea=true;

                    *outerArea=mincirleArea;



                }
                //if (  mincirleArea> 1900)//siping
                if (  mincirleArea> 19000)//wuzhong
                {
                    ret = 1;
                    bPresCircle=true;
                    bOuterArea =true;
                    bInerArea=true;

                    *outerArea=mincirleArea;
                }


                if(yansiArea2==yansiImg2.cols*yansiImg2.rows)
                {
                    yansiArea2=0;
                }

                if (yansiArea2 > presOut)
                {
                    cv::findContours(yansiImg2, yansiContours2, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point());
                    for (int i = 0; i < yansiContours2.size(); i++)
                    {
                        cv::drawContours(dst, yansiContours2, i, cv::Scalar(255), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
                    }
                    ret = 1;
                }





                //计算外圆长宽比 wuzhong
                cv::RotatedRect rotatedRect = minAreaRect(contours[maxindex]);
                int rWid = abs(rotatedRect.size.width);
                int rHei = abs(rotatedRect.size.height);
                if (rWid == 0 || rHei == 0) return 4;
                double dwhscale = (double)min(rWid, rHei) / (double)max(rWid, rHei);

                //                            int whscale = round(dwhscale * 100);
                //                            *score=whscale;


                //拟合内圆
                int sigleForegnArea = 0;
                int tmpfitForegnArea = 0;
                cv::Mat matSigleForegn;
                cv::Mat fitInnerImg = cv::Mat::zeros(cur.size(), CV_8UC1);
                cv::Mat fitForegnImg= cv::Mat::zeros(cur.size(), CV_8UC1);

                cv::RotatedRect rrt = cv::fitEllipse(contours[maxindex]);
                int fWid=rrt.size.width;
                int fHei=rrt.size.height;
                double dwhscale2 = (double)min(fWid, fHei) / (double)max(fWid, fHei);

                rrt = cv::fitEllipse(contours[minindex]);
                fWid=rrt.size.width;
                fHei=rrt.size.height;
                double  dwhscale3 = (double)min(fWid, fHei) / (double)max(fWid, fHei);
                dwhscale3=round(dwhscale3 * 100);




                int whscale =max( round(dwhscale * 100),round(dwhscale2 * 100));


                if(whscale<dwhscale3)
                    whscale=dwhscale3;





                *score=whscale;
                rrt = cv::fitEllipse(contours[minindex]);





                cv::ellipse(dst, rrt, cv::Scalar(64), 1);
                cv::ellipse(fitInnerImg, rrt, cv::Scalar(64), -1);

                for (int r = 0; r < fitInnerImg.rows; ++r)
                {
                    for (int c = 0; c < fitInnerImg.cols; ++c)
                    {
                        //if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c)&& cur.at<uchar>(r,c)>=ThreshInCirlVal)
                        if (fitInnerImg.at<uchar>(r, c) &&unit.at<uchar>(r,c))
                        {
                            fitForegnImg.at<uchar>(r,c)=255;
                        }
                    }
                }


                cv::morphologyEx(fitForegnImg, fitForegnImg, cv::MORPH_OPEN, structureElementOpen2);
                getLargestConnecttedComponent(fitForegnImg, matSigleForegn);

                for (int r = 0; r < fitForegnImg.rows; ++r)
                {
                    for (int c = 0; c < fitForegnImg.cols; ++c)
                    {
                        if (matSigleForegn.at<uchar>(r, c))
                        {
                            sigleForegnArea++;
                        }
                    }
                }
                if(sigleForegnArea>0&&sigleForegnArea<matSigleForegn.cols*matSigleForegn.rows)
                {
                    sigleForegnArea=0;
                    for (int r = 0; r < matSigleForegn.rows; ++r)
                    {
                        for (int c = 0; c < matSigleForegn.cols; ++c)
                        {
                            if (matSigleForegn.at<uchar>(r, c)&&cur.at<uchar>(r,c)>=ThreshInCirlVal)
                            {
                                sigleForegnArea++;
                            }
                        }
                    }
                }
                else
                {
                    sigleForegnArea=0;
                }


                //                *inerArea=sigleForegnArea;

                *outerArea=yansiArea2;



                if (whscale < presCircle)
                {
                    ret= 1;
                    bPresCircle=true;
                }
                else
                {
                    bPresCircle=false;
                }

                //烟丝异常
                if (yansiArea2>presOut)
                {
                    bOuterArea=true;
                    ret= 1;
                }
                else
                {
                    bOuterArea=false;
                }

                //内圆异常
                //                if (sigleForegnArea >presIn)
                //                {

                //                    cv::findContours(matSigleForegn, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());
                //                    drawContours(dst, contours, 0, cv::Scalar(32), 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
                //                    ret= 1;
                //                    bInerArea=true;
                //                }
                //                else
                //                {
                //                    bInerArea=false;
                //                }
            }

            //其他异常
            if(contours.size()==0||contours.size()>=3)
            {
                //                ret=1;
                //                bPresCircle=true;
                //                bOuterArea =true;
                //                bInerArea=true;
            }

            return  ret;
        }
    }
    

public:
    CheckOperatorCircleInspect * ClsCheckOperatorCircleInspect;
};

//目标检测
class CheckOperatorDLObjectDetectControl:public CheckOperatorControlBase
{
public:
    CheckOperatorDLObjectDetectControl(CheckOperatorDLObjectDetect * clsCheckOperatorDLObjectDetect):ClsCheckOperatorDLObjectDetect(clsCheckOperatorDLObjectDetect)
    {

    }
    virtual int LoadXml(XMLElement *pEleParent)
    {

        this->ClsCheckOperatorDLObjectDetect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string str = pEleParent->Name();
        int idex=pEleParent->IntAttribute("Index");
        int type = pEleParent->IntAttribute("Type");

        string temp = "";
        XmlHelper::GetElementTextFromParent("Confidence", pEleParent, temp);
        this->ClsCheckOperatorDLObjectDetect->m_iConfidence = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("MinArea", pEleParent, temp);
        this->ClsCheckOperatorDLObjectDetect->m_iMinArea = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("MinGrayVal", pEleParent, temp);
        this->ClsCheckOperatorDLObjectDetect->m_iMinGrayVal = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("GrayValUpLimt", pEleParent, temp);
        this->ClsCheckOperatorDLObjectDetect->m_iValUpLimt = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("AccuracyType", pEleParent, temp);
        this->ClsCheckOperatorDLObjectDetect->m_iaccuracyType = atoi(temp.c_str());


        this->ClsCheckOperatorDLObjectDetect->CheckType=static_cast<ENUMCHECKOPERATORTYPE>(type);
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDLObjectDetect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;

        return 0;
    }
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorDLObjectDetect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorDLObjectDetect->CheckType);

        XMLElement* eleConfidence = doc.NewElement("Confidence");
        eleConfidence->SetText(this->ClsCheckOperatorDLObjectDetect->m_iConfidence);

        XMLElement* eleMinArea = doc.NewElement("MinArea");
        eleMinArea->SetText(this->ClsCheckOperatorDLObjectDetect->m_iMinArea);

        XMLElement* eleMinGrayVal = doc.NewElement("MinGrayVal");
        eleMinGrayVal->SetText(this->ClsCheckOperatorDLObjectDetect->m_iMinGrayVal);

        XMLElement* eleMinPercent = doc.NewElement("GrayValUpLimt");
        eleMinPercent->SetText(this->ClsCheckOperatorDLObjectDetect->m_iValUpLimt);

        XMLElement* eleAccuracyTyoe = doc.NewElement("AccuracyType");
        eleAccuracyTyoe->SetText(this->ClsCheckOperatorDLObjectDetect->m_iaccuracyType);




        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDLObjectDetect->stu_CheckOperatorBasic);
        XMLElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;

        eleCheckNo->InsertEndChild(eleConfidence);
        eleCheckNo->InsertEndChild(eleMinArea);
        eleCheckNo->InsertEndChild(eleMinGrayVal);
        eleCheckNo->InsertEndChild(eleMinPercent);
        eleCheckNo->InsertEndChild(eleAccuracyTyoe);

        eleCheckNo->InsertEndChild(eleBasic);
        return eleCheckNo;
    }

    void getLargestConnecttedComponent(cv::Mat srcImage,cv:: Mat &dstImage)
    {
        cv::Mat temp;
        cv::Mat labels;
        srcImage.copyTo(temp);
        int n_comps = connectedComponents(temp, labels, 4, CV_16U);
        vector<int> histogram_of_labels;
        for (int i = 0; i < n_comps; i++)
        {
            histogram_of_labels.push_back(0);
        }

        int rows = labels.rows;
        int cols = labels.cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
            }
        }
        histogram_of_labels.at(0) = 0;


        int maximum = 0;
        int max_idx = 0;
        for (int i = 0; i < n_comps; i++)
        {
            if (histogram_of_labels.at(i) > maximum)
            {
                maximum = histogram_of_labels.at(i);
                max_idx = i;
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (labels.at<unsigned short>(row, col) == max_idx)
                {
                    labels.at<unsigned short>(row, col) = 255;
                }
                else
                {
                    labels.at<unsigned short>(row, col) = 0;
                }
            }
        }
        labels.convertTo(dstImage, CV_8U);
    }

    virtual int calculate(cv::Mat& ref, cv::Mat cur,BBoxInfo& bboxInfo)
    {

        int res=-1;
        {
            if((bboxInfo.prob>this->ClsCheckOperatorDLObjectDetect->m_iConfidence/100.0)&&bboxInfo.classId==0)
            {

                int ktArea=0;
                bool bAllWihte=false;
                cv::Mat ktImg=cv::Mat(ref,bboxInfo.rect);
                cv::cvtColor(ktImg,ktImg,cv::COLOR_RGB2GRAY);
                cv::Mat binImg,largeUnit;

                binImg=cv::Mat::zeros(ktImg.rows,ktImg.cols,CV_8UC1);
                for(int i=0;i<binImg.rows;i++)
                {
                    for(int j=0;j<binImg.cols;j++)
                    {
                        if(ktImg.at<uchar>(i,j)>=ClsCheckOperatorDLObjectDetect->m_iMinGrayVal&&ktImg.at<uchar>(i,j)<=ClsCheckOperatorDLObjectDetect->m_iValUpLimt)
                        {
                            binImg.at<uchar>(i,j)=255;
                            ktArea++;
                        }
                    }
                }
                if(ktArea==binImg.cols*binImg.rows)
                {
                    bAllWihte=true;
                }
                cv::Mat elementOpen=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7),cv::Point(-1,-1));
                cv::morphologyEx(binImg,binImg,cv::MORPH_OPEN,elementOpen);
                getLargestConnecttedComponent(binImg,largeUnit);

                ktArea=0;
                for(int i=0;i<largeUnit.rows;i++)
                {
                    for(int j=0;j<largeUnit.cols;j++)
                    {
                        if(largeUnit.at<uchar>(i,j)>0)
                        {
                            ktArea++;
                        }
                    }
                }
                if((ktArea==largeUnit.cols*largeUnit.rows)&&!bAllWihte)
                {
                    ktArea=0;
                }
                if((ktArea>=this->ClsCheckOperatorDLObjectDetect->m_iMinArea))
                {
                    res=0;
                    return  res;
                }

            }
            if((bboxInfo.prob>this->ClsCheckOperatorDLObjectDetect->m_iConfidence/100.0)&&bboxInfo.classId==1)
            {
                res=1;
                return res;
            }
        }
        return res;
    }

    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,cv::Mat& dst, cv::Rect rect, int confidence,int minArea,int minGrayVal,int GrayValUpLimit,int accuracyType,int * pDefectNum,int *pKtArea,int *pKtPercent)
    {
        //cv::imwrite(".test.bmp",cur);


        int defectNum=0;
        int okNum=0;

        int outminArea=999999999;
        int outminPercent=999999999;
        int ktAreaMax=0;

        cv::Size sz=cur.size();

        int ret=0;
        dst=cur.clone();
        int channel=dst.channels();

        cv::Mat tempImg;
        cv::Mat inputImg;

        int maxHW = ref.rows > ref.cols ? ref.rows : ref.cols;
        int toppad,bottompad, leftpad,rightpad;
        if (maxHW > 640)
        {
            toppad = (maxHW - ref.rows) / 2;
            bottompad = maxHW - ref.rows - toppad;

            leftpad = (maxHW - ref.cols) / 2;
            rightpad = maxHW - ref.cols - leftpad;
        }
        else
        {
            toppad = (640 - ref.rows) / 2;
            bottompad = 640 - ref.rows - toppad;

            leftpad = (640 - ref.cols) / 2;
            rightpad = 640 - ref.cols - leftpad;
        }

        cv::copyMakeBorder(ref, inputImg, toppad, bottompad, leftpad, rightpad, cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));
        cv::cvtColor(dst,tempImg,cv::COLOR_RGB2GRAY);



        //        int onnxIndex=0;


        //        //正常精度
        //        if(accuracyType==0)
        //            onnxIndex=0;
        //        //高精度
        //        else
        //            onnxIndex=1;
        //onnxIndex=0;


        int camIdx= this->ClsCheckOperatorDLObjectDetect->stu_CheckOperatorBasic->CamIdx;



        OnnxGloable::getInstance()->m_onnxMutex[camIdx-1].lock();
        OnnxGloable::getInstance()->onnxArray[camIdx-1].setConfidence(0.1);
        vector<vector<BBoxInfo>> vec_batch_result;
        OnnxGloable::getInstance()->onnxArray[camIdx-1].detect(inputImg,vec_batch_result);
        OnnxGloable::getInstance()->m_onnxMutex[camIdx-1].unlock();

        std::vector<BBoxInfo> vecBBoxInfo;

        int boxID=0;
        for(int i=0;i<vec_batch_result.size();i++)
        {
            for(int j=0;j<vec_batch_result[i].size();j++)
            {
                int x=(vec_batch_result[i][j].rect.x-leftpad)>0?(vec_batch_result[i][j].rect.x-leftpad):0;
                int y=(vec_batch_result[i][j].rect.y-toppad)>0?(vec_batch_result[i][j].rect.y-toppad):0;

                int  imgwid=cur.cols;
                int imghei=cur.rows;

                int width=(x+vec_batch_result[i][j].rect.width)<=cur.cols?(vec_batch_result[i][j].rect.width):(cur.cols-x);
                int height=(y+vec_batch_result[i][j].rect.height)<=cur.rows?(vec_batch_result[i][j].rect.height):(cur.rows-y);

                vec_batch_result[i][j].rect=cv::Rect( x,y,width,height);

                vec_batch_result[i][j].boxID=boxID;
                boxID++;
                vecBBoxInfo.push_back(vec_batch_result[i][j]);

            }
        }


        for(int i=0;i<vecBBoxInfo.size();i++)
        {

            if((vecBBoxInfo[i].prob>confidence/100.0)&&vecBBoxInfo[i].classId==0)
            {
                //
                //                    defectNum++;
                //                    cv::rectangle(dst,vecBBoxInfo[i].rect,cv::Scalar(255,0,0),1);
                //                    ret=1;

                //按面积筛查一次
                BBoxInfo r = vecBBoxInfo[i];
                cv::Rect temp=r.rect &rect;
                if(temp.area()>r.rect.area()/2)
                    //if(1)
                {
                    int ktArea=0;
                    bool bAllWihte=false;
                    cv::Mat binImg,largeUnit;
                    cv::Mat ktImg=cv::Mat(tempImg,r.rect);
                    binImg=cv::Mat::zeros(ktImg.rows,ktImg.cols,CV_8UC1);

                    for(int i=0;i<binImg.rows;i++)
                    {
                        for(int j=0;j<binImg.cols;j++)
                        {
                            if(ktImg.at<uchar>(i,j)>=minGrayVal&&ktImg.at<uchar>(i,j)<=GrayValUpLimit)
                            {
                                ktArea++;
                                binImg.at<uchar>(i,j)=255;
                            }
                        }
                    }
                    if(ktArea==binImg.cols*binImg.rows)
                    {
                        bAllWihte=true;
                    }
                    ktArea=0;
                    cv::Mat elementOpen=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7),cv::Point(-1,-1));
                    cv::morphologyEx(binImg,binImg,cv::MORPH_OPEN,elementOpen);
                    getLargestConnecttedComponent(binImg,largeUnit);

                    for(int i=0;i<largeUnit.rows;i++)
                    {
                        for(int j=0;j<largeUnit.cols;j++)
                        {
                            if(largeUnit.at<uchar>(i,j)>0)
                            {
                                ktArea++;
                            }
                        }
                    }
                    if((ktArea==largeUnit.cols*largeUnit.rows)&&!bAllWihte)
                    {
                        ktArea=0;
                    }

                    int ktPercent=(int)(100*ktArea/(double(ktImg.cols*ktImg.rows)));
                    if(ktArea>ktAreaMax)ktAreaMax=ktArea;
                    if(ktArea>=minArea)
                    {
                        if(ktArea<outminArea) outminArea=ktArea;
                        if(ktPercent<outminPercent) outminPercent=ktPercent;
                        cv::rectangle(dst,vecBBoxInfo[i].rect,cv::Scalar(255,0,0),1);
                        cv::putText(dst,std::to_string(int(vecBBoxInfo[i].prob*100)),cv::Point(vecBBoxInfo[i].rect.x,vecBBoxInfo[i].rect.y+10),cv::FONT_HERSHEY_COMPLEX,2,cv::Scalar(0,255,0));
                        defectNum++;
                        ret=1;
                    }
                    if(ktArea!=0)
                        for(int n=0;n<largeUnit.rows;n++)
                        {
                            for(int m=0;m<largeUnit.cols;m++)
                            {
                                if(largeUnit.at<uchar>(n,m)>0)
                                {
                                    dst.at<cv::Vec3b>(n+vecBBoxInfo[i].rect.y,m+vecBBoxInfo[i].rect.x)[0]=200;
                                    dst.at<cv::Vec3b>(n+vecBBoxInfo[i].rect.y,m+vecBBoxInfo[i].rect.x)[1]=200;
                                    dst.at<cv::Vec3b>(n+vecBBoxInfo[i].rect.y,m+vecBBoxInfo[i].rect.x)[2]=200;
                                }
                            }
                        }
                }
            }

            if((vecBBoxInfo[i].prob>confidence/100.0)&&vecBBoxInfo[i].classId==1)
            {
                  okNum++;
//                  cv::rectangle(dst,vecBBoxInfo[i].rect,cv::Scalar(0,255,0),1);
            }

        }


        if(ret==1)
        {
            outminArea=ktAreaMax;
            outminPercent=0;
            * pKtArea=outminArea;
            *pKtPercent=outminPercent;
            *pDefectNum=defectNum;
            return ret;

        }
        if(camIdx==1&&okNum<20)
        {
            ret=1;
            *pDefectNum=20-okNum;
            * pKtArea=0;
            *pKtPercent=0;
            return ret;
        }

        return  ret;


    }

public:
    CheckOperatorDLObjectDetect *ClsCheckOperatorDLObjectDetect;
};


//缺陷检查算子
class CheckOperatorDefectInspectControl:public CheckOperatorControlBase
{
public:
    CheckOperatorDefectInspectControl(CheckOperatorDefectInspect* clsCheckOperatorDefectInspect) :ClsCheckOperatorDefectInspect(clsCheckOperatorDefectInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string str = pEleParent->Name();
        int idex=pEleParent->IntAttribute("Index");
        int type = pEleParent->IntAttribute("Type");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("MaxArea", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect->MaxArea = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("BinThresh", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect->BinThresh = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("IsBlackBlob", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect->IsBlackBlob = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("IsDefectAll", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect->IsDefectAll = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("MinArea", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect->MinArea = atoi(temp.c_str());
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorDefectInspect->CheckType);
        
        XMLElement* eleMaxArea = doc.NewElement("MaxArea");
        eleMaxArea->SetText(this->ClsCheckOperatorDefectInspect->MaxArea);
        XMLElement* eleBinThresh = doc.NewElement("BinThresh");
        eleBinThresh->SetText(this->ClsCheckOperatorDefectInspect->BinThresh);
        XMLElement* eleIsBlackBlob = doc.NewElement("IsBlackBlob");
        eleIsBlackBlob->SetText(this->ClsCheckOperatorDefectInspect->IsBlackBlob?1:0);

        XMLElement* eleIsDefectAll = doc.NewElement("IsDefectAll");
        eleIsDefectAll->SetText(this->ClsCheckOperatorDefectInspect->IsDefectAll?1:0);

        XMLElement* eleMinArea = doc.NewElement("MinArea");
        eleMinArea->SetText(this->ClsCheckOperatorDefectInspect->MinArea);
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        
        eleCheckNo->InsertEndChild(eleMaxArea);
        eleCheckNo->InsertEndChild(eleBinThresh);
        eleCheckNo->InsertEndChild(eleIsBlackBlob);
        eleCheckNo->InsertEndChild(eleIsDefectAll);
        eleCheckNo->InsertEndChild(eleMinArea);
        eleCheckNo->InsertEndChild(eleBasic);
        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");

        QDomElement* eleCheckNo =new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorDefectInspect->CheckType);

        QDomElement eleMaxArea=doc.createElement("MaxArea");
        QDomText texteleMaxArea=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect->MaxArea));
        eleMaxArea.appendChild(texteleMaxArea);

        QDomElement eleBinThresh=doc.createElement("BinThresh");
        QDomText texteleBinThresh=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect->BinThresh));
        eleBinThresh.appendChild(texteleBinThresh);


        QDomElement eleIsBlackBlob=doc.createElement("IsBlackBlob");
        QDomText texteleIsBlackBlob=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect->IsBlackBlob?1:0));
        eleIsBlackBlob.appendChild(texteleIsBlackBlob);


        QDomElement eleMinArea=doc.createElement("MinArea");
        QDomText texteleMinArea=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect->MinArea));
        eleMinArea.appendChild(texteleIsBlackBlob);





        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic);
        QDomElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;


        eleCheckNo->appendChild(eleMaxArea);
        eleCheckNo->appendChild(eleBinThresh);
        eleCheckNo->appendChild(eleIsBlackBlob);
        eleCheckNo->appendChild(eleMinArea);
        eleCheckNo->appendChild(*eleBasic);
        return eleCheckNo;
    }
    
    
    void getLargestConnecttedComponent(cv::Mat srcImage,cv:: Mat &dstImage)
    {
        cv::Mat temp;
        cv::Mat labels;
        srcImage.copyTo(temp);


        int n_comps = connectedComponents(temp, labels, 4, CV_16U);
        vector<int> histogram_of_labels;
        for (int i = 0; i < n_comps; i++)
        {
            histogram_of_labels.push_back(0);
        }

        int rows = labels.rows;
        int cols = labels.cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
            }
        }
        histogram_of_labels.at(0) = 0;


        int maximum = 0;
        int max_idx = 0;
        for (int i = 0; i < n_comps; i++)
        {
            if (histogram_of_labels.at(i) > maximum)
            {
                maximum = histogram_of_labels.at(i);
                max_idx = i;
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (labels.at<unsigned short>(row, col) == max_idx)
                {
                    labels.at<unsigned short>(row, col) = 255;
                }
                else
                {
                    labels.at<unsigned short>(row, col) = 0;
                }
            }
        }
        labels.convertTo(dstImage, CV_8U);
    }
    
    //************************************
    // Method:    calculate
    // FullName:  CheckOperatorDefectInspectControl::calculate
    // Access:    virtual public
    // Returns:   -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref
    // Parameter: cv::Mat cur
    //************************************
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {
        
        if(!this->ClsCheckOperatorDefectInspect->stu_CheckOperatorBasic->Basic_IsCheck) return  0;
        //cv::Mat dst = cur.clone();
        int threshold = this->ClsCheckOperatorDefectInspect->BinThresh;
        bool isBlack=this->ClsCheckOperatorDefectInspect->IsBlackBlob;
        bool isDefectAll=this->ClsCheckOperatorDefectInspect->IsDefectAll;
        int maxArea=this->ClsCheckOperatorDefectInspect->MaxArea;


        cv::Mat temp;
        int areaWhite=0;
        if(!isBlack)
        {
            cv::threshold(cur,temp,threshold,255,cv::THRESH_BINARY);
        }
        else
        {
            cv::threshold(cur,temp,threshold,255,cv::THRESH_BINARY_INV);
        }

        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<temp.rows;r++)
            {
                for (int c=0;c<temp.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }


        //        std::vector<int> test;
        //        int a=test[10];
        //        qDebug()<<a<<endl;




        areaWhite=0;
        for(int r=0;r<temp.rows;r++)
        {
            for (int c=0;c<temp.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                }
            }
        }
        return areaWhite > maxArea?1:0;
    }

    //************************************
    // Method:    debugCalculator
    // FullName:  CheckOperatorDefectInspectControl::debugCalculator
    // Access:    virtual public
    // Returns:    -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref 参考图
    // Parameter: cv::Mat cur 当前图
    // Parameter: cv::Mat & dst 算子调试需要显示的图像
    // Parameter: cv::Rect rect： roi
    // Parameter: int threshold：阈值
    // Parameter: bool isBlack：是否黑色
    // Parameter: int minArea：比较最小面积
    // Parameter: int * pArea：实测面积,像素点数量
    //************************************
    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,cv::Mat& dst, cv::Rect rect, int threshold, bool isBlack,bool isDefectAll,int maxArea,int* pArea)
    {
        cv::Mat src = cv::Mat(cur, rect);
        dst = src.clone();
        cv::Mat temp;
        int areaWhite=0;

        if(!isBlack)
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);
        }
        else
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY_INV);
        }


        //只选择单个缺陷
        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<src.rows;r++)
            {
                for (int c=0;c<src.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }



        //最终统计缺陷面积
        areaWhite=0;
        for(int r=0;r<src.rows;r++)
        {
            for (int c=0;c<src.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                    dst.at<uchar>(r,c)=200;
                }
            }
        }
        *pArea = areaWhite;
        return areaWhite > maxArea?1:0;


        //        int area = rect.width * rect.height;
        //        int areaBlack = area - areaWhite;
        //        //大于设定面积报错,返回1
        //        if (isBlack)
        //        {
        //            *pArea = areaBlack;
        //            for(int r=0;r<src.rows;r++)
        //            {
        //                for (int c=0;c<src.cols;c++)
        //                {
        //                    if(temp.at<uchar>(r,c)==0)
        //                    {
        //                       // areaWhite++;
        //                        dst.at<uchar>(r,c)=200;
        //                    }
        //                }
        //            }
        //            return areaBlack > maxArea?1:0;
        //        }
        //        else
        //        {
        //            *pArea = areaWhite;

        //            for(int r=0;r<src.rows;r++)
        //            {
        //                for (int c=0;c<src.cols;c++)
        //                {
        //                    if(temp.at<uchar>(r,c)==255)
        //                    {
        //                       // areaWhite++;
        //                        dst.at<uchar>(r,c)=200;
        //                    }
        //                }
        //            }
        //            return areaWhite > maxArea?1:0;
        //        }
    }

public:
    CheckOperatorDefectInspect * ClsCheckOperatorDefectInspect;
};




//缺陷检测算子2
class CheckOperatorDefectInspect2Control:public CheckOperatorControlBase
{
public:
    CheckOperatorDefectInspect2Control(CheckOperatorDefectInspect2* clsCheckOperatorDefectInspect2) :ClsCheckOperatorDefectInspect2(clsCheckOperatorDefectInspect2) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string str = pEleParent->Name();
        int idex=pEleParent->IntAttribute("Index");
        int type = pEleParent->IntAttribute("Type");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("MaxArea", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->MaxArea = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("BinThresh", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->BinThresh = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("IsBlackBlob", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->IsBlackBlob = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("MinArea", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->MinArea = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("IsDefectAll", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->IsDefectAll = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("X1", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X1 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y1", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y1 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X2", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X2 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y2", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y2 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X3", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X3 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y3", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y3 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X4", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X4 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y4", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y4 = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("X5", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X5 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y5", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y5 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X6", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X6 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y6", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y6 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X7", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X7 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y7", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y7 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X8", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->X8 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y8", pEleParent, temp);
        this->ClsCheckOperatorDefectInspect2->Y8 = atoi(temp.c_str());
        
        
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorDefectInspect2->CheckType);
        
        XMLElement* eleMaxArea = doc.NewElement("MaxArea");
        eleMaxArea->SetText(this->ClsCheckOperatorDefectInspect2->MaxArea);
        XMLElement* eleBinThresh = doc.NewElement("BinThresh");
        eleBinThresh->SetText(this->ClsCheckOperatorDefectInspect2->BinThresh);
        XMLElement* eleIsBlackBlob = doc.NewElement("IsBlackBlob");
        eleIsBlackBlob->SetText(this->ClsCheckOperatorDefectInspect2->IsBlackBlob?1:0);
        XMLElement* eleMinArea = doc.NewElement("MinArea");
        eleMinArea->SetText(this->ClsCheckOperatorDefectInspect2->MinArea);

        XMLElement* eleIsDefectAll = doc.NewElement("IsDefectAll");
        eleIsDefectAll->SetText(this->ClsCheckOperatorDefectInspect2->IsDefectAll?1:0);

        
        
        XMLElement* eleX1 = doc.NewElement("X1");
        eleX1->SetText(this->ClsCheckOperatorDefectInspect2->X1);
        XMLElement* eleX2 = doc.NewElement("X2");
        eleX2->SetText(this->ClsCheckOperatorDefectInspect2->X2);
        XMLElement* eleX3 = doc.NewElement("X3");
        eleX3->SetText(this->ClsCheckOperatorDefectInspect2->X3);
        XMLElement* eleX4 = doc.NewElement("X4");
        eleX4->SetText(this->ClsCheckOperatorDefectInspect2->X4);
        XMLElement* eleX5 = doc.NewElement("X5");
        eleX5->SetText(this->ClsCheckOperatorDefectInspect2->X5);
        XMLElement* eleX6 = doc.NewElement("X6");
        eleX6->SetText(this->ClsCheckOperatorDefectInspect2->X6);
        XMLElement* eleX7 = doc.NewElement("X7");
        eleX7->SetText(this->ClsCheckOperatorDefectInspect2->X7);
        XMLElement* eleX8 = doc.NewElement("X8");
        eleX8->SetText(this->ClsCheckOperatorDefectInspect2->X8);
        
        XMLElement* eleY1 = doc.NewElement("Y1");
        eleY1->SetText(this->ClsCheckOperatorDefectInspect2->Y1);
        XMLElement* eleY2 = doc.NewElement("Y2");
        eleY2->SetText(this->ClsCheckOperatorDefectInspect2->Y2);
        XMLElement* eleY3 = doc.NewElement("Y3");
        eleY3->SetText(this->ClsCheckOperatorDefectInspect2->Y3);
        XMLElement* eleY4 = doc.NewElement("Y4");
        eleY4->SetText(this->ClsCheckOperatorDefectInspect2->Y4);
        XMLElement* eleY5 = doc.NewElement("Y5");
        eleY5->SetText(this->ClsCheckOperatorDefectInspect2->Y5);
        XMLElement* eleY6= doc.NewElement("Y6");
        eleY6->SetText(this->ClsCheckOperatorDefectInspect2->Y6);
        XMLElement* eleY7 = doc.NewElement("Y7");
        eleY7->SetText(this->ClsCheckOperatorDefectInspect2->Y7);
        XMLElement* eleY8 = doc.NewElement("Y8");
        eleY8->SetText(this->ClsCheckOperatorDefectInspect2->Y8);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic);
        XMLElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        
        eleCheckNo->InsertEndChild(eleMaxArea);
        eleCheckNo->InsertEndChild(eleBinThresh);
        eleCheckNo->InsertEndChild(eleIsBlackBlob);
        eleCheckNo->InsertEndChild(eleIsDefectAll);
        eleCheckNo->InsertEndChild(eleMinArea);
        eleCheckNo->InsertEndChild(eleBasic);

        
        
        eleCheckNo->InsertEndChild(eleX1);
        eleCheckNo->InsertEndChild(eleX2);
        eleCheckNo->InsertEndChild(eleX3);
        eleCheckNo->InsertEndChild(eleX4);
        eleCheckNo->InsertEndChild(eleX5);
        eleCheckNo->InsertEndChild(eleX6);
        eleCheckNo->InsertEndChild(eleX7);
        eleCheckNo->InsertEndChild(eleX8);
        eleCheckNo->InsertEndChild(eleY1);
        eleCheckNo->InsertEndChild(eleY2);
        eleCheckNo->InsertEndChild(eleY3);
        eleCheckNo->InsertEndChild(eleY4);
        eleCheckNo->InsertEndChild(eleY5);
        eleCheckNo->InsertEndChild(eleY6);
        eleCheckNo->InsertEndChild(eleY7);
        eleCheckNo->InsertEndChild(eleY8);
        
        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");

        QDomElement* eleCheckNo = new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorDefectInspect2->CheckType);


        QDomElement eleMaxArea=doc.createElement("MaxArea");
        QDomText texteleMaxArea=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->MaxArea));
        eleMaxArea.appendChild(texteleMaxArea);

        QDomElement eleBinThresh=doc.createElement("BinThresh");
        QDomText texteleBinThresh=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->BinThresh));
        eleBinThresh.appendChild(texteleBinThresh);

        QDomElement eleIsBlackBlob=doc.createElement("IsBlackBlob");
        QDomText texteleIsBlackBlob=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->IsBlackBlob));
        eleIsBlackBlob.appendChild(texteleIsBlackBlob);

        QDomElement eleMinArea=doc.createElement("MinArea");
        QDomText texteleMinArea=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->MinArea));
        eleMinArea.appendChild(texteleMinArea);

        QDomElement eleX1=doc.createElement("X1");
        QDomText texteleX1=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X1));
        eleX1.appendChild(texteleX1);

        QDomElement eleX2=doc.createElement("X2");
        QDomText texteleX2=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X2));
        eleX2.appendChild(texteleX2);

        QDomElement eleX3=doc.createElement("X3");
        QDomText texteleX3=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X3));
        eleX3.appendChild(texteleX3);


        QDomElement eleX4=doc.createElement("X4");
        QDomText texteleX4=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X4));
        eleX4.appendChild(texteleX4);

        QDomElement eleX5=doc.createElement("X5");
        QDomText texteleX5=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X5));
        eleX5.appendChild(texteleX5);

        QDomElement eleX6=doc.createElement("X6");
        QDomText texteleX6=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X6));
        eleX6.appendChild(texteleX6);

        QDomElement eleX7=doc.createElement("X7");
        QDomText texteleX7=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X7));
        eleX7.appendChild(texteleX7);

        QDomElement eleX8=doc.createElement("X8");
        QDomText texteleX8=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->X8));
        eleX8.appendChild(texteleX8);


        QDomElement eleY1=doc.createElement("Y1");
        QDomText texteleY1=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y1));
        eleY1.appendChild(texteleY1);

        QDomElement eleY2=doc.createElement("Y2");
        QDomText texteleY2=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y2));
        eleY2.appendChild(texteleY2);

        QDomElement eleY3=doc.createElement("Y3");
        QDomText texteleY3=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y3));
        eleY3.appendChild(texteleY3);


        QDomElement eleY4=doc.createElement("Y4");
        QDomText texteleY4=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y4));
        eleY4.appendChild(texteleY4);

        QDomElement eleY5=doc.createElement("Y5");
        QDomText texteleY5=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y5));
        eleY5.appendChild(texteleY5);

        QDomElement eleY6=doc.createElement("Y6");
        QDomText texteleY6=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y6));
        eleY6.appendChild(texteleY6);

        QDomElement eleY7=doc.createElement("Y7");
        QDomText texteleY7=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y7));
        eleY7.appendChild(texteleY7);

        QDomElement eleY8=doc.createElement("Y8");
        QDomText texteleY8=doc.createTextNode(QString::number(this->ClsCheckOperatorDefectInspect2->Y8));
        eleY8.appendChild(texteleY8);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic);
        QDomElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;


        eleCheckNo->appendChild(eleMaxArea);
        eleCheckNo->appendChild(eleBinThresh);
        eleCheckNo->appendChild(eleIsBlackBlob);
        eleCheckNo->appendChild(eleMinArea);
        eleCheckNo->appendChild(*eleBasic);


        eleCheckNo->appendChild(eleX1);
        eleCheckNo->appendChild(eleX2);
        eleCheckNo->appendChild(eleX3);
        eleCheckNo->appendChild(eleX4);
        eleCheckNo->appendChild(eleX5);
        eleCheckNo->appendChild(eleX6);
        eleCheckNo->appendChild(eleX7);
        eleCheckNo->appendChild(eleX8);
        eleCheckNo->appendChild(eleY1);
        eleCheckNo->appendChild(eleY2);
        eleCheckNo->appendChild(eleY3);
        eleCheckNo->appendChild(eleY4);
        eleCheckNo->appendChild(eleY5);
        eleCheckNo->appendChild(eleY6);
        eleCheckNo->appendChild(eleY7);
        eleCheckNo->appendChild(eleY8);

        return eleCheckNo;
    }

    void getLargestConnecttedComponent(cv::Mat srcImage,cv:: Mat &dstImage)
    {
        cv::Mat temp;
        cv::Mat labels;
        srcImage.copyTo(temp);


        int n_comps = connectedComponents(temp, labels, 4, CV_16U);
        vector<int> histogram_of_labels;
        for (int i = 0; i < n_comps; i++)
        {
            histogram_of_labels.push_back(0);
        }

        int rows = labels.rows;
        int cols = labels.cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
            }
        }
        histogram_of_labels.at(0) = 0;


        int maximum = 0;
        int max_idx = 0;
        for (int i = 0; i < n_comps; i++)
        {
            if (histogram_of_labels.at(i) > maximum)
            {
                maximum = histogram_of_labels.at(i);
                max_idx = i;
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (labels.at<unsigned short>(row, col) == max_idx)
                {
                    labels.at<unsigned short>(row, col) = 255;
                }
                else
                {
                    labels.at<unsigned short>(row, col) = 0;
                }
            }
        }
        labels.convertTo(dstImage, CV_8U);
    }
    
    
    
    //************************************
    // Method:    calculate
    // FullName:  CheckOperatorDefectInspectControl::calculate
    // Access:    virtual public
    // Returns:   -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref
    // Parameter: cv::Mat cur
    //************************************
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        if(!this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic->Basic_IsCheck) return  0;
        cv::Mat dst = cur.clone();
        int threshold = this->ClsCheckOperatorDefectInspect2->BinThresh;

        vector<cv::Point> vPts;
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X1,this->ClsCheckOperatorDefectInspect2->Y1));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X2,this->ClsCheckOperatorDefectInspect2->Y2));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X3,this->ClsCheckOperatorDefectInspect2->Y3));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X4,this->ClsCheckOperatorDefectInspect2->Y4));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X5,this->ClsCheckOperatorDefectInspect2->Y5));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X6,this->ClsCheckOperatorDefectInspect2->Y6));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X7,this->ClsCheckOperatorDefectInspect2->Y7));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X8,this->ClsCheckOperatorDefectInspect2->Y8));
        cv::Mat mask;
        DSIppImage::getMaskImage(mask,dst.rows,dst.cols,vPts,255);

        double area =0;
        //        int sumArea=0;
        //        DSIppImage::ippiSum_8u_C1R_CV(mask, &area);
        //        sumArea=area/255;




        cv::Mat temp;

        if(this->ClsCheckOperatorDefectInspect2->IsBlackBlob)
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY_INV);
        }
        else
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);

        }



        bool isDefectAll=true;
        int areaWhite=0;
        //只选择单个缺陷
        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<dst.rows;r++)
            {
                for (int c=0;c<dst.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }



        //最终统计缺陷面积
        areaWhite=0;
        for(int r=0;r<dst.rows;r++)
        {
            for (int c=0;c<dst.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255&&mask.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                    //dst.at<uchar>(r,c)=200;
                }
            }
        }


        //面积大于设定值为缺陷
        //      return areaWhite > this->ClsCheckOperatorDefectInspect2->MaxArea?1:0;


        //面积小于设定值为缺陷
        return   areaWhite > this->ClsCheckOperatorDefectInspect2->MaxArea?0:1;




        //        cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);
        //        int areaWhite=0;
        //        for(int r=0;r<dst.rows;r++)
        //        {
        //            for (int c=0;c<dst.cols;c++)
        //            {
        //                if(temp.at<uchar>(r,c)==255&&mask.at<uchar>(r,c)==255)
        //                {
        //                    areaWhite++;
        //                }
        //            }
        //        }

        //        //int area = rect.width * rect.height;
        //        int areaBlack = sumArea - areaWhite;
        //        //大于设定面积报错,返回1
        //        if (this->ClsCheckOperatorDefectInspect2->IsBlackBlob)
        //        {
        //            //DSDEBUG<<"---------------------area:"<<areaBlack<<endl;
        //            return areaBlack >  this->ClsCheckOperatorDefectInspect2->MaxArea?1:0;
        //        }
        //        else
        //        { //DSDEBUG<<"---------------------area:"<<areaWhite<<endl;
        //            return areaWhite >  this->ClsCheckOperatorDefectInspect2->MaxArea?1:0;
        //        }



        //        if(!this->ClsCheckOperatorDefectInspect2->stu_CheckOperatorBasic->Basic_IsCheck) return  0;
        //        cv::Mat dst = cur.clone();
        //        int threshold = this->ClsCheckOperatorDefectInspect2->BinThresh;
        
        //        if (DSIppImage::ippiCompareC_8u_C1R_CV(cur, threshold, dst)!=0)
        //        {
        //            //ipp执行失败
        //            return -1;
        //        }
        
        //        vector<cv::Point> vPts;
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X1,this->ClsCheckOperatorDefectInspect2->Y1));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X2,this->ClsCheckOperatorDefectInspect2->Y2));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X3,this->ClsCheckOperatorDefectInspect2->Y3));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X4,this->ClsCheckOperatorDefectInspect2->Y4));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X5,this->ClsCheckOperatorDefectInspect2->Y5));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X6,this->ClsCheckOperatorDefectInspect2->Y6));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X7,this->ClsCheckOperatorDefectInspect2->Y7));
        //        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X8,this->ClsCheckOperatorDefectInspect2->Y8));
        //        cv::Mat mask;
        //        DSIppImage::getMaskImage(mask,dst.rows,dst.cols,vPts,255);
        //        for(int r=0;r<mask.rows;++r)
        //            for(int c=0;c<mask.cols;++c)
        //                dst.at<unsigned char>(r,c)=dst.at<unsigned char>(r,c)&mask.at<unsigned char>(r,c);
        
        //        int area =0;
        
        //        double sumArea=0;
        //        DSIppImage::ippiSum_8u_C1R_CV(mask, &sumArea);
        //        area=sumArea/255;
        
        //        double pixelSum = 0;
        //        if (DSIppImage::ippiSum_8u_C1R_CV(dst,&pixelSum)!=0)
        //        {
        //            DSDEBUG<<"缺陷检查错误!";
        //            return -1;
        //        }
        
        //        int areaWhite = pixelSum / 255;
        //        int areaBlack = area - areaWhite;
        //        //        //大于设定面积报错,返回1
        //        //        if ((this->ClsCheckOperatorDefectInspect->IsBlackBlob ? areaBlack : areaWhite) < this->ClsCheckOperatorDefectInspect->MinArea)
        //        //        {
        //        //            DSDEBUG<<"缺陷检查正确!";
        //        //            return 0;
        //        //        }
        //        //        return 1;
        //        //大于设定面积报错,返回1
        //        if (this->ClsCheckOperatorDefectInspect2->IsBlackBlob)
        //        {
        //            //DSDEBUG<<"---------------------area:"<<areaBlack<<endl;
        //            return areaBlack >  this->ClsCheckOperatorDefectInspect2->MaxArea?1:0;

        //        }
        //        else
        //        { //DSDEBUG<<"---------------------area:"<<areaWhite<<endl;
        //            return areaWhite >  this->ClsCheckOperatorDefectInspect2->MaxArea?1:0;
        //        }


    }
    
    
    
    //************************************
    // Method:    debugCalculator
    // FullName:  CheckOperatorDefectInspectControl::debugCalculator
    // Access:    virtual public
    // Returns:    -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref 参考图
    // Parameter: cv::Mat cur 当前图
    // Parameter: cv::Mat & dst 算子调试需要显示的图像
    // Parameter: cv::Rect rect： roi
    // Parameter: int threshold：阈值
    // Parameter: bool isBlack：是否黑色
    // Parameter: int minArea：比较最小面积
    // Parameter: int * pArea：实测面积,像素点数量
    //************************************
    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,cv::Mat& dst, cv::Rect rect, int threshold, bool isBlack,bool isDefectAll,int maxArea,int* pArea,int * pSumArea)
    {

        cv::Mat src = cv::Mat(cur, rect);
        dst = src.clone();
        
        vector<cv::Point> vPts;
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X1,this->ClsCheckOperatorDefectInspect2->Y1));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X2,this->ClsCheckOperatorDefectInspect2->Y2));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X3,this->ClsCheckOperatorDefectInspect2->Y3));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X4,this->ClsCheckOperatorDefectInspect2->Y4));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X5,this->ClsCheckOperatorDefectInspect2->Y5));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X6,this->ClsCheckOperatorDefectInspect2->Y6));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X7,this->ClsCheckOperatorDefectInspect2->Y7));
        vPts.push_back(cv::Point(this->ClsCheckOperatorDefectInspect2->X8,this->ClsCheckOperatorDefectInspect2->Y8));
        cv::Mat mask;
        DSIppImage::getMaskImage(mask,dst.rows,dst.cols,vPts,255);


        //掩膜的面积
        double sumArea=0;
        DSIppImage::ippiSum_8u_C1R_CV(mask, &sumArea);
        *pSumArea=sumArea/255;

        //cv::imwrite("./mask.bmp",mask );

        cv::Mat temp;
        //        cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);


        if(isBlack)
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY_INV);
        }
        else
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);

        }



        // bool isDefectAll=true;
        int areaWhite=0;
        //只选择单个缺陷
        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<src.rows;r++)
            {
                for (int c=0;c<src.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }



        //最终统计缺陷面积
        areaWhite=0;
        for(int r=0;r<src.rows;r++)
        {
            for (int c=0;c<src.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255&&mask.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                    dst.at<uchar>(r,c)=200;
                }
            }
        }
        *pArea = areaWhite;

        //面积大于设定值为缺陷
        return areaWhite > maxArea?1:0;
        //面积小于设定值为缺陷
        //        return areaWhite > maxArea?0:1;


    }
    
public:
    CheckOperatorDefectInspect2 * ClsCheckOperatorDefectInspect2;
};


//关联检测算子2
class CheckOperatorAssociatedInspect2Control:public CheckOperatorControlBase
{
public:
    CheckOperatorAssociatedInspect2Control(CheckOperatorAssociatedInspect2* clsCheckOperatorAssociatedInspect2) :ClsCheckOperatorAssociatedInspect2(clsCheckOperatorAssociatedInspect2)
    {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string str = pEleParent->Name();
        int idex=pEleParent->IntAttribute("Index");
        int type = pEleParent->IntAttribute("Type");

        string temp = "";
        XmlHelper::GetElementTextFromParent("MaxArea", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->MaxArea = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("BinThresh", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->BinThresh = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("IsBlackBlob", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->IsBlackBlob = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("MinArea", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->MinArea = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("IsDefectAll", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->IsDefectAll = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("X1", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X1 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y1", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y1 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X2", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X2 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y2", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y2 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X3", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X3 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y3", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y3 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X4", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X4 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y4", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y4 = atoi(temp.c_str());

        XmlHelper::GetElementTextFromParent("X5", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X5 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y5", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y5 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X6", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X6 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y6", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y6 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X7", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X7 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y7", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y7 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("X8", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->X8 = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Y8", pEleParent, temp);
        this->ClsCheckOperatorAssociatedInspect2->Y8 = atoi(temp.c_str());



        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;

        return 0;
    }

    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {

        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorAssociatedInspect2->CheckType);

        XMLElement* eleMaxArea = doc.NewElement("MaxArea");
        eleMaxArea->SetText(this->ClsCheckOperatorAssociatedInspect2->MaxArea);
        XMLElement* eleBinThresh = doc.NewElement("BinThresh");
        eleBinThresh->SetText(this->ClsCheckOperatorAssociatedInspect2->BinThresh);
        XMLElement* eleIsBlackBlob = doc.NewElement("IsBlackBlob");
        eleIsBlackBlob->SetText(this->ClsCheckOperatorAssociatedInspect2->IsBlackBlob?1:0);
        XMLElement* eleMinArea = doc.NewElement("MinArea");
        eleMinArea->SetText(this->ClsCheckOperatorAssociatedInspect2->MinArea);

        XMLElement* eleIsDefectAll = doc.NewElement("IsDefectAll");
        eleIsDefectAll->SetText(this->ClsCheckOperatorAssociatedInspect2->IsDefectAll?1:0);



        XMLElement* eleX1 = doc.NewElement("X1");
        eleX1->SetText(this->ClsCheckOperatorAssociatedInspect2->X1);
        XMLElement* eleX2 = doc.NewElement("X2");
        eleX2->SetText(this->ClsCheckOperatorAssociatedInspect2->X2);
        XMLElement* eleX3 = doc.NewElement("X3");
        eleX3->SetText(this->ClsCheckOperatorAssociatedInspect2->X3);
        XMLElement* eleX4 = doc.NewElement("X4");
        eleX4->SetText(this->ClsCheckOperatorAssociatedInspect2->X4);
        XMLElement* eleX5 = doc.NewElement("X5");
        eleX5->SetText(this->ClsCheckOperatorAssociatedInspect2->X5);
        XMLElement* eleX6 = doc.NewElement("X6");
        eleX6->SetText(this->ClsCheckOperatorAssociatedInspect2->X6);
        XMLElement* eleX7 = doc.NewElement("X7");
        eleX7->SetText(this->ClsCheckOperatorAssociatedInspect2->X7);
        XMLElement* eleX8 = doc.NewElement("X8");
        eleX8->SetText(this->ClsCheckOperatorAssociatedInspect2->X8);

        XMLElement* eleY1 = doc.NewElement("Y1");
        eleY1->SetText(this->ClsCheckOperatorAssociatedInspect2->Y1);
        XMLElement* eleY2 = doc.NewElement("Y2");
        eleY2->SetText(this->ClsCheckOperatorAssociatedInspect2->Y2);
        XMLElement* eleY3 = doc.NewElement("Y3");
        eleY3->SetText(this->ClsCheckOperatorAssociatedInspect2->Y3);
        XMLElement* eleY4 = doc.NewElement("Y4");
        eleY4->SetText(this->ClsCheckOperatorAssociatedInspect2->Y4);
        XMLElement* eleY5 = doc.NewElement("Y5");
        eleY5->SetText(this->ClsCheckOperatorAssociatedInspect2->Y5);
        XMLElement* eleY6= doc.NewElement("Y6");
        eleY6->SetText(this->ClsCheckOperatorAssociatedInspect2->Y6);
        XMLElement* eleY7 = doc.NewElement("Y7");
        eleY7->SetText(this->ClsCheckOperatorAssociatedInspect2->Y7);
        XMLElement* eleY8 = doc.NewElement("Y8");
        eleY8->SetText(this->ClsCheckOperatorAssociatedInspect2->Y8);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic);
        XMLElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;


        eleCheckNo->InsertEndChild(eleMaxArea);
        eleCheckNo->InsertEndChild(eleBinThresh);
        eleCheckNo->InsertEndChild(eleIsBlackBlob);
        eleCheckNo->InsertEndChild(eleIsDefectAll);
        eleCheckNo->InsertEndChild(eleMinArea);
        eleCheckNo->InsertEndChild(eleBasic);



        eleCheckNo->InsertEndChild(eleX1);
        eleCheckNo->InsertEndChild(eleX2);
        eleCheckNo->InsertEndChild(eleX3);
        eleCheckNo->InsertEndChild(eleX4);
        eleCheckNo->InsertEndChild(eleX5);
        eleCheckNo->InsertEndChild(eleX6);
        eleCheckNo->InsertEndChild(eleX7);
        eleCheckNo->InsertEndChild(eleX8);
        eleCheckNo->InsertEndChild(eleY1);
        eleCheckNo->InsertEndChild(eleY2);
        eleCheckNo->InsertEndChild(eleY3);
        eleCheckNo->InsertEndChild(eleY4);
        eleCheckNo->InsertEndChild(eleY5);
        eleCheckNo->InsertEndChild(eleY6);
        eleCheckNo->InsertEndChild(eleY7);
        eleCheckNo->InsertEndChild(eleY8);

        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");

        QDomElement* eleCheckNo = new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorAssociatedInspect2->CheckType);


        QDomElement eleMaxArea=doc.createElement("MaxArea");
        QDomText texteleMaxArea=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->MaxArea));
        eleMaxArea.appendChild(texteleMaxArea);

        QDomElement eleBinThresh=doc.createElement("BinThresh");
        QDomText texteleBinThresh=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->BinThresh));
        eleBinThresh.appendChild(texteleBinThresh);

        QDomElement eleIsBlackBlob=doc.createElement("IsBlackBlob");
        QDomText texteleIsBlackBlob=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->IsBlackBlob));
        eleIsBlackBlob.appendChild(texteleIsBlackBlob);

        QDomElement eleMinArea=doc.createElement("MinArea");
        QDomText texteleMinArea=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->MinArea));
        eleMinArea.appendChild(texteleMinArea);

        QDomElement eleX1=doc.createElement("X1");
        QDomText texteleX1=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X1));
        eleX1.appendChild(texteleX1);

        QDomElement eleX2=doc.createElement("X2");
        QDomText texteleX2=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X2));
        eleX2.appendChild(texteleX2);

        QDomElement eleX3=doc.createElement("X3");
        QDomText texteleX3=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X3));
        eleX3.appendChild(texteleX3);


        QDomElement eleX4=doc.createElement("X4");
        QDomText texteleX4=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X4));
        eleX4.appendChild(texteleX4);

        QDomElement eleX5=doc.createElement("X5");
        QDomText texteleX5=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X5));
        eleX5.appendChild(texteleX5);

        QDomElement eleX6=doc.createElement("X6");
        QDomText texteleX6=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X6));
        eleX6.appendChild(texteleX6);

        QDomElement eleX7=doc.createElement("X7");
        QDomText texteleX7=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X7));
        eleX7.appendChild(texteleX7);

        QDomElement eleX8=doc.createElement("X8");
        QDomText texteleX8=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->X8));
        eleX8.appendChild(texteleX8);


        QDomElement eleY1=doc.createElement("Y1");
        QDomText texteleY1=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y1));
        eleY1.appendChild(texteleY1);

        QDomElement eleY2=doc.createElement("Y2");
        QDomText texteleY2=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y2));
        eleY2.appendChild(texteleY2);

        QDomElement eleY3=doc.createElement("Y3");
        QDomText texteleY3=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y3));
        eleY3.appendChild(texteleY3);


        QDomElement eleY4=doc.createElement("Y4");
        QDomText texteleY4=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y4));
        eleY4.appendChild(texteleY4);

        QDomElement eleY5=doc.createElement("Y5");
        QDomText texteleY5=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y5));
        eleY5.appendChild(texteleY5);

        QDomElement eleY6=doc.createElement("Y6");
        QDomText texteleY6=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y6));
        eleY6.appendChild(texteleY6);

        QDomElement eleY7=doc.createElement("Y7");
        QDomText texteleY7=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y7));
        eleY7.appendChild(texteleY7);

        QDomElement eleY8=doc.createElement("Y8");
        QDomText texteleY8=doc.createTextNode(QString::number(this->ClsCheckOperatorAssociatedInspect2->Y8));
        eleY8.appendChild(texteleY8);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic);
        QDomElement * eleBasic= stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;


        eleCheckNo->appendChild(eleMaxArea);
        eleCheckNo->appendChild(eleBinThresh);
        eleCheckNo->appendChild(eleIsBlackBlob);
        eleCheckNo->appendChild(eleMinArea);
        eleCheckNo->appendChild(*eleBasic);


        eleCheckNo->appendChild(eleX1);
        eleCheckNo->appendChild(eleX2);
        eleCheckNo->appendChild(eleX3);
        eleCheckNo->appendChild(eleX4);
        eleCheckNo->appendChild(eleX5);
        eleCheckNo->appendChild(eleX6);
        eleCheckNo->appendChild(eleX7);
        eleCheckNo->appendChild(eleX8);
        eleCheckNo->appendChild(eleY1);
        eleCheckNo->appendChild(eleY2);
        eleCheckNo->appendChild(eleY3);
        eleCheckNo->appendChild(eleY4);
        eleCheckNo->appendChild(eleY5);
        eleCheckNo->appendChild(eleY6);
        eleCheckNo->appendChild(eleY7);
        eleCheckNo->appendChild(eleY8);

        return eleCheckNo;
    }

    void getLargestConnecttedComponent(cv::Mat srcImage,cv:: Mat &dstImage)
    {
        cv::Mat temp;
        cv::Mat labels;
        srcImage.copyTo(temp);


        int n_comps = connectedComponents(temp, labels, 4, CV_16U);
        vector<int> histogram_of_labels;
        for (int i = 0; i < n_comps; i++)
        {
            histogram_of_labels.push_back(0);
        }

        int rows = labels.rows;
        int cols = labels.cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
            }
        }
        histogram_of_labels.at(0) = 0;


        int maximum = 0;
        int max_idx = 0;
        for (int i = 0; i < n_comps; i++)
        {
            if (histogram_of_labels.at(i) > maximum)
            {
                maximum = histogram_of_labels.at(i);
                max_idx = i;
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (labels.at<unsigned short>(row, col) == max_idx)
                {
                    labels.at<unsigned short>(row, col) = 255;
                }
                else
                {
                    labels.at<unsigned short>(row, col) = 0;
                }
            }
        }
        labels.convertTo(dstImage, CV_8U);
    }



    //************************************
    // Method:    calculate
    // FullName:  CheckOperatorDefectInspectControl::calculate
    // Access:    virtual public
    // Returns:   -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref
    // Parameter: cv::Mat cur
    //************************************
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        if(!this->ClsCheckOperatorAssociatedInspect2->stu_CheckOperatorBasic->Basic_IsCheck) return  0;
        cv::Mat dst = cur.clone();
        int threshold = this->ClsCheckOperatorAssociatedInspect2->BinThresh;

        vector<cv::Point> vPts;
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X1,this->ClsCheckOperatorAssociatedInspect2->Y1));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X2,this->ClsCheckOperatorAssociatedInspect2->Y2));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X3,this->ClsCheckOperatorAssociatedInspect2->Y3));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X4,this->ClsCheckOperatorAssociatedInspect2->Y4));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X5,this->ClsCheckOperatorAssociatedInspect2->Y5));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X6,this->ClsCheckOperatorAssociatedInspect2->Y6));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X7,this->ClsCheckOperatorAssociatedInspect2->Y7));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X8,this->ClsCheckOperatorAssociatedInspect2->Y8));
        cv::Mat mask;
        DSIppImage::getMaskImage(mask,dst.rows,dst.cols,vPts,255);

        double area =0;
        //        int sumArea=0;
        //        DSIppImage::ippiSum_8u_C1R_CV(mask, &area);
        //        sumArea=area/255;




        cv::Mat temp;

        if(this->ClsCheckOperatorAssociatedInspect2->IsBlackBlob)
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY_INV);
        }
        else
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);

        }



        bool isDefectAll=true;
        int areaWhite=0;
        //只选择单个缺陷
        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<dst.rows;r++)
            {
                for (int c=0;c<dst.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }



        //最终统计缺陷面积
        areaWhite=0;
        for(int r=0;r<dst.rows;r++)
        {
            for (int c=0;c<dst.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255&&mask.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                    //dst.at<uchar>(r,c)=200;
                }
            }
        }


        //面积大于设定值为缺陷
        return areaWhite > this->ClsCheckOperatorAssociatedInspect2->MaxArea?1:0;


        //面积小于设定值为缺陷
        //  return   areaWhite > this->ClsCheckOperatorAssociatedInspect2->MaxArea?0:1;

    }



    //************************************
    // Method:    debugCalculator
    // FullName:  CheckOperatorDefectInspectControl::debugCalculator
    // Access:    virtual public
    // Returns:    -1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref 参考图
    // Parameter: cv::Mat cur 当前图
    // Parameter: cv::Mat & dst 算子调试需要显示的图像
    // Parameter: cv::Rect rect： roi
    // Parameter: int threshold：阈值
    // Parameter: bool isBlack：是否黑色
    // Parameter: int minArea：比较最小面积
    // Parameter: int * pArea：实测面积,像素点数量
    //************************************
    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,cv::Mat& dst, cv::Rect rect, int threshold, bool isBlack,bool isDefectAll,int maxArea,int* pArea,int * pSumArea)
    {

        cv::Mat src = cv::Mat(cur, rect);
        dst = src.clone();

        vector<cv::Point> vPts;
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X1,this->ClsCheckOperatorAssociatedInspect2->Y1));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X2,this->ClsCheckOperatorAssociatedInspect2->Y2));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X3,this->ClsCheckOperatorAssociatedInspect2->Y3));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X4,this->ClsCheckOperatorAssociatedInspect2->Y4));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X5,this->ClsCheckOperatorAssociatedInspect2->Y5));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X6,this->ClsCheckOperatorAssociatedInspect2->Y6));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X7,this->ClsCheckOperatorAssociatedInspect2->Y7));
        vPts.push_back(cv::Point(this->ClsCheckOperatorAssociatedInspect2->X8,this->ClsCheckOperatorAssociatedInspect2->Y8));
        cv::Mat mask;
        DSIppImage::getMaskImage(mask,dst.rows,dst.cols,vPts,255);


        //掩膜的面积
        double sumArea=0;
        DSIppImage::ippiSum_8u_C1R_CV(mask, &sumArea);
        *pSumArea=sumArea/255;

        //cv::imwrite("./mask.bmp",mask );

        cv::Mat temp;
        //        cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);


        if(isBlack)
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY_INV);
        }
        else
        {
            cv::threshold(dst,temp,threshold,255,cv::THRESH_BINARY);

        }



        // bool isDefectAll=true;
        int areaWhite=0;
        //只选择单个缺陷
        if(!isDefectAll)
        {
            cv::Mat structureElementOpen2 = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(-1, -1));
            cv::morphologyEx(temp, temp, cv::MORPH_OPEN, structureElementOpen2);
            for(int r=0;r<src.rows;r++)
            {
                for (int c=0;c<src.cols;c++)
                {
                    if(temp.at<uchar>(r,c)==255)
                    {
                        areaWhite++;
                    }
                }
            }
            if(areaWhite==0)
            {
                return 0;
            }
            getLargestConnecttedComponent(temp, temp);
        }



        //最终统计缺陷面积
        areaWhite=0;
        for(int r=0;r<src.rows;r++)
        {
            for (int c=0;c<src.cols;c++)
            {
                if(temp.at<uchar>(r,c)==255&&mask.at<uchar>(r,c)==255)
                {
                    areaWhite++;
                    dst.at<uchar>(r,c)=200;
                }
            }
        }
        *pArea = areaWhite;

        //面积大于设定值为缺陷
        return areaWhite > maxArea?1:0;

        //面积小于设定值为缺陷
        //return areaWhite > maxArea?0:1;

    }
public:
    CheckOperatorAssociatedInspect2 * ClsCheckOperatorAssociatedInspect2;
};





//直线检查算子
class CheckOperatorLineInspectControl :public CheckOperatorControlBase
{
public:
    CheckOperatorLineInspectControl(CheckOperatorLineInspect* clsCheckOperatorLineInspect) :ClsCheckOperatorLineInspect(clsCheckOperatorLineInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorLineInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("BinThreshold", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->BinThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Precision", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->Precision = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->Score = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->LineDir = (CheckOperatorLineInspect::ENUMLINEDIR)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("MaxShift", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->MaxShift = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("AngleMax", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->AngleMax = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleMin", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->AngleMin = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleShift", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->AngleShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("HasStdLine", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->HasStdLine = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("StdLine_xs", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_ys", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_xe", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_ye", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_ye = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_a", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_a = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_b", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_b = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_c", pEleParent, temp);
        this->ClsCheckOperatorLineInspect->StdLine_c = atof(temp.c_str());
        
        
        //加载Basic参数
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorLineInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        //加载houghline参数
        XMLElement * eleHoughLine = XmlHelper::GetElementByParent("HoughLine", pEleParent);
        StuHoughLineControl* stuStuHoughLineControl = new StuHoughLineControl(ClsCheckOperatorLineInspect->StuStuHoughLine);
        stuStuHoughLineControl->LoadXml(eleHoughLine);
        delete stuStuHoughLineControl;
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorLineInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorLineInspect->CheckType);
        
        XMLElement* eleBinThreshold = doc.NewElement("BinThreshold");
        eleBinThreshold->SetText(this->ClsCheckOperatorLineInspect->BinThreshold);
        XMLElement* elePrecision = doc.NewElement("Precision");
        elePrecision->SetText(this->ClsCheckOperatorLineInspect->Precision);
        XMLElement* eleScore = doc.NewElement("Score");
        eleScore->SetText(this->ClsCheckOperatorLineInspect->Score);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText((int)this->ClsCheckOperatorLineInspect->LineDir);
        XMLElement* eleMaxShift = doc.NewElement("MaxShift");
        eleMaxShift->SetText(this->ClsCheckOperatorLineInspect->MaxShift);
        
        XMLElement* eleAngleMax = doc.NewElement("AngleMax");
        eleAngleMax->SetText(this->ClsCheckOperatorLineInspect->AngleMax);
        XMLElement* eleAngleMin = doc.NewElement("AngleMin");
        eleAngleMin->SetText(this->ClsCheckOperatorLineInspect->AngleMin);
        XMLElement* eleAngleShift = doc.NewElement("AngleShift");
        eleAngleShift->SetText(this->ClsCheckOperatorLineInspect->AngleShift);
        XMLElement* eleHasStdLine = doc.NewElement("HasStdLine");
        eleHasStdLine->SetText(this->ClsCheckOperatorLineInspect->HasStdLine?1:0);
        XMLElement* eleStdLine_xs = doc.NewElement("StdLine_xs");
        eleStdLine_xs->SetText(this->ClsCheckOperatorLineInspect->StdLine_xs);
        XMLElement* eleStdLine_ys = doc.NewElement("StdLine_ys");
        eleStdLine_ys->SetText(this->ClsCheckOperatorLineInspect->StdLine_ys);
        XMLElement* eleStdLine_xe = doc.NewElement("StdLine_xe");
        eleStdLine_xe->SetText(this->ClsCheckOperatorLineInspect->StdLine_xe);
        XMLElement* eleStdLine_ye = doc.NewElement("StdLine_ye");
        eleStdLine_ye->SetText(this->ClsCheckOperatorLineInspect->StdLine_ye);
        XMLElement* eleStdLine_a = doc.NewElement("StdLine_a");
        eleStdLine_a->SetText(this->ClsCheckOperatorLineInspect->StdLine_a);
        XMLElement* eleStdLine_b = doc.NewElement("StdLine_b");
        eleStdLine_b->SetText(this->ClsCheckOperatorLineInspect->StdLine_b);
        XMLElement* eleStdLine_c = doc.NewElement("StdLine_c");
        eleStdLine_c->SetText(this->ClsCheckOperatorLineInspect->StdLine_c);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorLineInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        
        StuHoughLineControl* stuStuHoughLineControl = new StuHoughLineControl(this->ClsCheckOperatorLineInspect->StuStuHoughLine);
        XMLElement * eleHoughLine = stuStuHoughLineControl->generateXmlElementBlock(doc);
        delete stuStuHoughLineControl;
        
        
        eleCheckNo->InsertEndChild(eleBinThreshold);
        eleCheckNo->InsertEndChild(elePrecision);
        eleCheckNo->InsertEndChild(eleScore);
        eleCheckNo->InsertEndChild(eleLineDir);
        eleCheckNo->InsertEndChild(eleMaxShift);
        eleCheckNo->InsertEndChild(eleAngleMax);
        eleCheckNo->InsertEndChild(eleAngleMin);
        eleCheckNo->InsertEndChild(eleAngleShift);
        eleCheckNo->InsertEndChild(eleHasStdLine);
        eleCheckNo->InsertEndChild(eleStdLine_xs);
        eleCheckNo->InsertEndChild(eleStdLine_ys);
        eleCheckNo->InsertEndChild(eleStdLine_xe);
        eleCheckNo->InsertEndChild(eleStdLine_ye);
        eleCheckNo->InsertEndChild(eleStdLine_a);
        eleCheckNo->InsertEndChild(eleStdLine_b);
        eleCheckNo->InsertEndChild(eleStdLine_c);
        
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleHoughLine);
        return eleCheckNo;
    }
    
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        return 0;
    }
public:
    CheckOperatorLineInspect * ClsCheckOperatorLineInspect;
};

//折线检查算子
class CheckOperatorBrokenlineInspectControl :public CheckOperatorControlBase
{
public:
    CheckOperatorBrokenlineInspectControl(CheckOperatorBrokenlineInspect* clsCheckOperatorBrokenlineInspect) :ClsCheckOperatorBrokenlineInspect(clsCheckOperatorBrokenlineInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorBrokenlineInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("BinThreshold", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->BinThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Precision", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Precision = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Score = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleMax", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->AngleMax = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleMin", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->AngleMin = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineLength", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->LineLength = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleShift", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->AngleShift = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("Line1Seg_xs", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line1Seg_xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line1Seg_ys", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line1Seg_ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line1Seg_xe", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line1Seg_xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line1Seg_ye", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line1Seg_ye = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line2Seg_xs", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line2Seg_xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line2Seg_ys", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line2Seg_ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line2Seg_xe", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line2Seg_xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("Line2Seg_ye", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->Line2Seg_ye = atof(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("HasStdLine", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->HasStdLine = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_xs", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_xs = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_ys", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_ys = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_xe", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_xe = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_ye", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_ye = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_a", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_a = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_b", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_b = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_c", pEleParent, temp);
        this->ClsCheckOperatorBrokenlineInspect->StdLine_c = atof(temp.c_str());
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorBrokenlineInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        
        XMLElement * eleHoughLine = XmlHelper::GetElementByParent("HoughLine", pEleParent);
        StuHoughLineControl* stuStuHoughLineControl = new StuHoughLineControl(ClsCheckOperatorBrokenlineInspect->StuStuHoughLine);
        stuStuHoughLineControl->LoadXml(eleHoughLine);
        delete stuStuHoughLineControl;
        
        XMLElement * eleLineLen= XmlHelper::GetElementByParent("LineLen", pEleParent);
        StuLineLenLineLenControl* stuStuLineLenLineLenControl = new StuLineLenLineLenControl(this->ClsCheckOperatorBrokenlineInspect->StuStuLineLenLineLen);
        stuStuLineLenLineLenControl->LoadXml(eleLineLen);
        delete stuStuLineLenLineLenControl;
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorBrokenlineInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorBrokenlineInspect->CheckType);
        
        XMLElement* eleBinThreshold = doc.NewElement("BinThreshold");
        eleBinThreshold->SetText(this->ClsCheckOperatorBrokenlineInspect->BinThreshold);
        XMLElement* elePrecision = doc.NewElement("Precision");
        elePrecision->SetText(this->ClsCheckOperatorBrokenlineInspect->Precision);
        XMLElement* eleScore = doc.NewElement("Score");
        eleScore->SetText(this->ClsCheckOperatorBrokenlineInspect->Score);
        XMLElement* eleAngleMax = doc.NewElement("AngleMax");
        eleAngleMax->SetText(this->ClsCheckOperatorBrokenlineInspect->AngleMax);
        XMLElement* eleAngleMin = doc.NewElement("AngleMin");
        eleAngleMin->SetText(this->ClsCheckOperatorBrokenlineInspect->AngleMin);
        XMLElement* eleLineLength = doc.NewElement("LineLength");
        eleLineLength->SetText(this->ClsCheckOperatorBrokenlineInspect->LineLength);
        XMLElement* eleAngleShift = doc.NewElement("AngleShift");
        eleAngleShift->SetText(this->ClsCheckOperatorBrokenlineInspect->AngleShift);
        XMLElement* eleLine1Seg_xs = doc.NewElement("Line1Seg_xs");
        eleLine1Seg_xs->SetText(this->ClsCheckOperatorBrokenlineInspect->Line1Seg_xs);
        XMLElement* eleLine1Seg_ys = doc.NewElement("Line1Seg_ys");
        eleLine1Seg_ys->SetText(this->ClsCheckOperatorBrokenlineInspect->Line1Seg_ys);
        
        XMLElement* eleLine1Seg_xe = doc.NewElement("Line1Seg_xe");
        eleLine1Seg_xe->SetText(this->ClsCheckOperatorBrokenlineInspect->Line1Seg_xe);
        XMLElement* eleLine1Seg_ye = doc.NewElement("Line1Seg_ye");
        eleLine1Seg_ye->SetText(this->ClsCheckOperatorBrokenlineInspect->Line1Seg_ye);
        XMLElement* eleLine2Seg_xs = doc.NewElement("Line2Seg_xs");
        eleLine2Seg_xs->SetText(this->ClsCheckOperatorBrokenlineInspect->Line2Seg_xs);
        XMLElement* eleLine2Seg_ys = doc.NewElement("Line2Seg_ys");
        eleLine2Seg_ys->SetText(this->ClsCheckOperatorBrokenlineInspect->Line2Seg_ys);
        XMLElement* eleLine2Seg_xe = doc.NewElement("Line2Seg_xe");
        eleLine2Seg_xe->SetText(this->ClsCheckOperatorBrokenlineInspect->Line2Seg_xe);
        XMLElement* eleLine2Seg_ye = doc.NewElement("Line2Seg_ye");
        eleLine2Seg_ye->SetText(this->ClsCheckOperatorBrokenlineInspect->Line2Seg_ye);
        XMLElement* eleHasStdLine = doc.NewElement("HasStdLine");
        eleHasStdLine->SetText(this->ClsCheckOperatorBrokenlineInspect->HasStdLine?1:0);
        
        XMLElement* eleStdLine_xs = doc.NewElement("StdLine_xs");
        eleStdLine_xs->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_xs);
        XMLElement* eleStdLine_ys = doc.NewElement("StdLine_ys");
        eleStdLine_ys->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_ys);
        XMLElement* eleStdLine_xe = doc.NewElement("StdLine_xe");
        eleStdLine_xe->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_xe);
        XMLElement* eleStdLine_ye = doc.NewElement("StdLine_ye");
        eleStdLine_ye->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_ye);
        XMLElement* eleStdLine_a = doc.NewElement("StdLine_a");
        eleStdLine_a->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_a);
        XMLElement* eleStdLine_b = doc.NewElement("StdLine_b");
        eleStdLine_b->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_b);
        XMLElement* eleStdLine_c = doc.NewElement("StdLine_c");
        eleStdLine_c->SetText(this->ClsCheckOperatorBrokenlineInspect->StdLine_c);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorBrokenlineInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        StuHoughLineControl *  stuStuHoughLineControl = new StuHoughLineControl(this->ClsCheckOperatorBrokenlineInspect->StuStuHoughLine);
        XMLElement * eleHoughLine = stuStuHoughLineControl->generateXmlElementBlock(doc);
        delete stuStuHoughLineControl;
        
        StuLineLenLineLenControl *  stuStuLineLenLineLenControl = new StuLineLenLineLenControl(this->ClsCheckOperatorBrokenlineInspect->StuStuLineLenLineLen);
        XMLElement * eleLineLen = stuStuLineLenLineLenControl->generateXmlElementBlock(doc);
        delete stuStuLineLenLineLenControl;
        
        eleCheckNo->InsertEndChild(eleBinThreshold);
        eleCheckNo->InsertEndChild(elePrecision);
        eleCheckNo->InsertEndChild(eleScore);
        
        eleCheckNo->InsertEndChild(eleAngleMax);
        eleCheckNo->InsertEndChild(eleAngleMin);
        eleCheckNo->InsertEndChild(eleLineLength);
        eleCheckNo->InsertEndChild(eleAngleShift);
        eleCheckNo->InsertEndChild(eleLine1Seg_xs);
        
        eleCheckNo->InsertEndChild(eleLine1Seg_ys);
        eleCheckNo->InsertEndChild(eleLine1Seg_xe);
        eleCheckNo->InsertEndChild(eleLine1Seg_ye);
        eleCheckNo->InsertEndChild(eleLine2Seg_xs);
        eleCheckNo->InsertEndChild(eleLine2Seg_ys);
        
        eleCheckNo->InsertEndChild(eleLine2Seg_xe);
        eleCheckNo->InsertEndChild(eleLine2Seg_ye);
        eleCheckNo->InsertEndChild(eleHasStdLine);
        eleCheckNo->InsertEndChild(eleStdLine_xs);
        eleCheckNo->InsertEndChild(eleStdLine_ys);
        
        eleCheckNo->InsertEndChild(eleStdLine_xe);
        eleCheckNo->InsertEndChild(eleStdLine_ye);
        eleCheckNo->InsertEndChild(eleStdLine_a);
        eleCheckNo->InsertEndChild(eleStdLine_b);
        eleCheckNo->InsertEndChild(eleStdLine_c);
        
        
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleHoughLine);
        eleCheckNo->InsertEndChild(eleLineLen);
        return eleCheckNo;
    }
    
    
    virtual int debugCalculator(cv::Mat src,
                                int line1_x1,int line1_y1, int line1_x2, int line1_y2,
                                int line2_x1, int line2_y1, int line2_x2, int line2_y2,
                                int thresh,int minLineThresh,int minLineLen,int maxLineGap ,
                                int angleMin,int angleMax,int disMin,bool hasRef,
                                int * p1x, int* p1y, int * p2x, int* p2y,double& resLineLen,int& score)
    {
        
        return 0;
    }
    
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {
        return 0;
    }
    
public:
    CheckOperatorBrokenlineInspect * ClsCheckOperatorBrokenlineInspect;
};

//边缘定位,边缘检测
class CheckOperatorEdgePositioningControl :public CheckOperatorControlBase
{
public:
    CheckOperatorEdgePositioningControl(CheckOperatorEdgePositioning* clsCheckOperatorEdgePositioning) :ClsCheckOperatorEdgePositioning(clsCheckOperatorEdgePositioning) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        string temp = "";
        XmlHelper::GetElementTextFromParent("EdgePos", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->EdgePos = (CheckOperatorEdgePositioning::ENUMEDGEPOS)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->Score = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("MaxPosShift", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->MaxPosShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("MaxAngleShift", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->MaxAngleShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("HasStdLine", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->HasStdLine = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_a", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->StdLine_a = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_b", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->StdLine_b = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLine_c", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->StdLine_c = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("ResetReg", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->ResetReg = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("BinThresh", pEleParent, temp);
        this->ClsCheckOperatorEdgePositioning->BinThresh = atoi(temp.c_str());
        
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        
        XMLElement * eleEdgeLine = XmlHelper::GetElementByParent("EdgeLine", pEleParent);
        
        StuEdgeLineControl *  stuStuEdgeLineControl = new StuEdgeLineControl(this->ClsCheckOperatorEdgePositioning->StuStuEdgeLine);
        stuStuEdgeLineControl->LoadXml(eleEdgeLine);
        
        delete stuStuEdgeLineControl;
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorEdgePositioning->CheckType);
        
        XMLElement* eleEdgePos = doc.NewElement("EdgePos");
        eleEdgePos->SetText(this->ClsCheckOperatorEdgePositioning->EdgePos);
        XMLElement* eleScore = doc.NewElement("Score");
        eleScore->SetText(this->ClsCheckOperatorEdgePositioning->Score);
        XMLElement* eleMaxPosShift = doc.NewElement("MaxPosShift");
        eleMaxPosShift->SetText(this->ClsCheckOperatorEdgePositioning->MaxPosShift);
        XMLElement* eleMaxAngleShift = doc.NewElement("MaxAngleShift");
        eleMaxAngleShift->SetText(this->ClsCheckOperatorEdgePositioning->MaxAngleShift);
        XMLElement* eleHasStdLine = doc.NewElement("HasStdLine");
        eleHasStdLine->SetText(this->ClsCheckOperatorEdgePositioning->HasStdLine);
        XMLElement* eleStdLine_a = doc.NewElement("StdLine_a");
        eleStdLine_a->SetText(this->ClsCheckOperatorEdgePositioning->StdLine_a);
        XMLElement* eleStdLine_b = doc.NewElement("StdLine_b");
        eleStdLine_b->SetText(this->ClsCheckOperatorEdgePositioning->StdLine_b);
        XMLElement* eleStdLine_c = doc.NewElement("StdLine_c");
        eleStdLine_c->SetText(this->ClsCheckOperatorEdgePositioning->StdLine_c);
        XMLElement* eleResetReg = doc.NewElement("ResetReg");
        eleResetReg->SetText(this->ClsCheckOperatorEdgePositioning->ResetReg);
        XMLElement* eleBinThresh = doc.NewElement("BinThresh");
        eleBinThresh->SetText(this->ClsCheckOperatorEdgePositioning->BinThresh);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        StuEdgeLineControl *  stuStuEdgeLineControl = new StuEdgeLineControl(this->ClsCheckOperatorEdgePositioning->StuStuEdgeLine);
        XMLElement * eleEdgeLine = stuStuEdgeLineControl->generateXmlElementBlock(doc);
        delete stuStuEdgeLineControl;
        
        eleCheckNo->InsertEndChild(eleEdgePos);
        eleCheckNo->InsertEndChild(eleScore);
        eleCheckNo->InsertEndChild(eleMaxPosShift);
        eleCheckNo->InsertEndChild(eleMaxAngleShift);
        eleCheckNo->InsertEndChild(eleHasStdLine);
        eleCheckNo->InsertEndChild(eleStdLine_a);
        eleCheckNo->InsertEndChild(eleStdLine_b);
        eleCheckNo->InsertEndChild(eleStdLine_c);
        eleCheckNo->InsertEndChild(eleResetReg);
        eleCheckNo->InsertEndChild(eleBinThresh);
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleEdgeLine);
        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument& doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");

        QDomElement* eleCheckNo = new QDomElement(tempeleCheckNo);


        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorEdgePositioning->CheckType);

        QDomElement eleEdgePos=doc.createElement("EdgePos");
        QDomText texteleEdgePos=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->EdgePos));
        eleEdgePos.appendChild(texteleEdgePos);

        QDomElement eleScore=doc.createElement("Score");
        QDomText texteleScore=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->Score));
        eleScore.appendChild(texteleScore);

        QDomElement eleMaxPosShift=doc.createElement("MaxPosShift");
        QDomText texteleMaxPosShift=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->MaxPosShift));
        eleMaxPosShift.appendChild(texteleMaxPosShift);

        QDomElement eleMaxAngleShift=doc.createElement("MaxAngleShift");
        QDomText texteleMaxAngleShift=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->MaxAngleShift));
        eleMaxAngleShift.appendChild(texteleMaxAngleShift);

        QDomElement eleHasStdLine=doc.createElement("HasStdLine");
        QDomText texteleHasStdLine=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->HasStdLine));
        eleHasStdLine.appendChild(texteleHasStdLine);

        QDomElement eleStdLine_a=doc.createElement("StdLine_a");
        QDomText texteleStdLine_a=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->StdLine_a));
        eleStdLine_a.appendChild(texteleStdLine_a);

        QDomElement eleStdLine_b=doc.createElement("StdLine_b");
        QDomText texteleStdLine_b=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->StdLine_b));
        eleStdLine_b.appendChild(texteleStdLine_b);


        QDomElement eleStdLine_c=doc.createElement("StdLine_c");
        QDomText texteleStdLine_c=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->StdLine_c));
        eleStdLine_c.appendChild(texteleStdLine_c);

        QDomElement eleResetReg=doc.createElement("ResetReg");
        QDomText texteleResetReg=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->ResetReg));
        eleResetReg.appendChild(texteleResetReg);


        QDomElement eleBinThresh=doc.createElement("BinThresh");
        QDomText texteleBinThresh=doc.createTextNode(QString::number(this->ClsCheckOperatorEdgePositioning->BinThresh));
        eleBinThresh.appendChild(texteleBinThresh);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorEdgePositioning->stu_CheckOperatorBasic);
        QDomElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;

        StuEdgeLineControl *  stuStuEdgeLineControl = new StuEdgeLineControl(this->ClsCheckOperatorEdgePositioning->StuStuEdgeLine);
        QDomElement * eleEdgeLine = stuStuEdgeLineControl->generateXmlElementBlock2(doc);
        delete stuStuEdgeLineControl;

        eleCheckNo->appendChild(eleEdgePos);
        eleCheckNo->appendChild(eleScore);
        eleCheckNo->appendChild(eleMaxPosShift);
        eleCheckNo->appendChild(eleMaxAngleShift);
        eleCheckNo->appendChild(eleHasStdLine);
        eleCheckNo->appendChild(eleStdLine_a);
        eleCheckNo->appendChild(eleStdLine_b);
        eleCheckNo->appendChild(eleStdLine_c);
        eleCheckNo->appendChild(eleResetReg);
        eleCheckNo->appendChild(eleBinThresh);
        eleCheckNo->appendChild(*eleBasic);
        eleCheckNo->appendChild(*eleEdgeLine);
        return eleCheckNo;
    }
    //    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    //    {
    
    //        return 0;
    //    }
    
    //************************************
    // Method:    calculate
    // FullName:  CheckOperatorEdgePositioningControl::calculate
    // Access:    virtual public
    // Returns:   int:-1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    // Qualifier:
    // Parameter: cv::Mat & ref
    // Parameter: cv::Mat & cur
    //************************************
    virtual int calculate(cv::Mat& ref, cv::Mat cur,int * ptx_basis,int * pty_basis,int * pscore)
    {
        return  0;
    }
    
    virtual void getRecommentThresh(cv::Mat & cur,CheckOperatorEdgePositioning::ENUMEDGEPOS dir,int* thresh)
    {

    }
    
    
    
    virtual int debugCalculator(cv::Mat& ref, cv::Mat cur,int dir, int thres,int * ptx,int * pty,int* score)
    {

        return  0;
    }
    
public:
    CheckOperatorEdgePositioning * ClsCheckOperatorEdgePositioning;
};

//灰度测量算子
class CheckOperatorGrayMeasurementControl :public CheckOperatorControlBase
{
public:
    CheckOperatorGrayMeasurementControl(CheckOperatorGrayMeasurement* clsCheckOperatorGrayMeasurement) :ClsCheckOperatorGrayMeasurement(clsCheckOperatorGrayMeasurement) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        
        this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("AverageStd", pEleParent, temp);
        this->ClsCheckOperatorGrayMeasurement->AverageStd = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("AverageLimited", pEleParent, temp);
        this->ClsCheckOperatorGrayMeasurement->AverageLimited = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("DevStd", pEleParent, temp);
        this->ClsCheckOperatorGrayMeasurement->DevStd = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("DevLimited", pEleParent, temp);
        this->ClsCheckOperatorGrayMeasurement->DevLimited = atof(temp.c_str());
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorGrayMeasurement->CheckType);
        
        XMLElement* eleAverageStd = doc.NewElement("AverageStd");
        eleAverageStd->SetText(this->ClsCheckOperatorGrayMeasurement->AverageStd);
        XMLElement* eleAverageLimited = doc.NewElement("AverageLimited");
        eleAverageLimited->SetText(this->ClsCheckOperatorGrayMeasurement->AverageLimited);
        XMLElement* eleDevStd = doc.NewElement("DevStd");
        eleDevStd->SetText(this->ClsCheckOperatorGrayMeasurement->DevStd);
        XMLElement* eleDevLimited = doc.NewElement("DevLimited");
        eleDevLimited->SetText(this->ClsCheckOperatorGrayMeasurement->DevLimited);
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        eleCheckNo->InsertEndChild(eleAverageStd);
        eleCheckNo->InsertEndChild(eleAverageLimited);
        eleCheckNo->InsertEndChild(eleDevStd);
        eleCheckNo->InsertEndChild(eleDevLimited);
        eleCheckNo->InsertEndChild(eleBasic);
        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {
        QDomElement tempeleCheckNo=doc.createElement("CheckNo");
        QDomElement* eleCheckNo = new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("Index", this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorGrayMeasurement->CheckType);


        QDomElement eleAverageStd=doc.createElement("AverageStd");
        QDomText texteleAverageStd=doc.createTextNode(QString::number(this->ClsCheckOperatorGrayMeasurement->AverageStd));
        eleAverageStd.appendChild(texteleAverageStd);

        QDomElement eleAverageLimited=doc.createElement("AverageLimited");
        QDomText texteleAverageLimited=doc.createTextNode(QString::number(this->ClsCheckOperatorGrayMeasurement->AverageLimited));
        eleAverageLimited.appendChild(texteleAverageLimited);

        QDomElement eleDevStd=doc.createElement("DevStd");
        QDomText texteleDevStd=doc.createTextNode(QString::number(this->ClsCheckOperatorGrayMeasurement->DevStd));
        eleDevStd.appendChild(texteleDevStd);

        QDomElement eleDevLimited=doc.createElement("DevLimited");
        QDomText texteleDevLimited=doc.createTextNode(QString::number(this->ClsCheckOperatorGrayMeasurement->DevLimited));
        eleDevLimited.appendChild(texteleDevLimited);

        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorGrayMeasurement->stu_CheckOperatorBasic);
        QDomElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;

        eleCheckNo->appendChild(eleAverageStd);
        eleCheckNo->appendChild(eleAverageLimited);
        eleCheckNo->appendChild(eleDevStd);
        eleCheckNo->appendChild(eleDevLimited);
        eleCheckNo->appendChild(*eleBasic);
        return eleCheckNo;
    }
    
    virtual int calculate(cv::Mat& ref, cv::Mat& cur)
    {
        return 0;

    }
    

    virtual int debugCalculator(cv::Mat& ref, cv::Mat& cur,cv::Mat& dst, cv::Rect rect, double setMean, double setMeanBasis, double setStddev, double setStddevBasis,double * calcMean,double* calcStddev)
    {
        return 0;

    }
    
public:
    CheckOperatorGrayMeasurement * ClsCheckOperatorGrayMeasurement;
};

//拉线算子
class CheckOperatorStayInspectControl :public CheckOperatorControlBase
{
public:
    CheckOperatorStayInspectControl(CheckOperatorStayInspect* clsCheckOperatorStayInspect) :ClsCheckOperatorStayInspect(clsCheckOperatorStayInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorStayInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("BinThreshold", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->BinThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Precision", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->Precision = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->Score = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->LineDir = (CheckOperatorStayInspect::ENUMLINEDIR)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleMax", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->AngleMax =atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleMin", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->AngleMin = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("PackWidth", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->PackWidth = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("GrayAve", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->GrayAve = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("GrayStd", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->GrayStd = atof(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdLineNum", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->StdLineNum = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleShift", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->AngleShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("StdWidth", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->StdWidth = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineType", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->LineType = (CheckOperatorStayInspect::ENUMLINTYPE)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineColor", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->LineColor = (CheckOperatorStayInspect::ENUMLINECOLOR)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AveShift", pEleParent, temp);
        this->ClsCheckOperatorStayInspect->AveShift = atoi(temp.c_str());
        
        //加载Basic参数
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorStayInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        //加载houghline参数
        XMLElement * eleHoughLine = XmlHelper::GetElementByParent("HoughLine", pEleParent);
        StuHoughLineControl* stuStuHoughLineControl = new StuHoughLineControl(ClsCheckOperatorStayInspect->StuStuHoughLine);
        stuStuHoughLineControl->LoadXml(eleHoughLine);
        delete stuStuHoughLineControl;
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorStayInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorStayInspect->CheckType);
        
        XMLElement* eleBinThreshold = doc.NewElement("BinThreshold");
        eleBinThreshold->SetText(this->ClsCheckOperatorStayInspect->BinThreshold);
        XMLElement* elePrecision = doc.NewElement("Precision");
        elePrecision->SetText(this->ClsCheckOperatorStayInspect->Precision);
        XMLElement* eleScore = doc.NewElement("Score");
        eleScore->SetText(this->ClsCheckOperatorStayInspect->Score);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText((int)this->ClsCheckOperatorStayInspect->LineDir);
        XMLElement* eleAngleMax = doc.NewElement("AngleMax");
        eleAngleMax->SetText((int)this->ClsCheckOperatorStayInspect->AngleMax);
        XMLElement* eleAngleMin = doc.NewElement("AngleMin");
        eleAngleMin->SetText((int)this->ClsCheckOperatorStayInspect->AngleMin);
        XMLElement* elePackWidth = doc.NewElement("PackWidth");
        elePackWidth->SetText((int)this->ClsCheckOperatorStayInspect->PackWidth);
        XMLElement* eleGrayAve = doc.NewElement("GrayAve");
        eleGrayAve->SetText((int)this->ClsCheckOperatorStayInspect->GrayAve);
        XMLElement* eleGrayStd = doc.NewElement("GrayStd");
        eleGrayStd->SetText((int)this->ClsCheckOperatorStayInspect->GrayStd);
        XMLElement* eleStdLineNum = doc.NewElement("StdLineNum");
        eleStdLineNum->SetText((int)this->ClsCheckOperatorStayInspect->StdLineNum);
        XMLElement* eleAngleShift = doc.NewElement("AngleShift");
        eleAngleShift->SetText((int)this->ClsCheckOperatorStayInspect->AngleShift);
        XMLElement* eleStdWidth = doc.NewElement("StdWidth");
        eleStdWidth->SetText((int)this->ClsCheckOperatorStayInspect->StdWidth);
        XMLElement* eleLineType = doc.NewElement("LineType");
        eleLineType->SetText((int)this->ClsCheckOperatorStayInspect->LineType);
        XMLElement* eleLineColor = doc.NewElement("LineColor");
        eleLineColor->SetText((int)this->ClsCheckOperatorStayInspect->LineColor);
        XMLElement* eleAveShift = doc.NewElement("AveShift");
        eleAveShift->SetText((int)this->ClsCheckOperatorStayInspect->AveShift);
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorStayInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        StuHoughLineControl *  stuStuHoughLineControl = new StuHoughLineControl(this->ClsCheckOperatorStayInspect->StuStuHoughLine);
        XMLElement * eleHoughLine = stuStuHoughLineControl->generateXmlElementBlock(doc);
        delete stuStuHoughLineControl;
        
        eleCheckNo->InsertEndChild(eleBinThreshold);
        eleCheckNo->InsertEndChild(elePrecision);
        eleCheckNo->InsertEndChild(eleScore);
        eleCheckNo->InsertEndChild(eleLineDir);
        eleCheckNo->InsertEndChild(eleAngleMax);
        eleCheckNo->InsertEndChild(eleAngleMin);
        eleCheckNo->InsertEndChild(elePackWidth);
        eleCheckNo->InsertEndChild(eleGrayAve);
        eleCheckNo->InsertEndChild(eleGrayStd);
        eleCheckNo->InsertEndChild(eleStdLineNum);
        eleCheckNo->InsertEndChild(eleAngleShift);
        eleCheckNo->InsertEndChild(eleStdWidth);
        eleCheckNo->InsertEndChild(eleLineType);
        eleCheckNo->InsertEndChild(eleLineColor);
        eleCheckNo->InsertEndChild(eleAveShift);
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleHoughLine);
        return eleCheckNo;
    }
    
    
    
    virtual int debugCalculate(cv::Mat& ref, cv::Mat cur,CheckOperatorStayInspect::ENUMLINEDIR dir,int anglebiasis,int setScore,int thresh,int* p1x,int* p1y,int* p2x,int* p2y,int* score,double * angel)
    {
        return 0;
    }
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        
        return 0;
    }
    
    virtual void getRecommentThresh(cv::Mat & cur,CheckOperatorStayInspect::ENUMLINEDIR dir,int* thresh)
    {

    }
    
    
    
    
    
    
public:
    CheckOperatorStayInspect * ClsCheckOperatorStayInspect;
    
    
};



//双拉线算子
class CheckOperatorDoubleStayInspectControl :public CheckOperatorControlBase
{
public:
    CheckOperatorDoubleStayInspectControl(CheckOperatorDoubleStayInspect* clsCheckOperatorDoubleStayInspect) :ClsCheckOperatorDoubleStayInspect(clsCheckOperatorDoubleStayInspect) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorDoubleStayInspect->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("BinThreshold", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->BinThreshold = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("Score", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->Score = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("LineDir", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->LineDir = (CheckOperatorDoubleStayInspect::ENUMLINEDIR)atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("AngleShift", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->AngleShift = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("DisMin", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->DisMin = atoi(temp.c_str());
        XmlHelper::GetElementTextFromParent("DisMax", pEleParent, temp);
        this->ClsCheckOperatorDoubleStayInspect->DisMax = atoi(temp.c_str());
        
        //加载Basic参数
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDoubleStayInspect->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        //加载houghline参数
        XMLElement * eleHoughLine = XmlHelper::GetElementByParent("HoughLine", pEleParent);
        StuHoughLineControl* stuStuHoughLineControl = new StuHoughLineControl(ClsCheckOperatorDoubleStayInspect->StuStuHoughLine);
        stuStuHoughLineControl->LoadXml(eleHoughLine);
        delete stuStuHoughLineControl;
        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorDoubleStayInspect->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorDoubleStayInspect->CheckType);
        
        XMLElement* eleBinThreshold = doc.NewElement("BinThreshold");
        eleBinThreshold->SetText(this->ClsCheckOperatorDoubleStayInspect->BinThreshold);
        XMLElement* eleScore = doc.NewElement("Score");
        eleScore->SetText(this->ClsCheckOperatorDoubleStayInspect->Score);
        XMLElement* eleLineDir = doc.NewElement("LineDir");
        eleLineDir->SetText((int)this->ClsCheckOperatorDoubleStayInspect->LineDir);
        
        XMLElement* eleDisMin = doc.NewElement("DisMin");
        eleDisMin->SetText((int)this->ClsCheckOperatorDoubleStayInspect->DisMin);
        
        XMLElement* eleDisMax = doc.NewElement("DisMax");
        eleDisMax->SetText((int)this->ClsCheckOperatorDoubleStayInspect->DisMax);
        
        XMLElement* eleAngleShift = doc.NewElement("AngleShift");
        eleAngleShift->SetText((int)this->ClsCheckOperatorDoubleStayInspect->AngleShift);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorDoubleStayInspect->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        StuHoughLineControl *  stuStuHoughLineControl = new StuHoughLineControl(this->ClsCheckOperatorDoubleStayInspect->StuStuHoughLine);
        XMLElement * eleHoughLine = stuStuHoughLineControl->generateXmlElementBlock(doc);
        delete stuStuHoughLineControl;
        
        eleCheckNo->InsertEndChild(eleBinThreshold);
        eleCheckNo->InsertEndChild(eleScore);
        eleCheckNo->InsertEndChild(eleLineDir);
        eleCheckNo->InsertEndChild(eleAngleShift);
        eleCheckNo->InsertEndChild(eleDisMin);
        eleCheckNo->InsertEndChild(eleDisMax);
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleHoughLine);
        return eleCheckNo;
    }
    
    
    
    virtual int debugCalculate(cv::Mat& ref, cv::Mat cur,CheckOperatorStayInspect::ENUMLINEDIR dir,int anglebiasis,
                               int setScore,int thresh,int dismin,int dismax,int* p1x1,int* p1y1,int* p1x2,int* p1y2,
                               int* p2x1,int* p2y1,int* p2x2,int* p2y2,
                               int* score1,int* score2,double * angel1,double * angel2)
    {
        

        return 0;
    }
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur)
    {

        return 0;
    }
    
    virtual void getRecommentThresh(cv::Mat & cur,CheckOperatorStayInspect::ENUMLINEDIR dir,int* thresh)
    {
    }
    
    
    
    
    
    
public:
    CheckOperatorDoubleStayInspect * ClsCheckOperatorDoubleStayInspect;
    
    
};



//对象搜索模板匹配算子
class CheckOperatorTemplateMatchControl :public CheckOperatorControlBase
{
public:
    CheckOperatorTemplateMatchControl(CheckOperatorTemplateMatch* clsCheckOperatorTemplateMatch) :ClsCheckOperatorTemplateMatch(clsCheckOperatorTemplateMatch) {
    }
    //从xml文件中读取
    virtual int LoadXml(XMLElement* pEleParent)
    {
        this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index = pEleParent->IntAttribute("Index");
        
        string temp = "";
        XmlHelper::GetElementTextFromParent("MatchThresh", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->MatchThresh = atoi(temp.c_str());
        
        XmlHelper::GetElementTextFromParent("Code", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->Code = temp;
        
        
        XmlHelper::GetElementTextFromParent("SearchNum", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->SearchNum = atoi(temp.c_str());
        
        XMLElement * eleBasic = XmlHelper::GetElementByParent("Basic", pEleParent);
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml(eleBasic);
        delete stuStuCheckOperatorBasicControl;
        
        XMLElement * eleGrayMatch = XmlHelper::GetElementByParent("GrayMatch", pEleParent);
        StuGrayMatchControl *  stuStuGrayMatchControl = new StuGrayMatchControl(this->ClsCheckOperatorTemplateMatch->StuStuGrayMatch);
        stuStuGrayMatchControl->LoadXml(eleGrayMatch);
        delete stuStuGrayMatchControl;
        
        return 0;
    }

    virtual int LoadXml2(QDomElement pEleParent)
    {
        this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index = pEleParent.attribute("Index").toInt();

        QString temp = "";
        XmlHelper::GetElementTextFromParent2("MatchThresh", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->MatchThresh = atoi(temp.toStdString().c_str());

        XmlHelper::GetElementTextFromParent2("Code", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->Code = temp.toStdString();


        XmlHelper::GetElementTextFromParent2("SearchNum", pEleParent, temp);
        this->ClsCheckOperatorTemplateMatch->SearchNum = atoi(temp.toStdString().c_str());

        QDomElement* eleBasic = XmlHelper::GetElementByParent2("Basic", pEleParent);

        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic);
        stuStuCheckOperatorBasicControl->LoadXml2(*eleBasic);
        delete stuStuCheckOperatorBasicControl;

        QDomElement * eleGrayMatch = XmlHelper::GetElementByParent2("GrayMatch", pEleParent);
        StuGrayMatchControl *  stuStuGrayMatchControl = new StuGrayMatchControl(this->ClsCheckOperatorTemplateMatch->StuStuGrayMatch);
        stuStuGrayMatchControl->LoadXml2(*eleGrayMatch);
        delete  eleGrayMatch;
        delete stuStuGrayMatchControl;

        return 0;
    }
    
    virtual XMLElement * generateXmlElementBlock(TinyXmlDocument & doc)
    {
        
        XMLElement* eleCheckNo = doc.NewElement("CheckNo");
        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index);
        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorTemplateMatch->CheckType);
        
        XMLElement* eleMatchThresh = doc.NewElement("MatchThresh");
        eleMatchThresh->SetText(this->ClsCheckOperatorTemplateMatch->MatchThresh);
        
        XMLElement* eleCode = doc.NewElement("Code");
        eleCode->SetText(this->ClsCheckOperatorTemplateMatch->Code.c_str());
        
        XMLElement* eleSearchNum = doc.NewElement("SearchNum");
        eleSearchNum ->SetText(this->ClsCheckOperatorTemplateMatch->SearchNum);
        
        
        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic);
        XMLElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock(doc);
        delete stuStuCheckOperatorBasicControl;
        
        StuGrayMatchControl * stuStuGrayMatchControl = new StuGrayMatchControl(this->ClsCheckOperatorTemplateMatch->StuStuGrayMatch);
        XMLElement * eleGrayMatch = stuStuGrayMatchControl->generateXmlElementBlock(doc);
        delete stuStuGrayMatchControl;
        
        eleCheckNo->InsertEndChild(eleCode);
        eleCheckNo->InsertEndChild(eleMatchThresh);
        eleCheckNo->InsertEndChild(eleSearchNum);
        eleCheckNo->InsertEndChild(eleBasic);
        eleCheckNo->InsertEndChild(eleGrayMatch);
        return eleCheckNo;
    }

    virtual QDomElement * generateXmlElementBlock2(QDomDocument &doc)
    {


        QDomElement tempeleCheckNo = doc.createElement("CheckNo");
        QDomElement *eleCheckNo=new QDomElement(tempeleCheckNo);
        eleCheckNo->setAttribute("index",this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index);
        eleCheckNo->setAttribute("Type", (int)this->ClsCheckOperatorTemplateMatch->CheckType);

        //        eleCheckNo->SetAttribute("Index", this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index);
        //        eleCheckNo->SetAttribute("Type", (int)this->ClsCheckOperatorTemplateMatch->CheckType);


        QDomElement eleMatchThresh=doc.createElement("MatchThresh");
        QDomText texteleMatchThresh=doc.createTextNode(QString::number(this->ClsCheckOperatorTemplateMatch->MatchThresh));
        eleMatchThresh.appendChild(texteleMatchThresh);

        QDomElement eleCode=doc.createElement("Code");
        QDomText texteleCode=doc.createTextNode(QString(this->ClsCheckOperatorTemplateMatch->Code.c_str()));
        eleCode.appendChild(texteleCode);

        QDomElement eleSearchNum=doc.createElement("SearchNum");
        QDomText texteleSearchNume=doc.createTextNode(QString::number(this->ClsCheckOperatorTemplateMatch->SearchNum));
        eleSearchNum.appendChild(texteleSearchNume);


        StuCheckOperatorBasicControl *  stuStuCheckOperatorBasicControl = new StuCheckOperatorBasicControl(this->ClsCheckOperatorTemplateMatch->stu_CheckOperatorBasic);
        QDomElement * eleBasic = stuStuCheckOperatorBasicControl->generateXmlElementBlock2(doc);
        delete stuStuCheckOperatorBasicControl;

        StuGrayMatchControl * stuStuGrayMatchControl = new StuGrayMatchControl(this->ClsCheckOperatorTemplateMatch->StuStuGrayMatch);
        QDomElement * eleGrayMatch = stuStuGrayMatchControl->generateXmlElementBlock2(doc);
        delete stuStuGrayMatchControl;

        eleCheckNo->appendChild(eleCode);
        eleCheckNo->appendChild(eleMatchThresh);
        eleCheckNo->appendChild(eleSearchNum);
        eleCheckNo->appendChild(*eleBasic);
        eleCheckNo->appendChild(*eleGrayMatch);


        return eleCheckNo;
    }
    
    
    virtual int calculate(cv::Mat& ref, cv::Mat cur,std::map<int,OpencvImage>& templates,int* x_basis,int* y_basis,int* score)
    {
        return 0;
    }
    
    
    // Returns:   int:-1：执行出错     0：执行正确，检查结果正确    1：执行正确，检查结果错误
    virtual int debugCalculator(cv::Mat& ref, cv::Mat& cur,cv::Mat& target,int setScore,int * pScore,int* px,int *py)
    {

        return  0;
    }
    
public:
    CheckOperatorTemplateMatch * ClsCheckOperatorTemplateMatch;
};




//AbstractCheckOperatorControlControl_t:抽象的算子类
template <class AbstractCheckOperatorControl_t,class _p>
class AbstractCheckOperatorControlFactory
{
public:
    virtual AbstractCheckOperatorControl_t *CreateCheckOperatorControl( _p p) = 0;
    
    virtual ~AbstractCheckOperatorControlFactory() {}
    
};

//ConcreteCheckOperatorControl_t：具体的算子
template <class AbstractCheckOperatorControl_t,  class _p,class ConcreteCheckOperatorControl_t>
class ConcreteCheckOperatorFactory :public AbstractCheckOperatorControlFactory<AbstractCheckOperatorControl_t,_p>
{
public:
    AbstractCheckOperatorControl_t* CreateCheckOperatorControl(_p p)
    {
        return new ConcreteCheckOperatorControl_t(p);
    }
};


#endif



