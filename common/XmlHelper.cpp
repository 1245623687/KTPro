#include "XmlHelper.h"
#include <vector>

int XmlHelper::GetElementTextFromParent(string nodeName, XMLElement* pEleParent, string& innerText)
{
    XMLElement * curEle = pEleParent->FirstChildElement(nodeName.c_str());
    if (curEle == NULL) return XMLHELPER_ERR_NULLELEMENT;
    string ss=curEle->Value();

    if (curEle->GetText())
    {
        innerText = curEle->GetText();
    }
    else innerText = "0";
    return XMLHELPER_ERR_SUCESS;
}

int XmlHelper::GetElementTextFromParent2(QString nodeName, QDomElement &pEleParent, QString &innerText)
{
    QDomElement  curEle = pEleParent.firstChildElement(nodeName);
    if (curEle.isNull())
        return XMLHELPER_ERR_NULLELEMENT;
    QString ss=curEle.text();

    if (!curEle.text().isEmpty())
    {
        innerText = curEle.text();
    }
    else innerText = "0";
    return XMLHELPER_ERR_SUCESS;
}

vector<string> splitString(const string &str, const string &pattern)
{
    vector<string> resVec;
    if ("" == str)return resVec;
    std::string strs = str + pattern;
    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != std::string::npos)
    {
        std::string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(pattern);
    }
    return resVec;
}

int XmlHelper::GetElementTextByPath(string absNodeName, string xmlPath,string& innerText)
{
    TinyXmlDocument doc;
    doc.LoadFile(xmlPath.c_str());
    vector<string> vstr = splitString(absNodeName, "/");
    XMLElement * curEle = doc.FirstChildElement((vstr[0]).c_str());
    if (vstr.size()==1)
    {
        if (curEle == NULL) return XMLHELPER_ERR_NULLELEMENT;
        innerText = curEle->GetText();
        return XMLHELPER_ERR_SUCESS;
    }
    vector<string>::const_iterator iter = vstr.cbegin();
    iter++;
    for (;iter != vstr.cend(); iter++)
    {
        curEle = curEle->FirstChildElement((*iter).c_str());
        if (curEle==NULL) return XMLHELPER_ERR_NULLELEMENT;
    }
    innerText = curEle->GetText();
    return XMLHELPER_ERR_SUCESS;
}


XMLElement* XmlHelper::GetElementByParent(string nodeName, XMLElement* pEleParent)
{
    return pEleParent->FirstChildElement(nodeName.c_str());
}

QDomElement* XmlHelper::GetElementByParent2(QString nodeName, QDomElement pEleParent)
{

     pEleParent.firstChildElement(nodeName);
     QDomElement *pele=new QDomElement(pEleParent);
     return pele;
}

