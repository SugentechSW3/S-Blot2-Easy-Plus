#ifndef WRITEPROTOCOLGENERALSETTING_H
#define WRITEPROTOCOLGENERALSETTING_H

#include <QString>
#include "Network/Protocols/Default/ProtocolGeneralSettingMode.h"

class WriteProtocolGeneralSetting
{
public:
    WriteProtocolGeneralSetting();

    enum PROTOCOL_RW
    {
        PROTOCOL_RW_READ,
        PROTOCOL_RW_WRITE
    };

    enum PROTOCOL_WASTE_STATUS
    {
        PROTOCOL_WASTE_STATUS_OFF,
        PROTOCOL_WASTE_STATUS_ON
    };

    static QString writeWasteSensorOnOff(bool isOn);

};

#endif // WRITEPROTOCOLGENERALSETTING_H
