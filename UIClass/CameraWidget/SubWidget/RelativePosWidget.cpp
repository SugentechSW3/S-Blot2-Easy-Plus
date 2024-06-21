#include <QVector>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "RelativePosWidget.h"
#include "ui_RelativePosWidget.h"

struct RelativePosWidget::privateStruct
{

};

RelativePosWidget::RelativePosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_RelativePosWidget),
    d(new privateStruct)
{
    ui->setupUi(this);
    ui->RelativePositionWidget->setGroupBoxName(tr("Relative Band Gap"));
    ui->RelativePositionWidget->setGroupBoxNameColor(Qt::green);

    connect(ui->RelativePositionWidget, &BandGapWidget::onChangedSpin, this, &RelativePosWidget::onChangedBoxCarSpin);
}

RelativePosWidget::~RelativePosWidget()
{
}

void RelativePosWidget::CreateEditBox(int count)
{
    ui->RelativePositionWidget->CreateEditBox(count);
}

void RelativePosWidget::reCreateEditBox(int count)
{
    ui->RelativePositionWidget->reCreateEditBox(count);
}

void RelativePosWidget::clearEditBox()
{
    ui->RelativePositionWidget->clearEditBox();
}

int RelativePosWidget::getEditBoxData(int index)
{
    return ui->RelativePositionWidget->getEditBoxData(index);
}

void RelativePosWidget::setEditBoxData(int index, int value)
{
    ui->RelativePositionWidget->setEditBoxData(index, value);
}

int RelativePosWidget::getWidgetCount()
{
    return ui->RelativePositionWidget->getWidgetCount();
}

void RelativePosWidget::setEnableSpin()
{
    ui->RelativePositionWidget->setEnableSpin();
}

void RelativePosWidget::setDisableSpin()
{
    ui->RelativePositionWidget->setDisableSpin();
}

void RelativePosWidget::setMaximumValue(int val)
{
    ui->RelativePositionWidget->setMaximumValue(val);
}

void RelativePosWidget::setMinimumValue(int val)
{
    ui->RelativePositionWidget->setMinimumValue(val);
}
