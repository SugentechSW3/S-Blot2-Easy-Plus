#include <QDateTime>
#include <QTimer>

#include "Common/Network/CTcpClient.h"

#include "UIClass/DialogWidget/InterfaceServerIPSettingDlg.h"

#include "HL7InterFaceAdapter.h"
#include "HL7InterFaceReader.h"
#include "HL7InterFaceWriter.h"
#include "Common/CLogWriter.h"

NS_SUGENTECH_BEGIN

struct HL7InterFaceAdapter::privateStruct
{
    CTcpClient mClientSocket;
    QTimer mTimeOut;
};

HL7InterFaceAdapter::HL7InterFaceAdapter(QObject *parent) :
                                                            QObject(parent),
                                                            d(new privateStruct)
{
    this->init();
    d->mTimeOut.setSingleShot(true);
}

HL7InterFaceAdapter::~HL7InterFaceAdapter()
{

}

void HL7InterFaceAdapter::init()
{
    this->initSignalSlot();
    this->setupIPAddressAndPort();
}

void HL7InterFaceAdapter::initSignalSlot()
{
    connect(&d->mClientSocket, &CTcpClient::onRecvData, this, &HL7InterFaceAdapter::onReadReady);
    connect(&d->mTimeOut, &QTimer::timeout, this, &HL7InterFaceAdapter::onTimeOut);
}

void HL7InterFaceAdapter::downloadLISData(const QDateTime& dateTime, const QMap<int, QString>& patientIDList)
{
    QByteArray readAllProtocol;
    HL7_MSH_DATA mshData;

    mshData.dateTime = dateTime.toString("yyyyMMddhhmmss");
    mshData.deviceID = HL7_DEVICE_ID;
    mshData.msgType = HL7_MSH_MESSAGE_TYPE_QRY;

    readAllProtocol.append(HL7InterFaceWriter::createMSH(mshData));
    readAllProtocol.append(HL7InterFaceWriter::createReadAllQRDForBarcode(patientIDList));

    if(this->sendData(readAllProtocol) == false)
        emit onTimeOut();
}

QString HL7InterFaceAdapter::createTestData()
{
    QByteArray testData;
    HL7_MSH_DATA mshData;
    mshData.msgType = HL7_MSH_MESSAGE_TYPE_QRY;
    mshData.dateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    mshData.deviceID = HL7_DEVICE_ID;

    testData.append(HL7InterFaceWriter::createMSH(mshData));

    this->addStxEtxCr(testData);

    return testData;
}

bool HL7InterFaceAdapter::open()
{
    if (d->mClientSocket.connectToServer() == false)
        return false;

    return true;
}

bool HL7InterFaceAdapter::sendData(const QByteArray& data)
{
    if(this->open() == false)
        return false;

    auto sendData = data;
    this->addStxEtxCr(sendData);

   auto isSuccess = d->mClientSocket.sendData(sendData);

   if(isSuccess)
       d->mTimeOut.start(10000);

   CLogWriter::printLog(QString(">>> (HL7InterFaceAdapter::)Send : %1").arg(QString(sendData)));
   return isSuccess;
}

void HL7InterFaceAdapter::setupIPAddressAndPort()
{
    auto ipAddress = InterfaceServerIPSettingDlg::getIPAddress();
    auto port = InterfaceServerIPSettingDlg::getPort();

    d->mClientSocket.setIpAddressAndPort(ipAddress, port.toInt());
}

bool HL7InterFaceAdapter::isValidData(const QByteArray& data)
{
    QString dataStr(data);

    if(dataStr.count() < 3)
        return false;

    if(dataStr.front() != HL7_START_BLOCK)
        return false;

    if(dataStr.back() != HL7_CARRIAGE_RETURN)
        return false;

    if(dataStr[dataStr.count()-2] != HL7_END_BLOCK)
        return false;

    return true;
}

void HL7InterFaceAdapter::parsingData(const QByteArray& data)
{
    auto parsingData = data;
    auto headerData = data.split(HL7_CARRIAGE_RETURN);

    HL7_ORDER_DATA orderData;

    for(auto& itr :headerData)
        this->parsingHeader(itr, orderData);

    emit onRespFinished(orderData);
}

void HL7InterFaceAdapter::parsingHeader(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{
    //read header data;
    auto header = HL7InterFaceReader::readHL7HeaderType(data);

    switch(header)
    {
    case HL7_HEADER_TYPE_MSH:
        this->parsingMSH(data, parsingResult);
        break;

    case HL7_HEADER_TYPE_QRD:
        this->parsingQRD(data, parsingResult);
        break;

    case HL7_HEADER_TYPE_PID:
        this->parsingPID(data, parsingResult);
        break;

    case HL7_HEADER_TYPE_OBR:
        this->parsingOBR(data, parsingResult);
        break;

    case HL7_HEADER_TYPE_OBX:
        this->parsingOBX(data, parsingResult);
        break;

     default:
        return;
    }
}

void HL7InterFaceAdapter::addStxEtxCr(QByteArray& data)
{
    data.push_front(HL7_START_BLOCK);
    data.push_back(HL7_END_BLOCK);
    data.push_back(HL7_CARRIAGE_RETURN);
}

void HL7InterFaceAdapter::removeStxEtxCr(QByteArray& data)
{
    data.remove(0, 1); //remove stx
    data.remove(data.count()-1, 1); //remove cr
    data.remove(data.count()-1, 1); //remove etx
}

void HL7InterFaceAdapter::parsingMSH(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{
    auto mshData = HL7InterFaceReader::parsingMSHData(data);

    parsingResult.deviceID = mshData.deviceID;
    parsingResult.dateTime = mshData.dateTime;
}

void HL7InterFaceAdapter::parsingQRD(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{

}

void HL7InterFaceAdapter::parsingPID(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{
    auto pidData = HL7InterFaceReader::parsingPIDData(data);
    auto currentData = parsingResult.data[pidData.index.toInt()];

    currentData.sex = pidData.sex;
    currentData.name = pidData.name;
    currentData.birthDate = pidData.birthDate;
    currentData.patientID = pidData.patientID;
    currentData.phoneNumber = pidData.phoneNumber;

    parsingResult.data[pidData.index.toInt()] = currentData;
}

void HL7InterFaceAdapter::parsingOBR(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{
    auto obrData = HL7InterFaceReader::parsingOBRData(data);
    auto currentData = parsingResult.data[obrData.index.toInt()];

    currentData.panelInitial = obrData.panelInitial;
    currentData.dateTime = obrData.dateTime;
    currentData.patientID = obrData.patientID;

    parsingResult.data[obrData.index.toInt()] = currentData;
}

void HL7InterFaceAdapter::parsingOBX(const QByteArray& data, HL7_ORDER_DATA& parsingResult)
{

}

void HL7InterFaceAdapter::onReadReady(QByteArray recvData)
{
    d->mTimeOut.stop();

    if(recvData.isEmpty())
        return;

    if(this->isValidData(recvData) == false)
        return;

    this->removeStxEtxCr(recvData);
    this->parsingData(recvData);
    CLogWriter::printLog(QString(">>> (HL7InterFaceAdapter::)Rev : %1").arg(QString(recvData)));
}

NS_SUGENTECH_END
