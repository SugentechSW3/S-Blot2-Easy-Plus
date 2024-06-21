#include "DispAsp3Widget.h"
#include "ui_DispAsp3Widget.h"

#include "Analysis/ConfigInformation.h"

DispAsp3Widget::DispAsp3Widget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_DispAsp3Widget)
{
    ui->setupUi(this);
}

DispAsp3Widget::~DispAsp3Widget()
{
}

QVariantMap DispAsp3Widget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP1)] = ui->spinPumpChannel1->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP2)] = ui->spinPumpChannel2->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP3)] = ui->spinPumpChannel3->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DispAsp3Widget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel1->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP1)].toInt());
    ui->spinPumpChannel2->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP2)].toInt());
    ui->spinPumpChannel3->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_PUMP3)].toInt());
    ui->spinVolume->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_DISP_ASP_3_VOLUME)].toInt());
}
