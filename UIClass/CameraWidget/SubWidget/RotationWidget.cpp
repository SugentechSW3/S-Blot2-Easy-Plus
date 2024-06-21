#include "RotationWidget.h"
#include "ui_RotationWidget.h"

RotationWidget::RotationWidget(QWidget *parent) :
                                                  QWidget(parent),
                                                  ui(new Ui_RotationWidget)
{
    ui->setupUi(this);
    this->initSignalSlots();
    this->readConfigData();
}

RotationWidget::~RotationWidget()
{
}

void RotationWidget::initSignalSlots()
{
    connect(ui->btnWrite, &QPushButton::clicked, this, [this] (bool)
            {
                this->writeConfigData();
            });
}

void RotationWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getRotation();
    ui->doubleSpinRotation->setValue(configData);
}

void RotationWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    double rotation = 0;
    rotation = ui->doubleSpinRotation->value();
    instance->setRotation(rotation);
}
