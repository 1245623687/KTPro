#include "systemrunlog.h"

std::mutex SystemRunLog::m_Mutex;
SystemRunLog* SystemRunLog::m_Instance = nullptr;
SystemRunLog ::GC SystemRunLog ::gc;

SystemRunLog::SystemRunLog()
{
    m_NumShow=100;



}


void SystemRunLog::pushback(QString info)
{
    //如果在检测线程中调用，就不要在别的线程中大量调用了，防止竞争，影响检测效率
    m_Qmutex.lock();
    while(m_Strqueue.size()>m_NumShow-1)
        m_Strqueue.dequeue();
    //m_Strqueue.push_back(ClsRunLog(QDateTime::currentDateTime(),info));
    m_Strqueue.push_back(QString("->%1:%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hh:mm:ss")).arg(info));
    m_Qmutex.unlock();
    emit signalUpdate();
}

//QQueue<SystemRunLog::ClsRunLog> SystemRunLog::getInfo()
//{
//    return this->m_Strqueue;
//}

QQueue<QString> SystemRunLog::getInfo()
{
     m_Qmutex.lock();
     QQueue<QString> ret=this->m_Strqueue;
     m_Qmutex.unlock();
     return  ret;
}


SystemRunLog* SystemRunLog::getInstance()
{
    m_Mutex.lock();
    if (m_Instance == nullptr) {
        m_Instance = new SystemRunLog();
    }
    m_Mutex.unlock();
    return m_Instance;
}
