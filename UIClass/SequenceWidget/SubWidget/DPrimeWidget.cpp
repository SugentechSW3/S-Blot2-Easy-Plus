#include "DPrimeWidget.h"
#include "ui_DPrimeWidget.h"

#include "Analysis/ConfigInformation.h"

DPrimeWidget::DPrimeWidget(QWidget *parent) :
                                  CommonSequenceWidget(parent),
                                  ui(new Ui_DPrimeWidget)
{
    ui->setupUi(this);
    this->init();
}

DPrimeWidget::~DPrimeWidget()
{

}

void DPrimeWidget::init()
{
    ui->comboOption->insertItems(0, QStringList()
                                     << QApplication::translate("DPrimeWidget", "None", Q_NULLPTR)
                                     << QApplication::translate("DPrimeWidget", "Single", Q_NULLPTR)
                                     << QApplication::translate("DPrimeWidget", "Dual", Q_NULLPTR)
                                     << QApplication::translate("DPrimeWidget", "RDual", Q_NULLPTR)
                                     << QApplication::translate("DPrimeWidget", "ADual", Q_NULLPTR)
                                );
    ui->comboOption->setItemData(0, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_NONE);
    ui->comboOption->setItemData(1, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_SINGLE);
    ui->comboOption->setItemData(2, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_DUAL);
    ui->comboOption->setItemData(3, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL);
    ui->comboOption->setItemData(4, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_A_DUAL);
}

QVariantMap DPrimeWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_PRIME_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_OPTION)] = ui->comboOption->currentData().toInt();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DPrimeWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_PRIME_CHANNEL)].toInt());
    ui->comboOption->setCurrentIndex(this->findIndex(parameter[QString::number(ConfigInformation::SEQUENCE_D_OPTION)].toInt()));
    ui->spinVolume->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_VOLUME)].toInt());
}

int DPrimeWidget::findIndex(int data)
{

    switch (data)
    {
    case ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL:
        return 3;

    default:
        return data;
    }
}
