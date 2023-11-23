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

    m_waitTime=200;
    m_strQZPortName="COM1";
    m_strKTPortName="COM2";

    m_strQZPortNameSim="COM3";
    m_strKTPortNameSim="COM4";


#ifdef FALG_PHE
    connect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    connect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);

    connect(&m_QZPortSim,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceivedSim);
    connect(&m_KTPortSim,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceivedSim);
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

    if(m_QZPortSim.isOpen())
    {
        m_QZPortSim.close();
    }
    if(m_KTPortSim.isOpen())
    {
        m_KTPortSim.close();
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
    // ret=openQZComPortSim();
    // if(!ret) return 3;
    ret=openKTComPortSim();
    if(!ret) return 4;
    return 0;
}


bool BaseCom::sendQZCommand(QByteArray strCommand, QByteArray &receivedByArray)
{
    if(!m_QZPort.isOpen() )
        return false;

    bool ret=false;
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

    int size=receivedByArray.size();
    if(receivedByArray.size()==17)
    {
        receivedByArray.truncate(16);
        if(receivedByArray==strCommand)
            ret=true;
        else
            ret=false;
        qDebug()<<"sendLen,"<<sendLen<<endl;
        return ret;
    }

    else
        if(receivedByArray.size()==155)
        {
            receivedByArray.truncate(154);
            return true;
        }

    return ret;
}




int BaseCom::sendKTCommand(QByteArray strCommand, QByteArray &receivedByArray)
{
    if(!m_KTPort.isOpen() )
        return false;

    bool ret=false;

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

    int size=receivedByArray.size();
    //从控制板返回相同指令
    if(receivedByArray.size()==17)
    {
        receivedByArray.truncate(16);
        if(receivedByArray==strCommand)
            ret=true;
        else
            ret=false;
        qDebug()<<"sendLen,"<<sendLen<<endl;
        return ret;
    }
    //从控制板读取参数
    else
        if(receivedByArray.size()==155)
        {
            receivedByArray.truncate(154);
            return true;
        }
        else
        {
            return false;
        }
    return ret;
}


