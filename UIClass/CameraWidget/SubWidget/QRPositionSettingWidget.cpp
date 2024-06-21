#include "QRPositionSettingWidget.h"
#include "ui_QRPositionSettingWidget.h"

#include "Network/PacketReader/PacketReader.h"
#include "Network/PacketWriter/WriteProtocolBarcodePage.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"

#include "Analysis/ConfigInformation.h"
#include "Common/CLogWriter.h"

USING_SUGENTECH


QRPositionSettingWidget::QRPositionSettingWidget(QWidget *parent) :
                                                                    CSerialNetworkWidget(parent),
                                                                    ui(new Ui_QRPositionSettingWidget)
{
    ui->setupUi(this);
    this->init();
}

QRPositionSettingWidget::~QRPositionSettingWidget()
{

}

void QRPositionSettingWidget::initVariables()
{

}

void QRPositionSettingWidget::initSignalSlots()
{
    connect(ui->btnQRTrayPositionRead, &QPushButton::clicked, this, &QRPositionSettingWidget::onClickedQRTrayPosRead);
    connect(ui->btnQRTrayPositionWrite, &QPushButton::clicked, this, &QRPositionSettingWidget::onClickedQRTrayPosSet);

    connect(this, &QRPositionSettingWidget::onReadReady, this, &QRPositionSettingWidget::onRecvData);
}

void QRPositionSettingWidget::startWidget()
{
    this->onClickedQRTrayPosRead();
}

void QRPositionSettingWidget::stopWidget()
{

}

void QRPositionSettingWidget::onClickedQRTrayPosSet()
{
    auto value = ui->spinQRTrayPosition->value();
    TunnningDataProtocolSendHelper::writeQRTrayPosWrite(value);
}

void QRPositionSettingWidget::onClickedQRTrayPosRead()
{
    auto QrTrayPosVaue = ConfigInformation::getInstance()->getQRTriplePosition();
    TunnningDataProtocolSendHelper::writeQRTrayPosRead();
    ui->spinQRTrayPosition->setValue(QrTrayPosVaue);
}

void QRPositionSettingWidget::onRecvData(QByteArray recvData)
{
    if(PacketReader::modeReader(recvData) != MD_MODE::MD_MODE_QR_SETTING_MODE)
        return;

    switch(PacketReader::commandReader(recvData))
    {

    case CMD_QR_SETTING_MODE::CMD_SETTING_MODE_QR_TRAY_POS_READ:
    {
        auto data = PacketReader::dataReader(recvData);
        int value = data.toUInt(nullptr, 16);
        ui->spinQRTrayPosition->setValue(value);
    }break;

    }
}

