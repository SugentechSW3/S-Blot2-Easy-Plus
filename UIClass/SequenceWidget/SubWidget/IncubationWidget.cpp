#include "IncubationWidget.h"
#include "ui_IncubationWidget.h"

IncubationWidget::IncubationWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_IncubationWidget)
{
    ui->setupUi(this);
}

IncubationWidget::~IncubationWidget()
{
}

QVariantMap IncubationWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_INCUBATION_TIME)] = ui->spinTime->value();

    return dataMap;
}

void IncubationWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_INCUBATION_TIME)].toInt());
}
