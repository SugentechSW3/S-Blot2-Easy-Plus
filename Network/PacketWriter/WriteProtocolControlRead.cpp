#include "Common/CUtil.h"
#include "WriteProtocolControlRead.h"

#include "Network/Protocols/Legacy/ProtocolMotorSettingMode.h"


USING_SUGENTECH

WriteProtocolControlRead::WriteProtocolControlRead()
{

}

QString WriteProtocolControlRead::writeSequenceSelect(uchar sequenceMemoryBlock)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_READ_SEQUENCE_SELECT));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(sequenceMemoryBlock));

    return command;
}

QString WriteProtocolControlRead::writeStartStepNumRead(uchar stepNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_READ_START_STEP_NUM));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepNumber));

    return command;
}

QString WriteProtocolControlRead::readSampleTotalNumber()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_READ_SAMPLE_TOTAL_NUM));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolControlRead::writeXAxisHome()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_X_HOMING));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}


QString WriteProtocolControlRead::writeASPHome()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_MOTOR_SETTING_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_MOTOR_SETTING_MODE_ASP_HOME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}




//#endif
