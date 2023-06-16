#ifndef SYSTEMRUNLOG_H
#define SYSTEMRUNLOG_H
#include<QObject>
#include<QQueue>
#include<mutex>
#include<QMutex>
#include<QDateTime>
#include"params/dssystemparam.h"
#include<QVector>
class SystemRunLog:public QObject
{
//public:
//    class ClsRunLog
//    {
//    public:
//        ClsRunLog();
//        ClsRunLog( QDateTime dateTime,QString info):
//            DateTime(dateTime),
//            Info(info)
//        {
//        }
//        QDateTime DateTime;
//        QString Info;
//    };

    Q_OBJECT


public:
    static SystemRunLog * getInstance();

    void pushback(QString info);
    //QQueue<ClsRunLog> getInfo();
    QQueue<QString> getInfo();
signals:
    void signalUpdate();


private:
    int m_NumShow;
    //QQueue<ClsRunLog> m_Strqueue;
    QQueue<QString> m_Strqueue;
    QMutex m_Qmutex;

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
    SystemRunLog();
    static std::mutex m_Mutex;
    static SystemRunLog* m_Instance;

};

#endif // SYSTEMRUNLOG_H
