#include "basecom.h"
#include"dsdebug.h"


BaseCom*  BaseCom::m_instance=NULL;
QMutex BaseCom::mutex;

BaseCom *BaseCom::getInstance()
{
    mutex.lock();
    if(m_instance==NULL)
    {
        m_instance=new BaseCom();
    }
    mutex.unlock();
    return m_instance;
}

BaseCom::BaseCom(QObject *parent) : QObject(parent)
{

    m_waitTime=50;
    m_strQZPortName="COM1";
    m_strKTPortName="COM2";


#ifdef FALG_PHE
    connect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    connect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);
#endif


    m_cnt=0;
    m_cnt2=0;

}

BaseCom::~BaseCom()
{
    if(m_QZPort.isOpen())
    {
        m_QZPort.close();
    }
    if(m_KTPort.isOpen())
    {
        m_KTPort.close();
    }
}

bool BaseCom::openQZComPort(QString qzPortName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    bool ret=true;
    m_QZPort.setPortName(m_strQZPortName);
    m_QZPort.setBaudRate(baudRate);
    m_QZPort.setDataBits(dataBits);
    m_QZPort.setStopBits(stopBits);
    m_QZPort.setParity(parity);
    m_QZPort.setFlowControl(flowControl);

    if(!m_QZPort.open(QIODevice::ReadWrite))
    {
        ret=false;
    }
    return ret;
}

bool BaseCom::openKTComPort(QString ktPortName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    bool ret=true;
    m_KTPort.setPortName(m_strKTPortName);
    m_KTPort.setBaudRate(baudRate);
    m_KTPort.setDataBits(dataBits);
    m_KTPort.setStopBits(stopBits);
    m_KTPort.setParity(parity);
    m_KTPort.setFlowControl(flowControl);

    if(!m_KTPort.open(QIODevice::ReadWrite))
    {
        ret=false;
    }
    return ret;
}

bool BaseCom::openQZComPortSim(QString qzPortName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    bool ret=true;
    m_QZPortSim.setPortName(m_strQZPortNameSim);
    m_QZPortSim.setBaudRate(baudRate);
    m_QZPortSim.setDataBits(dataBits);
    m_QZPortSim.setStopBits(stopBits);
    m_QZPortSim.setParity(parity);
    m_QZPortSim.setFlowControl(flowControl);

    if(!m_QZPortSim.open(QIODevice::ReadWrite))
    {
        ret=false;
    }
    return ret;
}

bool BaseCom::openKTComPortSim(QString ktPortName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    bool ret=true;
    m_KTPortSim.setPortName(m_strKTPortNameSim);
    m_KTPortSim.setBaudRate(baudRate);
    m_KTPortSim.setDataBits(dataBits);
    m_KTPortSim.setStopBits(stopBits);
    m_KTPortSim.setParity(parity);
    m_KTPortSim.setFlowControl(flowControl);

    if(!m_KTPortSim.open(QIODevice::ReadWrite))
    {
        ret=false;
    }
    return ret;
}

int  BaseCom::openComPort()
{
    bool ret=false;
    ret=openQZComPort();
    if(!ret) return 1;

    ret=openKTComPort();
    if(!ret) return 2;

    ret=openQZComPortSim();
    if(!ret) return 3;

    ret=openKTComPortSim();
    if(!ret) return 4;

    return 0;
}

