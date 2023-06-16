#include "clsphototelecconfig.h"

#include "clscameraconfig.h"
#include<QString>
#include<QFileInfo>
#include<QTextCodec>
#include<QDateTime>
#include"common/filehelper.h"
#include"params/dssystemparam.h"
#include"params/dsoptions.h"
#include"dsdebug.h"


std::mutex ClsPhototElecConfig::m_Mutex;
ClsPhototElecConfig* ClsPhototElecConfig::m_Instance = nullptr;
ClsPhototElecConfig ::GC ClsPhototElecConfig ::gc;

ClsPhototElecConfig::ClsPhototElecConfig()
{
    StuPhotoElecParam stu;
    for(int i=0;i<10;i++)
    {
        stu.capAngle[i]=10;
    }
    stu.kickAngle=257;
    stu.kickStep=30;
    stu.trigAngle=57;
    stu.isSendFaltSig=1;
    stu.selfCheckAngle=36;
    for(int i=0;i<20;i++)
    {
        stu.probThreshold[i]=15;
    }
    stu.ratio=1.0;
    m_photoElecParamArr.clear();
    m_photoElecParamArr.insert(1,stu);
    m_photoElecParamArr.insert(2,stu);


    load();
}

ClsPhototElecConfig *ClsPhototElecConfig::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == NULL) {
        m_Instance = new ClsPhototElecConfig();
    }
    m_Mutex.unlock();
    return m_Instance;
}

XMLElement *ClsPhototElecConfig::genCurrentXmlElementBlockSim(TinyXmlDocument &doc, int index)
{
    QString str;
    if(index==1)str="QZCurrentVal";
    if(index==2) str="KTCurrentVal";

    XMLElement* eleCurrent = doc.NewElement(str.toStdString().c_str());
    for(int i=1;i<=20;i++)
    {
        QString strCurrent=QString("CurrentVal_%1").arg(i);
        XMLElement* eletmp = doc.NewElement(strCurrent.toStdString().c_str());

        eletmp->SetText(this->m_photoElecParamArrSim[index].currentArr[i-1]);
        eleCurrent->InsertEndChild(eletmp);
    }
    return eleCurrent;
}

XMLElement *ClsPhototElecConfig::genGainXmlElementBlockSim(TinyXmlDocument &doc, int index)
{
    QString str;
    if(index==1)str="QZGainVal";
    if(index==2) str="KTGainVal";

    XMLElement* eleGain = doc.NewElement(str.toStdString().c_str());
    for(int i=1;i<=20;i++)
    {
        QString strGain=QString("GainVal_%1").arg(i);
        XMLElement* eletmp = doc.NewElement(strGain.toStdString().c_str());

        eletmp->SetText(this->m_photoElecParamArrSim[index].gainArr[i-1]);
        eleGain->InsertEndChild(eletmp);
    }
    return eleGain;
}

XMLElement *ClsPhototElecConfig::genProbValXmlElementBlockSim(TinyXmlDocument &doc, int index)
{
    QString str;
    if(index==1)str="QZProbVal";
    if(index==2) str="KTProbVal";

    XMLElement* eleProb = doc.NewElement(str.toStdString().c_str());
    for(int i=1;i<=20;i++)
    {
        QString strProb=QString("ProbVal_%1").arg(i);
        XMLElement* eletmp = doc.NewElement(strProb.toStdString().c_str());

        eletmp->SetText(this->m_photoElecParamArrSim[index].probThreshold[i-1]);
        eleProb->InsertEndChild(eletmp);
    }
    return eleProb;
}


