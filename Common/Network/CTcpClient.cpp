#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>

#include "CTcpClient.h"
#include "Common/CLogWriter.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19
// Modified : 2020-08-20
NS_SUGENTECH_BEGIN
struct CTcpClient::privateStruct
{
    QScopedPointer<QTcpSocket> mServerSocket;
    QString mIPAddress;
    int mPort;
};

CTcpClient::CTcpClient(QObject *parent) :
                                          QObject(parent),
                                          d(new privateStruct)
{
    d->mServerSocket.reset(new QTcpSocket(this));
    connect(d->mServerSocket.data(), &QTcpSocket::readyRead, this, &CTcpClient::onRecvLISServer);
}

CTcpClient::~CTcpClient()
{
    d->mServerSocket->close();
    CLogWriter::printLog(QString(">>> (CTcpClient::) : Close Connection"));
}

void CTcpClient::setIpAddressAndPort(const QString& ipAddress, int port)
{
    d->mIPAddress = ipAddress;
    d->mPort = port;
}

bool CTcpClient::connectToServer()
{
    if(d->mServerSocket->isOpen())
        return true;

    d->mServerSocket->connectToHost(d->mIPAddress, d->mPort);

    if(d->mServerSocket->waitForConnected(5000))
    {
        qDebug() << (QString("(CTcpClient) : Connection Success"));
        qDebug() << (QString("(CTcpClient) : IpAddress : %1, Port : %2").arg(d->mIPAddress).arg(d->mPort));
        CLogWriter::printLog(QString(">>> (CTcpClient::) : Connection Success IpAddress : %1, Port : %2").arg(d->mIPAddress).arg(d->mPort));
        return true;
    }

    qDebug() << (QString("(CTcpClient::CTcpClient) : ") + d->mServerSocket->errorString());

    return false;
}

bool CTcpClient::sendData(const QByteArray& data)
{
    if(d->mServerSocket->isOpen() == false)
        return false;

    d->mServerSocket->write(data);
    return true;
}

void CTcpClient::onRecvLISServer()
{
    auto data = d->mServerSocket->readAll();
    emit onRecvData(data);
}

NS_SUGENTECH_END
