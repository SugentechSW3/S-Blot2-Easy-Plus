#include "RollBackWidget.h"
#include "ui_RollBackWidget.h"

RollBackWidget::RollBackWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_RollBackWidget)
{
    ui->setupUi(this);
}

RollBackWidget::~RollBackWidget()
{
}

QVariantMap RollBackWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_ROLL_BACK_PUMP_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(SEQUENCE_ROLL_BACK_TIME)] = ui->spinTime->value();

    return dataMap;
}

void RollBackWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_PUMP_CHANNEL)].toInt());
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_ROLL_BACK_TIME)].toInt());
}
