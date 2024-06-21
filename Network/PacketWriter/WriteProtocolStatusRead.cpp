#include "WriteProtocolStatusRead.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolStatusRead::WriteProtocolStatusRead()
{

}

QString WriteProtocolStatusRead::writeHeartBeat()
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_DEVICE_CHECK_LEGACY));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeEquipmentSerialNumber()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_EQUIPMENT_SERIAL_NUMBER));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeFirmwareVersion()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_FIRMWARE_VERSION));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeHardwareVersion()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_HARDWARE_VERSION));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeReadStepID(uchar stepIdx)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_STEP_ID));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepIdx));

    return command;
}

QString WriteProtocolStatusRead::writeTotalSqNumber()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_TOTAL_SQ_NUMBER));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeSqID(uchar sequenceMemoryBlock)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_SQ_ID));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(sequenceMemoryBlock));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));

    return command;
}

QString WriteProtocolStatusRead::writeSequenceSelectR()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_SEQUENCE_SELECT_R));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeTotalStepNumber()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_TOTAL_STEP_NUMBER));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeCurrentTotalTime()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_CURRENT_TOTAL_TIME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeCurrentStepTime()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_CURRENT_STEP_TIME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolStatusRead::writeCurrentDeviceState()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_CURRENT_DEVICE_STATE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));


    return command;
}

QString WriteProtocolStatusRead::writeStepTimeRead(uchar stepIdx)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_STEP_TIME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepIdx));

    return command;
}


QString WriteProtocolStatusRead::writeCurrentStatusRead()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_STATUS_READ));
    command.append(CUtil::hexaToHexaString((char)CMD_STATUS_READ_CURRENT_STATUS_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

