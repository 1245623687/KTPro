#ifndef BASECOM_H
#define BASECOM_H

#include <QObject>
#include<QtSerialPort/QSerialPort>
#include<QDebug>
#include<QMutex>
#include<QThread>
#include<QQueue>
#include<QMutex>
#include<QElapsedTimer>
#include<QtConcurrent>
#include "glog/logging.h"

class BaseCom : public QObject
{
    Q_OBJECT
public:
    
    static BaseCom* getInstance();
    
    explicit BaseCom(QObject *parent = nullptr);
    
    ~BaseCom();


    void setQZPort(QString strQZport){m_strQZPortName=strQZport;}
    QString getQZPort(){return  m_strQZPortName;}

    void setKTPort(QString strKTport){m_strKTPortName=strKTport;}
    QString getKTPort(){return  m_strKTPortName;}


    void setQZPortSim(QString strQZport){m_strQZPortNameSim=strQZport;}
    QString getQZPortSim(){return  m_strQZPortNameSim;}

    void setKTPortSim(QString strKTport){m_strKTPortNameSim=strKTport;}
    QString getKTPortSim(){return  m_strKTPortNameSim;}



    bool openQZComPort(QString qzPortName="COM1", qint32 baudRate=QSerialPort::Baud115200,QSerialPort::DataBits dataBits=QSerialPort::Data8,
                       QSerialPort::StopBits stopBits=QSerialPort::OneStop,QSerialPort::Parity parity=QSerialPort::NoParity,QSerialPort::FlowControl flowControl=QSerialPort::NoFlowControl);


    bool openKTComPort(QString ktPortName="COM8", qint32 baudRate=QSerialPort::Baud115200,QSerialPort::DataBits dataBits=QSerialPort::Data8,
                       QSerialPort::StopBits stopBits=QSerialPort::OneStop,QSerialPort::Parity parity=QSerialPort::NoParity,QSerialPort::FlowControl flowControl=QSerialPort::NoFlowControl);


    bool openQZComPortSim(QString qzPortName="COM3", qint32 baudRate=QSerialPort::Baud115200,QSerialPort::DataBits dataBits=QSerialPort::Data8,
                          QSerialPort::StopBits stopBits=QSerialPort::OneStop,QSerialPort::Parity parity=QSerialPort::NoParity,QSerialPort::FlowControl flowControl=QSerialPort::NoFlowControl);


    bool openKTComPortSim(QString ktPortName="COM4", qint32 baudRate=QSerialPort::Baud115200,QSerialPort::DataBits dataBits=QSerialPort::Data8,
                          QSerialPort::StopBits stopBits=QSerialPort::OneStop,QSerialPort::Parity parity=QSerialPort::NoParity,QSerialPort::FlowControl flowControl=QSerialPort::NoFlowControl);



    int openComPort();
    bool sendQZCommand(QByteArray strCommand,QByteArray& receivedByArray);
    int  sendKTCommand(QByteArray strCommand,QByteArray& receivedByArray);

    bool sendQZCommandSim(QByteArray strCommand,QByteArray& receivedByArray,QVector<int>& receivedVec,int commondType=1);
    int  sendKTCommandSim(QByteArray strCommand,QByteArray& receivedByArray,QVector<int>& receivedVec,int commondType=1);


    //调试模式下
    bool sendAndGetQZCommand(QByteArray strCommand,int& Angle,int& probV1,int& probV2,int& probV3);
    int  sendAndGetKTCommand(QByteArray strCommand,int& Angle,int& probV1,int& probV2,int& probV3);

    //运行模式下
    bool sendAndGetQZCommand(QByteArray strCommand );
    bool sendAndGetKTCommand(QByteArray strCommand );

    
    void onQZDataReceived();
    void onKTDataReceived();

    void onQZDataReceivedSim();
    void onKTDataReceivedSim();

    bool readQZData(QByteArray& by);
    bool readKTData(QByteArray& by);

    bool readQZDataSim(QByteArray& by);
    bool readKTDataSim(QByteArray& by,int timeout);

    bool getQZData(QVector<int>&);

    //int getQZData(QVector<int>&,QByteArray& Receiveby);

    bool getKTData(QVector<int>&);

    int getQZDataSim(QVector<int>&,QByteArray& Receiveby);
    int getKTDataSim(QVector<int>&,QByteArray& Receiveby);

    void  threadGetReceivedDataQZ(QByteArray by);
    //    void  threadGetReceivedDataKT(QByteArray by);
    //    void  threadGetReceivedDataQZSim(QByteArray by);
    //    void  threadGetReceivedDataKTSim(QByteArray by);


    void  togleConnect();
    void  togleDisconnect();

    void clearQue();

    class GC
    {
    public:
        ~GC()
        {
            if(m_instance!=NULL)
            {
                delete m_instance;
                m_instance=NULL;
            }
        }
        
    };
    static GC gc;

    QSerialPort m_QZPort;
    QSerialPort m_KTPort;

    QSerialPort m_QZPortSim;
    QSerialPort m_KTPortSim;


    QFuture<void>  m_QZFuture;
    QFuture<void>  m_KTFuture;

    QFuture<void>  m_QZFutureSim;
    QFuture<void>  m_KTFutureSim;

private:

    QByteArray m_ByQZContainer;
    QByteArray m_ByKTContainer;

    QByteArray m_ByQZContainerSim;
    QByteArray m_ByKTContainerSim;



    QString m_strQZPortName;
    QString m_strKTPortName;

    QString m_strQZPortNameSim;
    QString m_strKTPortNameSim;

    static BaseCom* m_instance;
    static QMutex mutex;

    QQueue<QByteArray> m_queKTFeedBack;
    QQueue<QByteArray> m_queQZFeedBack;

    QQueue<QByteArray> m_queKTFeedBackSim;
    QQueue<QByteArray> m_queQZFeedBackSim;



    QMutex m_mutex;

    int m_cnt;
    int m_cnt2;

    int m_waitTime;



signals:

    void sendCommandRet(QString str);



public slots:

};

#endif // BASECOM_H
