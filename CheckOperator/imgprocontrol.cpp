#include "imgprocontrol.h"
#include<QDateTime>
#include "glog/logging.h"
ImgProControl::ImgProControl(ImgPro* imgPro) :ClsImgPro(imgPro)
{

}



//************************************
// Method:    从XmlElement中加载类数据
// FullName:  ImgProControl::LoadXml
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: XMLElement * pEleParent
//************************************
int ImgProControl::LoadXml(XMLElement* pEleParent)
{
    this->ClsImgPro->ImgIndex = pEleParent->IntAttribute("Index");

    string temp = "";
    XmlHelper::GetElementTextFromParent("Valid", pEleParent, temp);
    this->ClsImgPro->Valid = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("IsCheck", pEleParent, temp);
    this->ClsImgPro->IsCheck = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("ExistRefImage", pEleParent, temp);
    this->ClsImgPro->ExistRefImage = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("DirNum", pEleParent, temp);
    this->ClsImgPro->DirNum = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("RefNum", pEleParent, temp);
    this->ClsImgPro->RefNum = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("CheckNum", pEleParent, temp);
    this->ClsImgPro->CheckNum = atoi(temp.c_str());
    XmlHelper::GetElementTextFromParent("WorkPiece", pEleParent, temp);
    this->ClsImgPro->WorkPiece = atoi(temp.c_str());

    vector<XMLElement*> vEle;

    XMLElement* eleTemp = pEleParent->FirstChildElement();
    while (eleTemp != NULL)
    {
        string s = eleTemp->Name();
        if (string(eleTemp->Name()) == "CheckNo")
        {
            vEle.push_back(eleTemp);
        }
        eleTemp = eleTemp->NextSiblingElement();
    }

    for (int idx = 0; idx < vEle.size(); idx++)
    {
        switch ((ENUMCHECKOPERATORTYPE)vEle[idx]->IntAttribute("Type"))
        {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {
            CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = new CheckOperatorTemplateMatch;
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorTemplateMatch*, CheckOperatorTemplateMatchControl> CheckOperatorTemplateMatchFactory;
            //            CheckOperatorControlBase* TemplateMatch = CheckOperatorTemplateMatchFactory.CreateCheckOperatorControl(pCheckOperatorTemplateMatch);
            CheckOperatorTemplateMatchControl control(pCheckOperatorTemplateMatch);
            control.LoadXml(vEle[idx]);
            // delete TemplateMatch;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorTemplateMatch);
        }
            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = new CheckOperatorEdgePositioning;
            CheckOperatorEdgePositioningControl control(pCheckOperatorEdgePositioning);
            control.LoadXml(vEle[idx]);

            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorEdgePositioning*, CheckOperatorEdgePositioningControl> CheckOperatorEdgePositioningFactory;
            //            CheckOperatorControlBase* EdgePositioning = CheckOperatorEdgePositioningFactory.CreateCheckOperatorControl(pCheckOperatorEdgePositioning);
            //  EdgePositioning->LoadXml(vEle[idx]);
            //delete EdgePositioning;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorEdgePositioning);
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        {
            CheckOperatorDefectInspect* pCheckOperatorDefectInspect = new CheckOperatorDefectInspect;
            CheckOperatorDefectInspectControl control(pCheckOperatorDefectInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
            //            DefectInspect->LoadXml(vEle[idx]);
            //            delete DefectInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
        }
            break;
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        {
            CheckOperatorCircleInspect* pCheckOperatorCircleInspect = new CheckOperatorCircleInspect;
            CheckOperatorCircleInspectControl control(pCheckOperatorCircleInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
            //            DefectInspect->LoadXml(vEle[idx]);
            //            delete DefectInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorCircleInspect);
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        {
            CheckOperatorDefectInspect2* pCheckOperatorDefectInspect = new CheckOperatorDefectInspect2;
            CheckOperatorDefectInspect2Control control(pCheckOperatorDefectInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
            //            DefectInspect->LoadXml(vEle[idx]);
            //            delete DefectInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
        }
            break;

        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        {
            CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2 = new CheckOperatorAssociatedInspect2;
            CheckOperatorAssociatedInspect2Control control(pCheckOperatorAssociatedInspect2);
            control.LoadXml(vEle[idx]);

            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect2);
        }
            break;


        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        {
            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = new CheckOperatorBrokenlineInspect;
            CheckOperatorBrokenlineInspectControl control(pCheckOperatorBrokenlineInspect);
            control.LoadXml(vEle[idx]);

            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorBrokenlineInspect*, CheckOperatorBrokenlineInspectControl> CheckOperatorBrokenlineInspectFactory;
            //            CheckOperatorControlBase* BrokenlineInspect = CheckOperatorBrokenlineInspectFactory.CreateCheckOperatorControl(pCheckOperatorBrokenlineInspect);
            //            BrokenlineInspect->LoadXml(vEle[idx]);
            //            delete BrokenlineInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
        }
            break;
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        {
            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement = new CheckOperatorGrayMeasurement;
            CheckOperatorGrayMeasurementControl control(pCheckOperatorGrayMeasurement);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorGrayMeasurement*, CheckOperatorGrayMeasurementControl> CheckOperatorGrayMeasurementFactory;
            //            CheckOperatorControlBase* GrayMeasurement = CheckOperatorGrayMeasurementFactory.CreateCheckOperatorControl(pCheckOperatorGrayMeasurement);
            //            GrayMeasurement->LoadXml(vEle[idx]);
            //            delete GrayMeasurement;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorGrayMeasurement);
        }
            break;
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        {
            CheckOperatorLineInspect* pCheckOperatorLineInspect = new CheckOperatorLineInspect;

            CheckOperatorLineInspectControl control(pCheckOperatorLineInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorLineInspect*, CheckOperatorLineInspectControl> CheckOperatorLineInspectFactory;
            //            CheckOperatorControlBase* LineInspect = CheckOperatorLineInspectFactory.CreateCheckOperatorControl(pCheckOperatorLineInspect);
            //            LineInspect->LoadXml(vEle[idx]);
            //            delete LineInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorLineInspect);
        }
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        {
            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = new CheckOperatorAssociatedInspect;
            CheckOperatorAssociatedInspectControl control(pCheckOperatorAssociatedInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorAssociatedInspect*, CheckOperatorAssociatedInspectControl> CheckOperatorAssociatedInspectFactory;
            //            CheckOperatorControlBase* AssociatedInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorAssociatedInspect);
            //            AssociatedInspect->LoadXml(vEle[idx]);
            //            delete AssociatedInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect);
        }
            break;
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        {
            CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
            CheckOperatorStayInspectControl control(pCheckOperatorStayInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorAssociatedInspectFactory;
            //            CheckOperatorControlBase* StayInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
            //            StayInspect->LoadXml(vEle[idx]);
            //            delete StayInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

            //                CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
            //                CheckOperatorStayInspectControl checkOperatorStayInspectControl(pCheckOperatorStayInspect);
            //                checkOperatorStayInspectControl.LoadXml(vEle[idx]);
            //                this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

        }
            break;
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        {
            CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = new CheckOperatorDoubleStayInspect;
            CheckOperatorDoubleStayInspectControl control(pCheckOperatorDoubleStayInspect);
            control.LoadXml(vEle[idx]);
            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorAssociatedInspectFactory;
            //            CheckOperatorControlBase* StayInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
            //            StayInspect->LoadXml(vEle[idx]);
            //            delete StayInspect;
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDoubleStayInspect);

            //                CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
            //                CheckOperatorStayInspectControl checkOperatorStayInspectControl(pCheckOperatorStayInspect);
            //                checkOperatorStayInspectControl.LoadXml(vEle[idx]);
            //                this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

        }
            break;

        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        {
            CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect = new CheckOperatorDLObjectDetect;
            CheckOperatorDLObjectDetectControl control(pCheckOperatorDLObjectDetect);
            control.LoadXml(vEle[idx]);
            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDLObjectDetect);
        }
            break;
        }
    }
    return 0;
}

//int ImgProControl::LoadXml2(QDomElement pEleParent)
//{
//    this->ClsImgPro->ImgIndex = pEleParent.attribute("Index").toInt();

//    QString temp = "";
//    XmlHelper::GetElementTextFromParent2("Valid", pEleParent, temp);
//    this->ClsImgPro->Valid = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("IsCheck", pEleParent, temp);
//    this->ClsImgPro->IsCheck = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("ExistRefImage", pEleParent, temp);
//    this->ClsImgPro->ExistRefImage = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("DirNum", pEleParent, temp);
//    this->ClsImgPro->DirNum = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("RefNum", pEleParent, temp);
//    this->ClsImgPro->RefNum = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("CheckNum", pEleParent, temp);
//    this->ClsImgPro->CheckNum = atoi(temp.toStdString().c_str());
//    XmlHelper::GetElementTextFromParent2("WorkPiece", pEleParent, temp);
//    this->ClsImgPro->WorkPiece = atoi(temp.toStdString().c_str());

//    vector<QDomElement> vEle;

//    QDomElement eleTemp = pEleParent.firstChildElement();
//    while (!eleTemp.isNull())
//    {
//        QString s = eleTemp.tagName();
//        if (eleTemp.tagName() == "CheckNo")
//        {
//            vEle.push_back(eleTemp);
//        }
//        eleTemp = eleTemp.nextSiblingElement();
//    }

//    for (int idx = 0; idx < vEle.size(); idx++)
//    {
//        switch ((ENUMCHECKOPERATORTYPE)vEle[idx].attribute("Type").toInt())
//        {
//        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
//        {
//            CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = new CheckOperatorTemplateMatch;
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorTemplateMatch*, CheckOperatorTemplateMatchControl> CheckOperatorTemplateMatchFactory;
//            //            CheckOperatorControlBase* TemplateMatch = CheckOperatorTemplateMatchFactory.CreateCheckOperatorControl(pCheckOperatorTemplateMatch);
//            CheckOperatorTemplateMatchControl control(pCheckOperatorTemplateMatch);
//            control.LoadXml2(vEle[idx]);
//            // delete TemplateMatch;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorTemplateMatch);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
//        {
//            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = new CheckOperatorEdgePositioning;
//            CheckOperatorEdgePositioningControl control(pCheckOperatorEdgePositioning);
//            control.LoadXml(vEle[idx]);

//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorEdgePositioning*, CheckOperatorEdgePositioningControl> CheckOperatorEdgePositioningFactory;
//            //            CheckOperatorControlBase* EdgePositioning = CheckOperatorEdgePositioningFactory.CreateCheckOperatorControl(pCheckOperatorEdgePositioning);
//            //  EdgePositioning->LoadXml(vEle[idx]);
//            //delete EdgePositioning;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorEdgePositioning);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
//        {
//            CheckOperatorDefectInspect* pCheckOperatorDefectInspect = new CheckOperatorDefectInspect;
//            CheckOperatorDefectInspectControl control(pCheckOperatorDefectInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
//            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
//            //            DefectInspect->LoadXml(vEle[idx]);
//            //            delete DefectInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
//        {
//            CheckOperatorCircleInspect* pCheckOperatorCircleInspect = new CheckOperatorCircleInspect;
//            CheckOperatorCircleInspectControl control(pCheckOperatorCircleInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
//            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
//            //            DefectInspect->LoadXml(vEle[idx]);
//            //            delete DefectInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorCircleInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
//        {
//            CheckOperatorDefectInspect2* pCheckOperatorDefectInspect = new CheckOperatorDefectInspect2;
//            CheckOperatorDefectInspect2Control control(pCheckOperatorDefectInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
//            //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
//            //            DefectInspect->LoadXml(vEle[idx]);
//            //            delete DefectInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
//        {
//            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = new CheckOperatorBrokenlineInspect;
//            CheckOperatorBrokenlineInspectControl control(pCheckOperatorBrokenlineInspect);
//            control.LoadXml(vEle[idx]);

//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorBrokenlineInspect*, CheckOperatorBrokenlineInspectControl> CheckOperatorBrokenlineInspectFactory;
//            //            CheckOperatorControlBase* BrokenlineInspect = CheckOperatorBrokenlineInspectFactory.CreateCheckOperatorControl(pCheckOperatorBrokenlineInspect);
//            //            BrokenlineInspect->LoadXml(vEle[idx]);
//            //            delete BrokenlineInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
//        {
//            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement = new CheckOperatorGrayMeasurement;
//            CheckOperatorGrayMeasurementControl control(pCheckOperatorGrayMeasurement);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorGrayMeasurement*, CheckOperatorGrayMeasurementControl> CheckOperatorGrayMeasurementFactory;
//            //            CheckOperatorControlBase* GrayMeasurement = CheckOperatorGrayMeasurementFactory.CreateCheckOperatorControl(pCheckOperatorGrayMeasurement);
//            //            GrayMeasurement->LoadXml(vEle[idx]);
//            //            delete GrayMeasurement;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorGrayMeasurement);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
//        {
//            CheckOperatorLineInspect* pCheckOperatorLineInspect = new CheckOperatorLineInspect;

//            CheckOperatorLineInspectControl control(pCheckOperatorLineInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorLineInspect*, CheckOperatorLineInspectControl> CheckOperatorLineInspectFactory;
//            //            CheckOperatorControlBase* LineInspect = CheckOperatorLineInspectFactory.CreateCheckOperatorControl(pCheckOperatorLineInspect);
//            //            LineInspect->LoadXml(vEle[idx]);
//            //            delete LineInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorLineInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
//        {
//            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = new CheckOperatorAssociatedInspect;
//            CheckOperatorAssociatedInspectControl control(pCheckOperatorAssociatedInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorAssociatedInspect*, CheckOperatorAssociatedInspectControl> CheckOperatorAssociatedInspectFactory;
//            //            CheckOperatorControlBase* AssociatedInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorAssociatedInspect);
//            //            AssociatedInspect->LoadXml(vEle[idx]);
//            //            delete AssociatedInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect);
//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
//        {
//            CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
//            CheckOperatorStayInspectControl control(pCheckOperatorStayInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorAssociatedInspectFactory;
//            //            CheckOperatorControlBase* StayInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
//            //            StayInspect->LoadXml(vEle[idx]);
//            //            delete StayInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

//            //                CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
//            //                CheckOperatorStayInspectControl checkOperatorStayInspectControl(pCheckOperatorStayInspect);
//            //                checkOperatorStayInspectControl.LoadXml(vEle[idx]);
//            //                this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
//        {
//            CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = new CheckOperatorDoubleStayInspect;
//            CheckOperatorDoubleStayInspectControl control(pCheckOperatorDoubleStayInspect);
//            control.LoadXml(vEle[idx]);
//            //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorAssociatedInspectFactory;
//            //            CheckOperatorControlBase* StayInspect = CheckOperatorAssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
//            //            StayInspect->LoadXml(vEle[idx]);
//            //            delete StayInspect;
//            this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDoubleStayInspect);

//            //                CheckOperatorStayInspect* pCheckOperatorStayInspect = new CheckOperatorStayInspect;
//            //                CheckOperatorStayInspectControl checkOperatorStayInspectControl(pCheckOperatorStayInspect);
//            //                checkOperatorStayInspectControl.LoadXml(vEle[idx]);
//            //                this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);

//        }
//            break;
//        }
//    }
//    return 0;
//}




XMLElement* ImgProControl::generateInitXmlElementBlock(TinyXmlDocument& doc,int index)
{
    XMLElement* eleImgPro = doc.NewElement("ImgPro");
    eleImgPro->SetAttribute("Index", index);

    XMLElement* eleValid = doc.NewElement("Valid");
    eleValid->SetText(1);
    XMLElement* eleIsCheck = doc.NewElement("IsCheck");
    eleIsCheck->SetText(1);
    XMLElement* eleExistRefImage = doc.NewElement("ExistRefImage");
    eleExistRefImage->SetText(0);
    XMLElement* eleDirNum = doc.NewElement("DirNum");
    eleDirNum->SetText(0);
    XMLElement* eleRefNum = doc.NewElement("RefNum");
    eleRefNum->SetText(0);
    XMLElement* eleCheckNum = doc.NewElement("CheckNum");
    eleCheckNum->SetText(0);
    XMLElement* eleWorkPiece = doc.NewElement("WorkPiece");
    eleWorkPiece->SetText(0);

    eleImgPro->InsertEndChild(eleValid);
    eleImgPro->InsertEndChild(eleIsCheck);
    eleImgPro->InsertEndChild(eleExistRefImage);
    eleImgPro->InsertEndChild(eleDirNum);
    eleImgPro->InsertEndChild(eleRefNum);
    eleImgPro->InsertEndChild(eleCheckNum);
    eleImgPro->InsertEndChild(eleWorkPiece);

    return  eleImgPro;
}

QDomElement* ImgProControl::generateInitXmlElementBlock2(QDomDocument &qdoc, int index)
{

    QDomElement TempEle = qdoc.createElement("ImgPro");
    QDomElement *eleImgPro=new QDomElement(TempEle);
    eleImgPro->setAttribute("Index", index);

    QDomElement eleValid=qdoc.createElement("Valid");
    QDomText eleTextValid=qdoc.createTextNode("1");
    eleValid.appendChild(eleTextValid);

    QDomElement eleIsCheck=qdoc.createElement("IsCheck");
    QDomText eleTextIsCheck=qdoc.createTextNode("1");
    eleIsCheck.appendChild(eleTextIsCheck);

    QDomElement eleExistRefImage=qdoc.createElement("ExistRefImage");
    QDomText eleTextExistRefImage=qdoc.createTextNode("0");
    eleExistRefImage.appendChild(eleTextExistRefImage);

    QDomElement eleDirNum=qdoc.createElement("DirNum");
    QDomText eleTextDirNum=qdoc.createTextNode("0");
    eleDirNum.appendChild(eleTextDirNum);

    QDomElement eleRefNum=qdoc.createElement("RefNum");
    QDomText eleTextRefNum=qdoc.createTextNode("0");
    eleRefNum.appendChild(eleTextRefNum);

    QDomElement eleCheckNum=qdoc.createElement("CheckNum");
    QDomText eleTextCheckNum=qdoc.createTextNode("0");
    eleCheckNum.appendChild(eleTextCheckNum);

    QDomElement eleWorkPiece=qdoc.createElement("WorkPiece");
    QDomText eleTextWorkPiece=qdoc.createTextNode("0");
    eleWorkPiece.appendChild(eleTextWorkPiece);


    eleImgPro->appendChild(eleValid);
    eleImgPro->appendChild(eleIsCheck);
    eleImgPro->appendChild(eleExistRefImage);
    eleImgPro->appendChild(eleDirNum);
    eleImgPro->appendChild(eleRefNum);
    eleImgPro->appendChild(eleCheckNum);
    eleImgPro->appendChild(eleWorkPiece);

    return  eleImgPro;



}


//************************************
// Method:    根据数据类生成xmlElement结构
// FullName:  ImgProControl::generateXmlElementBlock
// Access:    virtual public
// Returns:   tinyxml2::XMLElement *
// Qualifier:
// Parameter: XMLDocument & doc
//************************************
XMLElement * ImgProControl::generateXmlElementBlock(TinyXmlDocument & doc)
{
    XMLElement* eleImgPro = doc.NewElement("ImgPro");
    eleImgPro->SetAttribute("Index", this->ClsImgPro->ImgIndex);

    XMLElement* eleValid = doc.NewElement("Valid");
    eleValid->SetText(this->ClsImgPro->Valid ? 1 : 0);
    XMLElement* eleIsCheck = doc.NewElement("IsCheck");
    eleIsCheck->SetText(this->ClsImgPro->IsCheck ? 1 : 0);
    XMLElement* eleExistRefImage = doc.NewElement("ExistRefImage");
    eleExistRefImage->SetText(this->ClsImgPro->ExistRefImage ? 1 : 0);
    XMLElement* eleDirNum = doc.NewElement("DirNum");
    eleDirNum->SetText(this->ClsImgPro->DirNum);
    XMLElement* eleRefNum = doc.NewElement("RefNum");
    eleRefNum->SetText(this->ClsImgPro->RefNum);
    XMLElement* eleCheckNum = doc.NewElement("CheckNum");
    eleCheckNum->SetText(this->ClsImgPro->CheckNum);
    XMLElement* eleWorkPiece = doc.NewElement("WorkPiece");
    eleWorkPiece->SetText(this->ClsImgPro->WorkPiece);

    eleImgPro->InsertEndChild(eleValid);
    eleImgPro->InsertEndChild(eleIsCheck);
    eleImgPro->InsertEndChild(eleExistRefImage);
    eleImgPro->InsertEndChild(eleDirNum);
    eleImgPro->InsertEndChild(eleRefNum);
    eleImgPro->InsertEndChild(eleCheckNum);
    eleImgPro->InsertEndChild(eleWorkPiece);

    for (list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin(); itor != this->ClsImgPro->LstCheckOperator.end(); itor++)
    {
        CheckOperatorBasic* pCheckOperatorBasic = *itor;
        switch ((*itor)->CheckType)
        {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {
            {
                CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>(pCheckOperatorBasic);
                ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorTemplateMatch*, CheckOperatorTemplateMatchControl> CheckOperatorTemplateMatchFactory;
                CheckOperatorControlBase* TemplateMatch = CheckOperatorTemplateMatchFactory.CreateCheckOperatorControl(pCheckOperatorTemplateMatch);
                XMLElement* ele = TemplateMatch->generateXmlElementBlock(doc);
                delete TemplateMatch;
                eleImgPro->InsertEndChild(ele);
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorEdgePositioning*, CheckOperatorEdgePositioningControl> CheckOperatorEdgePositioningFactory;
            CheckOperatorControlBase* EdgePositioning = CheckOperatorEdgePositioningFactory.CreateCheckOperatorControl(pCheckOperatorEdgePositioning);
            XMLElement* ele = EdgePositioning->generateXmlElementBlock(doc);
            delete EdgePositioning;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        {
            CheckOperatorDefectInspect* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
            XMLElement* ele = DefectInspect->generateXmlElementBlock(doc);
            delete DefectInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        {
            CheckOperatorCircleInspect* pCheckOperatorCircleInspect = dynamic_cast<CheckOperatorCircleInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorCircleInspect*, CheckOperatorCircleInspectControl> CheckOperatorCircleInspectFactory;
            CheckOperatorControlBase* DefectInspect = CheckOperatorCircleInspectFactory.CreateCheckOperatorControl(pCheckOperatorCircleInspect);
            XMLElement* ele = DefectInspect->generateXmlElementBlock(doc);
            delete DefectInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        {

            CheckOperatorDefectInspect2* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect2*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect2*, CheckOperatorDefectInspect2Control> CheckOperatorDefectInspectFactory;
            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
            XMLElement* ele = DefectInspect->generateXmlElementBlock(doc);
            delete DefectInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;

        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        {

            CheckOperatorAssociatedInspect2* pCheckOperatorAssociatedInspect2 = dynamic_cast<CheckOperatorAssociatedInspect2*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorAssociatedInspect2*, CheckOperatorAssociatedInspect2Control> CheckOperatorDefectInspectFactory;
            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorAssociatedInspect2);
            XMLElement* ele = DefectInspect->generateXmlElementBlock(doc);
            delete DefectInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;


        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        {
            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorBrokenlineInspect*, CheckOperatorBrokenlineInspectControl> CheckOperatorBrokenlineInspectFactory;
            CheckOperatorControlBase* BrokenlineInspect = CheckOperatorBrokenlineInspectFactory.CreateCheckOperatorControl(pCheckOperatorBrokenlineInspect);
            XMLElement* ele = BrokenlineInspect->generateXmlElementBlock(doc);
            delete BrokenlineInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        {
            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement = dynamic_cast<CheckOperatorGrayMeasurement*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorGrayMeasurement*, CheckOperatorGrayMeasurementControl> CheckOperatorGrayMeasurementFactory;
            CheckOperatorControlBase* GrayMeasurement = CheckOperatorGrayMeasurementFactory.CreateCheckOperatorControl(pCheckOperatorGrayMeasurement);
            XMLElement* ele = GrayMeasurement->generateXmlElementBlock(doc);
            delete GrayMeasurement;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        {
            CheckOperatorLineInspect* pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorLineInspect*, CheckOperatorLineInspectControl> CheckOperatorLineInspectFactory;
            CheckOperatorControlBase* LineInspect = CheckOperatorLineInspectFactory.CreateCheckOperatorControl(pCheckOperatorLineInspect);
            XMLElement* ele = LineInspect->generateXmlElementBlock(doc);
            delete LineInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        {
            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorAssociatedInspect*, CheckOperatorAssociatedInspectControl> AssociatedInspectFactory;
            CheckOperatorControlBase* AssociatedInspect = AssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorAssociatedInspect);
            XMLElement* ele = AssociatedInspect->generateXmlElementBlock(doc);
            delete AssociatedInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
        {
            CheckOperatorStayInspect* pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorStayInspectFactory;
            CheckOperatorControlBase* StayInspect = CheckOperatorStayInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
            XMLElement* ele = StayInspect->generateXmlElementBlock(doc);
            delete StayInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
        {
            CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = dynamic_cast<CheckOperatorDoubleStayInspect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDoubleStayInspect*, CheckOperatorDoubleStayInspectControl> CheckOperatorDoubleStayInspectFactory;
            CheckOperatorControlBase* DoubleStayInspect = CheckOperatorDoubleStayInspectFactory.CreateCheckOperatorControl(pCheckOperatorDoubleStayInspect);
            XMLElement* ele = DoubleStayInspect->generateXmlElementBlock(doc);
            delete DoubleStayInspect;
            eleImgPro->InsertEndChild(ele);
        }
            break;

        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        {
            CheckOperatorDLObjectDetect * pCheckOperatorDLObjectDetect = dynamic_cast<CheckOperatorDLObjectDetect*>(pCheckOperatorBasic);
            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDLObjectDetect*,CheckOperatorDLObjectDetectControl> CheckOperatorDLObjectDetectFactory;
            CheckOperatorControlBase* DLObjectDetect = CheckOperatorDLObjectDetectFactory.CreateCheckOperatorControl(pCheckOperatorDLObjectDetect);
            XMLElement* ele = DLObjectDetect->generateXmlElementBlock(doc);
            delete DLObjectDetect;
            eleImgPro->InsertEndChild(ele);
        }
            break;

        }

    }
    return eleImgPro;
}




//************************************
// Method:    获取算子数量
// FullName:  ImgProControl::getCheckOperatorNum
// Access:    public
// Returns:   int
// Qualifier:
//************************************
int ImgProControl::getCheckOperatorNum()
{
    if(this->ClsImgPro!=nullptr) return this->ClsImgPro->CheckNum;
    return 0;
}


//修改子节点内容
void ImgProControl::updateImgProNode(XMLElement * eleParent,string eleName,string text)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(text.c_str());
}
void ImgProControl::updateImgProNode(XMLElement * eleParent,string eleName, int inttext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(inttext).c_str());
}
void ImgProControl::updateImgProNode(XMLElement * eleParent, string eleName, bool btext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(btext?"1":"0");
}
void ImgProControl::updateImgProNode(XMLElement * eleParent, string eleName, double dtext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(dtext).c_str());
}
void ImgProControl::updateImgProNode(XMLElement * eleParent, string eleName, float ftext)
{
    eleParent->FirstChildElement(eleName.c_str())->SetText(to_string(ftext).c_str());
}


//************************************
// Method:    获取第index个算子
// FullName:  ImgProControl::getCheckNo
// Access:    public
// Returns:   CheckOperatorBasic*
// Qualifier:
// Parameter: int index
//************************************
CheckOperatorBasic* ImgProControl::getCheckNo(int index)
{
    list<CheckOperatorBasic*>::iterator reit;
    int idx = 1;
    for (list<CheckOperatorBasic*>::iterator it = this->ClsImgPro->LstCheckOperator.begin(); it != this->ClsImgPro->LstCheckOperator.end(); it++, idx++)
    {
        if (idx == index)
        {
            reit = it;
        }
    }
    return *reit;
}


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
void ImgProControl::insertXMLCheckOperator(XMLElement * EleParent,CheckOperatorBasic* checkOperator, ENUMCHECKOPERATORTYPE type, TinyXmlDocument& doc)
{
    //#if 0//可以跳过
    //        //xml文件添加
    //        switch (type)
    //        {
    //        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    //        {
    //            CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorTemplateMatch*, CheckOperatorTemplateMatchControl> CheckOperatorTemplateMatchFactory;
    //            CheckOperatorControlBase* TemplateMatch = CheckOperatorTemplateMatchFactory.CreateCheckOperatorControl(pCheckOperatorTemplateMatch);
    //            XMLElement* ele = TemplateMatch->generateXmlElementBlock(doc);
    //            delete TemplateMatch;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    //        {
    //            CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorEdgePositioning*, CheckOperatorEdgePositioningControl> CheckOperatorEdgePositioningFactory;
    //            CheckOperatorControlBase* EdgePositioning = CheckOperatorEdgePositioningFactory.CreateCheckOperatorControl(pCheckOperatorEdgePositioning);
    //            XMLElement* ele = EdgePositioning->generateXmlElementBlock(doc);
    //            delete EdgePositioning;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
    //        {
    //            CheckOperatorDefectInspect* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorDefectInspect*, CheckOperatorDefectInspectControl> CheckOperatorDefectInspectFactory;
    //            CheckOperatorControlBase* DefectInspect = CheckOperatorDefectInspectFactory.CreateCheckOperatorControl(pCheckOperatorDefectInspect);
    //            XMLElement* ele = DefectInspect->generateXmlElementBlock(doc);
    //            delete DefectInspect;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
    //        {
    //            CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorBrokenlineInspect*, CheckOperatorBrokenlineInspectControl> CheckOperatorBrokenlineInspectFactory;
    //            CheckOperatorControlBase* BrokenlineInspect = CheckOperatorBrokenlineInspectFactory.CreateCheckOperatorControl(pCheckOperatorBrokenlineInspect);
    //            XMLElement* ele = BrokenlineInspect->generateXmlElementBlock(doc);
    //            delete BrokenlineInspect;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
    //        {
    //            CheckOperatorGrayMeasurement* pCheckOperatorGrayMeasurement = dynamic_cast<CheckOperatorGrayMeasurement*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorGrayMeasurement*, CheckOperatorGrayMeasurementControl> CheckOperatorGrayMeasurementFactory;
    //            CheckOperatorControlBase* GrayMeasurement = CheckOperatorGrayMeasurementFactory.CreateCheckOperatorControl(pCheckOperatorGrayMeasurement);
    //            XMLElement* ele = GrayMeasurement->generateXmlElementBlock(doc);
    //            delete GrayMeasurement;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_LINEINSPECT:
    //        {
    //            CheckOperatorLineInspect* pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorLineInspect*, CheckOperatorLineInspectControl> CheckOperatorLineInspectFactory;
    //            CheckOperatorControlBase* LineInspect = CheckOperatorLineInspectFactory.CreateCheckOperatorControl(pCheckOperatorLineInspect);
    //            XMLElement* ele = LineInspect->generateXmlElementBlock(doc);
    //            delete LineInspect;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
    //        {
    //            CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorAssociatedInspect*, CheckOperatorAssociatedInspectControl> AssociatedInspectFactory;
    //            CheckOperatorControlBase* AssociatedInspect = AssociatedInspectFactory.CreateCheckOperatorControl(pCheckOperatorAssociatedInspect);
    //            XMLElement* ele = AssociatedInspect->generateXmlElementBlock(doc);
    //            delete AssociatedInspect;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        case ENUMCHECKOPERATORTYPE_STAYINSPECT:
    //        {
    //            CheckOperatorStayInspect* pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>(checkOperator);
    //            ConcreteCheckOperatorFactory<CheckOperatorControlBase, CheckOperatorStayInspect*, CheckOperatorStayInspectControl> CheckOperatorStayInspectFactory;
    //            CheckOperatorControlBase* StayInspect = CheckOperatorStayInspectFactory.CreateCheckOperatorControl(pCheckOperatorStayInspect);
    //            XMLElement* ele = StayInspect->generateXmlElementBlock(doc);
    //            delete StayInspect;
    //            EleParent->InsertEndChild(ele);
    //        }
    //            break;
    //        }
    //#endif // 0//可以跳过

    //内存添加
    switch (type)
    {
    case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    {
        CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorTemplateMatch);
    }
        break;
    case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    {
        CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorEdgePositioning);
    }
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
    {
        CheckOperatorDefectInspect* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
    {
        CheckOperatorCircleInspect* pCheckOperatorCircleInspect = dynamic_cast<CheckOperatorCircleInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorCircleInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
    {
        CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
    {
        CheckOperatorGrayMeasurement* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorGrayMeasurement*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_LINEINSPECT:
    {
        CheckOperatorLineInspect* pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorLineInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
    {
        CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_STAYINSPECT:
    {
        CheckOperatorStayInspect* pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorStayInspect);
    }
        break;
    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
    {
        CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = dynamic_cast<CheckOperatorDoubleStayInspect*>(checkOperator);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperatorDoubleStayInspect);
    }
        break;
    }

    this->ClsImgPro->CheckNum++;
    EleParent->FirstChildElement("CheckNum")->SetText(this->ClsImgPro->CheckNum);
    checkOperator->stu_CheckOperatorBasic->Index = this->ClsImgPro->CheckNum;
}


CheckOperatorBasic* ImgProControl::insertCheckOperator(ENUMCHECKOPERATORTYPE type)
{

    switch (type)
    {
    case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    {

        CheckOperatorTemplateMatch* pCheckOperator=new CheckOperatorTemplateMatch(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").toStdString());
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_TEMPLATEMATCH;

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=0;i<this->ClsImgPro->RefNum;i++,itor++){}
        this->ClsImgPro->LstCheckOperator.insert(itor,pCheckOperator);
        this->ClsImgPro->RefNum++;
        this->ClsImgPro->ExistRefImage=1;

        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }



        //更新位置
        list<CheckOperatorBasic*>::iterator it=  this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType!=ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) continue;
            CheckOperatorAssociatedInspect* pTmp=static_cast<CheckOperatorAssociatedInspect*>((*it));
            if(pTmp->Relaty1!=0) pTmp->Relaty1++;
            if(pTmp->Relaty2!=0) pTmp->Relaty2++;
        }
        return pCheckOperator;
    }
        break;
    case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    {

        CheckOperatorEdgePositioning* pCheckOperator=new CheckOperatorEdgePositioning;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_EDGEPOSITIONING;

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=0;i<this->ClsImgPro->RefNum;i++,itor++){}
        this->ClsImgPro->LstCheckOperator.insert(itor,pCheckOperator);
        this->ClsImgPro->RefNum++;
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }

        //更新位置
        list<CheckOperatorBasic*>::iterator it=this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType!=ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) continue;
            CheckOperatorAssociatedInspect* pTmp=static_cast<CheckOperatorAssociatedInspect*>((*it));
            if(pTmp->Relaty1!=0) pTmp->Relaty1++;
            if(pTmp->Relaty2!=0) pTmp->Relaty2++;
        }
        return pCheckOperator;
    }
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
    {

        CheckOperatorDefectInspect* pCheckOperator=new CheckOperatorDefectInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_DEFECTINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }


        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);


        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }
    case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
    {

        CheckOperatorDLObjectDetect* pCheckOperator=new CheckOperatorDLObjectDetect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_DLOBJECTDETECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }


        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);


        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }

    case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
    {

        CheckOperatorCircleInspect* pCheckOperator=new CheckOperatorCircleInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_CIRCLEINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }


        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);


        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
    {
        CheckOperatorDefectInspect2* pCheckOperator=new CheckOperatorDefectInspect2;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->X1=this->m_InitWidth/2;pCheckOperator->Y1=0;
        pCheckOperator->X2=(double)this->m_InitWidth*7/8;pCheckOperator->Y2=this->m_InitHeight*1/8;
        pCheckOperator->X3=this->m_InitWidth;pCheckOperator->Y3=this->m_InitHeight/2;
        pCheckOperator->X4=this->m_InitWidth*7/8;pCheckOperator->Y4=this->m_InitHeight*7/8;

        pCheckOperator->X5=this->m_InitWidth/2;pCheckOperator->Y5=this->m_InitHeight;
        pCheckOperator->X6=this->m_InitWidth*1/8;pCheckOperator->Y6=this->m_InitHeight*7/8;
        pCheckOperator->X7=0;pCheckOperator->Y7=this->m_InitHeight/2;
        pCheckOperator->X8=this->m_InitWidth*1/8;pCheckOperator->Y8=this->m_InitHeight*1/8;

        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_DEFECTINSPECT2;
        this->ClsImgPro->CheckNum++;
        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }

        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);


        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }

        break;

    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
    {
        CheckOperatorAssociatedInspect2* pCheckOperator=new CheckOperatorAssociatedInspect2;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->X1=this->m_InitWidth/2;pCheckOperator->Y1=0;
        pCheckOperator->X2=(double)this->m_InitWidth*7/8;pCheckOperator->Y2=this->m_InitHeight*1/8;
        pCheckOperator->X3=this->m_InitWidth;pCheckOperator->Y3=this->m_InitHeight/2;
        pCheckOperator->X4=this->m_InitWidth*7/8;pCheckOperator->Y4=this->m_InitHeight*7/8;

        pCheckOperator->X5=this->m_InitWidth/2;pCheckOperator->Y5=this->m_InitHeight;
        pCheckOperator->X6=this->m_InitWidth*1/8;pCheckOperator->Y6=this->m_InitHeight*7/8;
        pCheckOperator->X7=0;pCheckOperator->Y7=this->m_InitHeight/2;
        pCheckOperator->X8=this->m_InitWidth*1/8;pCheckOperator->Y8=this->m_InitHeight*1/8;

        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2;
        this->ClsImgPro->CheckNum++;
        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }

        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);


        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }

        break;
    case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
    {
        CheckOperatorBrokenlineInspect* pCheckOperator=new CheckOperatorBrokenlineInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }


        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;

    }
        break;
    case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
    {
        CheckOperatorGrayMeasurement* pCheckOperator=new CheckOperatorGrayMeasurement;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT;

        this->ClsImgPro->CheckNum++;
        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }
        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }
        break;
    case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
    {
        CheckOperatorAssociatedInspect* pCheckOperator=new CheckOperatorAssociatedInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=100;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=100;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }
        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }

        break;
    case ENUMCHECKOPERATORTYPE_STAYINSPECT:
    {
        CheckOperatorStayInspect* pCheckOperator=new CheckOperatorStayInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_STAYINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }
        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }
        break;

    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
    {
        CheckOperatorDoubleStayInspect* pCheckOperator=new CheckOperatorDoubleStayInspect;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xs=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ys=0;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Xe=this->m_InitWidth;
        pCheckOperator->stu_CheckOperatorBasic->Basic_RectValid_Ye=this->m_InitHeight;
        pCheckOperator->CheckType=ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT;

        this->ClsImgPro->CheckNum++;

        //  this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();
        for(;it!=this->ClsImgPro->LstCheckOperator.end();++it)
        {
            if((*it)->CheckType==ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) break;
        }
        this->ClsImgPro->LstCheckOperator.insert(it,pCheckOperator);

        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
        return pCheckOperator;
    }
        break;
    }



}


