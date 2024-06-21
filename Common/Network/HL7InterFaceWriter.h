#ifndef HL7INTERFACEWRITER_H
#define HL7INTERFACEWRITER_H

#include <QObject>
#include "HL7InterFaceHeader.h"

class HL7InterFaceWriter : public QObject
{
private:
    Q_OBJECT

public:

    explicit HL7InterFaceWriter(QObject *parent = nullptr);
    ~HL7InterFaceWriter();

    static QString createMSH(const HL7_MSH_DATA &mshData);
    static QString createQRD(const HL7_QRD_DATA &qrdData);
    static QString createReadAllQRDForBarcode(const QMap<int, QString> &patientIDList);
    static QString createReadAllQRD();
    static QString createPID(const HL7_PID_DATA& pidData);
    static QString createOBR(const HL7_OBR_DATA& obrData);
    static QString createOBX(const HL7_OBX_DATA& obxData);

private:
    static QString mshMessageTypeToString(const HL7_MSH_MESSAGE_TYPE& type);
};

#endif // HL7INTERFACEWRITER_H
