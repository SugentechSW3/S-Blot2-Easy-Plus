#include "DryWidget.h"
#include "ui_DryWidget.h"

DryWidget::DryWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_DryWidget)
{
    ui->setupUi(this);
}

DryWidget::~DryWidget()
{
}

QVariantMap DryWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_DRY_TIME)] = ui->spinDryTime->value();
    dataMap[QString::number(SEQUENCE_HEATING_TIME)] = ui->spinDryMaxTime->value();
    dataMap[QString::number(SEQUENCE_DRY_SPEED)] = ui->spinDrySpeed->value();
    return dataMap;
}

void DryWidget::setConfigParameters(const QVariantMap& parameter)
{
    ui->spinDryTime->setValue(parameter[QString::number(SEQUENCE_DRY_TIME)].toInt());
    ui->spinDrySpeed->setValue(parameter[QString::number(SEQUENCE_DRY_SPEED)].toInt());
    ui->spinDryMaxTime->setValue(parameter[QString::number(SEQUENCE_HEATING_TIME)].toInt());
}
