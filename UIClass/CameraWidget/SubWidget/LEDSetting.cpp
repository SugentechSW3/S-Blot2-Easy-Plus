#include "LEDSetting.h"
#include "ui_LEDSetting.h"

#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"

#include "Network/PacketReader/PacketReader.h"
#include "Common/CLogWriter.h"

USING_SUGENTECH

LEDSetting::LEDSetting(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_LEDSetting)
{
    ui->setupUi(this);
    this->init();
}

LEDSetting::~LEDSetting()
{
}

void LEDSetting::initVariables()
{
}

void LEDSetting::initSignalSlots()
{
    connect(ui->btnRead, SIGNAL(clicked()), SLOT(onClickedLEDRead()));
    connect(ui->btnWrite, SIGNAL(clicked()), SLOT(onClickedLEDWrite()));
    connect(ui->btnON, SIGNAL(clicked()), SLOT(onClickedLEDON()));
    connect(ui->btnOFF, SIGNAL(clicked()), SLOT(onClickedLEDOFF()));

    connect(this, SIGNAL(onReadReady(QByteArray)), SLOT(onRecvData(QByteArray)));
}

void LEDSetting::startWidget()
{
    this->onClickedLEDRead();
}

void LEDSetting::stopWidget()
{

}

void LEDSetting::onClickedLEDRead()
{
    TunnningDataProtocolSendHelper::writeLEDRead();
}

void LEDSetting::onClickedLEDWrite()
{
    uchar ch1 = ui->spinLED1->value();
    uchar ch2 = ui->spinLED2->value();
    uchar ch3 = ui->spinLED3->value();
    uchar ch4 = ui->spinLED4->value();


    TunnningDataProtocolSendHelper::writeLEDWrite(ch1, ch2, ch3, ch4);

    this->onClickedLEDOFF();
    QTimer::singleShot(500, this, [this]
                       {
                           this->onClickedLEDON();
                       });

}

void LEDSetting::onClickedLEDON()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_ON);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void LEDSetting::onClickedLEDOFF()
{
    QString protocolStr = WriteProtocolCameraLedSetting::writeLEDOnOff(WriteProtocolCameraLedSetting::PROTOCOL_LED::PROTOCOL_LED_OFF);
    QByteArray sendData = protocolStr.toUtf8();
    this->onQueuedSend(sendData);
}

void LEDSetting::onRecvData(QByteArray recvData)
{
    if(PacketReader::modeReader(recvData) != MD_MODE::MD_MODE_GENERAL_SETTING)
        return;

    switch(PacketReader::commandReader(recvData))
    {
        case CMD_GENERAL_SETTING_MODE::CMD_GENERAL_SETTING_MODE_LED_LIGHT_POWER_READ:
        {
        auto value = PacketReader::dataReader(recvData);
        auto led1 = value.left(2).toUInt(nullptr, 16);
        auto led2 = value.mid(2,2).toUInt(nullptr, 16);
        auto led3 = value.mid(4,2).toUInt(nullptr, 16);
        auto led4 = value.right(2).toUInt(nullptr, 16);

        ui->spinLED1->setValue(led1);
        ui->spinLED2->setValue(led2);
        ui->spinLED3->setValue(led3);
        ui->spinLED4->setValue(led4);

        CLogWriter::printLog(QString("[LEDSetting::] : CMD_CAMERA_LED_SETTING_LED_PARAM_READ_RESP"));
        }break;
    }
}