void ImgProControl::insertCheckOperator(CheckOperatorBasic* pCheckOperatorBasic)
{
    switch (pCheckOperatorBasic->CheckType)
    {
    case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
    {
        pCheckOperatorBasic->stu_CheckOperatorBasic->Index=this->ClsImgPro->RefNum+1;
        CheckOperatorTemplateMatch* pCheckOperator=static_cast<CheckOperatorTemplateMatch*>(pCheckOperatorBasic);
        // this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=0;i<this->ClsImgPro->RefNum;i++,itor++){}
        this->ClsImgPro->LstCheckOperator.insert(itor,pCheckOperator);
        this->ClsImgPro->RefNum++;
        this->ClsImgPro->ExistRefImage=1;

        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
    {

        CheckOperatorEdgePositioning* pCheckOperator=static_cast<CheckOperatorEdgePositioning*>(pCheckOperatorBasic);
        //this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=0;i<this->ClsImgPro->RefNum;i++,itor++){}
        this->ClsImgPro->LstCheckOperator.insert(itor,pCheckOperator);
        this->ClsImgPro->RefNum++;
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
    {
        this->ClsImgPro->CheckNum++;
        CheckOperatorDefectInspect* pCheckOperator=static_cast<CheckOperatorDefectInspect*>(pCheckOperatorBasic);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
    {
        this->ClsImgPro->CheckNum++;
        CheckOperatorCircleInspect* pCheckOperator=static_cast<CheckOperatorCircleInspect*>(pCheckOperatorBasic);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        break;
    case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
    {
        this->ClsImgPro->CheckNum++;
        CheckOperatorGrayMeasurement* pCheckOperator=static_cast<CheckOperatorGrayMeasurement*>(pCheckOperatorBasic);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_LINEINSPECT:
        break;
    case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:


        break;
    case ENUMCHECKOPERATORTYPE_STAYINSPECT:
    {
        this->ClsImgPro->CheckNum++;
        CheckOperatorStayInspect* pCheckOperator=static_cast<CheckOperatorStayInspect*>(pCheckOperatorBasic);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
    {
        this->ClsImgPro->CheckNum++;
        CheckOperatorDoubleStayInspect* pCheckOperator=static_cast<CheckOperatorDoubleStayInspect*>(pCheckOperatorBasic);
        this->ClsImgPro->LstCheckOperator.push_back(pCheckOperator);
        list<CheckOperatorBasic*>::iterator itor=this->ClsImgPro->LstCheckOperator.begin();
        itor=this->ClsImgPro->LstCheckOperator.begin();
        for(int i=1;itor!=this->ClsImgPro->LstCheckOperator.end();i++,itor++)
        {
            (*itor)->stu_CheckOperatorBasic->Index=i;
        }
    }
        break;
    }
}

//************************************
// Method:    删除对应编号的算子
// FullName:  ImgProControl::deleteCheckOperator
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: int index
//************************************
int ImgProControl::deleteCheckOperator(int index)
{


    ENUMCHECKOPERATORTYPE type=this->getCheckNo(index)->CheckType;
    for (list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();it!=this->ClsImgPro->LstCheckOperator.end();it++)
    {
        if((*it)->stu_CheckOperatorBasic->Index==index){
            switch ((*it)->CheckType) {
            case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
                this->ClsImgPro->RefNum--;
                break;
            case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
            case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
            case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
            case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
            case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
            case ENUMCHECKOPERATORTYPE_LINEINSPECT:
            case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
            case ENUMCHECKOPERATORTYPE_STAYINSPECT:
            case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
            case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
                this->ClsImgPro->CheckNum--;
                break;
            }
        }
    }

    this->ClsImgPro->LstCheckOperator.remove_if([index](CheckOperatorBasic* co) {return co->stu_CheckOperatorBasic->Index == index;});
    int newIndex = 1;
    bool isExistRefImage=false;
    for (list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();it!=this->ClsImgPro->LstCheckOperator.end();it++,newIndex++)
    {

        //删除参考位置
        StuCheckOperatorBasicControl control((*it)->stu_CheckOperatorBasic);
        //        switch (type)
        //        {
        //        case  ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        //            control.delRefX(index);
        //            control.delRefY(index);
        //            break;
        //        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        //            control.delRefX(index);
        //            control.delRefY(index);
        //            break;
        //        }




        control.delRefX(index);
        std::vector<int> vStr;
        control.getRefX(vStr);

        std::vector<int> vStrAdd;
        for(int idx_i=0;idx_i<vStr.size();++idx_i)
        {
            if(vStr[idx_i]>index)
            {
                vStrAdd.push_back(vStr[idx_i]-1);
            }
            else
            {
                vStrAdd.push_back(vStr[idx_i]);
            }
        }
        for(int idx_i=0;idx_i<vStr.size();++idx_i)
        {
            control.delRefX(vStr[idx_i]);
        }
        for(int idx_i=0;idx_i<vStrAdd.size();++idx_i)
        {
            control.addRefX(vStrAdd[idx_i]);
        }


        control.delRefY(index);
        vStr.clear();
        control.getRefY(vStr);
        vStrAdd.clear();
        for(int idx_i=0;idx_i<vStr.size();++idx_i)
        {
            if(vStr[idx_i]>index)
            {
                vStrAdd.push_back(vStr[idx_i]-1);
            }
            else
            {
                vStrAdd.push_back(vStr[idx_i]);
            }
        }
        for(int idx_i=0;idx_i<vStr.size();++idx_i)
        {
            control.delRefY(vStr[idx_i]);
        }
        for(int idx_i=0;idx_i<vStrAdd.size();++idx_i)
        {
            control.addRefY(vStrAdd[idx_i]);
        }



        if((*it)->CheckType==ENUMCHECKOPERATORTYPE_TEMPLATEMATCH){
            isExistRefImage=true;
        }
        (*it)->stu_CheckOperatorBasic->Index= newIndex;
    }
    this->ClsImgPro->ExistRefImage=isExistRefImage;


    //删除了算子，那么需要更新关联的算子编号
    for (list<CheckOperatorBasic*>::iterator it= this->ClsImgPro->LstCheckOperator.begin();it!=this->ClsImgPro->LstCheckOperator.end();it++,newIndex++)
    {
        if((*it)->CheckType!=ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT) continue;

        CheckOperatorAssociatedInspect* pTmp=static_cast<CheckOperatorAssociatedInspect*>((*it));

        if(pTmp->Relaty1==index)
        {
            pTmp->Relaty1=0;
        }
        else if(pTmp->Relaty1>index)
        {
            pTmp->Relaty1--;
        }
        else
        {

        }
        if(pTmp->Relaty2==index)
        {
            pTmp->Relaty2=0;
        }
        else if(pTmp->Relaty2>index)
        {
            pTmp->Relaty2--;
        }
        else
        {

        }
    }
    return 0;
}

void ImgProControl::setRefImage(OpencvImage& openCvImage)
{
    //    cv::Mat m = openCvImage.ImageGray;

    this->ClsImgPro->RefOpencvImage = openCvImage;
}

void ImgProControl::setCalcImage(OpencvImage& openCvImage)
{
    //    cv::Mat m = openCvImage.ImageGray;
    this->ClsImgPro->CalcOpencvImage = openCvImage;
}

void ImgProControl::getCalcImage(OpencvImage &openCvImage)
{
    openCvImage=this->ClsImgPro->CalcOpencvImage;
}

void ImgProControl::getCurBadImage(OpencvImage &openCvImage)
{

    //    openCvImage=this->ClsImgPro->CurBadOpencvImage;

    this->ClsImgPro->CurBadOpencvImage.ImageRGB.copyTo(openCvImage.ImageRGB);
}



int ImgProControl::calculateCV(unsigned char errArr[])
{

    if(!this->ClsImgPro->Valid) return 0;
    int ret = 0;
    //int tmpCameraNo=this->ClsImgPro->ImgIndex;

#ifdef IMG_TYPE_RGB
    cv::Mat ref = this->ClsImgPro->RefOpencvImage.ImageRGB;
    cv::Mat cur = this->ClsImgPro->CalcOpencvImage.ImageRGB;
    cv::cvtColor(ref,ref,cv::COLOR_BGR2GRAY);
    cv::cvtColor(cur,cur,cv::COLOR_BGR2GRAY);

#endif
#ifdef IMG_TYPE_GRAY
    cv::Mat ref = this->ClsImgPro->RefOpencvImage.ImageGray;
    cv::Mat cur = this->ClsImgPro->CalcOpencvImage.ImageGray;
#endif
    this->ClsImgPro->MapPostion.clear();


    //先进行关联检测
    //不需要关联检测可以注销
    int OpAssociatedRet=1;

    //检测精度类型
    int accuracyType=0;

    {
        cv::Mat curTmp;
        int maxArea=0;
        int errIndex = 1;
        int maxIndex=1;

        list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin();
        for (;itor!=this->ClsImgPro->LstCheckOperator.end();itor++,errIndex++)
        {
            if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
            {
                errArr[errIndex]=2;
                continue;
            }

            if((*itor)->CheckType==ENUMCHECKOPERATORTYPE::ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2)
            {
                cv::Rect rectInit(
                            (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                            (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                            (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                            (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys
                            );

                cv::Rect rectAdjust(rectInit.x,rectInit.y,rectInit.width,rectInit.height);
                cv::Mat refTmp=cv::Mat(ref,rectInit);
                StuCheckOperatorBasicControl control((*itor)->stu_CheckOperatorBasic);
                //choose best basis x and y
                std::vector<int> vRefx;
                std::vector<int> vRefy;
                control.getRefX(vRefx);
                control.getRefY(vRefy);
                cv::Point bestPt(0,0);
                int bestScore=0;
                for(int idx_x=0;idx_x<vRefx.size();idx_x++)
                {

                    if(this->ClsImgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                    {
                        bestScore=this->ClsImgPro->MapPostion[vRefx[idx_x]].Score;
                        bestPt.x=this->ClsImgPro->MapPostion[vRefx[idx_x]].Pt.x;
                    }
                }
                bestScore=0;
                for(int idx_y=0;idx_y<vRefy.size();idx_y++)
                {

                    if(this->ClsImgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                    {
                        bestScore=this->ClsImgPro->MapPostion[vRefy[idx_y]].Score;
                        bestPt.y=this->ClsImgPro->MapPostion[vRefy[idx_y]].Pt.y;
                    }
                }


                rectAdjust.x=rectAdjust.x+bestPt.x;
                rectAdjust.y=rectAdjust.y+bestPt.y;
                if(rectAdjust.x<0||rectAdjust.y<0||(rectAdjust.x+rectAdjust.width)>=640||(rectAdjust.y+rectAdjust.height)>=480)
                {
                    //LOG(INFO)<<"算子编号:"<<errIndex<<"位置调整出错,进行回调!";
                    rectAdjust=rectInit;
                }
                curTmp=cv::Mat(cur,rectAdjust);


                //LOG(INFO)<<"开始缺陷检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
                CheckOperatorAssociatedInspect2 * pCheckOperatorAssociatedInspect2 = dynamic_cast<CheckOperatorAssociatedInspect2*>((*itor));
                CheckOperatorAssociatedInspect2Control control2(pCheckOperatorAssociatedInspect2);
                int res=control2.calculate(refTmp, curTmp);
                if (res == 1)
                {
                    OpAssociatedRet=false;
                    errArr[errIndex] = 0;
                    // ret++;
                    //errArr[errIndex] = 1;
                    //LOG(INFO)<<"缺陷检查err";
                }
                else
                {
                    OpAssociatedRet=true;
                    errArr[errIndex]=0;
                    //LOG(INFO)<<"缺陷检查suc";
                }
            }
        }
    }


    {
        DSDEBUG<<"开始计算  camera:"<<this->ClsImgPro->ImgIndex<<endl;
        this->ClsImgPro->CalcOpencvImage.ImageRGB.copyTo(this->ClsImgPro->CurBadOpencvImage.ImageRGB);
    }

    //在进行其他算子检测
    list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin();
    int errIndex = 1;
    for (;itor!=this->ClsImgPro->LstCheckOperator.end();itor++,errIndex++)
    {
        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
        {
            errArr[errIndex]=2;
            continue;
        }
        cv::Rect rectInit(
                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys
                    );

        cv::Rect rectAdjust(rectInit.x,rectInit.y,rectInit.width,rectInit.height);
        cv::Mat refTmp=cv::Mat(ref,rectInit);
        StuCheckOperatorBasicControl control((*itor)->stu_CheckOperatorBasic);
        //choose best basis x and y
        std::vector<int> vRefx;
        std::vector<int> vRefy;
        control.getRefX(vRefx);
        control.getRefY(vRefy);
        cv::Point bestPt(0,0);
        int bestScore=0;
        for(int idx_x=0;idx_x<vRefx.size();idx_x++)
        {

            if(this->ClsImgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
            {
                bestScore=this->ClsImgPro->MapPostion[vRefx[idx_x]].Score;
                bestPt.x=this->ClsImgPro->MapPostion[vRefx[idx_x]].Pt.x;
            }
        }
        bestScore=0;
        for(int idx_y=0;idx_y<vRefy.size();idx_y++)
        {
            if(this->ClsImgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
            {
                bestScore=this->ClsImgPro->MapPostion[vRefy[idx_y]].Score;
                bestPt.y=this->ClsImgPro->MapPostion[vRefy[idx_y]].Pt.y;
            }
        }


        rectAdjust.x=rectAdjust.x+bestPt.x;
        rectAdjust.y=rectAdjust.y+bestPt.y;


        if(rectAdjust.x<0||rectAdjust.y<0||(rectAdjust.x+rectAdjust.width)>=640||(rectAdjust.y+rectAdjust.height)>=480)
        {
            //LOG(INFO)<<"算子编号:"<<errIndex<<"位置调整出错,进行回调!";
            rectAdjust=rectInit;
        }
        cv::Mat curTmp=cv::Mat(cur,rectAdjust);
        //LOG(INFO)<<"算子编号:"<<errIndex<<"  initRect:"<<rectInit<<"   adjustRect:"<<rectAdjust;



        ENUMCHECKOPERATORTYPE type = (*itor)->CheckType;
        switch (type)
        {
        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH://对象搜索
        {

            //LOG(INFO)<<"开始对象搜索,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorTemplateMatch * pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>((*itor));
            CheckOperatorTemplateMatchControl checkOperatorTemplateMatchControl(pCheckOperatorTemplateMatch);
            //检查结果错误，返回-1
            int x_basis=0,y_basis=0,score=0;
            if(this->ClsImgPro->TemplateImage[QString::fromStdString(pCheckOperatorTemplateMatch->Code)].size()==0)
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"对象搜索suc";
                break;
            }

            int res=checkOperatorTemplateMatchControl.calculate(refTmp, curTmp,
                                                                this->ClsImgPro->TemplateImage[QString::fromStdString(pCheckOperatorTemplateMatch->Code)],
                    &x_basis,& y_basis,&score);

            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res==1)
            {
                //LOG(INFO)<<"对象搜索err";
                ret++;
                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,
                                                                  ImgPro::StuPostionAdjust(0,cv::Point(0,0))));
                errArr[errIndex] = 1;
            }
            else
            {
                //LOG(INFO)<<"对象搜索suc";
                //                DSDEBUG<<QString("ref%1:(%2,%3)").arg(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index).arg(x_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs).arg(y_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys);
                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,
                                                                  ImgPro::StuPostionAdjust(score, cv::Point(x_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
                                                                                                            y_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys))));
                //                int index=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index;
                //                int xb=x_basis;
                //                int yb=y_basis;
                //                int posx=pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs;
                //                int posy=pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys;
                errArr[errIndex]=0;
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING://边缘定位
        {
            //LOG(INFO)<<"开始边缘定位,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorEdgePositioning * pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>((*itor));
            CheckOperatorEdgePositioningControl * pCheckOperatorEdgePositioningControl = new CheckOperatorEdgePositioningControl(pCheckOperatorEdgePositioning);
            int x_basis=0,y_basis=0,score=0;
            //检查结果错误，返回-1


            int res=pCheckOperatorEdgePositioningControl->calculate(refTmp, curTmp,&x_basis,&y_basis,&score);
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res==1)
            {
                //LOG(INFO)<<"边缘定位err";
                ret++;
                //找不到边缘,不做偏移
                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index,
                                                                  ImgPro::StuPostionAdjust(0,cv::Point(0,0))));
                //LOG(INFO)<<"边缘查找失败,插入调整编号:"<<pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index<<"  调整位置:(0,0)";
                errArr[errIndex] = 1;
            }
            else
            {
                //LOG(INFO)<<"边缘定位suc";
                if(pCheckOperatorEdgePositioning->EdgePos==CheckOperatorEdgePositioning::ENUMEDGEPOS_LEFT||
                        pCheckOperatorEdgePositioning->EdgePos==CheckOperatorEdgePositioning::ENUMEDGEPOS_RIGHT)
                {
                    this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index,
                                                                      ImgPro::StuPostionAdjust(score,cv::Point(x_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX,
                                                                                                               0))));

                    DSDEBUG<<QString("(%1,%2)").arg(x_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX).arg(   0);
                    //LOG(INFO)<<"边缘查找成功,插入调整编号:"<<pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index<<"  调整位置:"<<cv::Point(x_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Xs-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosX,0);
                }
                else
                {
                    this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index,
                                                                      ImgPro::StuPostionAdjust(score,cv::Point(0,
                                                                                                               y_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY))));

                    DSDEBUG<<QString("(%1,%2)").arg(0).arg( y_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY);
                    //LOG(INFO)<<"边缘查找成功,插入调整编号:"<<pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Index<<"  调整位置:"<<cv::Point(0,y_basis+pCheckOperatorEdgePositioning->stu_CheckOperatorBasic->Basic_RectValid_Ys-pCheckOperatorEdgePositioning->StuStuEdgeLine->PosY);
                }
                errArr[errIndex]=0;
            }
            delete pCheckOperatorEdgePositioningControl;
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT://缺陷检测
        {
            //LOG(INFO)<<"开始缺陷检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorDefectInspect * pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>((*itor));
            CheckOperatorDefectInspectControl control(pCheckOperatorDefectInspect);

            int res=control.calculate(refTmp, curTmp);
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&&OpAssociatedRet;

            if (res == 1)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"缺陷检查err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"缺陷检查suc";
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT://圆形检查
        {
            //LOG(INFO)<<"开始缺陷检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorCircleInspect * pCheckOperatorCircleInspect = dynamic_cast<CheckOperatorCircleInspect*>((*itor));
            CheckOperatorCircleInspectControl control(pCheckOperatorCircleInspect);

            int res=control.calculate(refTmp, curTmp);

            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res == 1)
            {
                //                cv::imwrite("./refTemp.bmp",refTmp);
                //                cv::imwrite("./curTemp.bmp",curTmp);
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"缺陷检查err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"缺陷检查suc";
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_DEFECTINSPECT2://缺陷检测
        {
            //LOG(INFO)<<"开始异形检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorDefectInspect2 * pCheckOperatorDefectInspect2 = dynamic_cast<CheckOperatorDefectInspect2*>((*itor));
            CheckOperatorDefectInspect2Control control(pCheckOperatorDefectInspect2);

            int res=control.calculate(refTmp, curTmp);

            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res == 1)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"异形检查err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"异形检查suc";
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT://折线检查
        {
            //LOG(INFO)<<"开始折现检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorBrokenlineInspect * pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>((*itor));
            CheckOperatorBrokenlineInspectControl * pCheckOperatorBrokenlineInspectControl = new CheckOperatorBrokenlineInspectControl(pCheckOperatorBrokenlineInspect);
            //检查结果错误，返回-1


            int res=pCheckOperatorBrokenlineInspectControl->calculate(refTmp, curTmp);
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res==1)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"折线检查err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"折线检查suc";
            }
            delete pCheckOperatorBrokenlineInspectControl;
        }
            break;
        case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT://灰度测量
        {
            //LOG(INFO)<<"开始灰度测量,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorGrayMeasurement * pCheckOperatorGrayMeasurement = dynamic_cast<CheckOperatorGrayMeasurement*>((*itor));
            CheckOperatorGrayMeasurementControl * pCheckOperatorGrayMeasurementControl = new CheckOperatorGrayMeasurementControl(pCheckOperatorGrayMeasurement);
            //检查结果错误，返回1
            int res=pCheckOperatorGrayMeasurementControl->calculate(refTmp, curTmp);

            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res== 1)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"灰度测量err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"灰度测量suc";
            }
            delete pCheckOperatorGrayMeasurementControl;
        }
            break;
        case ENUMCHECKOPERATORTYPE_LINEINSPECT://直线检测
        {
            CheckOperatorLineInspect * pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>((*itor));
            CheckOperatorLineInspectControl * pCheckOperatorLineInspectControl = new CheckOperatorLineInspectControl(pCheckOperatorLineInspect);
            //检查结果错误，返回-1

            int res=pCheckOperatorLineInspectControl->calculate(ref, cur);
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res== 1)
            {
                ret++;
                errArr[errIndex] = 1;
            }
            else
            {
                errArr[errIndex]=0;
            }
            delete pCheckOperatorLineInspectControl;
        }
            break;

#ifdef FLAG_KONGTOU
        case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT://目标检测
        {


            //LOG(INFO)<<"开始异形检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorDLObjectDetect * pCheckOperatorDLObjectDetect= dynamic_cast<CheckOperatorDLObjectDetect*>((*itor));
            CheckOperatorDLObjectDetectControl control(pCheckOperatorDLObjectDetect);

            //            accuracyType=pCheckOperatorDLObjectDetect->m_iaccuracyType;
            //            if(this->ClsImgPro->ImgIndex==2)

            DSDEBUG<<"开始推理  camera:"<<this->ClsImgPro->ImgIndex<<endl;
            if(OnnxGloable::getInstance()->onnxArray[this->ClsImgPro->ImgIndex-1].m_bIsValid)
            {
                //            this->ClsImgPro->CurBadOpencvImage.ImageRGB=this->ClsImgPro->CalcOpencvImage.ImageRGB;
                std::vector<BBoxInfo> vecBBoxInfo;
                cv::Mat inputImg;

                int dstSize=640;

                int maxHW = ref.rows > ref.cols ? ref.rows : ref.cols;
                int toppad,bottompad, leftpad,rightpad;
                if (maxHW > dstSize)
                {
                    toppad = (maxHW - ref.rows) / 2;
                    bottompad = maxHW - ref.rows - toppad;

                    leftpad = (maxHW - ref.cols) / 2;
                    rightpad = maxHW - ref.cols - leftpad;
                }
                else
                {
                    toppad = (dstSize - ref.rows) / 2;
                    bottompad = dstSize - ref.rows - toppad;

                    leftpad = (dstSize - ref.cols) / 2;
                    rightpad = dstSize - ref.cols - leftpad;
                }

                cv::copyMakeBorder(this->ClsImgPro->CalcOpencvImage.ImageRGB, inputImg, toppad, bottompad, leftpad, rightpad, cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));

                //在初始化的时候设置初始置信度为0.1，在检测出目标框之后再使用页面设定的阈值进行过滤
                OnnxGloable::getInstance()->m_onnxMutex[this->ClsImgPro->ImgIndex-1].lock();
                OnnxGloable::getInstance()->onnxArray[this->ClsImgPro->ImgIndex-1].setConfidence(0.1);
                vector<vector<BBoxInfo>> vec_batch_result;
                OnnxGloable::getInstance()->onnxArray[this->ClsImgPro->ImgIndex-1].detect(inputImg,vec_batch_result);
                OnnxGloable::getInstance()->m_onnxMutex[this->ClsImgPro->ImgIndex-1].unlock();

                DSDEBUG<<"推理结束  camera:"<<this->ClsImgPro->ImgIndex<<endl;
                int boxID=0;
                for(int i=0;i<vec_batch_result.size();i++)
                {
                    for(int j=0;j<vec_batch_result[i].size();j++)
                    {
                        int x=(vec_batch_result[i][j].rect.x-leftpad)>0?(vec_batch_result[i][j].rect.x-leftpad):0;
                        int y=(vec_batch_result[i][j].rect.y-toppad)>0?(vec_batch_result[i][j].rect.y-toppad):0;

                        int  imgwid=this->ClsImgPro->CalcOpencvImage.ImageRGB.cols;
                        int imghei=this->ClsImgPro->CalcOpencvImage.ImageRGB.rows;

                        int width=(x+vec_batch_result[i][j].rect.width)<=this->ClsImgPro->CalcOpencvImage.ImageRGB.cols?(vec_batch_result[i][j].rect.width):(this->ClsImgPro->CalcOpencvImage.ImageRGB.cols-x);
                        int height=(y+vec_batch_result[i][j].rect.height)<=this->ClsImgPro->CalcOpencvImage.ImageRGB.rows?(vec_batch_result[i][j].rect.height):(this->ClsImgPro->CalcOpencvImage.ImageRGB.rows-y);

                        vec_batch_result[i][j].rect=cv::Rect( x,y,width,height);

                        vec_batch_result[i][j].boxID=boxID;
                        boxID++;
                        vecBBoxInfo.push_back(vec_batch_result[i][j]);

                    }
                }

                //            DSDEBUG<<"推理结束1  camera:"<<this->ClsImgPro->ImgIndex<<endl;

                list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin();
                list<CheckOperatorBasic*>::iterator itorMax;
                int OKBoxs=0;
                int NGBoxs=0;
                int CigaTotalNum=0;

                int totalNum=0;

                for(int i=0;i<vecBBoxInfo.size();i++)
                {
                    int maxArea=0;
                    int errIndex = 1;
                    int maxIndex=1;
                    cv::Mat curTmp;

                    cv::Rect rectCropMap;
                    for (;itor!=this->ClsImgPro->LstCheckOperator.end();itor++,errIndex++)
                    {
                        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
                        {
                            errArr[errIndex]=2;
                            continue;
                        }

                        if((*itor)->CheckType==ENUMCHECKOPERATORTYPE::ENUMCHECKOPERATORTYPE_DLOBJECTDETECT)
                        {

                            cv::Rect rectInit(
                                        (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                        (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys,
                                        (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
                                        (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys
                                        );

                            cv::Rect rectAdjust(rectInit.x,rectInit.y,rectInit.width,rectInit.height);
                            cv::Mat refTmp=cv::Mat(ref,rectInit);
                            StuCheckOperatorBasicControl control((*itor)->stu_CheckOperatorBasic);

                            std::vector<int> vRefx;
                            std::vector<int> vRefy;
                            control.getRefX(vRefx);
                            control.getRefY(vRefy);
                            cv::Point bestPt(0,0);
                            int bestScore=0;
                            for(int idx_x=0;idx_x<vRefx.size();idx_x++)
                            {

                                if(this->ClsImgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
                                {
                                    bestScore=this->ClsImgPro->MapPostion[vRefx[idx_x]].Score;
                                    bestPt.x=this->ClsImgPro->MapPostion[vRefx[idx_x]].Pt.x;
                                }
                            }
                            bestScore=0;
                            for(int idx_y=0;idx_y<vRefy.size();idx_y++)
                            {

                                if(this->ClsImgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
                                {
                                    bestScore=this->ClsImgPro->MapPostion[vRefy[idx_y]].Score;
                                    bestPt.y=this->ClsImgPro->MapPostion[vRefy[idx_y]].Pt.y;
                                }
                            }

                            rectAdjust.x=rectAdjust.x+bestPt.x;
                            rectAdjust.y=rectAdjust.y+bestPt.y;
                            if(rectAdjust.x<0||rectAdjust.y<0||(rectAdjust.x+rectAdjust.width)>=640||(rectAdjust.y+rectAdjust.height)>=480)
                            {
                                //LOG(INFO)<<"算子编号:"<<errIndex<<"位置调整出错,进行回调!";
                                rectAdjust=rectInit;
                            }
                            curTmp=cv::Mat(cur,rectAdjust);

                            rectCropMap=rectAdjust;
                            itorMax=itor;
                            break;

                        }
                    }

                    if(itorMax._Ptr!=NULL)
                    {
                        CheckOperatorDLObjectDetect * pCheckOperatorDLObjectDetect= dynamic_cast<CheckOperatorDLObjectDetect*>((*itorMax));
                        CheckOperatorDLObjectDetectControl control(pCheckOperatorDLObjectDetect);
                        // errArr[errIndex]=0;

                        CigaTotalNum=   pCheckOperatorDLObjectDetect->m_iCigaTotalNum;
                        cv::Rect interRect=vecBBoxInfo[i].rect&rectCropMap;
                        int res=-1;
                        if(interRect.area()>vecBBoxInfo[i].rect.area()/2)
                        {
                            res=control.calculate(this->ClsImgPro->CalcOpencvImage.ImageRGB, curTmp,vecBBoxInfo[i]);

                            if (res == 0)
                            {
                                cv::rectangle(this->ClsImgPro->CurBadOpencvImage.ImageRGB,vecBBoxInfo[i].rect,cv::Scalar(255,0,0),3);
                                errArr[errIndex]=1;
                                ret++;
                                NGBoxs++;
                            }
                            if(res==1)
                            {
                                OKBoxs++;
                            }
                        }
                    }
                }


                //计算完之后再剔除重叠部分
                //剔除重叠的部分
                std::vector<BBoxInfo> vecBBoxFilter,vecBBoxFilterTmp;
                for(int i=0;i<vecBBoxInfo.size();i++)
                {
                    vecBBoxFilterTmp.clear();
                    for(int j=0;j<vecBBoxInfo.size();j++)
                    {
                        if(i!=j)
                        {
                            cv::Rect interRect=vecBBoxInfo[i].rect&vecBBoxInfo[j].rect;
                            double per=interRect.area()/double((vecBBoxInfo[i].rect.area()+vecBBoxInfo[j].rect.area())-interRect.area());

                            double per2=interRect.area()/double(qMax(vecBBoxInfo[i].rect.area(),vecBBoxInfo[j].rect.area()));

                            if(interRect.area()/double((vecBBoxInfo[i].rect.area()+vecBBoxInfo[j].rect.area())-interRect.area())>0.3)

                                //if(interRect.area()/double(qMax(vecBBoxInfo[i].rect.area(),vecBBoxInfo[j].rect.area()))>0.5)
                            {
                                vecBBoxFilterTmp.push_back(vecBBoxInfo[j]);
                            }
                        }


                    }
                    vecBBoxFilterTmp.push_back(vecBBoxInfo[i]);
                    //把最大的置信度选出来

                    float tenpprob=0;
                    int maxprobID=0;
                    for(int k=0;k<vecBBoxFilterTmp.size();k++)
                    {
                        if(vecBBoxFilterTmp[k].prob>tenpprob)
                        {
                            tenpprob=vecBBoxFilterTmp[k].prob;
                            maxprobID=k;
                        }
                    }
                    vecBBoxFilter.push_back(vecBBoxFilterTmp[maxprobID]);
                }
                //剔除相同的ID
                std::vector<BBoxInfo> vecBBoxInfoTemp;
                vecBBoxInfo.swap(vecBBoxInfoTemp);
                for(int m=0;m<vecBBoxInfoTemp.size();m++)
                {
                    for(int n=0;n<vecBBoxFilter.size();n++ )
                    {
                        if(vecBBoxFilter[n].boxID==m)
                        {
                            vecBBoxInfo.push_back(vecBBoxFilter[n]);
                            break;
                        }
                    }
                }


                for(int i=0;i<vecBBoxInfo.size();i++)
                {
                    if((vecBBoxInfo[i].prob>50/100.0))
                    {
                        totalNum++;
                    }
                }


                if(totalNum< pCheckOperatorDLObjectDetect->m_iCigaTotalNum)
                {
                    ret++;
                    errArr[errIndex]=1;
                }

            }

        }
            break;

#endif

        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2://关联检测
        {
            //LOG(INFO)<<"开始异形检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            //            CheckOperatorDLObjectDetect * pCheckOperatorDLObjectDetect= dynamic_cast<CheckOperatorDLObjectDetect*>((*itor));
            //            CheckOperatorDLObjectDetectControl control(pCheckOperatorDLObjectDetect);

            //            errArr[errIndex]=0;
            //            //            vector<cv::Rect> vec_result;
            //            int res=control.calculate(this->ClsImgPro->CalcOpencvImage.ImageRGB, curTmp,vecBBoxInfo);
            //            if (res == 1)
            //            {
            //                errArr[errIndex]=1;
            //                ret++;
            //            }
            //            else
            //            {
            //                errArr[errIndex]=0;
            //                //LOG(INFO)<<"异形检查suc";
            //            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT://关联检查
        {
            //LOG(INFO)<<"开始关联检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorAssociatedInspect * pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>((*itor));
            CheckOperatorAssociatedInspectControl * pCheckOperatorAssociatedInspectControl = new CheckOperatorAssociatedInspectControl(pCheckOperatorAssociatedInspect);



            //            int res=
            //            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
            //            res=res&OpAssociatedRet;

            //            if (res== 1)

            if(pCheckOperatorAssociatedInspect->Relaty1==0||pCheckOperatorAssociatedInspect->Relaty2==0)
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"关联项目只有一个,默认正确";
                delete pCheckOperatorAssociatedInspectControl;
                break;
            }

            switch(pCheckOperatorAssociatedInspect->LineType)
            {
            case CheckOperatorAssociatedInspect::ENUMLINETYPE_SINGLE:
            {
                ImgProControl control(this->ClsImgPro);
                int p1= static_cast<CheckOperatorStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->center;
                int p2= static_cast<CheckOperatorStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->center;

                //检查结果错误，返回-1
                if (pCheckOperatorAssociatedInspectControl->calculate(ref, cur,p1,p2) == 1)
                {
                    ret++;
                    errArr[errIndex] = 1;
                    //                    //LOG(INFO)<<"关联检测err";
                }
                else
                {
                    errArr[errIndex]=0;
                    //                    //LOG(INFO)<<"关联检测suc";
                }
            }
                break;

            case CheckOperatorAssociatedInspect::ENUMLINETYPE_DOUBLE:
            {
                ImgProControl control(this->ClsImgPro);
                //                int p11= static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->center1;
                //                int p12= static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->center2;
                //                int p21= static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->center1;
                //                int p22= static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->center2;

                int basis_c1_xs=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->stu_CheckOperatorBasic->Basic_RectValid_Xs;
                int basis_c1_ys=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->stu_CheckOperatorBasic->Basic_RectValid_Ys;
                int basis_c2_xs=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->stu_CheckOperatorBasic->Basic_RectValid_Xs;
                int basis_c2_ys=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->stu_CheckOperatorBasic->Basic_RectValid_Ys;

                int yCalc=(pCheckOperatorAssociatedInspectControl->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic->Basic_RectValid_Ys+pCheckOperatorAssociatedInspectControl->ClsCheckOperatorAssociatedInspect->stu_CheckOperatorBasic->Basic_RectValid_Ye)/2;


                int c1p1x1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->X11+basis_c1_xs;
                int c1p1y1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->Y11+basis_c1_ys;
                int c1p1x2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->X12+basis_c1_xs;
                int c1p1y2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->Y12+basis_c1_ys;

                int c1p2x1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->X21+basis_c1_xs;
                int c1p2y1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->Y21+basis_c1_ys;
                int c1p2x2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->X22+basis_c1_xs;
                int c1p2y2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->Y22+basis_c1_ys;

                int c2p1x1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->X11+basis_c2_xs;
                int c2p1y1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->Y11+basis_c2_ys;
                int c2p1x2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->X12+basis_c2_xs;
                int c2p1y2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->Y12+basis_c2_ys;

                int c2p2x1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->X21+basis_c2_xs;
                int c2p2y1=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->Y21+basis_c2_ys;
                int c2p2x2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->X22+basis_c2_xs;
                int c2p2y2=static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->Y22+basis_c2_ys;

                int p11=0,p12=0,p21=0,p22=0;

                p11=(double(yCalc-c1p1y1))/(c1p1y2-c1p1y1)*(c1p1x2-c1p1x1)+c1p1x1;
                p12=(double(yCalc-c1p2y1))/(c1p2y2-c1p2y1)*(c1p2x2-c1p2x1)+c1p2x1;
                p21=(double(yCalc-c2p1y1))/(c2p1y2-c2p1y1)*(c2p1x2-c2p1x1)+c2p1x1;
                p22=(double(yCalc-c2p2y1))/(c2p2y2-c2p2y1)*(c2p2x2-c2p2x1)+c2p2x1;
                DSDEBUG<<QString("C1Line1:(%1,%2,%3,%4)").arg(QString::number(c1p1x1)).arg(QString::number(c1p1y1)).arg(QString::number(c1p1x2)).arg(QString::number(c1p1y2));
                DSDEBUG<<QString("C1Line2:(%1,%2,%3,%4)").arg(QString::number(c1p2x1)).arg(QString::number(c1p2y1)).arg(QString::number(c1p2x2)).arg(QString::number(c1p2y2));
                DSDEBUG<<QString("C2Line1:(%1,%2,%3,%4)").arg(QString::number(c2p1x1)).arg(QString::number(c2p1y1)).arg(QString::number(c2p1x2)).arg(QString::number(c2p1y2));
                DSDEBUG<<QString("C2Line2:(%1,%2,%3,%4)").arg(QString::number(c2p2x1)).arg(QString::number(c2p2y1)).arg(QString::number(c2p2x2)).arg(QString::number(c2p2y2));
                DSDEBUG<<QString("(%1,%2,%3,%4)").arg(p11).arg(p12).arg(p21).arg(p22);

                static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->center1=p11;
                static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty1))->center2=p12;
                static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->center1=p21;
                static_cast<CheckOperatorDoubleStayInspect*>(control.getCheckNo(pCheckOperatorAssociatedInspect->Relaty2))->center2=p22;
                //检查结果错误，返回-1



                int res=pCheckOperatorAssociatedInspectControl->calculate(ref, cur,qMin(p11,p12),qMax(p11,p12),qMin(p21,p22),qMax(p21,p22)) == 1;
                if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                    res=res&OpAssociatedRet;
                if (res== 1)

                    // if (pCheckOperatorAssociatedInspectControl->calculate(ref, cur,qMin(p11,p12),qMax(p11,p12),qMin(p21,p22),qMax(p21,p22)) == 1)
                {
                    ret++;
                    errArr[errIndex] = 1;
                    //                    //LOG(INFO)<<"关联检测err";
                }
                else
                {
                    errArr[errIndex]=0;
                    //                    //LOG(INFO)<<"关联检测suc";
                }
            }
                break;
            }
        }
            break;
        case ENUMCHECKOPERATORTYPE_STAYINSPECT://拉线检测
        {
            //LOG(INFO)<<"开始拉线检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorStayInspect * pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>((*itor));
            CheckOperatorStayInspectControl * pCheckOperatorStayInspectControl = new CheckOperatorStayInspectControl(pCheckOperatorStayInspect);



            int res=pCheckOperatorStayInspectControl->calculate(refTmp, curTmp)!=0;
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res== 1)

                //检查结果错误，返回-1
                //if (pCheckOperatorStayInspectControl->calculate(refTmp, curTmp) != 0)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"拉线检测err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"拉线检测suc";
            }
            delete pCheckOperatorStayInspectControl;
        }
            break;
        case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT://双拉线检测
        {
            //LOG(INFO)<<"开始拉线检测,相机:"<<this->ClsImgPro->ImgIndex<<" 编号"<< (*itor)->stu_CheckOperatorBasic->Index;
            CheckOperatorDoubleStayInspect * pCheckOperatorDoubleStayInspect = dynamic_cast<CheckOperatorDoubleStayInspect*>((*itor));
            CheckOperatorDoubleStayInspectControl * pCheckOperatorDoubleStayInspectControl = new CheckOperatorDoubleStayInspectControl(pCheckOperatorDoubleStayInspect);

            //检查结果错误，返回-1

            int res=pCheckOperatorDoubleStayInspectControl->calculate(refTmp, curTmp) != 0;
            if((*itor)->stu_CheckOperatorBasic->IsAssociated)
                res=res&OpAssociatedRet;

            if (res== 1)
                //if (pCheckOperatorDoubleStayInspectControl->calculate(refTmp, curTmp) != 0)
            {
                ret++;
                errArr[errIndex] = 1;
                //LOG(INFO)<<"拉线检测err";
            }
            else
            {
                errArr[errIndex]=0;
                //LOG(INFO)<<"拉线检测suc";
            }
            delete pCheckOperatorDoubleStayInspectControl;
        }
            break;
        }
    }






    //    if(this->ClsImgPro->ImgIndex==2)
    //    {
    //        this->ClsImgPro->CalcOpencvImage.ImageRGB.copyTo(this->ClsImgPro->CurBadOpencvImage.ImageRGB);
    //    }

    //     DSDEBUG<<"开始推理0  camera:"<<this->ClsImgPro->ImgIndex<<endl;

    //#ifdef FLAG_KONGTOU
    //    if(this->ClsImgPro->ImgIndex==2||this->ClsImgPro->ImgIndex==4)

    //        if(1)
    //        {
    //            //            this->ClsImgPro->CurBadOpencvImage.ImageRGB=this->ClsImgPro->CalcOpencvImage.ImageRGB;
    //            std::vector<BBoxInfo> vecBBoxInfo;
    //            cv::Mat inputImg;

    //            int dstSize=640;

    //            int maxHW = ref.rows > ref.cols ? ref.rows : ref.cols;
    //            int toppad,bottompad, leftpad,rightpad;
    //            if (maxHW > dstSize)
    //            {
    //                toppad = (maxHW - ref.rows) / 2;
    //                bottompad = maxHW - ref.rows - toppad;

    //                leftpad = (maxHW - ref.cols) / 2;
    //                rightpad = maxHW - ref.cols - leftpad;
    //            }
    //            else
    //            {
    //                toppad = (dstSize - ref.rows) / 2;
    //                bottompad = dstSize - ref.rows - toppad;

    //                leftpad = (dstSize - ref.cols) / 2;
    //                rightpad = dstSize - ref.cols - leftpad;
    //            }

    //            cv::copyMakeBorder(this->ClsImgPro->CalcOpencvImage.ImageRGB, inputImg, toppad, bottompad, leftpad, rightpad, cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));

    //            //在初始化的时候设置初始置信度为0.1，在检测出目标框之后再使用页面设定的阈值进行过滤
    ////            int onnxIndex=(this->ClsImgPro->ImgIndex-2)/2;
    ////            if (accuracyType==0)
    ////            {
    ////                //正常精度
    ////                onnxIndex=0;
    ////            }
    ////            else
    ////            {
    ////                //高精度
    ////                onnxIndex=1;
    ////            }
    //  //          onnxIndex=0;




    //            OnnxGloable::getInstance()->m_onnxMutex[0].lock();
    //            OnnxGloable::getInstance()->onnxArray[0].setConfidence(0.1);
    //            vector<vector<BBoxInfo>> vec_batch_result;
    //            OnnxGloable::getInstance()->onnxArray[0].detect(inputImg,vec_batch_result);
    //            OnnxGloable::getInstance()->m_onnxMutex[0].unlock();

    //            DSDEBUG<<"推理结束0  camera:"<<this->ClsImgPro->ImgIndex<<endl;
    //            int boxID=0;
    //            for(int i=0;i<vec_batch_result.size();i++)
    //            {
    //                for(int j=0;j<vec_batch_result[i].size();j++)
    //                {
    //                    int x=(vec_batch_result[i][j].rect.x-leftpad)>0?(vec_batch_result[i][j].rect.x-leftpad):0;
    //                    int y=(vec_batch_result[i][j].rect.y-toppad)>0?(vec_batch_result[i][j].rect.y-toppad):0;

    //                    int  imgwid=this->ClsImgPro->CalcOpencvImage.ImageRGB.cols;
    //                    int imghei=this->ClsImgPro->CalcOpencvImage.ImageRGB.rows;

    //                    int width=(x+vec_batch_result[i][j].rect.width)<=this->ClsImgPro->CalcOpencvImage.ImageRGB.cols?(vec_batch_result[i][j].rect.width):(this->ClsImgPro->CalcOpencvImage.ImageRGB.cols-x);
    //                    int height=(y+vec_batch_result[i][j].rect.height)<=this->ClsImgPro->CalcOpencvImage.ImageRGB.rows?(vec_batch_result[i][j].rect.height):(this->ClsImgPro->CalcOpencvImage.ImageRGB.rows-y);

    //                    vec_batch_result[i][j].rect=cv::Rect( x,y,width,height);

    //                    vec_batch_result[i][j].boxID=boxID;
    //                    boxID++;
    //                    vecBBoxInfo.push_back(vec_batch_result[i][j]);

    //                }
    //            }

    ////            DSDEBUG<<"推理结束1  camera:"<<this->ClsImgPro->ImgIndex<<endl;


    //            list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin();
    //            list<CheckOperatorBasic*>::iterator itorMax;
    //            int OKBoxs=0;

    ////            //剔除重叠的部分
    ////            std::vector<BBoxInfo> vecBBoxFilter,vecBBoxFilterTmp;

    ////            for(int i=0;i<vecBBoxInfo.size();i++)
    ////            {
    ////                vecBBoxFilterTmp.clear();
    ////                for(int j=0;j<vecBBoxInfo.size();j++)
    ////                {
    ////                    if(i!=j)
    ////                    {
    ////                        cv::Rect interRect=vecBBoxInfo[i].rect&vecBBoxInfo[j].rect;
    ////                        double per=interRect.area()/double((vecBBoxInfo[i].rect.area()+vecBBoxInfo[j].rect.area())-interRect.area());

    ////                        double per2=interRect.area()/double(qMax(vecBBoxInfo[i].rect.area(),vecBBoxInfo[j].rect.area()));

    ////                        if(interRect.area()/double((vecBBoxInfo[i].rect.area()+vecBBoxInfo[j].rect.area())-interRect.area())>0.3)

    ////                            //if(interRect.area()/double(qMax(vecBBoxInfo[i].rect.area(),vecBBoxInfo[j].rect.area()))>0.5)
    ////                        {
    ////                            vecBBoxFilterTmp.push_back(vecBBoxInfo[j]);
    ////                        }
    ////                    }

    ////                }
    ////                vecBBoxFilterTmp.push_back(vecBBoxInfo[i]);
    ////                //把最大的置信度选出来

    ////                float tenpprob=0;
    ////                int maxprobID=0;
    ////                for(int k=0;k<vecBBoxFilterTmp.size();k++)
    ////                {
    ////                    if(vecBBoxFilterTmp[k].prob>tenpprob)
    ////                    {
    ////                        tenpprob=vecBBoxFilterTmp[k].prob;
    ////                        maxprobID=k;
    ////                    }
    ////                }
    ////                vecBBoxFilter.push_back(vecBBoxFilterTmp[maxprobID]);
    ////            }


    ////            //剔除相同的ID
    ////            std::vector<BBoxInfo> vecBBoxInfoTemp;
    ////            vecBBoxInfo.swap(vecBBoxInfoTemp);
    ////            for(int m=0;m<vecBBoxInfoTemp.size();m++)
    ////            {
    ////                for(int n=0;n<vecBBoxFilter.size();n++ )
    ////                {
    ////                    if(vecBBoxFilter[n].boxID==m)
    ////                    {
    ////                        vecBBoxInfo.push_back(vecBBoxFilter[n]);
    ////                        break;
    ////                    }
    ////                }
    ////            }


    //            for(int i=0;i<vecBBoxInfo.size();i++)
    //            {
    //                int maxArea=0;
    //                int errIndex = 1;
    //                int maxIndex=1;
    //                cv::Mat curTmp;

    //                cv::Rect rectCropMap;
    //                for (;itor!=this->ClsImgPro->LstCheckOperator.end();itor++,errIndex++)
    //                {
    //                    if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
    //                    {
    //                        errArr[errIndex]=2;
    //                        continue;
    //                    }

    //                    if((*itor)->CheckType==ENUMCHECKOPERATORTYPE::ENUMCHECKOPERATORTYPE_DLOBJECTDETECT)
    //                    {

    //                        cv::Rect rectInit(
    //                                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
    //                                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys,
    //                                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
    //                                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys
    //                                    );

    //                        cv::Rect rectAdjust(rectInit.x,rectInit.y,rectInit.width,rectInit.height);
    //                        cv::Mat refTmp=cv::Mat(ref,rectInit);
    //                        StuCheckOperatorBasicControl control((*itor)->stu_CheckOperatorBasic);

    //                        std::vector<int> vRefx;
    //                        std::vector<int> vRefy;
    //                        control.getRefX(vRefx);
    //                        control.getRefY(vRefy);
    //                        cv::Point bestPt(0,0);
    //                        int bestScore=0;
    //                        for(int idx_x=0;idx_x<vRefx.size();idx_x++)
    //                        {

    //                            if(this->ClsImgPro->MapPostion[vRefx[idx_x]].Score>bestScore)
    //                            {
    //                                bestScore=this->ClsImgPro->MapPostion[vRefx[idx_x]].Score;
    //                                bestPt.x=this->ClsImgPro->MapPostion[vRefx[idx_x]].Pt.x;
    //                            }
    //                        }
    //                        bestScore=0;
    //                        for(int idx_y=0;idx_y<vRefy.size();idx_y++)
    //                        {

    //                            if(this->ClsImgPro->MapPostion[vRefy[idx_y]].Score>bestScore)
    //                            {
    //                                bestScore=this->ClsImgPro->MapPostion[vRefy[idx_y]].Score;
    //                                bestPt.y=this->ClsImgPro->MapPostion[vRefy[idx_y]].Pt.y;
    //                            }
    //                        }

    //                        rectAdjust.x=rectAdjust.x+bestPt.x;
    //                        rectAdjust.y=rectAdjust.y+bestPt.y;
    //                        if(rectAdjust.x<0||rectAdjust.y<0||(rectAdjust.x+rectAdjust.width)>=640||(rectAdjust.y+rectAdjust.height)>=480)
    //                        {
    //                            //LOG(INFO)<<"算子编号:"<<errIndex<<"位置调整出错,进行回调!";
    //                            rectAdjust=rectInit;
    //                        }
    //                        curTmp=cv::Mat(cur,rectAdjust);

    //                        rectCropMap=rectAdjust;
    //                        //
    //                        //                    cv::Rect interRect=vecBBoxInfo[i].rect&rectAdjust;
    //                        //                    if((interRect.area()>maxArea)&&(interRect.area()>vecBBoxInfo[i].rect.area()/2))
    //                        //                    {
    //                        //                        maxArea=interRect.area();
    //                        //                        maxIndex=errIndex;
    //                        //                        itorMax=itor;
    //                        //                    }

    //                        itorMax=itor;
    //                        break;

    //                    }
    //                }
    //                DSDEBUG<<"推理结束2  camera:"<<this->ClsImgPro->ImgIndex<<endl;

    //                if(itorMax._Ptr!=NULL)
    //                {
    //                    CheckOperatorDLObjectDetect * pCheckOperatorDLObjectDetect= dynamic_cast<CheckOperatorDLObjectDetect*>((*itorMax));
    //                    CheckOperatorDLObjectDetectControl control(pCheckOperatorDLObjectDetect);
    //                    // errArr[errIndex]=0;

    //                    cv::Rect interRect=vecBBoxInfo[i].rect&rectCropMap;
    //                    int res=0;
    //                    if(interRect.area()>vecBBoxInfo[i].rect.area()/2)
    //                    {
    //                        res=control.calculate(this->ClsImgPro->CalcOpencvImage.ImageRGB, curTmp,vecBBoxInfo[i]);


    //                        if (res == 1)
    //                        {
    //                            cv::rectangle(this->ClsImgPro->CurBadOpencvImage.ImageRGB,vecBBoxInfo[i].rect,cv::Scalar(255,0,0),3);

    //                            errArr[maxIndex]=1;
    //                            ret++;
    //                        }
    //                        if(res==0)
    //                        {
    //                            OKBoxs++;
    //                        }
    //                    }

    //                }
    //            }
    //            DSDEBUG<<"推理结束3  camera:"<<this->ClsImgPro->ImgIndex<<endl;
    //        }

    //#endif



    return ret;
}


//int ImgProControl::updateRefPos()
//{
//    cv::Mat ref = this->ClsImgPro->RefOpencvImage.ImageGray;
//    cv::Mat cur = this->ClsImgPro->CalcOpenCvImage.ImageGray;
//    this->ClsImgPro->MapPostion.clear();

//    list<CheckOperatorBasic*>::iterator itor = this->ClsImgPro->LstCheckOperator.begin();

//    for (;itor!=this->ClsImgPro->LstCheckOperator.end();itor++)
//    {
//        if(!(*itor)->stu_CheckOperatorBasic->Basic_IsCheck)
//        {
//            continue;
//        }

//        cv::Rect rectInit(
//                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
//                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys,
//                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xe-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Xs,
//                    (*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ye-(*itor)->stu_CheckOperatorBasic->Basic_RectValid_Ys
//                    );

//        cv::Mat refTmp=cv::Mat(ref,rectInit);

//        ENUMCHECKOPERATORTYPE type = (*itor)->CheckType;

//        switch (type)
//        {
//        case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH://对象搜索
//        {
//            CheckOperatorTemplateMatch * pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>((*itor));
//            CheckOperatorTemplateMatchControl checkOperatorTemplateMatchControl(pCheckOperatorTemplateMatch);
//            //检查结果错误，返回-1
//            int x_basis=0,y_basis=0,score=0;
//            if(this->ClsImgPro->TemplateImage[pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index].size()==0)
//            {
//                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,
//                                                                  ImgPro::StuPostionAdjust(0,cv::Point(0,0))));
//                break;
//            }

//            if (checkOperatorTemplateMatchControl.calculate(refTmp, curTmp,
//                                                            this->ClsImgPro->TemplateImage[pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index],
//                                                            &x_basis,& y_basis,&score)==1)
//            {

//                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,
//                                                                  ImgPro::StuPostionAdjust(0,cv::Point(0,0))));

//            }
//            else
//            {
//                DSDEBUG<<QString("ref%1:(%2,%3)").arg(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index).arg(x_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs).arg(y_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys);
//                this->ClsImgPro->MapPostion.insert(std::make_pair(pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index,
//                                                                  ImgPro::StuPostionAdjust(score, cv::Point(x_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs,
//                                                                                                            y_basis-pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys))));
//                //                int index=pCheckOperatorTemplateMatch->stu_CheckOperatorBasic->Index;
//                //                int xb=x_basis;
//                //                int yb=y_basis;
//                //                int posx=pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Xs;
//                //                int posy=pCheckOperatorTemplateMatch->StuStuGrayMatch->PatternRect_Ys;
//                errArr[errIndex]=0;
//            }

//        }
//            break;
//        case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING://边缘定位
//        {

//        }
//            break;
//        }
//    }
//}
