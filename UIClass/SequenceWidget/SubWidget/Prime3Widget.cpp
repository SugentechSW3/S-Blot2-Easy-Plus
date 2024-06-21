#include "Prime3Widget.h"
#include "ui_Prime3Widget.h"

Prime3Widget::Prime3Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_Prime3Widget)
{
    ui->setupUi(this);
}

Prime3Widget::~Prime3Widget()
{
}

QVariantMap Prime3Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_3)] = ui->spinPumpChannel3->value();
    dataMap[QString::number(SEQUENCE_PRIME3_PUMP_TIME)] = ui->spinTime->value();

    return dataMap;
}

void Prime3Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_2)].toInt());
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_PRIME3_PUMP_CHANNEL_3)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_PRIME3_PUMP_TIME)].toInt());
}
