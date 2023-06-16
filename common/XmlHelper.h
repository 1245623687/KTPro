#pragma once


#include "tinyxml2.h"
#include<string>
#include<QtXml>
#include<QDomDocument>

using namespace tinyxml2;
using namespace std;

#define XMLHELPER_ERR_SUCESS 0
#define XMLHELPER_ERR_NULLELEMENT -1


class XmlHelper
{
public:
    //从父节点获取子节点的内容
    static int GetElementTextFromParent(string nodeName, XMLElement* pEleParent, string& innerText);

    static int GetElementTextFromParent2(QString nodeName, QDomElement& pEleParent, QString& innerText);

    //根据节点的绝对路径获取节点innertext
    static int GetElementTextByPath(string absNodeName, string xmlPath , string& innerText);

    static XMLElement* GetElementByParent(string nodeName, XMLElement* pEleParent);
    static QDomElement* GetElementByParent2(QString nodeName, QDomElement pEleParent);


private:

    string XmlPath;

};

