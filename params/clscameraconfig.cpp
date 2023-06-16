#include "clscameraconfig.h"
#include<QString>
#include<QFileInfo>
#include<QTextCodec>
#include<QDateTime>
#include"common/filehelper.h"
#include"params/dssystemparam.h"
#include"params/dsoptions.h"
#include"dsdebug.h"

std::mutex ClsCameraConfig::m_Mutex;
ClsCameraConfig* ClsCameraConfig::m_Instance = nullptr;
ClsCameraConfig ::GC ClsCameraConfig ::gc;
ClsCameraConfig::ClsCameraConfig()
{
    load();
}

void ClsCameraConfig::loadFromFile()
{
    m_CameraParamArr.clear();
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();

    TinyXmlDocument doc;
    doc.LoadFile(name.c_str());

    XMLElement * root=doc.RootElement();
    //DSDEBUG<<root->Name()<<endl;

    XMLElement* eleTemp = root->FirstChildElement();
    while (eleTemp != NULL)
    {
        if (string(eleTemp->Name()) == "Camera")
        {
            string strIndex=eleTemp->Attribute("index");

            string strIpAddress;
            XmlHelper::GetElementTextFromParent("IPAddress", eleTemp, strIpAddress);

            string strExposeTime;
            XmlHelper::GetElementTextFromParent("ExposureTime", eleTemp, strExposeTime);

            string strOffset;
            XmlHelper::GetElementTextFromParent("Offset", eleTemp, strOffset);

            string strWB_R;
            XmlHelper::GetElementTextFromParent("WB_R", eleTemp, strWB_R);

            string strWB_G;
            XmlHelper::GetElementTextFromParent("WB_G", eleTemp, strWB_G);

            string strWB_B;
            XmlHelper::GetElementTextFromParent("WB_B", eleTemp, strWB_B);

            string strLightCode;
            XmlHelper::GetElementTextFromParent("Light_Code", eleTemp, strLightCode);

           StuCameraParam tmpStu= StuCameraParam(QString::fromStdString(strIpAddress),
                                                                  std::atoi(strExposeTime.c_str()),
                                                                  std::atoi(strOffset.c_str()),
                                                                  std::atoi(strWB_R.c_str()),
                                                                  std::atoi(strWB_G.c_str()),
                                                                  std::atoi(strWB_B.c_str()),
                                                                  (unsigned int)std::atoi(strLightCode.c_str()));
           m_CameraParamArr[std::atoi(strIndex.c_str())]=tmpStu;
//            m_CameraParamArr[std::atoi(strIndex.c_str())]=StuCameraParam(QString::fromStdString(strIpAddress),
//                                                      std::atoi(strExposeTime.c_str()),
//                                                      std::atoi(strOffset.c_str()),
//                                                      std::atoi(strWB_R.c_str()),
//                                                      std::atoi(strWB_G.c_str()),
//                                                      std::atoi(strWB_B.c_str()));
        }
//        if (string(eleTemp->Name()) == "AllLightCode")
//        {
//            string allLightCode=eleTemp->GetText();
//        }
        eleTemp = eleTemp->NextSiblingElement();
    }

}