void ClsPhototElecConfig::load()
{

    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/PhotoElecConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();
    QFileInfo fi(path);

    DSClsOptions * options=DSClsOptions::getInstance();
    int max=options->MaxCameraNum();

    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"PhotoElecConfig.xml");

        std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
        //DSDEBUG<<"declaration:"<<QString::fromStdString(declaration)<<endl;
        TinyXmlDocument doc;
        doc.LoadFile(name.c_str());
        doc.Parse(declaration.c_str());

        XMLElement* elePhotoElecConfig=doc.NewElement("PhotoElecConfig");

        DSClsOptions * options=DSClsOptions::getInstance();
        //for(int idx=1;idx<=options->MaxCameraNum();idx++)
        for(int idx=1;idx<=2;idx++)
        {
            QString str,strCom;
            if(idx==1){str="QZ";}
            if(idx==2) {str="KT";}


            XMLElement* elePhotoElec = doc.NewElement(str.toStdString().c_str());
            elePhotoElec->SetAttribute("COM",std::to_string(idx).c_str());



            XMLElement* eleCapAngel= genCapAngleXmlElementBlock(doc,idx);

            XMLElement* eleKickAngle=doc.NewElement("KickAngle");
            eleKickAngle->SetText(this->m_photoElecParamArr[idx].kickAngle);

            XMLElement* eleKickStep=doc.NewElement("KickStep");
            eleKickStep->SetText(this->m_photoElecParamArr[idx].kickStep);

            XMLElement* eleTrigAngle=doc.NewElement("TrigAngle");
            eleTrigAngle->SetText(this->m_photoElecParamArr[idx].trigAngle);

            XMLElement* eleSelfCheckAngle=doc.NewElement("SelfCheckAngle");
            eleSelfCheckAngle->SetText(this->m_photoElecParamArr[idx].selfCheckAngle);

            XMLElement* eleProbVal=genProbValXmlElementBlock(doc,idx);


            XMLElement* eleSendFaltSig=doc.NewElement("SendFaltSig");
            eleSendFaltSig->SetText(this->m_photoElecParamArr[idx].isSendFaltSig);

            XMLElement* eleProbRatio=doc.NewElement("ProbRatio");
            eleProbRatio->SetText(this->m_photoElecParamArr[idx].ratio);


            elePhotoElec->InsertEndChild(eleCapAngel);
            elePhotoElec->InsertEndChild(eleKickAngle);
            elePhotoElec->InsertEndChild(eleKickStep);
            elePhotoElec->InsertEndChild(eleTrigAngle);
            elePhotoElec->InsertEndChild(eleSelfCheckAngle);
            elePhotoElec->InsertEndChild(eleProbVal);
            elePhotoElec->InsertEndChild(eleSendFaltSig);
            elePhotoElec->InsertEndChild(eleProbRatio);


            elePhotoElecConfig->InsertEndChild(elePhotoElec);
        }

        doc.InsertEndChild(elePhotoElecConfig);
        XMLError xe= doc.SaveFile(name.c_str());
    }
    loadFromFile();
}


XMLElement * ClsPhototElecConfig::genCapAngleXmlElementBlock(TinyXmlDocument & doc,int index)
{
    QString str;
    if(index==1)str="QZCapAngle";
    if(index==2) str="KTCapAngle";

    XMLElement* eleCapAngle = doc.NewElement(str.toStdString().c_str());
    //eleCapAngle->SetAttribute("QZ", "");
    for(int i=1;i<=10;i++)
    {
        QString strCapAngle=QString("CapAngle_%1").arg(i);
        XMLElement* eletmp = doc.NewElement(strCapAngle.toStdString().c_str());

        eletmp->SetText(this->m_photoElecParamArr[index].capAngle[i-1]);
        eleCapAngle->InsertEndChild(eletmp);
    }
    return eleCapAngle;
}

XMLElement *ClsPhototElecConfig::genProbValXmlElementBlock(TinyXmlDocument &doc, int index)
{
    QString str;
    if(index==1)str="QZProbVal";
    if(index==2) str="KTProbVal";

    XMLElement* eleProb = doc.NewElement(str.toStdString().c_str());
    for(int i=1;i<=20;i++)
    {
        QString strCapAngle=QString("ProbVal_%1").arg(i);
        XMLElement* eletmp = doc.NewElement(strCapAngle.toStdString().c_str());

        eletmp->SetText(this->m_photoElecParamArr[index].probThreshold[i-1]);
        eleProb->InsertEndChild(eletmp);
    }
    return eleProb;
}

void ClsPhototElecConfig::save()
{
    QString path= DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig;
    savetoFile(path);
}

