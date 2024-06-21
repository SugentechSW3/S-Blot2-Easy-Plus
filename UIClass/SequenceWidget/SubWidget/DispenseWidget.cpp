#include "DispenseWidget.h"
#include "ui_DispenseWidget.h"

DispenseWidget::DispenseWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_DispenseWidget)
{
    ui->setupUi(this);
}

DispenseWidget::~DispenseWidget()
{
}

QVariantMap DispenseWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_DISPENSE_PUMP_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(SEQUENCE_DISPENSE_PUMP_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DispenseWidget::setConfigParameters(const QVariantMap& parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(SEQUENCE_DISPENSE_PUMP_CHANNEL)].toInt());
    ui->spinVolume->setValue(parameter[QString::number(SEQUENCE_DISPENSE_PUMP_VOLUME)].toInt());
}
