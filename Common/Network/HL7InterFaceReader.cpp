#include "Common/CLogWriter.h"
#include "Analysis/ConfigInformation.h"

#include "HL7InterFaceReader.h"


USING_SUGENTECH
HL7InterFaceReader::HL7InterFaceReader(QObject *parent) :
                                                          QObject(parent)
{

}

HL7InterFaceReader::~HL7InterFaceReader()
{

}

HL7_MSH_DATA HL7InterFaceReader::parsingMSHData(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');
    HL7_MSH_DATA parsingData;

    if(splitedData.isEmpty())
        return parsingData;

    if(splitedData.count() <= HL7_MSH_STR_POSITION_RESERVED5_NULL)
        return parsingData;

    parsingData.msgType = HL7InterFaceReader::readMSHMessageType(data);
    parsingData.dateTime = splitedData[HL7_MSH_STR_POSITION_DATE_TIME];
    parsingData.deviceID = splitedData[HL7_MSH_STR_POSITION_DEVICE_ID];

    return parsingData;
}

HL7_PID_DATA HL7InterFaceReader::parsingPIDData(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');
    HL7_PID_DATA parsingData;

    if(splitedData.isEmpty())
        return parsingData;

    if(splitedData.count() <= HL7_PID_STR_POSITION_RESERVED8)
        return parsingData;

    parsingData.patientID = splitedData[HL7_PID_STR_POSITION_PATIENT_ID];
    parsingData.index = splitedData[HL7_PID_STR_POSITION_INDEX];
    parsingData.name = splitedData[HL7_PID_STR_POSITION_PATIENT_NAME];
    parsingData.sex = splitedData[HL7_PID_STR_POSITION_SEX];
    parsingData.birthDate = splitedData[HL7_PID_STR_POSITION_BIRTH_DATE];
    parsingData.phoneNumber = splitedData[HL7_PID_STR_POSITION_PHONE_NUMBER];

    return parsingData;
}

HL7_OBR_DATA HL7InterFaceReader::parsingOBRData(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');
    HL7_OBR_DATA parsingData;

    if(splitedData.isEmpty())
        return parsingData;

    if(splitedData.count() <= HL7_OBR_STR_POSITION_RESERVED3_NULL)
        return parsingData;

    parsingData.index = splitedData[HL7_OBR_STR_POSITION_INDEX];
    parsingData.patientID = splitedData[HL7_OBR_STR_POSITION_PATIENT_ID];
    parsingData.panelInitial = splitedData[HL7_OBR_STR_POSITION_PANEL_INITIAL];
    parsingData.dateTime = splitedData[HL7_OBR_STR_POSITION_OBSERVATION_DATE_TIME];

    return parsingData;
}

HL7_QRD_DATA HL7InterFaceReader::parsingQRDData(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');
    HL7_QRD_DATA parsingData;

    if(splitedData.isEmpty())
        return parsingData;

    if(splitedData.count() <= HL7_QRD_STR_POSITION_RESERVED_NULL)
        return parsingData;

    parsingData.index = splitedData[HL7_QRD_STR_POSITION_QUERYID];
    parsingData.patientID = splitedData[HL7_QRD_STR_POSITION_PATIENT_ID];

    return parsingData;
}

HL7_MSH_MESSAGE_TYPE HL7InterFaceReader::readMSHMessageType(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');

    if(splitedData.isEmpty())
        return HL7_MSH_MESSAGE_TYPE_NULL;

    if(splitedData.count() <= HL7_MSH_STR_POSITION_MESSAGE_TYPE)
        return HL7_MSH_MESSAGE_TYPE_NULL;

    auto messageType = splitedData[HL7_MSH_STR_POSITION_MESSAGE_TYPE];

    return HL7InterFaceReader::mshStringToMessageType(messageType);
}

HL7_HEADER_TYPE HL7InterFaceReader::readHL7HeaderType(const QByteArray& data)
{
    auto orgData = data;
    auto splitedData = orgData.split('|');

    if(splitedData.isEmpty())
        return HL7_HEADER_TYPE_NULL;

    auto header = splitedData.first();
    QString headerStr(header);

    if(headerStr == HL7_MSH)
        return HL7_HEADER_TYPE_MSH;

    else if(headerStr == HL7_QRD)
        return HL7_HEADER_TYPE_QRD;

    else if(headerStr == HL7_PID)
        return HL7_HEADER_TYPE_PID;

    else if(headerStr == HL7_OBR)
        return HL7_HEADER_TYPE_OBR;

    else if(headerStr == HL7_OBX)
        return HL7_HEADER_TYPE_OBX;

    return HL7_HEADER_TYPE_NULL;
}

QString HL7InterFaceReader::panelNameToPanelInitial(const QString& panelName)
{
    QString combinedString = ConfigInformation::convertQEnumToQString(ConfigInformation::ALLERGY_PANEL_LISTS::ALLERGY_PANEL,"ALLERGY_PANEL_LISTS");

    if(panelName == combinedString)
        return "P";

    CLogWriter::printLog("[Warnning] : This Panel Name Is Unknown");

    if(panelName.isEmpty())
        return QString("");

    return panelName.left(1);
}

QString HL7InterFaceReader::panelInitalToPanelName(const QString& panelInitial)
{
    QString combinedString = ConfigInformation::convertQEnumToQString(ConfigInformation::ALLERGY_PANEL_LISTS::ALLERGY_PANEL,"ALLERGY_PANEL_LISTS");

    if(panelInitial == "P")
        return combinedString;

    CLogWriter::printLog("[Warnning] : This PanelInital Name Is Unknown");

    return QString("");
}

HL7_MSH_MESSAGE_TYPE HL7InterFaceReader::mshStringToMessageType(const QString& mshString)
{
    if(mshString.contains(HL7_MSH_MESSAGE_TYPE_QRY_STRING))
        return HL7_MSH_MESSAGE_TYPE_QRY;

    else if(mshString.contains(HL7_MSH_MESSAGE_TYPE_ADR_STRING))
        return HL7_MSH_MESSAGE_TYPE_ADR;

    else if(mshString.contains(HL7_MSH_MESSAGE_TYPE_ORU_STRING))
        return HL7_MSH_MESSAGE_TYPE_ORU;

    return HL7_MSH_MESSAGE_TYPE_NULL;
}

