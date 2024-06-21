#include "WriteProtocolGeneralSetting.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolGeneralSetting::WriteProtocolGeneralSetting()
{

}

QString WriteProtocolGeneralSetting::writeWasteSensorOnOff(bool isOn)
{
    ushort flag  = 0x0100; // Off

    if(isOn == true)
        flag = 0x0101;

    QString command;
    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_GENERAL_SETTING)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_GENERAL_SETTING_MODE_WASTE_SENSOR)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(flag));

    return command;
}

