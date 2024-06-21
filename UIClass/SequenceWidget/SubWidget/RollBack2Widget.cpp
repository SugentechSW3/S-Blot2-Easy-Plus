#include "RollBack2Widget.h"
#include "ui_RollBack2Widget.h"

RollBack2Widget::RollBack2Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_RollBack2Widget)
{
    ui->setupUi(this);
}

RollBack2Widget::~RollBack2Widget()
{
}

QVariantMap RollBack2Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_2_TIME)] = ui->spinTime->value();

    return dataMap;
}

void RollBack2Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_2_PUMP_CHANNEL_2)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_2_TIME)].toInt());
}
