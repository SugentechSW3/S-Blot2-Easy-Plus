#include "ContetnsSelectDlg.h"
#include "ui_ContetnsSelectDlg.h"

#include "Analysis/ConfigInformation.h"

#include "Network/SerialCtrl.h"

#include "UIClass/DialogWidget/CirclePrograssDialog.h"


struct ContetnsSelectDlg::privateStruct
{
    QScopedPointer<CirclePrograssDialog> mPrograssCircleDlg;
    SerialCtrl* mSerialInstance = nullptr;
};

ContetnsSelectDlg::ContetnsSelectDlg(QWidget *parent) :
                                                        CDialogParentWidget(parent),
                                                        ui(new Ui::ContetnsSelectDlg),
                                                        d(new privateStruct)
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
    d->mSerialInstance = SerialCtrl::getInstance();
    d->mPrograssCircleDlg.reset(new CirclePrograssDialog(this));

    ui->comboContents->addItems(ConfigInformation::getInstance()->getContentsList());
    ui->comboContents->setCurrentText(ConfigInformation::getInstance()->getCurrentContents());
}

void ContetnsSelectDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
            (bool)
            {
                auto contents = ui->comboContents->currentText();

                this->setDisabled(true);

                ConfigInformation::getInstance()->setCurrentContents(contents);
                this->sequenceUpdate(contents);
            });

    connect(ui->btnCancel, &QPushButton::clicked, this, [this]
            {
                this->reject();
            });

    connect(d->mSerialInstance, &SerialCtrl::onSequenceSendFinished, this, &ContetnsSelectDlg::onFinishSeqeuence);
}

void ContetnsSelectDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void ContetnsSelectDlg::sequenceUpdate(const QString& contentsName)
{
    auto config = ConfigInformation::getInstance();

    auto sequenceData = config->getSequenceList(contentsName);
    auto sequenceProtocols = config->makeSequenceProtocol(sequenceData);
    d->mSerialInstance->inputSequence(sequenceProtocols);
    d->mPrograssCircleDlg->start();
}

void ContetnsSelectDlg::disableButton()
{
    this->setDisabled(true);
}

void ContetnsSelectDlg::onFinishSeqeuence()
{
    d->mPrograssCircleDlg->stop();
    this->accept();
}
