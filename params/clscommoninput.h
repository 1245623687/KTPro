#ifndef CLSCOMMONINPUT_H
#define CLSCOMMONINPUT_H
#include<mutex>
#include"common/IniFile.h"
#include<vector>
#include<QString>
class ClsCommonInput
{
public:

   static ClsCommonInput* ClsCommonInput::getInstance()
    {
        m_Mutex.lock();
        if (m_Instance == nullptr) {
            m_Instance = new ClsCommonInput();
        }
        m_Mutex.unlock();
        return m_Instance;
    }

    std::vector<QString>& getProductName()
    {
        return  m_ProductName;
    }

    std::vector<QString>& getMachineNames()
    {
        return  m_MachineName;
    }

    void load();
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

    ClsCommonInput();

    static std::mutex m_Mutex;
    static ClsCommonInput* m_Instance;
    INIFile m_IniFile;
    std::vector<QString> m_ProductName;
    std::vector<QString> m_MachineName;

};

#endif // CLSCOMMONINPUT_H
