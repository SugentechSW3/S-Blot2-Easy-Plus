#include "WriteProtocolDIWaterPage.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolDIWaterPage::WriteProtocolDIWaterPage()
{

}


QString WriteProtocolDIWaterPage::writeRunAutoCalibrationSet()
{
    ushort autoCalibrationVol = 0x03E8;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_PUMP_RUN_AUTO_CALIBRATION_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(autoCalibrationVol));

    return command;
}

QString WriteProtocolDIWaterPage::writeRunAutoCalibrationExcu()
{
    ushort autoCalibrationVol = 0x03E8;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_PUMP_RUN_AUTO_CALIBRATION_EXCU));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(autoCalibrationVol));

    return command;
}

QString WriteProtocolDIWaterPage::writePumpUseTimeSet(uchar pumpNum, ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_USE_TIME_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpNum));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolDIWaterPage::writePumpUseTimeRead(uchar pumpNum)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_USE_TIME_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpNum));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    return command;
}

QString WriteProtocolDIWaterPage::writePumpOffsetSet(uchar pumpNum, ushort offset)
{
    uchar dummyData = 0x01;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_OFFSET_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpNum));
    command.append(CUtil::hexaToHexaString(dummyData));
    command.append(CUtil::hexaToHexaString(offset));

    return command;
}

QString WriteProtocolDIWaterPage::writeAutoCalibration()
{
    ushort autoCalibrationVol = 0x03E8;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_PUMP_RUN_AUTO_CALIBRATION_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(autoCalibrationVol));
    return command;
}

QString WriteProtocolDIWaterPage::writeAmountBathRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_AMOUNT_BATH_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString WriteProtocolDIWaterPage::writePumpOffsetRead(uchar pumpCh)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_PUMP_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_PUMP_SETTING_MODE_OFFSET_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpCh));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));

    return command;
}
