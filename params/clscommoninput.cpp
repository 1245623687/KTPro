#include "clscommoninput.h"
#include<QTextCodec>

std::mutex ClsCommonInput::m_Mutex;
ClsCommonInput* ClsCommonInput::m_Instance = nullptr;
ClsCommonInput ::GC ClsCommonInput ::gc;

#include"params/dssystemparam.h"

ClsCommonInput::ClsCommonInput()
{
    load();
}


void ClsCommonInput::load()
{

    m_ProductName.clear();
    m_MachineName.clear();
    QString path=DSSystemParam::AppPath+"/settings.ini";
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name =code->fromUnicode(path).data();

    this->m_IniFile.Open(name);

    bool b;
    std::string val;

    b=this->m_IniFile.GetValue("Product", "ProductName", val);
    QStringList lstProNames= QString::fromStdString(val).split(" ");
    for(int idx=0;idx<lstProNames.size();++idx)
    {
        if(lstProNames[idx].trimmed()!="")
            m_ProductName.push_back(lstProNames[idx].trimmed());
    }


    b=this->m_IniFile.GetValue("Machine", "MachineName", val);
    QStringList lstMachineNames= QString::fromStdString(val).split(" ");
    for(int idx=0;idx<lstMachineNames.size();++idx)
    {
        if(lstMachineNames[idx].trimmed()!="")
            m_MachineName.push_back(lstMachineNames[idx].trimmed());
    }



}
