#include <QVector>
#include <QTimer>
#include <QTcpSocket>
#include "CClientSocket.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2017-08-17

NS_SUGENTECH_BEGIN

struct CClientSocket::privateStruct
{
    QVector<QByteArray> mSendQueue;
    QString mClientID;
};

CClientSocket::CClientSocket(QTcpSocket *clientSocket, QString clientUUID, QObject *parent) :
    QObject(parent),
    d(new privateStruct),
    mClientSocket(clientSocket)

{
    d->mClientID = clientUUID;

    connect(mClientSocket.data(), SIGNAL(readyRead()), this, SLOT(onRecvMapper()));
    connect(mClientSocket.data(), SIGNAL(disconnected()), this, SLOT(onDisconnectMapper()));
}

CClientSocket::~CClientSocket()
{
    mClientSocket->close();
}

void CClientSocket::sendData(const QByteArray& data)
{
    this->mClientSocket->write(data);
}

void CClientSocket::onRecvMapper()
{
    QByteArray data = mClientSocket->readAll();
    emit onRecv(d->mClientID, data);
}

void CClientSocket::onDisconnectMapper()
{
    emit onDisconnect(d->mClientID);
}

NS_SUGENTECH_END