bool BaseCom::sendQZCommandSim(QByteArray strCommand, QByteArray &receivedByArray, QVector<int>& receivedVec,int commondType)
{
    if(!m_QZPortSim.isOpen() || !m_QZPort.isOpen() )
        return false;

    bool ret=true;
    if(commondType==1||commondType==3)
    {
        int sendLen=  m_QZPortSim.write(strCommand);
        if(!m_QZPortSim.waitForBytesWritten(m_waitTime))
        {
            qDebug() << "waitForBytesWritten Error" ;
            return  false;
        }
        m_QZPortSim.flush();
    }

    //指令2通过控制板发送，通过模拟板接受
    if(commondType==2)
    {
        int sendLen=  m_QZPort.write(strCommand);
        if(!m_QZPort.waitForBytesWritten(m_waitTime))
        {
            qDebug() << "waitForBytesWritten Error" ;
            return  false;
        }
        m_QZPort.flush();
    }


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_QZPortSim.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;


    receivedByArray=m_QZPortSim.readAll();
    int size=receivedByArray.size();

    ret=false;
    if(receivedByArray.size()==17)
    {
        receivedByArray.truncate(16);
        if(receivedByArray==strCommand)
            ret=true;
        else
            ret=false;
        return ret;
    }

    if(receivedByArray.size()==91)
    {
        receivedByArray.truncate(90);

        return true;
    }

    //3个探头
    if(receivedByArray.size()==58)
    {
        receivedByArray=receivedByArray.mid(17,40);
        bool ok;
        QByteArray tmpReadData=receivedByArray;
        QByteArray mid;

        for(int i=0;i<3;i++)
        {
            mid= tmpReadData.mid(4+i*8,8);
            //电流
            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
            //增益
            value=QByteArray::fromHex(mid.mid(5,4)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
        }
        return true;
    }

    //20个探头
     if(receivedByArray.size()==308)
     {
         receivedByArray=receivedByArray.mid(17,290);
         bool ok;
         QByteArray tmpReadData=receivedByArray;

         QByteArray mid;
         for(int i=0;i<20;i++)
         {
             mid= tmpReadData.mid(4+i*8,8);

             //电流
             int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
             receivedVec.push_back(value);
             //增益
             value=QByteArray::fromHex(mid.mid(4,4)).toHex().toInt(&ok,16);
             receivedVec.push_back(value);
         }

         //20个门槛值
         for(int i=0;i<20;i++)
         {
             mid= tmpReadData.mid(164+i*6,6);

             mid= mid.mid(2,4);
             int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
             receivedVec.push_back(value);
         }
         //1个门槛值系数
         mid= tmpReadData.mid(284,2);
         int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
         receivedVec.push_back(value);
         // vecQZCheckRets一共有60个值
         return true;
     }

    return ret;
}


int BaseCom::sendKTCommandSim(QByteArray strCommand, QByteArray &receivedByArray,QVector<int>& receivedVec,int commondType)
{

    if(!m_KTPortSim.isOpen() || !m_KTPort.isOpen())
        return false;

    bool ret=true;

    //commondType=3代表从模拟板加载参数
    if(commondType==1||commondType==3)
    {
        int sendLen=  m_KTPortSim.write(strCommand);
        if(!m_KTPortSim.waitForBytesWritten(m_waitTime))
        {
            qDebug() << "waitForBytesWritten Error" ;
            return  false;
        }
        m_KTPortSim.flush();
    }

    //指令2通过控制板发送，通过模拟板接受
    if(commondType==2)
    {
        int sendLen=  m_KTPort.write(strCommand);
        if(!m_KTPort.waitForBytesWritten(m_waitTime))
        {
            qDebug() << "waitForBytesWritten Error" ;
            return  false;
        }
        m_KTPort.flush();
    }


    QThread::msleep(m_waitTime);
    QElapsedTimer elpas;
    elpas.restart();
    ret= m_KTPortSim.waitForReadyRead(m_waitTime);
    qDebug()<<elpas.elapsed()<<endl;

    receivedByArray=m_KTPortSim.readAll();
    int size=receivedByArray.size();


    DSDEBUG_<<"m_KTPortSim received, "<<receivedByArray<<endl;


    ret=false;
    if(receivedByArray.size()==17)
    {
        receivedByArray.truncate(16);
        if(receivedByArray==strCommand)
            ret=true;
        else
            ret=false;
        return ret;
    }
    if(receivedByArray.size()==91)
    {
        receivedByArray.truncate(90);

        return true;
    }

//    //3个探头
//    if(receivedByArray.size()==172)
//    {
//        receivedByArray=receivedByArray.mid(17,154);
//        bool ok;
//        QByteArray tmpReadData=receivedByArray;
//        QByteArray mid;

//        for(int i=0;i<3;i++)
//        {
//            mid= tmpReadData.mid(4+i*8,8);

//            //电流
//            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
//            receivedVec.push_back(value);
//            //增益
//            value=QByteArray::fromHex(mid.mid(5,4)).toHex().toInt(&ok,16);
//            receivedVec.push_back(value);
//        }

//        //20个门槛值
//        for(int i=0;i<20;i++)
//        {
//            mid= tmpReadData.mid(28+i*6,6);
//            //电流
//            mid= mid.mid(2,4);
//            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
//            receivedVec.push_back(value);
//        }
//        //1个门槛值系数
//        mid= tmpReadData.mid(148,2);
//        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
//        receivedVec.push_back(value);

//        return true;
//    }

    //3个探头
    if(receivedByArray.size()==58)
    {
        receivedByArray=receivedByArray.mid(17,40);
        bool ok;
        QByteArray tmpReadData=receivedByArray;
        QByteArray mid;

        for(int i=0;i<3;i++)
        {
            mid= tmpReadData.mid(4+i*8,8);

            //电流
            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
            //增益
            value=QByteArray::fromHex(mid.mid(5,4)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
        }

//        //20个门槛值
//        for(int i=0;i<20;i++)
//        {
//            mid= tmpReadData.mid(28+i*6,6);
//            //电流
//            mid= mid.mid(2,4);
//            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
//            receivedVec.push_back(value);
//        }
//        //1个门槛值系数
//        mid= tmpReadData.mid(148,2);
//        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
//        receivedVec.push_back(value);

        return true;
    }

   //20个探头
    if(receivedByArray.size()==308)
    {

        receivedByArray=receivedByArray.mid(17,290);
        bool ok;
        QByteArray tmpReadData=receivedByArray;

        QByteArray mid;
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(4+i*8,8);

            //电流
            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
            //增益
            value=QByteArray::fromHex(mid.mid(4,4)).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
        }

        //20个门槛值
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(164+i*6,6);

            mid= mid.mid(2,4);
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            receivedVec.push_back(value);
        }
        //1个门槛值系数
        mid= tmpReadData.mid(284,2);
        int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
        receivedVec.push_back(value);
        // vecQZCheckRets一共有60个值
        return true;
    }
    return ret;
}


