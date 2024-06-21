#include "CPacketTimer.h"
#include "Common/CLogWriter.h"

// Creater      : sp.Park by SystemDevelopment
// Description  : Public C++ 11 Util Class
// Created Date : 2017-08-17
// Modifyied    : temp disable timer
//const bool disable = false;

NS_SUGENTECH_BEGIN

struct CPacketTimer::privateStruct
{
    bool mIsSended = false;
};

CPacketTimer::CPacketTimer(QObject* parent) :
    QTimer(parent),
    d(new privateStruct)
{
    connect(this, SIGNAL(timeout()), SLOT(timeOutPacket()));

    this->setSingleShot(true);
}

CPacketTimer::~CPacketTimer()
{

}

void CPacketTimer::startPacket(int msec)
{
    this->start(msec);
    d->mIsSended = true;
}

void CPacketTimer::stopPacket()
{
    this->stop();
    d->mIsSended = false;
}

bool CPacketTimer::isSended()
{
    return d->mIsSended;
}


void CPacketTimer::timeOutPacket()
{
    this->stopPacket();
    emit packetTimeOut();
}


NS_SUGENTECH_END
