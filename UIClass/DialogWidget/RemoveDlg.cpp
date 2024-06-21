#include "RemoveDlg.h"
#include "ui_RemoveDlg.h"

#include "Analysis/ConfigInformation.h"
#include "MessageShowDlg.h"
#include "QuestionDlg.h"

struct RemoveDlg::privateStruct
{
    GlobalDataStruct::QUERY_CONDITION_DATA mConditionData;
    REMOVE_DIALOG_SELECLECTION_MODE mMode = REMOVE_DIALOG_SELECLECTION_MODE_SELECTED_ROW;
};

RemoveDlg::RemoveDlg(QWidget *parent) :
                                        CDialogParentWidget(parent),
                                        ui(new Ui_RemoveDlg),
                                        d(new privateStruct)
{
    ui->setupUi(this);
    this->initData();
    this->initButton();

    ui->radioSelectedRows->setChecked(true);
    ui->widgetOptionGroup->setDisabled(true);
}

RemoveDlg::~RemoveDlg()
{
}

void RemoveDlg::initData()
{
    auto currentDate = QDate::currentDate();
    auto pastDate = currentDate.addYears(-1);

    ui->dateEditFrom->setDate(pastDate);
    ui->dateEditTo->setDate(currentDate);

    auto panelList = ConfigInformation::getInstance()->getPanelList();
    ui->comboBox->addItem("ALL");

    for(auto& itr : panelList)
        ui->comboBox->addItem(itr);
}

void RemoveDlg::initButton()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    connect(ui->btnOK, &QPushButton::clicked, this, &RemoveDlg::onClickOK);
    connect(ui->btnCANCEL, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->radioSelectedRows, &QPushButton::clicked, this, [this]
            {
                d->mMode = REMOVE_DIALOG_SELECLECTION_MODE_SELECTED_ROW;
                ui->widgetOptionGroup->setDisabled(true);
            });

    connect(ui->radioOption, &QPushButton::clicked, this, [this]
            {
                d->mMode = REMOVE_DIALOG_SELECLECTION_MODE_CONDITION;
                ui->widgetOptionGroup->setEnabled(true);
            });
}

RemoveDlg::REMOVE_DIALOG_SELECLECTION_MODE RemoveDlg::getSelectedMode()
{
    return d->mMode;
}

GlobalDataStruct::QUERY_CONDITION_DATA RemoveDlg::getData() const
{
    return d->mConditionData;
}

bool RemoveDlg::conditionProcess()
{
    GlobalDataStruct::QUERY_CONDITION_DATA data;

    if(ui->checkTestDate->isChecked() == true)
    {
        auto from = ui->dateEditFrom->date();
        auto to = ui->dateEditTo->date();

        if(from.daysTo(to) < 0)
        {
            MessageShowDlg* dlg = new MessageShowDlg(tr("The start date cannot be greater than the end date."),this);
            dlg->exec();
            return false;
        }

        data.fromDate = from;
        data.toDate = to;
    }

    if(ui->editPatientID->text().isEmpty() == false)
        data.patientID = ui->editPatientID->text();

    if(ui->editPatientName->text().isEmpty() == false)
        data.patientName = ui->editPatientName->text();

    if(ui->comboBox->currentText().isEmpty() == false)
        data.patientPanel = ui->comboBox->currentText();

    d->mConditionData = data;

    return true;
}

void RemoveDlg::onClickOK()
{
    QuestionDlg dlg(this);
    auto isSuccess = true;

    switch(d->mMode)
    {
        case REMOVE_DIALOG_SELECLECTION_MODE_SELECTED_ROW:
            dlg.setQuestionMessage(tr("Selected row data will be deleted Are you sure you want to proceed?"));
            break;

        case REMOVE_DIALOG_SELECLECTION_MODE_CONDITION:
        {
            auto isEmptyDate = (ui->checkTestDate->isChecked() == false);
            auto isEmptyPatientID = ui->editPatientID->text().isEmpty();
            auto isEmptyName = ui->editPatientName->text().isEmpty();
            auto isEmptyPanel = ui->comboBox->currentText() == "ALL";

            if(isEmptyDate && isEmptyPatientID && isEmptyName && isEmptyPanel)
                dlg.setQuestionMessage(tr("All data will be deleted Are you sure you want to proceed?"));
            else
                dlg.setQuestionMessage(tr("Data for the selected condition will be deleted. Are you sure you want to proceed?"));

            isSuccess = this->conditionProcess();
        }   break;
    }

    if(isSuccess == false)
        return;

    if(dlg.exec() == QDialog::Accepted)
        this->accept();
}
