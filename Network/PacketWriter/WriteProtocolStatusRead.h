#ifndef WRITEPROTOCOLSTATUSREAD_H
#define WRITEPROTOCOLSTATUSREAD_H

#include <QString>
#include "Network/Protocols/Default/ProtocolStatusRead.h"

class WriteProtocolStatusRead
{
public:
    WriteProtocolStatusRead();

    static QString writeHeartBeat();
    static QString writeEquipmentSerialNumber();
    static QString writeFirmwareVersion();
    static QString writeHardwareVersion();
    static QString writeReadStepID(uchar stepIdx);
    static QString writeTotalSqNumber();
    static QString writeSqID(uchar sequenceMemoryBlock);
    static QString writeSequenceSelectR();
    static QString writeTotalStepNumber();
    static QString writeCurrentTotalTime();
    static QString writeCurrentStepTime();
    static QString writeCurrentDeviceState();
    static QString writeStepTimeRead(uchar stepIdx);
    static QString writeCurrentStatusRead();


};

#endif // WRITEPROTOCOLSTATUSREAD_H
