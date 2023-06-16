#ifndef KTHREADPROC_H
#define KTHREADPROC_H

#include <QObject>
#include"kthreadbase.h"
#include"packagechecker.h"
#include<mutex>
class KThreadProc : public KThreadBase
{
    Q_OBJECT
signals:
    void inforUpdate(int cameraidx);
    void inforUpdateRect();
    void sigupdateGPIO(int ,int);

    void sigCamOffLine(int camIndex);
public:
    KThreadProc();

    void run() override;

    void setCamera(int idx)
    {
        IdxCamera=idx;
    }

    ~KThreadProc()
    {

      //  PackageChecker::getInstance()->IOContol->setLevel(RUNPIN,0xFFFFFFFF,0xFF);
        stop();
    }
    
        int  getLogicRet(int,int);
private:
   int IdxCamera=1;
   static std::mutex m_Mutex;


};

#endif // KTHREADPROC_H
