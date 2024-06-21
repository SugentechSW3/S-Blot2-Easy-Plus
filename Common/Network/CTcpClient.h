#ifndef CTCP_CLIENT_H
#define CTCP_CLIENT_H

#include <QObject>
#include "Common/CUtilCommon.h"

NS_SUGENTECH_BEGIN

class CTcpClient : public QObject
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit CTcpClient(QObject *parent = nullptr);
    virtual ~CTcpClient();

    void setIpAddressAndPort(const QString& ipAddress, int port);
    bool sendData(const QByteArray& data);
    bool connectToServer();


signals:
    void onRecvData(QByteArray recvData);

private slots:
    void onRecvLISServer();

private:
    QScopedPointer<privateStruct> d;
};
NS_SUGENTECH_END


#endif // CTCPCLIENT_H
