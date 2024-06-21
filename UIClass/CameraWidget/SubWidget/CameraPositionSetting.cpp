#include "CameraPositionSetting.h"
#include "ui_CameraPositionSetting.h"

#include "Network/PacketWriter/WriteProtocolCameraLedSetting.h"
#include "Network/PacketWriter/WriteProtocolDispAspPage.h"
#include "Network/PacketWriter/TunnningDataProtocolSendHelper.h"
#include "Network/PacketReader/PacketReader.h"

CameraPositionSetting::CameraPositionSetting(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_CameraPositionSetting)
{
    ui->setupUi(this);
    this->init();
}

CameraPositionSetting::~CameraPositionSetting()
{
}

void CameraPositionSetting::initVariables()
{
}

void CameraPositionSetting::initSignalSlots()
{
    connect(ui->btnXPosRead, SIGNAL(clicked()), SLOT(onClickedXPosRead()));
    connect(ui->btnXPosWrite, SIGNAL(clicked()), SLOT(onClickedXPosWrite()));

    connect(ui->btnCameraWidthPosRead, &QPushButton::clicked, this, &CameraPositionSetting::onClickedCameraWidthPosRead);
    connect(ui->btnCameraWidthPosWrite, &QPushButton::clicked, this, &CameraPositionSetting::onClickedCameraWidthPosWrite);

    connect(this, &CameraPositionSetting::onReadReady, this, &CameraPositionSetting::onRecvData);
}

void CameraPositionSetting::startWidget()
{
    this->onClickedXPosRead();
    this->onClickedCameraWidthPosRead();
}

void CameraPositionSetting::stopWidget()
{
}

void CameraPositionSetting::onClickedXPosRead()
{
    TunnningDataProtocolSendHelper::writeXPosRead();
}

void CameraPositionSetting::onClickedXPosWrite()
{
    auto value = ui->spinXPos->value();
    TunnningDataProtocolSendHelper::writeXPosWrite(value);
}

void CameraPositionSetting::onClickedCameraWidthPosRead()
{
    TunnningDataProtocolSendHelper::writeCameraWidthPosRead();
}

void CameraPositionSetting::onClickedCameraWidthPosWrite()
{
    auto value = ui->spinCameraWidthPos->value();
    TunnningDataProtocolSendHelper::writeCameraWidthPosWrite(value);
}

void CameraPositionSetting::onRecvData(QByteArray recvData)
{
    if(PacketReader::modeReader(recvData) != MD_MODE::MD_MODE_MOTOR_SETTING_MODE)
        return;

    switch(PacketReader::commandReader(recvData))
    {
    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_TRAY_CAM_X_START_READ:
    {
        auto value = PacketReader::dataReader(recvData);
        int xPos = value.toUInt(nullptr, 16);

        ui->spinXPos->setValue(xPos);

    }break;


    case CMD_MOTOR_SETTING_MODE::CMD_MOTOR_SETTING_MODE_STRIP_WIDTH_READ:
    {
        auto value = PacketReader::dataReader(recvData);
        int xPos = value.toUInt(nullptr, 16);

        ui->spinCameraWidthPos->setValue(xPos);
    }break;
    }
}