void ClsPhototElecConfig::savetoFile(QString dirPath)
{

    QString path=dirPath+"/PhotoElecConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();

    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    //const char* declaration ="<?FpcChecker version=\"1.0\" LastModifyTime=\"2020.12.21 11:11:11\">";
    TinyXmlDocument doc;
    doc.Parse(declaration.c_str());

    XMLElement* elePhotoElecConfig=doc.NewElement("PhotoElecConfig");

    QMap<int,StuPhotoElecParam>::iterator it=m_photoElecParamArr.begin();
    for(;it!=m_photoElecParamArr.end();it++)
    {
        int idx=it.key();


        QString str,strCom;
        if(idx==1){str="QZ";strCom="COM1";}
        if(idx==2) {str="KT";strCom="COM2";}


        XMLElement* elePhotoElec = doc.NewElement(str.toStdString().c_str());
        elePhotoElec->SetAttribute("COM",std::to_string(idx).c_str());



        XMLElement* eleCapAngel= genCapAngleXmlElementBlock(doc,idx);

        XMLElement* eleKickAngle=doc.NewElement("KickAngle");
        eleKickAngle->SetText(this->m_photoElecParamArr[idx].kickAngle);

        XMLElement* eleKickStep=doc.NewElement("KickStep");
        eleKickStep->SetText(this->m_photoElecParamArr[idx].kickStep);

        XMLElement* eleTrigAngle=doc.NewElement("TrigAngle");
        eleTrigAngle->SetText(this->m_photoElecParamArr[idx].trigAngle);

        XMLElement* eleSelfCheckAngle=doc.NewElement("SelfCheckAngle");
        eleSelfCheckAngle->SetText(this->m_photoElecParamArr[idx].selfCheckAngle);

        XMLElement* eleProbVal=genProbValXmlElementBlock(doc,idx);


        XMLElement* eleSendFaltSig=doc.NewElement("SendFaltSig");
        eleSendFaltSig->SetText(this->m_photoElecParamArr[idx].isSendFaltSig);

        XMLElement* eleProbRatio=doc.NewElement("ProbRatio");
        eleProbRatio->SetText(this->m_photoElecParamArr[idx].ratio);


        elePhotoElec->InsertEndChild(eleCapAngel);
        elePhotoElec->InsertEndChild(eleKickAngle);
        elePhotoElec->InsertEndChild(eleKickStep);
        elePhotoElec->InsertEndChild(eleTrigAngle);
        elePhotoElec->InsertEndChild(eleSelfCheckAngle);
        elePhotoElec->InsertEndChild(eleProbVal);
        elePhotoElec->InsertEndChild(eleSendFaltSig);
        elePhotoElec->InsertEndChild(eleProbRatio);

        elePhotoElecConfig->InsertEndChild(elePhotoElec);


    }

    //模拟板
    QMap<int,StuPhotoElecParamSim>::iterator itSim=m_photoElecParamArrSim.begin();
    for(;itSim!=m_photoElecParamArrSim.end();itSim++)
    {
        int idx=itSim.key();


        QString str,strCom;
        if(idx==1){str="QZSim";strCom="COMSim";}
        if(idx==2) {str="KTSim";strCom="COMSim";}


        XMLElement* elePhotoElec = doc.NewElement(str.toStdString().c_str());
        elePhotoElec->SetAttribute("COMSim",std::to_string(idx).c_str());


        XMLElement* eleCurrentVal=genCurrentXmlElementBlockSim(doc,idx);

        XMLElement* eleGainVal=genGainXmlElementBlockSim(doc,idx);

        XMLElement* eleProbVal=genProbValXmlElementBlockSim(doc,idx);



        XMLElement* eleProbRatio=doc.NewElement("ProbRatio");
        eleProbRatio->SetText(this->m_photoElecParamArrSim[idx].ratio);


        elePhotoElec->InsertEndChild(eleCurrentVal);
        elePhotoElec->InsertEndChild(eleGainVal);

        elePhotoElec->InsertEndChild(eleProbVal);
        elePhotoElec->InsertEndChild(eleProbRatio);

        elePhotoElecConfig->InsertEndChild(elePhotoElec);


    }


    doc.InsertEndChild(elePhotoElecConfig);
    XMLError xe= doc.SaveFile(name.c_str());
    DSDEBUG<<name.c_str()<<"  保存结果:"<<xe<<endl;
}

