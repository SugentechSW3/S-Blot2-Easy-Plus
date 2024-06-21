#ifndef WRITEPROTOCOLETCPAGE_H
#define WRITEPROTOCOLETCPAGE_H

#include <QString>
#include "Network/Protocols/ETC/ProtocolETC.h"
#include "Network/Protocols/Legacy/ProtocolQCMode.h"


class WriteProtocolETCPage
{
public:
    WriteProtocolETCPage();

    static QString writeDryTestSet(ushort speed, uchar time);
    static QString writeAspirationTestSet(uchar stripIdx, uchar time);
    static QString writeDispenseTestSet(uchar channel, uchar sampleIdx, ushort volume);
    static QString writeRollBackTest(uchar channel, uchar time);
    static QString writePrimeTest(uchar channel, uchar time);
    static QString writeIncubation(ushort time);

    static QString writeFanTestONOFF(bool isON);
    static QString writeStatusLEDONOFF(bool isON);

    static QString writeHeatingPadTempCh1Set(int value);
    static QString writeHeatingPadTempCh1Read();
    static QString writeHeatingPadTempCh2Set(int value);
    static QString writeHeatingPadTempCh2Read();
    static QString writeHeatingPadONOFF(bool isOn);
    static QString writeTemperatureValue(uint volt);
    static QString writeAdcRawDataReverseTemperature(ushort volt, ushort res);


};

#endif // WRITEPROTOCOLETCPAGE_H
