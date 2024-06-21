#include "Dispense2Widget.h"
#include "ui_Dispense2Widget.h"

Dispense2Widget::Dispense2Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_Dispense2Widget)
{
    ui->setupUi(this);
}

Dispense2Widget::~Dispense2Widget()
{
}

QVariantMap Dispense2Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_DISPENSE2_PUMP_CHANNEL_1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_DISPENSE2_PUMP_CHANNEL_2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_DISPENSE2_PUMP_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void Dispense2Widget::setConfigParameters(const QVariantMap& parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_DISPENSE2_PUMP_CHANNEL_1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_DISPENSE2_PUMP_CHANNEL_2)].toInt());
    ui->spinVolume->setValue(parameter[QString::number(SEQUENCE_DISPENSE2_PUMP_VOLUME)].toInt());
}
