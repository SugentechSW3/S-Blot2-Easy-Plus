#include "PrimeWidget.h"
#include "ui_PrimeWidget.h"

PrimeWidget::PrimeWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_PrimeWidget)
{
    ui->setupUi(this);
}

PrimeWidget::~PrimeWidget()
{
}

QVariantMap PrimeWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_PRIME_PUMP_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(SEQUENCE_PRIME_PUMP_TIME)] = ui->spinTime->value();

    return dataMap;
}

void PrimeWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(SEQUENCE_PRIME_PUMP_CHANNEL)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_PRIME_PUMP_TIME)].toInt());
}
