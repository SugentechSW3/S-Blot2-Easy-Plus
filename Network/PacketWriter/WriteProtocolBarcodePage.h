#ifndef WRITEPROTOCOLBARCODEPAGE_H
#define WRITEPROTOCOLBARCODEPAGE_H

#include <QString>
#include "Network/Protocols/Position/ProtocolBarcodeSettingMode.h"

class WriteProtocolBarcodePage
{
public:
    WriteProtocolBarcodePage();

    static QString writeQRTrayMove();
    static QString writeBarcodeQRTrayPosSet(uint position);
    static QString writeBarcodeQRTrayPosRead();

};

#endif // WRITEPROTOCOLBARCODEPAGE_H
