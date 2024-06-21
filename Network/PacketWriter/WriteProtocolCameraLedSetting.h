#ifndef WRITEPROTOCOLCAMERALEDSETTING_H
#define WRITEPROTOCOLCAMERALEDSETTING_H

#include <QString>
#include "Network/Protocols/CameraLED/ProtocolCameraLedSetting.h"
#include "Network/Protocols/Default/ProtocolGeneralSettingMode.h"
#include "Network/Protocols/Default/ProtocolControlWrite.h"
#include "Network/Protocols/Legacy/ProtocolMotorSettingMode.h"
class WriteProtocolCameraLedSetting
{
public:
    WriteProtocolCameraLedSetting();


    enum class PROTOCOL_LED
    {
        PROTOCOL_LED_OFF,
        PROTOCOL_LED_ON
    };

    static QString writeLEDParamWriteReq(uchar led1, uchar led2, uchar led3, uchar led4);
    static QString writeLEDParamWriteReq(ushort channel, ushort power);
    static QString writeLEDParamReadReq();
    static QString writeLEDParamReadReq(ushort channel);

    static QString writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED status);
    static QString writeAnlyXPosSetReq(uint pos);
    static QString writeAnlyXPosReadReq();

    static QString writeAnlyXEndPosSetReq(uint pos);
    static QString writeAnlyXEndPosReadReq();

    static QString writeSendCameraPosMoveReq(uchar targetStripNumber);
    static QString writePostCameraPosMoveFinishedReq(uchar stripNumber);
    static QString writeCameraPosMoveFinishedResp(uchar stripNumber);

    static QString writeStripWidthPosWrite(uint stripWidth);
    static QString writeStripWidthPosRead();

};

#endif // WRITEPROTOCOLCAMERALEDSETTING_H