//调试模式下
bool BaseCom::sendAndGetQZCommand(QByteArray sendCommand,int& Angle,int& probV1,int& probV2,int& probV3)
{
    if(!m_QZPort.isOpen())
        return false;

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
        //m_QZPort.readAll();
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


    //从模拟板解析探头值
    //    ret= m_QZPortSim.waitForReadyRead(m_waitTime);
    //    QByteArray bySim=m_QZPortSim.readAll();
    //    DSDEBUG_<<"Test Modle Send Command,QZ, "<<sendCommand<<"Get Response QZSim, "<<bySim<<endl;

    //    size=bySim.size();
    //    if(bySim.size()!=91)
    //    {
    //        return false;
    //    }
    //    mid= bySim.mid(6,4);
    //    int probv1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    //    mid= bySim.mid(34,4);
    //    int probv2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    //    mid= bySim.mid(58,4);
    //    int probv3=QByteArray::fromHex(mid).toHex().toInt(&ok,16);


    Angle=angle;
    probV1=probv1;
    probV2=probv2;
    probV3=probv3;


    Angle=angle;
    probV1=probv1;
    probV2=probv2;
    probV3=probv3;

    return ret;
}

//调试模式下
int BaseCom::sendAndGetKTCommand(QByteArray sendCommand,int& Angle,int& probV1,int& probV2,int& probV3)
{
    if(!m_KTPort.isOpen())
        return false;

    bool ret=true;
    int sendLen=  m_KTPort.write(sendCommand);
    if(!m_KTPort.waitForBytesWritten(m_waitTime))
    {
        qDebug() << "waitForBytesWritten Error" ;
        return  false;
    }
    m_KTPort.flush();

    QThread::msleep(200);
    ret= m_KTPort.waitForReadyRead(m_waitTime);
    if(!ret)
    {
        qDebug()<<"no received,"<<endl;
        //return false;
    }

    //从控制板读取角度
    QByteArray by=m_KTPort.readAll();
    DSDEBUG_<<"Test Modle Send Command,KT, "<<sendCommand<<"Get Response KT, "<<by<<endl;

    // ret=readKTData(by);
    int size=by.size();
    if(by.size()!=19)
    {
        return false;
    }



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



    //从模拟板解析探头值
    //    ret= m_KTPortSim.waitForReadyRead(m_waitTime);
    //    QByteArray bySim=m_KTPortSim.readAll();
    //    DSDEBUG_<<"Test Modle Send Command,KT, "<<sendCommand<<"Get Response KTSim, "<<bySim<<endl;

    //    size=bySim.size();
    //    if(bySim.size()!=91)
    //    {
    //        m_KTPortSim.readAll();
    //        return false;

    //    }
    //    // 解析第1,8,14支数据
    //    mid= bySim.mid(6,4);
    //    int probv1=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    //    mid= bySim.mid(34,4);
    //    int probv2=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
    //    mid= bySim.mid(58,4);
    //    int probv3=QByteArray::fromHex(mid).toHex().toInt(&ok,16);

    Angle=angle;
    probV1=probv1;
    probV2=probv2;
    probV3=probv3;

    return ret;
}

//正常模式下
bool BaseCom::sendAndGetQZCommand(QByteArray strCommand)
{
    if(!m_QZPort.isOpen())
        return false;

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

    if(!m_KTPort.isOpen())
        return false;

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
    if(!m_QZPort.isOpen())
        return;

    QByteArray by=m_QZPort.readAll();

    DSDEBUG_<<"onQZDataReceived :"<<by<<endl;
    m_ByQZContainer.append(by);
    DSDEBUG_<<"m_ByQZContainer :"<<m_ByQZContainer<<endl;
    int size=m_ByQZContainer.size();

    for(int i=0;i<m_ByQZContainer.size();i++)
    {
        if(m_ByQZContainer[i]=='\n')
        {
            DSDEBUG_<<"onQZDataReceived QZContainer :"<<m_ByQZContainer<<endl;
            QByteArray byMsg=m_ByQZContainer.mid(0,i);
            m_mutex.lock();
            if(m_queQZFeedBack.size()>=30)
                m_queQZFeedBack.clear();
            m_queQZFeedBack.push_back(byMsg);
            DSDEBUG_<<"pushQZ  byMsg :"<<byMsg<<endl;
            m_mutex.unlock();
            emit updateCheckRetSig(1);
            //m_cnt++;
            m_ByQZContainer=m_ByQZContainer.mid(i+1,m_ByQZContainer.size()-(i+1));
            break;
        }
    }
}