void ClsPhototElecConfig::createPhotoElecFile(QString dirname, int cameraNum)
{
    QString filename=dirname+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(filename).data();


    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    TinyXmlDocument doc;
    doc.LoadFile(name.c_str());
    doc.Parse(declaration.c_str());

    XMLElement* elePhotoElecConfig=doc.NewElement("PhotoElecConfig");

    DSClsOptions * options=DSClsOptions::getInstance();

    for(int idx=1;idx<=2;idx++)
    {
        QString str,strCom;
        if(idx==1){str="QZ";strCom="COM1";}
        if(idx==2) {str="KT";strCom="COM2";}


        XMLElement* elePhotoElec = doc.NewElement(str.toStdString().c_str());
        elePhotoElec->SetAttribute("COM",std::to_string(idx).c_str());


        XMLElement* eleCapAngel= genCapAngleXmlElementBlock(doc,idx);

        XMLElement* eleKickAngle=doc.NewElement("KickAngle");
        eleKickAngle->SetText(this->m_photoElecParamArr[idx].kickAngle);

        XMLElement* eleKickStep=doc.NewElement("KickStep");
        eleKickStep->SetText(this->m_photoElecParamArr[idx].kickStep);

        XMLElement* eleTrigAngle=doc.NewElement("TrigAngle");
        eleTrigAngle->SetText(this->m_photoElecParamArr[idx].trigAngle);

        XMLElement* eleSelfCheckAngle=doc.NewElement("SelfCheckAngle");
        eleSelfCheckAngle->SetText(this->m_photoElecParamArr[idx].selfCheckAngle);

        XMLElement* eleProbVal=genProbValXmlElementBlock(doc,idx);


        XMLElement* eleSendFaltSig=doc.NewElement("SendFaltSig");
        eleSendFaltSig->SetText(this->m_photoElecParamArr[idx].isSendFaltSig);


        elePhotoElec->InsertEndChild(eleCapAngel);
        elePhotoElec->InsertEndChild(eleKickAngle);
        elePhotoElec->InsertEndChild(eleKickStep);
        elePhotoElec->InsertEndChild(eleTrigAngle);
        elePhotoElec->InsertEndChild(eleSelfCheckAngle);
        elePhotoElec->InsertEndChild(eleProbVal);
        elePhotoElec->InsertEndChild(eleSendFaltSig);
        elePhotoElecConfig->InsertEndChild(elePhotoElec);
    }

    doc.InsertEndChild(elePhotoElecConfig);
    XMLError xe= doc.SaveFile(name.c_str());
}


