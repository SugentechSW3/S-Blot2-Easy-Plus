#ifndef WRITEPROTOCOLTRAYSETTING_H
#define WRITEPROTOCOLTRAYSETTING_H

#include <QString>
#include "Network/Protocols/Position/ProtocolTraySettingMode.h"
#include "Network/Protocols/Legacy/ProtocolMotorSettingMode.h"

class WriteProtocolTraySetting
{
public:
    WriteProtocolTraySetting();


    static QString writeTrayAspPosSet(int position);
    static QString writeTrayAspPosRead();
    static QString writeTrayDispPosSet(int position);
    static QString writeTrayDispPosRead();

    static QString writeTrayAnalysisPosSet(int position);
    static QString writeTrayAnalysisPosRead();
    static QString writeTrayDryPosSet(int position);
    static QString writeTrayDryPosRead();
    static QString writeTraySpeedPosSet(int position);
    static QString writeTraySpeedPosRead();
    static QString writeTrayHome();

    static QString writeTrayAngleUpSet(int position);
    static QString writeTrayAngleUpRead();
    static QString writeTrayAngleDownSet(int position);
    static QString writeTrayAngleDownRead();

};

#endif // WRITEPROTOCOLTRAYSETTING_H
