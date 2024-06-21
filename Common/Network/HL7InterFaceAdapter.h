#ifndef HL7INTERFACEADAPTER_H
#define HL7INTERFACEADAPTER_H

#include <QObject>
#include "HL7InterFaceHeader.h"
#include "Common/CUtilCommon.h"

NS_SUGENTECH_BEGIN

class HL7InterFaceAdapter : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit HL7InterFaceAdapter(QObject *parent = nullptr);
    ~HL7InterFaceAdapter();

    void downloadLISData(const QDateTime &dateTime, const QMap<int, QString> &patientIDList);
    bool sendData(const QByteArray &data);
    bool open();
    QString createTestData();

signals:
    void onRespFinished(HL7_ORDER_DATA data);
    void onTimeOut();

private slots:
    void onReadReady(QByteArray recvData);

private:
    void init();
    void initSignalSlot();
    void setupIPAddressAndPort();
    void parsingData(const QByteArray& data);
    void parsingHeader(const QByteArray& data, HL7_ORDER_DATA &parsingResult);

    void addStxEtxCr(QByteArray& data);
    void removeStxEtxCr(QByteArray& data);

    void parsingMSH(const QByteArray& data, HL7_ORDER_DATA& parsingResult);
    void parsingQRD(const QByteArray& data, HL7_ORDER_DATA& parsingResult);
    void parsingPID(const QByteArray& data, HL7_ORDER_DATA& parsingResult);
    void parsingOBR(const QByteArray& data, HL7_ORDER_DATA& parsingResult);
    void parsingOBX(const QByteArray& data, HL7_ORDER_DATA& parsingResult);

    bool isValidData(const QByteArray& data);

private:
    QScopedPointer<privateStruct> d;
};

NS_SUGENTECH_END
#endif // HL7INTERFACEADAPTER_H
