#include "PCLineWidget.h"
#include "ui_PCLineWidget.h"

PCLineWidget::PCLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_PCLineWidget)
{
    ui->setupUi(this);
    this->setGroupBoxNameColor();
}

PCLineWidget::~PCLineWidget()
{
}

void PCLineWidget::setGroupBoxNameColor()
{
    QColor qcolor("orange");
    ui->groupBox->setStyleSheet(QString("QGroupBox::title  {color: %1;}").arg(qcolor.name()));
}

int PCLineWidget::getXStart()
{
    return ui->spinXStart->value();
}

void PCLineWidget::setXStart(int value)
{
    ui->spinXStart->setValue(value);
}

int PCLineWidget::getXRange()
{
    return ui->spinXRange->value();
}

void PCLineWidget::setXRange(int value)
{
    ui->spinXRange->setValue(value);
}

int PCLineWidget::getPCThres()
{
   return ui->spinPCThres->value();
}

void PCLineWidget::setPCThres(int value)
{
    ui->spinPCThres->setValue(value);
}

int PCLineWidget::getHousingThres()
{
    return ui->spinHousingThres->value();
}

void PCLineWidget::setHousingThres(int value)
{
    ui->spinHousingThres->setValue(value);
}
