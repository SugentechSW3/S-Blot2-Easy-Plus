#ifndef CCLIENT_SOCKET_H
#define CCLIENT_SOCKET_H

#include "Common/CUtilCommon.h"

#ifdef USE_QT

#include <QObject>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QString>

class QTcpSocket;


NS_SUGENTECH_BEGIN
class CClientSocket : public QObject
{
    Q_OBJECT

    struct privateStruct;
public:
    explicit CClientSocket(QTcpSocket* clientSocket, QString clientUUID ,QObject* parent = nullptr);
    virtual ~CClientSocket();

    void sendData(const QByteArray& data);
signals:
    void onRecv(QString clientID, QString data);
    void onDisconnect(QString clientID);

private slots:
    void onRecvMapper();
    void onDisconnectMapper();

private:
    QScopedPointer<privateStruct> d;
    QScopedPointer<QTcpSocket> mClientSocket;
};

NS_SUGENTECH_END

#endif

#endif // CCLIENTSOCKET_H
