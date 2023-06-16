#include "dslastconfig.h"
#include<QFileInfo>
#include<QTextCodec>
std::mutex DSClsLastConfig::m_Mutex;
DSClsLastConfig* DSClsLastConfig::m_Instance = nullptr;
DSClsLastConfig ::GC DSClsLastConfig ::gc;

DSClsLastConfig* DSClsLastConfig::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == nullptr) {
        m_Instance = new DSClsLastConfig();
    }
    m_Mutex.unlock();
    return m_Instance;
}


DSClsLastConfig::DSClsLastConfig()
{
    //load();
}

void DSClsLastConfig::load()
{
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/lastConfig.ini";
    QFileInfo fi(path);
    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"lastConfig.ini");
    }

    QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = code->fromUnicode(path).data();
    //std::string name=path.toStdString();

    //this->m_IniFile.Open(name);

    this-> m_IniFile2.Open(path);
    bool b;

   // std::string val;
    QString val;
    b=this->m_IniFile2.GetValue("Param", "LastBrand", val);
    if (b) setLastBrand(val);
    else setLastBrand("Default");
    val="";
    b = this->m_IniFile2.GetValue("Param", "LastShift", val);
    if (b) setLastShift(static_cast<ENUMSHIFT>(val.toInt()));
    else setLastShift(ENUMSHIFT::ENUMSHIFT_NULL);

    b = this->m_IniFile2.GetValue("Param", "CheckNum", val);
    if (b) setLastCheckNum((val.toInt()));
    else setLastCheckNum(0);

    b = this->m_IniFile2.GetValue("Param", "NgNum", val);
    if (b)setLastNgNum((val.toInt()));
    else setLastNgNum(0);

    b = this->m_IniFile2.GetValue("Param", "Date", val);
    if (b)setDate(QDate::fromString(val,"yyyyMMdd"));
    else setDate(QDate::currentDate());

}



void DSClsLastConfig::saveBackUp()
{
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/lastConfig(backup).ini";
    QFileInfo fi(path);
    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"lastConfig(backup).ini");
    }

   // QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = path.toStdString();
    this->m_IniFile2.Open(path);

    m_IniFile2.SetValue("Param", "LastBrand", this->lastBrand());
    m_IniFile2.SetValue("Param", "LastShift",QString::number((int)this->lastShift()));
    m_IniFile2.SetValue("Param", "CheckNum",QString::number(this->lastCheckNum()));
    m_IniFile2.SetValue("Param", "NgNum", QString::number(this->lastNgNum()));
    m_IniFile2.SetValue("Param", "Date",this->m_Date);

    m_IniFile2.Save();
}



void DSClsLastConfig::save()
{
    QString path=DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig+"/lastConfig.ini";
    QFileInfo fi(path);
    if(!fi.exists())
    {
        FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"lastConfig.ini");
    }

   // QTextCodec *code = QTextCodec::codecForName("GB2312");
    std::string name = path.toStdString();
    this->m_IniFile2.Open(path);

    m_IniFile2.SetValue("Param", "LastBrand", this->lastBrand());
    m_IniFile2.SetValue("Param", "LastShift",QString::number((int)this->lastShift()));
    m_IniFile2.SetValue("Param", "CheckNum",QString::number(this->lastCheckNum()));
    m_IniFile2.SetValue("Param", "NgNum", QString::number(this->lastNgNum()));
    m_IniFile2.SetValue("Param", "Date",this->m_Date);

    m_IniFile2.Save();
}
