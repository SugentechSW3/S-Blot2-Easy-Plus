#include <QDebug>
#include "SpinSliderWidget.h"
#include "ui_SpinSliderWidget.h"

SpinSliderWidget::SpinSliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_SpinSliderWidget)
{
    ui->setupUi(this);
    this->initSignalSlots();
}

SpinSliderWidget::~SpinSliderWidget()
{
}

void SpinSliderWidget::initSignalSlots()
{
    connect(ui->slider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->slider, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
}

void SpinSliderWidget::setMinimumValue(int value)
{
    ui->slider->setMinimum(value);
    ui->spinBox->setMinimum(value);
}

void SpinSliderWidget::setMaximumValue(int value)
{
    ui->slider->setMaximum(value);
    ui->spinBox->setMaximum(value);
}

void SpinSliderWidget::setStep(int value)
{
    ui->slider->setSingleStep(value);
    ui->spinBox->setSingleStep(value);
}

void SpinSliderWidget::setValue(int value)
{
    if(value == 0)
    {
        if(ui->spinBox->maximum() == 0 && ui->spinBox->minimum() == 0)
        {
            this->setDisabled(true);
            return;
        }
    }

    ui->slider->setValue(value);
    ui->spinBox->setValue(value);
}

int SpinSliderWidget::getValue()
{
    return ui->spinBox->value();
}
