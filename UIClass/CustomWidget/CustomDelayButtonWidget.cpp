#include <QTimer>
#include <QEvent>
#include <QTime>
#include "CustomDelayButtonWidget.h"

struct CustomDelayButtonWidget::privateStruct
{
    const int DELAY_TIME = 3;
    int mRemainCount = 0;
    QTimer mTimer;
    QString mButtonText;
};

CustomDelayButtonWidget::CustomDelayButtonWidget(QWidget *parent) :
                                                                    CustomImageButtonWidget(parent),
                                                                    d(new privateStruct)
{
    this->setDelayTime(1000);

    connect(&d->mTimer, &QTimer::timeout, this, [this]
            {
                d->mRemainCount++;
                this->showRemainTime();

                if(d->mRemainCount <= d->DELAY_TIME)
                    return;

                d->mTimer.stop();
                d->mRemainCount = 0;

                this->setText(d->mButtonText);
                this->setEnabled(true);
            });
}

CustomDelayButtonWidget::~CustomDelayButtonWidget()
{

}

void CustomDelayButtonWidget::saveButtonText()
{
    d->mButtonText = this->text();
}

void CustomDelayButtonWidget::setDelayTime(int msDelayTime)
{
    d->mTimer.setInterval(msDelayTime);
}

void CustomDelayButtonWidget::setEnabledChange()
{
    d->mRemainCount = 0;
    d->mTimer.stop();
    d->mTimer.start();
}

void CustomDelayButtonWidget::setDisabled(bool)
{
    d->mTimer.stop();
    this->setText(d->mButtonText);
    CustomImageButtonWidget::setDisabled(true);
}

void CustomDelayButtonWidget::showRemainTime()
{
    QTime remainTime(0,0,0);
    auto etcTime = d->DELAY_TIME - d->mRemainCount;
    auto calcTime = remainTime.addSecs(etcTime);
    auto timeString = calcTime.toString("mm:ss");

    this->setText(timeString);
}
