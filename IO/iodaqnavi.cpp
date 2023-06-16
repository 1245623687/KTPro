#include "iodaqnavi.h"

IODAQNavi::IODAQNavi()
{
    init();
}

int IODAQNavi::init()
{
    m_bolIsInitialize = false;
    instantDiCtrl = InstantDiCtrl::Create();
    instantDoCtrl = InstantDoCtrl::Create();
    Array<DeviceTreeNode> *supportedDevices = instantDoCtrl->getSupportedDevices();

    bool state = false;
    int i=supportedDevices->getCount();
    if (supportedDevices->getCount() == 0)
    {
        //QMessageBox::information(this, "提示", "No device to support the currently demonstrated function!");
        return -1;
    }
    else
    {
        for (int i = 0; i < supportedDevices->getCount(); i++)
        {
            DeviceTreeNode const &node = supportedDevices->getItem(i);
            //qDebug() << QString::fromWCharArray(node.Description);

            if(QString::fromWCharArray(node.Description) == "USB-4750,BID#0")
            {
                state = true;
            }
        }
        if(!state)
        {
            //QMessageBox::information(this, "提示", "未连接USB-4750,BID#0");
            return -2;
        }
    }

    //instantDioCtrl->Dispose();
    supportedDevices->Dispose();

    m_strIsDeviceName = "USB-4750,BID#0";
    std::wstring description = m_strIsDeviceName.toStdWString();
    DeviceInformation selected(description.c_str());

    ErrorCode errorCode = Success;
    errorCode = instantDiCtrl->setSelectedDevice(selected);
    CheckError(errorCode);
    errorCode = Success;
    errorCode = instantDoCtrl->setSelectedDevice(selected);
    CheckError(errorCode);
//    std::wstring profile = configure.profilePath.toStdWString();
//    errorCode = instantDioCtrl->LoadProfile(profile.c_str());
//    CheckError(errorCode);
    m_intIsIntPortCount = instantDiCtrl->getPortCount();
    m_intIsOutPortCount = instantDoCtrl->getPortCount();

    m_vecIsOutValue.resize(m_intIsOutPortCount);
    for (int i = 0; i < m_intIsOutPortCount; i++)
    {
        m_vecIsOutValue[i] = QVector<bool>(8, false);
    }

    m_vecIsIntValue.resize(m_intIsIntPortCount);
    for (int i = 0; i < m_intIsIntPortCount; i++)
    {
        m_vecIsIntValue[i] = QVector<bool>(8, false);
    }
    m_bolIsInitialize = true;

    return 0;
}

int IODAQNavi::unInitialize()
{
    return 0;
}

void IODAQNavi::CheckError(ErrorCode errorCode)
{
    if (BioFailed(errorCode))
    {
        QString message = "Sorry, there are some errors occurred, Error Code: 0x" +
            QString::number(errorCode, 16).right(8).toUpper();
        qDebug() << message;
    }
}

int IODAQNavi::getLevel(int iPin, int &pvalue)
{
    if(!m_bolIsInitialize)
    {
        return -1;
    }
    int port = 0;
    if(iPin > 7)
    {
        port = 1;
        iPin = iPin - 8;
    }
    quint8 portStates;
    ErrorCode errorCode = Success;
    errorCode = instantDiCtrl->Read(port, portStates);
    CheckError(errorCode);
    if(errorCode != Success)
    {
        return -1;
    }

    QByteArray arry = QByteArray::number(portStates, 2);
    if(arry.size() < 8)
    {
        arry.insert(0, "0");
    }

    QString str = QString("%1").arg(arry[7-iPin]);
    pvalue = str.toInt();
    return 0;
}

int IODAQNavi::setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    if(!m_bolIsInitialize)
    {
        return -1;
    }
    int port = 0;
    if(iPin > 7)
    {
        port = 1;
        iPin = iPin - 8;
    }
    bool Val = bool(setVal);
    m_vecIsOutValue[port][iPin] = !Val;
    quint8 count = 0;
    for(int i = 0; i < m_vecIsOutValue[port].size(); i++)
    {
        if(m_vecIsOutValue[port][i])
        {
            int n = pow(2,i);
            count += n;
        }
    }

    //qDebug() << QString("port:%1, key:%2, status:%3").arg(port).arg(index).arg(QString("%1").arg(count,0,16));
    ErrorCode errorCode = Success;
    errorCode = instantDoCtrl->Write(port, 1, &count);
    CheckError(errorCode);
    return 0;
}


int IODAQNavi::getDirection(int iPin, int &pvalue)
{
    return 0;
}

int IODAQNavi::setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    return 0;
}
