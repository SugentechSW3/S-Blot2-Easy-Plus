#include "WriteProtocolBarcodePage.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolBarcodePage::WriteProtocolBarcodePage()
{

}


QString WriteProtocolBarcodePage::writeQRTrayMove()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_SETTING_MODE_QR_TRAY_POS_MOVE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolBarcodePage::writeBarcodeQRTrayPosSet(uint position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_QR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_SETTING_MODE_QR_TRAY_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolBarcodePage::writeBarcodeQRTrayPosRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_QR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_SETTING_MODE_QR_TRAY_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

