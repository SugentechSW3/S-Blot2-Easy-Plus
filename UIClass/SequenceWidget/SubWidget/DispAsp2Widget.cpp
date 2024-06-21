#include "DispAsp2Widget.h"
#include "ui_DispAsp2Widget.h"

DispAsp2Widget::DispAsp2Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_DispAsp2Widget)
{
    ui->setupUi(this);
}

DispAsp2Widget::~DispAsp2Widget()
{
}

QVariantMap DispAsp2Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_DISP_ASP_2_PUMP1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(SEQUENCE_DISP_ASP_2_PUMP2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(SEQUENCE_DISP_ASP_2_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DispAsp2Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(SEQUENCE_DISP_ASP_2_PUMP1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(SEQUENCE_DISP_ASP_2_PUMP2)].toInt());
    ui->spinVolume->setValue(parameter[QString::number(SEQUENCE_DISP_ASP_2_VOLUME)].toInt());
}
