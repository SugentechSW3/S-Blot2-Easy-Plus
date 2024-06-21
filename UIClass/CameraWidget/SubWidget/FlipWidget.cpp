#include "FlipWidget.h"
#include "ui_FlipWidget.h"

FlipWidget::FlipWidget(QWidget *parent) :
                                          QWidget(parent),
                                          ui(new Ui_FlipWidget)
{
    ui->setupUi(this);
    this->initSignalSlots();
    this->readConfigData();
}

FlipWidget::~FlipWidget()
{

}


void FlipWidget::initSignalSlots()
{
    connect(ui->btnWrite, &QPushButton::clicked, this, [this] (bool)
            {
                this->writeConfigData();
            });
}


void FlipWidget::readConfigData()
{
    auto instance = this->getConfigInstance();
    auto configData = instance->getFlip();
    ui->comboFlip->setCurrentIndex(configData);
}

void FlipWidget::writeConfigData()
{
    auto instance = this->getConfigInstance();

    double flip = 0;
    flip = ui->comboFlip->currentIndex();
    instance->setFlip(flip);
}
