#include "AboutAndOpenSourceDlg.h"
#include "ui_AboutAndOpenSourceDlg.h"

AboutAndOpenSourceDlg::AboutAndOpenSourceDlg(QWidget *parent) :
                                                                CDialogParentWidget(parent),
                                                                ui(new Ui_AboutAndOpenSourceDlg)
{
    ui->setupUi(this);
    this->init();
}

AboutAndOpenSourceDlg::~AboutAndOpenSourceDlg()
{

}

void AboutAndOpenSourceDlg::init()
{
    this->initStyleSheet();
    this->initSignalSlots();
}

void AboutAndOpenSourceDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/About/sw_ok_normal.png", ":/Image/Image/About/sw_ok_over.png",
                                  ":/Image/Image/About/sw_ok_focus.png", ":/Image/Image/About/ok_disable.png");

}

void AboutAndOpenSourceDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, &AboutAndOpenSourceDlg::accept);
}
