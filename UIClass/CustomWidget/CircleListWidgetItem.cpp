#include "CircleListWidgetItem.h"
#include "ui_CircleListWidgetItem.h"

struct CircleListWidgetItem::privateStruct
{
    bool mIsPause = false;
};

CircleListWidgetItem::CircleListWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_CircleListWidgetItem),
    d(new privateStruct)
{
    ui->setupUi(this);
}

CircleListWidgetItem::~CircleListWidgetItem()
{
}

void CircleListWidgetItem::setLabelText(const QString& text)
{
    ui->labelText->setText(text);
}

QString CircleListWidgetItem::getLabelText() const
{
    return ui->labelText->text();
}

void CircleListWidgetItem::setMaximumValue(int maximum)
{
    ui->progressBar->setMaximum(maximum);
}

int CircleListWidgetItem::getMaximumValue() const
{
    return ui->progressBar->maximum();
}

void CircleListWidgetItem::setCircleMaximumValue(int maximum)
{
    ui->prograssCircleWidget->setMaximum(maximum);
}

int CircleListWidgetItem::getCircleMaximumValue() const
{
    return ui->prograssCircleWidget->maximum();
}

void CircleListWidgetItem::setValue(int value)
{
    if(value < ui->progressBar->maximum())
        ui->progressBar->setValue(value);
    else
        ui->progressBar->setValue(ui->progressBar->maximum());
}

int CircleListWidgetItem::getValue() const
{
    return ui->progressBar->value();
}

void CircleListWidgetItem::setPrograssFormat(const QString& text)
{
    ui->progressBar->setFormat(text);
}

QString CircleListWidgetItem::getPrograssFormat() const
{
    return ui->progressBar->format();
}

void CircleListWidgetItem::setCircleValue(int value)
{
    ui->prograssCircleWidget->setValue(value);
}

int CircleListWidgetItem::getCircleValue() const
{
    return ui->prograssCircleWidget->value();
}

void CircleListWidgetItem::hideCircle()
{
    ui->prograssCircleWidget->hide();
}

void CircleListWidgetItem::showCircle()
{
    ui->prograssCircleWidget->show();
}

void CircleListWidgetItem::startCircle()
{
    d->mIsPause = false;
    ui->prograssCircleWidget->startInfiniteAnimation();
    ui->prograssCircleWidget->show();
}

void CircleListWidgetItem::endCircle()
{
    d->mIsPause = false;
    ui->prograssCircleWidget->stopInfiniteAnimation();
}

void CircleListWidgetItem::pauseCircle()
{
    if(d->mIsPause == true)
        return;

    d->mIsPause = true;
    ui->prograssCircleWidget->pauseInfiniteAnimation();
}

bool CircleListWidgetItem::isPause() const
{
    return d->mIsPause;
}
