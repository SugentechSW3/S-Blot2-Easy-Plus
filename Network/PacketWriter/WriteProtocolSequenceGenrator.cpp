#include "WriteProtocolSequenceGenrator.h"
#include "Common/CUtil.h"
#include "Network/Protocols/Legacy/ProtocolQCMode.h"


USING_SUGENTECH

WriteProtocolSequenceGenrator::WriteProtocolSequenceGenrator()
{

}

//#if !USE_SCAN
QString WriteProtocolSequenceGenrator::writeSQInputMode(uchar pMode)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_INPUT_MODE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(pMode));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQTotalStep(uchar totalStep)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_TOTAL_STEP));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(totalStep));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQSave(uchar smb)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_SAVE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(smb));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQID(uchar smb, ushort sqID)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_ID));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(smb));
    command.append(CUtil::hexaToHexaString(sqID));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQStepNumber(uchar stepNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_STEP_NUMBER));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stepNumber));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessNumber(ushort pfID, ushort processNumber)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_NUMBER));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pfID));
    command.append(CUtil::hexaToHexaString(processNumber));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterByte(uchar d1, uchar d2, uchar d3, uchar d4)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_BYTE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(d1));
    command.append(CUtil::hexaToHexaString(d2));
    command.append(CUtil::hexaToHexaString(d3));
    command.append(CUtil::hexaToHexaString(d4));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterWord(ushort d1, ushort d2)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_WORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(d1));
    command.append(CUtil::hexaToHexaString(d2));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWord(uint d1)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(d1));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordPrime1(uchar pumpChannel, ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordPrime2(uchar pumpChannel1, uchar pumpChannel2, ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel1));
    command.append(CUtil::hexaToHexaString(pumpChannel2));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordPrime3(uchar pumpChannel1, uchar pumpChannel2, uchar pumpChannel3, ushort time)
{
    QString pc12;
    pc12.append(CUtil::hexaToHexaString(pumpChannel1).right(1)); //0x0F = F
    pc12.append(CUtil::hexaToHexaString(pumpChannel2).right(1)); //0x0F = F

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(pc12); //0x0F + 0xF0 = 0xFF
    command.append(CUtil::hexaToHexaString(pumpChannel3));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispense1(uchar pumpChannel, ushort volume)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispense2(uchar pumpChannel1, uchar pumpChannel2, ushort volume)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel1));
    command.append(CUtil::hexaToHexaString(pumpChannel2));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispAsp1(uchar pumpChannel, ushort volume)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispAsp2(uchar pumpChannel1, uchar pumpChannel2, ushort volume)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel1));
    command.append(CUtil::hexaToHexaString(pumpChannel2));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDispAsp3(uchar pumpChannel1, uchar pumpChannel2, uchar pumpChannel3, ushort volume)
{
    QString pc12;
    pc12.append(CUtil::hexaToHexaString(pumpChannel1).right(1)); //0x0F
    pc12.append(CUtil::hexaToHexaString(pumpChannel2).right(1)); //0x0F

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(pc12); //0x0F + 0xF0 = 0xFF
    command.append(CUtil::hexaToHexaString(pumpChannel3));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordIncubation(ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordIncubationWS(ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordAspiration(ushort time, uchar count)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(count));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordRollBack1(uchar channel, ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}


QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordDry(ushort speed, uchar temp, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(speed));
    command.append(CUtil::hexaToHexaString(temp));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDWordBuzzer(uchar buzzerCount)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(buzzerCount));

    return command;
}



QString WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDPrime(uchar channel, uchar option, ushort volumn)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(option));
    command.append(CUtil::hexaToHexaString(volumn));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDRollBack(uchar channel, uchar option, ushort duration)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(option));
    command.append(CUtil::hexaToHexaString(duration));

    return command;
}

QString WriteProtocolSequenceGenrator::writeSQProcessParameterDwordDDispense(uchar channel, uchar option, ushort volumn)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(option));
    command.append(CUtil::hexaToHexaString(volumn));

    return command;
}




QString WriteProtocolSequenceGenrator::writeQCPumpDispTest(uchar pump, uchar sampleNumber, ushort dispVol)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_DISPENSE_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pump));
    command.append(CUtil::hexaToHexaString(sampleNumber));
    command.append(CUtil::hexaToHexaString(dispVol));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCPumpAspTest(uchar sampleNumber, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_ASP_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(sampleNumber));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCPrimeTest(uchar pumpChannel, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_PRIME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCRollBack(uchar pumpChannel, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_ROLL_BACK));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpChannel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCShakeTest(uchar speed, ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_SHAKE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(speed));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCDryTest(ushort speed, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_DRY));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(speed));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCPumpTest(uchar pumpCh)
{
    uchar sampleCount = 3;
    ushort dispVol = 1000;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_PUMP_OFFSET));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(pumpCh));
    command.append(CUtil::hexaToHexaString(sampleCount));
    command.append(CUtil::hexaToHexaString(dispVol));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCBathDispCheck()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_BATH_DSP));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCBathAspCheck()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_BATH_ASP));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolSequenceGenrator::writeQCFanTest()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FAN_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

//1 off ,2 on
QString WriteProtocolSequenceGenrator::writeQCStatusLed(bool state)
{
    uchar onOffFlag = 0x06; //off

    if(state == true)
        onOffFlag = 0x01;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_STATUS_LED));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(onOffFlag));

    return command;
}


QString WriteProtocolSequenceGenrator::writeSQProcessNULLData()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_SEQUENCE_GENERATOR_SETTING));
    command.append(CUtil::hexaToHexaString((char)CMD_SEQUENCE_GENERATOR_SQ_PROCESS_PARAMETER_DWORD));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

