#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>
#include "CTcpServer.h"

#include "Common/CLogWriter.h"
#include "Common/Network/CClientSocket.h"


NS_SUGENTECH_BEGIN
struct CTcpServer::privateStruct
{
    QTcpServer* mServer = nullptr;
    QMap<QString, CClientSocket*> mClientList;

};

CTcpServer::CTcpServer(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    d->mServer = new QTcpServer(this);
    connect(d->mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));


    if(!d->mServer->listen(QHostAddress::Any, DEFAULT_PORT))
        CLogWriter::printLog("(CTcpServer) : Server Could Not Started");
    else
        CLogWriter::printLog(QString("(CTcpServer) : Server Start Port : %1").arg(QString::number(DEFAULT_PORT)));
}

CTcpServer::~CTcpServer()
{
    d->mServer->close();
}

void CTcpServer::sendData(const QString& clientID, const QByteArray& sendData)
{
    auto clientSocket =  d->mClientList[clientID];

    if(clientSocket != nullptr)
        clientSocket->sendData(sendData);
}

void CTcpServer::onNewConnection()
{
    auto socket = d->mServer->nextPendingConnection();
    QString clientUUID = QUuid::createUuid().toString();

    CClientSocket* client = new CClientSocket(socket, clientUUID, this);
    d->mClientList[clientUUID] = client;

    connect(client, &CClientSocket::onRecv, this, &CTcpServer::onRecvData);
    connect(client, &CClientSocket::onDisconnect, this ,  &CTcpServer::onDisconnected);
}

void CTcpServer::onDisconnected(QString id)
{
    d->mClientList.remove(id);
}

void CTcpServer::onRecvData(QString clientUUID, QString recvData)
{
    emit onRecv(clientUUID, recvData);
}


NS_SUGENTECH_END
