#include <QVariant>
#include "ParticleRemovalSettingWidget.h"
#include "ui_ParticleRemovalSettingWidget.h"

#include "Analysis/GlobalDataStruct.h"


ParticleRemovalSettingWidget::ParticleRemovalSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_ParticleRemovalSettingWidget)
{
    ui->setupUi(this);
    this->readConfigData();
}

ParticleRemovalSettingWidget::~ParticleRemovalSettingWidget()
{
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
