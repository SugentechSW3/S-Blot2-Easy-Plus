#include "WriteProtocolDispAspPage.h"
#include "Common/CUtil.h"


USING_SUGENTECH

WriteProtocolDispAspPage::WriteProtocolDispAspPage()
{

}

QString WriteProtocolDispAspPage::writeXAxisBathPosSet(uint value)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_BATH_X_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisAspPosSet(uint value)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_ASP_X_START_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisDspPosSet(uint value)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_DISP_X_START_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisStripWidthSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraHomePosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAM_HOME_POSITION_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraEndPosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAspPosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeDspPosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeDryPosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeCameraPosSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSampleDspSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SAMPLE_DSP_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeHomeSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_HOME_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeAspPinMove(ushort value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_ASP_JOG_RUN));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeAspPinBathSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeAspPinTraySet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_ASP_PIN_HEIGHT_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSpeedSpeedSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSpeedPeriodSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_PERIOD_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAngleMinSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MIN_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAngleMaxSet(uint value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MAX_POS_SET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisBathPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_BATH_X_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisAspPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_ASP_X_START_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisDspPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_DISP_X_START_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisStripWidthRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraHomePosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAM_HOME_POSITION_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeXAxisCameraEndPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_CAMERA_END_XPOS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAspPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_ASP_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeDspPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DSP_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeDryPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_DRY_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeCameraPosRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_CAMERA_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSampleDspRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SAMPLE_DSP_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeHomeRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_HOME_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}


QString WriteProtocolDispAspPage::writeAspPinBathRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_BATH_ASP_PIN_HEIGHT_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeAspPinTrayRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_TRAY_ASP_PIN_HEIGHT_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSpeedSpeedRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeSpeedPeriodRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_SPEED_PERIOD_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAngleMinRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MIN_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolDispAspPage::writeShakeAngleMaxRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_SHAKE_MAX_POS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

