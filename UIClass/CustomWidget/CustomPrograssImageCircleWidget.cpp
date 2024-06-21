#include <QStyleOption>
#include <QPainter>

#include "CustomPrograssImageCircleWidget.h"
#include "ui_CustomPrograssImageCircleWidget.h"

CustomPrograssImageCircleWidget::CustomPrograssImageCircleWidget(QWidget *parent) :
                                                                                    QWidget(parent),
                                                                                    ui(new Ui_CustomPrograssImageCircleWidget)
{
    ui->setupUi(this);

    connect(ui->widgetInner, &ImageCircleWidget::onValueChanged, this, [this]
            (int value)
            {
                ui->labelPercent->setText(QString::number(value));
            });
}

CustomPrograssImageCircleWidget::~CustomPrograssImageCircleWidget()
{
}

int CustomPrograssImageCircleWidget::getValue() const
{
    return ui->widgetInner->getValue();
}

void CustomPrograssImageCircleWidget::setValue(int value)
{
    ui->widgetInner->setMovingValue(value);
}

void CustomPrograssImageCircleWidget::setETATime(const QString& timeString)
{
    ui->labelTime->setText(timeString);
}
