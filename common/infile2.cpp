#include "infile2.h"

InFile2::InFile2(QObject *parent) : QObject(parent)
{
    m_bCreated=false;
}

bool InFile2::Save(QString filename)
{

    return true;
}

bool InFile2::Save()
{

    return  true;
}

bool InFile2::Open(QString filename)
{
    if(filename=="")
    {
        return false;
    }
    QFile qfi(filename);
    qfi.open(QIODevice::ReadWrite);
    if(!qfi.isOpen())
    {
        return  false;
    }
    qfi.close();
    m_bCreated=true;
    this->m_initPath=filename;
    return  true;



}

bool InFile2::GetValue(QString strSection, QString varName, QString &varvalue)
{
    if(!m_bCreated)
    {
        return false;
    }
    QSettings qsetting(this->m_initPath,QSettings::IniFormat);
    qsetting.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString defaultValue="default";
    varvalue= qsetting.value(strSection+"/"+varName,defaultValue).toString();

    if(varvalue=="default")
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool InFile2::SetValue(QString strSection, QString varName, QString varvalue, bool breplace)
{
    if(!m_bCreated)
    {
        return false;
    }
    QSettings qsetting(this->m_initPath,QSettings::IniFormat);
    qsetting.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QString defaultValue="defalut";
    qsetting.setValue(strSection+"/"+varName,varvalue);
    return true;
}

InFile2::InFile2(QString filename)
{
    m_bCreated=false;
    Open(filename);
    this->m_initPath=filename;
}
