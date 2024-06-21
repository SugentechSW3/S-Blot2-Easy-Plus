#include "WriteProtocolTraySetting.h"
#include "Common/CUtil.h"



USING_SUGENTECH

WriteProtocolTraySetting::WriteProtocolTraySetting()
{

}

QString WriteProtocolTraySetting::writeTrayAspPosSet(int position)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));
    return command;
}

QString WriteProtocolTraySetting::writeTrayAspPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString WriteProtocolTraySetting::writeTrayDispPosSet(int position)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));
    return command;
}

QString WriteProtocolTraySetting::writeTrayDispPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString WriteProtocolTraySetting::writeTrayAnalysisPosSet(int position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolTraySetting::writeTrayAnalysisPosRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolTraySetting::writeTrayDryPosSet(int position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolTraySetting::writeTrayDryPosRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolTraySetting::writeTraySpeedPosSet(int position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolTraySetting::writeTraySpeedPosRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolTraySetting::writeTrayHome()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_HOME_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}


QString WriteProtocolTraySetting::writeTrayAngleUpSet(int position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MAX_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolTraySetting::writeTrayAngleUpRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MAX_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolTraySetting::writeTrayAngleDownSet(int position)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MIN_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(position));

    return command;
}

QString WriteProtocolTraySetting::writeTrayAngleDownRead()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MIN_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}
