#ifndef KTHREADDATABASE_H
#define KTHREADDATABASE_H

#include <QObject>
#include"kthreadbase.h"

class KThreadDatabase : public KThreadBase
{
public:
    static KThreadDatabase* getInstance();

    void run();


    ~KThreadDatabase();
private:
    void stop(){

        KThreadBase::stop();
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
    static std::mutex m_Mutex;
    static KThreadDatabase* m_Instance;
    KThreadDatabase();

    std::mutex m_MutexSave;
};

#endif // KTHREADDATABASE_H
