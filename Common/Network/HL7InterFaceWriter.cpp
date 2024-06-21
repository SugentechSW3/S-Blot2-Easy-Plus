#include "HL7InterFaceWriter.h"

HL7InterFaceWriter::HL7InterFaceWriter(QObject *parent) : QObject(parent)
{

}

HL7InterFaceWriter::~HL7InterFaceWriter()
{

}


QString HL7InterFaceWriter::createMSH(const HL7_MSH_DATA& mshData)
{
    QString mshProtocol;
    QString deviceName;
    QString manufacture;
  //const QString DEVICE_NAME = "sblot2";
  //const QString MANUFACTURE = "sugentech";

    mshProtocol.append(HL7_MSH);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_ENCODING_CHARACTERS);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(deviceName);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(manufacture);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_RECEIVING_APPLICATION);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(mshData.dateTime);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7InterFaceWriter::mshMessageTypeToString(mshData.msgType));
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(mshData.deviceID);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_PROCESSING_ID);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_VERSION);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_ACCEPT_ACKNOWLEDGMENT_AL);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_APP_ACKNOWLEDGMENT_NE);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_SEPARATOR);
    mshProtocol.append(HL7_CARRIAGE_RETURN);

    return mshProtocol;
}

QString HL7InterFaceWriter::createQRD(const HL7_QRD_DATA& qrdData)
{
    QString qrdProtocol;
    qrdProtocol.append(HL7_QRD);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_FORMAT_CODE);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_PRIORITY);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(qrdData.index);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append("1");
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(qrdData.patientID);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_CARRIAGE_RETURN);

    return qrdProtocol;
}

QString HL7InterFaceWriter::createReadAllQRDForBarcode(const QMap<int, QString>& patientIDList)
{
    QString qrdProtocol;
    QMapIterator<int, QString> itr(patientIDList);

    while(itr.hasNext())
    {
        itr.next();

        HL7_QRD_DATA data;

        data.index = QString::number(itr.key());
        data.patientID = itr.value();

       auto protocol = HL7InterFaceWriter::createQRD(data);
       qrdProtocol.push_back(protocol);
    }


    return qrdProtocol;
}

QString HL7InterFaceWriter::createReadAllQRD()
{
    QString qrdProtocol;
    qrdProtocol.append(HL7_QRD);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_FORMAT_CODE);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_PRIORITY);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append("0");
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append("1");
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_SEPARATOR);
    qrdProtocol.append(HL7_CARRIAGE_RETURN);

    return qrdProtocol;
}

QString HL7InterFaceWriter::createPID(const HL7_PID_DATA& pidData)
{
    QString pidProtocol;
    pidProtocol.append(HL7_PID);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(pidData.index);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(pidData.patientID);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(pidData.name);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(pidData.sex);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_SEPARATOR);
    pidProtocol.append(HL7_CARRIAGE_RETURN);

    return pidProtocol;
}

QString HL7InterFaceWriter::createOBR(const HL7_OBR_DATA& obrData)
{
    QString obrProtocol;
    obrProtocol.append(HL7_OBR);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(obrData.index);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(obrData.patientID);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(obrData.panelInitial);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(obrData.dateTime);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(obrData.resultImagePath);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(HL7_SEPARATOR);
    obrProtocol.append(HL7_CARRIAGE_RETURN);

    return obrProtocol;
}

//OBX||ST|patient_id|1|1_tIgE_>100_P^ 2_d1_50.0_5 ….^62_-_-_-_-|||||
QString HL7InterFaceWriter::createOBX(const HL7_OBX_DATA& obxData)
{
    QString obxProtocol;
    obxProtocol.append(HL7_OBX);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_VALUE_TYPE);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(obxData.patientID);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append("1");
    obxProtocol.append(HL7_SEPARATOR);

    for(auto i = 0; i < obxData.dataResult.count(); i++)
    {
        auto itr = obxData.dataResult[i];
        auto index = i+1;

        QString observationValue;

        if(i != 0)
            observationValue.append(HL7_OBSERVATION_SEPARATOR);

        observationValue.append(QString::number(index));
        observationValue.append(HL7_OBSERVATION_CHAIN);
        observationValue.append(itr.bandCode);
        observationValue.append(HL7_OBSERVATION_CHAIN);
        observationValue.append(itr.iumlResult);
        observationValue.append(HL7_OBSERVATION_CHAIN);
        observationValue.append(itr.iumlClass);

        obxProtocol.push_back(observationValue);
    }

    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_SEPARATOR);
    obxProtocol.append(HL7_CARRIAGE_RETURN);

    return obxProtocol;
}

QString HL7InterFaceWriter::mshMessageTypeToString(const HL7_MSH_MESSAGE_TYPE& type)
{
    QString messageType;

    switch(type)
    {
    case HL7_MSH_MESSAGE_TYPE_QRY :
        messageType = "QRY^A19";
        break;

    case HL7_MSH_MESSAGE_TYPE_ADR:
        messageType = "ADR^A19";
        break;

    case HL7_MSH_MESSAGE_TYPE_ORU:
        messageType = "ORU^R31";
        break;

    default:
        break;
    }

    return messageType;
}
