#include "QuestionDlg.h"
#include "ui_QuestionDlg.h"

QuestionDlg::QuestionDlg(QWidget *parent) :
                                            CDialogParentWidget(parent),
                                            ui(new Ui_QuestionDlg)
{
    ui->setupUi(this);
    this->initButton();
    this->onDialogClose();
}

QuestionDlg::~QuestionDlg()
{
}

void QuestionDlg::initButton()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                  ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    connect(ui->btnOK, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->btnCANCEL, &QPushButton::clicked, this, &QDialog::reject);

    ui->btnOK->setFocus();
}

void QuestionDlg::setQuestionMessage(const QString& message)
{
    ui->label->setText(message);
}
