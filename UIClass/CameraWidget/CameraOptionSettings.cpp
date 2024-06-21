#include "CameraOptionSettings.h"
#include "ui_CameraOptionSettings.h"


CameraOptionSettings::CameraOptionSettings(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_CameraOptionSettings)
{
    ui->setupUi(this);
    this->init();
}

CameraOptionSettings::~CameraOptionSettings()
{
}

void CameraOptionSettings::initVariables()
{
}

void CameraOptionSettings::initSignalSlots()
{

}

void CameraOptionSettings::startWidget()
{

    ui->widgetCameraLightSetting->start();
    ui->widgetLEDSetting->start();
    ui->widgetPositionSetting->start();
    ui->widgetQRPositionSetting->start();
}

void CameraOptionSettings::stopWidget()
{
    ui->widgetLEDSetting->stop();
    ui->widgetPositionSetting->stop();
    ui->widgetQRPositionSetting->stop();
}

