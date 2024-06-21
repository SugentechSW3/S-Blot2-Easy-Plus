#include "WriteProtocolPageRead.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolPageRead::WriteProtocolPageRead()
{

}

QString WriteProtocolPageRead::writePositionPage()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_POSITION_PAGE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolPageRead::writeLLDPage()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_LLD_PAGE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolPageRead::writeDispensePage()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_DISPENSE_PAGE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolPageRead::writeCameraPage()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_CAMERA_PAGE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolPageRead::writeETCPage()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_ETC_PAGE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}
