#ifndef IODAQNAVI_H
#define IODAQNAVI_H

//#if _MSC_VER >= 1600
//#pragma execution_character_set("utf-8")
//#endif

#include <QObject>
#include <QDebug>
#include "ciobase.h"
#include "bdaqctrl.h"

using namespace Automation::BDaq;

class IODAQNavi : public CIOBase
{
public:
    IODAQNavi();

    int init() override;

    int unInitialize() override;

    int getDirection(int iPin, int &pvalue) override;

    int getLevel(int iPin, int &pvalue) override;



    int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal) override;
    int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);


private:
    void CheckError(ErrorCode errorCode);

private:
    InstantDiCtrl * instantDiCtrl;
    InstantDoCtrl* instantDoCtrl;
    QString m_strIsDeviceName;
    int m_intIsOutPortCount;
    int m_intIsIntPortCount;

    QVector<QVector<bool>> m_vecIsOutValue;
    QVector<QVector<bool>> m_vecIsIntValue;
    bool m_bolIsInitialize;
};

#endif // IODAQNAVI_H
