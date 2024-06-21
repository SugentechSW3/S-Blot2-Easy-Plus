#ifndef WRITEPROTOCOLCONTROLREAD_H
#define WRITEPROTOCOLCONTROLREAD_H

#include <QString>
#include "Network/Protocols/Default/ProtocolControlRead.h"

class WriteProtocolControlRead
{
public:
    WriteProtocolControlRead();

//#if !USE_SCAN
    static QString writeSequenceSelect(uchar sequenceMemoryBlock);
    static QString writeStartStepNumRead(uchar stepNumber);
    static QString readSampleTotalNumber();
    static QString writeXAxisHome();


    static QString writeASPHome();


//#endif
};

#endif // WRITEPROTOCOLCONTROLREAD_H
