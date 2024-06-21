#include "DispAsp1Widget.h"
#include "ui_DispAsp1Widget.h"

DispAsp1Widget::DispAsp1Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_DispAsp1Widget)
{
    ui->setupUi(this);
}

DispAsp1Widget::~DispAsp1Widget()
{
}

QVariantMap DispAsp1Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(SEQUENCE_DISP_ASP_1_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(SEQUENCE_DISP_ASP_1_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DispAsp1Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(SEQUENCE_DISP_ASP_1_CHANNEL)].toInt());
    ui->spinVolume->setValue(parameter[QString::number(SEQUENCE_DISP_ASP_1_VOLUME)].toInt());
}