//void BaseCom::onKTDataReceived()
//{
//    QByteArray by=m_KTPort.readAll();
//    DSDEBUG_<<"onKTDataReceived :"<<by<<endl;

//    m_ByKTContainer.append(by);
//    int size=m_ByKTContainer.size();
//    if(m_ByKTContainer.size()>=91)
//    {
//        for(int i=0;i<m_ByKTContainer.size();i++)
//        {
//            if(m_ByKTContainer[i]=='\n')
//            {
//                if(i>=90)
//                {
//                    DSDEBUG_<<"onKTDataReceived KTContainer :"<<m_ByKTContainer<<endl;

//                    QByteArray byMsg=m_ByKTContainer.mid(i-90,90);
//                    m_mutex.lock();
//                    m_queKTFeedBack.clear();
//                    m_queKTFeedBack.push_back(byMsg);
//                    m_mutex.unlock();
//                    DSDEBUG_<<"onKTDataReceived byMsg90 :"<<byMsg<<endl;
//                    //                    m_cnt2++;
//                    m_ByKTContainer=m_ByKTContainer.mid(i+1,m_ByKTContainer.size()-(i+1));
//                    break;
//                }
//            }
//        }
//    }
//}

void BaseCom::onKTDataReceived()
{
    if(!m_KTPort.isOpen())
        return;

    QByteArray by=m_KTPort.readAll();
    DSDEBUG_<<"onKTDataReceived :"<<by<<endl;

    m_ByKTContainer.append(by);
    int size=m_ByKTContainer.size();

    for(int i=0;i<m_ByKTContainer.size();i++)
    {
        if(m_ByKTContainer[i]=='\n')
        {
            DSDEBUG_<<"onKTDataReceived KTContainer :"<<m_ByKTContainer<<endl;
            QByteArray byMsg=m_ByKTContainer.mid(0,i);
            m_mutex.lock();

            if(m_queKTFeedBack.size()>=30)
                m_queKTFeedBack.clear();
            m_queKTFeedBack.push_back(byMsg);
            m_mutex.unlock();
            DSDEBUG_<<"pushKT  byMsg :"<<byMsg<<endl;
            emit updateCheckRetSig(2);
            //m_cnt2++;
            m_ByKTContainer=m_ByKTContainer.mid(i+1,m_ByKTContainer.size()-(i+1));
            break;
        }
    }
}


void BaseCom::onQZDataReceivedSim()
{
    if(!m_QZPortSim.isOpen())
        return;

    QByteArray by=m_QZPortSim.readAll();

    DSDEBUG__<<"onQZDataReceivedSim :"<<by<<endl;
    m_ByQZContainerSim.append(by);
    int size=m_ByQZContainerSim.size();

    for(int i=0;i<m_ByQZContainerSim.size();i++)
    {
        if(m_ByQZContainerSim[i]=='\n')
        {
            DSDEBUG__<<"onQZDataReceived QZContainer :"<<m_ByQZContainerSim<<endl;
            QByteArray byMsg=m_ByQZContainerSim.mid(0,i);
            m_mutex.lock();
            if(m_queQZFeedBackSim.size()>=30)
                m_queQZFeedBackSim.clear();
            m_queQZFeedBackSim.push_back(byMsg);
            DSDEBUG__<<"pushQZ  byMsgSim :"<<byMsg<<endl;
            m_mutex.unlock();
            emit updateCheckRetSig(3);
            //m_cnt++;
            m_ByQZContainerSim=m_ByQZContainerSim.mid(i+1,m_ByQZContainerSim.size()-(i+1));
            break;

        }
    }
}