void ClsCameraConfig::savetoFile(QString dirPath)
{

    QString path=dirPath+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();

    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    //const char* declaration ="<?FpcChecker version=\"1.0\" LastModifyTime=\"2020.12.21 11:11:11\">";
    TinyXmlDocument doc;
    doc.Parse(declaration.c_str());

    XMLElement* eleCameraConfig=doc.NewElement("CameraConfig");

    QMap<int,StuCameraParam>::iterator it=m_CameraParamArr.begin();
    for(;it!=m_CameraParamArr.end();it++)
    {
       int tmpIndex=it.key();
       XMLElement* eleCamera = doc.NewElement("Camera");
       eleCamera->SetAttribute("index",std::to_string(tmpIndex).c_str());

       XMLElement* eleIPAddress= doc.NewElement("IPAddress");
       eleIPAddress->SetText(it.value().IPAddress.toStdString().c_str());
       XMLElement* eleExposureTime= doc.NewElement("ExposureTime");
       eleExposureTime->SetText(QString::number(it.value().ExposeTime).toStdString().c_str());
       XMLElement* eleOffset= doc.NewElement("Offset");
       eleOffset->SetText(QString::number(it.value().Offset).toStdString().c_str());
       XMLElement* eleWB_R= doc.NewElement("WB_R");
       eleWB_R->SetText(it.value().WB_R);
       XMLElement* eleWB_G= doc.NewElement("WB_G");
       eleWB_G->SetText(it.value().WB_G);
       XMLElement* eleWB_B= doc.NewElement("WB_B");
       eleWB_B->SetText(it.value().WB_B);

       XMLElement* eleLight_Code= doc.NewElement("Light_Code");
       eleLight_Code->SetText(it.value().Light_Code);

       eleCamera->InsertEndChild(eleIPAddress);
       eleCamera->InsertEndChild(eleExposureTime);
       eleCamera->InsertEndChild(eleOffset);
       eleCamera->InsertEndChild(eleWB_R);
       eleCamera->InsertEndChild(eleWB_G);
       eleCamera->InsertEndChild(eleWB_B);
       eleCamera->InsertEndChild(eleLight_Code);


       eleCameraConfig->InsertEndChild(eleCamera);
    }


    doc.InsertEndChild(eleCameraConfig);
    XMLError xe= doc.SaveFile(name.c_str());
    DSDEBUG<<name.c_str()<<"  保存结果:"<<xe<<endl;

}



void ClsCameraConfig::save()
{
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();

    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    //const char* declaration ="<?FpcChecker version=\"1.0\" LastModifyTime=\"2020.12.21 11:11:11\">";
    TinyXmlDocument doc;
    doc.Parse(declaration.c_str());

    XMLElement* eleCameraConfig=doc.NewElement("CameraConfig");

    QMap<int,StuCameraParam>::iterator it=m_CameraParamArr.begin();
    for(;it!=m_CameraParamArr.end();it++)
    {
       int tmpIndex=it.key();
       XMLElement* eleCamera = doc.NewElement("Camera");
       eleCamera->SetAttribute("index",std::to_string(tmpIndex).c_str());

       XMLElement* eleIPAddress= doc.NewElement("IPAddress");
       eleIPAddress->SetText(it.value().IPAddress.toStdString().c_str());
       XMLElement* eleExposureTime= doc.NewElement("ExposureTime");
//       eleExposureTime->SetText(QString::number(it.value().ExposeTime,'f',1).toStdString().c_str());
       eleExposureTime->SetText(QString::number(it.value().ExposeTime).toStdString().c_str());
       XMLElement* eleOffset= doc.NewElement("Offset");
//       eleOffset->SetText(QString::number(it.value().Offset,'f',1).toStdString().c_str());
       eleOffset->SetText(QString::number(it.value().Offset).toStdString().c_str());
       XMLElement* eleWB_R= doc.NewElement("WB_R");
       eleWB_R->SetText(it.value().WB_R);
       XMLElement* eleWB_G= doc.NewElement("WB_G");
       eleWB_G->SetText(it.value().WB_G);
       XMLElement* eleWB_B= doc.NewElement("WB_B");
       eleWB_B->SetText(it.value().WB_B);

       XMLElement* eleLight_Code= doc.NewElement("Light_Code");
       eleLight_Code->SetText(it.value().Light_Code);

       eleCamera->InsertEndChild(eleIPAddress);
       eleCamera->InsertEndChild(eleExposureTime);
       eleCamera->InsertEndChild(eleOffset);
       eleCamera->InsertEndChild(eleWB_R);
       eleCamera->InsertEndChild(eleWB_G);
       eleCamera->InsertEndChild(eleWB_B);
       eleCamera->InsertEndChild(eleLight_Code);
       eleCameraConfig->InsertEndChild(eleCamera);
    }


    doc.InsertEndChild(eleCameraConfig);
    XMLError xe= doc.SaveFile(name.c_str());
    DSDEBUG<<name.c_str()<<"  保存结果:"<<xe<<endl;
}