bool BaseCom::sendQZCommand(QByteArray strCommand, QByteArray &receivedByArray)
{
    bool ret=true;

    //m_ByContainer.clear();

    int sendLen=  m_QZPort.write(strCommand);
    if(!m_QZPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_QZPort.flush();


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_QZPort.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;



    receivedByArray=m_QZPort.readAll();


    DSDEBUG_<<"Test Modle Send Command,QZ, "<<strCommand<<"Get Response QZ, "<<receivedByArray<<endl;

    receivedByArray.truncate(16);
    if(receivedByArray==strCommand)
        ret=true;
    else
        ret=false;


    //ret= checkSlaveFeedBack(0,strCommand,receivedByArray);
    qDebug()<<"sendLen,"<<sendLen<<endl;
    return ret;
}

int BaseCom::sendKTCommand(QByteArray strCommand, QByteArray &receivedByArray)
{
    bool ret=true;

    int sendLen=  m_KTPort.write(strCommand);
    if(!m_KTPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_KTPort.flush();


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_KTPort.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;


    receivedByArray=m_KTPort.readAll();

    DSDEBUG_<<"Test Modle Send Command,KT, "<<strCommand<<"Get Response KT, "<<receivedByArray<<endl;

    receivedByArray.truncate(16);
    if(receivedByArray==strCommand)
        ret=true;
    else
        ret=false;

    return ret;
}

bool BaseCom::sendQZCommandSim(QByteArray strCommand, QByteArray &receivedByArray)
{
    bool ret=true;

    //m_ByContainer.clear();

    int sendLen=  m_QZPortSim.write(strCommand);
    if(!m_QZPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_QZPortSim.flush();


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_QZPortSim.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;



    receivedByArray=m_QZPortSim.readAll();


    DSDEBUG_<<"Test Modle Send Command,QZSim, "<<strCommand<<"Get Response QZSim, "<<receivedByArray<<endl;

    receivedByArray.truncate(16);
    if(receivedByArray==strCommand)
        ret=true;
    else
        ret=false;


    //ret= checkSlaveFeedBack(0,strCommand,receivedByArray);
    qDebug()<<"sendLen,"<<sendLen<<endl;
    return ret;
}

int BaseCom::sendKTCommandSim(QByteArray strCommand, QByteArray &receivedByArray)
{
    bool ret=true;

    int sendLen=  m_KTPortSim.write(strCommand);
    if(!m_KTPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_KTPortSim.flush();


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_KTPortSim.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;


    receivedByArray=m_KTPortSim.readAll();

    DSDEBUG_<<"Test Modle Send Command,KTSim, "<<strCommand<<"Get Response KTSim, "<<receivedByArray<<endl;

    receivedByArray.truncate(16);
    if(receivedByArray==strCommand)
        ret=true;
    else
        ret=false;

    return ret;
}



//调试模式下
bool BaseCom::sendAndGetQZCommand(QByteArray sendCommand,int& Angle,int& probV1,int& probV2,int& probV3)
{
    bool ret=true;
    int sendLen=  m_QZPort.write(sendCommand);
    if(!m_QZPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_QZPort.flush();
    QThread::msleep(m_waitTime);
    ret= m_QZPort.waitForReadyRead(m_waitTime);
    if(!ret)
    {
        qDebug()<<"no received,"<<endl;
        // return false;
    }



    QByteArray by=m_QZPort.readAll();

    DSDEBUG_<<"Test Modle Send Command,QZ, "<<sendCommand<<"Get Response QZ, "<<by<<endl;

    int size=by.size();

    if(by.size()!=19)
    {
        // m_QZPort.readAll();
        return false;
    }
    qDebug()<<by.data()<<endl;

    bool ok;
    QByteArray tmpReadData=by;
    QByteArray mid= tmpReadData.mid(2,4);
    int  angle=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(6,4);
    int probv1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(10,4);
    int probv2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(14,4);
    int probv3=QByteArray::fromHex(mid).toHex().toInt(&ok,16);


    Angle=angle;
    probV1=probv1;
    probV2=probv2;
    probV3=probv3;

    return ret;
}

//调试模式下
int BaseCom::sendAndGetKTCommand(QByteArray sendCommand,int& Angle,int& probV1,int& probV2,int& probV3)
{
    bool ret=true;
    int sendLen=  m_KTPort.write(sendCommand);
    if(!m_KTPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_KTPort.flush();

    QThread::msleep(m_waitTime);
    ret= m_KTPort.waitForReadyRead(m_waitTime);
    if(!ret)
    {
        qDebug()<<"no received,"<<endl;
        //return false;
    }

    QByteArray by=m_KTPort.readAll();
    DSDEBUG_<<"Test Modle Send Command,KT, "<<sendCommand<<"Get Response KT, "<<by<<endl;

    // ret=readKTData(by);
    int size=by.size();
    if(by.size()!=19)
    {
        m_KTPort.readAll();
        return false;
    }
    qDebug()<<by.data()<<endl;

    bool ok;
    QByteArray tmpReadData=by;

    QByteArray mid= tmpReadData.mid(2,4);
    int  angle=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(6,4);
    int probv1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(10,4);
    int probv2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    mid= tmpReadData.mid(14,4);
    int probv3=QByteArray::fromHex(mid).toHex().toInt(&ok,16);


    Angle=angle;
    probV1=probv1;
    probV2=probv2;
    probV3=probv3;

    return ret;
}

//正常模式下
bool BaseCom::sendAndGetQZCommand(QByteArray strCommand)
{
    bool ret=true;
    QByteArray sendCommand= strCommand.toHex().toUpper();
    int sendLen=  m_QZPort.write(sendCommand);

    // QThread::msleep(5);
    if(!m_QZPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_QZPort.flush();
    return ret;
}

bool BaseCom::sendAndGetKTCommand(QByteArray strCommand)
{
    // connect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);

    bool ret=true;

    QByteArray sendCommand= strCommand.toHex().toUpper();
    int sendLen=  m_KTPort.write(sendCommand);

    //QThread::msleep(5);
    if(!m_KTPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_KTPort.flush();

    //    ret= m_KTPort.waitForReadyRead(m_waitTime);

    return ret;
}

void BaseCom::onQZDataReceived()
{
    QByteArray by=m_QZPort.readAll();

    DSDEBUG_<<"onQZDataReceived :"<<by<<endl;
    m_ByQZContainer.append(by);
    int size=m_ByQZContainer.size();
    if(m_ByQZContainer.size()>=91)
    {
        for(int i=0;i<m_ByQZContainer.size();i++)
        {
            if(m_ByQZContainer[i]=='\n')
            {
                if(i>=90)
                {
                    DSDEBUG_<<"onQZDataReceived QZContainer :"<<m_ByQZContainer<<endl;
                    QByteArray byMsg=m_ByQZContainer.mid(i-90,90);
                    m_mutex.lock();
                    m_queQZFeedBack.clear();
                    m_queQZFeedBack.push_back(byMsg);
                    DSDEBUG_<<"onQZDataReceived byMsg90 :"<<byMsg<<endl;
                    m_mutex.unlock();
                    //m_cnt++;
                    m_ByQZContainer=m_ByQZContainer.mid(i+1,m_ByQZContainer.size()-(i+1));

                    break;
                }
            }
        }
    }

}

void BaseCom::onKTDataReceived()
{
    QByteArray by=m_KTPort.readAll();
    DSDEBUG_<<"onKTDataReceived :"<<by<<endl;

    m_ByKTContainer.append(by);
    int size=m_ByKTContainer.size();
    if(m_ByKTContainer.size()>=91)
    {
        for(int i=0;i<m_ByKTContainer.size();i++)
        {
            if(m_ByKTContainer[i]=='\n')
            {
                if(i>=90)
                {
                    DSDEBUG_<<"onKTDataReceived KTContainer :"<<m_ByKTContainer<<endl;

                    QByteArray byMsg=m_ByKTContainer.mid(i-90,90);
                    m_mutex.lock();
                    m_queKTFeedBack.clear();
                    m_queKTFeedBack.push_back(byMsg);
                    m_mutex.unlock();
                    DSDEBUG_<<"onKTDataReceived byMsg90 :"<<byMsg<<endl;
//                    m_cnt2++;
                    m_ByKTContainer=m_ByKTContainer.mid(i+1,m_ByKTContainer.size()-(i+1));
                    break;
                }

            }
        }
    }

}



bool BaseCom::readQZData(QByteArray& by)
{
    bool ret=false;
    if(m_queQZFeedBack.size()>0)
    {


        m_mutex.lock();
        DSDEBUG_<<"m_queQZ size:"<<m_queQZFeedBack.size()<<endl;
        //        by=m_queQZFeedBack.dequeue();

        by=m_queQZFeedBack.back();
        m_queQZFeedBack.clear();
        m_mutex.unlock();
        ret=true;
    }
    return  ret;
}





bool BaseCom::readKTData(QByteArray& by)
{
    bool ret=false;
    if(m_queKTFeedBack.size()>0)
    {
        m_mutex.lock();
        DSDEBUG_<<"m_queKT size:"<<m_queKTFeedBack.size()<<endl;
        by=m_queKTFeedBack.back();
        m_queKTFeedBack.clear();
        m_mutex.unlock();

        ret=true;
    }

    return  ret;
}

bool BaseCom::getQZData(QVector<int> &vecQZCheckRets)
{
    bool ret=false;

    QByteArray by;
    ret=readQZData(by);

    if(!ret)
    {
        return  ret;
    }

    bool ok;
    QByteArray tmpReadData=by;

    QByteArray mid= tmpReadData.mid(2,2);
    int  protime=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    vecQZCheckRets.push_back(protime);

    mid= tmpReadData.mid(4,2);
    int chekcret=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    vecQZCheckRets.push_back(chekcret);

    for(int i=0;i<20;i++)
    {
        mid= tmpReadData.mid(6+i*4,4);
        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        vecQZCheckRets.push_back(value);
    }

    return ret;
}

bool BaseCom::getKTData(QVector<int> &vecKTCheckRets)
{

    bool ret=false;

    QByteArray by;
    ret=readKTData(by);

    if(!ret)
    {
        return  ret;
    }

    bool ok;
    QByteArray tmpReadData=by;

    QByteArray mid= tmpReadData.mid(2,2);
    int  protime=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    vecKTCheckRets.push_back(protime);

    mid= tmpReadData.mid(4,2);
    int chekcret=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    vecKTCheckRets.push_back(chekcret);

    for(int i=0;i<20;i++)
    {
        mid= tmpReadData.mid(6+i*4,4);
        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        vecKTCheckRets.push_back(value);
    }
    return ret;
}



void BaseCom::togleConnect()
{
    connect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    connect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);

    m_mutex.lock();
    m_ByQZContainer.clear();
    m_ByKTContainer.clear();
    m_mutex.unlock();



}

void BaseCom::togleDisconnect()
{
    disconnect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    disconnect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);
}




