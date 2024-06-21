#include <QTimer>
#include "StatusDialog.h"
#include "ui_StatusDialog.h"

struct StatusDialog::privateStruct
{
    QString mMessage;
    QTimer mTimer;
};


StatusDialog::StatusDialog(QWidget *parent) :
                                              CDialogParentWidget(parent),
                                              ui(new Ui_StatusDialog),
                                              d(new privateStruct)
{
    ui->setupUi(this);
    d->mMessage = tr("Wait for a few minutes");
    connect(&d->mTimer, &QTimer::timeout, this, &StatusDialog::setWaitingMessage);
    this->stopWaiting();
}

StatusDialog::~StatusDialog()
{
}

void StatusDialog::setWaitingMessage()
{
    this->raise();

    static char dotCount = 0;

    QString msg = d->mMessage;

    for(int i = 0 ; i < dotCount; i++ )
        msg.append(".");

    dotCount++;

    if(dotCount == 4)
        dotCount = 0;

    ui->labelMessage->setText(msg);
}

void StatusDialog::setMessage(QString msg)
{
    d->mMessage = msg;
}

void StatusDialog::setStatusMessage(QString msg)
{
    ui->labelStatus->setText(msg);
}

void StatusDialog::startWaiting()
{
    d->mTimer.start(1000);

    this->show();
    this->onDialogOpen();
}

void StatusDialog::stopWaiting()
{
    d->mTimer.stop();
    this->hide();
    this->onDialogClose();
}

bool StatusDialog::isActivatedTimer()
{
    return d->mTimer.isActive();
}

void StatusDialog::startWaitingRefreshApplication()
{
    QString msg(tr("Wait for a few minutes"));
    ui->labelMessage->setText(msg);
    this->show();
    this->onDialogOpen();
    QApplication::processEvents();
}
