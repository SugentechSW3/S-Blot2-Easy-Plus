#ifndef CCOMMONWIDGET_H
#define CCOMMONWIDGET_H

#include <Common/CUtilCommon.h>
#include <Network/SerialCtrl.h>
#include <Network/Protocols/SerialProtocol.h>

#ifdef USE_QT
#include <QWidget>
#include <QScopedPointer>

//this class is interface class
class CSerialNetworkWidget : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;

public:
    explicit CSerialNetworkWidget(QWidget *parent = nullptr);
    virtual ~CSerialNetworkWidget();

// 구현부에 접근하는 인터페이스
    void start();
    void stop();

signals:
    void onReadReady(QByteArray recvData);
    void onConnectedNetwork();
    void onDisconnectNetwork();


protected:
   void init();
   SerialCtrl* getSerialInstance();
   void onNonRespSend(const QByteArray& sendData);
   void onQueuedSend(const QByteArray& sendData);

private slots:
    virtual void onRecv(QByteArray recvData);

//실제 구현부(상속 인터페이스)
private:
    //init과 관련있음
    virtual void initVariables() = 0;
    virtual void initSignalSlots() = 0;

    //start stop과 관련있음
    virtual void startWidget() = 0;
    virtual void stopWidget() = 0;
private:
    QScopedPointer<privateStruct> d;
};

#endif

#endif // CCOMMONWIDGET_H
