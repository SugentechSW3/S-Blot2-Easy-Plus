#include <QDate>
#include "SearchDialog.h"
#include "ui_SearchDialog.h"

#include "Analysis/ConfigInformation.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"

struct SearchDialog::privateStruct
{
    GlobalDataStruct::QUERY_CONDITION_DATA data;
};


SearchDialog::SearchDialog(QWidget *parent) :
                                              CDialogParentWidget(parent),
                                              ui(new Ui_SearchDialog),
                                              d(new privateStruct)
{
    ui->setupUi(this);
    this->initVariable();
    this->initSignalSlots();
}

SearchDialog::~SearchDialog()
{
}

void SearchDialog::initVariable()
{
    auto currentDate = QDate::currentDate();
    auto pastDate = currentDate.addYears(-1);
    ui->dateEditFrom->setDate(pastDate);
    ui->dateEditTo->setDate(currentDate);

    auto panelList = ConfigInformation::getInstance()->getPanelList();
    ui->comboBox->addItem("ALL");

    for(auto& itr: panelList)
        ui->comboBox->addItem(itr);

    ui->btnSearch->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                       ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void SearchDialog::initSignalSlots()
{
    connect(ui->btnSearch, SIGNAL(clicked()), SLOT(onSearch()));
    connect(ui->btnCancel, SIGNAL(clicked()), SLOT(reject()));
}

GlobalDataStruct::QUERY_CONDITION_DATA SearchDialog::getData() const
{
    return d->data;
}

void SearchDialog::onSearch()
{
    GlobalDataStruct::QUERY_CONDITION_DATA data;

    if(ui->checkTestDate->isChecked() == true)
    {
        auto from = ui->dateEditFrom->date();
        auto to = ui->dateEditTo->date();

        if(from.daysTo(to) < 0)
        {
            MessageShowDlg* dlg = new MessageShowDlg(tr("The start date cannot be greater than the end date."),this);
            dlg->open();
            return;
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

    d->data = data;
    this->accept();
}

//ModelComplater = new QCompleter(this or string list)
//set model qdirmodel ();
//set complater
