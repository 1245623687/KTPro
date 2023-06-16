#ifndef KTHREADBASE_H
#define KTHREADBASE_H

#include<QThread>


class KThreadBase:public QThread
{

public:
    KThreadBase();

    virtual ~KThreadBase(){
        stop();
    }

   virtual void stop();
private:
    bool m_Exist;
};

#endif // KTHREADBASE_H
