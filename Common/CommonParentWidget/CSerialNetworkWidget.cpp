#include <QThread>

#include "CSerialNetworkWidget.h"

#include "UIClass/DialogWidget/ProgressBarDlg.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/Protocols/Default/ProtocolControlWrite.h"

#include "Common/CLogWriter.h"
#include "Common/CUtil.h"


USING_SUGENTECH

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2018-03-19

#ifdef USE_QT

struct CSerialNetworkWidget::privateStruct
{
    SerialCtrl* mSerial;
    bool mIsStarted = false;
};

CSerialNetworkWidget::CSerialNetworkWidget(QWidget *parent) :
    QWidget(parent),
    d(new privateStruct)
{
    d->mSerial = SerialCtrl::getInstance();
    d->mSerial->openSerial();
}

CSerialNetworkWidget::~CSerialNetworkWidget()
{

}

void CSerialNetworkWidget::init()
{
    connect(d->mSerial, &SerialCtrl::onRecvDataForSerial, this, &CSerialNetworkWidget::onRecv);
    connect(d->mSerial, &SerialCtrl::onDeviceIsConnected, this, &CSerialNetworkWidget::onConnectedNetwork);
    connect(d->mSerial, &SerialCtrl::onDeviceIsDisConnected, this, &CSerialNetworkWidget::onDisconnectNetwork);

    this->initVariables();
    this->initSignalSlots();
}

SerialCtrl* CSerialNetworkWidget::getSerialInstance()
{
    return d->mSerial;
}

void CSerialNetworkWidget::onNonRespSend(const QByteArray& sendData)
{
    if(!sendData.isEmpty())
        d->mSerial->writeData(sendData, false);
}

void CSerialNetworkWidget::onQueuedSend(const QByteArray& sendData)
{
    if(!sendData.isEmpty())
        d->mSerial->writeQueuedData(sendData);
}

void CSerialNetworkWidget::onRecv(QByteArray recvData)
{
    emit onReadReady(recvData);
}

void CSerialNetworkWidget::start()
{   
    if(d->mIsStarted)
        return;

    d->mIsStarted = true;
    this->startWidget();
}

void CSerialNetworkWidget::stop()
{
    if(!d->mIsStarted)
        return;

    d->mIsStarted = false;
    this->stopWidget();
}


#endif /* END OF USE_QT */
