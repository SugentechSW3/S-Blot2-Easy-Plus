#include "CameraSettings.h"
#include "ui_CameraSettings.h"

CameraSettings::CameraSettings(QWidget *parent) :
    CSerialNetworkWidget(parent),
    ui(new Ui_CameraSettings)
{
    ui->setupUi(this);
    this->init();
}

CameraSettings::~CameraSettings()
{
    this->stop();
}

void CameraSettings::initVariables()
{
}

void CameraSettings::initSignalSlots()
{
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onSelectedSaveImage, this, &CameraSettings::onSelectedSaveImage);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onSelectedLoadImage, this, &CameraSettings::onSelectedLoadImage);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onSelectedFindPC, this, &CameraSettings::onSelectedFindPC);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onSelectedAnalysis, this, &CameraSettings::onSelectedAnalysis);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onSelectedAngleReset, this, &CameraSettings::onSelectedAngleReset);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onReadQRTest, this, &CameraSettings::onReadQRTest);
    connect(ui->tabCameraStripSettings, &CameraStripSettings::onGraphHide, this, &CameraSettings::onGraphHide);

    connect(ui->tabCameraStripSettings, QOverload<int,int>::of(&CameraStripSettings::setBoxCarRect), this, QOverload<int,int>::of(&CameraSettings::setBoxCarRect));
    connect(ui->tabCameraStripSettings, QOverload<QVector<int>>::of(&CameraStripSettings::setBoxCarRect), this, QOverload<QVector<int>>::of(&CameraSettings::setBoxCarRect));
}

void CameraSettings::startWidget()
{
    ui->tabCameraStripSettings->start();
    ui->tabCameraOptionSettings->start();
}

void CameraSettings::stopWidget()
{
    ui->tabCameraStripSettings->stop();
    ui->tabCameraOptionSettings->stop();
}

void CameraSettings::setResultValue(const QVector<double>& intensity, const QVector<double>& background, const QVector<double>& band)
{
    ui->tabCameraStripSettings->setResultValue(intensity, background, band);
}
