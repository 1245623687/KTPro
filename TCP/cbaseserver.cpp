#include "cbaseserver.h"

CBaseServer::CBaseServer(QObject *parent) : QObject(parent)
{
    tcpServer=new QTcpServer();
    connect(tcpServer,&QTcpServer::newConnection,this,&CBaseServer::newConnectionSlot);
}

void CBaseServer::setHostIP(QString strHostIP)
{
    m_strHostIP=strHostIP;
}

void CBaseServer::setHostPort(uint16_t ui16HostPort)
{
    m_ui16HostPort=ui16HostPort;
}



bool CBaseServer::startListen()
{
    QHostAddress hostAddress(m_strHostIP);
    quint16 serport=m_ui16HostPort;
    return  tcpServer->listen(hostAddress,serport);
    
}

int CBaseServer::sendMessage(QString messsage)
{
    if(currentClient==nullptr)
        return 0;

   qint64 sendLenght =tcpClient[0]->write(messsage.toStdString().c_str());
   tcpClient[0]->flush();
   return sendLenght;

}

void CBaseServer::onErrorHandle(QAbstractSocket::SocketError socketEorrType)
{
    
//    StuAddLogInfo stuloginfo;
//    stuloginfo.s_eFileTypeName=EFileTypeName::eSystermLog;
//    stuloginfo.s_key="Socket Error Msg:  ";
//    stuloginfo.s_infoText="Error Msg Type "+QString::number(static_cast<int>(socketEorrType));
    
//    CSystermLog::GetInstance()->m_pClogThread->addInfo(stuloginfo);
}



void CBaseServer::newConnectionSlot()
{
    currentClient=tcpServer->nextPendingConnection();
    tcpClient.append(currentClient);
    connect(currentClient,&QTcpSocket::readyRead,this,&CBaseServer::readData);
    connect(currentClient,&QTcpSocket::disconnected,this,&CBaseServer::disConnectedSlot);
    connect(currentClient,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&CBaseServer::onErrorHandle);
    
    QString adderss= currentClient->peerAddress().toString();
    quint16 port=  currentClient->peerPort();
        m_strConnectStatus="连接成功";

    emit sendConnectStatus(0);
    qDebug()<<"新連接"<<adderss<<","<<port<<endl;
}


void CBaseServer::disConnectedSlot()
{
    QString adderss=  tcpClient[0]->peerAddress().toString();
    quint16 port=  tcpClient[0]->peerPort();
    tcpClient[0]->destroyed();
    tcpClient.removeAt(0);
    currentClient=nullptr;
      m_strConnectStatus="连接失败";
    emit sendConnectStatus(1);
    qDebug()<<"斷開連接："<<adderss<<","<<port<<endl; 
}


void CBaseServer::readData()
{
    QByteArray qbytearray;
    qbytearray= tcpClient[0]->readAll();
    int sz=qbytearray.size();   
}

