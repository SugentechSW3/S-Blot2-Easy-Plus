#ifndef SERIALCTRL_H
#define SERIALCTRL_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QScopedPointer>

#include "Protocols/SerialProtocol.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : SerialControlClass
// Created Date : 2018-01-01
// Modified    : 2018-03-26 // PACKET_TIME_OUT 1000 -> 3000(ms) -> 10000(ms)
// Modified    : 2019-11-06 // TRANSFER_SPEED ? -> 50(ms)

const int PACKET_TIME_OUT = 30000;
const int TRANSFER_SPEED = 50;
const int SEND_RETRY_COUNT = 5;

//this class is singleton class
//do not use multiful thread or paraell
class SerialCtrl : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(SerialCtrl)
    struct privateStruct;

    explicit SerialCtrl(QObject *parent = nullptr);
    virtual ~SerialCtrl();

public:
    static SerialCtrl* getInstance()
    {
        static SerialCtrl instance;
        return &instance;
    }

public:
    bool openSerial();
    bool isConnected();
    void closeSerial();
    void closeApplication();

    bool inputSequence(const QVector<QByteArray> &protocolQueue);
    void initPacketFailCount();

public slots:
    void writeQueuedData(const QByteArray &sendData);
    void writeQueuedDataForPushFront(const QByteArray& sendData);
    void writeData(const QByteArray &data, const bool isNeedResp = true, const bool isNeedGUIDisable = true);

    void onConnectCallBack();
    void onDisConnectCallBack();
    void writeDirectDataTansmit(const QByteArray& data);

signals:
    void onSendedData(bool isNeedResp);
    void onRecvDataForSerial(QByteArray data);
    void onResponseFromDevice();

    void onSendControl(QByteArray recvData);

    void onDeviceIsConnected();
    void onDeviceIsDisConnected();
    void onDeviceIsBusy();
    void onDeviceSendError();
    void onNetworkErrorMsg(QString msg);

    void onSequenceSendFinished();
    void onSetForcedEnable();

    void onResponseResetCommand();
    void onResponseResetEndCommand();

    void onRecvHeartBeatCommand();
private:
    void initSignalSlots();
    void stackRecvData(QByteArray &queue, const QByteArray recvData);

    void useSetXPacket(QByteArray queue);

    void packetBroadCaster(QByteArray& queue);

    void successProcessing(QByteArray &queue);

    void purgeToNextSTX(QByteArray& queue);

    void attachStxEtxChecksum(QByteArray& data);

    bool checkSumCheck(const QByteArray &data);

    bool isReadyMove(const QByteArray& queue);

    void stopReadyProtocol();

private slots:
    void onCallBackFunction(QByteArray recvData);
    void onRecvControl(QByteArray recvData);
    void onRecvQueueControl();
    void onDefaultControl(const QByteArray& recvData);
    void onSequenceControl(const QByteArray& data);
    void onClearSerial();

private:
    QScopedPointer<privateStruct> d;
};

#endif // SERIALCTRL_H
