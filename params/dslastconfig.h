#ifndef DSLASTCONFIG_H
#define DSLASTCONFIG_H
#include<QString>
#include<QDate>
#include<mutex>
#include"common/filehelper.h"
#include"common/IniFile.h"
#include"common/infile2.h"
#include"dssystemparam.h"
class DSClsLastConfig
{
public:

    static DSClsLastConfig* getInstance();

    void save();
    void saveBackUp();
    void load();

    QString lastBrand(){return this->m_LastBrand;}
    void setLastBrand(QString brandName){this->m_LastBrand=brandName;}

    ENUMSHIFT lastShift(){return this->m_LastShift;}
    void setLastShift(ENUMSHIFT shift){this->m_LastShift=shift;}

    int lastCheckNum(){return this->m_LastCheckNum;}
    void setLastCheckNum(double num){this->m_LastCheckNum=num;}

    int lastNgNum(){return this->m_LastNGNum;}
    void setLastNgNum(double num ){
        this->m_LastNGNum=num; }

    QDate date() {return  QDate::fromString(this->m_Date,"yyyyMMdd");}
    void setDate(QDate date)
    {
        this->m_Date=date.toString("yyyyMMdd");
    }



    ~DSClsLastConfig()
    {

    }

private:
    class GC
    {
    public :
        ~GC()
        {

            if (m_Instance != nullptr )
            {
                delete m_Instance;
                m_Instance = nullptr ;
            }
        }
    };
    static GC gc;

private:

    DSClsLastConfig();

    static std::mutex m_Mutex;
    static DSClsLastConfig* m_Instance;
    INIFile m_IniFile;

    InFile2 m_IniFile2;
    QString m_LastBrand;
    ENUMSHIFT m_LastShift;
    int m_LastCheckNum;
    int m_LastNGNum;
    QString m_Date;

};

#endif // DSLASTCONFIG_H
