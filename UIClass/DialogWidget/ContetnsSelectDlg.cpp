#include "ContetnsSelectDlg.h"
#include "ui_ContetnsSelectDlg.h"

#include "Analysis/ConfigInformation.h"

ContetnsSelectDlg::ContetnsSelectDlg(QWidget *parent) :
                                                        CDialogParentWidget(parent),
                                                        ui(new Ui::ContetnsSelectDlg)
{
    ui->setupUi(this);
    this->initVariable();
    this->initSignalSlots();
    this->initStyleSheet();
}

ContetnsSelectDlg::~ContetnsSelectDlg()
{
}

void ContetnsSelectDlg::initVariable()
{
    ui->comboContents->addItems(ConfigInformation::getInstance()->getContentsList());
}

void ContetnsSelectDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
            (bool)
            {
                ConfigInformation::getInstance()->setCurrentContents(ui->comboContents->currentText());
                this->accept();
            });

    connect(ui->btnCancel, &QPushButton::clicked, this, [this]
            {
                this->reject();
            });
}

void ContetnsSelectDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}
