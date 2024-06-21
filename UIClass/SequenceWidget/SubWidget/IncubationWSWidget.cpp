#include "IncubationWSWidget.h"
#include "ui_IncubationWSWidget.h"

IncubationWSWidget::IncubationWSWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_IncubationWSWidget)
{
    ui->setupUi(this);
}

IncubationWSWidget::~IncubationWSWidget()
{
}

QVariantMap IncubationWSWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_INCUBATION_WS_TIME)] = ui->spinTime->value();

    return dataMap;
}

void IncubationWSWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinTime->setValue(parameter[QString::number(SEQUENCE_INCUBATION_WS_TIME)].toInt());
}
