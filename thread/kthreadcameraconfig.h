#ifndef KTHREADCAMERACONFIG_H
#define KTHREADCAMERACONFIG_H

#include <QObject>
#include"kthreadbase.h"

class KThreadCameraConfig : public KThreadBase
{
    Q_OBJECT
public:
    KThreadCameraConfig(){}
    void run() override;
    void setCurCameraNo(int idx)
    {
        m_CurCameraNo=idx;
    }
private:
    int m_CurCameraNo=1;
signals:
    void inforUpdate();
};

#endif // KTHREADCAMERACONFIG_H
