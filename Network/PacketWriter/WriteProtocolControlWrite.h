#ifndef WRITEPROTOCOLCONTROLWRITE_H
#define WRITEPROTOCOLCONTROLWRITE_H

#include <QString>
#include "Network/Protocols/Default/ProtocolControlWrite.h"

class WriteProtocolControlWrite
{
public:
    WriteProtocolControlWrite();

    static QString writeSequenceSelect(uchar sequenceMemoryBlock);
    static QString writeSequenceStart(uchar stepNumber);
    static QString writeStepSingleRun(uchar stepNumber);
    static QString writeStepProcessSingleRun(uchar stepNumber, uchar processNumber);
    static QString writeSequenceStop();
    static QString writePause(uchar pause);
    static QString writeDeviceRESET();

    static QString writeSampleTotalNumber(uchar panel1, uchar panel2, uchar panel3, uchar stripCount);
    static QString writeBuzzerONOFF();
    static QString writeAspirationSel();

    static QString writeAutoClean(ushort count = 0 , ushort primeTime = 0);

};

#endif // WRITEPROTOCOLCONTROLWRITE_H