void ClsPhototElecConfig::loadFromFile()
{
    m_photoElecParamArr.clear();

    m_photoElecParamArrSim.clear();

    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/PhotoElecConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();

    TinyXmlDocument doc;
    doc.LoadFile(name.c_str());

    XMLElement * root=doc.RootElement();
    //DSDEBUG<<root->Name()<<endl;

    XMLElement* eleTemp = root->FirstChildElement();
    while (eleTemp != NULL)
    {
        if (string(eleTemp->Name()) == "QZ")
        {
            StuPhotoElecParam stuPho;

            string strIndex=eleTemp->Attribute("COM");


            XMLElement* eleCap = eleTemp->FirstChildElement();
            std::string name=eleCap->Name();

            while (true)
            {
                if(QString(eleCap->Name())=="QZCapAngle")
                {
                    for(int i=1;i<=10;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("CapAngle_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleCap, strTmp);
                        stuPho.capAngle[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleCap = eleCap->NextSiblingElement();
            }

            XMLElement* eleProb = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleProb->Name())=="QZProbVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("ProbVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleProb, strTmp);
                        stuPho.probThreshold[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleProb = eleProb->NextSiblingElement();
            }


            string strKickAngle;
            XmlHelper::GetElementTextFromParent("KickAngle", eleTemp, strKickAngle);
            stuPho.kickAngle=std::atoi(strKickAngle.c_str());


            string strKickStep;
            XmlHelper::GetElementTextFromParent("KickStep", eleTemp, strKickStep);
            stuPho.kickStep=std::atoi(strKickStep.c_str());

            string strTrigAngle;
            XmlHelper::GetElementTextFromParent("TrigAngle", eleTemp, strTrigAngle);
            stuPho.trigAngle=std::atoi(strTrigAngle.c_str());


            string strSelfCheckAngle;
            XmlHelper::GetElementTextFromParent("SelfCheckAngle", eleTemp, strSelfCheckAngle);
            stuPho.selfCheckAngle=std::atoi(strSelfCheckAngle.c_str());


            string strSendFaltSig;
            XmlHelper::GetElementTextFromParent("SendFaltSig", eleTemp, strSendFaltSig);
            stuPho.isSendFaltSig=std::atoi(strSendFaltSig.c_str());

            string strProbRatio="";
            XmlHelper::GetElementTextFromParent("ProbRatio", eleTemp, strProbRatio);
            if(strProbRatio=="") strProbRatio="1.0";
            stuPho.ratio=std::atof(strProbRatio.c_str());



            m_photoElecParamArr[std::atoi(strIndex.c_str())]=stuPho;

        }


        if (string(eleTemp->Name()) == "KT")
        {
            StuPhotoElecParam stuPho;

            string strIndex=eleTemp->Attribute("COM");


            XMLElement* eleCap = eleTemp->FirstChildElement();
            std::string name=eleCap->Name();

            while (true)
            {
                if(string(eleCap->Name())=="KTCapAngle")
                {
                    for(int i=1;i<=10;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("CapAngle_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleCap, strTmp);
                        stuPho.capAngle[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleCap = eleCap->NextSiblingElement();
            }

            XMLElement* eleProb = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleProb->Name())=="KTProbVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("ProbVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleProb, strTmp);
                        stuPho.probThreshold[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleProb = eleProb->NextSiblingElement();
            }


            string strKickAngle;
            XmlHelper::GetElementTextFromParent("KickAngle", eleTemp, strKickAngle);
            stuPho.kickAngle=std::atoi(strKickAngle.c_str());


            string strKickStep;
            XmlHelper::GetElementTextFromParent("KickStep", eleTemp, strKickStep);
            stuPho.kickStep=std::atoi(strKickStep.c_str());

            string strTrigAngle;
            XmlHelper::GetElementTextFromParent("TrigAngle", eleTemp, strTrigAngle);
            stuPho.trigAngle=std::atoi(strTrigAngle.c_str());


            string strSelfCheckAngle;
            XmlHelper::GetElementTextFromParent("SelfCheckAngle", eleTemp, strSelfCheckAngle);
            stuPho.selfCheckAngle=std::atoi(strSelfCheckAngle.c_str());


            string strSendFaltSig;
            XmlHelper::GetElementTextFromParent("SendFaltSig", eleTemp, strSendFaltSig);
            stuPho.isSendFaltSig=std::atoi(strSendFaltSig.c_str());

            string strProbRatio="";
            XmlHelper::GetElementTextFromParent("ProbRatio", eleTemp, strProbRatio);
            if(strProbRatio=="") strProbRatio="1.0";
            stuPho.ratio=std::atof(strProbRatio.c_str());

            m_photoElecParamArr[std::atoi(strIndex.c_str())]=stuPho;
        }



        if (string(eleTemp->Name()) == "QZSim")
        {
            StuPhotoElecParamSim stuPho;

            string strIndex=eleTemp->Attribute("COMSim");

            XMLElement* eleCurrent = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleCurrent->Name())=="QZCurrentVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("CurrentVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleCurrent, strTmp);
                        stuPho.currentArr[i-1]=std::atof(strTmp.c_str());
                    }
                    break;
                }
                eleCurrent = eleCurrent->NextSiblingElement();
            }


            XMLElement* eleGain = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleGain->Name())=="QZGainVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("GainVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleGain, strTmp);
                        stuPho.gainArr[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleGain = eleGain->NextSiblingElement();
            }


            XMLElement* eleProb = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleProb->Name())=="QZProbVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("ProbVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleProb, strTmp);
                        stuPho.probThreshold[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleProb = eleProb->NextSiblingElement();
            }


            string strProbRatio="";
            XmlHelper::GetElementTextFromParent("ProbRatio", eleTemp, strProbRatio);
            if(strProbRatio=="") strProbRatio="1.0";
            stuPho.ratio=std::atof(strProbRatio.c_str());

            m_photoElecParamArrSim[std::atoi(strIndex.c_str())]=stuPho;
        }


        if (string(eleTemp->Name()) == "KTSim")
        {
            StuPhotoElecParamSim stuPho;

            string strIndex=eleTemp->Attribute("COMSim");

            XMLElement* eleCurrent = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleCurrent->Name())=="KTCurrentVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("CurrentVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleCurrent, strTmp);
                        stuPho.currentArr[i-1]=std::atof(strTmp.c_str());
                    }
                    break;
                }
                eleCurrent = eleCurrent->NextSiblingElement();
            }


            XMLElement* eleGain = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleGain->Name())=="KTGainVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("GainVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleGain, strTmp);
                        stuPho.gainArr[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleGain = eleGain->NextSiblingElement();
            }


            XMLElement* eleProb = eleTemp->FirstChildElement();
            while (true)
            {
                if(QString(eleProb->Name())=="KTProbVal")
                {
                    for(int i=1;i<=20;i++)
                    {
                        string strTmp;
                        QString strNodeName=QString("ProbVal_%1").arg(i);
                        XmlHelper::GetElementTextFromParent(strNodeName.toStdString().c_str(), eleProb, strTmp);
                        stuPho.probThreshold[i-1]=std::atoi(strTmp.c_str());
                    }
                    break;
                }
                eleProb = eleProb->NextSiblingElement();
            }





            string strProbRatio="";
            XmlHelper::GetElementTextFromParent("ProbRatio", eleTemp, strProbRatio);
            if(strProbRatio=="") strProbRatio="1.0";
            stuPho.ratio=std::atof(strProbRatio.c_str());

            m_photoElecParamArrSim[std::atoi(strIndex.c_str())]=stuPho;
        }



        eleTemp = eleTemp->NextSiblingElement();
    }
}
