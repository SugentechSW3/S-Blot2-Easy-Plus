#ifndef WRITEPROTOCOLDISPASPPAGE_H
#define WRITEPROTOCOLDISPASPPAGE_H

#include <QString>
#include "Network/Protocols/Position/ProtocolDispAspSettingMode.h"

#include "Network/Protocols/Legacy/ProtocolMotorSettingMode.h"


class WriteProtocolDispAspPage
{
public:
    WriteProtocolDispAspPage();

    static QString writeXAxisBathPosSet(uint value);
    static QString writeXAxisAspPosSet(uint value);
    static QString writeXAxisDspPosSet(uint value);
    static QString writeXAxisCameraSet(uint value);
    static QString writeXAxisStripWidthSet(uint value);
    static QString writeXAxisCameraHomePosSet(uint value);
    static QString writeXAxisCameraEndPosSet(uint value);

    static QString writeShakeAspPosSet(uint value);
    static QString writeShakeDspPosSet(uint value);
    static QString writeShakeDryPosSet(uint value);
    static QString writeShakeCameraPosSet(uint value);
    static QString writeShakeSampleDspSet(uint value);
    static QString writeShakeHomeSet(uint value);

    static QString writeAspPinMove(ushort value);
    static QString writeAspPinBathSet(uint value);
    static QString writeAspPinTraySet(uint value);

    static QString writeShakeSpeedSpeedSet(uint value);
    static QString writeShakeSpeedPeriodSet(uint value);

    static QString writeShakeAngleMinSet(uint value);
    static QString writeShakeAngleMaxSet(uint value);


    static QString writeXAxisBathPosRead();
    static QString writeXAxisAspPosRead();
    static QString writeXAxisDspPosRead();
    static QString writeXAxisCameraRead();
    static QString writeXAxisStripWidthRead();
    static QString writeXAxisCameraHomePosRead();
    static QString writeXAxisCameraEndPosRead();

    static QString writeShakeAspPosRead();
    static QString writeShakeDspPosRead();
    static QString writeShakeDryPosRead();
    static QString writeShakeCameraPosRead();
    static QString writeShakeSampleDspRead();
    static QString writeShakeHomeRead();

    static QString writeAspPinBathRead();
    static QString writeAspPinTrayRead();

    static QString writeShakeSpeedSpeedRead();
    static QString writeShakeSpeedPeriodRead();

    static QString writeShakeAngleMinRead();
    static QString writeShakeAngleMaxRead();

};

#endif // WRITEPROTOCOLDISPASPPAGE_H
