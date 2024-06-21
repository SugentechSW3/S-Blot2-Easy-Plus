#ifndef CTCP_SERVER_H
#define CTCP_SERVER_H

#include "Common/CUtilCommon.h"


#ifdef USE_QT

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QTcpSocket>

const int DEFAULT_PORT = 11520;

NS_SUGENTECH_BEGIN

class CTcpServer : public QObject
{
    Q_OBJECT
   struct privateStruct;
public:
    explicit CTcpServer(QObject *parent = nullptr);
    virtual ~CTcpServer();

   void sendData(const QString& clientID, const QByteArray &sendData);

signals:
   void onRecv(QString id , QString recvData);

public slots:
    void onNewConnection();
    void onDisconnected(QString id);
    void onRecvData(QString clientUUID , QString recvData);


private:
    QScopedPointer<privateStruct> d;

};

NS_SUGENTECH_END

#endif /* USE_QT */

#endif // CTCP_SERVER_H
