#include "Network/Protocols/Default/ProtocolControlRead.h"
#include "WriteProtocolControlWrite.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolControlWrite::WriteProtocolControlWrite()
{

}

QString WriteProtocolControlWrite::writeSequenceSelect(uchar sequenceMemoryBlock)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_SEQUENCE_SELECT_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(sequenceMemoryBlock));

    return command;
}

QString WriteProtocolControlWrite::writeSequenceStart(uchar stepNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_CONTINUOUS_RUN_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepNumber));

    return command;
}

QString WriteProtocolControlWrite::writeStepSingleRun(uchar stepNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_STEP_SINGLE_RUN_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepNumber));

    return command;
}

//user sequence
QString WriteProtocolControlWrite::writeStepProcessSingleRun(uchar stepNumber, uchar processNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_STEP_PROCESS_SINGLE_RUN_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepNumber));
    command.append(CUtil::hexaToHexaString(processNumber));

    return command;
}

QString WriteProtocolControlWrite::writeSequenceStop()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_SEQUENCE_STOP_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolControlWrite::writePause(uchar pause)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_PAUSE_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(pause));

    return command;
}

QString WriteProtocolControlWrite::writeDeviceRESET()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_DEVICE_RESET_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString((uchar)0xa1));
    command.append(CUtil::hexaToHexaString((uchar)0xb2));
    command.append(CUtil::hexaToHexaString((uchar)0xc3));
    command.append(CUtil::hexaToHexaString((uchar)0xd4));

    return command;
}

QString WriteProtocolControlWrite::writeSampleTotalNumber(uchar panel1, uchar panel2, uchar panel3, uchar stripCount)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_SAMPLE_TOTAL_NUMBER_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(panel1));
    command.append(CUtil::hexaToHexaString(panel2));
    command.append(CUtil::hexaToHexaString(panel3));
    command.append(CUtil::hexaToHexaString(stripCount));
    return command;
}


QString WriteProtocolControlWrite::writeBuzzerONOFF()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_BUZZER_ON_OFF_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));
    return command;
}

QString WriteProtocolControlWrite::writeAspirationSel()
{
    uchar fixedValue = 0x01;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_ASPIRATION_SEL_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(fixedValue));

    return command;
}



QString WriteProtocolControlWrite::writeAutoClean(ushort count, ushort primeTime)
{
    QString command;

    command.append(CUtil::hexaToHexaString((char)MD_MODE_CONTROL_WRITE));
    command.append(CUtil::hexaToHexaString((char)CMD_CONTROL_WRITE_AUTO_CLEAN_REQ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}
