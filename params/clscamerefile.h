#ifndef CLSCAMEREFILE_H
#define CLSCAMEREFILE_H
#include<mutex>
#include"common/filehelper.h"
#include"common/IniFile.h"
#include"dssystemparam.h"
#include"dssystemparam.h"
#include<QTextCodec>
class ClsCamereFile
{
public:
    static ClsCamereFile* getInstance()
    {
        m_Mutex.lock();
        if (m_Instance == nullptr) {
            m_Instance = new ClsCamereFile();
        }
        m_Mutex.unlock();


        return m_Instance;
    }


    int width()
    {
        return this->m_Wid;
    }

    int height()
    {
        return this->m_Hei;
    }

    void load()
    {
        QString path= QString("%1/camerafile.ccf").arg(DSSystemParam::AppPath);
        QFileInfo fi(path);
        if(!fi.exists())
        {
            FileHelper::createFile(DSSystemParam::AppPath+"/"+DSSystemParam::ParamsConfig,"lastConfig.ini");
        }

        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(path).data();
        this->m_IniFile.Open(name);

        bool b;
        std::string val;


        b=this->m_IniFile.GetValue("Stream Conditioning", "Crop Height", val);
        this->m_Hei=atoi(val.c_str());

        b=this->m_IniFile.GetValue("Stream Conditioning", "Crop Width", val);
        this->m_Wid=atoi(val.c_str());

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

    ClsCamereFile();

    static std::mutex m_Mutex;
    static ClsCamereFile* m_Instance;
    INIFile m_IniFile;

private:
    int m_Wid;
    int m_Hei;

};

#endif // CLSCAMEREFILE_H
