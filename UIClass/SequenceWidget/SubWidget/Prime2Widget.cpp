#include "Prime2Widget.h"
#include "ui_Prime2Widget.h"

Prime2Widget::Prime2Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_Prime2Widget)
{
    ui->setupUi(this);
}

Prime2Widget::~Prime2Widget()
{
}

QVariantMap Prime2Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_PRIME2_PUMP_CHANNEL_1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_PRIME2_PUMP_CHANNEL_2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_PRIME2_PUMP_TIME)] = ui->spinTime->value();

    return dataMap;
}

void Prime2Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_PRIME2_PUMP_CHANNEL_1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_PRIME2_PUMP_CHANNEL_2)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_PRIME2_PUMP_TIME)].toInt());
}