void ClsCameraConfig::createCameraConfigFile(QString dirname,int cameraNum)
{
    QString filename=dirname+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(filename).data();



    //FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName,"CameraConfig.xml");

    std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
    TinyXmlDocument doc;
    doc.LoadFile(name.c_str());
    doc.Parse(declaration.c_str());

    XMLElement* eleCameraConfig=doc.NewElement("CameraConfig");

    for(int idx=1;idx<=cameraNum;idx++)
    {
        XMLElement* eleCamera = doc.NewElement("Camera");
        eleCamera->SetAttribute("index",std::to_string(idx).c_str());
        XMLElement* eleIpAddress=doc.NewElement("IPAddress");
        eleIpAddress->SetText((std::string("169.254.0.")+std::to_string(idx)).c_str());
        XMLElement* eleExposureTime=doc.NewElement("ExposureTime");
        eleExposureTime->SetText("20");
        XMLElement* eleOffset=doc.NewElement("Offset");
        eleOffset->SetText("1");
        XMLElement* eleWB_R=doc.NewElement("WB_R");
        eleWB_R->SetText(128);
        XMLElement* eleWB_G=doc.NewElement("WB_G");
        eleWB_G->SetText(128);
        XMLElement* eleWB_B=doc.NewElement("WB_B");
        eleWB_B->SetText(128);
        XMLElement* eleLight_Code=doc.NewElement("Light_Code");
        eleLight_Code->SetText(1);

        eleCamera->InsertEndChild(eleIpAddress);
        eleCamera->InsertEndChild(eleExposureTime);
        eleCamera->InsertEndChild(eleOffset);
        eleCamera->InsertEndChild(eleWB_R);
        eleCamera->InsertEndChild(eleWB_G);
        eleCamera->InsertEndChild(eleWB_B);
        eleCamera->InsertEndChild(eleLight_Code);

        eleCameraConfig->InsertEndChild(eleCamera);
    }
    doc.InsertEndChild(eleCameraConfig);
    XMLError xe= doc.SaveFile(name.c_str());
}

void ClsCameraConfig::load()
{
    if(DSSystemParam::BrandName=="Default") return;

    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName+"/CameraConfig.xml";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();
    QFileInfo fi(path);
    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/"+DSSystemParam::BrandDirPath+"/"+DSSystemParam::BrandName,"CameraConfig.xml");

        std::string declaration="<?FpcChecker version=\"1.0\" LastModifyTime=\""+QDateTime ::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+"\"?>";
        //DSDEBUG<<"declaration:"<<QString::fromStdString(declaration)<<endl;
        TinyXmlDocument doc;
        doc.LoadFile(name.c_str());
        doc.Parse(declaration.c_str());

        XMLElement* eleCameraConfig=doc.NewElement("CameraConfig");

        DSClsOptions * options=DSClsOptions::getInstance();
        for(int idx=1;idx<=options->MaxCameraNum();idx++)
        {
            XMLElement* eleCamera = doc.NewElement("Camera");
            eleCamera->SetAttribute("index",std::to_string(idx).c_str());

            XMLElement* eleIpAddress=doc.NewElement("IPAddress");
            eleIpAddress->SetText((std::string("192.168.100.")+std::to_string(idx)).c_str());
            XMLElement* eleExposureTime=doc.NewElement("ExposureTime");
            eleExposureTime->SetText(20);
            XMLElement* eleOffset=doc.NewElement("Offset");
            eleOffset->SetText(1);
            XMLElement* eleWB_R=doc.NewElement("WB_R");
            eleWB_R->SetText(128);
            XMLElement* eleWB_G=doc.NewElement("WB_G");
            eleWB_G->SetText(128);
            XMLElement* eleWB_B=doc.NewElement("WB_B");
            eleWB_B->SetText(128);
            XMLElement* eleLight_Code=doc.NewElement("Light_Code");
            eleLight_Code->SetText(1);


            eleCamera->InsertEndChild(eleIpAddress);
            eleCamera->InsertEndChild(eleExposureTime);
            eleCamera->InsertEndChild(eleOffset);
            eleCamera->InsertEndChild(eleWB_R);
            eleCamera->InsertEndChild(eleWB_G);
            eleCamera->InsertEndChild(eleWB_B);
            eleCamera->InsertEndChild(eleLight_Code);

            eleCameraConfig->InsertEndChild(eleCamera);
        }
        doc.InsertEndChild(eleCameraConfig);
        XMLError xe= doc.SaveFile(name.c_str());
    }
    loadFromFile();
}




ClsCameraConfig* ClsCameraConfig::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == NULL) {
        m_Instance = new ClsCameraConfig();
    }
    m_Mutex.unlock();
    return m_Instance;
}
