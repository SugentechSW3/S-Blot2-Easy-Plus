#ifndef HL7INTERFACEREADER_H
#define HL7INTERFACEREADER_H

#include <QObject>
#include "HL7InterFaceHeader.h"

class HL7InterFaceReader : public QObject
{
    Q_OBJECT
public:
    explicit HL7InterFaceReader(QObject *parent = nullptr);
    ~HL7InterFaceReader();

    static HL7_MSH_DATA parsingMSHData(const QByteArray& data);
    static HL7_PID_DATA parsingPIDData(const QByteArray& data);
    static HL7_OBR_DATA parsingOBRData(const QByteArray& data);
    static HL7_QRD_DATA parsingQRDData(const QByteArray& data);

    static HL7_MSH_MESSAGE_TYPE readMSHMessageType(const QByteArray& data);
    static HL7_HEADER_TYPE readHL7HeaderType(const QByteArray &data);
    static QString panelNameToPanelInitial(const QString& panelName);
    static QString panelInitalToPanelName(const QString& panelInitial);

private:
    static HL7_MSH_MESSAGE_TYPE mshStringToMessageType(const QString& mshString);


};

#endif // HL7INTERFACEREADER_H
