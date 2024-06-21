#include "WriteProtocolETCPage.h"
#include "Common/CUtil.h"

USING_SUGENTECH

WriteProtocolETCPage::WriteProtocolETCPage()
{
}

QString WriteProtocolETCPage::writeDryTestSet(ushort speed, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_DRY));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(speed));
    command.append(CUtil::hexaToHexaString(time));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));

    return command;
}

QString WriteProtocolETCPage::writeAspirationTestSet(uchar stripIdx, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_ASP_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(stripIdx));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolETCPage::writeDispenseTestSet(uchar channel, uchar sampleIdx, ushort volume)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_DISPENSE_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(sampleIdx));
    command.append(CUtil::hexaToHexaString(volume));

    return command;
}

QString WriteProtocolETCPage::writeRollBackTest(uchar channel, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_DISPENSE_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolETCPage::writePrimeTest(uchar channel, uchar time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_PRIME));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(channel));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolETCPage::writeIncubation(ushort time)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FUNCTION_SHAKE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(time));

    return command;
}

QString WriteProtocolETCPage::writeFanTestONOFF(bool isON)
{
    uchar flag = 1;

    if(isON == false)
        flag = 2;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_FAN_TEST));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(flag));

    return command;
}

QString WriteProtocolETCPage::writeStatusLEDONOFF(bool isON)
{
    uchar flag = 0x01;

    if(isON == false)
        flag = 0x06;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_STATUS_LED));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(NULL_PADDING));
    command.append(CUtil::hexaToHexaString(flag));

    return command;
}

QString WriteProtocolETCPage::writeHeatingPadTempCh1Set(int value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_WRITE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolETCPage::writeHeatingPadTempCh1Read()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH1_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolETCPage::writeHeatingPadTempCh2Set(int value)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_WRITE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(value));

    return command;
}

QString WriteProtocolETCPage::writeHeatingPadTempCh2Read()
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMP_CH2_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(NULL_PADDINGS));

    return command;
}

QString WriteProtocolETCPage::writeHeatingPadONOFF(bool isOn)
{
    int flag = 0;

    if(isOn)
        flag = 1;

    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_ON_OFF));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(flag));

    return command;
}

QString WriteProtocolETCPage::writeTemperatureValue(uint volt)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_HEATING_PAD_TEMPERATURE_READ));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(volt));

    return command;
}

QString WriteProtocolETCPage::writeAdcRawDataReverseTemperature(ushort volt, ushort res)
{
    QString command;
    command.append(CUtil::hexaToHexaString((char)MD_MODE_QC_MODE));
    command.append(CUtil::hexaToHexaString((char)CMD_PROTOCOL_QC_MODE_ADC_RAW_DATA_REVERSE_TEMPERATURE));
    command.append(CUtil::hexaToHexaString(FIXED_SIZE));
    command.append(CUtil::hexaToHexaString(volt));
    command.append(CUtil::hexaToHexaString(res));

    return command;
}
