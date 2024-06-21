#include <QTimer>
#include "RealTimeDateTimeWidget.h"

struct RealTimeDateTimeWidget::privateStruct
{
    QTimer* mTimer = nullptr;
};


RealTimeDateTimeWidget::RealTimeDateTimeWidget(QWidget *parent) :
    QDateEdit(parent),
    d(new privateStruct)
{
    d->mTimer = new QTimer(this);

    connect(d->mTimer, &QTimer::timeout, this, &RealTimeDateTimeWidget::onSyncDateTime);

    this->startDateTime();
}

RealTimeDateTimeWidget::~RealTimeDateTimeWidget()
{

}

void RealTimeDateTimeWidget::startDateTime()
{
    d->mTimer->start(1000);
}

void RealTimeDateTimeWidget::stopDateTime()
{
    d->mTimer->stop();
}

void RealTimeDateTimeWidget::onSyncDateTime()
{
    this->setDateTime(QDateTime::currentDateTime());
}

