#include "DDispenseWidget.h"
#include "ui_DDispenseWidget.h"

#include "Analysis/ConfigInformation.h"

DDispenseWidget::DDispenseWidget(QWidget *parent) :
                                        CommonSequenceWidget(parent),
                                        ui(new Ui_DDispenseWidget)
{
    ui->setupUi(this);
    this->init();
}

DDispenseWidget::~DDispenseWidget()
{

}
void DDispenseWidget::init()
{
    ui->comboOption->insertItems(0, QStringList()
                                     << QApplication::translate("DDispenseWidget", "None", Q_NULLPTR)
                                     << QApplication::translate("DDispenseWidget", "Single", Q_NULLPTR)
                                     << QApplication::translate("DDispenseWidget", "Dual", Q_NULLPTR)
                                     << QApplication::translate("DDispenseWidget", "RDual", Q_NULLPTR)
                                     << QApplication::translate("DDispenseWidget", "ADual", Q_NULLPTR)
                                 );
    ui->comboOption->setItemData(0, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_NONE);
    ui->comboOption->setItemData(1, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_SINGLE);
    ui->comboOption->setItemData(2, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_DUAL);
    ui->comboOption->setItemData(3, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL);
    ui->comboOption->setItemData(4, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_A_DUAL);
}

QVariantMap DDispenseWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_OPTION)] = ui->comboOption->currentData().toInt();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_VOLUME)] = ui->spinVolume->value();

    return dataMap;
}

void DDispenseWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_CHANNEL)].toInt());
    ui->comboOption->setCurrentIndex(this->findIndex(parameter[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_OPTION)].toInt()));
    ui->spinVolume->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_DISPENSE_VOLUME)].toInt());
}

int DDispenseWidget::findIndex(int data)
{
    switch (data)
    {
    case ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL:
        return 3;

    default:
        return data;
    }
}
