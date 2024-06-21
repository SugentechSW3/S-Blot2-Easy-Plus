#ifndef WRITEPROTOCOLDIWATERPAGE_H
#define WRITEPROTOCOLDIWATERPAGE_H

#include <QString>

#include "Network/Protocols/Legacy/ProtocolPumpSettingMode.h"


class WriteProtocolDIWaterPage
{
public:
    WriteProtocolDIWaterPage();

    static QString writeRunAutoCalibrationSet();
    static QString writeRunAutoCalibrationExcu();
    static QString writePumpUseTimeSet(uchar pumpNum, ushort time);
    static QString writePumpUseTimeRead(uchar pumpNum);
    static QString writePumpOffsetSet(uchar pumpNum, ushort offset);
    static QString writeAutoCalibration();
    static QString writeAmountBathRead();
    static QString writePumpOffsetRead(uchar pumpCh);

};

#endif // WRITEPROTOCOLDIWATERPAGE_H