void BaseCom::onKTDataReceivedSim()
{
    if(!m_KTPortSim.isOpen())
        return;
    QByteArray by=m_KTPortSim.readAll();
    DSDEBUG__<<"onKTDataReceivedSim :"<<by<<endl;

    m_ByKTContainerSim.append(by);
    int size=m_ByKTContainerSim.size();

    for(int i=0;i<m_ByKTContainerSim.size();i++)
    {
        if(m_ByKTContainerSim[i]=='\n')
        {
            DSDEBUG__<<"onKTDataReceivedSim KTContainer :"<<m_ByKTContainerSim<<endl;
            QByteArray byMsg=m_ByKTContainerSim.mid(0,i);
            m_mutex.lock();
            if(m_queKTFeedBackSim.size()>=30)
                m_queKTFeedBackSim.clear();

            m_queKTFeedBackSim.push_back(byMsg);
            m_mutex.unlock();
            DSDEBUG__<<"pushKT  byMsgSim :"<<byMsg<<endl;
            emit updateCheckRetSig(4);

            //m_cnt2++;
            m_ByKTContainerSim=m_ByKTContainerSim.mid(i+1,m_ByKTContainerSim.size()-(i+1));
            break;
        }
    }

}


bool BaseCom::readQZData(QByteArray& by)
{
    bool ret=false;

    DSDEBUG_<<"m_queQZ size:"<<m_queQZFeedBack.size()<<by<<endl;
    if(m_queQZFeedBack.size()>0)
    {
        m_mutex.lock();
        //        by=m_queQZFeedBack.dequeue();
        by=m_queQZFeedBack.back();
        DSDEBUG_<<"m_queQZ size:"<<m_queQZFeedBack.size()<<by<<endl;

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

bool BaseCom::readQZDataSim(QByteArray &by)
{
    bool ret=false;
    if(m_queQZFeedBackSim.size()>0)
    {
        m_mutex.lock();
        DSDEBUG_<<"m_queQZSim size:"<<m_queQZFeedBackSim.size()<<endl;
        // by=m_queQZFeedBack.dequeue();

        by=m_queQZFeedBackSim.back();
        m_queQZFeedBackSim.clear();
        m_mutex.unlock();
        ret=true;
    }
    return  ret;
}

bool BaseCom::readKTDataSim(QByteArray &by,int timeout)
{
        bool ret=false;
        if(m_queKTFeedBackSim.size()>0)
        {
            m_mutex.lock();
            DSDEBUG_<<"m_queKTSim size:"<<m_queKTFeedBackSim.size()<<endl;

            by=m_queKTFeedBackSim.back();
            m_queKTFeedBackSim.clear();
            m_mutex.unlock();
            ret=true;
        }
        return  ret;


//    bool ret=false;
//    QElapsedTimer elapstime;
//    elapstime.restart();
//    while (true)
//    {
//        QThread::msleep(1);
//        if(elapstime.elapsed()>timeout)
//        {
//            break;
//        }
//        if(m_queKTFeedBackSim.size()>0)
//        {
//            m_mutex.lock();
//            DSDEBUG_<<"m_queKTSim size:"<<m_queKTFeedBackSim.size()<<endl;

//            by=m_queKTFeedBackSim.back();
//            m_queKTFeedBackSim.clear();
//            m_mutex.unlock();
//            ret=true;
//        }
//    }

//    return ret;

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

int BaseCom::getQZDataSim(QVector<int> &vecQZCheckRets,QByteArray& Receiveby)
{
    int ret=false;

    QByteArray by;
    ret=readQZDataSim(by);

    if(!ret)
    {
        return  ret;
    }
    //获取的by分3种类型，第一是返回的指令，第二是20支烟的ADC采样数据，第三是从模拟板获取的电流增益和门槛值

    //第一是返回的指令
    if(by.size()==16)
    {
        Receiveby=by;
        return 1;
    }

    //第二是20支烟的ADC采样数据
    if(by.size()==90)
    {
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
        return 2;
    }

    //第三是从模拟板获取的电流增益和门槛值
    if(by.size()>90)
    {
        bool ok;
        QByteArray tmpReadData=by;
        QByteArray mid;

        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(4+i*8,8);

            //电流
            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
            vecQZCheckRets.push_back(value);
            //增益
            value=QByteArray::fromHex(mid.mid(5,4)).toHex().toInt(&ok,16);
            vecQZCheckRets.push_back(value);
        }

        //20个门槛值
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(164+i*4,4);

            //电流
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            vecQZCheckRets.push_back(value);
        }
        // vecQZCheckRets一共有60个值
        return 3;
    }

    return  0;
}


int BaseCom::getKTDataSim(QVector<int> &vecKTCheckRets,QByteArray& Receiveby)
{
    int ret=false;

    QByteArray by;
    ret=readKTDataSim(by);

    if(!ret)
    {
        return  ret;
    }
    //获取的by分3种类型，第一是返回的指令，第二是20支烟的ADC采样数据，第三是从模拟板获取的电流增益和门槛值

    //第一是返回的指令
    if(by.size()==16)
    {
        Receiveby=by;
        return 1;
    }

    //第二是20支烟的ADC采样数据
    if(by.size()==90)
    {
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
        return 2;
    }

    //第三是从模拟板获取的电流增益和门槛值
    if(by.size()>90)
    {
        bool ok;
        QByteArray tmpReadData=by;
        QByteArray mid;

        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(4+i*8,8);
            //电流
            int  value=QByteArray::fromHex(mid.mid(2,2)).toHex().toInt(&ok,16);
            vecKTCheckRets.push_back(value);
            //增益
            value=QByteArray::fromHex(mid.mid(5,4)).toHex().toInt(&ok,16);
            vecKTCheckRets.push_back(value);
        }

        //20个门槛值
        for(int i=0;i<20;i++)
        {
            mid= tmpReadData.mid(164+i*4,4);
            //电流
            int  value=QByteArray::fromHex(mid).toHex().toInt(&ok,16);
            vecKTCheckRets.push_back(value);
        }

        // vecQZCheckRets一共有60个值
        return 3;
    }

    return  0;
}

