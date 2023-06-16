#include "kthreaddatabase.h"
#include"dsdebug.h"
#include"database/dsfilerecord.h"
std::mutex KThreadDatabase::m_Mutex;
KThreadDatabase* KThreadDatabase::m_Instance = nullptr;
KThreadDatabase ::GC KThreadDatabase ::gc;

KThreadDatabase::KThreadDatabase()
{

}

KThreadDatabase::~KThreadDatabase()
{
    QThread::msleep(100);
    DsFileRecord::updateRecord();
    stop();

}
void KThreadDatabase::run()
{
//    DSDEBUG<<"thread id:"<<GetCurrentThreadId()<<endl;

    int counttime=0;
    while(!isInterruptionRequested())
    {
        DSDEBUG<<"开始更新NG统计信息";
        DsFileRecord::updateRecord();
        DSDEBUG<<"完成更新NG统计信息";
        counttime=0;
        while(counttime++<300&&!isInterruptionRequested())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

KThreadDatabase* KThreadDatabase::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == nullptr)
    {
        m_Instance = new KThreadDatabase();
        m_Instance->start();
    }
    m_Mutex.unlock();
    return m_Instance;
}
