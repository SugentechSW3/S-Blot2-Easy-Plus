#include "ReagentStripWidget.h"
#include "ui_ReagentStripWidget.h"

#include "Common/CLogWriter.h"

USING_SUGENTECH

struct ReagentStripWidget::privateStruct
{
    QVector<privateWidget> mStripWidget;
};

ReagentStripWidget::ReagentStripWidget(QWidget *parent) :
                                                          QWidget(parent),
                                                          ui(new Ui_ReagentStripWidget),
                                                          d(new privateStruct)
{
    ui->setupUi(this);
}

ReagentStripWidget::~ReagentStripWidget()
{
}

void ReagentStripWidget::updateStrip(const QVector<ConfigInformation::STRIP_COLOR> &data)
{
    this->clearStrip();

    for(int i = 0 ; i < data.count() ; i++)
        this->createStrip(data[i], i);
}

void ReagentStripWidget::createStrip(ConfigInformation::STRIP_COLOR color, int idx)
{
    auto widget = new QWidget(ui->widgetBackground);
    auto label  = new QLabel(ui->widgetBackground);

    if(d->mStripWidget.isEmpty() == true)
    {
        widget->setGeometry(108,34,8,132);
        label->setGeometry(106,175,16,13);
    }
    else
    {
        auto prevWidgetGeometry = d->mStripWidget.last().widget->geometry();
        auto prevLabelGeometry = d->mStripWidget.last().label->geometry();

        auto additinalWidgetGeometry = prevWidgetGeometry;
        auto additinalLabelGeometry = prevLabelGeometry;

        if(d->mStripWidget.count()%12 == 0)
        {
            auto additinalX = prevWidgetGeometry.x() + 46;
            additinalWidgetGeometry.setX(additinalX);
            additinalLabelGeometry.setX(additinalX - 2);
        }
        else
        {
            auto additinalX = prevWidgetGeometry.x() + 18;
            additinalWidgetGeometry.setX(additinalX);
            additinalLabelGeometry.setX(additinalX - 2);
        }

        additinalWidgetGeometry.setWidth(8);
        additinalLabelGeometry.setWidth(16);
        widget->setGeometry(additinalWidgetGeometry);
        label->setGeometry(additinalLabelGeometry);
    }

    QString colorStr;

    switch((int)color)
    {
    case ConfigInformation::STRIP_COLOR_PINK:
        colorStr.push_back("#FF45C4");
        break;
    case ConfigInformation::STRIP_COLOR_GRAY:
        colorStr.push_back("#6FA0D9");
        break;
    case ConfigInformation::STRIP_COLOR_GREEN:
        colorStr.push_back("#00DB9B");
        break;
    }

    widget->setStyleSheet(QString("background-color: %1; border-radius: 4px;").arg(colorStr));
    label->setStyleSheet("text-align: center; font: Regular 11px/56px Gulim;");

    label->setText(QString("%1").arg(idx + 1));

    privateWidget widgets;
    widgets.widget = widget;
    widgets.label = label;

    d->mStripWidget.push_back(widgets);
}

void ReagentStripWidget::clearStrip()
{
    for(auto itr : d->mStripWidget)
    {
        itr.widget->deleteLater();
        itr.label->deleteLater();
    }

    d->mStripWidget.clear();
}