void BaseCom::threadGetReceivedDataQZ(QByteArray strCommand)
{
    int ret=0;
    QVector<int> receivedVec;
    QByteArray receivedByArray2;
    //启动线程

    //     QThread::msleep(1);

    DSDEBUG_<<"m_queQZFeedBack.size"<<m_queQZFeedBack.size()<<endl;
    m_queQZFeedBack.clear();
    QElapsedTimer elpas;
    elpas.restart();
    while (true)
    {
        QThread::msleep(20);
        if(elpas.elapsed()>5000)
            break;
        ret=getQZData(receivedVec);
        if(ret>0)
        {
            DSDEBUG_<<"getQZData"<<receivedByArray2<<endl;
            break;
        }
    }

    if(ret==1)
    {
        if(receivedByArray2==strCommand)
            ret=true;
        else
            ret=false;

        if(ret)
        {
            QString txtTip="";
            txtTip=QString("QZ")+",Set Success,"+"send,"+QString(strCommand)+",receive,"+QString(receivedByArray2);
            emit sendCommandRet(txtTip);
        }
        else
        {
            QString txtTip="";
            txtTip=QString("QZ")+",Set Failed,"+"send,"+QString(strCommand)+",receive,"+QString(receivedByArray2);
            emit sendCommandRet(txtTip);
        }
    }

    DSDEBUG_<<"Test Modle Send Command,QZ, "<<strCommand<<"Get Response QZ, "<<receivedByArray2<<endl;


}




void BaseCom::togleConnect()
{
    connect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    connect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);

    connect(&m_QZPortSim,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceivedSim);
    connect(&m_KTPortSim,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceivedSim);

    m_mutex.lock();
    m_ByQZContainer.clear();
    m_ByKTContainer.clear();

    m_ByQZContainerSim.clear();
    m_ByKTContainerSim.clear();
    m_mutex.unlock();



}

void BaseCom::togleDisconnect()
{
    disconnect(&m_QZPort,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceived);
    disconnect(&m_KTPort,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceived);

    disconnect(&m_QZPortSim,&QSerialPort::readyRead,this,&BaseCom::onQZDataReceivedSim);
    disconnect(&m_KTPortSim,&QSerialPort::readyRead,this,&BaseCom::onKTDataReceivedSim);
}

void BaseCom::clearQue()
{

    m_mutex.lock();
    m_queKTFeedBack.clear();
    m_queQZFeedBack.clear();
    m_queKTFeedBackSim.clear();
    m_queQZFeedBackSim.clear();


    m_ByQZContainer.clear();
    m_ByKTContainer.clear();
    m_ByQZContainerSim.clear();
    m_ByKTContainerSim.clear();
    m_mutex.unlock();

}




