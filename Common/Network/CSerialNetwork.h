#ifndef SERIALNETWORK_H
#define SERIALNETWORK_H
#include "Common/CUtilCommon.h"

#ifdef USE_QT
#include <QList>
#include <QObject>
#include <QIODevice>
#include <QSerialPort>
#include <QScopedPointer>



NS_SUGENTECH_BEGIN

class CSerialNetwork : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    enum OPEN_MODE
    {
        OPEN_MODE_READ_ONLY = 0,
        OPEN_MODE_WRITE_ONLY,
        OPEN_MODE_READ_WRITE
    };


    explicit CSerialNetwork(QObject *parent = nullptr);
    virtual ~CSerialNetwork();


    bool open(CSerialNetwork::OPEN_MODE openMode = CSerialNetwork::OPEN_MODE_READ_WRITE);
    bool open(CSerialNetwork::OPEN_MODE openMode, QSerialPort::BaudRate baudRate = QSerialPort::Baud57600,
              QSerialPort::DataBits dataBits = QSerialPort::Data8, QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl,
              QSerialPort::Parity parity = QSerialPort::NoParity, QSerialPort::StopBits stopBits = QSerialPort::OneStop);


    void close();
    bool isOpen();

    bool sendData(QByteArray &data);
    static QString getConnectedSerialPortName();

signals:
    void recvData(QByteArray recvData);

private:
     QIODevice::OpenMode getOpenMode(OPEN_MODE openMode);

private slots:
     void recvingData();

private:
    QScopedPointer<privateStruct> d;

};

NS_SUGENTECH_END


#endif /*END OF USE_QT */

#endif // SERIALNETWORK_H
