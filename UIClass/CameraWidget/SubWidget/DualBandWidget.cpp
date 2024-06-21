#include "DualBandWidget.h"
#include "ui_DualBandWidget.h"

struct DualBandWidget::privateStruct
{
    ConfigInformation* mConfigInstance = nullptr;
};

DualBandWidget::DualBandWidget(QWidget *parent) :
                                                  QWidget(parent),
                                                  ui(new Ui_DualBandWidget),
                                                  d(new privateStruct)
{
    ui->setupUi(this);
    d->mConfigInstance = this->getConfigInstance();

    this->readConfigData();
}

DualBandWidget::~DualBandWidget()
{
}

void DualBandWidget::readConfigData()
{
    auto config = d->mConfigInstance;
    auto dualBandData = config->getMultiBandSetting();
    auto analysis = config->getBoxCarAnalysis();

    this->setBandBlock2XStart(dualBandData.bandBlock2XStart);
    this->setBandBlock3XStart(dualBandData.bandBlock3XStart);
    this->setBandBlock4XStart(dualBandData.bandBlock4XStart);

    this->setMultiPCThres(dualBandData.mutiplePcThres);

    this->setPCXEndPos(dualBandData.pcXGap);
    this->setBandBlock2XEndPos(dualBandData.bandBlock2XGap);
    this->setBandBlock3XEndPos(dualBandData.bandBlock3XGap);
    this->setBandBlock4XEndPos(dualBandData.bandBlock4XGap);

    this->setWidth(analysis.width);
    this->setHeight(analysis.height);
    this->setBoxCarBandCount(analysis.boxCarbandCount);
    this->setXRange(analysis.xRange);
    this->setXStart(analysis.xStart);
    this->setPCThres(analysis.pcThres);
    this->setHousingThres(analysis.housingThres);
}

void DualBandWidget::writeConfigData()
{
    GlobalDataStruct::USE_MULTIPLE_BAND data;
    GlobalDataStruct::USE_BOXCAR_ANALYSIS configAnalaysis;
    auto config = d->mConfigInstance;

    data.bandBlock2XStart = this->getBandBlock2XStart();
    data.bandBlock3XStart = this->getBandBlock3XStart();
    data.bandBlock4XStart = this->getBandBlock4XStart();

    data.mutiplePcThres = this->getMultiPCThres();

    data.pcXGap = this->getPCXEndPos();
    data.bandBlock2XGap = this->getBandBlock2XEndPos();
    data.bandBlock3XGap = this->getBandBlock3XEndPos();
    data.bandBlock4XGap = this->getBandBlock4XEndPos();

    configAnalaysis.width = this->getWidth();
    configAnalaysis.height = this->getHeight();
    configAnalaysis.boxCarbandCount = this->getBoxCarBandCount();
    configAnalaysis.xRange = this->getXRange();
    configAnalaysis.xStart = this->getXStart();
    configAnalaysis.pcThres = this->getPCThres();
    configAnalaysis.housingThres = this->getHousingThres();

    config->setMultiBandSetting(data);
    config->setBoxCarAnalysis(configAnalaysis);
}

int DualBandWidget::getBandBlock2XStart()
{
    return ui->spin2xLine->value();
}

void DualBandWidget::setBandBlock2XStart(int value)
{
    ui->spin2xLine->setValue(value);
}

int DualBandWidget::getBandBlock3XStart()
{
    return ui->spin3xLine->value();
}

void DualBandWidget::setBandBlock3XStart(int value)
{
    ui->spin3xLine->setValue(value);
}

int DualBandWidget::getBandBlock4XStart()
{
    return ui->spin4xLine->value();
}

void DualBandWidget::setBandBlock4XStart(int value)
{
    ui->spin4xLine->setValue(value);
}

int DualBandWidget::getMultiPCThres()
{
    return ui->spinMultiPCThres->value();
}

void DualBandWidget::setMultiPCThres(int value)
{
    ui->spinMultiPCThres->setValue(value);
}

int DualBandWidget::getPCXEndPos()
{
    return ui->spinPCXEndPos->value();
}

void DualBandWidget::setPCXEndPos(int value)
{
    ui->spinPCXEndPos->setValue(value);
}

int DualBandWidget::getBandBlock2XEndPos()
{
    return ui->spin2xGap->value();
}

void DualBandWidget::setBandBlock2XEndPos(int value)
{
    ui->spin2xGap->setValue(value);
}

int DualBandWidget::getBandBlock3XEndPos()
{
    return ui->spin3xGap->value();
}

void DualBandWidget::setBandBlock3XEndPos(int value)
{
    ui->spin3xGap->setValue(value);
}

int DualBandWidget::getBandBlock4XEndPos()
{
    return ui->spin4xGap->value();
}

void DualBandWidget::setBandBlock4XEndPos(int value)
{
    ui->spin4xGap->setValue(value);
}

int DualBandWidget::getWidth()
{
    return ui->spinWidth->value();
}

void DualBandWidget::setWidth(int value)
{
    ui->spinWidth->setValue(value);
}

int DualBandWidget::getHeight()
{
    return ui->spinHeight->value();
}

void DualBandWidget::setHeight(int value)
{
    ui->spinHeight->setValue(value);
}

int DualBandWidget::getBoxCarBandCount()
{
    return ui->spinBandCount->value();
}

void DualBandWidget::setBoxCarBandCount(int value)
{
    ui->spinBandCount->setValue(value);
}

int DualBandWidget::getXRange()
{
    return ui->pcLineWidget->getXRange();
}

void DualBandWidget::setXRange(int value)
{
    ui->pcLineWidget->setXRange(value);
}

int DualBandWidget::getXStart()
{
    return ui->pcLineWidget->getXStart();
}

void DualBandWidget::setXStart(int value)
{
    ui->pcLineWidget->setXStart(value);
}

int DualBandWidget::getPCThres()
{
    return ui->pcLineWidget->getPCThres();
}

void DualBandWidget::setPCThres(int value)
{
    ui->pcLineWidget->setPCThres(value);
}

int DualBandWidget::getHousingThres()
{
    return ui->pcLineWidget->getHousingThres();
}

void DualBandWidget::setHousingThres(int value)
{
    ui->pcLineWidget->setHousingThres(value);
}

