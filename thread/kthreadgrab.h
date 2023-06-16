#ifndef KTHREADGRAB_H
#define KTHREADGRAB_H

#include <QObject>
#include"kthreadbase.h"
class KThreadGrab : public KThreadBase
{
    Q_OBJECT

public:
    KThreadGrab();
    void run() override;
    void setCameraIdx(int idx)
    {
        this->cameraIdx=idx;
    }
signals:
    void inforUpdate();

private:
    int cameraIdx=1;
};

#endif // KTHREADGRAB_H
