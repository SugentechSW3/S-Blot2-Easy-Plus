#include "RollBack3Widget.h"
#include "ui_RollBack3Widget.h"

RollBack3Widget::RollBack3Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_RollBack3Widget)
{
    ui->setupUi(this);
}

RollBack3Widget::~RollBack3Widget()
{
}


QVariantMap RollBack3Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_3)] = ui->spinPumpChannel3->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_3_TIME)] = ui->spinTime->value();

    return dataMap;
}

void RollBack3Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_2)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_3_PUMP_CHANNEL_3)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_3_TIME)].toInt());
}
