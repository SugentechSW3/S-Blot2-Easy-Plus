#include "IntensityWidget.h"
#include "ui_IntensityWidget.h"

IntensityWidget::IntensityWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_IntensityWidget)
{
    ui->setupUi(this);
    this->readConfigData();
}

IntensityWidget::~IntensityWidget()
{
}

int IntensityWidget::getParticle()
{
    return ui->spinParticle->value();
}

void IntensityWidget::setParticle(int value)
{
    ui->spinParticle->setValue(value);
}

int IntensityWidget::getBand()
{
    return ui->spinParticle->value();
}

void IntensityWidget::setBand(int value)
{
    ui->spinParticle->setValue(value);
}

int IntensityWidget::getBackGround()
{
    return ui->spinBackGround->value();
}

void IntensityWidget::setBackGround(int value)
{
    ui->spinBackGround->setValue(value);
}

int IntensityWidget::getWhite()
{
    return ui->spinWhite->value();
}

void IntensityWidget::setWhite(int value)
{
    ui->spinWhite->setValue(value);
}

void IntensityWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getIntensity();

    ui->spinParticle->setValue(configData.paticle);
    ui->spinBand->setValue(configData.band);
    ui->spinBackGround->setValue(configData.background);
    ui->spinWhite->setValue(configData.white);
}

void IntensityWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    GlobalDataStruct::INTENSITY configData;
    configData.paticle = ui->spinParticle->value();
    configData.band = ui->spinBand->value();
    configData.background = ui->spinBackGround->value();
    configData.white = ui->spinWhite->value();

    instance->setIntensity(configData);
}
