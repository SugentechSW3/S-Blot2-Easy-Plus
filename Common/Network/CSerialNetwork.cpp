#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMutex>
#include <QDebug>
#include "CSerialNetwork.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19

NS_SUGENTECH_BEGIN

//시리얼 포트클래스
//담당기능 : 연결, 연결종료 인터페이스, 재전송기능, 데이터 송수신 보장
struct CSerialNetwork::privateStruct
{
    QSerialPort mSerialPort;
    QMutex mMutex;
    bool mIsConnected = false;
};

CSerialNetwork::CSerialNetwork(QObject *parent) :
    QObject(parent),
    d(new privateStruct)
{
    connect(&d->mSerialPort, SIGNAL(readyRead()), SLOT(recvingData()), Qt::QueuedConnection);
    connect(&d->mSerialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error), this, []
            (QSerialPort::SerialPortError error)
            {
                QString errorStr = CUtil::convertQEnumToQString(int(error), "SerialPortError", QSerialPort::staticMetaObject);

                if(error != QSerialPort::NoError)
                    CLogWriter::printLog(QString(">>> [Serial I/O ERROR] Error Number: %1 / Error Constant: %2").arg(error).arg(errorStr));
            });
}

//ScopedPointer사용시 소멸자 필수
CSerialNetwork::~CSerialNetwork()
{
    this->close();
}

bool CSerialNetwork::open(CSerialNetwork::OPEN_MODE openMode)
{
    QString portName = CSerialNetwork::getConnectedSerialPortName();

    if(portName.isEmpty())
        return false;

    d->mSerialPort.setPortName(portName);

    return d->mSerialPort.open(this->getOpenMode(openMode));
}

bool CSerialNetwork::open(OPEN_MODE openMode, QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits,
                         QSerialPort::FlowControl flowControl, QSerialPort::Parity parity, QSerialPort::StopBits stopBits)
{
    QString portName = CSerialNetwork::getConnectedSerialPortName();
    d->mSerialPort.setPortName(portName);

    if(d->mSerialPort.open(this->getOpenMode(openMode)) == false)
        return false;

    if(portName.isEmpty())
        return false;

    if(d->mSerialPort.setBaudRate(baudRate) == false)
        return false;

    if(d->mSerialPort.setDataBits(dataBits) == false)
        return false;

    if(d->mSerialPort.setFlowControl(flowControl) == false)
        return false;

    if(d->mSerialPort.setParity(parity) == false)
        return false;

    if(d->mSerialPort.setStopBits(stopBits) == false)
        return false;

    return true;
}

void CSerialNetwork::close()
{
    if(d->mSerialPort.isOpen())
        d->mSerialPort.close();
}

bool CSerialNetwork::isOpen()
{
    return d->mSerialPort.isOpen();
}

bool CSerialNetwork::sendData(QByteArray& data)
{
    if(!this->isOpen())
        return false;

    QMutexLocker locker(&d->mMutex);
    d->mSerialPort.write(data);

    return true;
}

QString CSerialNetwork::getConnectedSerialPortName()
{
    QSerialPortInfo info;

    auto infomation = info.availablePorts();

    if(infomation.count() == 0)
        return QString();

    QString portName;

    for(auto itr : infomation)
    {
        if(itr.manufacturer() == QString("FTDI"))
        {
            portName = itr.portName();
            break;
        }
    }

    return portName;
}

QIODevice::OpenMode CSerialNetwork::getOpenMode(OPEN_MODE openMode)
{
    switch(openMode)
    {
    case OPEN_MODE_READ_ONLY :
        return QIODevice::ReadOnly;

    case OPEN_MODE_WRITE_ONLY :
        return QIODevice::WriteOnly;

    case OPEN_MODE_READ_WRITE :
        return QIODevice::ReadWrite;
    }

    return QIODevice::NotOpen;
}

void CSerialNetwork::recvingData()
{
   QByteArray readData = d->mSerialPort.readAll();

   emit recvData(readData);
}


NS_SUGENTECH_END
