#include "Common/CUtil.h"
#include "WriteProtocolCameraLedSetting.h"



USING_SUGENTECH

WriteProtocolCameraLedSetting::WriteProtocolCameraLedSetting()
{

}

QString WriteProtocolCameraLedSetting::writeLEDParamWriteReq(uchar led1, uchar led2, uchar led3, uchar led4)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_LED_LIGHT_POWER_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(led1));
    command.append(CUtil::hexaToHexaString(led2));
    command.append(CUtil::hexaToHexaString(led3));
    command.append(CUtil::hexaToHexaString(led4));

    return command;
}

QString WriteProtocolCameraLedSetting::writeLEDParamReadReq()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_LED_LIGHT_POWER_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString WriteProtocolCameraLedSetting::writeLEDOnOff(PROTOCOL_LED status)
{
    uchar flag = static_cast<uchar> (status);

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_GENERAL_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_GENERAL_SETTING_MODE_LED_POWER_ON_OFF));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(flag));

    return command;
}

QString WriteProtocolCameraLedSetting::writeAnlyXPosSetReq(uint pos)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pos));

    return command;
}

QString WriteProtocolCameraLedSetting::writeAnlyXPosReadReq()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolCameraLedSetting::writeAnlyXEndPosSetReq(uint pos)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pos));

    return command;
}

QString WriteProtocolCameraLedSetting::writeAnlyXEndPosReadReq()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolCameraLedSetting::writeSendCameraPosMoveReq(uchar targetStripNumber)
{
    QString command;

    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_CONTROL_WRITE)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_REQ)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(targetStripNumber));

    return command;
}


QString WriteProtocolCameraLedSetting::writePostCameraPosMoveFinishedReq(uchar stripNumber)
{
    QString command;

    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_CONTROL_WRITE)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_REQ)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stripNumber));

    return command;
}

QString WriteProtocolCameraLedSetting::writeCameraPosMoveFinishedResp(uchar stripNumber)
{
    QString command;

    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_CONTROL_WRITE)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_CONTROL_WRITE_DEVICE_CAMERA_MOVE_RESP)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stripNumber));

    return command;
}

QString WriteProtocolCameraLedSetting::writeStripWidthPosWrite(uint stripWidth)
{
    QString command;
    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_MOTOR_SETTING_MODE)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_SET)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(stripWidth));

    return command;
}

QString WriteProtocolCameraLedSetting::writeStripWidthPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString(static_cast<char> (MD_MODE_MOTOR_SETTING_MODE)));
    command.append(CUtil::hexaToHexaString(static_cast<char>(CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_READ)));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}


