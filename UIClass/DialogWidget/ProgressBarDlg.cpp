#include <QTimer>
#include <QDesktopWidget>
#include "ProgressBarDlg.h"
#include "ui_ProgressBarDlg.h"

struct ProgressBarDlg::privateStruct
{
    QTimer mTimer;
    bool mIsUseTimer = true;
    double mTimeOutMSec = 0.0;
    double mCurrentProcess = 0.0;
};

ProgressBarDlg::ProgressBarDlg(QWidget *parent) :
    CDialogParentWidget(parent),
    ui(new Ui_ProgressBarDlg),
    d(new privateStruct)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    this->initSignalSlots();
    this->onDialogClose();
}

ProgressBarDlg::~ProgressBarDlg()
{
}

void ProgressBarDlg::start(double timeOutmSec)
{
    if(d->mIsUseTimer == false)
        return;

    ui->progressBar->setValue(0);
    d->mTimeOutMSec = timeOutmSec;
    d->mCurrentProcess = 0;
    d->mTimer.start(1000); // 1sec
    this->show();
    this->setFocus();
    this->onDialogOpen();
}

void ProgressBarDlg::stop()
{
    if(d->mTimer.isActive() == true)
        d->mTimer.stop();

    this->onDialogClose();
    this->hide();
}

void ProgressBarDlg::initSignalSlots()
{
    connect(&d->mTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

void ProgressBarDlg::setUseTimer(bool isUse)
{
    d->mIsUseTimer = isUse;
}

void ProgressBarDlg::setValue(double value)
{
    this->show();
    this->raise();

    ui->progressBar->setMovingValue(value);

    if(ui->progressBar->maximum() <= value)
        this->hide();
}

void ProgressBarDlg::setValue(double value , QString& text)
{
    this->setValue(value);
    this->setText(text);
}

void ProgressBarDlg::setText(QString text)
{
    ui->labelProgress->setText(text);
}

void ProgressBarDlg::onTimeOut()
{
    d->mCurrentProcess +=  (d->mTimeOutMSec / 1000);
    this->setValue(d->mCurrentProcess);

    auto progressValue = d->mCurrentProcess;

    if(ui->progressBar->maximum() <= progressValue)
    {
        emit onCompleted();
        this->stop();
    }
}
