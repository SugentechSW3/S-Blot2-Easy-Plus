#include "AspirationWidget.h"
#include "ui_AspirationWidget.h"

#include "Analysis/ConfigInformation.h"

AspirationWidget::AspirationWidget(QWidget *parent) :
    CommonSequenceWidget(parent),
    ui(new Ui_AspirationWidget)
{
    ui->setupUi(this);
}

AspirationWidget::~AspirationWidget()
{
}


QVariantMap AspirationWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_TIME)] = ui->spinASPTime->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_COUNT)]= ui->spinASPCount->value();

    return dataMap;
}

void AspirationWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinASPTime->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_TIME)].toInt());
    ui->spinASPCount->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_ASPIRATION_COUNT)].toInt());
}
