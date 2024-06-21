#include "PatientListWidgetStep2.h"
#include "ui_PatientListWidgetStep2.h"

#include "UIClass/CustomWidget/CheckBoxTableWidgetItem.h"
#include "UIClass/CustomWidget/ComboBoxTableWidgetItem.h"
#include "UIClass/CustomWidget/SpinTableWidgetItem.h"


#include "Analysis/ConfigInformation.h"

PatientListWidgetStep2::PatientListWidgetStep2(QWidget *parent) :
                                                                  QWidget(parent),
                                                                  ui(new Ui_PatientListWidgetStep2)
{
    ui->setupUi(this);
    this->initTableItem();
}

PatientListWidgetStep2::~PatientListWidgetStep2()
{
}

void PatientListWidgetStep2::initTableItem()
{
    auto config = ConfigInformation::getInstance();
    auto stripCount = config->getStripCount();

    ui->tablePatientInfomationWidget->setRowCount(stripCount);
    this->initTableWidgetItems();
}

void PatientListWidgetStep2::initTableWidgetItems()
{
    ui->tablePatientInfomationWidget->clearContents();
    auto config = ConfigInformation::getInstance();
    auto stripCount = config->getStripCount();

    for(int i = 0; i< stripCount; i ++)
    {
        auto id = new QTableWidgetItem;
        auto patientName = new QTableWidgetItem;
        auto panel = new ComboBoxTableWidgetItem(i, PATIENT_TABLE_COLUMN_PANEL, ui->tablePatientInfomationWidget);

        panel->setDisabled(true);
        panel->setComboBoxMinimumWidth(150);

        ui->tablePatientInfomationWidget->setItem(i, PATIENT_TABLE_COLUMN_ID, id);
        ui->tablePatientInfomationWidget->setItem(i, PATIENT_TABLE_COLUMN_PATIENT_NAME, patientName);
        ui->tablePatientInfomationWidget->setCellWidget(i, PATIENT_TABLE_COLUMN_PANEL, panel);
        ui->tablePatientInfomationWidget->setRowHeight(i,50);
    }

}

void PatientListWidgetStep2::setData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data)
{
    this->initTableWidgetItems();

    for(int i =0; i< data.count(); i++)
    {
        auto currentData = data[i];

        auto columnId = ui->tablePatientInfomationWidget->item(i, PATIENT_TABLE_COLUMN_ID);
        auto patientName = ui->tablePatientInfomationWidget->item(i, PATIENT_TABLE_COLUMN_PATIENT_NAME);
        auto comboBox = qobject_cast<ComboBoxTableWidgetItem*> (ui->tablePatientInfomationWidget->cellWidget(i, PATIENT_TABLE_COLUMN_PANEL));

        if(columnId == nullptr)
            Q_ASSERT("columnId TableWidget Item Is Null");

        if(patientName == nullptr)
            Q_ASSERT("patientName TableWidget Item Is Null");

        if(comboBox == nullptr)
            Q_ASSERT("ComboBox TableWidget Item Is Null");

        columnId->setText(currentData.patientID);
        patientName->setText(currentData.name);
        comboBox->addItem(currentData.panelName);
    }
}
