#include <QVariant>
#include "ParticleRemovalSettingWidget.h"
#include "ui_ParticleRemovalSettingWidget.h"

#include "Analysis/GlobalDataStruct.h"

struct ParticleRemovalSettingWidget::privateStruct
{
    ConfigInformation* mConfig = nullptr;
};

ParticleRemovalSettingWidget::ParticleRemovalSettingWidget(QWidget *parent) :
                                                                              QWidget(parent),
                                                                              ui(new Ui_ParticleRemovalSettingWidget),
                                                                              d(new privateStruct)
{
    ui->setupUi(this);
    this->initVariables();
    this->initSignalSlots();
    this->readConfigData();
}

ParticleRemovalSettingWidget::~ParticleRemovalSettingWidget()
{
}

void ParticleRemovalSettingWidget::initVariables()
{
    d->mConfig = this->getConfigInstance();
}

void ParticleRemovalSettingWidget::initSignalSlots()
{
    connect(d->mConfig, &ConfigInformation::onChangedPaticleRemoval, this, &ParticleRemovalSettingWidget::onChangedConfig);
}

int ParticleRemovalSettingWidget::getRemovalPaticleCount()
{
    return ui->spinRemovalPaticle->value();
}

void ParticleRemovalSettingWidget::setRemovalPaticleCount(int count)
{
    ui->spinRemovalPaticle->setValue(count);
}

int ParticleRemovalSettingWidget::getAveragePaticleCount()
{
    return ui->spinAveragePaticle->value();
}

void ParticleRemovalSettingWidget::setAveragePaticleCount(int count)
{
    ui->spinAveragePaticle->setValue(count);
}

int ParticleRemovalSettingWidget::getThreshold()
{
    return ui->spinThreshold->value();
}

void ParticleRemovalSettingWidget::setThreshold(int threshold)
{
    ui->spinThreshold->setValue(threshold);
}

void ParticleRemovalSettingWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getPaticleRemoval();

    this->setAveragePaticleCount(configData.averagePaticle);
    this->setRemovalPaticleCount(configData.removalPaticle);
    this->setThreshold(configData.threshold);
}

void ParticleRemovalSettingWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    GlobalDataStruct::PATICLE_REMOVAL configData;
    configData.averagePaticle = this->getAveragePaticleCount();
    configData.removalPaticle = this->getRemovalPaticleCount();
    configData.threshold = this->getThreshold();

    instance->setPaticleRemoval(configData);
}

void ParticleRemovalSettingWidget::onChangedConfig(GlobalDataStruct::PATICLE_REMOVAL)
{
    this->readConfigData();
}
