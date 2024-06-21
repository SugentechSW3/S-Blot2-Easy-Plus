#include "DRollBackWidget.h"
#include "ui_DRollBackWidget.h"

#include "Analysis/ConfigInformation.h"

DRollBackWidget::DRollBackWidget(QWidget *parent) :
                                        CommonSequenceWidget(parent),
                                        ui(new Ui_DRollBackWidget)
{
    ui->setupUi(this);
    this->init();
}

DRollBackWidget::~DRollBackWidget()
{
}

void DRollBackWidget::init()
{
    ui->comboOption->insertItems(0, QStringList()
                                     << QApplication::translate("DRollBackWidget", "None", Q_NULLPTR)
                                     << QApplication::translate("DRollBackWidget", "Single", Q_NULLPTR)
                                     << QApplication::translate("DRollBackWidget", "Dual", Q_NULLPTR)
                                     << QApplication::translate("DRollBackWidget", "RDual", Q_NULLPTR)
                                     << QApplication::translate("DRollBackWidget", "ADual", Q_NULLPTR)
                                                                                                                                                                                   );
    ui->comboOption->setItemData(0, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_NONE);
    ui->comboOption->setItemData(1, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_SINGLE);
    ui->comboOption->setItemData(2, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_DUAL);
    ui->comboOption->setItemData(3, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL);
    ui->comboOption->setItemData(4, ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_A_DUAL);
}

QVariantMap DRollBackWidget::getConfigParameters()
{
    QVariantMap dataMap;
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_CHANNEL)] = ui->spinPumpChannel->value();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_OPTION)] = ui->comboOption->currentData().toInt();
    dataMap[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_DURATION)] = ui->spinDuration->value();

    return dataMap;
}

void DRollBackWidget::setConfigParameters(const QVariantMap &parameter)
{
    ui->spinPumpChannel->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_CHANNEL)].toInt());
    ui->comboOption->setCurrentIndex(this->findIndex(parameter[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_OPTION)].toInt()));
    ui->spinDuration->setValue(parameter[QString::number(ConfigInformation::SEQUENCE_D_ROLL_BACK_DURATION)].toInt());
}

int DRollBackWidget::findIndex(int data)
{
    switch (data)
    {
    case ConfigInformation::SEMI_AUTO_PROCESS_COMMAND_D_R_DUAL:
        return 3;

    default:
        return data;
    }
}
