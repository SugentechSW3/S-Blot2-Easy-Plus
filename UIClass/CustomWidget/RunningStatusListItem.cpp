#include <QTimer>
#include "RunningStatusListItem.h"
#include "ui_RunningStatusListItem.h"

struct RunningStatusListItem::privateStruct
{
    bool mIsActivated = false;
    bool mIsPause = false;
};

RunningStatusListItem::RunningStatusListItem(QWidget *parent) :
                                                                QWidget(parent),
                                                                ui(new Ui_RunningStatusListItem),
                                                                d(new privateStruct)
{
    ui->setupUi(this);
    this->setPrograssFormat("");

    connect(ui->progressBar, &QProgressBar::valueChanged, this, [this]
            (int value)
            {
                ui->labelPercent->setText(QString("%1%").arg(value));
            });
    this->hideRotation();
}

RunningStatusListItem::~RunningStatusListItem()
{

}

void RunningStatusListItem::setLabelText(const QString& text)
{
    ui->labelText->setText(text);
}

QString RunningStatusListItem::getLabelText() const
{
    return ui->labelText->text();
}

void RunningStatusListItem::setMaximumValue(int maximum)
{
    ui->progressBar->setMaximum(maximum);
}

int RunningStatusListItem::getMaximumValue() const
{
    return ui->progressBar->maximum();
}

void RunningStatusListItem::setValue(int value)
{
    if(value < ui->progressBar->maximum())
        ui->progressBar->setMovingValue(value);
    else
        ui->progressBar->setMovingValue(ui->progressBar->maximum());
}

int RunningStatusListItem::getValue() const
{
    return ui->progressBar->value();
}

void RunningStatusListItem::setPrograssFormat(const QString& text)
{
    ui->progressBar->setFormat(text);
}

QString RunningStatusListItem::getPrograssFormat() const
{
    return ui->progressBar->format();
}

void RunningStatusListItem::hideRotation()
{
    ui->circleWidget->hide();
    ui->dummyWidget->show();
}

void RunningStatusListItem::showRotation()
{
    ui->circleWidget->show();
    ui->dummyWidget->hide();
}

void RunningStatusListItem::startRotation()
{
    d->mIsActivated = true;
    d->mIsPause = false;
    this->showRotation();
    ui->circleWidget->startInfiniteAnimation();
}

void RunningStatusListItem::stopRotation()
{
    d->mIsActivated = false;
    this->showRotation();
    ui->circleWidget->stopInfiniteAnimation();
}

void RunningStatusListItem::pauseRotation()
{
    d->mIsPause = true;
    this->showRotation();
    ui->circleWidget->pauseInfiniteAnimation();
}

void RunningStatusListItem::completionStopRotation()
{
    d->mIsActivated = false;
    this->showRotation();
    ui->circleWidget->stopDirectAnimation();
}

void RunningStatusListItem::setStatus(int percent)
{
    this->setValue(percent);
}

void RunningStatusListItem::setPrograssBarColor(const QColor& color)
{
    ui->progressBar->setColor(color);
}

bool RunningStatusListItem::isActivated()
{
    return d->mIsActivated;
}

bool RunningStatusListItem::isPaused()
{
    return d->mIsPause;
}
