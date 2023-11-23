#ifndef CBASESERVER_H
#define CBASESERVER_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include<QList>
#include<QByteArray>
#include<QQueue>
#include<QStack>
#include<QMap>
#include<QMutex>
#include<QVariant>
#include<iostream>





class CBaseServer : public QObject
{
    Q_OBJECT
public:
    explicit CBaseServer(QObject *parent = nullptr);
    int sendMesToClinet(QByteArray &byteArray);
    void setHostIP(QString strHostIP);
    void setHostPort(uint16_t ui16HostPort);






    bool startListen();




    QTcpServer *tcpServer;
    QTcpSocket *currentClient=nullptr;
    QList<QTcpSocket*> tcpClient;
    QByteArray m_ByteArrCRC;
    QByteArray m_ByteArrEventCode;

    QString m_strConnectStatus="未连接";


private:
    QString m_strHostIP;
    quint16 m_ui16HostPort;



    QMutex m_mutexWafer;


    int m_iReceiveCnt1,m_iReceiveCnt2;
    int m_iSendCnt;

signals:
    void excuteSoftTrigSignal(int);



    void sendConnectStatus(int);
    void sendToMainDisp(unsigned int waferID,int checkedRet);
public slots:
    void newConnectionSlot();
    void disConnectedSlot();
    void readData();
    int sendMessage(QString message);
    void onErrorHandle(QAbstractSocket::SocketError socketEorrType);


};

#endif // CBASESERVER_H
